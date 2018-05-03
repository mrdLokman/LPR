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

vector<int> positionPossible(Mat data) {
	vector<bool> posIntermds;
	vector<int> posFinales;
	int h = data.rows, w = data.cols;

	for (int i = 0; i < w; i++) {
		Mat col = data.col(i);
		int intersection = 0;
		int j = 0;
		while (j < h) {
			while (j < h && col.at<unsigned char>(j) == 255) j++;
			if (j < h) intersection++;
			while (j < h && col.at<unsigned char>(j) == 0) j++;
		}
		if (intersection <= 1)
			posIntermds.push_back(true);
		else
			posIntermds.push_back(false);
	}

	int i = 0, s = posIntermds.size();
	int d = 0, f = s - 1;
	while (i < s) {
		while (d < s && !posIntermds.at(i)) i++;
		if (i < s) {
			d = i;
			while (i < s && posIntermds.at(i))i++;
			f = i;
			/*if (d != 0) {
				posFinales.push_back(d);
				if (f != 0 && f != d) {
					posFinales.push_back((f + d) / 2);
					if(f < s)
						posFinales.push_back(f);
				}
			}
			else {
				if( f != 0) {
					posFinales.push_back((f + d) / 2);
					if (f < s)
						posFinales.push_back(f);
				}

			}*/
			
			if((f + d) / 2 != 0)
			posFinales.push_back((f + d) / 2);
			
		}
	}
	return posFinales;
}

Composant creerComposant(Composant c, int d, int f) {
	Composant sc;

	Mat m(c.data, Rect(d, 0, f - d + 1, c.data.rows));
	m = elargir(m);
	m.copyTo(sc.data);

	int h = m.rows, w = m.cols, startY = 0, finY = h - 1;

	int i = 0;
	while (i < h) {
		int j = 0;
		while (j < w && m.at<unsigned char>(i, j) != 0) j++;

		if (j < w) {
			startY = i;
			break;
		}
		i++;
	}

	i = h - 1;
	while (i >= 0) {
		int j = 0;
		while (j < w && m.at<unsigned char>(i, j) != 0) j++;

		if (j < w) {
			finY = i;
			break;
		}
		i--;
	}


	sc.ordreHorizontal = 0;
	sc.debutX = c.debutX + d;
	sc.finX = c.debutX + f;
	sc.debutY = startY;
	sc.finY = finY;
	sc.decision = false;
	sc.setContourExterne();


	return sc;
}

vector<Composant> separartionComposantsChevauches(Composant composant,int largeurMax, int hauteurMoy, double EpaisseurMoy, CBN cbn) {
	vector<Composant> sousComposants;
	int width = composant.data.cols;

	vector<int> positions = positionPossible(composant.data);
	//std::cout << "nbr position de coupure possibles : " << positions.size() << endl << endl;

	int debut = 0, fin = positions.at(0), s = positions.size(), i = 0, j = 1;

	/*
	if (fin == 0) {
		i = 1;
		j = 2;
		fin = positions.at(1);
	}
	*/

	while (i < s) {
		// creation composant
		Composant c = creerComposant(composant, debut, fin);
		imshow("c", c.data); 

		//classification composant
		map<string, double> data;
		data["ratio"] = c.getRatio();
		data["densite"] = c.getDensite();
		data["ph"] = c.getPortionHauteur();
		data["nbrContours"] = c.getNbrContours();
		
		if(hauteurMoy != -1)
			data["hr"] = c.getHauteurRelative(hauteurMoy);
		/*
		if (EpaisseurMoy != -1)
			data["emr"] = c.getEpaisseurMoyenneRelative(EpaisseurMoy);
		*/
		map<string, int> instance = cbn.interval(data);
		cbn.classifier(instance, c, 0.85);

		AfficheMap(c.probabilites_classes);

		if (c.probabilites_classes["caractere"] > 0.85) {
			std::cout << "decide charactere" << endl << endl;
			sousComposants.push_back(c);

			if (j < s) {
				debut = fin;
				i = j;
				fin = positions[j];
				j++;
			}
			else {
				i = j;
				debut = fin;
				fin = width - 1;
			}
		}
		else {
			cout << "n'est pas un charactere" << endl << endl;
			if (c.getLargeur() <= largeurMax) {
				if (j < s) {
					fin = positions[j];
					j++;
				}
				else {
					i = j;
				}
			}
			else {
				i++;
				if (i < s) {
					debut = positions[i];
					j = i + 1;
					if(j < s)
						fin = positions[j];
				}
			}
		}
		waitKey(0);
		destroyWindow("c");
	}
	return sousComposants;
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

			map<string, int> instance = cbn.interval(data);
			cbn.classifier(instance, composants.at(i), seuil);
			
			AfficheMap(composants.at(i).probabilites_classes);
			cout << endl;

			imshow("img", composants.at(i).data);
			waitKey(0);
			destroyWindow("img");
			

		}

		system("pause");

		int nbrCharChev = 0,nbrChar = 0;
		double HauteurMoy = 0,EpaisseurMoy = 0;
		for (int i = 0; i < composants.size(); i++) {
			if (composants.at(i).estDecide() && (composants.at(i).probabilites_classes["caractere"] > seuil || composants.at(i).probabilites_classes["chevauches"] > seuil)) {
				HauteurMoy += composants.at(i).getHauteur();
				if (composants.at(i).probabilites_classes["caractere"] > seuil) {
					EpaisseurMoy += composants.at(i).getEpaisseurMoyenne();
					nbrChar++;
				}
				nbrCharChev++;
			}
		}

		cout << endl << "nbr decides caractere+chevauches " << nbrCharChev << " , nbr decides caracteres " << nbrChar << endl << endl;

		if (nbrCharChev != 0) {
			HauteurMoy = HauteurMoy / nbrCharChev;

			for (int i = 0; i < composants.size(); i++) {
				if (!composants.at(i).estDecide()) {

					data["ratio"] = composants.at(i).getRatio();
					data["densite"] = composants.at(i).getDensite();
					data["ph"] = composants.at(i).getPortionHauteur();
					data["nbrContours"] = composants.at(i).getNbrContours();
					data["hr"] = composants.at(i).getHauteurRelative(HauteurMoy);
					
					if (nbrChar != 0) {
						EpaisseurMoy = EpaisseurMoy / nbrChar;
						//data["emr"] = composants.at(i).getEpaisseurMoyenneRelative(EpaisseurMoy);
					}

					map<string, int> instance = cbn.interval(data);
					cbn.classifier(instance, composants.at(i), seuil);
					
					AfficheMap(composants.at(i).probabilites_classes);
					cout << endl;

					imshow("img", composants.at(i).data);
					waitKey(0);
					destroyWindow("img");
					
				}
			}
		}

		nbrChar = 0;
		nbrCharChev = 0;
		HauteurMoy = 0.0;
		EpaisseurMoy = 0.0;
		vector<Mat> chars;
		double largeurMax = 0;
		for (int i = 0; i < composants.size(); i++) {
			if (composants.at(i).estDecide() && (composants.at(i).probabilites_classes["caractere"] > seuil || composants.at(i).probabilites_classes["chevauches"] > seuil)) {
				if (composants.at(i).probabilites_classes["caractere"] > seuil) {
					if (largeurMax < composants.at(i).getLargeur())
						largeurMax = composants.at(i).getLargeur();
					nbrChar++;
					EpaisseurMoy += composants.at(i).getEpaisseurMoyenne();
					chars.push_back(composants.at(i).data);
				}
				HauteurMoy += composants.at(i).getHauteur();
				nbrCharChev++;
			}
		}

		imshow("Resultat-1", display_images(chars, 1000, 6));
		cout << "nbr caractere est : " << nbrChar << endl << endl;
		waitKey(0);

		if (nbrChar != 0) {
			EpaisseurMoy = EpaisseurMoy / nbrChar;
		}
		else {
			EpaisseurMoy = -1;
		}

		if (nbrCharChev) {
			HauteurMoy = HauteurMoy / nbrCharChev;
		}
		else {
			HauteurMoy = -1;
		}

		for (int i = 0; i < composants.size(); i++) {
			if (composants.at(i).probabilites_classes["chevauches"] > 0.5) {
				
				imshow("chevauche", composants.at(i).data);
				waitKey(0);

				if (nbrChar <= 0)
					largeurMax = composants.at(i).getLargeur();

				vector<Composant> sousComposants = separartionComposantsChevauches(composants.at(i),largeurMax, HauteurMoy, EpaisseurMoy, cbn);

				if (sousComposants.size() != 0) {
					for (int i = 0; i < sousComposants.size(); i++) {
						nbrChar++;
						chars.push_back(sousComposants.at(i).data);
						/*
						AfficheMap(sousComposants.at(i).probabilites_classes);
						std::cout << endl;

						imshow("img", sousComposants.at(i).data);
						waitKey(0);
						destroyWindow("img");
						*/					
					}
				}

				//waitKey(0);
				destroyWindow("chevauche");
			}
		}
		imshow("Resultat-2", display_images(chars, 1000, 6));
		cout << endl << "nbr caractere est : " << nbrChar << endl;

		finSegmentation = testFinSegmentation();
	}

	return 0;
}

