#ifndef VIOLAJONES_H_
#define VIOLAJONES_H_
#endif

using namespace cv;

std::vector<Rect> ViolaJonesDetection(Mat image, Mat &retImage);
std::vector<Rect> detectAndDisplay(Mat frame);