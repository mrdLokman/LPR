#ifndef DATASETGENERATOR_H
#define DATASETGENERATOR_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "classes/Selector.h"
#include "Pretraitement.h"
#include <vector>
#include <iostream>
#include <fstream>
#include "Segmentation.h"
#include "classes/Composant.h"


void genererDataSetImagesV2(string carsFolder, string outImagesFolder);

void genererDataSetAttributsTxt(string imagesFolder, string labelsFile, string dataFile, vector<string> attributs, bool type);

void genererDataSetImagesTxt(string imagesFolder, string labelsFile, string dataFile, bool type);

void genererValeursAttribut(string carsFolder, string outFile, string Attribut, bool type);


//transformation des fichiers TXT au XML pour les classifieurs :

int dataAttributs_TxtFileToXml(string dataSetD, string dataSetL, string dataFile, string labelsFile);

int dataImageChiffres_ImgToXml(string folder, string saveFolder, string dataFile, string labelsFile, int WIDTH, int HEIGHT);



// Version 1 :

void genererDataSetAttributs(string carsFolder, string labelsFile, string dataFile, vector<string> attributs, bool type);

void genererDataSetImages(string carsFolder, string outImagesFolder, bool type);

void organiserDataSetCaracteres(string dataSetSource, string destinationFolder);

#endif