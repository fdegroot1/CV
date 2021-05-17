#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace cv;
using namespace std;


///////////////  Project 3 - License Plate Detector //////////////////////

struct Left_Right_contour_sorter // 'less' for contours
{
	bool operator ()(const vector<Point>& a, const vector<Point>& b)
	{
		Rect ra(boundingRect(a));
		Rect rb(boundingRect(b));
		return (ra.x < rb.x);
	}
};

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

		int plateLetterNumber = 0;
		int plateNumber = 0;
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
			sort(contours.begin(), contours.end(), Left_Right_contour_sorter());

			for (int j = 0; j < contours.size(); j++) {
				int area = contourArea(contours[j]);
				vector<Rect> boundRect(contours.size());

				boundRect[j]= boundingRect(contours[j]);
				//rectangle(imgCrop, boundRect[j].tl(), boundRect[j].br(), Scalar(0, 255, 0), 5);
				//drawContours(imgCrop, contours, j, Scalar(0, 255, 0), 2);
				if (area < 5000 && area >300) {
					Rect crop_region(boundRect[j].tl(), boundRect[j].br());
					Mat letter = imgCrop(crop_region);
					imwrite("Resources/Plates/nummerbord" + to_string(plateNumber) + "_" + to_string(plateLetterNumber)+".png",letter);
					cout << "Resources/Plates/nummerbord" + to_string(plateNumber) + "_" + to_string(plateLetterNumber) + " area:" + to_string(area) << endl;
					imshow("letter", letter);
					plateLetterNumber++;
				}
				
				
			}
			plateNumber++;
		}

		imshow("Web", web);
		if (!img.empty()) {
			imshow("IMG", img);
		}
		

		waitKey(1);
	}
}
