#include "DataSetGenerator.h"

Composant nouveauComposant(string file) {
	Mat src = imread(file, 0);
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
	c.setData(src);
	c.debutX = startX;
	c.finX = finX;
	c.debutY = startY;
	c.finY = finY;
	c.setContourExterne();
	return c;
}


void genererDataSetImagesV2(string carsFolder, string outImagesFolder) {

	cout << "tapez 1 a 9 pour les caracteres" << endl;
	cout << "tapez n pour les non caracteres" << endl;
	cout << "tapez b pour les caracteres chevauches" << endl;

	map<string, string> classes;

	classes["0"] = "c";
	classes["1"] = "c";
	classes["2"] = "c";
	classes["3"] = "c";
	classes["4"] = "c";
	classes["5"] = "c";
	classes["6"] = "c";
	classes["7"] = "c";
	classes["8"] = "c";
	classes["9"] = "c";
	classes["n"] = "nc";
	classes["b"] = "cv";




	cv::String path(carsFolder);
	vector<cv::String> fn;
	cv::glob(path, fn, true);
	for (size_t k = 131; k < fn.size(); ++k)
	{
		cv::Mat src = cv::imread(fn[k], 0);

		if (src.empty()) continue;

		Selector selector;

		cv::Point* points = selector.getPlaqueCorners(src);
		if (points == NULL)return;
		int s = 0;
		for (size_t i = 0; i < 4; i++)
		{
			s += points[i].x;
		}
		if (s == 0)continue;

		Mat crop = extractionDePlaque(src, points);
		Mat rotCrop = correctionRotation(crop, points);
		Mat ajustee = correctionInclinaisonEliminerBordure(rotCrop);
		Mat sharped;
		sharped = unsharpFiltre2(ajustee);
		Mat normalisee = normalisation(sharped, 50);
		Mat binaire = binarisation(normalisee, B_G_OTSU, No_Blur);
		Mat resultat = operationsMorphologiques(binaire);
		imwrite("output/img.png", resultat);
		Mat plaque = imread("output/img.png");

		vector<Composant> composants = segmentationACC(resultat);

		for (int i = 0; i < composants.size(); i++) {

			Point pt1(composants.at(i).debutX, composants.at(i).debutY), pt2(composants.at(i).finX, composants.at(i).finY);
			Mat img = plaque.clone();
			rectangle(img, pt1, pt2, Scalar(0, 0, 255));
			imshow("src", img);

			string classe;
			int c = waitKey(0);
			if (c != 27) {
				stringstream ss;
				ss << char(c);

				if (classes.find(ss.str()) == classes.end()) {
					cout << "entree invalide !" << endl;
					destroyWindow("src");
					continue;
				}

				classe = classes[ss.str()];

				stringstream sName;
				sName << classe;

				if (classe == "c")
					sName << ss.str();

				sName << "-";
				sName << k;
				sName << i;
				sName << ".png";

				string outImage = outImagesFolder + "/" + sName.str();

				imwrite(outImage, composants.at(i).data);

			}
			else {
				destroyWindow("src");
				return;
			}
			destroyWindow("src");

		}
	}

}

void genererDataSetAttributsTxt(string imagesFolder, string labelsFile, string dataFile, vector<string> attributs, bool type) {
	cv::String path(imagesFolder);
	vector<cv::String> fn;
	cv::glob(path, fn, true);

	ofstream donnees(dataFile);
	ofstream etiq(labelsFile);

	if (type) {
		for (size_t k = 0; k < fn.size(); ++k) {
			istringstream filePath(fn[k]);
			string name;
			getline(filePath, name, '\\');
			getline(filePath, name, '\\');

			char classe = name[0];
			if (classe == 'c') {
				classe = name[1];
				cout << name << " : " << classe << endl;

				Composant c = nouveauComposant(fn[k]);

				map<string, double> data;
				for (int j = 0; j < attributs.size(); j++) {
					data[attributs.at(j)] = c.getAttribut(attributs.at(j));
				}

				stringstream ss;
				ss << data[attributs.at(0)];
				for (int j = 1; j < attributs.size(); j++) {
					ss << " ";
					ss << data[attributs.at(j)];
				}

				donnees << ss.str() << endl;
				etiq << classe << endl;

			}
		}
	}
	else {
		for (size_t k = 0; k < fn.size(); ++k) {
			istringstream filePath(fn[k]);
			string name;
			getline(filePath, name, '\\');
			getline(filePath, name, '\\');

			char classe = name[0];
			cout << name << " : " << classe << endl;

			Composant c = nouveauComposant(fn[k]);

			map<string, double> data;
			for (int j = 0; j < attributs.size(); j++) {
				data[attributs.at(j)] = c.getAttribut(attributs.at(j));
			}

			stringstream ss;
			ss << data[attributs.at(0)];
			for (int j = 1; j < attributs.size(); j++) {
				ss << " ";
				ss << data[attributs.at(j)];
			}

			donnees << ss.str() << endl;
			etiq << classe << endl;
		}
	}

	donnees.close();
	etiq.close();
}

void genererDataSetAttributsXml(string imagesFolder, string labelsFile, string dataFile, vector<string> attributs, bool type) {
	// to do
}

void genererDataSetImagesXml(string imagesFolder, string labelsFile, string dataFile, vector<string> attributs, bool type) {
	// to do
}

void genererValeursAttribut(string carsFolder, string outFile, string Attribut, bool type) {

	/*

	type == true <=> generer seulement l'attributs des caractères de 0 à 9 et les ordonnés dans des dossiers
	type == false <=> generer l'attributs des: caractères(dossier : c) , non caractères (dossier : nc) et les caractères chevauchés(dossier : cv)

	*/
	map<string, string> classes;
	if (type == true) {
		classes["0"] = "0";
		classes["1"] = "1";
		classes["2"] = "2";
		classes["3"] = "3";
		classes["4"] = "4";
		classes["5"] = "5";
		classes["6"] = "6";
		classes["7"] = "7";
		classes["8"] = "8";
		classes["9"] = "9";
	}
	else {
		classes["1"] = "c";
		classes["2"] = "nc";
		classes["3"] = "cv";
	}

	ofstream file(outFile);

	cv::String path(carsFolder);
	vector<cv::String> fn;
	cv::glob(path, fn, true);
	for (size_t k = 0; k < fn.size(); ++k)
	{
		cv::Mat src = cv::imread(fn[k], 0);
		if (src.empty()) continue;

		Selector selector;

		cv::Point* points = selector.getPlaqueCorners(src);
		if (points == NULL)return;
		int s = 0;
		for (size_t i = 0; i < 4; i++)
		{
			s += points[i].x;
		}
		if (s == 0)continue;

		Mat crop = extractionDePlaque(src, points);
		Mat rotCrop = correctionRotation(crop, points);
		Mat ajustee = correctionInclinaisonEliminerBordure(rotCrop);
		Mat sharped;
		sharped = unsharpFiltre2(ajustee);
		Mat normalisee = normalisation(sharped, 50);
		Mat binaire = binarisation(normalisee, B_G_OTSU, No_Blur);
		Mat resultat = operationsMorphologiques(binaire);


		vector<Composant> composants = segmentationACC(resultat);

		for (int i = 0; i < composants.size(); i++) {
			/*
			map<string, double> data;

			data["ratio"] = composants.at(i).getRatio();
			data["densite"] = composants.at(i).getDensite();
			data["ph"] = composants.at(i).getPortionHauteur();
			data["m00"] = composants.at(i).getM00();
			data["m10"] = composants.at(i).getM10();
			data["m01"] = composants.at(i).getM01();
			data["m11"] = composants.at(i).getM11();
			data["m02"] = composants.at(i).getM02();
			data["m20"] = composants.at(i).getM20();
			data["m12"] = composants.at(i).getM12();
			data["m21"] = composants.at(i).getM21();
			data["nbrContours"] = composants.at(i).getNbrContours();
			data["surface"] = composants.at(i).getSurface();
			data["perimetre"] = composants.at(i).getPerimeter();
			data["yCentreDeMasse"] = composants.at(i).getYcentreDeMasse();
			*/

			imshow("src", composants.at(i).data);

			string classe;
			int c = waitKey(0);
			if (c != 27) {
				stringstream ss;
				ss << char(c);


				if (classes.find(ss.str()) == classes.end()) {
					cout << "entree invalide !" << endl;
					destroyWindow("src");
					continue;
				}

				classe = classes[ss.str()];

				//file << classe << ":  " << data[Attribut] << endl;
				file << classe << ":  " << composants.at(i).getAttribut(Attribut) << endl;
			}
			else {
				destroyWindow("src");
				file.close();
				return;
			}

			destroyWindow("src");

		}
		waitKey(0);
	}
}



//transformation des donnees TXT au XML pour les classifieurs :

int dataAttributs_TxtFileToXml(string dataSetD, string dataSetL, string dataFile, string labelsFile) {
	ifstream in_state_data(dataSetD.c_str(), ifstream::in);
	ifstream in_state_labels(dataSetL.c_str(), ifstream::in);

	Mat labels;
	Mat data;

	string dataComposant;
	string labelComposant;
	while (getline(in_state_data, dataComposant) && getline(in_state_labels, labelComposant)) {
		istringstream in_data(dataComposant);
		double value;
		Mat dataC;
		while (in_data >> value) {
			dataC.push_back(value);
		}
		Mat dataCFloat;
		dataC.convertTo(dataCFloat, CV_32FC1);
		Mat dataCFlattenedFloat = dataCFloat.reshape(1, 1);

		data.push_back(dataCFlattenedFloat);

		labels.push_back(stoi(labelComposant));

	}

	FileStorage fsClassifications(labelsFile, FileStorage::WRITE);
	if (fsClassifications.isOpened() == false) {
		std::cout << "error, unable to open training classifications file, exiting program\n\n";
		return -1;
	}
	fsClassifications << "labels" << labels;
	fsClassifications.release();

	cv::FileStorage fsTrainingImages(dataFile, FileStorage::WRITE);

	if (fsTrainingImages.isOpened() == false) {
		std::cout << "error, unable to open training images file, exiting program\n\n";
		return -1;
	}
	fsTrainingImages << "data" << data;
	fsTrainingImages.release();

	return 0;
}

int dataImageChiffres_ImgToXml(string folder, string dataFile, string labelsFile, int WIDTH, int HEIGHT) {
	Mat labels;
	Mat data;

	for (int i = 0; i < 10; i++) {
		stringstream ss;
		ss << i;
		string source = folder + "/" + ss.str();
		vector<cv::String> fn;
		glob(source, fn, true);

		for (size_t k = 0; k < fn.size(); ++k) {
			Mat src = imread(fn[k], CV_8U);
			Mat resized;
			cv::resize(src, resized, Size(WIDTH, HEIGHT));
			labels.push_back(i);
			Mat imageFloat;
			resized.convertTo(imageFloat, CV_32FC1);
			Mat imageFlattenedFloat = imageFloat.reshape(1, 1);
			data.push_back(imageFlattenedFloat);
		}
	}

	FileStorage fsClassifications(labelsFile, FileStorage::WRITE);
	if (fsClassifications.isOpened() == false) {
		std::cout << "error, unable to open training classifications file, exiting program\n\n";
		return -1;
	}
	fsClassifications << "labels" << labels;
	fsClassifications.release();

	cv::FileStorage fsTrainingImages(dataFile, FileStorage::WRITE);

	if (fsTrainingImages.isOpened() == false) {
		std::cout << "error, unable to open training images file, exiting program\n\n";
		return -1;
	}
	fsTrainingImages << "data" << data;
	fsTrainingImages.release();

	return 0;
}


// Version 1 :

void genererDataSetAttributs(string carsFolder,string labelsFile, string dataFile, vector<string> attributs, bool type) {
	/*

	type == true <=> generer seulement les attributs des caractères de 0 à 9 et les ordonnés dans des dossiers
	type == false <=> generer les attributs caractères(dossier : c) , non caractère (dossier : nc) et les caractères chevauchés(dossier : cv)

	*/

	map<string, string> classes;
	if (type == true) {
		classes["0"] = "0";
		classes["1"] = "1";
		classes["2"] = "2";
		classes["3"] = "3";
		classes["4"] = "4";
		classes["5"] = "5";
		classes["6"] = "6";
		classes["7"] = "7";
		classes["8"] = "8";
		classes["9"] = "9";
	}
	else {
		classes["1"] = "c";
		classes["2"] = "nc";
		classes["3"] = "cv";
	}

	
	ofstream donnees(dataFile);
	ofstream etiq(labelsFile);
	//donnees << "ratio densite ph m00 m10 m01 m11 m02 m20 m12 m21 nbrContours surface perimetre yCentreDeMasse" << endl;
	//etiq << "classe" << endl;

	cv::String path(carsFolder);
	vector<cv::String> fn;
	cv::glob(path, fn, true);
	for (size_t k = 0; k < fn.size(); ++k)
	{
		cv::Mat src = cv::imread(fn[k], 0);
		if (src.empty()) continue;

		Selector selector;

		cv::Point* points = selector.getPlaqueCorners(src);
		if (points == NULL)return;
		int s = 0;
		for (size_t i = 0; i < 4; i++)
		{
			s += points[i].x;
		}
		if (s == 0)continue;

		Mat crop = extractionDePlaque(src, points);
		Mat rotCrop = correctionRotation(crop, points);
		Mat ajustee = correctionInclinaisonEliminerBordure(rotCrop);
		Mat sharped;
		sharped = unsharpFiltre2(ajustee);
		Mat normalisee = normalisation(sharped, 50);
		Mat binaire = binarisation(normalisee, B_G_OTSU, No_Blur);
		Mat resultat = operationsMorphologiques(binaire);
		

		vector<Composant> composants = segmentationACC(resultat);
		
		for (int i = 0; i < composants.size(); i++) {

			map<string, double> data;
			for (int j = 0; j < attributs.size(); j++) {
				data[attributs.at(j)] = composants.at(i).getAttribut(attributs.at(j));
			}

			imshow("src", composants.at(i).data);

			string classe;
			int c = waitKey(0);
			if (c != 27) {
				stringstream ss;
				ss << char(c);


				if(classes.find(ss.str()) == classes.end()){
					cout << "entree invalide !" << endl;
					destroyWindow("src");
					continue;
				}

				classe = classes[ss.str()];

				ss.clear();
				ss << data[attributs.at(0)];
				for (int j = 1; j < attributs.size(); j++) {
					ss << " ";
					ss << data[attributs.at(j)];
				}
				
				
				donnees << ss.str() << endl;
				etiq << classe << endl;
			}
			else {
				destroyWindow("src");
				donnees.close();
				etiq.close();
				return;
			}
			destroyWindow("src");

		}
		waitKey(0);
	}

}

void genererDataSetImages(string carsFolder, string outImagesFolder, bool type) {

	/*

	type == true <=> generer seulement les images des caractères de 0 à 9 et les ordonnés dans des dossiers
	type == false <=> generer les images caractères(dossier : c) , non caractère (dossier : nc) et les caractères chevauchés(dossier : cv)

	*/

	map<string, string> classes;
	if (type == true) {
		classes["0"] = "0";
		classes["1"] = "1";
		classes["2"] = "2";
		classes["3"] = "3";
		classes["4"] = "4";
		classes["5"] = "5";
		classes["6"] = "6";
		classes["7"] = "7";
		classes["8"] = "8";
		classes["9"] = "9";
	}
	else {
		classes["1"] = "c";
		classes["2"] = "nc";
		classes["3"] = "cv";
	}


	cv::String path(carsFolder);
	vector<cv::String> fn;
	cv::glob(path, fn, true);
	for (size_t k = 0; k < fn.size(); ++k)
	{
		cv::Mat src = cv::imread(fn[k], 0);

		if (src.empty()) continue;

		Selector selector;

		cv::Point* points = selector.getPlaqueCorners(src);
		if (points == NULL)return;
		int s = 0;
		for (size_t i = 0; i < 4; i++)
		{
			s += points[i].x;
		}
		if (s == 0)continue;

		Mat crop = extractionDePlaque(src, points);
		Mat rotCrop = correctionRotation(crop, points);
		Mat ajustee = correctionInclinaisonEliminerBordure(rotCrop);
		Mat sharped;
		sharped = unsharpFiltre2(ajustee);
		Mat normalisee = normalisation(sharped, 50);
		Mat binaire = binarisation(normalisee, B_G_OTSU, No_Blur);
		Mat resultat = operationsMorphologiques(binaire);


		vector<Composant> composants = segmentationACC(resultat);

		for (int i = 0; i < composants.size(); i++) {

			Mat img = composants.at(i).data;
			imshow("src", img);

			string classe;
			int c = waitKey(0);
			if (c != 27) {
				stringstream ss;
				ss << char(c);

				if (classes.find(ss.str()) == classes.end()) {
					cout << "entree invalide !" << endl;
					destroyWindow("src");
					continue;
				}

				classe = classes[ss.str()];

				ss << "-";
				ss << k;
				ss << i;
				ss << ".png";

				string outImage = outImagesFolder + "/" + classe + "/" + ss.str();

				imwrite(outImage, img);

			}
			else {
				destroyWindow("src");
				return;
			}
			destroyWindow("src");

		}
		waitKey(0);
	}

}

void organiserDataSetCaracteres(string dataSetSource, string destinationFolder) {

	map<string, string> classes;
		classes["0"] = "0";
		classes["1"] = "1";
		classes["2"] = "2";
		classes["3"] = "3";
		classes["4"] = "4";
		classes["5"] = "5";
		classes["6"] = "6";
		classes["7"] = "7";
		classes["8"] = "8";
		classes["9"] = "9";

	cv::String path(dataSetSource);
	vector<cv::String> fn;
	cv::glob(path, fn, true);
	for (size_t k = 0; k < fn.size(); ++k)
	{
		Mat src = imread(fn[k], 0);

		if (src.empty()) continue;

		imshow("src", src);

		string classe;
		int c = waitKey(0);
		if (c != 27) {
			stringstream ss;
			ss << char(c);

			if (classes.find(ss.str()) == classes.end()) {
				cout << "entree invalide !" << endl;
				destroyWindow("src");
				continue;
			}

			classe = classes[ss.str()];

			ss << "-";
			ss << k;
			ss << ".png";

			string outImage = destinationFolder + "/" + classe + "/" + ss.str();

			imwrite(outImage, src);

			destroyWindow("src");
		}
	}

}


