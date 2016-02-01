#pragma once

#include <QtGui/QImage>
#include <QtGui/QPixmap>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>

namespace util {

QImage openCVtoQtImage(const cv::Mat& inputImage);

}
