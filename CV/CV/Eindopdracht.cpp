#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace cv;
using namespace std;


///////////////  Project 3 - License Plate Detector //////////////////////

void main() {

	Mat web;
	Mat img;
	VideoCapture cap(2);

	CascadeClassifier plateCascade;
	plateCascade.load("Resources/haarcascade_russian_plate_number.xml");

	if (plateCascade.empty()) { cout << "XML file not loaded" << endl; }

	vector<Rect> plates;
	
	Mat plate = imread("Recources/plate.png");

	while (true) {

		cap.read(web);
		plateCascade.detectMultiScale(web, plates, 1.1, 10);

		for (int i = 0; i < plates.size(); i++)
		{
			Mat imgCrop = web(plates[i]);
			//imshow(to_string(i), imgCrop);
			imwrite("Resources/Plates/" + to_string(i) + ".png", imgCrop);
			
			rectangle(web, plates[i].tl(), plates[i].br(), Scalar(255, 0, 255), 3);
			Mat imgGray;
			cvtColor(imgCrop, imgGray, COLOR_BGR2GRAY);
			Mat imgT;
			threshold(imgGray, imgT, 0, 255, THRESH_BINARY_INV + THRESH_OTSU);

			imshow("Threshhold", imgT);

			vector<vector<Point>> contours;
			vector<Vec4i> hierarchy;

			findContours(imgT, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

			for (int j = 0; j < contours.size(); j++) {
				int area = contourArea(contours[j]);
				cout << area << endl;
				vector<Rect> boundRect(contours.size());

				boundRect[j]= boundingRect(contours[j]);
				//rectangle(imgCrop, boundRect[j].tl(), boundRect[j].br(), Scalar(0, 255, 0), 5);
				//drawContours(imgCrop, contours, j, Scalar(0, 255, 0), 2);
				Rect crop_region(boundRect[j].tl(), boundRect[j].br());
				Mat letter = imgCrop(crop_region);
				imwrite("Resources/Plates/nummerbord" + to_string(i) + "_" + to_string(j)+".png",letter);
				imshow("leter", letter);
				
			}



		}

		imshow("Web", web);
		if (!img.empty()) {
			imshow("IMG", img);
		}
		

		waitKey(1);
	}
}
