#include"Composant.h"


Composant::Composant() {

}

Composant::~Composant() {

}

double Composant::ratio()
{
	return (finX - debutX)/(finY - debutY);
}

double Composant::surface()
{
	return (finX - debutX)*(finY - debutY);
}

double Composant::densite()
{
	int nbrPixelBlanc = countNonZero(data);
	return 1.0 - ((double)nbrPixelBlanc)/surface();
}
