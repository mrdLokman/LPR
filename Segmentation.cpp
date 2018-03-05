#include"Segmentation.h"


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
			c.hauteur = finY - startY + 1;
			c.largeur = finX - startX + 1;
			c.ordreHorizontal = composants.size() + 1;
			c.positionX = startX;
			c.positionY = startY;
			Mat m(plaque, Rect(startX, startY, c.largeur, c.hauteur));
			m.copyTo(c.data);

			composants.push_back(c);
		}


		j++;
	}

	return composants;
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

	c.hauteur = finY - startY + 1;
	c.largeur = finX - startX + 1;
	c.ordreHorizontal = 0;
	c.positionX = startX;
	c.positionY = startY;
	Mat m(src, Rect(startX, startY, c.largeur, c.hauteur));
	m.copyTo(c.data);


	return c;
}

vector<Composant> segmentationACC(Mat plaque) {
	vector<Composant> composants;
	int w = plaque.cols, h = plaque.rows;
	Mat bw = plaque < 250;
	Mat labelImage(plaque.size(), CV_32S);
	int nLabels = connectedComponents(bw, labelImage, 4);
	cout << "nombre de composantes connexes : " << nLabels - 1 << endl;

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
		composants.push_back(c);
	}

	return composants;
}
