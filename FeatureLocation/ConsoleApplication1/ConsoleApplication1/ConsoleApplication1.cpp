// ConsoleApplication1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<opencv2\opencv.hpp>
#include <iostream>
#include<cv.h>  
#include <opencv2\core\core.hpp>
#include<opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <math.h>

using namespace cv;
using namespace std;
bool compare_2matrix(Rect initial, Rect judge);//判断两矩形是否按照字符排列
//bool comp(vector<Rect> a, vector<Rect> b) { return (a[0].height<b[0].height); }
int _tmain(int argc, _TCHAR* argv[])
{
	vector<vector<Point> > contours;
	vector<Rect>contours2,temp;
	vector<vector<Rect>> Classify;
	vector<Vec4i> hierarchy;
	Mat test;
	string path = "H:\\FeatureLocation\\ConsoleApplication1\\TCKU2465203";
	string pathname = path + ".jpg";
	Mat img = imread(pathname,1);
	cvtColor(img, test, COLOR_BGR2GRAY);
//	blur(test, test, Size(3, 3));
	adaptiveThreshold(test, test, 255, ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, 25, -8);
	test = ~test;
	findContours(test, contours, hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE, Point(0, 0));
	Rect boundRect;
	for (int i = 0; i < contours.size(); i++)
	{
		boundRect = boundingRect(Mat(contours[i]));
		if (boundRect.height>10 && boundRect.height<50 && boundRect.width < boundRect.height)
		{
			contours2.push_back(boundRect);
		}
	}

	/*for (int i = 0; i < contours2.size(); i++)
	{
	rectangle(test, contours2[i], Scalar(255, 255, 255), 1, 4, 0);
	}*/
	int count = 0;
	int sum = contours2.size();
	while (count < sum)
	{
		bool flag = false;
		for (int i = 0; i < Classify.size(); i++)
		{
			for (int j = 0; j < Classify[i].size(); j++)
			{
				for (int k = 0; k < contours2.size(); k++)
				{
					if (compare_2matrix(Classify[i][j], contours2[k]))
					{
						Classify[i].push_back(contours2[k]);
						flag = true;
						contours2.erase(contours2.begin() + k);
						k--;
						count++;
					}
				}
			}
			if (Classify[i].size()<4)
			{
				Classify.erase(Classify.begin() + i);
			}
		}
		if (!flag)
		{
			temp.push_back(contours2[contours2.size()-1]);
			Classify.push_back(temp);
			temp.pop_back();
			contours2.pop_back();
			count++;
		}
	}
	/*for (int i = 0; i < contours2.size();i++)
	{
	bool flag = false;
	for (int j = 0; j < Classify.size();j++)
	{
	for (int k = 0; k < Classify[j].size();k++)
	{
	if (compare_2matrix(Classify[j][k],contours2[i]))
	{
	Classify[j].push_back(contours2[i]);
	flag = true;
	break;
	}
	}
	if (flag)
	{
	break;
	}
	}
	if (!flag)
	{
	temp.push_back(contours2[i]);
	Classify.push_back(temp);
	temp.clear();
	}
	}*/

	for (int i = 0; i < Classify.size();i++)
	{
		if (Classify[i].size()>3)
		{
			for (int j = 0; j < Classify[i].size();j++)
			{
				rectangle(img, Classify[i][j], Scalar(0, 255, 255), 1, 4, 0);
			}
		}
	}
	string testpath = path + "_test.jpg";
	imwrite(testpath, img);
	img.release();
	test.release();
//	sort(contours2.begin(), contours2.end(), comp);
	/*Mat savedGrayMat = Mat::zeros(test.rows, test.cols, CV_8UC1);
	drawContours(savedGrayMat, contours, 0, Scalar(255), CV_FILLED);
	imshow("savedGrayMat", savedGrayMat);
	waitKey();*/
	return 0;
}
bool compare_2matrix(Rect initial, Rect judge)
{
	int Height = initial.height;
	int Width = int(Height * 0.75);
	int x0 = initial.x;
	int y0 = initial.y;
	int x1 = judge.x;
	int y1 = judge.y;
	if (abs(judge.height - Height) > Height / 4)//大小相近
	{
		return false;
	}
	if (abs(x1 - x0) < Width / 2 && abs(y1 - y0) - Height < Height / 2)//y方向排列
	{
		return true;
	}
	if (abs(abs(x1 - x0) - Width) < Width / 2 /*&& (abs(x1 - x0) - initial.width) > 0*/ && abs(y1 - y0) < Height / 4)//x方向排列
	{
		return true;
	}
	return false;
}

