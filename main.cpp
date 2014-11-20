// include statements
#ifndef IOSTREAM_H_
#define IOSTREAM_H_
#include <iostream>
#endif

#ifndef STDIO_H_
#define STDIO_H_
#include <stdio.h>
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
// Arguements: Image Path
//             .XML filename for raw image
//             .XML filename for mag image

#define H_THRESHOLD 10
#define M_THRESHOLD 20
#define MIN_RAD 20
#define MAX_RAD 100

#define HOUGH_VAL 100


int main(int argc, char* argv[])
{
	// Parse input commands
	if (argc != NUM_CMD_LINE_INPUTS+1) {
		cout << "Not enough input arguments" << endl;
		return(0);
	}
	char ImageName[500];
	char ImageNumber[10];
	char* ImagePath = argv[1];
	char* RawCascadeFile = argv[2];
	char* MagCascadeFile = argv[3];

	// ----- Main functions ----- //
	// image import
	for (int i=0; i<=11; i++) {
		
		strcpy(ImageName, ImagePath);
		strcat(ImageName, "dart");
		sprintf(ImageNumber, "%d", i);
		strcat(ImageName, ImageNumber);
		strcat(ImageName, ".jpg");

		cout << "Image: " << ImageName << endl;

		Mat OriginalImage;
		OriginalImage = imread(ImageName, CV_LOAD_IMAGE_COLOR);

		Mat ResultsImage;
		ResultsImage = OriginalImage.clone();

		// grayscale image
		Mat GrayImage;
		cvtColor( OriginalImage, GrayImage, CV_BGR2GRAY );
		imshow("Original Image",OriginalImage);
		

/*		// Viola-Jones face detection method
		Mat RawVJImage;
		vector<Rect> RawVJResults;
		RawVJResults = ViolaJonesDetection(OriginalImage, RawVJImage, RawCascadeFile, 1);
		imshow("Raw Viola-Jones Results", RawVJImage);
*/
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

		// MedianImage
		Mat MedianImage;
		equalizeHist(HoughImage, HoughImage);
		medianBlur(HoughImage, MedianImage, 25);
		imshow("median",MedianImage);

		// Comparison for "likelihood" of location
		// - Now have two vector<Rect>s showing possible locations
		// - - RawVJResults
		// - - MagVJResults
		// - - HTResults

		// Declare vector for filtered results
		vector<Rect> FilteredMagVJ;
		int xVal;
		int yVal;

		// Compare MagVJResults to Hough Transform
		for (int j=0; j<=MagVJResults.size(); j++) {
			xVal = MagVJResults[j].x + MagVJResults[j].width/2;
			yVal = MagVJResults[j].y + MagVJResults[j].height/2;

			if (xVal > HoughImage.cols)
				xVal = HoughImage.cols;

			if (yVal > HoughImage.rows)
				yVal = HoughImage.rows;


			// compare to hough transform
			if (MedianImage.at<uchar>(xVal,yVal) > HOUGH_VAL) {
				FilteredMagVJ.push_back(MagVJResults[j]);
			}
		}

		cout << "Found: " << FilteredMagVJ.size() << endl;


		// highlight on image
		for (int j=0; j<FilteredMagVJ.size(); j++) {
			//cout << "making rectangle" << endl;
			rectangle(ResultsImage, Point(FilteredMagVJ[j].x, FilteredMagVJ[j].y), Point(FilteredMagVJ[j].x + FilteredMagVJ[j].width, FilteredMagVJ[j].y + FilteredMagVJ[j].height), Scalar( 0, 255, 0 ), 2);
		}

		imshow("Results",ResultsImage);

		waitKey(0);
	}


	
	return(0);
}