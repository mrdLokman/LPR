#include "DataSetGenerator.h"

void genererDataSetAttributs(string carsFolder,string labelsFile, string dataFile, bool type) {
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
			composants.at(i).setContourExterne();

			data["ratio"] = composants.at(i).ratio();
			data["densite"] = composants.at(i).densite();
			data["ph"] = composants.at(i).portionHauteur();
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

				donnees << data["ratio"] << " " << data["densite"] << " " << data["ph"] << " " << data["m00"] << " " << data["m10"] << " " << data["m01"] << " " << data["m11"] << " " << data["m02"] << " " << data["m20"] << " " << data["m12"] << " " << data["m21"] << " " << data["nbrContours"] << " " << data["surface"] << " " << data["perimetre"] << " " << data["yCentreDeMasse"] << endl;
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

			map<string, double> data;
			composants.at(i).setContourExterne();
			
			data["ratio"] = composants.at(i).ratio();
			data["densite"] = composants.at(i).densite();
			data["ph"] = composants.at(i).portionHauteur();
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

				file << classe << ":  " << data[Attribut] << endl;
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

int genererDataXml(string dataSetD, string dataSetL, string dataFile, string labelsFile) {
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
