#include <opencv2/opencv.hpp>
using namespace cv;


int main() {
	Mat image = imread("seed.jpg", IMREAD_UNCHANGED);
	Mat grayScaleImage;
	cvtColor(image, grayScaleImage, COLOR_BGR2GRAY);
	Mat binImage;
	threshold(grayScaleImage, binImage, 0, 255, THRESH_BINARY | THRESH_OTSU);
	imshow("IMAGE", binImage);



	waitKey(0);
	return 0;
}