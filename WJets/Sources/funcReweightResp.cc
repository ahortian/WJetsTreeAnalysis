#include <iostream>
#include <sstream>
#include <fstream>


#include <vector>
#include <cstdarg>

#include "funcReweightResp.h"


using namespace std;



double ReweightForResp(vector<double> vecFw, vector<double> binEdge, double iniWeight, double para)
{
    double currentFw(1.0);
    double newWeight(1.0);
    
    for (int i = 0; i < int(binEdge.size()-1); i++){
        if (para >= binEdge.at(i) && para < binEdge.at(i+1)){
            currentFw = vecFw.at(i);
            newWeight = iniWeight * currentFw;
            return newWeight;
        }
    }
    
    return iniWeight;
}

vector<double> buildVecFineBin( int nStdBin, double arrStdBin[], int factChop)
{
    vector<double> vecTemp;
    for (int i = 0; i < nStdBin; i++){
        double binWidth = (arrStdBin[i+1] - arrStdBin[i])/5;
        for (int j = 0; j < factChop; j++){
            double element(0.);
            element = arrStdBin[i] + (j * binWidth);
            vecTemp.push_back(element);
        }
    }
    vecTemp.push_back(arrStdBin[nStdBin]);
    return vecTemp;
}

vector<double> getFW(string variable)
{
    vector<double> vecFw;
    string InputDir = "SFRespForMeanNJ/";
    string Finame = InputDir + "FW_" + variable + ".txt";
    ifstream myfile (Finame.c_str());
    cout << "--- opening txt file for fw : " << Finame << endl;
    if (myfile.is_open())
    {
        int count(0);
        double num(0.0);
        while (myfile >> num) {
            vecFw.push_back(num);
            count++;
        }
        //--- debugging ---
        //cout << " count line : " << count << endl;
        //cout << " size : " << vecFw.size() << endl;
        //for (int i=0; i< vecFw.size(); i++) cout << vecFw.at(i) << endl;
    }
    else cout << "Unable to open FW txt file" << endl;
    myfile.close();
    
    return vecFw;
}

vector<double> getXBin(string variable)
{
    vector<double> FineXBin;
    if (variable == "MeanNJetsHT_Zinc1jet"){
        int nJetHT_Zinc1jet(17);
        double jetHT_Zinc1jet[18] = {30, 39, 49, 62, 78, 96, 118, 150, 190, 240, 300, 370, 450, 540, 650, 800, 1000, 1500};
        vector<double> jetHT_2_Zinc1jet;
        jetHT_2_Zinc1jet = buildVecFineBin(nJetHT_Zinc1jet, jetHT_Zinc1jet, 5);
        FineXBin = jetHT_2_Zinc1jet;
    }
    else if (variable == "MeanNJetsHT_Zinc2jet"){
        int nJetHT_Zinc2jet(13);
        double jetHT_Zinc2jet[14] = {60, 78, 96, 118, 150, 190, 240, 300, 370, 450, 540, 650, 800, 1200};
        vector<double> jetHT_2_Zinc2jet;
        jetHT_2_Zinc2jet = buildVecFineBin(nJetHT_Zinc2jet, jetHT_Zinc2jet, 5);
        FineXBin = jetHT_2_Zinc2jet;
    }
    else if (variable == "MeanNJetsdRapidity_Zinc2jet" || variable == "MeanNJetsdRapidityFB_Zinc2jet"){
        double binWidth = (4.8 - 0.0)/240.0;
        double edge(0.0);
        vector<double> dRap_2_Zinc2jet;
        for(int i = 0; i <= 240; i++){
            //cout << edge << endl;
            dRap_2_Zinc2jet.push_back(edge);
            edge += binWidth;
        }
        FineXBin = dRap_2_Zinc2jet;
        //cout << "size : " << FineXBin.size() << endl;
    }
    
    return FineXBin;
}





