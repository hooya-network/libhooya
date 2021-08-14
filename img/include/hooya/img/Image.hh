#pragma once

#include <vector>
#include <filesystem>
#include "hooya/img/Exception.hh"

namespace cv { class Mat; }
namespace hooya::img {
class Image {
public:
	Image();

	/**
	 * Byte stream as an image
	 * \param d byte stream
	 */
	void Load(const std::vector<uint8_t> &d);

	/**
	 * Read image data from a file path
	 * \param f File on disk
	 */
	void Load(std::filesystem::path f);

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
	 * Scale an image s/t the largest dimension is maxdim
	 * \return Length of the shorter side after being resized
	 */
	unsigned Resize(unsigned maxdim);

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
