#include <opencv2/opencv.hpp>
#include<string>
#include<vector>
#include<iostream>

using namespace cv;
using namespace std;

int main() {
	Mat image = imread("root.jpg", IMREAD_UNCHANGED);
	imshow("IMAGE", image);
	Mat grayScaleImage;
	cvtColor(image, grayScaleImage, COLOR_BGR2GRAY);
	
	//二値化
	Mat binImage;
	//threshold(grayScaleImage, binImage, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
	adaptiveThreshold(grayScaleImage, binImage, 255,  ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY_INV,45,15);
	Mat binImageClone = binImage.clone();
	imshow("BINIMAGE", binImage);

	//全体の根の長さ計測
	vector<vector<Point>> contours;
	findContours(binImage, contours, RETR_LIST, CHAIN_APPROX_NONE);
	double coutourLenAll = 0;
	for (int i = 0; i < contours.size(); i++) {
		coutourLenAll += arcLength(contours[i], 1);
	}
	cout << "estimated root length(total)=" << coutourLenAll / 2 << endl;

	//主根の長さ計測
	erode(binImageClone, binImageClone, Mat(), Point(-1, -1), 4);
	findContours(binImageClone, contours, RETR_LIST, CHAIN_APPROX_NONE);
	double contourLenMain = 0;
	for (int i = 0; i < contours.size(); i++) {
		contourLenMain += arcLength(contours[i], 1);
	}
	cout << "estimated root length(main)=" << contourLenMain / 2 << endl;
	cout << "estimated root length(lateral)=" << (coutourLenAll-contourLenMain) / 2 << endl;
	imshow("Eroded",binImageClone);


	imwrite("outputImage.png", image);
	waitKey(0);
	return 0;
}