#include <converter.h>

QImage util::openCVtoQtImage(const cv::Mat& inputImage) {
	auto type = inputImage.type();
	if (type == CV_8UC4) {
		return QImage(inputImage.data, inputImage.cols, inputImage.rows, inputImage.step, QImage::Format_RGB32);
	} else if (type == CV_8UC3) {
		return QImage(inputImage.data, inputImage.cols, inputImage.rows, inputImage.step, QImage::Format_RGB888).rgbSwapped();
	} else {
		throw std::runtime_error("Unsupported image type!");
	}
}
