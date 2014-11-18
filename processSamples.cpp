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

#include <string.h>
#include <stdio.h>
#include <fstream>

using namespace std;
using namespace cv;

#define MAG_THRESHOLD 20


void processImage(Mat inputImage, Mat &outputImage) {
	// init matrices
	Mat grayImage;
	Mat gradXImage;
	Mat gradYImage;
	Mat magImage;

	// grayscale
	cvtColor(inputImage, grayImage, CV_BGR2GRAY);

	// gradients
	gradientX(grayImage, gradXImage);
	gradientY(grayImage, gradYImage);

	// magnitude
	magnitude(gradXImage, gradYImage, MAG_THRESHOLD, magImage);

	outputImage = magImage.clone();
}

int main(int argc, char* argv[]) {
	// process input filename
	if (argc != 3) {
		cout << "Improper number of input arguements:" << endl;
		cout << "Expecting: Input file path" << endl;
		cout << "           Output file path" << endl;
	}

	char* inPathName = argv[1];
	char* outPathName = argv[2];

	// convert bitmap image
	char inFile[100];
	strcpy(inFile,inPathName);
	strcat(inFile,"dart.bmp");

	char outFile[100];
	strcpy(outFile,outPathName);
	strcat(outFile,"dart.bmp");

	Mat OriginalImage;
	OriginalImage = imread(inFile, CV_LOAD_IMAGE_COLOR);

	Mat outputImage;
	processImage(OriginalImage, outputImage);
	imwrite(outFile,outputImage);

	char fileNumber[10];

	// Cycle through positives
	for (int i=0; i<=11; i++) {
		strcpy(inFile,inPathName);
		strcat(inFile,"positives/");
		strcat(inFile,"dart");
		sprintf(fileNumber,"%d",i);
		strcat(inFile,fileNumber);
		strcat(inFile,".jpg");

		strcpy(outFile,outPathName);
		strcat(outFile,"positives/");
		strcat(outFile,"dart");
		strcat(outFile,fileNumber);
		strcat(outFile,".jpg");

		OriginalImage = imread(inFile, CV_LOAD_IMAGE_COLOR);
		processImage(OriginalImage, outputImage);
		imwrite(outFile,outputImage);

	}
	
	// Cycle through negatives according to negatives.dat
	char negStr[100];
	char imageNum[100];
	strcpy(negStr,inPathName);
	strcat(negStr,"negatives.dat");

	ifstream inputfile(negStr);

	while (inputfile >> imageNum) {
		cout << imageNum << endl;

		strcpy(inFile,inPathName);
		strcat(inFile,imageNum);

		strcpy(outFile,outPathName);
		strcat(outFile,imageNum);

		OriginalImage = imread(inFile, CV_LOAD_IMAGE_COLOR);
		processImage(OriginalImage, outputImage);
		imwrite(outFile,outputImage);
	}







	return(0);
}