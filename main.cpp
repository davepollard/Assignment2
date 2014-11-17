// include statements
#ifndef IOSTREAM_H_
#define IOSTREAM_H_
#include <iostream>
#endif

#ifndef OPENCV2_H_
#define OPENCV2_H_
#include <opencv2/opencv.hpp>
#endif

#ifndef HOUGH_H_
#include "hough.h"
#endif

#ifndef VIOLAJONES_H_
#include "violajones.h"
#endif

// Namespaces
using namespace std;
using namespace cv;

// parameter definitions
#define NUM_CMD_LINE_INPUTS 1

#define H_THRESHOLD 10
#define M_THRESHOLD 5
#define MIN_RAD 5
#define MAX_RAD 10


int main(int argc, char* argv[])
{
	// Parse input commands
	if (argc != NUM_CMD_LINE_INPUTS+1) {
		cout << "Not enough input arguments" << endl;
		return(0);
	}

	// ----- Main functions ----- //
	// image import
	Mat OriginalImage;
	OriginalImage = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	imshow("Original Image",OriginalImage);
	

	// Viola-Jones face detection method
	Mat VJImage;
	vector<Rect> VJResults;
	VJResults = ViolaJonesDetection(OriginalImage, VJImage);
	imshow("Viola-Jones Results", VJImage);

	// Hough transform
	Mat HoughImage;
	vector<Rect> HTResults;
	HTResults = houghTransform(OriginalImage, H_THRESHOLD, M_THRESHOLD, MIN_RAD, MAX_RAD, HoughImage);
	imshow("Hough Transform", HoughImage);


	// Comparison for "likelihood" of location
	// - Now have two vector<Rect>s showing possible locations
	// - - VJResults
	// - - HTResults


	waitKey(0);
	return(0);
}