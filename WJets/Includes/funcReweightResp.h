#ifndef _funcReweightResp_h_
#define _funcReweightResp_h_

#include <iostream>
#include <cstdarg>
#include <vector>


using namespace std;


double ReweightForResp(vector<double> vecFw, vector<double> binEdge, double iniWeight = 1.0, double para = 1.0);

vector<double> buildVecFineBin(int nStdBin, double arrStdBin[], int factChop);

vector<double> getFW(string variable = "MeanNJetsHT_Zinc1jet");

vector<double> getXBin(string variable = "MeanNJetsHT_Zinc1jet");


#endif