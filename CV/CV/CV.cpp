#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;


/////////////////  Images  //////////////////////

void main() {
	//Path to my image
	string path = "Resources/ballsandstripes.png";
	//Load in the image

	
	
	Mat img = imread(path);
	Mat thresholdImg;
	threshold(img,thresholdImg,127, 1, THRESH_BINARY_INV);
	Mat erosionImg, dilateImg;

	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5), Point(-1, -1));

	erode(thresholdImg, erosionImg, element);
	dilate(erosionImg, dilateImg, element);

	imshow("Image", img);
	imshow("Image E", erosionImg*255);
	imshow("Image D", dilateImg*255);
	waitKey(0);

}
