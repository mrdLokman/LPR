#ifndef SEGMENTATION_H
#define SEGMENTATION_H


#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<vector>
#include<iostream>

#include"classes/Composant.h"
#include "classes/BN.h"
#include "Utilitaire.h"
#include "display.h"

//using namespace std;
//using namespace cv;

vector<Composant> segmentationProjection(Mat plaque);
vector<Composant> segmentationACC(Mat plaque);

int AutomateSegmentation(Mat plaque);


vector<int> positionPossible(Mat data);
Composant creerComposant(Composant c, int d, int f);

vector<Composant> separartionComposantsChevauches(Composant composant, int largeurMax, int hauteurMoy, double EpaisseurMoy, CBN cbn);

#endif