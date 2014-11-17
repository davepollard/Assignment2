// header inclusion
#ifndef OPENCV2_H_
#define OPENCV2_H_
#include <opencv2/opencv.hpp>
#endif

#include "conv.h"

void conv(cv::Mat &input, cv::Mat kernel, cv::Mat &Output)
{
	// intialise the output using the input
	Output.create(input.size(), CV_32S);
	
	//Create a padded output
	cv::Mat paddedInput;
	Size s = kernel.size();
	int size = (s.height-1)/2;
	cv::copyMakeBorder( input, paddedInput, size, size, size, size,cv::BORDER_REPLICATE);

	// now we can do the convolution
	//Store max/min values for normalisation
	double maxval = -9999;
	double minval = 9999;

	//Perform the convolution
	//Loop through image
	for ( int i = 0; i < input.rows; i++ )
	{	
		for( int j = 0; j < input.cols; j++ )
		{
			//Sum stores the value to be put in pixel (i,j)
			//Loop through kernel
			double sum = 0.0;
			for( int m = -size; m <= size; m++ )
			{
				for( int n = -size; n <= size; n++ )
				{
					// Correct the indices
					int imagex = i + m + size;
					int imagey = j + n + size;
					int kernelx = m + size;
					int kernely = n + size;

					// get the values from the padded image and the kernel
					int imageval = (int) paddedInput.at<uchar>(imagex, imagey);
					double kernelval = kernel.at<double>(kernelx, kernely);
					//printf("Image val is %d, kernel val is %0.2lf\n", imageval, kernelval);
					sum += ((double) imageval * kernelval);
				}
			}

			//Value for pixel(i,j) has been calculated
			//Set the output as the convolution sum
			Output.at<int>(i, j) = (int) sum;
			//printf("Pixel at (%d, %d) has value %d\n", i, j, sum);

			//Check to see if we have a new min/max pixel intensity
			if (sum > maxval)
			{
				maxval = sum;
				//printf("New max is %d\n", sum);
			} 

			if (sum < minval)
			{
				minval = sum;
				//printf("New max is %d\n", sum);
			}
		}
	}
	
	//Calculate scaling factor
	double scaling = 255.0 / (maxval-minval);
	
	//shift & normalise the output
	for ( int i = 0; i < input.rows; i++ )
	{	
	  for( int j = 0; j < input.cols; j++ )
	  {
	    double imageval = (double) Output.at<int>(i, j);
	    //printf("Imageval is %lf\n", imageval);
	    double scaledval = ((imageval - (double)minval) * scaling);
	    Output.at<int>(i,j) = (int) scaledval;
	    //printf("Pixel at (%d, %d) has value %lf\n", i, j, scaledval);
	    //printf("Stored pixel at (%d, %d) has value %d\n\n", i, j, Output.at<int>(i, j));
	  }
	}
	//printf("Max is %d, Min is %d\n", maxval, minval);
	//printf("Scaling factor is %lf\n\n\n", scaling);
	Output.convertTo(Output, CV_8U);
	 
}


