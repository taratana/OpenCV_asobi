#include <opencv2/opencv.hpp>
#include<string>
#include<vector>
#include<iostream>

using namespace cv;
using namespace std;

int main() {
	Mat image = imread("seed.jpg", IMREAD_UNCHANGED);
	Mat grayScaleImage;
	cvtColor(image, grayScaleImage, COLOR_BGR2GRAY);
	
	//ìÒílâª
	Mat binImage;
	//threshold(grayScaleImage, binImage, 0, 255, THRESH_BINARY | THRESH_OTSU);
	adaptiveThreshold(grayScaleImage, binImage, 255,  ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY_INV,99,15);
	
	//é˚èkèàóù
	Mat element = Mat::ones(3, 3, CV_8UC1);
	erode(binImage, binImage, element, Point(-1, -1), 3);
	
	//ó÷äsíäèo
	vector<vector<Point>> contours;
	findContours(binImage, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	for (int i = 0; i<contours.size(); i++) {
		int count = contours[i].size();
		double x = 0, y = 0;
		for (int j = 0; j < count; j++) {
			x += contours[i][j].x;
			y += contours[i][j].y;
		}
		x /= count;
		y /= count;
		circle(image, Point(x, y), 3, Scalar(0, 0, 255), 2, 4);
	}
	cout << contours.size() << " seeds" << endl;

	imshow("IMAGE", image);	
	imwrite("outputImage.png", image);
	waitKey(0);
	return 0;
}