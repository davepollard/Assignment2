#ifndef CONV_H_
#define CONV_H_

#include <opencv2/opencv.hpp>

using namespace cv;

void conv(cv::Mat &input, cv::Mat kernel, cv::Mat &blurredOutput);

#endif