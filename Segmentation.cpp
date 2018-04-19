#include"Segmentation.h"

Mat elargir(Mat src) {

	Rect r(1, 1, src.cols, src.rows);
	Mat dest(Size(src.cols + 2, src.rows + 2), CV_8U, Scalar(255));
	src.copyTo(dest(r));

	return dest;
}

Composant crop(Mat src) {
	int w = src.cols, h = src.rows;

	int startX = 0, startY = 0, finX = w - 1, finY = h - 1;

	int i = 0;
	while (i < h) {
		int j = 0;
		while (j < w && src.at<unsigned char>(i, j) != 0) j++;

		if (j < w) {
			startY = i;
			break;
		}
		i++;
	}

	i = h - 1;
	while (i >= 0) {
		int j = 0;
		while (j < w && src.at<unsigned char>(i, j) != 0) j++;

		if (j < w) {
			finY = i;
			break;
		}
		i--;
	}

	i = 0;
	while (i < w) {
		int j = startY;
		while (j <= finY && src.at<unsigned char>(j, i) != 0) j++;

		if (j <= finY) {
			startX = i;
			break;
		}
		i++;
	}

	i = w - 1;
	while (i >= 0) {
		int j = startY;
		while (j <= finY && src.at<unsigned char>(j, i) != 0)j++;

		if (j <= finY) {
			finX = i;
			break;
		}
		i--;
	}

	Composant c;
	c.ordreHorizontal = 0;
	c.debutX = startX;
	c.finX = finX;
	c.debutY = startY;
	c.finY = finY;

	Mat m(src, Rect(startX, 0, finX - startX + 1, h));
	m = elargir(m);
	m.copyTo(c.data);


	return c;
}

vector<Composant> segmentationProjection(Mat plaque) {
	int w = plaque.cols;
	int h = plaque.rows;

	vector<int> projVerticale(w, 0);
	vector<Composant> composants;

	for (int i = 0; i < w; i++){
		Mat data = plaque.col(i);
		projVerticale.at(i) = h - countNonZero(data);
	}

	int j = 0;
	while (j < w) {
		while (j < w && projVerticale.at(j) == 0) j++;

		int startX = 0, finX = w - 1;

		if (j != w) {
			startX = j;
			while (j < w && projVerticale.at(j) > 0) j++;
			finX = j - 1;

			int startY = 0, finY = h - 1;

			int y = 0;
			while (y < h) {
				int i = 0;
				for (i = startX; i <= finX; i++)
					if (plaque.at<unsigned char>(y, i) == 0) break;

				if (i <= finX) {
					startY = y;
					break;
				}
				y++;
			}

			y = h - 1;
			while (y >= 0) {
				int i = 0;
				for (i = startX; i <= finX; i++)
					if (plaque.at<unsigned char>(y, i) == 0) break;

				if (i <= finX) {
					finY = y;
					break;
				}
				y--;
			}

			Composant c;
			c.ordreHorizontal = composants.size() + 1;
			c.debutX = startX;
			c.finX = finX;
			c.debutY = startY;
			c.finY = finY;

			Mat m(plaque, Rect(startX, 0, finX - startX + 1, h));
			m = elargir(m);
			m.copyTo(c.data);

			composants.push_back(c);
		}


		j++;
	}

	return composants;
}

vector<Composant> segmentationACC(Mat plaque) {
	vector<Composant> composants;
	int w = plaque.cols, h = plaque.rows;
	Mat bw = plaque < 250;
	Mat labelImage(plaque.size(), CV_32S);
	int nLabels = connectedComponents(bw, labelImage, 4);
	//cout << "nombre de composantes connexes : " << nLabels - 1 << endl;

	vector<Mat> comps(nLabels - 1);
	for (int i = 0; i < nLabels - 1; i++){
		comps.at(i) = Mat(h,w,CV_8U,Scalar(255));
	}

	for (int i = 0; i < h; i++){
		for (int j = 0; j < w; j++) {
			int l = labelImage.at<int>(i, j);
			
			if(l != 0)
				comps.at(l-1).at<unsigned char>(i, j) = 0;
		}
	}

	for (int i = 0; i < nLabels - 1; i++){
		Composant c = crop(comps.at(i));
		c.setContourExterne();
		composants.push_back(c);
	}

	return composants;
}

bool testFinSegmentation(){

	return true;
}

int AutomateSegmentation(Mat plaque) {
	CBN cbn;
	cbn.chargerReseau("data/dataR.txt");

	double seuil = 0.85;

	vector<Composant> composants = segmentationACC(plaque);
	//vector<Composant> composants = segmentationProjection(plaque);

	bool finSegmentation = false;

	while(!finSegmentation){
		map<string, double> data;

		for (int i = 0; i < composants.size(); i++) {

			data["ratio"] = composants.at(i).getRatio();
			data["densite"] = composants.at(i).getDensite();
			data["ph"] = composants.at(i).getPortionHauteur();
			data["nbrContours"] = composants.at(i).getNbrContours();

			//AfficheMap(data);

			map<string, int> instance = cbn.interval(data);
			cbn.classifier(instance, composants.at(i), seuil);
			AfficheMap(composants.at(i).probabilites_classes);
			imshow("img", composants.at(i).data);

			waitKey(0);
			destroyWindow("img");

		}

		system("pause");

		int n = 0;
		double s = 0;
		for (int i = 0; i < composants.size(); i++) {
			if (composants.at(i).estDecide() && composants.at(i).probabilites_classes["caractere"] > seuil) {
				s += composants.at(i).getHauteur();
				n++;
			}
		}

		cout << "nbr decides caractere " << n << endl << endl;

		if (n != 0) {
			s = s / n;

			for (int i = 0; i < composants.size(); i++) {
				if (!composants.at(i).estDecide()) {

					data["ratio"] = composants.at(i).getRatio();
					data["densite"] = composants.at(i).getDensite();
					data["ph"] = composants.at(i).getPortionHauteur();
					data["nbrContours"] = composants.at(i).getNbrContours();
					data["hr"] = composants.at(i).getHauteurRelative(s);

					//AfficheMap(data);

					map<string, int> instance = cbn.interval(data);
					cbn.classifier(instance, composants.at(i), seuil);
					AfficheMap(composants.at(i).probabilites_classes);
					imshow("img", composants.at(i).data);

					waitKey(0);
					destroyWindow("img");
				}
			}
		}


		finSegmentation = testFinSegmentation();
	}

	return 0;
}
