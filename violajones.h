#ifndef VIOLAJONES_H_
#define VIOLAJONES_H_
#endif

using namespace cv;

std::vector<Rect> ViolaJonesDetection(Mat image, Mat &retImage, char* cascadeFile, int method);
std::vector<Rect> detectAndDisplay(Mat frame, char* cascadeFile, int method);