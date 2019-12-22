#include <memory>
#include <string>

#include "BezierCurve.h"


template<typename T>
static void process(const VSFrameRef* src, VSFrameRef* dst, const VSFormat* fi, const quadraticBezierCurveData* d, const VSAPI* vsapi)
{
	for (int plane = 0; plane < fi->numPlanes; plane++)
	{
        if (d->process[plane])
        {
            const T* srcp = reinterpret_cast<const T*>(vsapi->getReadPtr(src, plane));
            int src_stride = vsapi->getStride(src, plane) / sizeof(T);

            T* dstp = reinterpret_cast<T*>(vsapi->getWritePtr(dst, plane));
            int dst_stride = vsapi->getStride(dst, plane) / sizeof(T);

            int w = vsapi->getFrameWidth(src, plane);
            int h = vsapi->getFrameHeight(src, plane);

            for (int y = 0; y < h; y++)
            {
                for (int x = 0; x < w; x++)
                {
                    dstp[x] = d->lutTable[srcp[x]];
                }

                dstp += dst_stride;
                srcp += src_stride;
            }
        }
    }
}

template <typename T>
static void process(const VSFrameRef* src, VSFrameRef* dst, const VSFormat* fi, const cubicBezierCurveData* d, const VSAPI* vsapi)
{
	for (int plane = 0; plane < fi->numPlanes; plane++)
	{
        if (d->process[plane])
        {
            const T* srcp = reinterpret_cast<const T*>(vsapi->getReadPtr(src, plane));
            int src_stride = vsapi->getStride(src, plane) / sizeof(T);

            T* dstp = reinterpret_cast<T*>(vsapi->getWritePtr(dst, plane));
            int dst_stride = vsapi->getStride(dst, plane) / sizeof(T);

            int w = vsapi->getFrameWidth(src, plane);
            int h = vsapi->getFrameHeight(src, plane);

            for (int y = 0; y < h; y++)
            {
                for (int x = 0; x < w; x++)
                {
                    dstp[x] = d->lutTable[srcp[x]];
                }

                dstp += dst_stride;
                srcp += src_stride;
            }
        }
    }
}

static void VS_CC quadraticBezierCurveInit(VSMap* in, VSMap* out, void** instanceData, VSNode* node, VSCore* core, const VSAPI* vsapi)
{
	quadraticBezierCurveData* d = static_cast<quadraticBezierCurveData *>(*instanceData);
	vsapi->setVideoInfo(d->vi, 1, node);
}

static const VSFrameRef* VS_CC quadraticBezierCurveGetFrame(int n, int activationReason, void** instanceData, void** frameData, VSFrameContext* frameCtx, VSCore* core, const VSAPI* vsapi)
{
	quadraticBezierCurveData* d = static_cast<quadraticBezierCurveData *>(*instanceData);

	if (activationReason == arInitial)
	{
		vsapi->requestFrameFilter(n, d->node, frameCtx);
	}
	else if (activationReason == arAllFramesReady)
	{
		const VSFrameRef* src = vsapi->getFrameFilter(n, d->node, frameCtx);
		const VSFormat* fi = d->vi->format;

		int width = d->vi->width;
		int height = d->vi->height;
		int bytesPerSample = d->vi->format->bytesPerSample;

		const VSFrameRef* frames[] = {
			d->process[0] ? nullptr : src,
			d->process[1] ? nullptr : src,
			d->process[2] ? nullptr : src,
		};

		const int processPlane[] = {0, 1, 2};

		VSFrameRef* dst = vsapi->newVideoFrame2(d->vi->format, width, height, frames, processPlane, src, core);

		if (bytesPerSample == 1)
		{
			process<uint8_t>(src, dst, fi, d, vsapi);
		}
		else if (bytesPerSample == 2)
		{
			process<uint16_t>(src, dst, fi, d, vsapi);
		}

		vsapi->freeFrame(src);
		return dst;
	}

	return nullptr;
}

static void VS_CC quadraticBezierCurveFree(void* instanceData, VSCore* core, const VSAPI* vsapi)
{
	quadraticBezierCurveData* d = static_cast<quadraticBezierCurveData *>(instanceData);
	vsapi->freeNode(d->node);
	delete d->lutTable;
	delete d;
}

void VS_CC quadraticBezierCurveCreate(const VSMap* in, VSMap* out, void* userData, VSCore* core, const VSAPI* vsapi)
{
	std::unique_ptr<quadraticBezierCurveData> d(new quadraticBezierCurveData);
	d->lutTable = nullptr;
	int err;

	d->node = vsapi->propGetNode(in, "clip", 0, nullptr);
	d->vi = vsapi->getVideoInfo(d->node);

	int bitsPerSample = d->vi->format->bitsPerSample;
	int scale = ((1 << bitsPerSample) - 1) / 255;

	// Set default values
	d->range = int64ToIntS(vsapi->propGetInt(in, "input_range", 0, &err));
	if (err)
		d->range = 0;  // Default range is PC range

	d->accur = vsapi->propGetFloat(in, "accur", 0, &err);
	if (err)
		d->accur = 0.01;

	d->begin = int64ToIntS(vsapi->propGetInt(in, "begin", 0, &err));
	if (err)
		d->begin = 0;

	d->end = int64ToIntS(vsapi->propGetInt(in, "end", 0, &err));
	if (err)
		d->end = 255 * scale;

	d->x1 = int64ToIntS(vsapi->propGetInt(in, "x1", 0, &err));
	if (err)
		d->x1 = 128 * scale;

	d->y1 = int64ToIntS(vsapi->propGetInt(in, "y1", 0, &err));
	if (err)
		d->y1 = 128 * scale;

	const int numPlanes = vsapi->propNumElements(in, "planes");

	for (int i = 0; i < 3; i++)
    {
	    d->process[i] = (numPlanes <= 0);
    }

	// Check parameters
	try
	{
		if (!isConstantFormat(d->vi) || d->vi->format->sampleType != stInteger ||
			d->vi->format->bitsPerSample < 8 || d->vi->format->bitsPerSample > 16)
		{
			throw std::string("only constant format of 8bit or 16bit integer input is supported");
		}

		if (d->range != 0 && d->range != 1)
		{
			throw std::string("range must be 0 - PC range or 1 - TV range");
		}

		if (d->accur < 0 || d->accur > 1)
		{
			throw std::string("accur must be between 0 and 1");
		}

		if (d->x1 < 0 || d->x1 > 255 * scale)
		{
			throw std::string("x1 must be between 0 and " + std::to_string(255 * scale));
		}

		for (int i = 0; i < numPlanes; i++)
        {
		    const int plane = int64ToIntS(vsapi->propGetInt(in, "planes", i, nullptr));

            if (plane < 0 || plane >= d->vi->format->numPlanes)
            {
                throw std::string("plane index invalid");
            }

		    if (d->process[plane])
            {
		        throw std::string("duplicate plane specific");
            }

		    d->process[plane] = true;
        }
	}
	catch (const std::string& error)
	{
		vsapi->setError(out, ("QuadraticBezierCurve: " + error).c_str());
		vsapi->freeNode(d->node);
		return;
	}

	// Allocate memory for lut table
	try
	{
		d->lutTable = new int[255 * scale + 1];
	}
	catch (const std::bad_alloc& e)
	{
		e;  // Supress the warning of C4101
		vsapi->setError(out, "QuadraticBezierCurve: failed to allocate memory for lut table");
		vsapi->freeNode(d->node);
		return;
	}

	QuadraticBezierCurve curve(d->range, d->accur, bitsPerSample, d->begin, d->end, d->x1, d->y1);
	for (int i = 0; i <= 255 * scale; i++)
	{
		double t = curve.bezierT(curve.normalize(i));
		double y = floor(curve.bezierY(t));

		d->lutTable[i] = static_cast<int>(y < 0 ? 0 : y > 255 * scale ? 255 * scale : y);
	}

	vsapi->createFilter(in, out, "Quadratic", quadraticBezierCurveInit, quadraticBezierCurveGetFrame,
		quadraticBezierCurveFree, fmParallel, 0, d.release(), core);
}

static void VS_CC cubicBezierCurveInit(VSMap* in, VSMap* out, void** instanceData, VSNode* node, VSCore* core, const VSAPI* vsapi)
{
	cubicBezierCurveData* d = static_cast<cubicBezierCurveData *>(*instanceData);
	vsapi->setVideoInfo(d->vi, 1, node);
}

static const VSFrameRef* VS_CC cubicBezierCurveGetFrame(int n, int activationReason, void** instanceData, void** frameData, VSFrameContext* frameCtx, VSCore* core, const VSAPI* vsapi)
{
	cubicBezierCurveData* d = static_cast<cubicBezierCurveData *>(*instanceData);

	if (activationReason == arInitial)
	{
		vsapi->requestFrameFilter(n, d->node, frameCtx);
	}
	else if (activationReason == arAllFramesReady)
	{
		const VSFrameRef* src = vsapi->getFrameFilter(n, d->node, frameCtx);
		const VSFormat* fi = d->vi->format;

		int width = d->vi->width;
		int height = d->vi->height;
		int bytesPerSample = d->vi->format->bytesPerSample;

		const VSFrameRef* frames[] = {
				d->process[0] ? nullptr : src,
				d->process[1] ? nullptr : src,
				d->process[2] ? nullptr : src,
		};

		const int processPlane[] = {0, 1, 2};

		VSFrameRef* dst = vsapi->newVideoFrame2(d->vi->format, width, height, frames, processPlane, src, core);

		if (bytesPerSample == 1)
		{
			process<uint8_t>(src, dst, fi, d, vsapi);
		}
		else if (bytesPerSample == 2)
		{
			process<uint16_t>(src, dst, fi, d, vsapi);
		}

		vsapi->freeFrame(src);
		return dst;
	}

	return nullptr;
}

static void VS_CC cubicBezierCurveFree(void* instanceData, VSCore* core, const VSAPI* vsapi)
{
	cubicBezierCurveData* d = static_cast<cubicBezierCurveData *>(instanceData);
	vsapi->freeNode(d->node);
	delete d->lutTable;
	delete d;
}

void VS_CC cubicBezierCurveCreate(const VSMap* in, VSMap* out, void* userData, VSCore* core, const VSAPI* vsapi)
{
	std::unique_ptr<cubicBezierCurveData> d(new cubicBezierCurveData);
	d->lutTable = nullptr;
	int err;

	d->node = vsapi->propGetNode(in, "clip", 0, nullptr);
	d->vi = vsapi->getVideoInfo(d->node);

	int bitsPerSample = d->vi->format->bitsPerSample;
	int scale = ((1 << bitsPerSample) - 1) / 255;

	// Set default values
	d->range = int64ToIntS(vsapi->propGetInt(in, "input_range", 0, &err));
	if (err)
		d->range = 0;  // Default range is PC range

	d->accur = vsapi->propGetFloat(in, "accur", 0, &err);
	if (err)
		d->accur = 0.01;

	d->begin = int64ToIntS(vsapi->propGetInt(in, "begin", 0, &err));
	if (err)
		d->begin = 0;

	d->end = int64ToIntS(vsapi->propGetInt(in, "end", 0, &err));
	if (err)
		d->end = 255 * scale;

	d->x1 = int64ToIntS(vsapi->propGetInt(in, "x1", 0, &err));
	if (err)
		d->x1 = 85 * scale;

	d->x2 = int64ToIntS(vsapi->propGetInt(in, "x2", 0, &err));
	if (err)
		d->x2 = 170 * scale;

	d->y1 = int64ToIntS(vsapi->propGetInt(in, "y1", 0, &err));
	if (err)
		d->y1 = 85 * scale;

	d->y2 = int64ToIntS(vsapi->propGetInt(in, "y2", 0, &err));
	if (err)
		d->y2 = 170 * scale;

	const int numPlanes = vsapi->propNumElements(in, "planes");

	for (int i = 0; i < 3; i++)
	{
		d->process[i] = (numPlanes <= 0);
	}

	// Check parameters
	try
	{
		if (!isConstantFormat(d->vi) || d->vi->format->sampleType != stInteger ||
			d->vi->format->bitsPerSample < 8 || d->vi->format->bitsPerSample > 16)
		{
			throw std::string("only constant format of 8bit or 16bit integer input is supported");
		}

		if (d->range != 0 && d->range != 1)
		{
			throw std::string("range must be 0 - PC range or 1 - TV range");
		}

		if (d->accur < 0 || d->accur > 1)
		{
			throw std::string("accur must be between 0 and 1");
		}

		if (d->x1 < 0 || d->x1 > 255 * scale)
		{
			throw std::string("x1 must be between 0 and " + std::to_string(255 * scale));
		}

		if (d->x2 < 0 || d->x2 > 255 * scale)
		{
			throw std::string("x2 must be between 0 and " + std::to_string(255 * scale));
		}

		/*if (d->x1 >= d->x2)
		{
			throw std::string("x1 must be smaller than x2");
		}*/

		for (int i = 0; i < numPlanes; i++)
		{
			const int plane = int64ToIntS(vsapi->propGetInt(in, "planes", i, nullptr));

			if (plane < 0 || plane >= d->vi->format->numPlanes)
			{
				throw std::string("plane index invalid");
			}

			if (d->process[plane])
			{
				throw std::string("duplicate plane specific");
			}

			d->process[plane] = true;
		}
	}
	catch (const std::string& error)
	{
		vsapi->setError(out, ("CubicBezierCurve: " + error).c_str());
		vsapi->freeNode(d->node);
		return;
	}

	// Allocate memory for lookup table
	try
	{
		d->lutTable = new int[255 * scale + 1];
	}
	catch (const std::bad_alloc& e)
	{
		e;  // Supress the warning of C4101
		vsapi->setError(out, "CubicBezierCurve: failed to allocate memory for lookup table");
		vsapi->freeNode(d->node);
		return;
	}

	CubicBezierCurve curve(d->range, d->accur, bitsPerSample, d->begin, d->end, d->x1, d->y1, d->x2, d->y2);
	for (int i = 0; i <= 255 * scale; i++)
	{
		double t = curve.bezierT(curve.normalize(i));
		double y = floor(curve.bezierY(t));

		d->lutTable[i] = static_cast<int>(y < 0 ? 0 : y > 255 * scale ? 255 * scale : y);
	}

	vsapi->createFilter(in, out, "Cubic", cubicBezierCurveInit, cubicBezierCurveGetFrame,
		cubicBezierCurveFree, fmParallel, 0, d.release(), core);
}

VS_EXTERNAL_API(void) VapourSynthPluginInit(VSConfigPlugin configFunc, VSRegisterFunction registerFunc, VSPlugin *plugin)
{
	configFunc(
		"com.kewenyu.bezier",
		"bezier",
		"vapoursynth bezier curve test",
		VAPOURSYNTH_API_VERSION, 1, plugin);

	registerFunc(
		"Cubic",
		"clip:clip;"
		"accur:float:opt;"
		"input_range:int:opt;"
		"begin:int:opt;"
		"end:int:opt;"
		"x1:int:opt;"
		"y1:int:opt;"
		"x2:int:opt;"
		"y2:int:opt;"
		"planes:int[]:opt;",
		cubicBezierCurveCreate, nullptr, plugin);

	registerFunc(
		"Quadratic",
		"clip:clip;"
		"accur:float:opt;"
		"input_range:int:opt;"
		"begin:int:opt;"
		"end:int:opt;"
		"x1:int:opt;"
		"y1:int:opt;"
		"planes:int[]:opt;",
		quadraticBezierCurveCreate, nullptr, plugin);
}
