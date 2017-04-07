#ifndef _myFinalUnfoldEdit_h_
#define _myFinalUnfoldEdit_h_
#include "getFilesAndHistograms.h"
#include <iostream>

using namespace std;

void myFinalUnfoldEdit(int start = 0, int end = -1, string leptonFlavor = "SMu", string unfAlg = "Bayes");

void FuncUnfold(string leptonFlavor = "SMu", bool isMuon = true, string unfAlg = "Bayes", string variable = "ZNGoodJets_Zexc", int UsedKtermBayes = 5, int UsedKtermSVD = 5, bool doFlat = false , bool doVarWidth = true);

#endif