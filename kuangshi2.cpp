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
		//string path = "C:\\Users\\PC\\Desktop\\5.jpg";
		//img = imread(path);
		Mat imgGray, imgBlur, imgHSV, imgCanny, imgTwo, imgDil, imgErode;
		resize(img, img, Size(500, 500));
		cvtColor(img, imgHSV, COLOR_BGR2HSV);
		inRange(imgHSV, Scalar(18, 50, 103), Scalar(60, 255, 255), imgTwo);
		Mat kernel = getStructuringElement(MORPH_RECT, Size(9, 9));
		erode(imgTwo, imgErode, kernel);
		dilate(imgErode, imgDil, kernel);
		vector<vector<Point>>contours;
		vector<Vec4i>hierarchy;
		findContours(imgDil, contours, hierarchy, RETR_LIST, CHAIN_APPROX_NONE);
		double maxarea = 0;
		int m, t = 0; {};
		for (int i = 0; i < contours.size(); i++) {
			double area = contourArea(contours[i]);
			cout << area << endl;
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
		Rect rect = boundingRect(contours[t]);
		//Point2f rectPoints[4];
		//rect.points(rectPoints);
		//for (int j = 0; j < 4; j++) {
			//line(img, rectPoints[j], rectPoints[(j + 1) % 4], Scalar(0, 255, 0), 4, 8, 0);
		//}
		rectangle(img, rect.tl(), rect.br(), Scalar(0, 255, 0), 5);
		float a = rect.width / rect.height;
		cout << a << endl;
		float area1 = rect.height * rect.width;
		cout << area1 << endl;
		if (area1 < 5500) {
			cout << "空白" << endl;
		}
		else if (a < 1) {
			cout << "R面" << endl;
		}
		else if (a >= 1) {
			cout << "条形码面" << endl;
		}
		imshow("img Two", imgTwo);
		imshow("img Erode", imgErode);
		imshow("img Dil", imgDil);
		imshow("img", img);
		waitKey(1);
	}
	return 0;
}