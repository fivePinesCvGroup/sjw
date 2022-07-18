#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include<vector>
using namespace cv;
using namespace std;
Mat img;
int main()
{
	VideoCapture cap(0);
	while (true) {
		cap.read(img);
		//string path = "C:\\Users\\PC\\Desktop\\7.jpg";
		//img = imread(path, IMREAD_COLOR);
		if (img.empty())
		{
			cout << "Could not open or find the image" << std::endl;
			return -1;
		}
		resize(img, img, Size(500, 500));
		Mat  imgHSV,thresHold, a, b, c, d;
		vector<Mat>hsvSplit;
		vector<Mat>channels;
		cvtColor(img, imgHSV, COLOR_BGR2HSV);
		split(imgHSV, hsvSplit);
		equalizeHist(hsvSplit[2], hsvSplit[2]);
		merge(hsvSplit, imgHSV);
		split(img, channels);
		meanStdDev(channels[0], a, b);
		meanStdDev(channels[2], c, d);
		double m = a.at<double>(0, 0);
		double n = c.at<double>(0, 0);
		if (m > n) {
			cout << "蓝色" << endl;
		}
		else {
			cout << "红色" << endl;
		}
		threshold(hsvSplit[2], thresHold, 240, 245, THRESH_BINARY);
		blur(thresHold, thresHold, Size(3, 3));
		Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
		dilate(thresHold, element, element);
		vector<RotatedRect> h;
		vector<vector<Point>>contours;

		findContours(element.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		for (int i = 0; i < contours.size(); i++) {
			double area = contourArea(contours[i]);
			//cout << area << endl;
			if (area < 15 || contours[i].size() <= 10)
				continue;
			RotatedRect Light = fitEllipse(contours[i]);
			if (Light.size.width / Light.size.height > 1.5
				|| area / Light.size.area() < 0.5)
				continue;
			Light.size.height *= 1.1;
			Light.size.width *= 1.1;
			h.push_back(Light);
		}
		for (size_t i = 0; i < h.size(); i++)
		{
			for (size_t j = i + 1; (j < h.size()); j++)
			{
				Point2f point1;
				Point2f point2;
				point1.x = h[i].center.x;
				point1.y = h[i].center.y + 20;
				point2.x = h[j].center.x;
				point2.y = h[j].center.y - 20;
				rectangle(img, point1, point2, Scalar(0, 255, 0), 2);
				double height, width;
				height = (h[i].size.height + h[j].size.height) / 2;
				width = sqrt((h[i].center.x - h[j].center.x) * (h[i].center.x - h[j].center.x) + (h[i].center.y - h[j].center.y) * (h[i].center.y - h[j].center.y));
				double ratio = height / width;
				if (ratio <= 1) {
					cout << "小装甲板" << endl;
				}
				else
				{
					cout << "大装甲板" << endl;
				}
			}
		}
		imshow("img", img);
		imshow("img thresHold", thresHold);
		imshow("img hsv", imgHSV);
		waitKey(1);
	}
	return 0;
}