#include "Tests.h"
#include "Pretraitement.h"



int main1()
{
	//filtre();

	//testOprsMorph("input/binaire.png");

	testUnsharpeFiltre("input/4-ajustee.png");
	testUnsharpeFiltre2("input/4-ajustee.png");

	//testFiltrePersonnel("input/4-ajustee.png");

	waitKey(0);
	return 0;
}
