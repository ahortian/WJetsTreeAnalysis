#ifndef __writeFastPlotsTex__
#define __writeFastPlotsTex__ 

using namespace std;

void myReplace(string& str, const string& oldStr, const string& newStr);
void writeHeaderPage(ofstream& texFile, string leptonFlavor, string variable);
void writeComparisonPage(ofstream& texFile, string leptonFlavor, string variable, string energy, int jetPtMin, bool doFlat, bool log, bool closureTest, bool doVarWidth);
void writeMethodPage(ofstream& texFile, string leptonFlavor, string variable, string energy, int jetPtMin, string method, bool closureTest, bool doVarWidth);
void writeSystematicsTexFile(string leptonFlavor, string energy, int jetPtMin, string variable, ofstream& texFile);

#endif
