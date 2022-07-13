#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>
using namespace std;
using namespace cv;
Mat img;
int main()
{
	VideoCapture cap(0);
	while (true) {
		cap.read(img);
		Mat imgGray, imgBlur, imgHSV, imgCanny, imgTwo, imgDil, imgErode;
		//resize(img, img, Size(500, 500));
		cvtColor(img, imgHSV, COLOR_BGR2HSV);
		inRange(imgHSV, Scalar(100, 50, 50), Scalar(130, 255, 255), imgTwo);
		Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
		erode(imgTwo, imgErode, kernel);
		dilate(imgErode, imgDil, kernel);
		vector<vector<Point>>contours;
		vector<Vec4i>hierarchy;
		findContours(imgDil, contours, hierarchy, RETR_LIST, CHAIN_APPROX_NONE);
		double maxarea = 0;
		int m, t;
		for (int i = 0; i < contours.size(); i++) {
			double area = contourArea(contours[i]);
			//cout << area << endl;
			if (area > maxarea) {
				maxarea = area;
				m = i;
			}
		}
		maxarea = contourArea(contours[0], true);
		for (int i = 0; i < contours.size() && i != m; i++) {
			double area = contourArea(contours[i]);
			if (area > maxarea) {
				maxarea = area;
				t = i;
			}
		}
		RotatedRect rect = minAreaRect(contours[t]);
		Point2f rectPoints[4];
		rect.points(rectPoints);
		for (int j = 0; j < 4; j++) {
			line(img, rectPoints[j], rectPoints[(j + 1) % 4], Scalar(0, 255, 0), 4, 8, 0);
		}
		//cout << "X坐标：" << rect.center.x << " Y坐标：" << rect.center.y << " 偏转角度：" << rect.angle << endl;
		if (rect.angle <= 50 && rect.angle >= 30) {
			cout << "右转" << endl;
		}
		else if ((rect.angle > 0 && rect.angle < 20) || (rect.angle>=80 &&rect.angle<90) ) {
			cout << "直行" << endl;
		}
		else {
			cout << "左转" << endl;
		}
		//imshow("img Two", imgTwo);
		//imshow("img Erode", imgErode);
		//imshow("img Dil", imgDil);
		imshow("img", img);
		waitKey(1);
	}
	return 0;
}