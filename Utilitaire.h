#ifndef UTILITAIRE_H
#define UTILITAIRE_H

#include <string>
#include <iostream>
#include <map>
#include "classes\Composant.h"

using namespace std;

void AfficheMap(map<string, double> m);

bool ordre(Composant c1, Composant c2);
#endif

