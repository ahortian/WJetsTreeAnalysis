#ifndef _plottingFunctions_h_
#define _plottingFunctions_h_

#include <iostream>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1.h>
#include <string>

//using namespace std;
TCanvas*  plottingFunctions(int NHistos, TH1D* histos[], bool logZ, bool decrease, string variable, string sLegend[], float YMin = 0.59, float YMax = 1.19);


#endif

