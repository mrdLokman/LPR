#ifndef CLASSIFIEUR_BAYESIEN_H
#define CLASSIFIEUR_BAYESIEN_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>
#include <map>
#include "Composant.h"

using namespace std;

class CBN
{
public:
	CBN();
	~CBN();

	int nbr_attributs;
	int nbr_classes;

	vector <string> label_classes;
	vector <string> label_attributs;

	map <string, double> proba_apriorie;
	map <string, map<string, vector<double>>> proba_conditionnelles;
	map <string, vector<double>> intervals_attributs;

	void apprentissage(vector<vector<double> > data, vector<string>  labels);

	string prediction(map<string, int> instance);
	void classifier(map<string, int> instance, Composant& c, double seuil);

	void chargerReseau(string file_name);

	map<string, int> interval(map<string, double> data);

};

#endif