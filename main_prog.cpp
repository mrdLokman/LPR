#include "Tests.h"
#include "Pretraitement.h"


void test() {
	Mat src = imread("../input/test.png", 0);
	vector<int> h = histogrammeProjectionHorizontal(src);
	
	int taille = h.size();
	cout << "taille = " << taille <<endl;
	int moy = moyenne(h);
	cout << "moyenne = " << moy << endl;
	int minG = minGauche(h, moy);
	cout << "minGauche = " << minG << endl;
	int minD = minDroit(h, moy);
	cout << "minDroit = " << minD << endl;

	int w = src.cols;
	for (int i = 0; i < w; i++)
	{
		src.at<unsigned char>(minG, i) = 128;
		src.at<unsigned char>(minD, i) = 128;
	}

	imshow("resultat", src);
	imwrite("../input/resultat.png",src);
}


int main1()
{
	
	testOprsMorph("input/binaire.png");

	waitKey(0);
	return 0;
}
