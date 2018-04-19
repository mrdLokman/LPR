#include "Utilitaire.h"


void AfficheMap(map<string, double> m) {
	for (map<string, double>::iterator it = m.begin(); it != m.end(); ++it) {
		string attribut = it->first;
		double valeur = it->second;
		cout << attribut << " => " << valeur << endl;
	}
	cout << endl;
}