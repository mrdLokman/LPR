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


void genererDataSetAttributs(string carsFolder, string labelsFile, string dataFile, bool type);

void genererDataSetImages(string carsFolder, string outImagesFolder, bool type);

void organiserDataSetCaracteres(string dataSetSource, string destinationFolder);

void genererValeursAttribut(string carsFolder, string outFile, string Attribut, bool type);

#endif