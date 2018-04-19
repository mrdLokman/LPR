#include"Composant.h"
Composant::Composant() {

}
Composant::~Composant() {

}

void Composant::setData(cv::Mat src)
{
	this->data = src;
}
void Composant::setContourExterne()
{
	if (data.empty())return;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	Mat m;
	this->data.copyTo(m);
	try {
		findContours(m, contours, hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
		this->contour_externe = contours[0];
	}
	catch (Exception e) {}
}

double Composant::ratio() {

	if (attributs.find("ratio") == attributs.end())
		attributs["ratio"] = double(this->data.cols)/ (double)(this->data.rows);

	return attributs["ratio"];
}
double Composant::densite() {
	if (data.empty())return-1;
	if (attributs.find("densite") == attributs.end()) {

		int nbrPixelNoir = data.rows * data.cols - countNonZero(data);
		attributs["densite"] = ((double)nbrPixelNoir) / ((double)((finX - debutX)*(finY - debutY)));

	}

	return attributs["densite"];
}
double Composant::portionHauteur() {
	if (attributs.find("ph") == attributs.end()) {
		attributs["ph"] = ((double)(finY - debutY)) / ((double)data.rows);
	}

	return attributs["ph"];
}

void Composant::setData(cv::Mat src)
{
	this->data = src;
}

void Composant::setContourExterne()
{
	if (data.empty())return;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	Mat m;
	this->data.copyTo(m);
	threshold(m, m, 100, 255, CV_THRESH_BINARY_INV);
	findContours(m, contours, hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	this->contour_externe = contours[0];
}




double Composant::getNbrContours()
{
	if (attributs.find("nbrContours") == attributs.end()) {
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		Mat m;
		this->data.copyTo(m);
		threshold(m, m, 100, 255, CV_THRESH_BINARY_INV);
		findContours(m, contours, hierarchy, CV_RETR_CCOMP, CHAIN_APPROX_SIMPLE, Point(0, 0));
		attributs["nbrCnotours"] = contours.size();
		
	}
		return attributs["nbrCnotours"];
}
double Composant::getSurface()
{
	if (attributs.find("surface") == attributs.end()) {
		this->attributs["surface"] = this->contour_externe.empty()? -1:contourArea(this->contour_externe);
	}
	return this->attributs["surface"];
}
double Composant::getPerimeter()
{
	if (attributs.find("perimetre") == attributs.end()) {
		this->attributs["perimetre"] = this->contour_externe.empty() ? -1 : arcLength(this->contour_externe,true);
	}
	return this->attributs["perimetre"];
}


double Composant::getContourAproximationScores()
{
	return 0.0;
}


double Composant::getYcentreDeMasse()
{
	if (attributs.find("yCentreDeMasse") == attributs.end()) {
		this->attributs["yCentreDeMasse"] = this->contour_externe.empty() ? -1 : static_cast<float>(getM01() / getM00());
	}
	return this->attributs["yCentreDeMasse"];
}
double Composant::getM00()
{
	if (attributs.find("m00") == attributs.end()) {
		this->attributs["m00"] = this->contour_externe.empty() ? -1 : cv::moments(this->contour_externe, true).m00;
	}
	return this->attributs["m00"];
}
double Composant::getM01()
{
	if (attributs.find("m01") == attributs.end()) {
		this->attributs["m01"] = this->contour_externe.empty() ? -1 : cv::moments(this->contour_externe, true).m01;
	}
	return this->attributs["m01"];
}
double Composant::getM10()
{
	if (attributs.find("m10") == attributs.end()) {
		this->attributs["m10"] = this->contour_externe.empty() ? -1 : cv::moments(this->contour_externe, true).m10;
	}
	return this->attributs["m10"];
}
double Composant::getM11()
{
	if (attributs.find("m11") == attributs.end()) {
		this->attributs["m11"] = this->contour_externe.empty() ? -1 : cv::moments(this->contour_externe, true).m11;
	}
	return this->attributs["m11"];
}
double Composant::getM02()
{
	if (attributs.find("m02") == attributs.end()) {
		this->attributs["m02"] = this->contour_externe.empty() ? -1 : cv::moments(this->contour_externe, true).m02;
	}
	return this->attributs["m02"];
}
double Composant::getM20()
{
	if (attributs.find("m20") == attributs.end()) {
		this->attributs["m20"] = this->contour_externe.empty() ? -1 : cv::moments(this->contour_externe, true).m20;
	}
	return this->attributs["m20"];
}
double Composant::getM21()
{
	if (attributs.find("m21") == attributs.end()) {
		this->attributs["m21"] = this->contour_externe.empty() ? -1 : cv::moments(this->contour_externe, true).m21;
	}
	return this->attributs["m21"];
}
double Composant::getM12()
{
	if (attributs.find("m12") == attributs.end()) {
		this->attributs["m12"] = this->contour_externe.empty() ? -1 : cv::moments(this->contour_externe, true).m12;
	}
	return this->attributs["m12"];
}

vector<double> Composant::getAll() {
	vector<double> d;
	d.push_back(this->densite());
	//d.push_back(this->getNbrContours());
	d.push_back(this->ratio());
	/*d.push_back(this->getPerimeter());
	d.push_back(this->getSurface());
	d.push_back(this->getYcentreDeMasse());
	d.push_back(this->getM01());
	d.push_back(this->getM10());
	d.push_back(this->getM11());
	d.push_back(this->getM02());
	d.push_back(this->getM12());
	d.push_back(this->getM20());
	d.push_back(this->getM21());*/
	
	return d;
}


double Composant::getCrossings()
{
	return 0.0;
}

double Composant::setDTW()
{
	return 0.0;
}


