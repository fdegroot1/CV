#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;


/////////////////  Images  //////////////////////

float w = 250, h = 500;
Mat matrix, imgWarp;

void main() {
	//Path to my image
	string path = "Resources/ds2.jpg";
	//Load in the image

	Mat img = imread(path);
	Mat resizedImage;
	Size size(img.size().width/4, img.size().height/4);
	resize(img, resizedImage, size);

	Point2f src[4] = { {90,1950},{1160,1220}, {1500,3900},{2500,3160} };
	Point2f dst[4] = { {0.0f,0.0f},{w,0.0f},{0.0f,h},{w,h} };

	matrix = getPerspectiveTransform(src, dst);
	warpPerspective(img, imgWarp, matrix,Point(w,h));
	//Show the image
	imshow("Image", resizedImage);
	imshow("Image Warp", imgWarp);
	waitKey(0);

}
