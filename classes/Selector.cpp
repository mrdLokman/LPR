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


Mat Selector::cropImage(Mat src, Point* coins) {
	// coins[0] top left corner ,  coins[1] top right corner ,  coins[2] bottom right corner ,  coins[3] bottom left corner
	int xmin = coins[0].x < coins[3].x ? coins[0].x : coins[3].x;
	int xmax = coins[1].x < coins[2].x ? coins[2].x : coins[1].x;
	int ymin = coins[0].y < coins[1].y ? coins[0].y : coins[1].y;
	int ymax = coins[2].y < coins[3].y ? coins[3].y : coins[2].y;

	int w = xmax - xmin + 1;
	int h = ymax - ymin + 1;
	//cout << "width = " << w << " ;heigth = " << h << endl;
	 Mat plaque(src, Rect(xmin, ymin, w, h)); 
	return plaque;
}


