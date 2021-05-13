#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace cv;
using namespace std;

///////////////  Project 3 - License Plate Detector //////////////////////

void main() {

	Mat img, imgBlur, imgGray, imgCanny, imgDil, imgErode, thresholdImg;
	VideoCapture cap(0);

	CascadeClassifier plateCascade, letterCascade;
	plateCascade.load("Resources/haarcascade_russian_plate_number.xml");
	letterCascade.load("Resources/haarcascade_letters.xml");

	if (plateCascade.empty()) { cout << "XML file not loaded" << endl; }

	vector<Rect> plates;

	while (true) {

		cap.read(img);
		plateCascade.detectMultiScale(img, plates, 1.1, 10);

		for (int i = 0; i < plates.size(); i++)
		{
			Mat imgCrop = img(plates[i]);
			imshow(to_string(i), imgCrop);
			imwrite("Resources/Plates/" + to_string(i) + ".png", imgCrop);
			rectangle(img, plates[i].tl(), plates[i].br(), Scalar(255, 0, 255), 3);

			//PreProcessing
			//threshold(imgCrop, thresholdImg, 127, 1, THRESH_BINARY_INV);
			cvtColor(imgCrop, imgGray, COLOR_BGR2GRAY);
			GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
			Canny(imgBlur, imgCanny, 25, 75);

			Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
			dilate(imgCanny, imgDil, kernel);
			erode(imgDil, imgErode, kernel);
			vector<Rect> objects;
			letterCascade.detectMultiScale(imgErode, objects, 1.05, 5,0, Size(30,30));

			vector<vector<Point>> contours;
			findContours(imgDil, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
			//boundingRect(contours);
			drawContours(img(plates[i]), contours, 0, Scalar(255, 255, 0), FILLED, 8);
			imshow("Color", imgGray);
			imshow("Blur", imgBlur);
			imshow("Canny", imgCanny);
			imshow("Dilate", imgDil);
			imshow("Erode", imgErode);
		}

		imshow("Image", img);
		waitKey(1);
	}
}