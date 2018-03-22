#include"Composant.h"


Composant::Composant() {

}

Composant::~Composant() {

}

double Composant::ratio(){
	
	if (attributs.find("ratio") == attributs.end())
		attributs["ratio"] = (finX - debutX) / (finY - debutY);
	
	return attributs["ratio"];
}

double Composant::densite(){

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
