#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace cv;
using namespace std;


///////////////  Project 3 - License Plate Detector //////////////////////

/// <summary>
/// Struct made to sort contours from right to left
/// </summary>
struct Left_Right_contour_sorter
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
	VideoCapture cap(0);

	// Load the russian plate number identifier
	CascadeClassifier plateCascade;
	plateCascade.load("Resources/haarcascade_russian_plate_number.xml");

	// Check if the load was succesfull
	if (plateCascade.empty()) { cout << "XML file not loaded" << endl; }

	// Create variable to put the plates in scanned by the webcam
	vector<Rect> plates;

	// Loop to update webcam image
	while (true) {
		int plateLetterNumber = 0;
		int plateNumber = 0;

		// Set the capturing webcam
		cap.read(web);
		// Detect any plates in the image using the haar cascade
		plateCascade.detectMultiScale(web, plates, 1.1, 10);

		// Loop trough all the plates found
		for (int i = 0; i < plates.size(); i++)
		{
			// Get the cropped image of the plate
			Mat imgCrop = web(plates[i]);

			// Write the cropped image to the folder Plates with the name as the number assigned to the plate
			imwrite("Resources/Plates/" + to_string(i) + ".png", imgCrop);
			
			// Draw a rectangle around the found plate
			rectangle(web, plates[i].tl(), plates[i].br(), Scalar(255, 0, 255), 3);

			// Convert the image to gray scale
			Mat imgGray;
			cvtColor(imgCrop, imgGray, COLOR_BGR2GRAY);
			// Threshold the grayscale image so we can find the characters
			Mat imgT;
			threshold(imgGray, imgT, 0, 255, THRESH_BINARY_INV + THRESH_OTSU);

			// Show in a window the tresholded image
			imshow("Threshhold", imgT);


			vector<vector<Point>> contours;
			vector<Vec4i> hierarchy;

			// Find the character in the image
			findContours(imgT, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
			// Sort them from left to right
			sort(contours.begin(), contours.end(), Left_Right_contour_sorter());

			// For every character
			for (int j = 0; j < contours.size(); j++) {
				// Calculate the area of the character
				int area = contourArea(contours[j]);
				vector<Rect> boundRect(contours.size());

				boundRect[j] = boundingRect(contours[j]);

				// If the area of the character is bigger then 300 and smaller then 5000 it might be a valid 
				if (area < 5000 && area >300) {

					// Crop the image to that character
					Rect crop_region(boundRect[j].tl(), boundRect[j].br());
					Mat letter = imgCrop(crop_region);

					// Write the image to a file called nummerbord with an index number in folder Plates
					imwrite("Resources/Plates/nummerbord" + to_string(plateNumber) + "_" + to_string(plateLetterNumber)+".png",letter);

					// Write to the console what that we found a character
					cout << "Resources/Plates/nummerbord" + to_string(plateNumber) + "_" + to_string(plateLetterNumber) + " area:" + to_string(area) << endl;

					plateLetterNumber++;
				}
				
				
			}
			plateNumber++;
		}

		// Show the webcam footage
		imshow("Web", web);

		// Make the program wait to finish
		waitKey(1);
	}
}
