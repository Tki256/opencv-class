#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>
//#include "opencv2/calib3d.hpp"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {

	// 1. Load Raw Iamge
	string imagepath = "/home/tki256/myProject/opencv-class/img/view4.jpg";
	Mat rawImage = imread(imagepath, IMREAD_COLOR);

	// 画像のリサイズ
	resize(rawImage, rawImage, Size(), 0.2, 0.2);

	namedWindow("Raw Image", WINDOW_AUTOSIZE);
	imshow("Raw Image", rawImage);

	// 2. Undistort Image Based on Calibration Matrix.
	Mat undistortImage(rawImage.rows, rawImage.cols, CV_8UC3);
	Matx33d K(	627.235434, 0,			654.957574,
				0,			630.482585,	494.346943,
				0,			0,			1			);
	cv::Vec4d D(-0.210146, 0.030563, 0.001172, -0.001306);
	cv::undistort(rawImage, undistortImage, K, D);
	namedWindow("Undistorted Image", WINDOW_AUTOSIZE);
	//line(undistortImage, Point(640, 0), Point(640, 960), Scalar(255, 255, 255), 10);
	//line(undistortImage, Point(0, 460), Point(1280, 460), Scalar(255, 255, 255), 10);
	imshow("Undistorted Image", undistortImage);

	// 3. Convert Image to Gray
	Mat grayImage(rawImage.rows, rawImage.cols, CV_8UC1);
	cvtColor(undistortImage, grayImage, COLOR_RGB2GRAY);
	namedWindow("Gray Image", WINDOW_AUTOSIZE);
	imshow("Gray Image", grayImage);

	// 4. Top View Conversion
	Mat topImage(rawImage.rows, rawImage.cols, CV_8UC3);
	topImage.setTo(Scalar(0));

	Mat topImageGray(rawImage.rows, rawImage.cols, CV_8UC1);
	topImageGray.setTo(Scalar(0));
	{
		double Hvc = 1.0;
		double Hc = 0.1;
		double Dvc = 0.1;
		double f = 509;
		double fp = f;
		double theta = 30.0 / 180.0 * M_PI;
		double s = sin(theta);
		double c = cos(theta);
		int cx = 450;
		int cy = 500;
		int cxp = 450;
		int cyp = 500;

		for (int y = 0; y < topImage.rows; y++) {
			for (int x = 0; x < topImage.cols; x++) {

				int xOrg = x - cx;
				int yOrg = - y + cy;

				int newX = fp / Hvc * Hc * xOrg / (f * s - yOrg * c);
				int newY = fp / Hvc * (Hc * (f * c + yOrg * s) / (f * s - yOrg * c) - Dvc);

				newX = newX + cxp;
				newY = -newY + cyp;

				if (newX < 0 || topImage.cols - 1 < newX || newY < 0 || topImage.rows - 1 < newY ) {
					continue;
				}

				topImageGray.data[newY * topImageGray.cols + newX] = grayImage.data[y * grayImage.cols + x];

				topImage.data[(newY * topImage.cols + newX) * topImage.channels()] = undistortImage.data[(y * topImage.cols + x) * topImage.channels()];
				topImage.data[(newY * topImage.cols + newX) * topImage.channels() + 1] = undistortImage.data[(y * topImage.cols + x) * topImage.channels() + 1];
				topImage.data[(newY * topImage.cols + newX) * topImage.channels() + 2] = undistortImage.data[(y * topImage.cols + x) * topImage.channels() + 2];

			}
		}
	}

	namedWindow("Top Image", WINDOW_AUTOSIZE);
	imshow("Top Image", topImage);
	imwrite("/home/tki256/myProject/opencv-class/result/topview4.jpg", topImage);
	namedWindow("Top Image Gray", WINDOW_AUTOSIZE);
	imshow("Top Image Gray", topImageGray);
	imwrite("/home/tki256/myProject/opencv-class/result/topview_gray4.jpg", topImageGray);

	while(true) {
		waitKey(0);
	}
	return 0;
}

