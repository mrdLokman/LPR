#include "Pretraitement.h"

Mat extractionDePlaque(Mat src, Point* coins) {
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

Mat correctionRotation(Mat origine, Point* coins) {
	Point2f pointSrc[3];

	int xmin = coins[0].x < coins[3].x ? coins[0].x : coins[3].x;
	int ymin = coins[0].y < coins[1].y ? coins[0].y : coins[1].y;

	pointSrc[0] = Point2f(coins[0].x - xmin, coins[0].y - ymin);
	pointSrc[1] = Point2f(coins[1].x - xmin, coins[1].y - ymin);
	pointSrc[2] = Point2f(coins[3].x - xmin, coins[3].y - ymin);

	Point2f pointDest[3];

	Mat rot_mat(2, 3, CV_32FC1);
	Mat warp_mat(2, 3, CV_32FC1);
	Mat correction;

	int w = ceil(sqrt(float((pointSrc[0].x - pointSrc[1].x)*(pointSrc[0].x - pointSrc[1].x) + (pointSrc[0].y - pointSrc[1].y)*(pointSrc[0].y - pointSrc[1].y))));
	int h = ceil(sqrt(float((pointSrc[0].x - pointSrc[2].x)*(pointSrc[0].x - pointSrc[2].x) + (pointSrc[0].y - pointSrc[2].y)*(pointSrc[0].y - pointSrc[2].y))));

	//cout << "width = " << w << " ;heigth = " << h <<endl;

	correction = Mat::zeros(h, w, origine.type());

	pointDest[0] = Point2f(0, 0);
	pointDest[1] = Point2f(correction.cols - 1, 0);
	pointDest[2] = Point2f(0, correction.rows - 1);

	warp_mat = getAffineTransform(pointSrc, pointDest);

	warpAffine(origine, correction, warp_mat, correction.size());

	return correction;
}

Mat correctionInclinaison(Mat origine) {
	Mat dest, rot_mat,finale;
	int min = 100000000000;

	Point center = Point(origine.cols / 2, origine.rows / 2);
	double angle = 0.0;
	double scale = 1.0;
	string name = "../output/r";

	double theta = 5.0;
	while (theta >= -5) {

		rot_mat = getRotationMatrix2D(center, theta, scale);
		warpAffine(origine, dest, rot_mat, origine.size(), 1, 0,255);

		vector<int> hist = histogrammeProjectionHorizontal(dest);
		
		/*
		for (int i = 0; i < hist.size(); i++){
			cout << hist.at(i) << ", ";
		}
		cout << endl;
		*/
		
		
		int moy = moyenne(hist);
		int minD = minDroit(hist, moy);
		int minG = minGauche(hist, moy);
		int seg1 = abs(minG - minD);
		int seg2 = segma(hist);
		int seg = seg1*seg2;

		if (seg < min) {
			min = seg;
			dest.copyTo(finale);
		}

		Mat h = afficheHistogrammeProjectionHorizontal(hist); // mareHistogram(dest, 1);
		imwrite(name + to_string(theta) + "---" + "hist.png", h);
		imwrite(name+ to_string(theta)+"---"+to_string(seg)+".png", dest);
		theta=theta-1;
	}
	

	return finale;
}

Mat correctionInclinaisonEliminerBordure(Mat origine) {
	Mat dest, rot_mat, finale;
	int max = -1;
	int MAX_D = 0, MAX_G = origine.cols - 1;


	Point center = Point(origine.cols / 2, origine.rows / 2);
	double angle = 0.0;
	double scale = 1.0;
	string name = "../output/r";

	double theta = 5.0;
	while (theta >= -5) {

		rot_mat = getRotationMatrix2D(center, theta, scale);
		warpAffine(origine, dest, rot_mat, origine.size(), 1, 0, 0);

		vector<int> hist = histogrammeProjectionHorizontalGrey(dest);

		/*
		for (int i = 0; i < hist.size(); i++){
		cout << hist.at(i) << ", ";
		}
		cout << endl;
		*/

		int moy = moyenne(hist);
		int maxD = maxDroit(hist, moy);
		int maxG = maxGauche(hist, moy);

		int valG = hist.at(maxG), valD = hist.at(maxD);

		int maxL = valD > valG ? valD : valG;

		/*
		cout << "---------------" << endl;
		cout << "rotation = " << theta << endl;
		cout << "max is " << max << " and maxL is " << maxL << endl;
		cout << "maxG = " << maxG << " maxD = " << maxD << endl;
		cout << endl;
		*/

		if (maxL >= max) {
			max = maxL;
			MAX_D = maxD;
			MAX_G = maxG;
			dest.copyTo(finale);
		}
		
		/*
		int w = dest.cols;
		for (int i = 0; i < w; i++)
		{
			dest.at<unsigned char>(maxG, i) = 255;
			dest.at<unsigned char>(maxD, i) = 255;
		}
		

		Mat h = afficheHistogrammeProjectionHorizontal(hist);
		imwrite(name + to_string(theta) + "-" + "hist.png", h);
		imwrite(name + to_string(theta) + "-" + to_string(maxL) + ".png", dest);
		*/

		theta = theta - 1;
	}
	
	Mat plaque(finale, Rect(0, MAX_G, origine.cols, abs(MAX_D - MAX_G + 1)));

	// bordure à gauche et à droite:
	vector<int> hist = histogrammeProjectionVerticalGrey(dest);
	

	return plaque;
}

vector<int> histogrammeProjectionHorizontalGrey(Mat img) {
	int sz = img.rows;
	int max = img.cols;

	vector<int> counts(sz, 0);

	for (int i = 0; i < sz; i++){
		int sum = 0;
		for (int j = 0; j < max; j++){
			sum += img.at< unsigned char >(i, j);
		}
		sum = sum/255;
		counts.at(i) = sum;
	}

	return counts;
}

vector<int> histogrammeProjectionVerticalGrey(Mat img) {
	int w = img.cols;
	int h = img.rows;

	vector<int> counts(w, 0);

	for (int i = 0; i < w; i++) {
		int sum = 0;
		for (int j = 0; j < h; j++) {
			sum += img.at< unsigned char >(j, i);
		}
		sum = sum / 255;
		counts.at(i) = sum;
	}

	return counts;
}

vector<int> histogrammeProjectionHorizontal(Mat img) {

	int sz = img.rows;
	int max = img.cols;

	vector<int> counts(sz,0);

	for (int j = 0; j < sz; ++j)
	{
		Mat data = img.row(j);
		counts.at(j) = max - countNonZero(data);
	}

	return counts;
}

vector<int> histogrammeProjectionVertical(Mat img) {
	int sz = img.cols;
	int max = img.rows;

	vector<int> counts(sz, 0);

	for (int j = 0; j < sz; ++j)
	{
		Mat data = img.col(j);
		counts.at(j) = max - countNonZero(data);
	}

	return counts;
}

 
int moyenne(vector<int> hist) {
	int somme = 0, taille = hist.size(), moyenne = 0;

	for (int i = 0; i < taille; i++) {
		somme += hist.at(i);
		moyenne += i*hist.at(i);
	}

	return moyenne / somme;
}
int segma(vector<int> hist) {
	int s = 0, taille = hist.size(), moy = 0;

	moy = moyenne(hist);

	for (int i = 0; i < taille; i++) {
		s += abs(moy - i)*hist.at(i);
	}

	return s;
}

int minDroit(vector<int> hist, int moy) {
	
	int iMin = moy, min= hist.at(moy),t=hist.size();
	for (int i = moy; i < t;i++)
		if (hist.at(i) < min) {
			min = hist.at(i);
			iMin = i;
		}

	return iMin;
}
int minGauche(vector<int> hist, int moy) {
	int iMin = moy, min = hist.at(moy), t = hist.size();
	for (int i = moy; i >= 0; i--)
		if (hist.at(i) < min) {
			min = hist.at(i);
			iMin = i;
		}

	return iMin;
}

int maxDroit(vector<int> hist, int moy) {

	int iMax = moy, max = hist.at(moy), t = hist.size();
	for (int i = moy; i < t; i++)
		if (hist.at(i) > max) {
			max = hist.at(i);
			iMax = i;
		}

	return iMax;
}
int maxGauche(vector<int> hist, int moy) {
	int iMax = moy, max = hist.at(moy), t = hist.size();
	for (int i = moy; i >= 0; i--)
		if (hist.at(i) > max) {
			max = hist.at(i);
			iMax = i;
		}

	return iMax;
}

Mat mareHistogram(Mat img, int t)
{

	//col or row histogram?
	int sz = (t) ? img.rows : img.cols;
	int max = -1; // (t) ? img.cols : img.rows;
	vector<int> counts;

	for (int j = 0; j < sz; ++j)
	{
		Mat data = (t) ? img.row(j) : img.col(j);
		int v = countNonZero(data);
		counts.push_back(v);
		if (v > max)
			max = v;
	}

	Mat histo;
	int width, height;
	if (t)
	{
		width = max;
		height = sz;
		histo = Mat::zeros(Size(width, height), CV_8U);

		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < counts.at(i); ++j)
				histo.at< unsigned char >(i, j) = 255;
		}

	}
	else {
		width = sz;
		height = max;
		histo = Mat::zeros(Size(width, height), CV_8U);

		for (int i = 0; i< width; ++i)
		{
			for (int j = 0; j<  counts.at(i); ++j)
				histo.at< unsigned char >(max - j - 1, i) = 255;
		}

	}



	return histo;

}

void histogrammeImage(string img) {

	Mat src = imread(img, 0);
	Mat bin;
	threshold(src, bin, 128, 255, CV_THRESH_BINARY);

	Mat hHisto = mareHistogram(bin, 0);
	Mat vHisto = mareHistogram(bin, 1);

	namedWindow("origin", CV_WINDOW_AUTOSIZE);
	namedWindow("hh", CV_WINDOW_AUTOSIZE);
	namedWindow("vv", CV_WINDOW_AUTOSIZE);

	imshow("origin", src);
	imshow("hh", hHisto);
	imshow("vv", vHisto);
}

Mat afficheHistogrammeProjectionHorizontal(vector<int> hist) {
	vector<int>::iterator it = max_element(hist.begin(), hist.end());
	int w =  *(it) + 10, h = hist.size();

	Mat histogram = Mat::zeros(Size(w, h), CV_8U);

	for (int i = 0; i < h; i++) {
		int v = w - hist.at(i);
		for (int j = 0; j < v; j++) {
			histogram.at< unsigned char >(i, j) = 255;
		}
	}
	return histogram;
}

Mat binarisation(Mat origine, int type, int filtre) {

	Mat binaire;

	if (filtre = 1)
		GaussianBlur(origine, binaire, Size(5, 5), 0);
	else
		if (filtre = 2)
			medianBlur(origine, binaire, 3);
		else
			origine.copyTo(binaire);
	

	if(type == 0)
		adaptiveThreshold(origine, binaire, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 21, 2);
	if(type == 1)
		adaptiveThreshold(origine, binaire, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 21, 2);
	if(type == 2)
		threshold(origine, binaire, 0, 255, THRESH_BINARY | THRESH_OTSU);
	if (type == 3)
		threshold(origine, binaire, 150, 255, THRESH_BINARY);
	

	return binaire;
}

Mat normalisation(Mat origine, int hauteurNorm) {
	Mat rot_mat, finale;

	int hauteur = origine.rows;

	double scale = ((double)hauteurNorm) / ((double)hauteur);
	cout << "scale = " << scale << endl;

	int largeurNorm = origine.cols * scale;

	Mat dest;
	resize(origine, dest, Size(largeurNorm, hauteurNorm));

	return dest;
}

Mat operationsMorphologiques(Mat origine) {
	Mat opening, closing, eroded, delated;
	
	/*
	types:
		0 -> RECT
		1 -> CROSS
		2 -> ELIPSE
	*/

	int typeErode = 0;
	Mat elementErode = getStructuringElement(typeErode, Size(4, 4), Point(0, 0));

	erode(origine, eroded, elementErode);
	

	int typeDilation = 0;
	Mat elementDelate = getStructuringElement(typeErode, Size(4, 4), Point(0, 0));

	dilate(origine, delated, elementDelate);
	

	dilate(eroded, opening, elementDelate);
	
	Mat opning_bin;
	//threshold(opening, opning_bin, 200, 255, THRESH_BINARY);
	adaptiveThreshold(opening, opning_bin, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 21, 2);

	erode(delated, closing, elementErode);
	

	Mat closing_bin;
	//threshold(closing, closing_bin, 254, 255, THRESH_BINARY);
	adaptiveThreshold(closing, closing_bin, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 21, 2);

	Mat d_bin;
	//threshold(delated, d_bin, 254, 255, THRESH_BINARY);
	adaptiveThreshold(delated, d_bin, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 21, 2);

	Mat bin_closing;
	Mat elementErode_bin = getStructuringElement(typeErode, Size(3, 3), Point(0, 0));
	erode(d_bin, bin_closing, elementErode_bin);

	Mat bin_closing_bin;
	//threshold(bin_closing, bin_closing_bin, 1, 255, THRESH_BINARY);
	adaptiveThreshold(bin_closing, bin_closing_bin, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 21, 2);
	
	
	//imshow("source", origine);
	//imshow("eroded", eroded);
	//imshow("delated", delated);
	//imshow("opning", opening);
	//imshow("opning_bin", opning_bin);
	//imshow("closing", closing);
	//imshow("closing_bin", closing_bin);
	//imshow("delated binaire", d_bin);
	//imshow("bin_closing", bin_closing);
	//imshow("bin_closing_bin", bin_closing_bin);
	

	//return bin_closing_bin;
	//return bin_closing;
	//return closing;
	return closing_bin;
	//return opning;
	//return opning_bin;
}

Mat unsharpFiltre(Mat origine) {
	Mat blurred, blurred1;
	double thresholdBilateral = 2, amountBilateral = 8;
	int winSize = 17, segmaColor = 80, segmaSpace = 80;
	double sigmaUnsharp = 1, thresholdUnsharp = 5, amountUnsharp = 1;

	// using BelateralFilter
	Mat bFilter;
	bilateralFilter(origine, bFilter, winSize, segmaColor, segmaSpace);
	Mat lowConstrastMask1 = abs(origine - bFilter) < thresholdBilateral;
	Mat sharpened1 = origine*(1 + amountBilateral) + bFilter*(-amountBilateral);
	origine.copyTo(sharpened1, lowConstrastMask1);

	// using GaussianBlure
	GaussianBlur(origine, blurred, Size(), sigmaUnsharp, sigmaUnsharp);
	Mat lowConstrastMask = abs(origine - blurred) < thresholdUnsharp;
	Mat sharpened = origine*(1 + amountUnsharp) + blurred*(-amountUnsharp);
	origine.copyTo(sharpened, lowConstrastMask);

	/*
	imshow("origine", origine);

	imshow("blurred", blurred);
	imshow("lowConstrastMask", lowConstrastMask);

	imshow("bFilter", bFilter);
	imshow("lowConstrastMask1", lowConstrastMask1);

	imshow("sharpened", sharpened);
	imshow("sharpened1", sharpened1);
	*/

	return sharpened;
}

Mat unsharpFiltre2(Mat origine) {
	Mat blured, sharped;
	GaussianBlur(origine, blured, cv::Size(0, 0), 3);
	addWeighted(origine, 3, blured, -2, 0, sharped);
	return sharped;
}



Mat filtrePersonnel(Mat src, Mat& dest, int segma, Size winSize) {
	int w = src.cols, h = src.rows;
	src.copyTo(dest);

	for (int i = 0; i < h; i++){
		for (int j = 0; j < w; j++) {
			int minRow = i - winSize.height < 0 ? 0 : i - winSize.height;
			int maxRow = i + winSize.height > h ? h : i + winSize.height;
			
			int minCol = j - winSize.width < 0 ? 0 : j - winSize.width;
			int maxCol = j + winSize.width > w ? w : j + winSize.width;

			int value = src.at<unsigned char>(i, j) + segma;

			int k = minRow;
			while (k < h && k < maxRow) {
				int l = minCol;
				while (l < w && l < maxCol) {
					if (src.at<unsigned char>(k, l) > value)
						dest.at<unsigned char>(k, l) = 255;
					l++;
				}
				k++;
			}
		}
	}

	return dest;
}