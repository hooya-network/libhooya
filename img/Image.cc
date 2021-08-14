#include "hooya/img/Image.hh"

/* Implementation */
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/mat.hpp>

#ifdef USE_OPENCV_HIGHGUI
#include <opencv2/highgui.hpp>
#endif

namespace hooya::img {
Image::Image() :
mat(std::make_shared<::cv::Mat>())
{ }

void Image::FromFile(std::filesystem::path f) {
	mat = std::make_shared<::cv::Mat>(::cv::imread(f));
	if (mat->empty())
		throw(exception::FileRead("File not found"));
}

unsigned Image::CropSquare() {
	unsigned dim, offX = 0, offY = 0;

	if (Width() > Height()) {
		/* Keep height the same */
		dim = Height();

		/* Trim width to center */
		offX = dim >> 1;
	} else if (Height() > Width()) {
		/* Keep width the same */
		dim = Width();

		/* Trim height to center */
		offY = dim >> 1;
	} else {
		/* Nothing to do - already square */
		return Height();
	}

	/* Crop image to area of interest */
	::cv::Rect roi(offX, offY, dim, dim);
	mat = std::make_shared<::cv::Mat>((*mat)(roi));

	return Width();
}

unsigned Image::Width() {
	return mat->size().width;
}

unsigned Image::Height() {
	return mat->size().height;
}

unsigned Image::Resize(unsigned maxdim) {
	::cv::Mat resized;
	::cv::Size dsize;
	::cv::InterpolationFlags flag;
	unsigned clampHeight = 0, clampWidth = 0;

	/* Calculate new dimensions */
	if (Height() > Width()) {
		clampHeight = maxdim;
		clampWidth = (clampHeight / (double)Height()) * Width();
	} else if (Height() < Width()) {
		clampWidth = maxdim;
		clampHeight = (clampWidth / (double)Width()) * Height();
	} else {
		clampWidth = maxdim;
		clampHeight = maxdim;
	}

	if (Height() < maxdim && Width() < maxdim)
		flag = ::cv::INTER_AREA;
	else
		/* TODO
		 * Expose choice between speed vs. quality in the downscale case
		 */
		flag = ::cv::INTER_CUBIC;

	/* Resize according to clamp calculation */
	::cv::resize(*mat, resized, ::cv::Size(clampWidth, clampHeight), 0, 0, flag);
	mat = std::make_shared<::cv::Mat>(resized);

	return clampWidth == maxdim ? clampHeight : clampWidth;
}

#ifdef USE_OPENCV_HIGHGUI
void Image::Show() {
	::cv::imshow("Image Show", *mat);
	::cv::waitKey(0);
}
#endif

}
