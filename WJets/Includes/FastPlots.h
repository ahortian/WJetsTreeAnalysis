#ifndef _FastPlots_h_
#define _FastPlots_h_

using namespace std;

void FastPlotsRun(const int *sel, int nsel, string leptonFlavor, string variable, bool logZ, bool decrease, int SVDkterm, int Bayeskterm, bool closureTest = 0, int JetPtMin = 30, int JetPtMax = 0, bool doFlat = 0, bool doVarWidth = 1);
void FastPlots(string leptonFlavor = "DE", string var = "");

#endif
