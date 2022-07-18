#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>
using namespace std;
using namespace cv;
Mat img;
void one() {
	VideoCapture cap(1);
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
			//return"右转";
		}
		else if ((rect.angle > 0 && rect.angle < 20) || (rect.angle >= 80 && rect.angle < 90)) {
			cout << "直行" << endl;
			//return"直行";
		}
		else {
			cout << "左转" << endl;
			//return"左转";
		}
		//imshow("img Two", imgTwo);
		//imshow("img Erode", imgErode);
		//imshow("img Dil", imgDil);
		imshow("img", img);
		waitKey(1);
	}
}
void two() {
	VideoCapture cap(1);
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
			//return"空白";
		}
		else if (a < 1) {
			cout << "R面" << endl;
			//return"R面";
		}
		else if (a >= 1) {
			cout << "条形码面" << endl;
			//return"条形码面";
		}
		imshow("img Two", imgTwo);
		imshow("img Erode", imgErode);
		imshow("img Dil", imgDil);
		imshow("img", img);
		waitKey(1);
	}
}
void three() {
	VideoCapture cap(1);
    while (true) {
	cap.read(img);
	//string path = "C:\\Users\\PC\\Desktop\\7.jpg";
	//img = imread(path, IMREAD_COLOR);
	//if (img.empty())
	//{
		//cout << "Could not open or find the image" << std::endl;
		//return -1;
	//}
	resize(img, img, Size(500, 500));
	Mat imgGray, imgBlur, imgHSV, imgCanny, imgTwo, imgDil, imgErode, thresHold, a, b, c, d;
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
		//return"蓝色";
	}
	else {
		cout << "红色" << endl;
		//return"红色";
	}
	threshold(hsvSplit[2], thresHold, 240, 245, THRESH_BINARY);
	blur(thresHold, thresHold, Size(3, 3));
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
	dilate(thresHold, element, element);
	vector<RotatedRect> vc;
	vector<RotatedRect> vRec;
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
		vc.push_back(Light);
		//vector<Rect>rect(contours.size());
   //rectangle(img, rect[i].tl(), rect[i].br(), Scalar(0, 255, 0), 5);
	}
	for (size_t i = 0; i < vc.size(); i++)
	{
		for (size_t j = i + 1; (j < vc.size()); j++)
		{
			Point2f point1;
			Point2f point2;
			point1.x = vc[i].center.x;
			point1.y = vc[i].center.y + 20;
			point2.x = vc[j].center.x;
			point2.y = vc[j].center.y - 20;
			rectangle(img, point1, point2, Scalar(0, 255, 0), 2);
			double height, width;
			height = (vc[i].size.height + vc[j].size.height) / 2;
			width = sqrt((vc[i].center.x - vc[j].center.x) * (vc[i].center.x - vc[j].center.x) + (vc[i].center.y - vc[j].center.y) * (vc[i].center.y - vc[j].center.y));
			double ratio = height / width;
			if (ratio <= 1) {
				cout << "小装甲板" << endl;
				//return"小装甲板";
			}
			else
			{
				cout << "大装甲板" << endl;
				//return"大装甲板";
			}
		}
	}
	//imshow("img", img);
	imshow("img thresHold", thresHold);
	imshow("img hsv", imgHSV);
	waitKey(1);
    }
}
int main()
{
	int i = 0;
	while (true) {
		if (i == 0) {
			cin >> i;
		}
		if (i == 1) {
		    one();
			i--;
		}
		if (i == 2) {
			two();
			i = i - 2;
		}
		if (i == 3) {
		    three();
			i = i - 3;
		}
	}
	return 0;
}