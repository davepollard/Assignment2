#ifndef HOUGH_H_
#define HOUGH_H_
#endif

using namespace cv;

std::vector<Rect> houghTransform(Mat image, int h_threshold, int m_threshold, int min_rad, int max_rad, Mat &retImage);
void getHoughSpace(Mat image, int h_threshold, int m_threshold, int min_rad, int max_rad, Mat &retImage);
std::vector<Rect> analyseHoughSpace(Mat blurredHough);
int*** get_memory(int xlen, int ylen, int radlen);