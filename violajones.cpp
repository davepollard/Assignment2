#ifndef IOSTREAM_H_
#define IOSTREAM_H_
#include <iostream>
#endif

#ifndef OPENCV2_H_
#define OPENCV2_H_
#include <opencv2/opencv.hpp>
#endif

#include "violajones.h"

using namespace cv;

/** Global variables */
// pass this in later
String cascade_name = "dartcascade.xml";
CascadeClassifier cascade;

std::vector<Rect> ViolaJonesDetection(Mat image, Mat &retImage){
	std::vector<Rect> returnRect;
	returnRect = detectAndDisplay(image);
	//imshow("detected faces", image);
	retImage = image.clone();

	return(returnRect);
}


std::vector<Rect> detectAndDisplay( Mat frame )
{
	std::vector<Rect> faces;
	Mat frame_gray;

	// 1. Prepare Image by turning it into Grayscale and normalising lighting
	cvtColor( frame, frame_gray, CV_BGR2GRAY );
	equalizeHist( frame_gray, frame_gray );

	// 2. Perform Viola-Jones Object Detection 
	cascade.detectMultiScale( frame_gray, faces, 1.1, 1, 0|CV_HAAR_SCALE_IMAGE, Size(50, 50), Size(500,500) );

    // 3. Draw box around faces found
	for( int i = 0; i < faces.size(); i++ )
	{
		rectangle(frame, Point(faces[i].x, faces[i].y), Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), Scalar( 0, 255, 0 ), 2);
	}

	return(faces);

}
