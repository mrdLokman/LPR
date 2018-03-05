#ifndef CLASSIFIER_H
#define CLASSIFIER_H
#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>
#include <map>

using namespace std;

class ReseauBayesien {
public:
	
    map <string, double> proba_classes;
    map <string, vector<vector<double>>> proba_conditionnelles;

    vector <string> label_classes;
    int nbr_attributs;
	int nbr_classes;

    ReseauBayesien();
    virtual ~ReseauBayesien();

    void apprentissage(vector<vector<double> > data, vector<string>  labels);

    string prediction(vector<int> instance);

	void chargerApprentissage(string file_name);

};

#endif



