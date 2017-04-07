#ifndef _myFastPlotsIter_h_
#define _myFastPlotsIter_h_

using namespace std;

void myFastPlotsIter(string leptonFlavor = "DE", string var = "", string var2 = "", bool closureTest = false);

void FastPlotsRun(const int *sel, int nsel, string leptonFlavor, string variableStd, string variable, bool logZ, bool decrease, int SVDkterm, int Bayeskterm, bool closureTest = 0, int JetPtMin = 30, int JetPtMax = 0, bool doFlat = 0, bool doVarWidth = 1);




#endif
