#include "Selector.h"
using namespace cv;



Selector::Selector()
{
	
}
Selector::~Selector()
{
}

void Selector::addPoint(cv::Point p)
{
	this->getPoints()->push_back(p);
}

vector<cv::Point>* Selector::getPoints()
{
	return &this->points;
}

void Selector::selectPointsOnClick(int event, int x, int y, int flags, void * vec)
{ 
	Selector* s = (Selector*)vec;
	if (event == cv::EVENT_LBUTTONDOWN)
	{	
			cv::Point p = cv::Point(x, y);
			s->addPoint(p);	
	}
	else if (event == cv::EVENT_RBUTTONDOWN)
	{
		if (!s->getPoints()->empty()) {
			cv::Point rp = s->getPoints()->back();
			(s->getPoints())->pop_back(); 	
		}
		else {
			cout << "please select points with the left button" << endl;
		}
	}

}

Point * Selector::getPlaqueCorners(Mat img)
{
	cv::namedWindow("My Window", 1);
	Selector sel;
	cv::setMouseCallback("My Window", Selector::selectPointsOnClick, &sel);
	//showing the image
	imshow("My Window", img);
	// Wait until press some key..
	cv::waitKey(0);
	cvDestroyWindow("My Window");
	Point* corner_list = new cv::Point[4];
	int i = 0;
	for each (cv::Point p in *sel.getPoints())
	{
		if (i<4) corner_list[i++] = cv::Point(p.x, p.y);
	}
	cvWaitKey(0);
	return corner_list;
}
