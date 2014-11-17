#ifndef IOSTREAM_H_
#define IOSTREAM_H_
#include <iostream>
#endif

#ifndef MATH_H_
#define MATH_H_
#include <math.h>
#endif

#ifndef OPENCV2_H_
#define OPENCV2_H_
#include <opencv2/opencv.hpp>
#endif

#include "sobel.h"

#ifndef PI
#define PI 3.14159
#endif

#ifndef CONV_H_
#include "conv.h"
#endif

using namespace cv;


// -------------------------------------- GRADIENT FUNCTIONS
// - NB: currently usinging Filter2D
// 			-  may need to rewrite and scale properly

void gradientX(Mat image, Mat &retImage) 
{
	// define df/dx kernel
	double m[3][3] = {{-1, 0, 1} , {-2, 0, 2} , {-1, 0, 1}};
	Mat kernel(3,3,CV_64F, m);

	// Filter image
	conv(image, kernel, retImage);
}

void gradientY(Mat image, Mat &retImage)
{
	// define df/dx kernel
	double m[3][3] = {{-1, -2, -1} , {0, 0, 0} , {1, 2, 1}};
	Mat kernel(3,3,CV_64F, m);

	// Filter image
	conv(image, kernel, retImage);
}

// --------------------------------------------- MAGNITUDE AND ANGLE
void magnitude(Mat gradX, Mat gradY, int Threshold, Mat &retImage) 
{
	//initialise output matrix
	retImage.create(gradX.size(), gradX.type());	

	int numRows = gradX.rows;
	int numCols = gradX.cols;

	int xVal;
	int yVal;

	double xSq;
	double ySq;
	double result;

	//take the magnitude of the x/y grads.
	for (int x=0; x<numRows; x++) 
	{
		for (int y=0; y<numCols; y++) 
		{
			xVal = (int) gradX.at<uchar>(x,y) - 127;
			yVal = (int) gradY.at<uchar>(x,y) - 127;

			xSq = (double)xVal * (double)xVal;
			ySq = (double)yVal * (double)yVal;

			result = sqrt(xSq+ySq);

			if (result > Threshold)
			{
				retImage.at<uchar>(x,y) = 255;
			} 
			else
			{
				retImage.at<uchar>(x,y) = 0;
			}
			//retImage.at<uchar>(x,y) = (uchar)result;
		}
	}

}


void angle(Mat gradX, Mat gradY, Mat &retImage) 
{
	//initialise output matrix
	retImage.create(gradX.size(), gradX.type());

	// perform magnitude operation on matrix
	int numRows = gradX.rows;
	int numCols = gradX.cols;

	uchar xVal;
	uchar yVal;

	double result;

	// scaling factor to scale from 0->2*pi in image to 0->255 uchar
	double scalingFactor;
	scalingFactor = 255.0/(2.0*PI);

	for (int x=0; x<numRows; x++) 
	{
		for (int y=0; y<numCols; y++) 
		{
			xVal = gradX.at<uchar>(x,y)-127;
			yVal = gradY.at<uchar>(x,y)-127;

			result = atan2((double)yVal,(double)xVal) * scalingFactor;

			retImage.at<uchar>(x,y) = (int)result;
		}
	}

}

