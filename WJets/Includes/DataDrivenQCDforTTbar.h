#ifndef _DataDrivenQCDforTTbar_h_
#define _DataDrivenQCDforTTbar_h_

#include <iostream>

using namespace std;

void DataDrivenQCDforTTbar();
void FuncDataDrivenQCD(string variable="ZNGoodJets_Zexc" , string leptonFlavor = "SMuE", bool doFlat = false, bool doVarWidth = true, int MET = 30);

#endif

