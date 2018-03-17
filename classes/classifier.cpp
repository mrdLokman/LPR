#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>
#include <map>
#include <assert.h>
#include "classifier.h"

using namespace std;


ReseauBayesien::ReseauBayesien() {

}

ReseauBayesien::~ReseauBayesien() {}

void ReseauBayesien::apprentissage(vector<vector<double>> data, vector<string> labels)
{  
    
}

string ReseauBayesien::prediction(vector <int> instance)
{
    
    map <string, double> proba_aposteriorie;
	map <string, double> proba_instanceSachantClasse;

    double max = 0;
	double somme = 0;
    string resultat;

	//factorisation = calcule de probabilite d'un vecteur sachant les classes

	for (auto lab : label_classes) {
		proba_instanceSachantClasse[lab] = 1.0;
		for (int i = 0; i < nbr_attributs;i++){
			proba_instanceSachantClasse[lab] = proba_instanceSachantClasse[lab] * proba_conditionnelles[lab].at(i).at(instance.at(i));
		}
	}

	//proba apriorie

	somme = 0;
	for (auto lab : label_classes) {
		proba_aposteriorie[lab] = proba_classes[lab] * proba_instanceSachantClasse[lab];
		somme += proba_aposteriorie[lab];
	}

	for (auto lab : label_classes) {
		proba_aposteriorie[lab] = proba_aposteriorie[lab] / somme;
	}

	// prediction max des probas

    for (auto lab:label_classes) {
		cout << lab << " : proba est : " << proba_aposteriorie[lab] << endl;
        if (max < proba_aposteriorie[lab]) {
            max = proba_aposteriorie[lab];
            resultat = lab;
        }
    }
    return resultat;
}

void ReseauBayesien::chargerApprentissage(string file_name) {
	ifstream in_state_(file_name.c_str(), ifstream::in);

	vector< vector<double >> state_out;
	string str;

	//nombre des classes
	in_state_ >> nbr_classes;
	//cout << " nombre de classes  " << nbr_classes << endl;
	in_state_.ignore();
	//label et proba des classes
	for (int i = 0; i < nbr_classes; i++) {
		string nomClasse;
		getline(in_state_, nomClasse);
		label_classes.push_back(nomClasse);

		double probaClasse;
		in_state_ >> probaClasse;
		proba_classes[nomClasse] = probaClasse;
		in_state_.ignore();

		//cout << "La classe : " << nomClasse << " ,proba : " << probaClasse << endl;

	}
	//cout << endl;

	//nombre des attributs
	in_state_ >> nbr_attributs;
	//cout << " nombre des attributs  " << nbr_attributs << endl;
	in_state_.ignore();

	for (int c = 0; c < nbr_classes; c++) {

		vector < vector<double>> probaASC;
		for (int a = 0; a < nbr_attributs; a++) {

			vector<double> probaASCA;
			getline(in_state_, str);
			istringstream ss(str);
			double val;
			ss >> val;

			probaASCA.push_back(val);

			string value;

			while (getline(ss, value, ',')) {
				double b;
				ss >> b;
				probaASCA.push_back(b);
			}

			probaASC.push_back(probaASCA);
		}

		proba_conditionnelles[label_classes.at(c)] = probaASC;
	}

}

vector<int> ReseauBayesien::interval(vector<double> data) {
	int size = data.size();
	vector<int> instance(size);

	// 0 : le ratio
	if (data.at(0) == -1)
		instance.at(0) = -1;
	else {
		int interval_val = -1;
		double donnee = data.at(0);
		
		if (donnee < 0.15)
			interval_val = 0;
		else
			if (donnee < 0.3)
				interval_val = 1;
			else
				if (donnee < 0.6)
					interval_val = 2;
				else
					if (donnee < 1.0)
						interval_val = 3;
					else
						if (donnee < 1.2)
							interval_val = 4;
						else
							interval_val = 5;
		
		instance.at(0) = interval_val;
		//cout << "fct : ratio est " << donnee << ", l'interval est "<< interval_val<< endl;
	}

	// 1 : la densit�
	if (data.at(1) == -1)
		instance.at(1) = -1;
	else {
		int interval_val = -1;
		double donnee = data.at(1);
		if (donnee < 0.2)
			interval_val = 0;
		else
			if (donnee < 0.35)
				interval_val = 1;
			else
				if (donnee < 0.7)
					interval_val = 2;
				else
					if (donnee < 0.9)
						interval_val = 3;
					else
						interval_val = 4;

		instance.at(1) = interval_val;
		//cout << "fct : densite est " << donnee << ", l'interval est "<< interval_val<< endl;
	}


	return instance;
}