#ifndef SOBEL_H_
#define SOBEL_H_

using namespace cv;

void gradientX(Mat image, Mat &retImage);
void gradientY(Mat image, Mat &retImage);

void magnitude(Mat gradX, Mat gradY, int Threshold, Mat &retImage);
void angle(Mat gradX, Mat gradY, Mat &retImage);

#endif