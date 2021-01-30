//4. Напишете програма, с която да откриете и да преброите обектите във входно изображение. За всеки обект определете
//следните параметри: център (x и y координати), Aspect Ratio, Extent, Solidity, Equivalent Diameter, Orientation, Mean Intensity

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <iomanip>
using namespace cv;
using namespace std;
Mat src_gray;
int main(int argc, char** argv)
{
	CommandLineParser parser(argc, argv, "{@input | FindingContours.png | input image}");
	Mat src = imread(samples::findFile(parser.get<String>("@input")));
	if (src.empty())
	{
		cout << "Could not open or find the image!\n" << endl;
		cout << "usage: " << argv[0] << " <Input image>" << endl;
		return -1;
	}
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
	blur(src_gray, src_gray, Size(3, 3));
	const char* source_window = "Source";
	namedWindow(source_window);
	imshow(source_window, src);
	vector<vector<Point>> contours;
	findContours(src_gray, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	cout << "Count of objects: " << contours.size() << endl;
	for (int i = 0; i < contours.size(); i++) 
	{
		Rect rectangle = boundingRect(contours[i]);
		Moments m = moments(contours[i]);
		float aspect_ratio = float(rectangle.width) / rectangle.height;
		cout << "Aspect ratio of object[" << i << "] with center (x = " <<  int(m.m10 / m.m00) << " and y = " << int(m.m01 / m.m00) << ") is: " << aspect_ratio << endl;
		Point p(int(m.m10 / m.m00), int(m.m01 / m.m00));
		circle(src, p, 5, Scalar(50, 150, 70), -1);
		double area = contourArea(contours[i]);
		vector<Point> hull;
		convexHull(contours[i], hull);
		double hull_area = contourArea(hull);
		cout << "Solidity of object[" << i << "] is: " << float(area) / hull_area << endl;
	}
	imshow("Image with center", src);
	waitKey();
	return 0;
}