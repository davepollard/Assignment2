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
#define NUM_CMD_LINE_INPUTS 3
// Arguements: Image filename
//             .XML filename for raw image
//             .XML filename for mag image

#define H_THRESHOLD 10
#define M_THRESHOLD 20
#define MIN_RAD 20
#define MAX_RAD 100


int main(int argc, char* argv[])
{
	// Parse input commands
	if (argc != NUM_CMD_LINE_INPUTS+1) {
		cout << "Not enough input arguments" << endl;
		return(0);
	}

	char* RawCascadeFile = argv[2];
	char* MagCascadeFile = argv[3];

	// ----- Main functions ----- //
	// image import
	Mat OriginalImage;
	OriginalImage = imread(argv[1], CV_LOAD_IMAGE_COLOR);

	// grayscale image
	Mat GrayImage;
	cvtColor( OriginalImage, GrayImage, CV_BGR2GRAY );
	imshow("Original Image",OriginalImage);
	

	// Viola-Jones face detection method
	Mat RawVJImage;
	vector<Rect> RawVJResults;
	RawVJResults = ViolaJonesDetection(OriginalImage, RawVJImage, RawCascadeFile, 1);
	imshow("Raw Viola-Jones Results", RawVJImage);

	// Viola-Jones method on magnitude image
	Mat MagVJImage;
	vector<Rect> MagVJResults;
	MagVJResults = ViolaJonesDetection(OriginalImage, MagVJImage, MagCascadeFile, 2);
	imshow("Mag Viola-Jones Results", MagVJImage);
	cout << "Number of MagVJ Results found: " << MagVJResults.size() << endl;

	// Hough transform
	Mat HoughImage;
	vector<Rect> HTResults;
	HTResults = houghTransform(GrayImage, H_THRESHOLD, M_THRESHOLD, MIN_RAD, MAX_RAD, HoughImage);
	imshow("Hough Transform", HoughImage);


	// Comparison for "likelihood" of location
	// - Now have two vector<Rect>s showing possible locations
	// - - VJResults
	// - - HTResults


	waitKey(0);
	return(0);
}