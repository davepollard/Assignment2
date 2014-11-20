#ifndef IOSTREAM_H_
#define IOSTREAM_H_
#include <iostream>
#endif

#ifndef OPENCV2_H_
#define OPENCV2_H_
#include <opencv2/opencv.hpp>
#endif

#ifndef SOBEL_H_
#include "sobel.h"
#endif

#ifndef CONV_H_
#include "conv.h"
#endif

#include "hough.h"
#include <stdlib.h>

#ifndef STDIO_H_
#define STDIO_H_
#include <stdio.h>
#endif

#define PI 3.14159

using namespace cv;

std::vector<Rect> houghTransform(Mat image, int h_threshold, int m_threshold, int min_rad, int max_rad, Mat &retImage){

	// Calculate hough space transform
	Mat houghSpace;
	getHoughSpace(image, h_threshold, m_threshold, min_rad, max_rad, houghSpace);

	// apply Gaussian Blur
	int output_blur = 15;
	Mat blurredHough;
	GaussianBlur(houghSpace, blurredHough, Size(output_blur,output_blur), 1);

	// analyse hough space
	std::vector<Rect> locations;
	locations = analyseHoughSpace(blurredHough);

	retImage = blurredHough.clone();

	return(locations);

}

void getHoughSpace(Mat image, int h_threshold, int m_threshold, int min_rad, int max_rad, Mat &retImage)
{
	//Initalise retimage
	retImage.create(image.size(), CV_8U);
	// Get gradient in X
	Mat gradXImage;
	gradientX(image, gradXImage);

	// Get gradient in Y
	Mat gradYImage;
	gradientY(image, gradYImage);

	// Calculate magnitude of image
	Mat magImage;
	magnitude(gradXImage, gradYImage, m_threshold, magImage);

	/*
	imshow("gradX",gradXImage);
	imshow("gradY",gradYImage);
	imshow("magImage",magImage);    */

	// Calculate angle of image
	Mat angImage;
	angle(gradXImage, gradYImage, angImage);

	//Make a 3D array to store the hough space
	int*** space = get_memory(image.rows, image.cols, max_rad-min_rad);
	int x;
	int y;
	int hough_max = 0;
	double imAngle;
    double angle;

	if (space!=NULL)
	{
		//Do more stuff later
		//printf("Got memory OK\n");

		//Loop through image
		for (int i = 0; i<image.rows; i++)
		{
			for (int j = 0; j<image.cols; j++)
			{
				//Check magnitude of gradient here
				if (magImage.at<uchar>(i,j) == 255)
				{
					//Point found - add votes to hough space
					//printf("Calculating triplets\n");
					//Calc potential (x,y,rad) triplets
					for (int r=0;r<max_rad-min_rad;r++)
					{

						imAngle = (double) angImage.at<uchar>(i,j) * 2 * PI / 255.0;

						for (int s =-3;s<4;s++)
						{	
							angle = imAngle + s * PI/8.0;

							x = i + (int) (r+min_rad) * cos(angle);
							y = j + (int) (r+min_rad) * sin(angle);

							//If the point found is within the image, increment the hough space
							if (x > 0 && x < image.rows && y> 0 && y<image.cols)
							{
								//printf("Found a potential circle centre\n");
								space[x][y][r]++;

								if (space[x][y][r] > hough_max)
								{
									hough_max = space[x][y][r];
								}
							}
							
							x = i + (int) (-r-min_rad) * cos(angle);
							y = j + (int) (-r-min_rad) * sin(angle);

							//If the point found is within the image, increment the hough space
							if (x > 0 && x < image.rows && y> 0 && y<image.cols)
							{
								//printf("Found a potential circle centre\n");
								space[x][y][r]++;

								if (space[x][y][r] > hough_max)
								{
									hough_max = space[x][y][r];
								}
							
							}
							
						}
					}
				}
			}
		}

		//printf("Found hough space\n");
		//Hough space found - threshold on vals

		//Create an image to hold the circle centres
		Mat houghDisplayImage;
	    retImage.create(image.size(), CV_8U);

    	int sum;
    	//Loop through hough space
		for (int i=0; i<image.rows;i++)
		{
			for (int j=0;j<image.cols;j++)
			{
				//Set point to zero to begin with
				retImage.at<uchar>(i,j) = 0;

				//Loop through all radii to see if any values are above threshold
				for (int r=0;r<max_rad-min_rad;r++)
				{
					if (space[i][j][r] > h_threshold)
					{
						//printf("(%d, %d, %d)", i, j,r);
						retImage.at<uchar>(i,j) = space[i][j][r] * (int) (255.0 / (double) hough_max);
					}
				}
			}
		}
		//printf("Made hough image\n");

	}
}

std::vector<Rect> analyseHoughSpace(Mat blurredHough) {
	// analyses number of peaks and locations, returns approx size+location? Or maybe just location
	std::vector<Rect> locations;

	std::cout << "write me" << std::endl;

	return(locations);
}

int*** get_memory(int xlen, int ylen, int radlen)
{
	//This function returns a 3D array of size xlen*ylen*rlen initialised to zero.
	int*** tmp = NULL;
	tmp = (int***) malloc (xlen * sizeof(int **));

	if (tmp!=NULL)
	{
		for (int i=0;i<xlen;i++)
		{
			tmp[i] = (int**) malloc(ylen*sizeof(int*));

			for (int j=0;j<ylen;j++)
			{
				tmp[i][j] = (int*) malloc(radlen*sizeof(int));

				if (tmp[i][j] == NULL)
				{
					fprintf(stderr, "Problem with memory");
					exit(1);
				}

				for (int k=0;k<radlen;k++)
				{
					tmp[i][j][k] = 0;
				}
			}
		}
	}

	return tmp;
}


/////////////////////////////////////////////////
// OLD MAIN FUNCTION
/////////////////////////////////////////////////

/*
int main(int argc, char* argv[]) {

	//Token input checking
	if (argc != 8)
	{
		printf("Not enough input args\n");
		exit(1);
	}

	// import original image, convert to greyscale
	Mat in_image;
	Mat greyImage;
	Mat image;
	in_image = imread(argv[1],1);
	cvtColor(in_image, greyImage, CV_BGR2GRAY);


	int h_threshold = atoi(argv[2]);
	int m_threshold = atoi(argv[3]);
	int min_rad = atoi(argv[4]);
	int max_rad = atoi(argv[5]);
	int input_blur = atoi(argv[6]);
	int output_blur = atoi(argv[7]);


	GaussianBlur(greyImage, image, Size(input_blur,input_blur), 0);

	//printf("Parameters are:\n hough threshold: %d\n mag threshold: %d\n min radius: %d\n max radius: %d\n", h_threshold, m_threshold, min_rad, max_rad);

	Mat gradXImage;
	gradientX(image, gradXImage);

	// Get gradient in Y
	Mat gradYImage;
	gradientY(image, gradYImage);

	// Calculate magnitude of image
	Mat magImage;
	magnitude(gradXImage,  gradYImage, m_threshold, magImage);

    //Calculate the hough space of the image
    Mat houghImage;
	hough(image, h_threshold, m_threshold, min_rad, max_rad, houghImage);

	Mat blurredHough;
	GaussianBlur(houghImage, blurredHough, Size(output_blur,output_blur), 0);

	//printf("Time to show the image");
	imshow("Original", image);
	imshow("gradX",gradXImage);
	imshow("gradY",gradYImage);
	imshow("Mag", magImage);
	imshow("Circle centres", houghImage);

	Mat finalImage;
	finalImage.create(image.size(), CV_8UC3);

	for (int i=0;i<image.rows;i++)
	{
		for (int j=0;j<image.cols;j++)
		{
			finalImage.at<Vec3b>(i,j)[2] = houghImage.at<uchar>(i,j);
			finalImage.at<Vec3b>(i,j)[1] = magImage.at<uchar>(i,j);
			finalImage.at<Vec3b>(i,j)[0] = 0;
		}
	}

	imshow("Combined", finalImage);

	imshow("Blurred Hough",blurredHough);

	waitKey(0);

	return 0;
}*/