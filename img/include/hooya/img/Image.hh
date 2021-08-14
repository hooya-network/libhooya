#pragma once

#include <filesystem>
#include "hooya/img/Exception.hh"

namespace cv { class Mat; }
namespace hooya::img {
class Image {
public:
	Image();

	/**
	 * Read image data from a file path
	 * \param f File on disk
	 */
	void FromFile(std::filesystem::path f);

	/**
	 * Loaded image width
	 * \return Width
	 */
	unsigned Width();

	/**
	 * Loaded image height
	 * \return Height
	 */
	unsigned Height();

	/**
	 * Fit center of image into a square bounded by the largest dimension
	 * \return Length of any side
	 */
	unsigned CropSquare();

#ifdef USE_OPENCV_HIGHGUI
	/**
	 * Quick render in a window for debugging
	 */
	void Show();
#endif
private:

	/**
	 * Represented image data
	 */
	std::shared_ptr<::cv::Mat> mat;
}; }
