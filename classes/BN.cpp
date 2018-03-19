#include "BN.h"



CBN::CBN()
{
}
CBN::~CBN()
{
}

void CBN::apprentissage(vector<vector<double>> data, vector<string> labels){

}

string CBN::prediction(map<string, int> instance){

	map <string, double> proba_aposteriorie;
	map <string, double> proba_instanceSachantClasse;

	double max = 0;
	double somme = 0;
	string resultat;

	//factorisation = calcule de probabilite d'un vecteur sachant les classes P(A|C)
	for (auto classe : label_classes) {
		proba_instanceSachantClasse[classe] = 1.0;
		for (map<string, int>::iterator it = instance.begin(); it != instance.end(); ++it) {
			string attribut = it->first;
			int val = it->second;
			proba_instanceSachantClasse[classe] = proba_instanceSachantClasse[classe] * proba_conditionnelles[attribut][classe].at(val);
		}
	}

	//proba apriorie
	somme = 0;
	for (auto classe : label_classes) {
		proba_aposteriorie[classe] = proba_apriorie[classe] * proba_instanceSachantClasse[classe];
		somme += proba_aposteriorie[classe];
	}

	for (auto classe : label_classes) {
		proba_aposteriorie[classe] = proba_aposteriorie[classe] / somme;
	}

	// prediction max des probas
	for (auto classe : label_classes) {
		//cout << classe << " : proba est : " << proba_aposteriorie[classe] << endl;
		if (max < proba_aposteriorie[classe]) {
			max = proba_aposteriorie[classe];
			resultat = classe;
		}
	}

	return resultat;
}

void CBN::chargerReseau(string file_name){
	ifstream in_state_(file_name.c_str(), ifstream::in);
	string commentaire;
	getline(in_state_, commentaire);

	in_state_ >> nbr_classes;
	//cout << "nombre des classes est : " << nbr_classes << endl;
	getline(in_state_, commentaire);

	string liste;
	getline(in_state_, liste);
	istringstream sc(liste);
	string value;
	while (getline(sc, value, ' ')) {
		//cout << "classe : " << value << endl;
		label_classes.push_back(value);
	}
	getline(in_state_, commentaire);

	getline(in_state_, liste);
	istringstream sp(liste);
	double v;
	int i = 0;
	while (sp >> v) {
		string classe = label_classes.at(i);
		proba_apriorie[classe] = v;
		//cout << "classe : " << classe << ", proba : " << v << endl;
		i++;
	}
	getline(in_state_, commentaire);

	while (getline(in_state_, value)) {
		getline(in_state_, value);
		//nom de l'attribut
		string nomAttr = value;
		label_attributs.push_back(nomAttr);
		//cout << "attribut : " << nomAttr << endl;

		//intervals de l'attribut
		getline(in_state_, liste);
		istringstream sa(liste);
		vector<double> interval;
		while (sa >> v) {
			interval.push_back(v);
		}
		intervals_attributs[nomAttr] = interval;
		
		//probas contitionnelles
		map < string, vector<double>> probaAttrSachantClasse;
		for (int l = 0; l < nbr_classes; l++) {
			getline(in_state_, liste);
			istringstream sn(liste);
			vector<double> probCond;
			while (sn >> v) {
				probCond.push_back(v);
			}
			probaAttrSachantClasse[label_classes.at(l)] = probCond;
		}
		proba_conditionnelles[nomAttr] = probaAttrSachantClasse;
	}

}

map<string, int> CBN::interval(map<string, double> data){
	map<string, int> interval;
	
	for (map<string, double>::iterator it = data.begin(); it != data.end(); ++it) {
		string attribut = it->first;
		double valeur = it->second;

		vector<double> seuils = intervals_attributs[attribut];
		int i = 0;
		while (i < seuils.size() && seuils.at(i) < valeur) i++;
		interval[attribut] = i;
	}


	return interval;
}

