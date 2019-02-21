#include <opencv2/opencv.hpp>
#include<string>
#include<iostream>
#include<math.h>

using namespace cv;
using namespace std;

#define LANE_WIDTH  50

double r(double x1, double y1, double x2, double y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

int main() {
	Mat tmpl = imread("template-1.png");
	Mat screws = imread("screw.png");
	imshow("template", tmpl);
	imshow("screws", screws);

	for (int i = 0; i < 3; i++) {
		int roi_x = 0;
		if (i == 0)roi_x = 84;
		if (i == 1)roi_x = 155;
		if (i == 2)roi_x = 229;

		Rect roi_rect = Rect(roi_x, 0, LANE_WIDTH, screws.rows);
		Mat singleScrew(screws.clone(), roi_rect);
		rectangle(screws, roi_rect, Scalar(0, 0, 255), 1);
		imshow("screws highlight", screws);

		Mat singleScrewLarge = Mat::zeros(screws.rows, screws.rows, CV_8UC3);
		Rect copyRect = Rect(singleScrewLarge.rows*0.5 - LANE_WIDTH*0.5, 0, LANE_WIDTH, singleScrewLarge.rows);
		singleScrew.copyTo(singleScrewLarge(copyRect));
		imshow("singleScrewLarge", singleScrewLarge);

		int maxAngle = -1;
		double maxValGlobal = -1;

		for (int angle = -180; angle < 180; angle++) {
			Mat rotSingleScrew;
			float scale = 1.0;

			Point2f center(singleScrewLarge.cols*0.5, singleScrewLarge.rows*0.5);
			const Mat affineMatrix = getRotationMatrix2D(center, angle, scale);
			warpAffine(singleScrewLarge, rotSingleScrew, affineMatrix, singleScrewLarge.size());

			Mat resultImage;
			matchTemplate(rotSingleScrew, tmpl, resultImage, TM_CCOEFF_NORMED);
			Point maxPt;
			double maxVal;
			minMaxLoc(resultImage, NULL, &maxVal, NULL, &maxPt);
			
			Rect roiRect2(0, 0, tmpl.cols, tmpl.rows);
			roiRect2.x = maxPt.x;
			roiRect2.y = maxPt.y;
			rectangle(rotSingleScrew, roiRect2, Scalar(0, 0, 255 * maxVal), 2);

			imshow("rot single screw", rotSingleScrew);
			imshow("result", resultImage);
			
			if (maxVal > maxValGlobal) {
				maxValGlobal = maxVal;
				maxAngle = angle;
			}
			waitKey(1);
		}
		cout << "‚Ë‚¶#" << i << "‚Í" << maxAngle << "ŒX‚¢‚Ä‚¢‚Ü‚·" << endl;
	}


	waitKey(0);
	return 0;
}