#define PI 3.14159265359
#include <iostream>
#include <TH1.h>
#include <TH2.h>
#include <RooUnfoldResponse.h>
#include "HistoSet.h"

using namespace std;

ClassImp(HistoSet)

HistoSet::~HistoSet()
{
}

vector<double> HistoSet::makeVector(int num, ...)
{
    va_list list;
    va_start(list, num);
    vector<double> vec;
    for (int i(0); i < num; i++) {
        double next = va_arg(list, double);
        vec.push_back(next);
    }
    va_end(list);
    return vec;
}

void HistoSet::insertVector(vector<double>& veca, int num, ...)
{
    va_list list;
    va_start(list, num);
    vector<double> vecb;
    for (int i(0); i < num; i++) {
        double next = va_arg(list, double);
        vecb.push_back(next);
    }
    va_end(list);
    veca.insert(veca.end(), vecb.begin(), vecb.end());
}

vector<double> HistoSet::buildVecFineBin( int nStdBin, double arrStdBin[], int factChop)
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

TH1D* HistoSet::newTH1D(string name, string title, string xTitle, int nBins, double *xBins){
    TH1D* hist = new TH1D(name.c_str(), title.c_str(), nBins, xBins);
    hist->GetXaxis()->SetTitle(xTitle.c_str());
    hist->GetYaxis()->SetTitle("# Events");
    listOfHistograms.push_back(hist);
    return hist;
}

TH1D* HistoSet::newTH1D(string name, string title, string xTitle, vector<double>& xBinsVect)
{
    int nBins = xBinsVect.size()-1;
    double *xBins = new double[xBinsVect.size()];
    std::copy(xBinsVect.begin(), xBinsVect.end(), xBins);
    TH1D* hist = new TH1D(name.c_str(), title.c_str(), nBins, xBins);
    hist->GetXaxis()->SetTitle(xTitle.c_str());
    hist->GetYaxis()->SetTitle("# Events");
    delete [] xBins;
    listOfHistograms.push_back(hist);
    return hist;
}

TH1D* HistoSet::newTH1D(string name, string title, string xTitle, int nBins, double xLow, double xUp){
    TH1D* hist = new TH1D(name.c_str(), title.c_str(), nBins, xLow, xUp);
    hist->GetXaxis()->SetTitle(xTitle.c_str());
    hist->GetYaxis()->SetTitle("# Events");
    hist->SetOption("HIST");
    listOfHistograms.push_back(hist);
    return hist;
}

TH2D* HistoSet::newTH2D(string name, string title, int nBinsX, double *xBins, int nBinsY, double *yBinsY){
    TH2D* hist = new TH2D(name.c_str(), title.c_str(), nBinsX, xBins, nBinsY, yBinsY);
    hist->GetZaxis()->SetTitle("# Events");
    listOfHistograms.push_back(hist);
    return hist;
}

TH2D* HistoSet::newTH2D(string name, string title, vector<double>& xBinsVect, vector<double>& yBinsVect)
{
    int nBins_x = xBinsVect.size()-1;
    int nBins_y = yBinsVect.size()-1;
    double *xBins = new double[xBinsVect.size()];
    double *yBins = new double[yBinsVect.size()];
    std::copy(xBinsVect.begin(), xBinsVect.end(), xBins);
    std::copy(yBinsVect.begin(), yBinsVect.end(), yBins);
    TH2D* hist = new TH2D(name.c_str(), title.c_str(), nBins_x, xBins, nBins_y, yBins);
    hist->GetZaxis()->SetTitle("# Events");
    delete [] xBins;
    delete [] yBins;
    listOfHistograms.push_back(hist);
    return hist;
}

TH2D* HistoSet::newTH2D(string name, string title, int nBinsX, double *xBins, int nBinsY, double yLow, double yUp){
    TH2D* hist = new TH2D(name.c_str(), title.c_str(), nBinsX, xBins, nBinsY, yLow, yUp);
    hist->GetZaxis()->SetTitle("# Events");
    listOfHistograms.push_back(hist);
    return hist;
}

TH2D* HistoSet::newTH2D(string name, string title, int nBinsX, double xLow, double xUp, int nBinsY, double *yBins){
    TH2D* hist = new TH2D(name.c_str(), title.c_str(), nBinsX, xLow, xUp, nBinsY, yBins);
    hist->GetZaxis()->SetTitle("# Events");
    listOfHistograms.push_back(hist);
    return hist;
}

TH2D* HistoSet::newTH2D(string name, string title, int nBinsX, double xLow, double xUp, int nBinsY, double yLow, double yUp){
    TH2D* hist = new TH2D(name.c_str(), title.c_str(), nBinsX, xLow, xUp, nBinsY, yLow, yUp);
    hist->GetZaxis()->SetTitle("# Events");
    hist->SetOption("HIST");
    listOfHistograms.push_back(hist);
    return hist;
}




RooUnfoldResponse* HistoSet::newResp(TH1D* reco, TH1D* gen)
{
    RooUnfoldResponse *response = new RooUnfoldResponse(reco, gen);
    listOfResponses.push_back(response);
    return response;
}

RooUnfoldResponse* HistoSet::newResp(TH2D* reco, TH2D* gen)
{
    RooUnfoldResponse *response = new RooUnfoldResponse(reco, gen);
    listOfResponses.push_back(response);
    return response;
}

HistoSet::HistoSet(string leptonFlavor)
{
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();

    string ZpT = "p_{T}(Z) [GeV]", Zrap = "y(Z)", Zeta = "#eta(Z)";
    string HT = "H_{T}(jets) [GeV]", Mjj = "M_{j_{1}j_{2}} [GeV]", jSpt = "#Delta_{pT}^{rel}(j_{1}j_{2})", jdPhi = "#Delta#phi(j_{1}j_{2})", jdEta = "#Delta#eta(j_{1}j_{2})";
    string Mll = "M_{#mu#mu} [GeV]", leta = "#eta(#mu)", lphi = "#phi(#mu)",lpT = "p_{T}(#mu) [GeV]", ldPhi = "#Delta#phi(#mu_{1}#mu_{2})", ldEta = "#Delta#eta(#mu_{1}#mu_{2})", ldR = "#DeltaR(#mu_{1}#mu_{2})";
    string lSpt = "#Delta_{pT}^{rel}(#mu_{1}#mu_{2})";
    string Spt = "#Delta_{pT}^{rel}(j_{1}j_{2}#mu_{1}#mu_{2})";
    string Sphi = "Sphi(j_{1}j_{2}#mu_{1}#mu_{2})";
    string lJetdEta = "#Delta#eta(#mu_{1}#mu_{2},j_{1})";

    bool doWJets = false;
    if (leptonFlavor == "Electrons" || leptonFlavor == "DE" || leptonFlavor == "DE_") {
        Mll = "M_{ee} [GeV]";
        leta = "#eta(e)";
        lpT = "p_{T}(e) [GeV]";
        ldPhi = "#Delta#phi(e_{1}e_{2})";
        ldEta = "#Delta#eta(e_{1}e_{2})";
        ldR = "#DeltaR(e_{1}e_{2})";
        lSpt = "#Delta_{pT}^{rel}(e_{1}e_{2})";
        Spt = "#Delta_{pT}^{rel}(j_{1}j_{2}e_{1}e_{2})";
        Sphi = "Sphi(j_{1}j_{2}e_{1}e_{2})";
        lJetdEta = "#Delta#eta(e_{1}e_{2},j_{1})";
    }
    else if ( leptonFlavor == "Electron" || leptonFlavor == "SE" || leptonFlavor == "SE_") {
        doWJets = true ; 
        Mll = "M_{e#nu} [GeV]";
        leta = "#eta(e)";
        lpT = "p_{T}(e) [GeV]";
        ldPhi = "#Delta#phi(e_{1}#nu_{2})";
        ldEta = "#Delta#eta(e_{1}#nu_{2})";
        ldR = "#DeltaR(e_{1}#nu_{2})";
        lSpt = "#Delta_{pT}^{rel}(e_{1}#nu_{2})";
        Spt = "#Delta_{pT}^{rel}(j_{1}j_{2}e_{1}#nu_{2})";
        Sphi = "Sphi(j_{1}j_{2}e_{1}#nu_{2})";
        lJetdEta = "#Delta#eta(e,j_{1})";

    } 
    else if ( leptonFlavor == "Muon" || leptonFlavor == "SMu" || leptonFlavor == "SMu_") {
        doWJets = true ;
        Mll = "M_{#mu#nu} [GeV]";
        ldPhi = "#Delta#phi(#mu_{1}#nu_{2})";
        ldEta = "#Delta#eta(#mu_{1}#nu_{2})";
        ldR = "#DeltaR(e_{1}#nu_{2})";
        lSpt = "#Delta_{pT}^{rel}(#mu_{1}#nu_{2})";
        Spt = "#Delta_{pT}^{rel}(j_{1}j_{2}#mu_{1}#nu_{2})";
        Sphi = "Sphi(j_{1}j_{2}#mu_{1}#nu_{2})";
        lJetdEta = "#Delta#eta(#mu,j_{1})";

    }

    //int nJetPt_Zinc1jet(14);
    //double jetPt_Zinc1jet[15] = {20, 24, 30, 39, 49, 62, 79, 105, 138, 181, 231, 294, 375, 494, 800};
    int nJetPt_Zinc1jet(14);
    double jetPt_Zinc1jet[15] = {20, 24, 30, 39, 49, 62, 79, 105, 138, 181, 231, 294, 375, 550, 900};
    //int nJetPt_Zinc2jet(13);
    //double jetPt_Zinc2jet[14] = {20, 24, 30, 39, 49, 62, 78, 105, 142, 185, 235, 300, 380, 500};
    int nJetPt_Zinc2jet(13);
    double jetPt_Zinc2jet[14] = {20, 24, 30, 39, 49, 62, 78, 105, 142, 185, 235, 300, 380, 500};
    //int nJetPt_Zinc3jet(9);
    //double jetPt_Zinc3jet[10] =   {20, 24, 30, 41, 59, 81, 110, 152, 200, 300};
    int nJetPt_Zinc3jet(9);
    double jetPt_Zinc3jet[10] =   {20, 24, 30, 41, 59, 81, 110, 152, 200, 300};
    int nJetPt_Zinc4jet(8);
    double jetPt_Zinc4jet[9] = {20, 24, 30, 39, 49, 62, 78, 96, 180};
    int nJetPt_Zinc5jet(6);
    double jetPt_Zinc5jet[7] = {20, 24, 30, 39, 49, 62, 100};
    
    int nJetPt_1_Zinc1jet(23);
    double jetPt_1_Zinc1jet[24] = {20, 24, 30, 39, 49, 60, 72, 85, 100, 117, 136, 157, 187, 220, 258, 300, 350, 400, 450, 500, 590, 700, 900, 1400};
    int nJetPt_1_Zinc2jet(22);
    double jetPt_1_Zinc2jet[23] = {20, 24, 30, 39, 49, 60, 72, 85, 100, 117, 136, 157, 187, 220, 258, 300, 350, 400, 450, 500, 590, 700, 1000};
    int nJetPt_1_Zinc3jet(12);
    double jetPt_1_Zinc3jet[13] = {20, 24, 30, 39, 49, 62, 78, 105, 142, 185, 235, 300, 500};
    int nJetPt_1_Zinc4jet(10);
    double jetPt_1_Zinc4jet[11] = {20, 24, 30, 39, 49, 62, 78, 96, 120, 160, 250};
    int nJetPt_1_Zinc5jet(7);
    double jetPt_1_Zinc5jet[8] = {20, 24, 30, 39, 49, 62, 90, 140};
    
    vector<double> jetPt_2_Zinc1jet;
    vector<double> jetPt_2_Zinc2jet;
    vector<double> jetPt_2_Zinc3jet;
    vector<double> jetPt_2_Zinc4jet;
    vector<double> jetPt_2_Zinc5jet;
    jetPt_2_Zinc1jet = buildVecFineBin(nJetPt_Zinc1jet, jetPt_Zinc1jet, 5);
    jetPt_2_Zinc2jet = buildVecFineBin(nJetPt_Zinc2jet, jetPt_Zinc2jet, 5);
    jetPt_2_Zinc3jet = buildVecFineBin(nJetPt_Zinc3jet, jetPt_Zinc3jet, 5);
    jetPt_2_Zinc4jet = buildVecFineBin(nJetPt_Zinc4jet, jetPt_Zinc4jet, 5);
    jetPt_2_Zinc5jet = buildVecFineBin(nJetPt_Zinc5jet, jetPt_Zinc5jet, 5);
    
    
    //int nJetHT_Zinc1jet(17);
    //double jetHT_Zinc1jet[18] = {30, 39, 49, 62, 78, 96, 118, 150, 190, 240, 300, 370, 450, 540, 650, 800, 1000, 1500};
    int nJetHT_Zinc1jet(17);
    double jetHT_Zinc1jet[18] = {30, 39, 49, 62, 78, 96, 118, 150, 190, 240, 300, 370, 450, 540, 650, 820, 1100, 1500};
    //int nJetHT_Zinc2jet(13);
    //double jetHT_Zinc2jet[14] = {60, 78, 96, 118, 150, 190, 240, 300, 370, 450, 540, 650, 800, 1200};
    int nJetHT_Zinc2jet(13);
    double jetHT_Zinc2jet[14] = {60, 78, 96, 118, 150, 190, 240, 300, 370, 450, 540, 650, 800, 1200};
    //int nJetHT_Zinc3jet(11);
    //double jetHT_Zinc3jet[12] = {90, 105, 125, 151, 185, 230, 290, 366, 466, 586, 767, 990};
    //int nJetHT_Zinc3jet(8);
    //double jetHT_Zinc3jet[9] = {90, 118, 168, 220, 300, 400, 550, 780, 1100};
    int nJetHT_Zinc3jet(8);
    double jetHT_Zinc3jet[9] = {90, 118, 168, 220, 300, 400, 550, 780, 1100};
    int nJetHT_Zinc4jet(9);
    //double jetHT_Zinc4jet[10] = {120, 140, 167, 203, 253, 320, 410, 530, 690, 910};
    double jetHT_Zinc4jet[10] = {120, 140, 167, 203, 253, 320, 410, 530, 690, 910};
    int nJetHT_Zinc5jet(7);
    double jetHT_Zinc5jet[8] = {150, 180, 222, 282, 365, 485, 650, 880};

    int nJetHT_20_Zinc1jet(17);
    double jetHT_20_Zinc1jet[18] = {20, 30, 39, 49, 62, 78, 96, 118, 150, 190, 240, 300, 370, 450, 540, 650, 800, 1000};
    int nJetHT_20_Zinc2jet(13);
    double jetHT_20_Zinc2jet[14] = {40, 60, 78, 96, 118, 150, 190, 240, 300, 370, 450, 540, 650, 800};
    int nJetHT_20_Zinc3jet(8);
    double jetHT_20_Zinc3jet[9] = {60, 90, 118, 168, 220, 300, 400, 550, 780};

    int nJetHT_20_30_Zinc1jet(17);
    double jetHT_20_30_Zinc1jet[18] = {20, 30, 39, 49, 62, 78, 96, 118, 150, 190, 240, 300, 370, 450, 540, 650, 800, 1000};
    int nJetHT_20_30_Zinc2jet(13);
    double jetHT_20_30_Zinc2jet[14] = {40, 60, 78, 96, 118, 150, 190, 240, 300, 370, 450, 540, 650, 800};
    int nJetHT_20_30_Zinc3jet(8);
    double jetHT_20_30_Zinc3jet[9] = {60, 90, 118, 168, 220, 300, 400, 550, 780};
    
    int nJetHT_1_Zinc1jet(18);
    double jetHT_1_Zinc1jet[19] = {30, 39, 49, 62, 78, 96, 118, 150, 190, 240, 300, 370, 450, 540, 650, 800, 1000, 1300, 2000};
    int nJetHT_1_Zinc2jet(15);
    double jetHT_1_Zinc2jet[16] = {60, 78, 96, 118, 150, 190, 240, 300, 370, 450, 540, 650, 800, 1000, 1300, 2000};
    int nJetHT_1_Zinc3jet(12);
    double jetHT_1_Zinc3jet[13] = {90, 118, 150, 190, 240, 300, 370, 450, 540, 650, 800, 1100, 1800};
    int nJetHT_1_Zinc4jet(11);
    double jetHT_1_Zinc4jet[12] = {120, 140, 165, 200, 250, 310, 380, 480, 600, 800, 1100, 1800};
    int nJetHT_1_Zinc5jet(8);
    double jetHT_1_Zinc5jet[9] = {150, 180, 222, 282, 365, 485, 650, 880, 1300};

    vector<double> jetHT_2_Zinc1jet;
    vector<double> jetHT_2_Zinc2jet;
    vector<double> jetHT_2_Zinc3jet;
    vector<double> jetHT_2_Zinc4jet;
    vector<double> jetHT_2_Zinc5jet;
    jetHT_2_Zinc1jet = buildVecFineBin(nJetHT_Zinc1jet, jetHT_Zinc1jet, 5);
    jetHT_2_Zinc2jet = buildVecFineBin(nJetHT_Zinc2jet, jetHT_Zinc2jet, 5);
    jetHT_2_Zinc3jet = buildVecFineBin(nJetHT_Zinc3jet, jetHT_Zinc3jet, 5);
    jetHT_2_Zinc4jet = buildVecFineBin(nJetHT_Zinc4jet, jetHT_Zinc4jet, 5);
    jetHT_2_Zinc5jet = buildVecFineBin(nJetHT_Zinc5jet, jetHT_Zinc5jet, 5);
    

    int nJetMass_Zinc2jet(17);
    double ArrJetmass_Zinc2jet[18] = {0, 25, 52, 81, 112, 145, 180, 217, 256, 297, 340, 385, 432, 481, 532, 585, 640, 700};
    int nJetMass_Zinc3jet(15);
    double ArrJetmass_Zinc3jet[16] = {0, 30, 62, 96, 132, 170, 210, 252, 296, 342, 390, 440, 492, 546, 602, 660};
    int nJetMass_Zinc4jet(14);
    double ArrJetmass_Zinc4jet[15] = {0, 34, 70, 108, 148, 190, 234, 280, 328, 378, 430, 484, 540, 598, 660};
    
    vector<double> vJetmass_2_Zinc2jet;
    vJetmass_2_Zinc2jet = buildVecFineBin(nJetMass_Zinc2jet, ArrJetmass_Zinc2jet, 5);
    vector<double> vJetmass_2_Zinc3jet;
    vJetmass_2_Zinc3jet = buildVecFineBin(nJetMass_Zinc3jet, ArrJetmass_Zinc3jet, 5);
    vector<double> vJetmass_2_Zinc4jet;
    vJetmass_2_Zinc4jet = buildVecFineBin(nJetMass_Zinc4jet, ArrJetmass_Zinc4jet, 5);
    
    
    int nJetPtEta_Zinc1jet(8);
    double jetPtEta_Zinc1jet[9] = {30, 40, 52, 68, 88, 113, 144, 184, 480};
    int nJetPtEta_Zinc2jet(7);
    double jetPtEta_Zinc2jet[8] = {30, 40, 52, 68, 88, 113, 144, 377};

    //***************************** Basic plots for Wjets *****************************//
    //--- For calculateing b-tagging efficiency---
    int npt_b_range(7);
    double pt_b_range[8] = {30,50,70,100,140,200,300,670};
    h_pt_eta_b           = newTH2D("h_pt_eta_b",             "h_pt_eta_b",           npt_b_range,  pt_b_range,   10,-2.4,2.4);
    h_pt_eta_b_tagged    = newTH2D("h_pt_eta_b_tagged",      "h_pt_eta_b_tagged",    npt_b_range,  pt_b_range,   10,-2.4,2.4);
    h_pt_eta_c           = newTH2D("h_pt_eta_c",             "h_pt_eta_c",           npt_b_range,  pt_b_range,   10,-2.4,2.4);
    h_pt_eta_c_tagged    = newTH2D("h_pt_eta_c_tagged",      "h_pt_eta_c_tagged",    npt_b_range,  pt_b_range,   10,-2.4,2.4);
    h_pt_eta_udsg        = newTH2D("h_pt_eta_udsg",          "h_pt_eta_udsg",        npt_b_range,  pt_b_range,   10,-2.4,2.4);
    h_pt_eta_udsg_tagged = newTH2D("h_pt_eta_udsg_tagged",   "h_pt_eta_udsg_tagged", npt_b_range,  pt_b_range,   10,-2.4,2.4);
    
    h_pt_b              = newTH1D("h_pt_b",             "h_pt_b",           "p_{T} [GeV]",  20,   0., 200.);
    h_pt_b_tagged       = newTH1D("h_pt_b_tagged",      "h_pt_b_tagged",    "p_{T} [GeV]",  20,   0., 200.);
    h_pt_udsg           = newTH1D("h_pt_udsg",          "h_pt_udsg",        "p_{T} [GeV]",  20,   0., 200.);
    h_pt_udsg_tagged    = newTH1D("h_pt_udsg_tagged",   "h_pt_udsg_tagged", "p_{T} [GeV]",  20,   0., 200.);
    
	
	NEventsPassCuts = newTH1D("NEventsPassCuts","Events Passing Cuts", "Cuts", 6, -0.5, 5.5);
	NEventsPassCuts->GetXaxis()->SetBinLabel(1, "= Total");
	NEventsPassCuts->GetXaxis()->SetBinLabel(2, "= PassMETFilter");
	NEventsPassCuts->GetXaxis()->SetBinLabel(3, "= PassTrigger");
	NEventsPassCuts->GetXaxis()->SetBinLabel(4, "= PassLeptonRequirements");
	NEventsPassCuts->GetXaxis()->SetBinLabel(5, "= PassMTcut");
	NEventsPassCuts->GetXaxis()->SetBinLabel(6, "= PassBtagveto");
	
    
    //--- Jet multiplicity -----------
    if ( doWJets ) ZNGoodJets_Zexc = newTH1D("ZNGoodJets_Zexc","Jet Counter (excl.)", "N_{jets}", 8, -0.5, 7.5);
    //if ( doWJets ) ZNGoodJets_Zexc = newTH1D("ZNGoodJets_Zexc","Jet Counter (excl.)", "N_{jets}", 11, -0.5, 10.5);
    else ZNGoodJets_Zexc = newTH1D("ZNGoodJets_Zexc","Jet Counter (excl.)", "N_{jets}", 8, -0.5, 7.5);
    ZNGoodJets_Zexc->GetXaxis()->SetBinLabel(1, "= 0");
    ZNGoodJets_Zexc->GetXaxis()->SetBinLabel(2, "= 1");
    ZNGoodJets_Zexc->GetXaxis()->SetBinLabel(3, "= 2");
    ZNGoodJets_Zexc->GetXaxis()->SetBinLabel(4, "= 3");
    ZNGoodJets_Zexc->GetXaxis()->SetBinLabel(5, "= 4");
    ZNGoodJets_Zexc->GetXaxis()->SetBinLabel(6, "= 5");
    ZNGoodJets_Zexc->GetXaxis()->SetBinLabel(7, "= 6");
    ZNGoodJets_Zexc->GetXaxis()->SetBinLabel(8, "= 7");
    /*if ( doWJets ){
        ZNGoodJets_Zexc->GetXaxis()->SetBinLabel(9, "= 8");
        ZNGoodJets_Zexc->GetXaxis()->SetBinLabel(10,"= 9");
        ZNGoodJets_Zexc->GetXaxis()->SetBinLabel(11,"= 10");
    }*/
    
    if ( doWJets )  genZNGoodJets_Zexc = newTH1D("genZNGoodJets_Zexc","Jet Counter (excl.)", "N_{jets}", 8, -0.5, 7.5);
    else genZNGoodJets_Zexc = newTH1D("genZNGoodJets_Zexc","Jet Counter (excl.)", "N_{jets}", 8, -0.5, 7.5);
    genZNGoodJets_Zexc->GetXaxis()->SetBinLabel(1,"= 0");
    genZNGoodJets_Zexc->GetXaxis()->SetBinLabel(2,"= 1");
    genZNGoodJets_Zexc->GetXaxis()->SetBinLabel(3,"= 2");
    genZNGoodJets_Zexc->GetXaxis()->SetBinLabel(4,"= 3");
    genZNGoodJets_Zexc->GetXaxis()->SetBinLabel(5,"= 4");
    genZNGoodJets_Zexc->GetXaxis()->SetBinLabel(6,"= 5");
    genZNGoodJets_Zexc->GetXaxis()->SetBinLabel(7,"= 6");
    genZNGoodJets_Zexc->GetXaxis()->SetBinLabel(8,"= 7");
   /* if ( doWJets ){
        genZNGoodJets_Zexc->GetXaxis()->SetBinLabel(9, "= 8");
        genZNGoodJets_Zexc->GetXaxis()->SetBinLabel(10,"= 9");
        genZNGoodJets_Zexc->GetXaxis()->SetBinLabel(11,"= 10");
    }*/
    
    ZNGoodJets_Zinc = newTH1D("ZNGoodJets_Zinc","Jet Counter (incl.)", "N_{jets}", 8, -0.5, 7.5);
    //ZNGoodJets_Zinc = newTH1D("ZNGoodJets_Zinc","Jet Counter (incl.)", "N_{jets}", 11, -0.5, 10.5);
    ZNGoodJets_Zinc->GetXaxis()->SetBinLabel(1, "#geq 0");
    ZNGoodJets_Zinc->GetXaxis()->SetBinLabel(2, "#geq 1");
    ZNGoodJets_Zinc->GetXaxis()->SetBinLabel(3, "#geq 2");
    ZNGoodJets_Zinc->GetXaxis()->SetBinLabel(4, "#geq 3");
    ZNGoodJets_Zinc->GetXaxis()->SetBinLabel(5, "#geq 4");
    ZNGoodJets_Zinc->GetXaxis()->SetBinLabel(6, "#geq 5");
    ZNGoodJets_Zinc->GetXaxis()->SetBinLabel(7, "#geq 6");
    ZNGoodJets_Zinc->GetXaxis()->SetBinLabel(8, "#geq 7");
    /*
    ZNGoodJets_Zinc->GetXaxis()->SetBinLabel(9, "#geq 8");
    ZNGoodJets_Zinc->GetXaxis()->SetBinLabel(10,"#geq 9");
    ZNGoodJets_Zinc->GetXaxis()->SetBinLabel(11,"#geq 10");*/
    
    genZNGoodJets_Zinc = newTH1D("genZNGoodJets_Zinc","Jet Counter (incl.)", "N_{jets}", 8, -0.5, 7.5);
    genZNGoodJets_Zinc->GetXaxis()->SetBinLabel(1,"#geq 0");
    genZNGoodJets_Zinc->GetXaxis()->SetBinLabel(2,"#geq 1");
    genZNGoodJets_Zinc->GetXaxis()->SetBinLabel(3,"#geq 2");
    genZNGoodJets_Zinc->GetXaxis()->SetBinLabel(4,"#geq 3");
    genZNGoodJets_Zinc->GetXaxis()->SetBinLabel(5,"#geq 4");
    genZNGoodJets_Zinc->GetXaxis()->SetBinLabel(6,"#geq 5");
    genZNGoodJets_Zinc->GetXaxis()->SetBinLabel(7,"#geq 6");
    genZNGoodJets_Zinc->GetXaxis()->SetBinLabel(8,"#geq 7");
    /*genZNGoodJets_Zinc->GetXaxis()->SetBinLabel(9,"#geq 8");
    genZNGoodJets_Zinc->GetXaxis()->SetBinLabel(10,"#geq 9");
    genZNGoodJets_Zinc->GetXaxis()->SetBinLabel(11,"#geq 10"); */
    
    if ( doWJets ) hresponseZNGoodJets_Zexc = newTH2D("hresponseZNGoodJets_Zexc", "hresp ZNGoodJets_Zexc", 8, -0.5, 7.5, 8, -0.5, 7.5);
    else hresponseZNGoodJets_Zexc = newTH2D("hresponseZNGoodJets_Zexc", "hresp ZNGoodJets_Zexc", 8, -0.5, 7.5, 8, -0.5, 7.5);
    hresponseZNGoodJets_Zinc = newTH2D("hresponseZNGoodJets_Zinc", "hresp ZNGoodJets_Zinc", 8, -0.5, 7.5, 8, -0.5, 7.5);
    
    
    //--- Jet pt -----------
    FirstJetPt_Zinc1jet        = newTH1D("FirstJetPt_Zinc1jet",       "1st jet p_{T} (N_{jets} #geq 1)",    "p_{T}(j_{1}) [GeV]",  nJetPt_Zinc1jet,   jetPt_Zinc1jet);
    FirstJetPt_1_Zinc1jet      = newTH1D("FirstJetPt_1_Zinc1jet",     "1st jet p_{T} (N_{jets} #geq 1)1",   "p_{T}(j_{1}) [GeV]",  nJetPt_1_Zinc1jet, jetPt_1_Zinc1jet);
    FirstJetPt_2_Zinc1jet      = newTH1D("FirstJetPt_2_Zinc1jet",     "1st jet p_{T} (N_{jets} #geq 1)2",   "p_{T}(j_{1}) [GeV]",   jetPt_2_Zinc1jet);
    
    SecondJetPt_Zinc2jet       = newTH1D("SecondJetPt_Zinc2jet",      "2nd jet p_{T} (N_{jets} #geq 2)",    "p_{T}(j_{2}) [GeV]",  nJetPt_Zinc2jet,   jetPt_Zinc2jet);
    SecondJetPt_1_Zinc2jet     = newTH1D("SecondJetPt_1_Zinc2jet",    "2nd jet p_{T} (N_{jets} #geq 2)1",   "p_{T}(j_{2}) [GeV]",  nJetPt_1_Zinc2jet, jetPt_1_Zinc2jet);
    SecondJetPt_2_Zinc2jet     = newTH1D("SecondJetPt_2_Zinc2jet",    "2nd jet p_{T} (N_{jets} #geq 2)2",   "p_{T}(j_{2}) [GeV]",   jetPt_2_Zinc2jet);
    
    ThirdJetPt_Zinc3jet        = newTH1D("ThirdJetPt_Zinc3jet",       "3rd jet p_{T} (N_{jets} #geq 3)",    "p_{T}(j_{3}) [GeV]",  nJetPt_Zinc3jet,   jetPt_Zinc3jet);
    ThirdJetPt_1_Zinc3jet      = newTH1D("ThirdJetPt_1_Zinc3jet",     "3rd jet p_{T} (N_{jets} #geq 3)1",   "p_{T}(j_{3}) [GeV]",  nJetPt_1_Zinc3jet, jetPt_1_Zinc3jet);
    ThirdJetPt_2_Zinc3jet      = newTH1D("ThirdJetPt_2_Zinc3jet",     "3rd jet p_{T} (N_{jets} #geq 3)2",   "p_{T}(j_{3}) [GeV]",   jetPt_2_Zinc3jet);
    
    FourthJetPt_Zinc4jet       = newTH1D("FourthJetPt_Zinc4jet",      "4th jet p_{T} (N_{jets} #geq 4)",    "p_{T}(j_{4}) [GeV]",  nJetPt_Zinc4jet,   jetPt_Zinc4jet);
    FourthJetPt_1_Zinc4jet     = newTH1D("FourthJetPt_1_Zinc4jet",    "4th jet p_{T} (N_{jets} #geq 4)1",   "p_{T}(j_{4}) [GeV]",  nJetPt_1_Zinc4jet, jetPt_1_Zinc4jet);
    FourthJetPt_2_Zinc4jet     = newTH1D("FourthJetPt_2_Zinc4jet",    "4th jet p_{T} (N_{jets} #geq 4)2",   "p_{T}(j_{4}) [GeV]",   jetPt_2_Zinc4jet);
    
    FifthJetPt_Zinc5jet        = newTH1D("FifthJetPt_Zinc5jet",       "5th jet p_{T} (N_{jets} #geq 5)",    "p_{T}(j_{5}) [GeV]",  nJetPt_Zinc5jet,   jetPt_Zinc5jet);
    FifthJetPt_1_Zinc5jet      = newTH1D("FifthJetPt_1_Zinc5jet",     "5th jet p_{T} (N_{jets} #geq 5)1",   "p_{T}(j_{5}) [GeV]",  nJetPt_1_Zinc5jet, jetPt_1_Zinc5jet);
    FifthJetPt_2_Zinc5jet      = newTH1D("FifthJetPt_2_Zinc5jet",     "5th jet p_{T} (N_{jets} #geq 5)2",   "p_{T}(j_{5}) [GeV]",   jetPt_2_Zinc5jet);
    
    SixthJetPt_Zinc6jet        = newTH1D("SixthJetPt_Zinc6jet",       "6th jet p_{T} (N_{jets} #geq 6)",    "p_{T}(j_{6}) [GeV]",  nJetPt_Zinc5jet,   jetPt_Zinc5jet);
    SixthJetPt_1_Zinc6jet      = newTH1D("SixthJetPt_1_Zinc6jet",     "6th jet p_{T} (N_{jets} #geq 6)1",   "p_{T}(j_{6}) [GeV]",  nJetPt_1_Zinc5jet, jetPt_1_Zinc5jet);
    
    genFirstJetPt_Zinc1jet     = newTH1D("genFirstJetPt_Zinc1jet",    "gen 1st jet p_{T} (N_{jets} #geq 1)",  "p_{T}(j_{1}) [GeV]",   nJetPt_Zinc1jet,   jetPt_Zinc1jet);
    genFirstJetPt_1_Zinc1jet   = newTH1D("genFirstJetPt_1_Zinc1jet",  "gen 1st jet p_{T} (N_{jets} #geq 1)1", "p_{T}(j_{1}) [GeV]",   nJetPt_1_Zinc1jet, jetPt_1_Zinc1jet);
    genFirstJetPt_2_Zinc1jet   = newTH1D("genFirstJetPt_2_Zinc1jet",  "gen 1st jet p_{T} (N_{jets} #geq 1)2", "p_{T}(j_{1}) [GeV]",    jetPt_2_Zinc1jet);
    
    genSecondJetPt_Zinc2jet    = newTH1D("genSecondJetPt_Zinc2jet",   "gen 2nd jet p_{T} (N_{jets} #geq 2)",  "p_{T}(j_{2}) [GeV]",   nJetPt_Zinc2jet,   jetPt_Zinc2jet);
    genSecondJetPt_1_Zinc2jet  = newTH1D("genSecondJetPt_1_Zinc2jet", "gen 2nd jet p_{T} (N_{jets} #geq 2)1", "p_{T}(j_{2}) [GeV]",   nJetPt_1_Zinc2jet, jetPt_1_Zinc2jet);
    genSecondJetPt_2_Zinc2jet  = newTH1D("genSecondJetPt_2_Zinc2jet", "gen 2nd jet p_{T} (N_{jets} #geq 2)2", "p_{T}(j_{2}) [GeV]",    jetPt_2_Zinc2jet);
    
    genThirdJetPt_Zinc3jet     = newTH1D("genThirdJetPt_Zinc3jet",    "gen 3rd jet p_{T} (N_{jets} #geq 3)",  "p_{T}(j_{3}) [GeV]",   nJetPt_Zinc3jet,   jetPt_Zinc3jet);
    genThirdJetPt_1_Zinc3jet   = newTH1D("genThirdJetPt_1_Zinc3jet",  "gen 3rd jet p_{T} (N_{jets} #geq 3)1", "p_{T}(j_{3}) [GeV]",   nJetPt_1_Zinc3jet, jetPt_1_Zinc3jet);
    genThirdJetPt_2_Zinc3jet   = newTH1D("genThirdJetPt_2_Zinc3jet",  "gen 3rd jet p_{T} (N_{jets} #geq 3)2", "p_{T}(j_{3}) [GeV]",    jetPt_2_Zinc3jet);
    
    genFourthJetPt_Zinc4jet    = newTH1D("genFourthJetPt_Zinc4jet",   "gen 4th jet p_{T} (N_{jets} #geq 4)",  "p_{T}(j_{4}) [GeV]",   nJetPt_Zinc4jet,   jetPt_Zinc4jet);
    genFourthJetPt_1_Zinc4jet  = newTH1D("genFourthJetPt_1_Zinc4jet", "gen 4th jet p_{T} (N_{jets} #geq 4)1", "p_{T}(j_{4}) [GeV]",   nJetPt_1_Zinc4jet, jetPt_1_Zinc4jet);
    genFourthJetPt_2_Zinc4jet  = newTH1D("genFourthJetPt_2_Zinc4jet", "gen 4th jet p_{T} (N_{jets} #geq 4)2", "p_{T}(j_{4}) [GeV]",    jetPt_2_Zinc4jet);
    
    genFifthJetPt_Zinc5jet     = newTH1D("genFifthJetPt_Zinc5jet",    "gen 5th jet p_{T} (N_{jets} #geq 5)",  "p_{T}(j_{5}) [GeV]",   nJetPt_Zinc5jet,   jetPt_Zinc5jet);
    genFifthJetPt_1_Zinc5jet   = newTH1D("genFifthJetPt_1_Zinc5jet",  "gen 5th jet p_{T} (N_{jets} #geq 5)1", "p_{T}(j_{5}) [GeV]",   nJetPt_1_Zinc5jet, jetPt_1_Zinc5jet);
    genFifthJetPt_2_Zinc5jet   = newTH1D("genFifthJetPt_2_Zinc5jet",  "gen 5th jet p_{T} (N_{jets} #geq 5)2", "p_{T}(j_{5}) [GeV]",    jetPt_2_Zinc5jet);
    
    genSixthJetPt_Zinc6jet     = newTH1D("genSixthJetPt_Zinc6jet",    "gen 6th jet p_{T} (N_{jets} #geq 6)",  "p_{T}(j_{6}) [GeV]",   nJetPt_Zinc5jet,   jetPt_Zinc5jet);
    genSixthJetPt_1_Zinc6jet   = newTH1D("genSixthJetPt_1_Zinc6jet",  "gen 6th jet p_{T} (N_{jets} #geq 6)1", "p_{T}(j_{6}) [GeV]",   nJetPt_1_Zinc5jet, jetPt_1_Zinc5jet);
    
    hresponseFirstJetPt_Zinc1jet      = newTH2D("hresponseFirstJetPt_Zinc1jet", "hresp 1st jet pt", nJetPt_Zinc1jet, jetPt_Zinc1jet, nJetPt_Zinc1jet, jetPt_Zinc1jet);
    hresponseSecondJetPt_Zinc2jet     = newTH2D("hresponseSecondJetPt_Zinc2jet","hresp 2nd jet pt", nJetPt_Zinc2jet, jetPt_Zinc2jet, nJetPt_Zinc2jet, jetPt_Zinc2jet);
    hresponseThirdJetPt_Zinc3jet      = newTH2D("hresponseThirdJetPt_Zinc3jet", "hresp 3rd jet pt", nJetPt_Zinc3jet, jetPt_Zinc3jet, nJetPt_Zinc3jet, jetPt_Zinc3jet);
    hresponseFourthJetPt_Zinc4jet     = newTH2D("hresponseFourthJetPt_Zinc4jet","hresp 4th jet pt", nJetPt_Zinc4jet, jetPt_Zinc4jet, nJetPt_Zinc4jet, jetPt_Zinc4jet);
    hresponseFifthJetPt_Zinc5jet      = newTH2D("hresponseFifthJetPt_Zinc5jet", "hresp 5th jet pt", nJetPt_Zinc5jet, jetPt_Zinc5jet, nJetPt_Zinc5jet, jetPt_Zinc5jet);
    
    hresponseFirstJetPt_1_Zinc1jet      = newTH2D("hresponseFirstJetPt_1_Zinc1jet", "hresp 1st jet pt ()1", nJetPt_1_Zinc1jet, jetPt_1_Zinc1jet, nJetPt_1_Zinc1jet, jetPt_1_Zinc1jet);
    hresponseSecondJetPt_1_Zinc2jet     = newTH2D("hresponseSecondJetPt_1_Zinc2jet","hresp 2nd jet pt ()1", nJetPt_1_Zinc2jet, jetPt_1_Zinc2jet, nJetPt_1_Zinc2jet, jetPt_1_Zinc2jet);
    hresponseThirdJetPt_1_Zinc3jet      = newTH2D("hresponseThirdJetPt_1_Zinc3jet", "hresp 3rd jet pt ()1", nJetPt_1_Zinc3jet, jetPt_1_Zinc3jet, nJetPt_1_Zinc3jet, jetPt_1_Zinc3jet);
    hresponseFourthJetPt_1_Zinc4jet     = newTH2D("hresponseFourthJetPt_1_Zinc4jet","hresp 4th jet pt ()1", nJetPt_1_Zinc4jet, jetPt_1_Zinc4jet, nJetPt_1_Zinc4jet, jetPt_1_Zinc4jet);
    hresponseFifthJetPt_1_Zinc5jet      = newTH2D("hresponseFifthJetPt_1_Zinc5jet", "hresp 5th jet pt ()1", nJetPt_1_Zinc5jet, jetPt_1_Zinc5jet, nJetPt_1_Zinc5jet, jetPt_1_Zinc5jet);
    
    hresponseFirstJetPt_2_Zinc1jet      = newTH2D("hresponseFirstJetPt_2_Zinc1jet", "hresp 1st jet pt ()2", jetPt_2_Zinc1jet, jetPt_2_Zinc1jet);
    hresponseSecondJetPt_2_Zinc2jet     = newTH2D("hresponseSecondJetPt_2_Zinc2jet","hresp 2nd jet pt ()2", jetPt_2_Zinc2jet, jetPt_2_Zinc2jet);
    hresponseThirdJetPt_2_Zinc3jet      = newTH2D("hresponseThirdJetPt_2_Zinc3jet", "hresp 3rd jet pt ()2", jetPt_2_Zinc3jet, jetPt_2_Zinc3jet);
    hresponseFourthJetPt_2_Zinc4jet     = newTH2D("hresponseFourthJetPt_2_Zinc4jet","hresp 4th jet pt ()2", jetPt_2_Zinc4jet, jetPt_2_Zinc4jet);
    hresponseFifthJetPt_2_Zinc5jet      = newTH2D("hresponseFifthJetPt_2_Zinc5jet", "hresp 5th jet pt ()2", jetPt_2_Zinc5jet, jetPt_2_Zinc5jet);
    
    
    //--- Jet Ht -----------
    JetsHT_Zinc1jet            = newTH1D("JetsHT_Zinc1jet",        "Scalar sum jets p_{T} (N_{jets} #geq 1)",     HT,     nJetHT_Zinc1jet, jetHT_Zinc1jet);
    JetsHT_20_Zinc1jet         = newTH1D("JetsHT_20_Zinc1jet",        "Scalar sum jets p_{T} (N_{jets} #geq 1)",     HT,     nJetHT_20_Zinc1jet, jetHT_20_Zinc1jet);
    JetsHT_20_30_Zinc1jet      = newTH1D("JetsHT_20_30_Zinc1jet",        "Scalar sum jets p_{T} (N_{jets} #geq 1)",     HT,     nJetHT_20_30_Zinc1jet, jetHT_20_30_Zinc1jet);
    JetsHT_1_Zinc1jet          = newTH1D("JetsHT_1_Zinc1jet",      "Scalar sum jets p_{T} (N_{jets} #geq 1)1",    HT,     nJetHT_1_Zinc1jet, jetHT_1_Zinc1jet);
    JetsHT_2_Zinc1jet          = newTH1D("JetsHT_2_Zinc1jet",      "Scalar sum jets p_{T} (N_{jets} #geq 1)2",    HT,     jetHT_2_Zinc1jet);
    
    JetsHT_Zinc2jet            = newTH1D("JetsHT_Zinc2jet",        "Scalar sum jets p_{T} (N_{jets} #geq 2)",     HT,     nJetHT_Zinc2jet, jetHT_Zinc2jet);
    JetsHT_20_Zinc2jet         = newTH1D("JetsHT_20_Zinc2jet",        "Scalar sum jets p_{T} (N_{jets} #geq 2)",     HT,     nJetHT_20_Zinc2jet, jetHT_20_Zinc2jet);
    JetsHT_20_30_Zinc2jet      = newTH1D("JetsHT_20_30_Zinc2jet",        "Scalar sum jets p_{T} (N_{jets} #geq 2)",     HT,     nJetHT_20_30_Zinc2jet, jetHT_20_30_Zinc2jet);
    JetsHT_1_Zinc2jet          = newTH1D("JetsHT_1_Zinc2jet",      "Scalar sum jets p_{T} (N_{jets} #geq 2)1",    HT,     nJetHT_1_Zinc2jet, jetHT_1_Zinc2jet);
    JetsHT_2_Zinc2jet          = newTH1D("JetsHT_2_Zinc2jet",      "Scalar sum jets p_{T} (N_{jets} #geq 2)2",    HT,     jetHT_2_Zinc2jet);
    
    JetsHT_Zinc3jet            = newTH1D("JetsHT_Zinc3jet",                     "Scalar sum jets p_{T} (N_{jets} #geq 3)",     HT,     nJetHT_Zinc3jet, jetHT_Zinc3jet);
    JetsHT_20_Zinc3jet         = newTH1D("JetsHT_20_Zinc3jet",                 "Scalar sum jets p_{T} (N_{jets} #geq 3)",     HT,     nJetHT_20_Zinc3jet, jetHT_20_Zinc3jet);
    JetsHT_20_30_Zinc3jet      = newTH1D("JetsHT_20_30_Zinc3jet",        "Scalar sum jets p_{T} (N_{jets} #geq 3)",     HT,     nJetHT_20_30_Zinc3jet, jetHT_20_30_Zinc3jet);
    JetsHT_1_Zinc3jet          = newTH1D("JetsHT_1_Zinc3jet",                   "Scalar sum jets p_{T} (N_{jets} #geq 3)1",    HT,     nJetHT_1_Zinc3jet, jetHT_1_Zinc3jet);
    JetsHT_2_Zinc3jet          = newTH1D("JetsHT_2_Zinc3jet",                   "Scalar sum jets p_{T} (N_{jets} #geq 3)2",    HT,      jetHT_2_Zinc3jet);
    
    JetsHT_Zinc4jet            = newTH1D("JetsHT_Zinc4jet",                     "Scalar sum jets p_{T} (N_{jets} #geq 4)",     HT,     nJetHT_Zinc4jet, jetHT_Zinc4jet);
    JetsHT_1_Zinc4jet          = newTH1D("JetsHT_1_Zinc4jet",                   "Scalar sum jets p_{T} (N_{jets} #geq 4)1",    HT,     nJetHT_1_Zinc4jet, jetHT_1_Zinc4jet);
    JetsHT_2_Zinc4jet          = newTH1D("JetsHT_2_Zinc4jet",                   "Scalar sum jets p_{T} (N_{jets} #geq 4)2",    HT,      jetHT_2_Zinc4jet);
    
    JetsHT_Zinc5jet            = newTH1D("JetsHT_Zinc5jet",                     "Scalar sum jets p_{T} (N_{jets} #geq 5)",     HT,     nJetHT_Zinc5jet, jetHT_Zinc5jet);
    JetsHT_1_Zinc5jet          = newTH1D("JetsHT_1_Zinc5jet",                   "Scalar sum jets p_{T} (N_{jets} #geq 5)1",    HT,     nJetHT_1_Zinc5jet, jetHT_1_Zinc5jet);
    JetsHT_2_Zinc5jet          = newTH1D("JetsHT_2_Zinc5jet",                   "Scalar sum jets p_{T} (N_{jets} #geq 5)2",    HT,      jetHT_2_Zinc5jet);
    
    JetsHT_Zinc6jet            = newTH1D("JetsHT_Zinc6jet",                     "Scalar sum jets p_{T} (N_{jets} #geq 6)",     HT,     nJetHT_Zinc5jet, jetHT_Zinc5jet);
    JetsHT_1_Zinc6jet          = newTH1D("JetsHT_1_Zinc6jet",                   "Scalar sum jets p_{T} (N_{jets} #geq 6)1",    HT,     nJetHT_1_Zinc5jet, jetHT_1_Zinc5jet);
    
    genJetsHT_Zinc1jet         = newTH1D("genJetsHT_Zinc1jet",                  "Scalar sum jets p_{T} (N_{jets} #geq 1)",     HT,     nJetHT_Zinc1jet, jetHT_Zinc1jet);
    genJetsHT_20_Zinc1jet      = newTH1D("genJetsHT_20_Zinc1jet",              "Scalar sum jets p_{T} (N_{jets} #geq 1)",     HT,     nJetHT_20_Zinc1jet, jetHT_20_Zinc1jet);
    genJetsHT_20_30_Zinc1jet   = newTH1D("genJetsHT_20_30_Zinc1jet",     "Scalar sum jets p_{T} (N_{jets} #geq 1)",     HT,     nJetHT_20_30_Zinc1jet, jetHT_20_30_Zinc1jet);
    genJetsHT_1_Zinc1jet       = newTH1D("genJetsHT_1_Zinc1jet",                "Scalar sum jets p_{T} (N_{jets} #geq 1)1",    HT,     nJetHT_1_Zinc1jet, jetHT_1_Zinc1jet);
    genJetsHT_2_Zinc1jet       = newTH1D("genJetsHT_2_Zinc1jet",                "Scalar sum jets p_{T} (N_{jets} #geq 1)2",    HT,      jetHT_2_Zinc1jet);
    
    genJetsHT_Zinc2jet         = newTH1D("genJetsHT_Zinc2jet",                  "Scalar sum jets p_{T} (N_{jets} #geq 2)",     HT,     nJetHT_Zinc2jet, jetHT_Zinc2jet);
    genJetsHT_20_Zinc2jet      = newTH1D("genJetsHT_20_Zinc2jet",              "Scalar sum jets p_{T} (N_{jets} #geq 2)",     HT,     nJetHT_20_Zinc2jet, jetHT_20_Zinc2jet);
    genJetsHT_20_30_Zinc2jet   = newTH1D("genJetsHT_20_30_Zinc2jet",     "Scalar sum jets p_{T} (N_{jets} #geq 2)",     HT,     nJetHT_20_30_Zinc2jet, jetHT_20_30_Zinc2jet);
    genJetsHT_1_Zinc2jet       = newTH1D("genJetsHT_1_Zinc2jet",                "Scalar sum jets p_{T} (N_{jets} #geq 2)1",    HT,     nJetHT_1_Zinc2jet, jetHT_1_Zinc2jet);
    genJetsHT_2_Zinc2jet       = newTH1D("genJetsHT_2_Zinc2jet",                "Scalar sum jets p_{T} (N_{jets} #geq 2)2",    HT,      jetHT_2_Zinc2jet);
    
    genJetsHT_Zinc3jet         = newTH1D("genJetsHT_Zinc3jet",                  "Scalar sum jets p_{T} (N_{jets} #geq 3)",     HT,     nJetHT_Zinc3jet, jetHT_Zinc3jet);
    genJetsHT_20_Zinc3jet      = newTH1D("genJetsHT_20_Zinc3jet",              "Scalar sum jets p_{T} (N_{jets} #geq 3)",     HT,     nJetHT_20_Zinc3jet, jetHT_20_Zinc3jet);
    genJetsHT_20_30_Zinc3jet   = newTH1D("genJetsHT_20_30_Zinc3jet",     "Scalar sum jets p_{T} (N_{jets} #geq 3)",     HT,     nJetHT_20_30_Zinc3jet, jetHT_20_30_Zinc3jet);
    genJetsHT_1_Zinc3jet       = newTH1D("genJetsHT_1_Zinc3jet",                "Scalar sum jets p_{T} (N_{jets} #geq 3)1",    HT,     nJetHT_1_Zinc3jet, jetHT_1_Zinc3jet);
    genJetsHT_2_Zinc3jet       = newTH1D("genJetsHT_2_Zinc3jet",                "Scalar sum jets p_{T} (N_{jets} #geq 3)2",    HT,      jetHT_2_Zinc3jet);
    
    genJetsHT_Zinc4jet         = newTH1D("genJetsHT_Zinc4jet",                  "Scalar sum jets p_{T} (N_{jets} #geq 4)",     HT,     nJetHT_Zinc4jet, jetHT_Zinc4jet);
    genJetsHT_1_Zinc4jet       = newTH1D("genJetsHT_1_Zinc4jet",                "Scalar sum jets p_{T} (N_{jets} #geq 4)1",    HT,     nJetHT_1_Zinc4jet, jetHT_1_Zinc4jet);
    genJetsHT_2_Zinc4jet       = newTH1D("genJetsHT_2_Zinc4jet",                "Scalar sum jets p_{T} (N_{jets} #geq 4)2",    HT,      jetHT_2_Zinc4jet);
    
    genJetsHT_Zinc5jet         = newTH1D("genJetsHT_Zinc5jet",                  "Scalar sum jets p_{T} (N_{jets} #geq 5)",     HT,     nJetHT_Zinc5jet, jetHT_Zinc5jet);
    genJetsHT_1_Zinc5jet       = newTH1D("genJetsHT_1_Zinc5jet",                "Scalar sum jets p_{T} (N_{jets} #geq 5)1",    HT,     nJetHT_1_Zinc5jet, jetHT_1_Zinc5jet);
    genJetsHT_2_Zinc5jet       = newTH1D("genJetsHT_2_Zinc5jet",                "Scalar sum jets p_{T} (N_{jets} #geq 5)2",    HT,      jetHT_2_Zinc5jet);
    
    genJetsHT_Zinc6jet         = newTH1D("genJetsHT_Zinc6jet",                  "Scalar sum jets p_{T} (N_{jets} #geq 6)",     HT,     nJetHT_Zinc5jet, jetHT_Zinc5jet);
    genJetsHT_1_Zinc6jet       = newTH1D("genJetsHT_1_Zinc6jet",                "Scalar sum jets p_{T} (N_{jets} #geq 6)1",    HT,     nJetHT_1_Zinc5jet, jetHT_1_Zinc5jet);
    
    
    hresponseJetsHT_Zinc1jet = newTH2D("hresponseJetsHT_Zinc1jet", "hresp Scalar sum jets p_{T} (N_{jets} #geq 1)", nJetHT_Zinc1jet, jetHT_Zinc1jet, nJetHT_Zinc1jet, jetHT_Zinc1jet);
    hresponseJetsHT_20_Zinc1jet = newTH2D("hresponseJetsHT_20_Zinc1jet", "hresp Scalar sum jets p_{T} (N_{jets} #geq 1)", nJetHT_20_Zinc1jet, jetHT_20_Zinc1jet, nJetHT_20_Zinc1jet, jetHT_20_Zinc1jet);
    hresponseJetsHT_20_30_Zinc1jet = newTH2D("hresponseJetsHT_20_30_Zinc1jet", "hresp Scalar sum jets p_{T} (N_{jets} #geq 1)", nJetHT_20_30_Zinc1jet, jetHT_20_30_Zinc1jet, nJetHT_20_30_Zinc1jet, jetHT_20_30_Zinc1jet);
    hresponseJetsHT_Zinc2jet = newTH2D("hresponseJetsHT_Zinc2jet", "hresp Scalar sum jets p_{T} (N_{jets} #geq 2)", nJetHT_Zinc2jet, jetHT_Zinc2jet, nJetHT_Zinc2jet, jetHT_Zinc2jet);
    hresponseJetsHT_20_Zinc2jet = newTH2D("hresponseJetsHT_20_Zinc2jet", "hresp Scalar sum jets p_{T} (N_{jets} #geq 2)", nJetHT_20_Zinc2jet, jetHT_20_Zinc2jet, nJetHT_20_Zinc2jet, jetHT_20_Zinc2jet);
    hresponseJetsHT_20_30_Zinc2jet = newTH2D("hresponseJetsHT_20_30_Zinc2jet", "hresp Scalar sum jets p_{T} (N_{jets} #geq 2)", nJetHT_20_30_Zinc2jet, jetHT_20_30_Zinc2jet, nJetHT_20_30_Zinc2jet, jetHT_20_30_Zinc2jet);
    hresponseJetsHT_Zinc3jet = newTH2D("hresponseJetsHT_Zinc3jet", "hresp Scalar sum jets p_{T} (N_{jets} #geq 3)", nJetHT_Zinc3jet, jetHT_Zinc3jet, nJetHT_Zinc3jet, jetHT_Zinc3jet);
    hresponseJetsHT_20_Zinc3jet = newTH2D("hresponseJetsHT_20_Zinc3jet", "hresp Scalar sum jets p_{T} (N_{jets} #geq 3)", nJetHT_20_Zinc3jet, jetHT_20_Zinc3jet, nJetHT_20_Zinc3jet, jetHT_20_Zinc3jet);
    hresponseJetsHT_20_30_Zinc3jet = newTH2D("hresponseJetsHT_20_30_Zinc3jet", "hresp Scalar sum jets p_{T} (N_{jets} #geq 3)", nJetHT_20_30_Zinc3jet, jetHT_20_30_Zinc3jet, nJetHT_20_30_Zinc3jet, jetHT_20_30_Zinc3jet);
    hresponseJetsHT_Zinc4jet = newTH2D("hresponseJetsHT_Zinc4jet", "hresp Scalar sum jets p_{T} (N_{jets} #geq 4)", nJetHT_Zinc4jet, jetHT_Zinc4jet, nJetHT_Zinc4jet, jetHT_Zinc4jet);
    hresponseJetsHT_Zinc5jet = newTH2D("hresponseJetsHT_Zinc5jet", "hresp Scalar sum jets p_{T} (N_{jets} #geq 5)", nJetHT_Zinc5jet, jetHT_Zinc5jet, nJetHT_Zinc5jet, jetHT_Zinc5jet);
    
    hresponseJetsHT_1_Zinc1jet = newTH2D("hresponseJetsHT_1_Zinc1jet", "hresp Scalar sum jets p_{T} (N_{jets} #geq 1)1", nJetHT_1_Zinc1jet, jetHT_1_Zinc1jet, nJetHT_1_Zinc1jet, jetHT_1_Zinc1jet);
    hresponseJetsHT_1_Zinc2jet = newTH2D("hresponseJetsHT_1_Zinc2jet", "hresp Scalar sum jets p_{T} (N_{jets} #geq 2)1", nJetHT_1_Zinc2jet, jetHT_1_Zinc2jet, nJetHT_1_Zinc2jet, jetHT_1_Zinc2jet);
    hresponseJetsHT_1_Zinc3jet = newTH2D("hresponseJetsHT_1_Zinc3jet", "hresp Scalar sum jets p_{T} (N_{jets} #geq 3)1", nJetHT_1_Zinc3jet, jetHT_1_Zinc3jet, nJetHT_1_Zinc3jet, jetHT_1_Zinc3jet);
    hresponseJetsHT_1_Zinc4jet = newTH2D("hresponseJetsHT_1_Zinc4jet", "hresp Scalar sum jets p_{T} (N_{jets} #geq 4)1", nJetHT_1_Zinc4jet, jetHT_1_Zinc4jet, nJetHT_1_Zinc4jet, jetHT_1_Zinc4jet);
    hresponseJetsHT_1_Zinc5jet = newTH2D("hresponseJetsHT_1_Zinc5jet", "hresp Scalar sum jets p_{T} (N_{jets} #geq 5)1", nJetHT_1_Zinc5jet, jetHT_1_Zinc5jet, nJetHT_1_Zinc5jet, jetHT_1_Zinc5jet);
    
    hresponseJetsHT_2_Zinc1jet = newTH2D("hresponseJetsHT_2_Zinc1jet", "hresp Scalar sum jets p_{T} (N_{jets} #geq 1)2", jetHT_2_Zinc1jet, jetHT_2_Zinc1jet);
    hresponseJetsHT_2_Zinc2jet = newTH2D("hresponseJetsHT_2_Zinc2jet", "hresp Scalar sum jets p_{T} (N_{jets} #geq 2)2", jetHT_2_Zinc2jet, jetHT_2_Zinc2jet);
    hresponseJetsHT_2_Zinc3jet = newTH2D("hresponseJetsHT_2_Zinc3jet", "hresp Scalar sum jets p_{T} (N_{jets} #geq 3)2", jetHT_2_Zinc3jet, jetHT_2_Zinc3jet);
    hresponseJetsHT_2_Zinc4jet = newTH2D("hresponseJetsHT_2_Zinc4jet", "hresp Scalar sum jets p_{T} (N_{jets} #geq 4)2", jetHT_2_Zinc4jet, jetHT_2_Zinc4jet);
    hresponseJetsHT_2_Zinc5jet = newTH2D("hresponseJetsHT_2_Zinc5jet", "hresp Scalar sum jets p_{T} (N_{jets} #geq 5)2", jetHT_2_Zinc5jet, jetHT_2_Zinc5jet);
    
    
    //--- Jet eta -----------
    FirstJetEta_Zinc1jet                = newTH1D("FirstJetEta_Zinc1jet",                "1st jet #eta (N_{jets} #geq 1)",              "|#eta(j_{1})|",   32, 0., 2.4);
    FirstJetEta_2_Zinc1jet              = newTH1D("FirstJetEta_2_Zinc1jet",              "1st jet #eta (N_{jets} #geq 1)2",             "|#eta(j_{1})|",  160, 0., 2.4);
    
    SecondJetEta_Zinc2jet               = newTH1D("SecondJetEta_Zinc2jet",               "2nd jet #eta (N_{jets} #geq 2)",              "|#eta(j_{2})|",   32, 0., 2.4);
    SecondJetEta_2_Zinc2jet             = newTH1D("SecondJetEta_2_Zinc2jet",             "2nd jet #eta (N_{jets} #geq 2)2",             "|#eta(j_{2})|",  160, 0., 2.4);
    
    ThirdJetEta_Zinc3jet                = newTH1D("ThirdJetEta_Zinc3jet",                "3rd jet #eta (N_{jets} #geq 3)",              "|#eta(j_{3})|",   12, 0., 2.4);
    ThirdJetEta_2_Zinc3jet              = newTH1D("ThirdJetEta_2_Zinc3jet",              "3rd jet #eta (N_{jets} #geq 3)2",             "|#eta(j_{3})|",  60, 0., 2.4);
    
    FourthJetEta_Zinc4jet               = newTH1D("FourthJetEta_Zinc4jet",               "4th jet #eta (N_{jets} #geq 4)",              "|#eta(j_{4})|",  12, 0., 2.4);
    FourthJetEta_2_Zinc4jet             = newTH1D("FourthJetEta_2_Zinc4jet",             "4th jet #eta (N_{jets} #geq 4)2",             "|#eta(j_{4})|",  60, 0., 2.4);
    
    FifthJetEta_Zinc5jet                = newTH1D("FifthJetEta_Zinc5jet",                "5th jet #eta (N_{jets} #geq 5)",              "|#eta(j_{5})|",    6, 0., 2.4);
    FifthJetEta_2_Zinc5jet              = newTH1D("FifthJetEta_2_Zinc5jet",              "5th jet #eta (N_{jets} #geq 5)2",             "|#eta(j_{5})|",   30, 0., 2.4);
    
    SixthJetEta_Zinc6jet                = newTH1D("SixthJetEta_Zinc6jet",                "#geq 6th jets #eta (N_{jets} #geq 6)",        "|#eta(j_{6})|",   6, 0., 2.4);
    

    genFirstJetEta_Zinc1jet             = newTH1D("genFirstJetEta_Zinc1jet",             "gen 1st jet #eta (N_{jets} #geq 1)",          "|#eta(j_{1})|",  32, 0., 2.4);
    genFirstJetEta_2_Zinc1jet           = newTH1D("genFirstJetEta_2_Zinc1jet",           "gen 1st jet #eta (N_{jets} #geq 1)2",         "|#eta(j_{1})|",  160, 0., 2.4);
    
    genSecondJetEta_Zinc2jet            = newTH1D("genSecondJetEta_Zinc2jet",            "gen 2nd jet #eta (N_{jets} #geq 2)",          "|#eta(j_{2})|",  32, 0., 2.4);
    genSecondJetEta_2_Zinc2jet          = newTH1D("genSecondJetEta_2_Zinc2jet",          "gen 2nd jet #eta (N_{jets} #geq 2)2",         "|#eta(j_{2})|",  160, 0., 2.4);
    
    genThirdJetEta_Zinc3jet             = newTH1D("genThirdJetEta_Zinc3jet",             "gen 3rd jet #eta (N_{jets} #geq 3)",          "|#eta(j_{3})|",  12, 0., 2.4);
    genThirdJetEta_2_Zinc3jet           = newTH1D("genThirdJetEta_2_Zinc3jet",           "gen 3rd jet #eta (N_{jets} #geq 3)2",         "|#eta(j_{3})|",  60, 0., 2.4);
    
    genFourthJetEta_Zinc4jet            = newTH1D("genFourthJetEta_Zinc4jet",            "gen 4th jet #eta (N_{jets} #geq 4)",          "|#eta(j_{4})|",  12, 0., 2.4);
    genFourthJetEta_2_Zinc4jet          = newTH1D("genFourthJetEta_2_Zinc4jet",          "gen 4th jet #eta (N_{jets} #geq 4)2",         "|#eta(j_{4})|",  60, 0., 2.4);
    
    genFifthJetEta_Zinc5jet             = newTH1D("genFifthJetEta_Zinc5jet",             "gen 5th jet #eta (N_{jets} #geq 5)",          "|#eta(j_{5})|",   6, 0., 2.4);
    genFifthJetEta_2_Zinc5jet           = newTH1D("genFifthJetEta_2_Zinc5jet",           "gen 5th jet #eta (N_{jets} #geq 5)2",         "|#eta(j_{5})|",  30, 0., 2.4);
    
    genSixthJetEta_Zinc6jet             = newTH1D("genSixthJetEta_Zinc6jet",             "gen #geq 6th jets #eta (N_{jets} #geq 6)",    "|#eta(j_{6})|",   6, 0., 2.4);
    
    hresponseFirstJetEta_Zinc1jet     = newTH2D("hresponseFirstJetEta_Zinc1jet",  "hresp 1st jet #eta (N_{jets} #geq 1)", 32, 0, 2.4, 32, 0, 2.4);
    hresponseSecondJetEta_Zinc2jet    = newTH2D("hresponseSecondJetEta_Zinc2jet", "hresp 2nd jet #eta (N_{jets} #geq 2)", 32, 0, 2.4, 32, 0, 2.4);
    hresponseThirdJetEta_Zinc3jet     = newTH2D("hresponseThirdJetEta_Zinc3jet",  "hresp 3rd jet #eta (N_{jets} #geq 3)", 12, 0, 2.4, 12, 0, 2.4);
    hresponseFourthJetEta_Zinc4jet    = newTH2D("hresponseFourthJetEta_Zinc4jet", "hresp 4th jet #eta (N_{jets} #geq 4)", 12, 0, 2.4, 12, 0, 2.4);
    hresponseFifthJetEta_Zinc5jet     = newTH2D("hresponseFifthJetEta_Zinc5jet",  "hresp 5th jet #eta (N_{jets} #geq 5)",  6, 0, 2.4,  6, 0, 2.4);
    
    hresponseFirstJetEta_2_Zinc1jet     = newTH2D("hresponseFirstJetEta_2_Zinc1jet",  "hresp 1st jet #eta (N_{jets} #geq 1)2", 160, 0., 2.4, 160, 0., 2.4);
    hresponseSecondJetEta_2_Zinc2jet    = newTH2D("hresponseSecondJetEta_2_Zinc2jet", "hresp 2nd jet #eta (N_{jets} #geq 2)2", 160, 0., 2.4, 160, 0., 2.4);
    hresponseThirdJetEta_2_Zinc3jet     = newTH2D("hresponseThirdJetEta_2_Zinc3jet",  "hresp 3rd jet #eta (N_{jets} #geq 3)2", 60,  0., 2.4,  60, 0., 2.4);
    hresponseFourthJetEta_2_Zinc4jet    = newTH2D("hresponseFourthJetEta_2_Zinc4jet", "hresp 4th jet #eta (N_{jets} #geq 4)2", 60,  0., 2.4,  60, 0., 2.4);
    hresponseFifthJetEta_2_Zinc5jet     = newTH2D("hresponseFifthJetEta_2_Zinc5jet",  "hresp 5th jet #eta (N_{jets} #geq 5)2", 30,  0., 2.4,  30, 0., 2.4);
    
    //************************************************************** Additional Variables *********************************************************************************//
    
    //------ dRapidityJets -----------
    dRapidityJets_Zinc2jet              = newTH1D("dRapidityJets_Zinc2jet",         "#Delta y btwn jets (N_{jets} #geq 2)",    "#Deltay(j_{1}j_{2})",  20, 0, 4.8);
    dRapidityJets_Zinc3jet              = newTH1D("dRapidityJets_Zinc3jet",         "#Delta y btwn jets (N_{jets} #geq 3)",    "#Deltay(j_{1}j_{2})",  20, 0, 4.8);
    dRapidityJets_Zinc4jet              = newTH1D("dRapidityJets_Zinc4jet",         "#Delta y btwn jets (N_{jets} #geq 4)",    "#Deltay(j_{1}j_{2})",  16, 0, 4.8);
    
    gendRapidityJets_Zinc2jet           = newTH1D("gendRapidityJets_Zinc2jet",      "gen #Delta y btwn jets (N_{jets} #geq 2)",    "#Deltay(j_{1}j_{2})",  20, 0, 4.8);
    gendRapidityJets_Zinc3jet           = newTH1D("gendRapidityJets_Zinc3jet",      "gen #Delta y btwn jets (N_{jets} #geq 3)",    "#Deltay(j_{1}j_{2})",  20, 0, 4.8);
    gendRapidityJets_Zinc4jet           = newTH1D("gendRapidityJets_Zinc4jet",      "gen #Delta y btwn jets (N_{jets} #geq 4)",    "#Deltay(j_{1}j_{2})",  16, 0, 4.8);
    
    hresponsedRapidityJets_Zinc2jet = newTH2D("hresponsedRapidityJets_Zinc2jet",  "hresp #Delta y btwn jets (N_{jets} #geq 2)", 20, 0, 4.8, 20, 0, 4.8);
    hresponsedRapidityJets_Zinc3jet = newTH2D("hresponsedRapidityJets_Zinc3jet",  "hresp #Delta y btwn jets (N_{jets} #geq 3)", 20, 0, 4.8, 20, 0, 4.8);
    hresponsedRapidityJets_Zinc4jet = newTH2D("hresponsedRapidityJets_Zinc4jet",  "hresp #Delta y btwn jets (N_{jets} #geq 4)", 16, 0, 4.8, 16, 0, 4.8);
    
    dRapidityJets_2_Zinc2jet            = newTH1D("dRapidityJets_2_Zinc2jet",         "#Delta y btwn jets (N_{jets} #geq 2)2",    "#Deltay(j_{1}j_{2})",  100, 0, 4.8);
    dRapidityJets_2_Zinc3jet            = newTH1D("dRapidityJets_2_Zinc3jet",         "#Delta y btwn jets (N_{jets} #geq 3)2",    "#Deltay(j_{1}j_{2})",  100, 0, 4.8);
    dRapidityJets_2_Zinc4jet            = newTH1D("dRapidityJets_2_Zinc4jet",         "#Delta y btwn jets (N_{jets} #geq 4)2",    "#Deltay(j_{1}j_{2})",  80, 0, 4.8);
    
    gendRapidityJets_2_Zinc2jet         = newTH1D("gendRapidityJets_2_Zinc2jet",      "gen #Delta y btwn jets (N_{jets} #geq 2)2",    "#Deltay(j_{1}j_{2})",  100, 0, 4.8);
    gendRapidityJets_2_Zinc3jet         = newTH1D("gendRapidityJets_2_Zinc3jet",      "gen #Delta y btwn jets (N_{jets} #geq 3)2",    "#Deltay(j_{1}j_{2})",  100, 0, 4.8);
    gendRapidityJets_2_Zinc4jet         = newTH1D("gendRapidityJets_2_Zinc4jet",      "gen #Delta y btwn jets (N_{jets} #geq 4)2",    "#Deltay(j_{1}j_{2})",  80, 0, 4.8);
    
    hresponsedRapidityJets_2_Zinc2jet = newTH2D("hresponsedRapidityJets_2_Zinc2jet",  "hresp #Delta y btwn jets (N_{jets} #geq 2)2", 100, 0, 4.8, 100, 0, 4.8);
    hresponsedRapidityJets_2_Zinc3jet = newTH2D("hresponsedRapidityJets_2_Zinc3jet",  "hresp #Delta y btwn jets (N_{jets} #geq 3)2", 100, 0, 4.8, 100, 0, 4.8);
    hresponsedRapidityJets_2_Zinc4jet = newTH2D("hresponsedRapidityJets_2_Zinc4jet",  "hresp #Delta y btwn jets (N_{jets} #geq 4)2", 80, 0, 4.8, 80, 0, 4.8);
    
    
    //------ dRapidityJetsFB -----------
    dRapidityJetsFB_Zinc2jet            = newTH1D("dRapidityJetsFB_Zinc2jet",       "#Delta y btwn FBjets (N_{jets} #geq 2)",  "#Deltay(j_{F}j_{B})",  20, 0, 4.8);
    dRapidityJetsFB_Zinc3jet            = newTH1D("dRapidityJetsFB_Zinc3jet",       "#Delta y btwn FBjets (N_{jets} #geq 3)",  "#Deltay(j_{F}j_{B})",  20, 0, 4.8);
    dRapidityJetsFB_Zinc4jet            = newTH1D("dRapidityJetsFB_Zinc4jet",       "#Delta y btwn FBjets (N_{jets} #geq 4)",  "#Deltay(j_{F}j_{B})",  16, 0, 4.8);
    
    gendRapidityJetsFB_Zinc2jet         = newTH1D("gendRapidityJetsFB_Zinc2jet",    "gen #Delta y btwn FBjets (N_{jets} #geq 2)",  "#Deltay(j_{F}j_{B})",  20, 0, 4.8);
    gendRapidityJetsFB_Zinc3jet         = newTH1D("gendRapidityJetsFB_Zinc3jet",    "gen #Delta y btwn FBjets (N_{jets} #geq 3)",  "#Deltay(j_{F}j_{B})",  20, 0, 4.8);
    gendRapidityJetsFB_Zinc4jet         = newTH1D("gendRapidityJetsFB_Zinc4jet",    "gen #Delta y btwn FBjets (N_{jets} #geq 4)",  "#Deltay(j_{F}j_{B})",  16, 0, 4.8);
    
    hresponsedRapidityJetsFB_Zinc2jet = newTH2D("hresponsedRapidityJetsFB_Zinc2jet", "hresp #Delta y btwn FBjets (N_{jets} #geq 2)", 20, 0, 4.8, 20, 0, 4.8);
    hresponsedRapidityJetsFB_Zinc3jet = newTH2D("hresponsedRapidityJetsFB_Zinc3jet", "hresp #Delta y btwn FBjets (N_{jets} #geq 3)", 20, 0, 4.8, 20, 0, 4.8);
    hresponsedRapidityJetsFB_Zinc4jet = newTH2D("hresponsedRapidityJetsFB_Zinc4jet", "hresp #Delta y btwn FBjets (N_{jets} #geq 4)", 16, 0, 4.8, 16, 0, 4.8);
    
    dRapidityJetsFB_2_Zinc2jet      = newTH1D("dRapidityJetsFB_2_Zinc2jet",       "#Delta y btwn FBjets (N_{jets} #geq 2)2",  "#Deltay(j_{F}j_{B})",  100, 0, 4.8);
    dRapidityJetsFB_2_Zinc3jet      = newTH1D("dRapidityJetsFB_2_Zinc3jet",       "#Delta y btwn FBjets (N_{jets} #geq 3)2",  "#Deltay(j_{F}j_{B})",  100, 0, 4.8);
    dRapidityJetsFB_2_Zinc4jet      = newTH1D("dRapidityJetsFB_2_Zinc4jet",       "#Delta y btwn FBjets (N_{jets} #geq 4)2",  "#Deltay(j_{F}j_{B})",  80, 0, 4.8);
    
    gendRapidityJetsFB_2_Zinc2jet   = newTH1D("gendRapidityJetsFB_2_Zinc2jet",    "gen #Delta y btwn FBjets (N_{jets} #geq 2)2",  "#Deltay(j_{F}j_{B})",  100, 0, 4.8);
    gendRapidityJetsFB_2_Zinc3jet   = newTH1D("gendRapidityJetsFB_2_Zinc3jet",    "gen #Delta y btwn FBjets (N_{jets} #geq 3)2",  "#Deltay(j_{F}j_{B})",  100, 0, 4.8);
    gendRapidityJetsFB_2_Zinc4jet   = newTH1D("gendRapidityJetsFB_2_Zinc4jet",    "gen #Delta y btwn FBjets (N_{jets} #geq 4)2",  "#Deltay(j_{F}j_{B})",  80, 0, 4.8);
    
    hresponsedRapidityJetsFB_2_Zinc2jet = newTH2D("hresponsedRapidityJetsFB_2_Zinc2jet", "hresp #Delta y btwn FBjets (N_{jets} #geq 2)2", 100, 0, 4.8, 100, 0, 4.8);
    hresponsedRapidityJetsFB_2_Zinc3jet = newTH2D("hresponsedRapidityJetsFB_2_Zinc3jet", "hresp #Delta y btwn FBjets (N_{jets} #geq 3)2", 100, 0, 4.8, 100, 0, 4.8);
    hresponsedRapidityJetsFB_2_Zinc4jet = newTH2D("hresponsedRapidityJetsFB_2_Zinc4jet", "hresp #Delta y btwn FBjets (N_{jets} #geq 4)2", 80, 0, 4.8, 80, 0, 4.8);
    
    
    //------ dRap 1,3 -----------
    dRapidityJets_First_Third_Zinc3jet  = newTH1D("dRapidityJets_First_Third_Zinc3jet",  "#Delta y btwn1_3 jets (N_{jets} #geq 3)", "#Deltay(j_{1}j_{3})",  20, 0, 4.8);
    dRapidityJets_First_Third_Zinc4jet  = newTH1D("dRapidityJets_First_Third_Zinc4jet",  "#Delta y btwn1_3 jets (N_{jets} #geq 4)", "#Deltay(j_{1}j_{3})",  16, 0, 4.8);
    
    gendRapidityJets_First_Third_Zinc3jet  = newTH1D("gendRapidityJets_First_Third_Zinc3jet",  "gen #Delta y btwn1_3 jets (N_{jets} #geq 3)", "#Deltay(j_{1}j_{3})", 20, 0, 4.8);
    gendRapidityJets_First_Third_Zinc4jet  = newTH1D("gendRapidityJets_First_Third_Zinc4jet",  "gen #Delta y btwn1_3 jets (N_{jets} #geq 4)", "#Deltay(j_{1}j_{3})", 16, 0, 4.8);
    
    hresponsedRapidityJets_First_Third_Zinc3jet  = newTH2D("hresponsedRapidityJets_First_Third_Zinc3jet",  "hresp #Delta y btwn1_3 jets (N_{jets} #geq 3)", 20, 0, 4.8, 20, 0, 4.8);
    hresponsedRapidityJets_First_Third_Zinc4jet  = newTH2D("hresponsedRapidityJets_First_Third_Zinc4jet",  "hresp #Delta y btwn1_3 jets (N_{jets} #geq 4)", 16, 0, 4.8, 16, 0, 4.8);
    
    dRapidityJets_2_First_Third_Zinc3jet  = newTH1D("dRapidityJets_2_First_Third_Zinc3jet",  "#Delta y btwn1_3 jets (N_{jets} #geq 3)2", "#Deltay(j_{1}j_{3})",  100, 0, 4.8);
    dRapidityJets_2_First_Third_Zinc4jet  = newTH1D("dRapidityJets_2_First_Third_Zinc4jet",  "#Delta y btwn1_3 jets (N_{jets} #geq 4)2", "#Deltay(j_{1}j_{3})",   80, 0, 4.8);
    
    gendRapidityJets_2_First_Third_Zinc3jet  = newTH1D("gendRapidityJets_2_First_Third_Zinc3jet",  "gen #Delta y btwn1_3 jets (N_{jets} #geq 3)2", "#Deltay(j_{1}j_{3})", 100, 0, 4.8);
    gendRapidityJets_2_First_Third_Zinc4jet  = newTH1D("gendRapidityJets_2_First_Third_Zinc4jet",  "gen #Delta y btwn1_3 jets (N_{jets} #geq 4)2", "#Deltay(j_{1}j_{3})",  80, 0, 4.8);
    
    hresponsedRapidityJets_2_Second_Third_Zinc3jet =newTH2D("hresponsedRapidityJets_2_Second_Third_Zinc3jet","hresp #Delta y btwn2_3 jets (N_{jets} #geq 3)2",100, 0, 4.8,100, 0, 4.8);
    hresponsedRapidityJets_2_Second_Third_Zinc4jet =newTH2D("hresponsedRapidityJets_2_Second_Third_Zinc4jet","hresp #Delta y btwn2_3 jets (N_{jets} #geq 4)2", 80, 0, 4.8, 80, 0, 4.8);
    
    //------ dRap 2,3 -----------
    dRapidityJets_Second_Third_Zinc3jet = newTH1D("dRapidityJets_Second_Third_Zinc3jet", "#Delta y btwn2_3 jets (N_{jets} #geq 3)", "#Deltay(j_{2}j_{3})",  20, 0, 4.8);
    dRapidityJets_Second_Third_Zinc4jet = newTH1D("dRapidityJets_Second_Third_Zinc4jet", "#Delta y btwn2_3 jets (N_{jets} #geq 4)", "#Deltay(j_{2}j_{3})",  16, 0, 4.8);
    
    gendRapidityJets_Second_Third_Zinc3jet = newTH1D("gendRapidityJets_Second_Third_Zinc3jet", "gen #Delta y btwn2_3 jets (N_{jets} #geq 3)", "#Deltay(j_{2}j_{3})", 20, 0, 4.8);
    gendRapidityJets_Second_Third_Zinc4jet = newTH1D("gendRapidityJets_Second_Third_Zinc4jet", "gen #Delta y btwn2_3 jets (N_{jets} #geq 4)", "#Deltay(j_{2}j_{3})", 16, 0, 4.8);
    
    hresponsedRapidityJets_Second_Third_Zinc3jet = newTH2D("hresponsedRapidityJets_Second_Third_Zinc3jet", "hresp #Delta y btwn2_3 jets (N_{jets} #geq 3)", 20, 0, 4.8, 20, 0, 4.8);
    hresponsedRapidityJets_Second_Third_Zinc4jet = newTH2D("hresponsedRapidityJets_Second_Third_Zinc4jet", "hresp #Delta y btwn2_3 jets (N_{jets} #geq 4)", 16, 0, 4.8, 16, 0, 4.8);
    
    dRapidityJets_2_Second_Third_Zinc3jet = newTH1D("dRapidityJets_2_Second_Third_Zinc3jet", "#Delta y btwn2_3 jets (N_{jets} #geq 3)2", "#Deltay(j_{2}j_{3})",  100, 0, 4.8);
    dRapidityJets_2_Second_Third_Zinc4jet = newTH1D("dRapidityJets_2_Second_Third_Zinc4jet", "#Delta y btwn2_3 jets (N_{jets} #geq 4)2", "#Deltay(j_{2}j_{3})",   80, 0, 4.8);
    
    gendRapidityJets_2_Second_Third_Zinc3jet = newTH1D("gendRapidityJets_2_Second_Third_Zinc3jet", "gen #Delta y btwn2_3 jets (N_{jets} #geq 3)2", "#Deltay(j_{2}j_{3})", 100, 0, 4.8);
    gendRapidityJets_2_Second_Third_Zinc4jet = newTH1D("gendRapidityJets_2_Second_Third_Zinc4jet", "gen #Delta y btwn2_3 jets (N_{jets} #geq 4)2", "#Deltay(j_{2}j_{3})",  80, 0, 4.8);
    
    hresponsedRapidityJets_2_First_Third_Zinc3jet  =newTH2D("hresponsedRapidityJets_2_First_Third_Zinc3jet", "hresp #Delta y btwn1_3 jets (N_{jets} #geq 3)2",100, 0, 4.8,100, 0, 4.8);
    hresponsedRapidityJets_2_First_Third_Zinc4jet  =newTH2D("hresponsedRapidityJets_2_First_Third_Zinc4jet", "hresp #Delta y btwn1_3 jets (N_{jets} #geq 4)2", 80, 0, 4.8, 80, 0, 4.8);
    
    //------ dPhi Jets 1,2 -----------
    dPhiJets_Zinc2jet               = newTH1D("dPhiJets_Zinc2jet",            "#Delta#phi btwn jets (N_{jets} #geq 2)",        jdPhi,  20,  0, PI);
    dPhiJets_Zinc3jet               = newTH1D("dPhiJets_Zinc3jet",            "#Delta#phi btwn jets (N_{jets} #geq 3)",        jdPhi,  16,  0, PI);
    dPhiJets_Zinc4jet               = newTH1D("dPhiJets_Zinc4jet",            "#Delta#phi btwn jets (N_{jets} #geq 4)",        jdPhi,  16,  0, PI);
    
    gendPhiJets_Zinc2jet            = newTH1D("gendPhiJets_Zinc2jet",         "gen #Delta#phi btwn jets (N_{jets} #geq 2)",    jdPhi,  20,  0, PI);
    gendPhiJets_Zinc3jet            = newTH1D("gendPhiJets_Zinc3jet",         "gen #Delta#phi btwn jets (N_{jets} #geq 3)",    jdPhi,  16,  0, PI);
    gendPhiJets_Zinc4jet            = newTH1D("gendPhiJets_Zinc4jet",         "gen #Delta#phi btwn jets (N_{jets} #geq 4)",    jdPhi,  16,  0, PI);
    
    hresponsedPhiJets_Zinc2jet      = newTH2D("hresponsedPhiJets_Zinc2jet",   "hresp #Delta#phi btwn jets (N_{jets} #geq 2)",   20, 0, PI, 20, 0, PI);
    hresponsedPhiJets_Zinc3jet      = newTH2D("hresponsedPhiJets_Zinc3jet",   "hresp #Delta#phi btwn jets (N_{jets} #geq 3)",   16, 0, PI, 16, 0, PI);
    hresponsedPhiJets_Zinc4jet      = newTH2D("hresponsedPhiJets_Zinc4jet",   "hresp #Delta#phi btwn jets (N_{jets} #geq 4)",   16, 0, PI, 16, 0, PI);
    
    dPhiJets_2_Zinc2jet             = newTH1D("dPhiJets_2_Zinc2jet",              "#Delta#phi btwn jets (N_{jets} #geq 2)2",     jdPhi,  100,  0, PI);
    dPhiJets_2_Zinc3jet             = newTH1D("dPhiJets_2_Zinc3jet",              "#Delta#phi btwn jets (N_{jets} #geq 3)2",     jdPhi,   80,  0, PI);
    dPhiJets_2_Zinc4jet             = newTH1D("dPhiJets_2_Zinc4jet",              "#Delta#phi btwn jets (N_{jets} #geq 4)2",     jdPhi,   80,  0, PI);
    
    gendPhiJets_2_Zinc2jet          = newTH1D("gendPhiJets_2_Zinc2jet",         "gen #Delta#phi btwn jets (N_{jets} #geq 2)2",   jdPhi,  100,  0, PI);
    gendPhiJets_2_Zinc3jet          = newTH1D("gendPhiJets_2_Zinc3jet",         "gen #Delta#phi btwn jets (N_{jets} #geq 3)2",   jdPhi,   80,  0, PI);
    gendPhiJets_2_Zinc4jet          = newTH1D("gendPhiJets_2_Zinc4jet",         "gen #Delta#phi btwn jets (N_{jets} #geq 4)2",   jdPhi,   80,  0, PI);
    
    hresponsedPhiJets_2_Zinc2jet    = newTH2D("hresponsedPhiJets_2_Zinc2jet",     "hresp #Delta#phi btwn jets (N_{jets} #geq 2)2",   100, 0, PI, 100, 0, PI);
    hresponsedPhiJets_2_Zinc3jet    = newTH2D("hresponsedPhiJets_2_Zinc3jet",     "hresp #Delta#phi btwn jets (N_{jets} #geq 3)2",   80, 0, PI, 80, 0, PI);
    hresponsedPhiJets_2_Zinc4jet    = newTH2D("hresponsedPhiJets_2_Zinc4jet",     "hresp #Delta#phi btwn jets (N_{jets} #geq 4)2",   80, 0, PI, 80, 0, PI);
    
    //------ dPhi Jets F,B -----------
    dPhiJetsFB_Zinc2jet             = newTH1D("dPhiJetsFB_Zinc2jet",          "#Delta#phi btwn FBjets (N_{jets} #geq 2)", "#Delta#phi(j_{F}j_{B})", 20,  0, PI);
    dPhiJetsFB_Zinc3jet             = newTH1D("dPhiJetsFB_Zinc3jet",          "#Delta#phi btwn FBjets (N_{jets} #geq 3)", "#Delta#phi(j_{F}j_{B})", 16,  0, PI);
    dPhiJetsFB_Zinc4jet             = newTH1D("dPhiJetsFB_Zinc4jet",          "#Delta#phi btwn FBjets (N_{jets} #geq 4)", "#Delta#phi(j_{F}j_{B})", 16,  0, PI);
    
    gendPhiJetsFB_Zinc2jet          = newTH1D("gendPhiJetsFB_Zinc2jet",       "gen #Delta#phi btwn FBjets (N_{jets} #geq 2)", "#Delta#phi(j_{F}j_{B})", 20,  0, PI);
    gendPhiJetsFB_Zinc3jet          = newTH1D("gendPhiJetsFB_Zinc3jet",       "gen #Delta#phi btwn FBjets (N_{jets} #geq 3)", "#Delta#phi(j_{F}j_{B})", 16,  0, PI);
    gendPhiJetsFB_Zinc4jet          = newTH1D("gendPhiJetsFB_Zinc4jet",       "gen #Delta#phi btwn FBjets (N_{jets} #geq 4)", "#Delta#phi(j_{F}j_{B})", 16,  0, PI);
    
    hresponsedPhiJetsFB_Zinc2jet    = newTH2D("hresponsedPhiJetsFB_Zinc2jet", "hresp #Delta#phi btwn FBjets (N_{jets} #geq 2)", 20, 0, PI, 20, 0, PI);
    hresponsedPhiJetsFB_Zinc3jet    = newTH2D("hresponsedPhiJetsFB_Zinc3jet", "hresp #Delta#phi btwn FBjets (N_{jets} #geq 3)", 16, 0, PI, 16, 0, PI);
    hresponsedPhiJetsFB_Zinc4jet    = newTH2D("hresponsedPhiJetsFB_Zinc4jet", "hresp #Delta#phi btwn FBjets (N_{jets} #geq 4)", 16, 0, PI, 16, 0, PI);
    
    dPhiJetsFB_2_Zinc2jet           = newTH1D("dPhiJetsFB_2_Zinc2jet",          "#Delta#phi btwn FBjets (N_{jets} #geq 2)2",    "#Delta#phi(j_{F}j_{B})", 100,  0, PI);
    dPhiJetsFB_2_Zinc3jet           = newTH1D("dPhiJetsFB_2_Zinc3jet",          "#Delta#phi btwn FBjets (N_{jets} #geq 3)2",    "#Delta#phi(j_{F}j_{B})", 80,  0, PI);
    dPhiJetsFB_2_Zinc4jet           = newTH1D("dPhiJetsFB_2_Zinc4jet",          "#Delta#phi btwn FBjets (N_{jets} #geq 4)2",    "#Delta#phi(j_{F}j_{B})", 80,  0, PI);
    
    gendPhiJetsFB_2_Zinc2jet        = newTH1D("gendPhiJetsFB_2_Zinc2jet",     "gen #Delta#phi btwn FBjets (N_{jets} #geq 2)2",   "#Delta#phi(j_{F}j_{B})", 100,  0, PI);
    gendPhiJetsFB_2_Zinc3jet        = newTH1D("gendPhiJetsFB_2_Zinc3jet",     "gen #Delta#phi btwn FBjets (N_{jets} #geq 3)2",   "#Delta#phi(j_{F}j_{B})", 80,  0, PI);
    gendPhiJetsFB_2_Zinc4jet        = newTH1D("gendPhiJetsFB_2_Zinc4jet",     "gen #Delta#phi btwn FBjets (N_{jets} #geq 4)2",   "#Delta#phi(j_{F}j_{B})", 80,  0, PI);
    
    hresponsedPhiJetsFB_2_Zinc2jet  = newTH2D("hresponsedPhiJetsFB_2_Zinc2jet",   "hresp #Delta#phi btwn FBjets (N_{jets} #geq 2)2", 100, 0, PI, 100, 0, PI);
    hresponsedPhiJetsFB_2_Zinc3jet  = newTH2D("hresponsedPhiJetsFB_2_Zinc3jet",   "hresp #Delta#phi btwn FBjets (N_{jets} #geq 3)2", 80, 0, PI, 80, 0, PI);
    hresponsedPhiJetsFB_2_Zinc4jet  = newTH2D("hresponsedPhiJetsFB_2_Zinc4jet",   "hresp #Delta#phi btwn FBjets (N_{jets} #geq 4)2", 80, 0, PI, 80, 0, PI);
     
    //--- dPhi (muon, nth_jet) -----------
    dPhiLepJet1_Zinc1jet    = newTH1D("dPhiLepJet1_Zinc1jet",     "#Delta#phi btwn muon jet1 (N_{jets} #geq 1)", "#Delta#phi(#mu,j_{1})", 20,  0, PI);
    dPhiLepJet2_Zinc2jet    = newTH1D("dPhiLepJet2_Zinc2jet",     "#Delta#phi btwn muon jet2 (N_{jets} #geq 2)", "#Delta#phi(#mu,j_{2})", 20,  0, PI);
    dPhiLepJet3_Zinc3jet    = newTH1D("dPhiLepJet3_Zinc3jet",     "#Delta#phi btwn muon jet3 (N_{jets} #geq 3)", "#Delta#phi(#mu,j_{3})", 16,  0, PI);
    dPhiLepJet4_Zinc4jet    = newTH1D("dPhiLepJet4_Zinc4jet",     "#Delta#phi btwn muon jet4 (N_{jets} #geq 4)", "#Delta#phi(#mu,j_{4})", 16,  0, PI);
    dPhiLepJet5_Zinc5jet    = newTH1D("dPhiLepJet5_Zinc5jet",     "#Delta#phi btwn muon jet5 (N_{jets} #geq 5)", "#Delta#phi(#mu,j_{5})", 12,  0, PI);
    
    gendPhiLepJet1_Zinc1jet    = newTH1D("gendPhiLepJet1_Zinc1jet",     "gen #Delta#phi btwn muon jet1 (N_{jets} #geq 1)", "#Delta#phi(#mu,j_{1})", 20,  0, PI);
    gendPhiLepJet2_Zinc2jet    = newTH1D("gendPhiLepJet2_Zinc2jet",     "gen #Delta#phi btwn muon jet2 (N_{jets} #geq 2)", "#Delta#phi(#mu,j_{2})", 20,  0, PI);
    gendPhiLepJet3_Zinc3jet    = newTH1D("gendPhiLepJet3_Zinc3jet",     "gen #Delta#phi btwn muon jet3 (N_{jets} #geq 3)", "#Delta#phi(#mu,j_{3})", 16,  0, PI);
    gendPhiLepJet4_Zinc4jet    = newTH1D("gendPhiLepJet4_Zinc4jet",     "gen #Delta#phi btwn muon jet4 (N_{jets} #geq 4)", "#Delta#phi(#mu,j_{4})", 16,  0, PI);
    gendPhiLepJet5_Zinc5jet    = newTH1D("gendPhiLepJet5_Zinc5jet",     "gen #Delta#phi btwn muon jet5 (N_{jets} #geq 5)", "#Delta#phi(#mu,j_{5})", 12,  0, PI);
    
    hresponsedPhiLepJet1_Zinc1jet = newTH2D("hresponsedPhiLepJet1_Zinc1jet", "hresp #Delta#phi btwn muon jet1 (N_{jets} #geq 1)", 20, 0, PI, 20, 0, PI);
    hresponsedPhiLepJet2_Zinc2jet = newTH2D("hresponsedPhiLepJet2_Zinc2jet", "hresp #Delta#phi btwn muon jet2 (N_{jets} #geq 2)", 20, 0, PI, 20, 0, PI);
    hresponsedPhiLepJet3_Zinc3jet = newTH2D("hresponsedPhiLepJet3_Zinc3jet", "hresp #Delta#phi btwn muon jet3 (N_{jets} #geq 3)", 16, 0, PI, 16, 0, PI);
    hresponsedPhiLepJet4_Zinc4jet = newTH2D("hresponsedPhiLepJet4_Zinc4jet", "hresp #Delta#phi btwn muon jet4 (N_{jets} #geq 4)", 16, 0, PI, 16, 0, PI);
    hresponsedPhiLepJet5_Zinc5jet = newTH2D("hresponsedPhiLepJet5_Zinc5jet", "hresp #Delta#phi btwn muon jet5 (N_{jets} #geq 5)", 12, 0, PI, 12, 0, PI);
    
    dPhiLepJet1_2_Zinc1jet    = newTH1D("dPhiLepJet1_2_Zinc1jet",     "#Delta#phi btwn muon jet1 (N_{jets} #geq 1)2", "#Delta#phi(#mu,j_{1})", 100,  0, PI);
    dPhiLepJet2_2_Zinc2jet    = newTH1D("dPhiLepJet2_2_Zinc2jet",     "#Delta#phi btwn muon jet2 (N_{jets} #geq 2)2", "#Delta#phi(#mu,j_{2})", 100,  0, PI);
    dPhiLepJet3_2_Zinc3jet    = newTH1D("dPhiLepJet3_2_Zinc3jet",     "#Delta#phi btwn muon jet3 (N_{jets} #geq 3)2", "#Delta#phi(#mu,j_{3})", 80,  0, PI);
    dPhiLepJet4_2_Zinc4jet    = newTH1D("dPhiLepJet4_2_Zinc4jet",     "#Delta#phi btwn muon jet4 (N_{jets} #geq 4)2", "#Delta#phi(#mu,j_{4})", 80,  0, PI);
    dPhiLepJet5_2_Zinc5jet    = newTH1D("dPhiLepJet5_2_Zinc5jet",     "#Delta#phi btwn muon jet5 (N_{jets} #geq 5)2", "#Delta#phi(#mu,j_{5})", 60,  0, PI);
    
    gendPhiLepJet1_2_Zinc1jet    = newTH1D("gendPhiLepJet1_2_Zinc1jet",     "gen #Delta#phi btwn muon jet1 (N_{jets} #geq 1)2", "#Delta#phi(#mu,j_{1})", 100,  0, PI);
    gendPhiLepJet2_2_Zinc2jet    = newTH1D("gendPhiLepJet2_2_Zinc2jet",     "gen #Delta#phi btwn muon jet2 (N_{jets} #geq 2)2", "#Delta#phi(#mu,j_{2})", 100,  0, PI);
    gendPhiLepJet3_2_Zinc3jet    = newTH1D("gendPhiLepJet3_2_Zinc3jet",     "gen #Delta#phi btwn muon jet3 (N_{jets} #geq 3)2", "#Delta#phi(#mu,j_{3})", 80,  0, PI);
    gendPhiLepJet4_2_Zinc4jet    = newTH1D("gendPhiLepJet4_2_Zinc4jet",     "gen #Delta#phi btwn muon jet4 (N_{jets} #geq 4)2", "#Delta#phi(#mu,j_{4})", 80,  0, PI);
    gendPhiLepJet5_2_Zinc5jet    = newTH1D("gendPhiLepJet5_2_Zinc5jet",     "gen #Delta#phi btwn muon jet5 (N_{jets} #geq 5)2", "#Delta#phi(#mu,j_{5})", 60,  0, PI);
    
    hresponsedPhiLepJet1_2_Zinc1jet = newTH2D("hresponsedPhiLepJet1_2_Zinc1jet", "hresp #Delta#phi btwn muon jet1 (N_{jets} #geq 1)2", 100, 0, PI, 100, 0, PI);
    hresponsedPhiLepJet2_2_Zinc2jet = newTH2D("hresponsedPhiLepJet2_2_Zinc2jet", "hresp #Delta#phi btwn muon jet2 (N_{jets} #geq 2)2", 100, 0, PI, 100, 0, PI);
    hresponsedPhiLepJet3_2_Zinc3jet = newTH2D("hresponsedPhiLepJet3_2_Zinc3jet", "hresp #Delta#phi btwn muon jet3 (N_{jets} #geq 3)2", 80, 0, PI, 80, 0, PI);
    hresponsedPhiLepJet4_2_Zinc4jet = newTH2D("hresponsedPhiLepJet4_2_Zinc4jet", "hresp #Delta#phi btwn muon jet4 (N_{jets} #geq 4)2", 80, 0, PI, 80, 0, PI);
    hresponsedPhiLepJet5_2_Zinc5jet = newTH2D("hresponsedPhiLepJet5_2_Zinc5jet", "hresp #Delta#phi btwn muon jet5 (N_{jets} #geq 5)2", 60, 0, PI, 60, 0, PI);
    
	//--- dR (muon, closest jet) -----------
	
	dRLepCloseJet_Zinc1jet			= newTH1D("dRLepCloseJet_Zinc1jet",          "#Delta R btwn muon closest jet (N_{jets} #geq 1)",      "#DeltaR(#mu, closest jet)", 20,  0., 4.);
	gendRLepCloseJet_Zinc1jet		= newTH1D("gendRLepCloseJet_Zinc1jet",         "gen #Delta R btwn muon closest jet (N_{jets} #geq 1)",  "#DeltaR(#mu, closest jet)", 20,  0., 4.);
	hresponsedRLepCloseJet_Zinc1jet = newTH2D("hresponsedRLepCloseJet_Zinc1jet", "hresp #Delta R btwn muon closest jet (N_{jets} #geq 1)", 20,  0., 4., 20,  0., 4.);
	
	dRLepCloseJet_2_Zinc1jet		  = newTH1D("dRLepCloseJet_2_Zinc1jet",          "#Delta R btwn muon closest jet (N_{jets} #geq 1)2",      "#DeltaR(#mu, closest jet)", 100,  0., 4.);
	gendRLepCloseJet_2_Zinc1jet		  = newTH1D("gendRLepCloseJet_2_Zinc1jet",       "gen #Delta R btwn muon closest jet (N_{jets} #geq 1)2",  "#DeltaR(#mu, closest jet)", 100,  0., 4.);
	hresponsedRLepCloseJet_2_Zinc1jet = newTH2D("hresponsedRLepCloseJet_2_Zinc1jet", "hresp #Delta R btwn muon closest jet (N_{jets} #geq 1)2", 100,  0., 4., 100,  0., 4.);
	
	dRLepCloseJetCo_Zinc1jet			= newTH1D("dRLepCloseJetCo_Zinc1jet",          "#Delta R btwn muon closest jet Co (N_{jets} #geq 1)",      "#DeltaR(#mu, closest jet)", 20,  0., 4.);
	gendRLepCloseJetCo_Zinc1jet		= newTH1D("gendRLepCloseJetCo_Zinc1jet",         "gen #Delta R btwn muon closest jet Co (N_{jets} #geq 1)",  "#DeltaR(#mu, closest jet)", 20,  0., 4.);
	hresponsedRLepCloseJetCo_Zinc1jet = newTH2D("hresponsedRLepCloseJetCo_Zinc1jet", "hresp #Delta R btwn muon closest jet Co (N_{jets} #geq 1)", 20,  0., 4., 20,  0., 4.);
	
	dRLepCloseJetCo_2_Zinc1jet		  = newTH1D("dRLepCloseJetCo_2_Zinc1jet",          "#Delta R btwn muon closest jet Co (N_{jets} #geq 1)2",      "#DeltaR(#mu, closest jet)", 100,  0., 4.);
	gendRLepCloseJetCo_2_Zinc1jet		  = newTH1D("gendRLepCloseJetCo_2_Zinc1jet",       "gen #Delta R btwn muon closest jet Co (N_{jets} #geq 1)2",  "#DeltaR(#mu, closest jet)", 100,  0., 4.);
	hresponsedRLepCloseJetCo_2_Zinc1jet = newTH2D("hresponsedRLepCloseJetCo_2_Zinc1jet", "hresp #Delta R btwn muon closest jet Co (N_{jets} #geq 1)2", 100,  0., 4., 100,  0., 4.);
	
	dRptmin100LepCloseJetCo300dR04_Zinc1jet			= newTH1D("dRptmin100LepCloseJetCo300dR04_Zinc1jet",          "#Delta R btwn muon closest jet Co 300 (N_{jets} #geq 1)",      "#DeltaR(#mu, closest jet)", 20,  0., 4.);
	gendRptmin100LepCloseJetCo300dR04_Zinc1jet		= newTH1D("gendRptmin100LepCloseJetCo300dR04_Zinc1jet",         "gen #Delta R btwn muon closest jet Co 300 (N_{jets} #geq 1)",  "#DeltaR(#mu, closest jet)", 20,  0., 4.);
	hresponsedRptmin100LepCloseJetCo300dR04_Zinc1jet = newTH2D("hresponsedRptmin100LepCloseJetCo300dR04_Zinc1jet", "hresp #Delta R btwn muon closest jet Co 300 (N_{jets} #geq 1)", 20,  0., 4., 20,  0., 4.);
	
	dRptmin100LepCloseJetCo300dR04_2_Zinc1jet		   = newTH1D("dRptmin100LepCloseJetCo300dR04_2_Zinc1jet",          "#Delta R btwn muon closest jet Co 300 (N_{jets} #geq 1)2",      "#DeltaR(#mu, closest jet)", 100,  0., 4.);
	gendRptmin100LepCloseJetCo300dR04_2_Zinc1jet	   = newTH1D("gendRptmin100LepCloseJetCo300dR04_2_Zinc1jet",       "gen #Delta R btwn muon closest jet Co 300 (N_{jets} #geq 1)2",  "#DeltaR(#mu, closest jet)", 100,  0., 4.);
	hresponsedRptmin100LepCloseJetCo300dR04_2_Zinc1jet = newTH2D("hresponsedRptmin100LepCloseJetCo300dR04_2_Zinc1jet", "hresp #Delta R btwn muon closest jet Co 300 (N_{jets} #geq 1)2", 100,  0., 4., 100,  0., 4.);

	//--- dR (muon, closest jet in dijet) -----------

	dRptmin100LepCloseDiJetCo300dR04_Zinc2jet			= newTH1D("dRptmin100LepCloseDiJetCo300dR04_Zinc2jet",          "#Delta R btwn muon closest dijet Co 300 (N_{jets} #geq 2)",      "#DeltaR(#mu, closest jet in dijet)", 20,  0., 4.);
	gendRptmin100LepCloseDiJetCo300dR04_Zinc2jet		= newTH1D("gendRptmin100LepCloseDiJetCo300dR04_Zinc2jet",         "gen #Delta R btwn muon closest dijet Co 300 (N_{jets} #geq 2)",  "#DeltaR(#mu, closest jet in dijet)", 20,  0., 4.);
	hresponsedRptmin100LepCloseDiJetCo300dR04_Zinc2jet = newTH2D("hresponsedRptmin100LepCloseDiJetCo300dR04_Zinc2jet", "hresp #Delta R btwn muon closest dijet Co 300 (N_{jets} #geq 2)", 20,  0., 4., 20,  0., 4.);
	
	dRptmin100LepCloseDiJetCo300dR04_2_Zinc2jet		   = newTH1D("dRptmin100LepCloseDiJetCo300dR04_2_Zinc2jet",          "#Delta R btwn muon closest dijet Co 300 (N_{jets} #geq 2)2",      "#DeltaR(#mu, closest jet in dijet)", 100,  0., 4.);
	gendRptmin100LepCloseDiJetCo300dR04_2_Zinc2jet	   = newTH1D("gendRptmin100LepCloseDiJetCo300dR04_2_Zinc2jet",       "gen #Delta R btwn muon closest dijet Co 300 (N_{jets} #geq 2)2",  "#DeltaR(#mu, closest jet in dijet)", 100,  0., 4.);
	hresponsedRptmin100LepCloseDiJetCo300dR04_2_Zinc2jet = newTH2D("hresponsedRptmin100LepCloseDiJetCo300dR04_2_Zinc2jet", "hresp #Delta R btwn muon closest dijet Co 300 (N_{jets} #geq 2)2", 100,  0., 4., 100,  0., 4.);
	
	
    //------ dR Jets -----------
    dRJets_Zinc2jet                 = newTH1D("dRJets_Zinc2jet",              "#Delta R btwn jets (N_{jets} #geq 2)",     "#DeltaR(j_{1}j_{2})",   30,  0., 6.);
    gendRJets_Zinc2jet              = newTH1D("gendRJets_Zinc2jet",             "gen #Delta R btwn jets (N_{jets} #geq 2)",     "#DeltaR(j_{1}j_{2})",   30,  0., 6.);
    hresponsedRJets_Zinc2jet        = newTH2D("hresponsedRJets_Zinc2jet",       "hresp #Delta R btwn jets (N_{jets} #geq 2)", 30,  0., 6., 30,  0., 6.);
    
    dRJets_2_Zinc2jet               = newTH1D("dRJets_2_Zinc2jet",                  "#Delta R btwn jets (N_{jets} #geq 2)2",    "#DeltaR(j_{1}j_{2})",   150,  0., 6.);
    gendRJets_2_Zinc2jet            = newTH1D("gendRJets_2_Zinc2jet",             "gen #Delta R btwn jets (N_{jets} #geq 2)2",     "#DeltaR(j_{1}j_{2})", 150,  0., 6.);
    hresponsedRJets_2_Zinc2jet      = newTH2D("hresponsedRJets_2_Zinc2jet",       "hresp #Delta R btwn jets (N_{jets} #geq 2)2", 150,  0., 6., 150,  0., 6.);
    
    //------  diJetMass -----------
    diJetMass_Zinc2jet         = newTH1D("diJetMass_Zinc2jet",       "2Jets Invariant Mass (N_{jets} #geq 2)",     Mjj, nJetMass_Zinc2jet, ArrJetmass_Zinc2jet);
    diJetMass_Zinc3jet         = newTH1D("diJetMass_Zinc3jet",       "2Jets Invariant Mass (N_{jets} #geq 3)",     Mjj, nJetMass_Zinc3jet, ArrJetmass_Zinc3jet);
    diJetMass_Zinc4jet         = newTH1D("diJetMass_Zinc4jet",       "2Jets Invariant Mass (N_{jets} #geq 4)",     Mjj, nJetMass_Zinc4jet, ArrJetmass_Zinc4jet);
    gendiJetMass_Zinc2jet         = newTH1D("gendiJetMass_Zinc2jet",      "gen 2Jets Invariant Mass (N_{jets} #geq 2)",     Mjj,  nJetMass_Zinc2jet, ArrJetmass_Zinc2jet);
    gendiJetMass_Zinc3jet         = newTH1D("gendiJetMass_Zinc3jet",      "gen 2Jets Invariant Mass (N_{jets} #geq 3)",     Mjj,  nJetMass_Zinc3jet, ArrJetmass_Zinc3jet);
    gendiJetMass_Zinc4jet         = newTH1D("gendiJetMass_Zinc4jet",      "gen 2Jets Invariant Mass (N_{jets} #geq 4)",     Mjj,  nJetMass_Zinc4jet, ArrJetmass_Zinc4jet);
    
    hresponsediJetMass_Zinc2jet = newTH2D("hresponsediJetMass_Zinc2jet", "hresp 2Jets Invariant Mass (N_{jets} #geq 2)", nJetMass_Zinc2jet, ArrJetmass_Zinc2jet, nJetMass_Zinc2jet, ArrJetmass_Zinc2jet);
    hresponsediJetMass_Zinc3jet = newTH2D("hresponsediJetMass_Zinc3jet", "hresp 2Jets Invariant Mass (N_{jets} #geq 3)", nJetMass_Zinc3jet, ArrJetmass_Zinc3jet, nJetMass_Zinc3jet, ArrJetmass_Zinc3jet);
    hresponsediJetMass_Zinc4jet = newTH2D("hresponsediJetMass_Zinc4jet", "hresp 2Jets Invariant Mass (N_{jets} #geq 4)", nJetMass_Zinc4jet, ArrJetmass_Zinc4jet, nJetMass_Zinc4jet, ArrJetmass_Zinc4jet);
    
    diJetMass_2_Zinc2jet                = newTH1D("diJetMass_2_Zinc2jet",             "2Jets Invariant Mass (N_{jets} #geq 2)2",       Mjj,       vJetmass_2_Zinc2jet);
    diJetMass_2_Zinc3jet                = newTH1D("diJetMass_2_Zinc3jet",             "2Jets Invariant Mass (N_{jets} #geq 3)2",       Mjj,       vJetmass_2_Zinc3jet);
    diJetMass_2_Zinc4jet                = newTH1D("diJetMass_2_Zinc4jet",             "2Jets Invariant Mass (N_{jets} #geq 4)2",       Mjj,       vJetmass_2_Zinc4jet);
    gendiJetMass_2_Zinc2jet                = newTH1D("gendiJetMass_2_Zinc2jet",      "gen 2Jets Invariant Mass (N_{jets} #geq 2)2",    Mjj,      vJetmass_2_Zinc2jet);
    gendiJetMass_2_Zinc3jet                = newTH1D("gendiJetMass_2_Zinc3jet",      "gen 2Jets Invariant Mass (N_{jets} #geq 3)2",    Mjj,      vJetmass_2_Zinc3jet);
    gendiJetMass_2_Zinc4jet                = newTH1D("gendiJetMass_2_Zinc4jet",      "gen 2Jets Invariant Mass (N_{jets} #geq 4)2",    Mjj,      vJetmass_2_Zinc4jet);
    
    hresponsediJetMass_2_Zinc2jet = newTH2D("hresponsediJetMass_2_Zinc2jet", "hresp 2Jets Invariant Mass (N_{jets} #geq 2)2", vJetmass_2_Zinc2jet, vJetmass_2_Zinc2jet);
    hresponsediJetMass_2_Zinc3jet = newTH2D("hresponsediJetMass_2_Zinc3jet", "hresp 2Jets Invariant Mass (N_{jets} #geq 3)2", vJetmass_2_Zinc3jet, vJetmass_2_Zinc3jet);
    hresponsediJetMass_2_Zinc4jet = newTH2D("hresponsediJetMass_2_Zinc4jet", "hresp 2Jets Invariant Mass (N_{jets} #geq 4)2", vJetmass_2_Zinc4jet, vJetmass_2_Zinc4jet);

    
    //------ diJetPt -----------
    diJetPt_Zinc2jet                    = newTH1D("diJetPt_Zinc2jet",    "2Jets p_{T} (N_{jets} #geq 2)",  "dijet p_{T} [GeV]",     nJetPt_Zinc2jet, jetPt_Zinc2jet);
    diJetPt_Zinc3jet                    = newTH1D("diJetPt_Zinc3jet",    "2Jets p_{T} (N_{jets} #geq 3)",  "dijet p_{T} [GeV]",     nJetPt_Zinc3jet, jetPt_Zinc3jet);
    diJetPt_Zinc4jet                    = newTH1D("diJetPt_Zinc4jet",    "2Jets p_{T} (N_{jets} #geq 4)",  "dijet p_{T} [GeV]",     nJetPt_Zinc4jet, jetPt_Zinc4jet);
    gendiJetPt_Zinc2jet              = newTH1D("gendiJetPt_Zinc2jet",    "gen 2Jets p_{T} (N_{jets} #geq 2)",  "dijet p_{T} [GeV]",     nJetPt_Zinc2jet, jetPt_Zinc2jet);
    gendiJetPt_Zinc3jet              = newTH1D("gendiJetPt_Zinc3jet",    "gen 2Jets p_{T} (N_{jets} #geq 3)",  "dijet p_{T} [GeV]",     nJetPt_Zinc3jet, jetPt_Zinc3jet);
    gendiJetPt_Zinc4jet              = newTH1D("gendiJetPt_Zinc4jet",    "gen 2Jets p_{T} (N_{jets} #geq 4)",  "dijet p_{T} [GeV]",     nJetPt_Zinc4jet, jetPt_Zinc4jet);
    
    hresponsediJetPt_Zinc2jet       = newTH2D("hresponsediJetPt_Zinc2jet",  "hresp 2Jets p_{T} (N_{jets} #geq 2)", nJetPt_Zinc2jet, jetPt_Zinc2jet, nJetPt_Zinc2jet, jetPt_Zinc2jet);
    hresponsediJetPt_Zinc3jet       = newTH2D("hresponsediJetPt_Zinc3jet",  "hresp 2Jets p_{T} (N_{jets} #geq 3)", nJetPt_Zinc3jet, jetPt_Zinc3jet, nJetPt_Zinc3jet, jetPt_Zinc3jet);
    hresponsediJetPt_Zinc4jet       = newTH2D("hresponsediJetPt_Zinc4jet",  "hresp 2Jets p_{T} (N_{jets} #geq 4)", nJetPt_Zinc4jet, jetPt_Zinc4jet, nJetPt_Zinc4jet, jetPt_Zinc4jet);
    
    diJetPt_2_Zinc2jet                    = newTH1D("diJetPt_2_Zinc2jet",    "2Jets p_{T} (N_{jets} #geq 2)2",  "dijet p_{T} [GeV]",      jetPt_2_Zinc2jet);
    diJetPt_2_Zinc3jet                    = newTH1D("diJetPt_2_Zinc3jet",    "2Jets p_{T} (N_{jets} #geq 3)2",  "dijet p_{T} [GeV]",      jetPt_2_Zinc3jet);
    diJetPt_2_Zinc4jet                    = newTH1D("diJetPt_2_Zinc4jet",    "2Jets p_{T} (N_{jets} #geq 4)2",  "dijet p_{T} [GeV]",      jetPt_2_Zinc4jet);
    gendiJetPt_2_Zinc2jet              = newTH1D("gendiJetPt_2_Zinc2jet",    "gen 2Jets p_{T} (N_{jets} #geq 2)2",  "dijet p_{T} [GeV]",      jetPt_2_Zinc2jet);
    gendiJetPt_2_Zinc3jet              = newTH1D("gendiJetPt_2_Zinc3jet",    "gen 2Jets p_{T} (N_{jets} #geq 3)2",  "dijet p_{T} [GeV]",      jetPt_2_Zinc3jet);
    gendiJetPt_2_Zinc4jet              = newTH1D("gendiJetPt_2_Zinc4jet",    "gen 2Jets p_{T} (N_{jets} #geq 4)2",  "dijet p_{T} [GeV]",      jetPt_2_Zinc4jet);
    
    hresponsediJetPt_2_Zinc2jet = newTH2D("hresponsediJetPt_2_Zinc2jet", "hresp 2Jets p_{T} (N_{jets} #geq 2)2", jetPt_2_Zinc2jet, jetPt_2_Zinc2jet);
    hresponsediJetPt_2_Zinc3jet = newTH2D("hresponsediJetPt_2_Zinc3jet", "hresp 2Jets p_{T} (N_{jets} #geq 3)2", jetPt_2_Zinc3jet, jetPt_2_Zinc3jet);
    hresponsediJetPt_2_Zinc4jet = newTH2D("hresponsediJetPt_2_Zinc4jet", "hresp 2Jets p_{T} (N_{jets} #geq 4)2", jetPt_2_Zinc4jet, jetPt_2_Zinc4jet);
    
    
    //---
    FirstJetAbsRapidity_Zinc1jet           = newTH1D("FirstJetAbsRapidity_Zinc1jet",          "1st jet |y| (N_{jets} #geq 1)",       "|y(j_{1})|",  12, 0, 2.4);
    SecondJetAbsRapidity_Zinc2jet          = newTH1D("SecondJetAbsRapidity_Zinc2jet",         "2nd jet |y| (N_{jets} #geq 2)",       "|y(j_{2})|",  12, 0, 2.4);
    ThirdJetAbsRapidity_Zinc3jet           = newTH1D("ThirdJetAbsRapidity_Zinc3jet",          "3rd jet |y| (N_{jets} #geq 3)",       "|y(j_{3})|",  8, 0., 2.4);
    FourthJetAbsRapidity_Zinc4jet          = newTH1D("FourthJetAbsRapidity_Zinc4jet",         "4th jet |y| (N_{jets} #geq 4)",       "|y(j_{4})|",  8, 0., 2.4);
    FifthJetAbsRapidity_Zinc5jet           = newTH1D("FifthJetAbsRapidity_Zinc5jet",          "5th jet |y| (N_{jets} #geq 5)",       "|y(j_{5})|",  6, 0., 2.4);
    genFirstJetAbsRapidity_Zinc1jet         = newTH1D("genFirstJetAbsRapidity_Zinc1jet",       "gen 1st jet |y| (N_{jets} #geq 1)",   "|y(j_{1})|",  12, 0, 2.4);
    genSecondJetAbsRapidity_Zinc2jet        = newTH1D("genSecondJetAbsRapidity_Zinc2jet",      "gen 2nd jet |y| (N_{jets} #geq 2)",   "|y(j_{2})|",  12, 0, 2.4);
    genThirdJetAbsRapidity_Zinc3jet         = newTH1D("genThirdJetAbsRapidity_Zinc3jet",       "gen 3rd jet |y| (N_{jets} #geq 3)",   "|y(j_{3})|",  8, 0., 2.4);
    genFourthJetAbsRapidity_Zinc4jet        = newTH1D("genFourthJetAbsRapidity_Zinc4jet",      "gen 4th jet |y| (N_{jets} #geq 4)",   "|y(j_{4})|",  8, 0., 2.4);
    genFifthJetAbsRapidity_Zinc5jet         = newTH1D("genFifthJetAbsRapidity_Zinc5jet",       "gen 5th jet |y| (N_{jets} #geq 5)",   "|y(j_{5})|",  6, 0., 2.4);
    
    hresponseFirstJetAbsRapidity_Zinc1jet   = newTH2D("hresponseFirstJetAbsRapidity_Zinc1jet",   "hresp 1st jet |y| (N_{jets} #geq 1)", 12, 0, 2.4, 12, 0, 2.4);
    hresponseSecondJetAbsRapidity_Zinc2jet  = newTH2D("hresponseSecondJetAbsRapidity_Zinc2jet",  "hresp 2nd jet |y| (N_{jets} #geq 2)", 12, 0, 2.4, 12, 0, 2.4);
    hresponseThirdJetAbsRapidity_Zinc3jet   = newTH2D("hresponseThirdJetAbsRapidity_Zinc3jet",   "hresp 3rd jet |y| (N_{jets} #geq 3)",  8, 0, 2.4,  8, 0, 2.4);
    hresponseFourthJetAbsRapidity_Zinc4jet  = newTH2D("hresponseFourthJetAbsRapidity_Zinc4jet",  "hresp 4th jet |y| (N_{jets} #geq 4)",  8, 0, 2.4,  8, 0, 2.4);
    hresponseFifthJetAbsRapidity_Zinc5jet   = newTH2D("hresponseFifthJetAbsRapidity_Zinc5jet",   "hresp 5th jet |y| (N_{jets} #geq 5)",  6, 0, 2.4,  6, 0, 2.4);
    
    FirstJetAbsRapidity_2_Zinc1jet           = newTH1D("FirstJetAbsRapidity_2_Zinc1jet",          "1st jet |y| (N_{jets} #geq 1)2",       "|y(j_{1})|",  60, 0, 2.4);
    SecondJetAbsRapidity_2_Zinc2jet          = newTH1D("SecondJetAbsRapidity_2_Zinc2jet",         "2nd jet |y| (N_{jets} #geq 2)2",       "|y(j_{2})|",  60, 0, 2.4);
    ThirdJetAbsRapidity_2_Zinc3jet           = newTH1D("ThirdJetAbsRapidity_2_Zinc3jet",          "3rd jet |y| (N_{jets} #geq 3)2",       "|y(j_{3})|",  40, 0., 2.4);
    FourthJetAbsRapidity_2_Zinc4jet          = newTH1D("FourthJetAbsRapidity_2_Zinc4jet",         "4th jet |y| (N_{jets} #geq 4)2",       "|y(j_{4})|",  40, 0., 2.4);
    FifthJetAbsRapidity_2_Zinc5jet           = newTH1D("FifthJetAbsRapidity_2_Zinc5jet",          "5th jet |y| (N_{jets} #geq 5)2",       "|y(j_{5})|",  30, 0., 2.4);
    genFirstJetAbsRapidity_2_Zinc1jet         = newTH1D("genFirstJetAbsRapidity_2_Zinc1jet",       "gen 1st jet |y| (N_{jets} #geq 1)2",   "|y(j_{1})|",  60, 0, 2.4);
    genSecondJetAbsRapidity_2_Zinc2jet        = newTH1D("genSecondJetAbsRapidity_2_Zinc2jet",      "gen 2nd jet |y| (N_{jets} #geq 2)2",   "|y(j_{2})|",  60, 0, 2.4);
    genThirdJetAbsRapidity_2_Zinc3jet         = newTH1D("genThirdJetAbsRapidity_2_Zinc3jet",       "gen 3rd jet |y| (N_{jets} #geq 3)2",   "|y(j_{3})|",  40, 0., 2.4);
    genFourthJetAbsRapidity_2_Zinc4jet        = newTH1D("genFourthJetAbsRapidity_2_Zinc4jet",      "gen 4th jet |y| (N_{jets} #geq 4)2",   "|y(j_{4})|",  40, 0., 2.4);
    genFifthJetAbsRapidity_2_Zinc5jet         = newTH1D("genFifthJetAbsRapidity_2_Zinc5jet",       "gen 5th jet |y| (N_{jets} #geq 5)2",   "|y(j_{5})|",  30, 0., 2.4);

    hresponseFirstJetAbsRapidity_2_Zinc1jet   = newTH2D("hresponseFirstJetAbsRapidity_2_Zinc1jet",   "hresp 1st jet |y| (N_{jets} #geq 1)2", 60, 0, 2.4, 60, 0, 2.4);
    hresponseSecondJetAbsRapidity_2_Zinc2jet  = newTH2D("hresponseSecondJetAbsRapidity_2_Zinc2jet",  "hresp 2nd jet |y| (N_{jets} #geq 2)2", 60, 0, 2.4, 60, 0, 2.4);
    hresponseThirdJetAbsRapidity_2_Zinc3jet   = newTH2D("hresponseThirdJetAbsRapidity_2_Zinc3jet",   "hresp 3rd jet |y| (N_{jets} #geq 3)2", 40, 0, 2.4, 40, 0, 2.4);
    hresponseFourthJetAbsRapidity_2_Zinc4jet  = newTH2D("hresponseFourthJetAbsRapidity_2_Zinc4jet",  "hresp 4th jet |y| (N_{jets} #geq 4)2", 40, 0, 2.4, 40, 0, 2.4);
    hresponseFifthJetAbsRapidity_2_Zinc5jet   = newTH2D("hresponseFifthJetAbsRapidity_2_Zinc5jet",   "hresp 5th jet |y| (N_{jets} #geq 5)2", 30, 0, 2.4, 30, 0, 2.4);
    
    //---------------------------------
    
    //====== MeanNJets ===========================
    MeanNJetsHT_Zinc1jet                = newTH2D("MeanNJetsHT_Zinc1jet",   "N_{jets} VS Scalar sum jets p_{T} (N_{jets} #geq 1)",  nJetHT_Zinc1jet,  jetHT_Zinc1jet,   15, 0.5, 15.5);
    MeanNJetsHT_Zinc2jet                = newTH2D("MeanNJetsHT_Zinc2jet",   "N_{jets} VS Scalar sum jets p_{T} (N_{jets} #geq 2)",  nJetHT_Zinc2jet,  jetHT_Zinc2jet,   15, 0.5, 15.5);
    MeanNJetsdRapidity_Zinc2jet         = newTH2D("MeanNJetsdRapidity_Zinc2jet",        "N_{jets} VS #Delta y btwn jets (N_{jets} #geq 2)",     20, 0, 4.8,   15, 0.5, 15.5);
    MeanNJetsdRapidityFB_Zinc2jet       = newTH2D("MeanNJetsdRapidityFB_Zinc2jet",      "N_{jets} VS #Delta y btwn FBjets (N_{jets} #geq 2)",   20, 0, 4.8,   15, 0.5, 15.5);
    
    genMeanNJetsHT_Zinc1jet     = newTH2D("genMeanNJetsHT_Zinc1jet", "gen N_{jets} VS Scalar sum jets p_{T} (N_{jets} #geq 1)",  nJetHT_Zinc1jet,  jetHT_Zinc1jet,   15, 0.5, 15.5);
    genMeanNJetsHT_Zinc2jet     = newTH2D("genMeanNJetsHT_Zinc2jet", "gen N_{jets} VS Scalar sum jets p_{T} (N_{jets} #geq 2)",  nJetHT_Zinc2jet,  jetHT_Zinc2jet,   15, 0.5, 15.5);
    genMeanNJetsdRapidity_Zinc2jet      = newTH2D("genMeanNJetsdRapidity_Zinc2jet",        "gen N_{jets} VS #Delta y btwn jets (N_{jets} #geq 2)",       20, 0, 4.8,   15, 0.5, 15.5);
    genMeanNJetsdRapidityFB_Zinc2jet    = newTH2D("genMeanNJetsdRapidityFB_Zinc2jet",      "gen N_{jets} VS #Delta y btwn FBjets (N_{jets} #geq 2)",     20, 0, 4.8,   15, 0.5, 15.5);
    
    responseMeanNJetsHT_Zinc1jet                = newResp(MeanNJetsHT_Zinc1jet,  genMeanNJetsHT_Zinc1jet);
    responseMeanNJetsHT_Zinc2jet                = newResp(MeanNJetsHT_Zinc2jet,  genMeanNJetsHT_Zinc2jet);
    responseMeanNJetsdRapidity_Zinc2jet         = newResp(MeanNJetsdRapidity_Zinc2jet,  genMeanNJetsdRapidity_Zinc2jet);
    responseMeanNJetsdRapidityFB_Zinc2jet       = newResp(MeanNJetsdRapidityFB_Zinc2jet,  genMeanNJetsdRapidityFB_Zinc2jet);
    //************************************************************  End Additional Variables  ******************************************************************************//
    
    
    //---------------------------------
    MeanNJetsHT_1D_Zinc1jet           = newTH1D("MeanNJetsHT_1D_Zinc1jet",           "<N_{jets}> VS Scalar sum (N_{jets} #geq 1)",    HT,      nJetHT_Zinc1jet, jetHT_Zinc1jet);
    MeanNJetsHT_1D_Zinc2jet           = newTH1D("MeanNJetsHT_1D_Zinc2jet",           "<N_{jets}> VS Scalar sum (N_{jets} #geq 2)",    HT,      nJetHT_Zinc2jet, jetHT_Zinc2jet);
    MeanNJetsdRapidity_1D_Zinc2jet    = newTH1D("MeanNJetsdRapidity_1D_Zinc2jet",    "<N_{jets}> VS #Delta y btwn jets (N_{jets} #geq 2)",     "#Deltay(j_{1}j_{2})",  20, 0, 4.8);
    MeanNJetsdRapidityFB_1D_Zinc2jet  = newTH1D("MeanNJetsdRapidityFB_1D_Zinc2jet",  "<N_{jets}> VS #Delta y btwn FBjets (N_{jets} #geq 2)",   "#Deltay(j_{F}j_{B})",  20, 0, 4.8);
    
    genMeanNJetsHT_1D_Zinc1jet     = newTH1D("genMeanNJetsHT_1D_Zinc1jet",           "gen <N_{jets}> VS Scalar sum (N_{jets} #geq 1)",    HT,      nJetHT_Zinc1jet, jetHT_Zinc1jet);
    genMeanNJetsHT_1D_Zinc2jet     = newTH1D("genMeanNJetsHT_1D_Zinc2jet",           "gen <N_{jets}> VS Scalar sum (N_{jets} #geq 2)",    HT,      nJetHT_Zinc2jet, jetHT_Zinc2jet);
    genMeanNJetsdRapidity_1D_Zinc2jet   = newTH1D("genMeanNJetsdRapidity_1D_Zinc2jet",   "gen <N_{jets}> VS #Delta y btwn jets (N_{jets} #geq 2)",   "#Deltay(j_{1}j_{2})", 20, 0, 4.8);
    genMeanNJetsdRapidityFB_1D_Zinc2jet = newTH1D("genMeanNJetsdRapidityFB_1D_Zinc2jet", "gen <N_{jets}> VS #Delta y btwn FBjets (N_{jets} #geq 2)", "#Deltay(j_{F}j_{B})", 20, 0, 4.8);
    //---------------------------------
    
    int nJetmass_Zinc1jet(12);
    vector<double> vJetmass_Zinc1jet = makeVector(nJetmass_Zinc1jet, 0., 10., 22., 36., 52., 70., 90., 112., 136., 162., 190., 250.);
    
    //---
    FirstJetmass_Zinc1jet               = newTH1D("FirstJetmass_Zinc1jet",               "1st jet mass (N_{jets} #geq 1)",              "mass(j_{1})",  25, 0, 250);
    SecondJetmass_Zinc2jet              = newTH1D("SecondJetmass_Zinc2jet",              "2nd jet mass (N_{jets} #geq 2)",              "mass(j_{2})",  25, 0, 250);
    ThirdJetmass_Zinc3jet               = newTH1D("ThirdJetmass_Zinc3jet",               "3rd jet mass (N_{jets} #geq 3)",              "mass(j_{3})",  25, 0, 250);
    FourthJetmass_Zinc4jet              = newTH1D("FourthJetmass_Zinc4jet",              "4th jet mass (N_{jets} #geq 4)",              "mass(j_{4})",  25, 0, 250);
    FirstJetmass_1_Zinc1jet             = newTH1D("FirstJetmass_1_Zinc1jet",             "1st jet mass (N_{jets} #geq 1)1",             "mass(j_{1})",  vJetmass_Zinc1jet);
    SecondJetmass_1_Zinc2jet            = newTH1D("SecondJetmass_1_Zinc2jet",            "2nd jet mass (N_{jets} #geq 2)1",             "mass(j_{2})",  vJetmass_Zinc1jet);
    ThirdJetmass_1_Zinc3jet             = newTH1D("ThirdJetmass_1_Zinc3jet",             "3rd jet mass (N_{jets} #geq 3)1",             "mass(j_{3})",  vJetmass_Zinc1jet);
    FourthJetmass_1_Zinc4jet            = newTH1D("FourthJetmass_1_Zinc4jet",            "4th jet mass (N_{jets} #geq 4)1",             "mass(j_{4})",  vJetmass_Zinc1jet);
    
    FirstJetRapidityFull_Zinc1jet       = newTH1D("FirstJetRapidityFull_Zinc1jet",       "1st jet y (N_{jets} #geq 1)",                 "y(j_{1})",  48,-2.4, 2.4);
    SecondJetRapidityFull_Zinc2jet      = newTH1D("SecondJetRapidityFull_Zinc2jet",      "2nd jet y (N_{jets} #geq 2)",                 "y(j_{2})",  48,-2.4, 2.4);
    ThirdJetRapidityFull_Zinc3jet       = newTH1D("ThirdJetRapidityFull_Zinc3jet",       "3rd jet y (N_{jets} #geq 3)",                 "y(j_{3})",  24,-2.4, 2.4);
    FourthJetRapidityFull_Zinc4jet      = newTH1D("FourthJetRapidityFull_Zinc4jet",      "4th jet y (N_{jets} #geq 4)",                 "y(j_{4})",  12,-2.4, 2.4);
    
    genFirstJetRapidityFull_Zinc1jet       = newTH1D("genFirstJetRapidityFull_Zinc1jet",       "gen 1st jet y (N_{jets} #geq 1)",      "y(j_{1})",  48,-2.4, 2.4);
    genSecondJetRapidityFull_Zinc2jet      = newTH1D("genSecondJetRapidityFull_Zinc2jet",      "gen 2nd jet y (N_{jets} #geq 2)",      "y(j_{2})",  48,-2.4, 2.4);
    genThirdJetRapidityFull_Zinc3jet       = newTH1D("genThirdJetRapidityFull_Zinc3jet",       "gen 3rd jet y (N_{jets} #geq 3)",      "y(j_{3})",  24,-2.4, 2.4);
    genFourthJetRapidityFull_Zinc4jet      = newTH1D("genFourthJetRapidityFull_Zinc4jet",      "gen 4th jet y (N_{jets} #geq 4)",      "y(j_{4})",  12,-2.4, 2.4);
    //********************************
    ZNGoodJetsFull_Zexc = newTH1D("ZNGoodJetsFull_Zexc","Jet Counter (excl.)", "N_{jets}", 11, -0.5, 10.5);
    ZNGoodJetsFull_Zexc->GetXaxis()->SetBinLabel(1, "= 0");
    ZNGoodJetsFull_Zexc->GetXaxis()->SetBinLabel(2, "= 1");
    ZNGoodJetsFull_Zexc->GetXaxis()->SetBinLabel(3, "= 2");
    ZNGoodJetsFull_Zexc->GetXaxis()->SetBinLabel(4, "= 3");
    ZNGoodJetsFull_Zexc->GetXaxis()->SetBinLabel(5, "= 4");
    ZNGoodJetsFull_Zexc->GetXaxis()->SetBinLabel(6, "= 5");
    ZNGoodJetsFull_Zexc->GetXaxis()->SetBinLabel(7, "= 6");
    ZNGoodJetsFull_Zexc->GetXaxis()->SetBinLabel(8, "= 7");
    ZNGoodJetsFull_Zexc->GetXaxis()->SetBinLabel(9, "= 8");
    ZNGoodJetsFull_Zexc->GetXaxis()->SetBinLabel(10, "= 9");
    ZNGoodJetsFull_Zexc->GetXaxis()->SetBinLabel(11, "= 10");
    
    genZNGoodJetsFull_Zexc = newTH1D("genZNGoodJetsFull_Zexc","Jet Counter (excl.)", "N_{jets}", 11, -0.5, 10.5);
    genZNGoodJetsFull_Zexc->GetXaxis()->SetBinLabel(1,"= 0");
    genZNGoodJetsFull_Zexc->GetXaxis()->SetBinLabel(2,"= 1");
    genZNGoodJetsFull_Zexc->GetXaxis()->SetBinLabel(3,"= 2");
    genZNGoodJetsFull_Zexc->GetXaxis()->SetBinLabel(4,"= 3");
    genZNGoodJetsFull_Zexc->GetXaxis()->SetBinLabel(5,"= 4");
    genZNGoodJetsFull_Zexc->GetXaxis()->SetBinLabel(6,"= 5");
    genZNGoodJetsFull_Zexc->GetXaxis()->SetBinLabel(7,"= 6");
    genZNGoodJetsFull_Zexc->GetXaxis()->SetBinLabel(8,"= 7");
    genZNGoodJetsFull_Zexc->GetXaxis()->SetBinLabel(9,"= 8");
    genZNGoodJetsFull_Zexc->GetXaxis()->SetBinLabel(10,"= 9");
    genZNGoodJetsFull_Zexc->GetXaxis()->SetBinLabel(11,"= 10");
    
    ZNGoodJetsFull_Zinc = newTH1D("ZNGoodJetsFull_Zinc","Jet Counter (incl.)", "N_{jets}", 11, -0.5, 10.5);
    ZNGoodJetsFull_Zinc->GetXaxis()->SetBinLabel(1, "#geq 0");
    ZNGoodJetsFull_Zinc->GetXaxis()->SetBinLabel(2, "#geq 1");
    ZNGoodJetsFull_Zinc->GetXaxis()->SetBinLabel(3, "#geq 2");
    ZNGoodJetsFull_Zinc->GetXaxis()->SetBinLabel(4, "#geq 3");
    ZNGoodJetsFull_Zinc->GetXaxis()->SetBinLabel(5, "#geq 4");
    ZNGoodJetsFull_Zinc->GetXaxis()->SetBinLabel(6, "#geq 5");
    ZNGoodJetsFull_Zinc->GetXaxis()->SetBinLabel(7, "#geq 6");
    ZNGoodJetsFull_Zinc->GetXaxis()->SetBinLabel(8, "#geq 7");
    ZNGoodJetsFull_Zinc->GetXaxis()->SetBinLabel(9, "#geq 8");
    ZNGoodJetsFull_Zinc->GetXaxis()->SetBinLabel(10, "#geq 9");
    ZNGoodJetsFull_Zinc->GetXaxis()->SetBinLabel(11, "#geq 10");
    
    genZNGoodJetsFull_Zinc = newTH1D("genZNGoodJetsFull_Zinc","Jet Counter (incl.)", "N_{jets}", 11, -0.5, 10.5);
    genZNGoodJetsFull_Zinc->GetXaxis()->SetBinLabel(1,"#geq 0");
    genZNGoodJetsFull_Zinc->GetXaxis()->SetBinLabel(2,"#geq 1");
    genZNGoodJetsFull_Zinc->GetXaxis()->SetBinLabel(3,"#geq 2");
    genZNGoodJetsFull_Zinc->GetXaxis()->SetBinLabel(4,"#geq 3");
    genZNGoodJetsFull_Zinc->GetXaxis()->SetBinLabel(5,"#geq 4");
    genZNGoodJetsFull_Zinc->GetXaxis()->SetBinLabel(6,"#geq 5");
    genZNGoodJetsFull_Zinc->GetXaxis()->SetBinLabel(7,"#geq 6");
    genZNGoodJetsFull_Zinc->GetXaxis()->SetBinLabel(8,"#geq 7");
    genZNGoodJetsFull_Zinc->GetXaxis()->SetBinLabel(9,"#geq 8");
    genZNGoodJetsFull_Zinc->GetXaxis()->SetBinLabel(10,"#geq 9");
    genZNGoodJetsFull_Zinc->GetXaxis()->SetBinLabel(11,"#geq 10");
    
    hresponseZNGoodJetsFull_Zexc = newTH2D("hresponseZNGoodJetsFull_Zexc", "hresp ZNGoodJetsFull_Zexc", 11, -0.5, 10.5, 11, -0.5, 10.5);
    hresponseZNGoodJetsFull_Zinc = newTH2D("hresponseZNGoodJetsFull_Zinc", "hresp ZNGoodJetsFull_Zinc", 11, -0.5, 10.5, 11, -0.5, 10.5);
    
    //********************************
    
    NumberPFcandidates                  = newTH1D("NumberPFcandidates",                  "NumberPFcandidates",           "Number of lepton PF candidates",    20, -0.5, 19.5);
    
    ZMass_lowDeltaR                     = newTH1D("ZMass_lowDeltaR",                     "ZMass_lowDeltaR",                             Mll,    120, 50, 169);
    AllPassLepID			      = newTH1D("AllPassLepID",                        "Lepton pair ID for all passing leptons",    "leptonPairID" ,    54, -26.5, 26.5);
    AllPassWithMassCutLepID	      = newTH1D("AllPassWithMassCutLepID",                        "Lepton pair ID for leptons + mass cut",    "leptonPairID" ,    54, -26.5, 26.5);
    AllPassWithMassCutLepIDCharge	      = newTH1D("AllPassWithMassCutLepIDCharge",                        "Lepton pair ID, charge for leptons + mass cut",    "leptonPairID" ,    54, -26.5, 26.5);
    ZMassAllPassLep                     = newTH1D("ZMassAllPassLep",                     "Z Invariant Mass for all passing leptons",          Mll,    240, 0, 480 );
    
    
    ZMass_Zinc0jet                      = newTH1D("ZMass_Zinc0jet",                      "Z Invariant Mass (N_{jets} #geq 0)",          Mll,    111, 50, 260 );
    ZMass_Zinc1jet                      = newTH1D("ZMass_Zinc1jet",                      "Z Invariant Mass (N_{jets} #geq 1)",          Mll,    111, 50, 260 );
    ZMass_Zinc2jet                      = newTH1D("ZMass_Zinc2jet",                      "Z Invariant Mass (N_{jets} #geq 2)",          Mll,    111, 50, 260 );
    ZMass_Zinc3jet                      = newTH1D("ZMass_Zinc3jet",                      "Z Invariant Mass (N_{jets} #geq 3)",          Mll,    111, 50, 260 );
    ZMass_Zinc4jet                      = newTH1D("ZMass_Zinc4jet",                      "Z Invariant Mass (N_{jets} #geq 4)",          Mll,    111, 50, 260 );
    ZMass_Zinc5jet                      = newTH1D("ZMass_Zinc5jet",                      "Z Invariant Mass (N_{jets} #geq 5)",          Mll,    111, 50, 260 );
    ZMass_Zinc6jet                      = newTH1D("ZMass_Zinc6jet",                      "Z Invariant Mass (N_{jets} #geq 6)",          Mll,    111, 50, 260 );
    
    genZMass_Zinc0jet                   = newTH1D("genZMass_Zinc0jet",                   "Z Invariant Mass (N_{jets} #geq 0)",          Mll,    111, 50, 260 );
    genZMass_Zinc1jet                   = newTH1D("genZMass_Zinc1jet",                   "Z Invariant Mass (N_{jets} #geq 1)",          Mll,    111, 50, 260 );
    genZMass_Zinc2jet                   = newTH1D("genZMass_Zinc2jet",                   "Z Invariant Mass (N_{jets} #geq 2)",          Mll,    111, 50, 260 );
    genZMass_Zinc3jet                   = newTH1D("genZMass_Zinc3jet",                   "Z Invariant Mass (N_{jets} #geq 3)",          Mll,    111, 50, 260 );
    genZMass_Zinc4jet                   = newTH1D("genZMass_Zinc4jet",                   "Z Invariant Mass (N_{jets} #geq 4)",          Mll,    111, 50, 260 );
    genZMass_Zinc5jet                   = newTH1D("genZMass_Zinc5jet",                   "Z Invariant Mass (N_{jets} #geq 5)",          Mll,    111, 50, 260 );
    genZMass_Zinc6jet                   = newTH1D("genZMass_Zinc6jet",                   "Z Invariant Mass (N_{jets} #geq 6)",          Mll,    111, 50, 260 );
    
    ZMass_Zexc0jet                      = newTH1D("ZMass_Zexc0jet",                      "Z Invariant Mass (N_{jets} = 0)",             Mll,    111, 50, 260 );
    ZMass_Zexc1jet                      = newTH1D("ZMass_Zexc1jet",                      "Z Invariant Mass (N_{jets} = 1)",             Mll,    111, 50, 260 );
    ZMass_Zexc2jet                      = newTH1D("ZMass_Zexc2jet",                      "Z Invariant Mass (N_{jets} = 2)",             Mll,    111, 50, 260 );
    ZMass_Zexc3jet                      = newTH1D("ZMass_Zexc3jet",                      "Z Invariant Mass (N_{jets} = 3)",             Mll,    111, 50, 260 );
    ZMass_Zexc4jet                      = newTH1D("ZMass_Zexc4jet",                      "Z Invariant Mass (N_{jets} = 4)",             Mll,    111, 50, 260 );
    ZMass_Zexc5jet                      = newTH1D("ZMass_Zexc5jet",                      "Z Invariant Mass (N_{jets} = 5)",             Mll,    111, 50, 260 );
    ZMass_Zexc6jet                      = newTH1D("ZMass_Zexc6jet",                      "Z Invariant Mass (N_{jets} = 6)",             Mll,    111, 50, 260 );
    
    ZPt_Zinc0jet                        = newTH1D("ZPt_Zinc0jet",                        "Z p_{T} (N_{jets} #geq 0)",                   ZpT,    40, 0, 400);
    ZPt_Zinc1jet                        = newTH1D("ZPt_Zinc1jet",                        "Z p_{T} (N_{jets} #geq 1)",                   ZpT,    40, 0, 400);
    ZPt_Zinc2jet                        = newTH1D("ZPt_Zinc2jet",                        "Z p_{T} (N_{jets} #geq 2)",                   ZpT,    40, 0, 400);
    ZPt_Zinc3jet                        = newTH1D("ZPt_Zinc3jet",                        "Z p_{T} (N_{jets} #geq 3)",                   ZpT,    40, 0, 300);
    ZPt_Zinc4jet                        = newTH1D("ZPt_Zinc4jet",                        "Z p_{T} (N_{jets} #geq 4)",                   ZpT,    40, 0, 200);
    ZPt_Zinc5jet                        = newTH1D("ZPt_Zinc5jet",                        "Z p_{T} (N_{jets} #geq 5)",                   ZpT,    40, 0, 200);
    ZPt_Zinc6jet                        = newTH1D("ZPt_Zinc6jet",                        "Z p_{T} (N_{jets} #geq 6)",                   ZpT,    40, 0, 200);
    
    genZPt_Zinc0jet                     = newTH1D("genZPt_Zinc0jet",                     "gen Z p_{T} (N_{jets} #geq 0)",               ZpT,    40, 0, 200);
    genZPt_Zinc1jet                     = newTH1D("genZPt_Zinc1jet",                     "gen Z p_{T} (N_{jets} #geq 1)",               ZpT,    40, 0, 400);
    genZPt_Zinc2jet                     = newTH1D("genZPt_Zinc2jet",                     "gen Z p_{T} (N_{jets} #geq 2)",               ZpT,    40, 0, 400);
    genZPt_Zinc3jet                     = newTH1D("genZPt_Zinc3jet",                     "gen Z p_{T} (N_{jets} #geq 3)",               ZpT,    40, 0, 300);
    genZPt_Zinc4jet                     = newTH1D("genZPt_Zinc4jet",                     "gen Z p_{T} (N_{jets} #geq 4)",               ZpT,    40, 0, 200);
    genZPt_Zinc5jet                     = newTH1D("genZPt_Zinc5jet",                     "gen Z p_{T} (N_{jets} #geq 5)",               ZpT,    40, 0, 200);
    genZPt_Zinc6jet                     = newTH1D("genZPt_Zinc6jet",                     "gen Z p_{T} (N_{jets} #geq 6)",               ZpT,    40, 0, 200);
    
    ZPt_Zexc0jet                        = newTH1D("ZPt_Zexc0jet",                        "Z p_{T} (N_{jets} = 0)",                      ZpT,    40, 0, 400);
    ZPt_Zexc1jet                        = newTH1D("ZPt_Zexc1jet",                        "Z p_{T} (N_{jets} = 1)",                      ZpT,    40, 0, 400);
    ZPt_Zexc2jet                        = newTH1D("ZPt_Zexc2jet",                        "Z p_{T} (N_{jets} = 2)",                      ZpT,    40, 0, 400);
    ZPt_Zexc3jet                        = newTH1D("ZPt_Zexc3jet",                        "Z p_{T} (N_{jets} = 3)",                      ZpT,    40, 0, 300);
    ZPt_Zexc4jet                        = newTH1D("ZPt_Zexc4jet",                        "Z p_{T} (N_{jets} = 4)",                      ZpT,    40, 0, 200);
    ZPt_Zexc5jet                        = newTH1D("ZPt_Zexc5jet",                        "Z p_{T} (N_{jets} = 5)",                      ZpT,    40, 0, 200);
    ZPt_Zexc6jet                        = newTH1D("ZPt_Zexc6jet",                        "Z p_{T} (N_{jets} = 6)",                      ZpT,    40, 0, 200);
    
    
    ZRapidity_Zinc0jet                  = newTH1D("ZRapidity_Zinc0jet",                  "Z Rapidity (N_{jets} #geq 0)",                Zrap,   30,-3, 3);
    ZRapidity_Zinc1jet                  = newTH1D("ZRapidity_Zinc1jet",                  "Z Rapidity (N_{jets} #geq 1)",                Zrap,   30,-3, 3);
    ZRapidity_Zinc2jet                  = newTH1D("ZRapidity_Zinc2jet",                  "Z Rapidity (N_{jets} #geq 2)",                Zrap,   30,-3, 3);
    ZRapidity_Zinc3jet                  = newTH1D("ZRapidity_Zinc3jet",                  "Z Rapidity (N_{jets} #geq 3)",                Zrap,   30,-3, 3);
    ZRapidity_Zinc4jet                  = newTH1D("ZRapidity_Zinc4jet",                  "Z Rapidity (N_{jets} #geq 4)",                Zrap,   30,-3, 3);
    ZRapidity_Zinc5jet                  = newTH1D("ZRapidity_Zinc5jet",                  "Z Rapidity (N_{jets} #geq 5)",                Zrap,   30,-3, 3);
    ZRapidity_Zinc6jet                  = newTH1D("ZRapidity_Zinc6jet",                  "Z Rapidity (N_{jets} #geq 6)",                Zrap,   30,-3, 3);
    
    genZRapidity_Zinc0jet               = newTH1D("genZRapidity_Zinc0jet",               "gen Z Rapidity (N_{jets} #geq 0)",            Zrap,   30,-3, 3);
    genZRapidity_Zinc1jet               = newTH1D("genZRapidity_Zinc1jet",               "gen Z Rapidity (N_{jets} #geq 1)",            Zrap,   30,-3, 3);
    genZRapidity_Zinc2jet               = newTH1D("genZRapidity_Zinc2jet",               "gen Z Rapidity (N_{jets} #geq 2)",            Zrap,   30,-3, 3);
    genZRapidity_Zinc3jet               = newTH1D("genZRapidity_Zinc3jet",               "gen Z Rapidity (N_{jets} #geq 3)",            Zrap,   30,-3, 3);
    genZRapidity_Zinc4jet               = newTH1D("genZRapidity_Zinc4jet",               "gen Z Rapidity (N_{jets} #geq 4)",            Zrap,   30,-3, 3);
    genZRapidity_Zinc5jet               = newTH1D("genZRapidity_Zinc5jet",               "gen Z Rapidity (N_{jets} #geq 5)",            Zrap,   30,-3, 3);
    genZRapidity_Zinc6jet               = newTH1D("genZRapidity_Zinc6jet",               "gen Z Rapidity (N_{jets} #geq 6)",            Zrap,   30,-3, 3);
    
    ZRapidity_Zexc0jet                  = newTH1D("ZRapidity_Zexc0jet",                  "Z Rapidity (N_{jets} = 0)",                   Zrap,   30,-3, 3);
    ZRapidity_Zexc1jet                  = newTH1D("ZRapidity_Zexc1jet",                  "Z Rapidity (N_{jets} = 1)",                   Zrap,   30,-3, 3);
    ZRapidity_Zexc2jet                  = newTH1D("ZRapidity_Zexc2jet",                  "Z Rapidity (N_{jets} = 2)",                   Zrap,   30,-3, 3);
    ZRapidity_Zexc3jet                  = newTH1D("ZRapidity_Zexc3jet",                  "Z Rapidity (N_{jets} = 3)",                   Zrap,   30,-3, 3);
    ZRapidity_Zexc4jet                  = newTH1D("ZRapidity_Zexc4jet",                  "Z Rapidity (N_{jets} = 4)",                   Zrap,   30,-3, 3);
    ZRapidity_Zexc5jet                  = newTH1D("ZRapidity_Zexc5jet",                  "Z Rapidity (N_{jets} = 5)",                   Zrap,   30,-3, 3);
    ZRapidity_Zexc6jet                  = newTH1D("ZRapidity_Zexc6jet",                  "Z Rapidity (N_{jets} = 6)",                   Zrap,   30,-3, 3);
    
    ZEta_Zinc0jet                       = newTH1D("ZEta_Zinc0jet",                       "Z #eta (N_{jets} #geq 0)",                    Zeta,   30,-3, 3);
    ZEta_Zinc1jet                       = newTH1D("ZEta_Zinc1jet",                       "Z #eta (N_{jets} #geq 1)",                    Zeta,   30,-3, 3);
    ZEta_Zinc2jet                       = newTH1D("ZEta_Zinc2jet",                       "Z #eta (N_{jets} #geq 2)",                    Zeta,   30,-3, 3);
    ZEta_Zinc3jet                       = newTH1D("ZEta_Zinc3jet",                       "Z #eta (N_{jets} #geq 3)",                    Zeta,   30,-3, 3);
    ZEta_Zinc4jet                       = newTH1D("ZEta_Zinc4jet",                       "Z #eta (N_{jets} #geq 4)",                    Zeta,   30,-3, 3);
    ZEta_Zinc5jet                       = newTH1D("ZEta_Zinc5jet",                       "Z #eta (N_{jets} #geq 5)",                    Zeta,   30,-3, 3);
    ZEta_Zinc6jet                       = newTH1D("ZEta_Zinc6jet",                       "Z #eta (N_{jets} #geq 6)",                    Zeta,   30,-3, 3);
    
    genZEta_Zinc0jet                    = newTH1D("genZEta_Zinc0jet",                    "gen Z #eta (N_{jets} #geq 0)",                Zeta,   30,-3, 3);
    genZEta_Zinc1jet                    = newTH1D("genZEta_Zinc1jet",                    "gen Z #eta (N_{jets} #geq 1)",                Zeta,   30,-3, 3);
    genZEta_Zinc2jet                    = newTH1D("genZEta_Zinc2jet",                    "gen Z #eta (N_{jets} #geq 2)",                Zeta,   30,-3, 3);
    genZEta_Zinc3jet                    = newTH1D("genZEta_Zinc3jet",                    "gen Z #eta (N_{jets} #geq 3)",                Zeta,   30,-3, 3);
    genZEta_Zinc4jet                    = newTH1D("genZEta_Zinc4jet",                    "gen Z #eta (N_{jets} #geq 4)",                Zeta,   30,-3, 3);
    genZEta_Zinc5jet                    = newTH1D("genZEta_Zinc5jet",                    "gen Z #eta (N_{jets} #geq 5)",                Zeta,   30,-3, 3);
    genZEta_Zinc6jet                    = newTH1D("genZEta_Zinc6jet",                    "gen Z #eta (N_{jets} #geq 6)",                Zeta,   30,-3, 3);
    
    
    ZEta_Zexc0jet                       = newTH1D("ZEta_Zexc0jet",                       "Z #eta (N_{jets} = 0)",                       Zeta,   30,-3, 3);
    ZEta_Zexc1jet                       = newTH1D("ZEta_Zexc1jet",                       "Z #eta (N_{jets} = 1)",                       Zeta,   30,-3, 3);
    ZEta_Zexc2jet                       = newTH1D("ZEta_Zexc2jet",                       "Z #eta (N_{jets} = 2)",                       Zeta,   30,-3, 3);
    ZEta_Zexc3jet                       = newTH1D("ZEta_Zexc3jet",                       "Z #eta (N_{jets} = 3)",                       Zeta,   30,-3, 3);
    ZEta_Zexc4jet                       = newTH1D("ZEta_Zexc4jet",                       "Z #eta (N_{jets} = 4)",                       Zeta,   30,-3, 3);
    ZEta_Zexc5jet                       = newTH1D("ZEta_Zexc5jet",                       "Z #eta (N_{jets} = 5)",                       Zeta,   30,-3, 3);
    ZEta_Zexc6jet                       = newTH1D("ZEta_Zexc6jet",                       "Z #eta (N_{jets} = 6)",                       Zeta,   30,-3, 3);
    
    lepEta_Zinc0jet                     = newTH1D("lepEta_Zinc0jet",                     "1st & 2nd lep #eta (N_{jets} #geq 0)",        leta,   24,-2.4, 2.4);
    lepEta_Zinc1jet                     = newTH1D("lepEta_Zinc1jet",                     "1st & 2nd lep #eta (N_{jets} #geq 1)",        leta,   24,-2.4, 2.4);
    lepEta_Zinc2jet                     = newTH1D("lepEta_Zinc2jet",                     "1st & 2nd lep #eta (N_{jets} #geq 2)",        leta,   24,-2.4, 2.4);
    lepEta_Zinc3jet                     = newTH1D("lepEta_Zinc3jet",                     "1st & 2nd lep #eta (N_{jets} #geq 3)",        leta,   24,-2.4, 2.4);
    lepEta_Zinc4jet                     = newTH1D("lepEta_Zinc4jet",                     "1st & 2nd lep #eta (N_{jets} #geq 4)",        leta,   24,-2.4, 2.4);
    lepEta_Zinc5jet                     = newTH1D("lepEta_Zinc5jet",                     "1st & 2nd lep #eta (N_{jets} #geq 5)",        leta,   24,-2.4, 2.4);
    
    lepPhi_Zinc0jet                     = newTH1D("lepPhi_Zinc0jet",                     "1st & 2nd lep #phi (N_{jets} #geq 0)",           lphi,   24,-PI, PI);
    lepPhi_Zinc1jet                     = newTH1D("lepPhi_Zinc1jet",                     "1st & 2nd lep #phi (N_{jets} #geq 1)",           lphi,   24,-PI, PI);
    
    GLepBareEtaZinc0jet                  = newTH1D("GLepBareEtaZinc0jet",                  "1st & 2nd lep #eta (N_{jets} #geq 0)",        leta,   24,-2.4, 2.4);
    GLepBareEtaZinc1jet                  = newTH1D("GLepBareEtaZinc1jet",                  "1st & 2nd lep #eta (N_{jets} #geq 1)",        leta,   24,-2.4, 2.4);
    GLepBareEtaZinc2jet                  = newTH1D("GLepBareEtaZinc2jet",                  "1st & 2nd lep #eta (N_{jets} #geq 2)",        leta,   24,-2.4, 2.4);
    
    lepEta_Zexc0jet                     = newTH1D("lepEta_Zexc0jet",                     "1st & 2nd lep #eta (N_{jets} = 0)",           leta,   24,-2.4, 2.4);
    lepEta_Zexc1jet                     = newTH1D("lepEta_Zexc1jet",                     "1st & 2nd lep #eta (N_{jets} = 1)",           leta,   24,-2.4, 2.4);
    lepEta_Zexc2jet                     = newTH1D("lepEta_Zexc2jet",                     "1st & 2nd lep #eta (N_{jets} = 2)",           leta,   24,-2.4, 2.4);
    lepEta_Zexc3jet                     = newTH1D("lepEta_Zexc3jet",                     "1st & 2nd lep #eta (N_{jets} = 3)",           leta,   24,-2.4, 2.4);
    lepEta_Zexc4jet                     = newTH1D("lepEta_Zexc4jet",                     "1st & 2nd lep #eta (N_{jets} = 4)",           leta,   24,-2.4, 2.4);
    lepEta_Zexc5jet                     = newTH1D("lepEta_Zexc5jet",                     "1st & 2nd lep #eta (N_{jets} = 5)",           leta,   24,-2.4, 2.4);
    
    lepPhi_Zexc0jet                     = newTH1D("lepPhi_Zexc0jet",                     "1st & 2nd lep #phi (N_{jets} = 0)",           lphi,   24,-PI, PI);
    lepPhi_Zexc1jet                     = newTH1D("lepPhi_Zexc1jet",                     "1st & 2nd lep #phi (N_{jets} = 1)",           lphi,   24,-PI, PI);
    lepPhi_Zexc2jet                     = newTH1D("lepPhi_Zexc2jet",                     "1st & 2nd lep #phi (N_{jets} = 2)",           lphi,   24,-PI, PI);
    lepPhi_Zexc3jet                     = newTH1D("lepPhi_Zexc3jet",                     "1st & 2nd lep #phi (N_{jets} = 3)",           lphi,   24,-PI, PI);
    lepPhi_Zexc4jet                     = newTH1D("lepPhi_Zexc4jet",                     "1st & 2nd lep #phi (N_{jets} = 4)",           lphi,   24,-PI, PI);
    lepPhi_Zexc5jet                     = newTH1D("lepPhi_Zexc5jet",                     "1st & 2nd lep #phi (N_{jets} = 5)",           lphi,   24,-PI, PI);
    
    
    lepEtaEta_Zinc0jet                  = newTH2D("lepEtaEta_Zinc0jet",                  "#eta #eta (N_{jets} #geq 0)",                  12, -2.4, 2.4, 12,-2.4, 2.4);
    
    FirstJetEtaFull_Zinc1jet                = newTH1D("FirstJetEtaFull_Zinc1jet",                "1st jet #eta (N_{jets} #geq 1)",              "#eta(j_{1})",  48,-2.4, 2.4);
    SecondJetEtaFull_Zinc2jet               = newTH1D("SecondJetEtaFull_Zinc2jet",               "2nd jet #eta (N_{jets} #geq 2)",              "#eta(j_{2})",  48,-2.4, 2.4);
    ThirdJetEtaFull_Zinc3jet                = newTH1D("ThirdJetEtaFull_Zinc3jet",                "3rd jet #eta (N_{jets} #geq 3)",              "#eta(j_{3})",  16,-2.4, 2.4);
    FourthJetEtaFull_Zinc4jet               = newTH1D("FourthJetEtaFull_Zinc4jet",               "4th jet #eta (N_{jets} #geq 4)",              "#eta(j_{4})",   8,-2.4, 2.4);
    FifthJetEtaFull_Zinc5jet                = newTH1D("FifthJetEtaFull_Zinc5jet",                "5th jet #eta (N_{jets} #geq 5)",              "#eta(j_{5})",   4,-2.4, 2.4);
    SixthJetEtaFull_Zinc6jet                = newTH1D("SixthJetEtaFull_Zinc6jet",                "#geq 6th jets #eta (N_{jets} #geq 6)",        "#eta(j_{6})",   4,-2.4, 2.4);
    
    FirstJetEta_Zexc1jet                = newTH1D("FirstJetEta_Zexc1jet",                "1st jet #eta (N_{jets} = 1)",                 "#eta(j_{1})",  47,-4.7, 4.7);
    SecondJetEta_Zexc2jet               = newTH1D("SecondJetEta_Zexc2jet",               "2nd jet #eta (N_{jets} = 2)",                 "#eta(j_{2})",  47,-4.7, 4.7);
    
    AllJetEta_Zinc1jet                  = newTH1D("AllJetEta_Zinc1jet",                  "All jets #eta (N_{jets} #geq 1)",             "#eta(jets)",   47,-4.7, 4.7);
    AllJetEta_Zinc2jet                  = newTH1D("AllJetEta_Zinc2jet",                  "All jets #eta (N_{jets} #geq 2)",             "#eta(jets)",   47,-4.7, 4.7);
    AllJetEta_Zinc3jet                  = newTH1D("AllJetEta_Zinc3jet",                  "All jets #eta (N_{jets} #geq 3)",             "#eta(jets)",   47,-4.7, 4.7);
    AllJetEta_Zinc4jet                  = newTH1D("AllJetEta_Zinc4jet",                  "All jets #eta (N_{jets} #geq 4)",             "#eta(jets)",   47,-4.7, 4.7);
    
    //--------
    FirstJetPhi_Zinc1jet                = newTH1D("FirstJetPhi_Zinc1jet",                "1st jet #phi (N_{jets} #geq 1)",              "#phi(j_{1})",  30,-PI, PI );
    SecondJetPhi_Zinc2jet               = newTH1D("SecondJetPhi_Zinc2jet",               "2nd jet #phi (N_{jets} #geq 2)",              "#phi(j_{2})",  30,-PI, PI );
    ThirdJetPhi_Zinc3jet                = newTH1D("ThirdJetPhi_Zinc3jet",                "3rd jet #phi (N_{jets} #geq 3)",              "#phi(j_{3})",  30,-PI, PI );
    FourthJetPhi_Zinc4jet               = newTH1D("FourthJetPhi_Zinc4jet",               "4th jet #phi (N_{jets} #geq 4)",              "#phi(j_{4})",  30,-PI, PI );
    FifthJetPhi_Zinc5jet                = newTH1D("FifthJetPhi_Zinc5jet",                "5th jet #phi (N_{jets} #geq 5)",              "#phi(j_{5})",  30,-PI, PI );
    SixthJetPhi_Zinc6jet                = newTH1D("SixthJetPhi_Zinc6jet",                "6th jet #phi (N_{jets} #geq 6)",              "#phi(j_{6})",  30,-PI, PI );

    FirstJetPhi_Zexc1jet                = newTH1D("FirstJetPhi_Zexc1jet",                "1st jet #phi (N_{jets} = 1)",                 "#phi(j_{1})",  30,-PI, PI );
    SecondJetPhi_Zexc2jet               = newTH1D("SecondJetPhi_Zexc2jet",               "2nd jet #phi (N_{jets} = 2)",                 "#phi(j_{2})",  30,-PI, PI );

    AllJetPhi_Zinc1jet                  = newTH1D("AllJetPhi_Zinc1jet",                  "All jets #phi (N_{jets} #geq 1)",             "#phi(jets)",   30,-PI, PI );
    AllJetPhi_Zinc2jet                  = newTH1D("AllJetPhi_Zinc2jet",                  "All jets #phi (N_{jets} #geq 2)",             "#phi(jets)",   30,-PI, PI );
    AllJetPhi_Zinc3jet                  = newTH1D("AllJetPhi_Zinc3jet",                  "All jets #phi (N_{jets} #geq 3)",             "#phi(jets)",   30,-PI, PI );
    AllJetPhi_Zinc4jet                  = newTH1D("AllJetPhi_Zinc4jet",                  "All jets #phi (N_{jets} #geq 4)",             "#phi(jets)",   30,-PI, PI );

    lepPt_Zinc0jet                      = newTH1D("lepPt_Zinc0jet",                      "1st & 2nd lep p_{T} (N_{jets} #geq 0)",       lpT,     40, 0, 200);
    lepPt_Zinc1jet                      = newTH1D("lepPt_Zinc1jet",                      "1st & 2nd lep p_{T} (N_{jets} #geq 1)",       lpT,     40, 0, 200);
    lepPt_Zinc2jet                      = newTH1D("lepPt_Zinc2jet",                      "1st & 2nd lep p_{T} (N_{jets} #geq 2)",       lpT,     40, 0, 200);
    lepPt_Zinc3jet                      = newTH1D("lepPt_Zinc3jet",                      "1st & 2nd lep p_{T} (N_{jets} #geq 3)",       lpT,     40, 0, 200);
    lepPt_Zinc4jet                      = newTH1D("lepPt_Zinc4jet",                      "1st & 2nd lep p_{T} (N_{jets} #geq 4)",       lpT,     40, 0, 200);
    lepPt_Zinc5jet                      = newTH1D("lepPt_Zinc5jet",                      "1st & 2nd lep p_{T} (N_{jets} #geq 5)",       lpT,     40, 0, 200);

    GLepBarePtZinc0jet                   = newTH1D("GLepBarePtZinc0jet",                   "gen 1st & 2nd lep p_{T} (N_{jets} #geq 0)",   lpT,     40, 0, 200);
    GLepBarePtZinc1jet                   = newTH1D("GLepBarePtZinc1jet",                   "gen 1st & 2nd lep p_{T} (N_{jets} #geq 1)",   lpT,     40, 0, 200);
    GLepBarePtZinc2jet                   = newTH1D("GLepBarePtZinc2jet",                   "gen 1st & 2nd lep p_{T} (N_{jets} #geq 2)",   lpT,     40, 0, 200);

    lepPt_Zexc0jet                      = newTH1D("lepPt_Zexc0jet",                      "1st & 2nd lep p_{T} (N_{jets} = 0)",          lpT,     40, 0, 200);
    lepPt_Zexc1jet                      = newTH1D("lepPt_Zexc1jet",                      "1st & 2nd lep p_{T} (N_{jets} = 1)",          lpT,     40, 0, 200);
    lepPt_Zexc2jet                      = newTH1D("lepPt_Zexc2jet",                      "1st & 2nd lep p_{T} (N_{jets} = 2)",          lpT,     40, 0, 200);
    lepPt_Zexc3jet                      = newTH1D("lepPt_Zexc3jet",                      "1st & 2nd lep p_{T} (N_{jets} = 3)",          lpT,     40, 0, 200);
    lepPt_Zexc4jet                      = newTH1D("lepPt_Zexc4jet",                      "1st & 2nd lep p_{T} (N_{jets} = 4)",          lpT,     40, 0, 200);
    lepPt_Zexc5jet                      = newTH1D("lepPt_Zexc5jet",                      "1st & 2nd lep p_{T} (N_{jets} = 5)",          lpT,     40, 0, 200);

    dPhiLeptons_Zexc0jet                = newTH1D("dPhiLeptons_Zexc0jet",                "#Delta #phi btw lep (N_{jets} = 0)",          ldPhi,     50, 0, PI);
    dPhiLeptons_Zexc1jet                = newTH1D("dPhiLeptons_Zexc1jet",                "#Delta #phi btw lep (N_{jets} = 1)",          ldPhi,     50, 0, PI);
    dPhiLeptons_Zexc2jet                = newTH1D("dPhiLeptons_Zexc2jet",                "#Delta #phi btw lep (N_{jets} = 2)",          ldPhi,     50, 0, PI);
    dPhiLeptons_Zexc3jet                = newTH1D("dPhiLeptons_Zexc3jet",                "#Delta #phi btw lep (N_{jets} = 3)",          ldPhi,     50, 0, PI);
    dPhiLeptons_Zexc4jet                = newTH1D("dPhiLeptons_Zexc4jet",                "#Delta #phi btw lep (N_{jets} = 4)",          ldPhi,     50, 0, PI);
    dPhiLeptons_Zexc5jet                = newTH1D("dPhiLeptons_Zexc5jet",                "#Delta #phi btw lep (N_{jets} = 5)",          ldPhi,     50, 0, PI);

    dPhiLeptons_Zinc0jet                = newTH1D("dPhiLeptons_Zinc0jet",                "#Delta #phi btw lep (N_{jets} #geq 0)",       ldPhi,     50, 0, PI);
    dPhiLeptons_Zinc1jet                = newTH1D("dPhiLeptons_Zinc1jet",                "#Delta #phi btw lep (N_{jets} #geq 1)",       ldPhi,     50, 0, PI);
    dPhiLeptons_Zinc2jet                = newTH1D("dPhiLeptons_Zinc2jet",                "#Delta #phi btw lep (N_{jets} #geq 2)",       ldPhi,     50, 0, PI);
    dPhiLeptons_Zinc3jet                = newTH1D("dPhiLeptons_Zinc3jet",                "#Delta #phi btw lep (N_{jets} #geq 3)",       ldPhi,     50, 0, PI);
    dPhiLeptons_Zinc4jet                = newTH1D("dPhiLeptons_Zinc4jet",                "#Delta #phi btw lep (N_{jets} #geq 4)",       ldPhi,     50, 0, PI);
    dPhiLeptons_Zinc5jet                = newTH1D("dPhiLeptons_Zinc5jet",                "#Delta #phi btw lep (N_{jets} #geq 5)",       ldPhi,     50, 0, PI);

    dEtaLeptons_Zexc0jet                = newTH1D("dEtaLeptons_Zexc0jet",                "#Delta #eta btw lep (N_{jets} = 0)",          ldEta,      50,-5, 5);
    dEtaLeptons_Zexc1jet                = newTH1D("dEtaLeptons_Zexc1jet",                "#Delta #eta btw lep (N_{jets} = 1)",          ldEta,      50,-5, 5);
    dEtaLeptons_Zexc2jet                = newTH1D("dEtaLeptons_Zexc2jet",                "#Delta #eta btw lep (N_{jets} = 2)",          ldEta,      50,-5, 5);
    dEtaLeptons_Zexc3jet                = newTH1D("dEtaLeptons_Zexc3jet",                "#Delta #eta btw lep (N_{jets} = 3)",          ldEta,      50,-5, 5);
    dEtaLeptons_Zexc4jet                = newTH1D("dEtaLeptons_Zexc4jet",                "#Delta #eta btw lep (N_{jets} = 4)",          ldEta,      50,-5, 5);
    dEtaLeptons_Zexc5jet                = newTH1D("dEtaLeptons_Zexc5jet",                "#Delta #eta btw lep (N_{jets} = 5)",          ldEta,      50,-5, 5);

    dEtaLeptons_Zinc0jet                = newTH1D("dEtaLeptons_Zinc0jet",                "#Delta #eta btw lep (N_{jets} #geq 0)",       ldEta,      50,-5, 5);
    dEtaLeptons_Zinc1jet                = newTH1D("dEtaLeptons_Zinc1jet",                "#Delta #eta btw lep (N_{jets} #geq 1)",       ldEta,      50,-5, 5);
    dEtaLeptons_Zinc2jet                = newTH1D("dEtaLeptons_Zinc2jet",                "#Delta #eta btw lep (N_{jets} #geq 2)",       ldEta,      50,-5, 5);
    dEtaLeptons_Zinc3jet                = newTH1D("dEtaLeptons_Zinc3jet",                "#Delta #eta btw lep (N_{jets} #geq 3)",       ldEta,      50,-5, 5);
    dEtaLeptons_Zinc4jet                = newTH1D("dEtaLeptons_Zinc4jet",                "#Delta #eta btw lep (N_{jets} #geq 4)",       ldEta,      50,-5, 5);
    dEtaLeptons_Zinc5jet                = newTH1D("dEtaLeptons_Zinc5jet",                "#Delta #eta btw lep (N_{jets} #geq 5)",       ldEta,      50,-5, 5);

    dRLeptons_Zinc0jet                  = newTH1D("dRLeptons_Zinc0jet",                  "#Delta R btw lep (N_{jets} #geq 0)",          ldR,        50, 0, 5);
    dRLeptons_Zinc1jet                  = newTH1D("dRLeptons_Zinc1jet",                  "#Delta R btw lep (N_{jets} #geq 1)",          ldR,        50, 0, 5);
    dRLeptons_Zinc2jet                  = newTH1D("dRLeptons_Zinc2jet",                  "#Delta R btw lep (N_{jets} #geq 2)",          ldR,        50, 0, 5);
    dRLeptons_Zinc3jet                  = newTH1D("dRLeptons_Zinc3jet",                  "#Delta R btw lep (N_{jets} #geq 3)",          ldR,        50, 0, 5);
    dRLeptons_Zinc4jet                  = newTH1D("dRLeptons_Zinc4jet",                  "#Delta R btw lep (N_{jets} #geq 4)",          ldR,        50, 0, 5);
    dRLeptons_Zinc5jet                  = newTH1D("dRLeptons_Zinc5jet",                  "#Delta R btw lep (N_{jets} #geq 5)",          ldR,        50, 0, 5);

    SpTLeptons_Zexc0jet                 = newTH1D("SpTLeptons_Zexc0jet",                 "#Delta_{pT}^{rel} lep (N_{jets} = 0)",            lSpt,          50, 0, 1);
    SpTLeptons_Zexc1jet                 = newTH1D("SpTLeptons_Zexc1jet",                 "#Delta_{pT}^{rel} lep (N_{jets} = 1)",            lSpt,          50, 0, 1);
    SpTLeptons_Zexc2jet                 = newTH1D("SpTLeptons_Zexc2jet",                 "#Delta_{pT}^{rel} lep (N_{jets} = 2)",            lSpt,          50, 0, 1);
    SpTLeptons_Zexc3jet                 = newTH1D("SpTLeptons_Zexc3jet",                 "#Delta_{pT}^{rel} lep (N_{jets} = 3)",            lSpt,          50, 0, 1);
    SpTLeptons_Zexc4jet                 = newTH1D("SpTLeptons_Zexc4jet",                 "#Delta_{pT}^{rel} lep (N_{jets} = 4)",            lSpt,          50, 0, 1);
    SpTLeptons_Zexc5jet                 = newTH1D("SpTLeptons_Zexc5jet",                 "#Delta_{pT}^{rel} lep (N_{jets} = 5)",            lSpt,          50, 0, 1);

    genSpTLeptons_Zexc2jet              = newTH1D("genSpTLeptons_Zexc2jet",              "gen #Delta_{pT}^{rel} lep (N_{jets} = 2)",        lSpt,          50,0.,1.);

    SpTLeptons_Zinc0jet                 = newTH1D("SpTLeptons_Zinc0jet",                 "#Delta_{pT}^{rel} lep (N_{jets} #geq 0)",         lSpt,          50, 0, 1);
    SpTLeptons_Zinc1jet                 = newTH1D("SpTLeptons_Zinc1jet",                 "#Delta_{pT}^{rel} lep (N_{jets} #geq 1)",         lSpt,          50, 0, 1);
    SpTLeptons_Zinc2jet                 = newTH1D("SpTLeptons_Zinc2jet",                 "#Delta_{pT}^{rel} lep (N_{jets} #geq 2)",         lSpt,          50, 0, 1);
    SpTLeptons_Zinc3jet                 = newTH1D("SpTLeptons_Zinc3jet",                 "#Delta_{pT}^{rel} lep (N_{jets} #geq 3)",         lSpt,          50, 0, 1);
    SpTLeptons_Zinc4jet                 = newTH1D("SpTLeptons_Zinc4jet",                 "#Delta_{pT}^{rel} lep (N_{jets} #geq 4)",         lSpt,          50, 0, 1);
    SpTLeptons_Zinc5jet                 = newTH1D("SpTLeptons_Zinc5jet",                 "#Delta_{pT}^{rel} lep (N_{jets} #geq 5)",         lSpt,          50, 0, 1);

    genSpTLeptons_Zinc2jet              = newTH1D("genSpTLeptons_Zinc2jet",              "gen #Delta_{pT}^{rel} lep (N_{jets} #geq 2)",     lSpt,          50, 0, 1);
    

    RatioJetPt21_Zinc2jet              = newTH1D("RatioJetPt21_Zinc2jet",      "ratio 2nd/1st jet p_{T} (N_{jets} #geq 2)",         "p_{T}(j_{2})/p_{T}(j_{1})",    10 , 0 , 1);
    RatioJetPt32_Zinc3jet              = newTH1D("RatioJetPt32_Zinc3jet",      "ratio 3rd/2nd jet p_{T} (N_{jets} #geq 3)",         "p_{T}(j_{3})/p_{T}(j_{2})",    10 , 0 , 1);
    
    genRatioJetPt21_Zinc2jet           = newTH1D("genRatioJetPt21_Zinc2jet",           "gen ratio 2nd/1st jet p_{T} (N_{jets} #geq 2)",         "p_{T}(j_{2})/p_{T}(j_{1})",    10 , 0 , 1);
    genRatioJetPt32_Zinc3jet           = newTH1D("genRatioJetPt32_Zinc3jet",           "gen ratio 3rd/2nd jet p_{T} (N_{jets} #geq 3)",         "p_{T}(j_{3})/p_{T}(j_{2})",    10 , 0 , 1);

    FirstJetPt_Zexc1jet                 = newTH1D("FirstJetPt_Zexc1jet",                 "1st jet p_{T} (N_{jets} = 1)",                "p_{T}(j_{1}) [GeV]",     nJetPt_Zinc1jet,   jetPt_Zinc1jet);
    SecondJetPt_Zexc2jet                = newTH1D("SecondJetPt_Zexc2jet",                "2nd jet p_{T} (N_{jets} = 2)",                "p_{T}(j_{2}) [GeV]",     nJetPt_Zinc2jet, jetPt_Zinc2jet); 

    genFirstJetPt_Zexc1jet              = newTH1D("genFirstJetPt_Zexc1jet",              "gen 1st jet p_{T} (N_{jets} = 1)",            "p_{T}(j_{1}) [GeV]",     nJetPt_Zinc1jet,   jetPt_Zinc1jet);
    genSecondJetPt_Zexc2jet             = newTH1D("genSecondJetPt_Zexc2jet",             "gen 2nd jet p_{T} (N_{jets} = 2)",            "p_{T}(j_{2}) [GeV]",     nJetPt_Zinc2jet, jetPt_Zinc2jet);


    FirstHighestJetPt_Zinc1jet          = newTH1D("FirstHighestJetPt_Zinc1jet",          "1st Highest jet p_{T} (N_{jets} #geq 1)",     "p_{T}(j_{1}) [GeV]",    nJetPt_Zinc1jet,   jetPt_Zinc1jet);
    FirstHighestJetPt_Zinc2jet          = newTH1D("FirstHighestJetPt_Zinc2jet",          "1st Highest jet p_{T} (N_{jets} #geq 2)",     "p_{T}(j_{1}) [GeV]",    nJetPt_Zinc1jet,   jetPt_Zinc1jet);
    FirstHighestJetPt_Zinc3jet          = newTH1D("FirstHighestJetPt_Zinc3jet",          "1st Highest jet p_{T} (N_{jets} #geq 3)",     "p_{T}(j_{1}) [GeV]",    nJetPt_Zinc1jet,   jetPt_Zinc1jet);
    FirstHighestJetPt_Zinc4jet          = newTH1D("FirstHighestJetPt_Zinc4jet",          "1st Highest jet p_{T} (N_{jets} #geq 4)",     "p_{T}(j_{1}) [GeV]",    nJetPt_Zinc1jet,   jetPt_Zinc1jet);
    FirstHighestJetPt_Zinc5jet          = newTH1D("FirstHighestJetPt_Zinc5jet",          "1st Highest jet p_{T} (N_{jets} #geq 5)",     "p_{T}(j_{1}) [GeV]",    nJetPt_Zinc1jet,   jetPt_Zinc1jet);
    FirstHighestJetPt_Zinc6jet          = newTH1D("FirstHighestJetPt_Zinc6jet",          "1st Highest jet p_{T} (N_{jets} #geq 6)",     "p_{T}(j_{1}) [GeV]",    nJetPt_Zinc1jet,   jetPt_Zinc1jet);

    genFirstHighestJetPt_Zinc1jet       = newTH1D("genFirstHighestJetPt_Zinc1jet",       "gen 1st Highest jet p_{T} (N_{jets} #geq 1)", "p_{T}(j_{1}) [GeV]",    nJetPt_Zinc1jet,   jetPt_Zinc1jet);
    genFirstHighestJetPt_Zinc2jet       = newTH1D("genFirstHighestJetPt_Zinc2jet",       "gen 1st Highest jet p_{T} (N_{jets} #geq 2)", "p_{T}(j_{1}) [GeV]",    nJetPt_Zinc1jet,   jetPt_Zinc1jet);
    genFirstHighestJetPt_Zinc3jet       = newTH1D("genFirstHighestJetPt_Zinc3jet",       "gen 1st Highest jet p_{T} (N_{jets} #geq 3)", "p_{T}(j_{1}) [GeV]",    nJetPt_Zinc1jet,   jetPt_Zinc1jet);;
    genFirstHighestJetPt_Zinc4jet       = newTH1D("genFirstHighestJetPt_Zinc4jet",       "gen 1st Highest jet p_{T} (N_{jets} #geq 4)", "p_{T}(j_{1}) [GeV]",    nJetPt_Zinc1jet,   jetPt_Zinc1jet);
    genFirstHighestJetPt_Zinc5jet       = newTH1D("genFirstHighestJetPt_Zinc5jet",       "gen 1st Highest jet p_{T} (N_{jets} #geq 5)", "p_{T}(j_{1}) [GeV]",    nJetPt_Zinc1jet,   jetPt_Zinc1jet);
    genFirstHighestJetPt_Zinc6jet       = newTH1D("genFirstHighestJetPt_Zinc6jet",       "gen 1st Highest jet p_{T} (N_{jets} #geq 6)", "p_{T}(j_{1}) [GeV]",    nJetPt_Zinc1jet,   jetPt_Zinc1jet);

    SecondHighestJetPt_Zinc2jet         = newTH1D("SecondHighestJetPt_Zinc2jet",         "2nd Highest jet p_{T} (N_{jets} #geq 2)",     "p_{T}(j_{2}) [GeV]",     nJetPt_Zinc2jet, jetPt_Zinc2jet); 
    SecondHighestJetPt_Zinc3jet         = newTH1D("SecondHighestJetPt_Zinc3jet",         "2nd Highest jet p_{T} (N_{jets} #geq 3)",     "p_{T}(j_{2}) [GeV]",     nJetPt_Zinc2jet, jetPt_Zinc2jet); 
    SecondHighestJetPt_Zinc4jet         = newTH1D("SecondHighestJetPt_Zinc4jet",         "2nd Highest jet p_{T} (N_{jets} #geq 4)",     "p_{T}(j_{2}) [GeV]",     nJetPt_Zinc2jet, jetPt_Zinc2jet); 
    SecondHighestJetPt_Zinc5jet         = newTH1D("SecondHighestJetPt_Zinc5jet",         "2nd Highest jet p_{T} (N_{jets} #geq 5)",     "p_{T}(j_{2}) [GeV]",     nJetPt_Zinc2jet, jetPt_Zinc2jet); 
    SecondHighestJetPt_Zinc6jet         = newTH1D("SecondHighestJetPt_Zinc6jet",         "2nd Highest jet p_{T} (N_{jets} #geq 6)",     "p_{T}(j_{2}) [GeV]",     nJetPt_Zinc2jet, jetPt_Zinc2jet); 

    genSecondHighestJetPt_Zinc2jet      = newTH1D("genSecondHighestJetPt_Zinc2jet",      "gen 2nd Highest jet p_{T} (N_{jets} #geq 2)", "p_{T}(j_{2}) [GeV]",     nJetPt_Zinc2jet, jetPt_Zinc2jet); 
    genSecondHighestJetPt_Zinc3jet      = newTH1D("genSecondHighestJetPt_Zinc3jet",      "gen 2nd Highest jet p_{T} (N_{jets} #geq 3)", "p_{T}(j_{2}) [GeV]",     nJetPt_Zinc2jet, jetPt_Zinc2jet); 
    genSecondHighestJetPt_Zinc4jet      = newTH1D("genSecondHighestJetPt_Zinc4jet",      "gen 2nd Highest jet p_{T} (N_{jets} #geq 4)", "p_{T}(j_{2}) [GeV]",     nJetPt_Zinc2jet, jetPt_Zinc2jet); 
    genSecondHighestJetPt_Zinc5jet      = newTH1D("genSecondHighestJetPt_Zinc5jet",      "gen 2nd Highest jet p_{T} (N_{jets} #geq 5)", "p_{T}(j_{2}) [GeV]",     nJetPt_Zinc2jet, jetPt_Zinc2jet); 
    genSecondHighestJetPt_Zinc6jet      = newTH1D("genSecondHighestJetPt_Zinc6jet",      "gen 2nd Highest jet p_{T} (N_{jets} #geq 6)", "p_{T}(j_{2}) [GeV]",     nJetPt_Zinc2jet, jetPt_Zinc2jet); 

    ThirdHighestJetPt_Zinc3jet          = newTH1D("ThirdHighestJetPt_Zinc3jet",          "3rd Highest jet p_{T} (N_{jets} #geq 3)",     "p_{T}(j_{3}) [GeV]",     nJetPt_Zinc3jet, jetPt_Zinc3jet);
    ThirdHighestJetPt_Zinc4jet          = newTH1D("ThirdHighestJetPt_Zinc4jet",          "3rd Highest jet p_{T} (N_{jets} #geq 4)",     "p_{T}(j_{3}) [GeV]",     nJetPt_Zinc3jet, jetPt_Zinc3jet);
    ThirdHighestJetPt_Zinc5jet          = newTH1D("ThirdHighestJetPt_Zinc5jet",          "3rd Highest jet p_{T} (N_{jets} #geq 5)",     "p_{T}(j_{3}) [GeV]",     nJetPt_Zinc3jet, jetPt_Zinc3jet);
    ThirdHighestJetPt_Zinc6jet          = newTH1D("ThirdHighestJetPt_Zinc6jet",          "3rd Highest jet p_{T} (N_{jets} #geq 6)",     "p_{T}(j_{3}) [GeV]",     nJetPt_Zinc3jet, jetPt_Zinc3jet);

    genThirdHighestJetPt_Zinc3jet       = newTH1D("genThirdHighestJetPt_Zinc3jet",       "gen 3rd Highest jet p_{T} (N_{jets} #geq 3)", "p_{T}(j_{3}) [GeV]",     nJetPt_Zinc3jet, jetPt_Zinc3jet); 
    genThirdHighestJetPt_Zinc4jet       = newTH1D("genThirdHighestJetPt_Zinc4jet",       "gen 3rd Highest jet p_{T} (N_{jets} #geq 4)", "p_{T}(j_{3}) [GeV]",     nJetPt_Zinc3jet, jetPt_Zinc3jet); 
    genThirdHighestJetPt_Zinc5jet       = newTH1D("genThirdHighestJetPt_Zinc5jet",       "gen 3rd Highest jet p_{T} (N_{jets} #geq 5)", "p_{T}(j_{3}) [GeV]",     nJetPt_Zinc3jet, jetPt_Zinc3jet); 
    genThirdHighestJetPt_Zinc6jet       = newTH1D("genThirdHighestJetPt_Zinc6jet",       "gen 3rd Highest jet p_{T} (N_{jets} #geq 6)", "p_{T}(j_{3}) [GeV]",     nJetPt_Zinc3jet, jetPt_Zinc3jet); 

    AllJetPt_Zinc1jet                   = newTH1D("AllJetPt_Zinc1jet",                   "All jets (N_{jets} #geq 1)",                  "p_{T}(jets) [GeV]",     80, 0, 400);
    AllJetPt_Zinc2jet                   = newTH1D("AllJetPt_Zinc2jet",                   "All jets (N_{jets} #geq 2)",                  "p_{T}(jets) [GeV]",     80, 0, 400);
    AllJetPt_Zinc3jet                   = newTH1D("AllJetPt_Zinc3jet",                   "All jets (N_{jets} #geq 3)",                  "p_{T}(jets) [GeV]",     80, 0, 400);
    AllJetPt_Zinc4jet                   = newTH1D("AllJetPt_Zinc4jet",                   "All jets (N_{jets} #geq 4)",                  "p_{T}(jets) [GeV]",     80, 0, 400);


    hPtEtaBackJet_Zexc1jet              = newTH2D("PtEtaBackJet_Zexc1jet",              "p_{T} #eta (N_{jets} = 1)",       nJetPtEta_Zinc1jet, jetPtEta_Zinc1jet   ,      12  ,  0., 2.4);
    hPtEtaBackJetMVA_Zexc1jet           = newTH2D("PtEtaBackJetMVA_Zexc1jet",           "p_{T} #eta (N_{jets} = 1)",       nJetPtEta_Zinc1jet, jetPtEta_Zinc1jet   ,      12  ,  0., 2.4);
    FirstJetPtEta_Zinc1jet              = newTH2D("FirstJetPtEta_Zinc1jet",              "1st jet p_{T} #eta (N_{jets} #geq 1)",       nJetPtEta_Zinc1jet, jetPtEta_Zinc1jet   ,      12  ,  0., 2.4);
    SecondJetPtEta_Zinc2jet             = newTH2D("SecondJetPtEta_Zinc2jet",             "2nd jet p_{T} #eta (N_{jets} #geq 2)",       nJetPtEta_Zinc2jet, jetPtEta_Zinc2jet   ,       8  ,  0., 2.4);
    ThirdJetPtEta_Zinc3jet              = newTH2D("ThirdJetPtEta_Zinc3jet",              "3rd jet p_{T} #eta (N_{jets} #geq 3)",       nJetPt_Zinc3jet, jetPt_Zinc3jet	        ,       6  ,  0., 2.4);
    FourthJetPtEta_Zinc4jet             = newTH2D("FourthJetPtEta_Zinc4jet",             "4th jet p_{T} #eta (N_{jets} #geq 4)",       nJetPt_Zinc4jet, jetPt_Zinc4jet         ,       4  ,  0., 2.4);
    FifthJetPtEta_Zinc5jet              = newTH2D("FifthJetPtEta_Zinc5jet",              "5th jet p_{T} #eta (N_{jets} #geq 5)",       nJetPt_Zinc5jet, jetPt_Zinc5jet         ,       2  ,  0., 2.4);
    SixthJetPtEta_Zinc6jet              = newTH2D("SixthJetPtEta_Zinc6jet",              "6th jet p_{T} #eta (N_{jets} #geq 6)",       nJetPt_Zinc5jet, jetPt_Zinc5jet         ,       2  ,  0., 2.4);

    genFirstJetPtEta_Zinc1jet           = newTH2D("genFirstJetPtEta_Zinc1jet",           "gen 1st jet p_{T} #eta (N_{jets} #geq 1)",    nJetPtEta_Zinc1jet, jetPtEta_Zinc1jet  ,      12  ,  0., 2.4);
    genSecondJetPtEta_Zinc2jet          = newTH2D("genSecondJetPtEta_Zinc2jet",          "gen 2nd jet p_{T} #eta (N_{jets} #geq 2)",    nJetPtEta_Zinc2jet, jetPtEta_Zinc2jet  ,       8  ,  0., 2.4);
    genThirdJetPtEta_Zinc3jet           = newTH2D("genThirdJetPtEta_Zinc3jet",           "gen 3rd jet p_{T} #eta (N_{jets} #geq 3)",    nJetPt_Zinc3jet, jetPt_Zinc3jet        ,       6  ,  0., 2.4);
    genFourthJetPtEta_Zinc4jet          = newTH2D("genFourthJetPtEta_Zinc4jet",          "gen 4th jet p_{T} #eta (N_{jets} #geq 4)",    nJetPt_Zinc4jet, jetPt_Zinc4jet        ,       4  ,  0., 2.4);
    genFifthJetPtEta_Zinc5jet           = newTH2D("genFifthJetPtEta_Zinc5jet",           "gen 5th jet p_{T} #eta (N_{jets} #geq 5)",    nJetPt_Zinc5jet, jetPt_Zinc5jet        ,       2  ,  0., 2.4);
    genSixthJetPtEta_Zinc6jet           = newTH2D("genSixthJetPtEta_Zinc6jet",           "gen 6th jet p_{T} #eta (N_{jets} #geq 6)",    nJetPt_Zinc5jet, jetPt_Zinc5jet        ,       2  ,  0., 2.4);
    
    

    ZNGoodJetsNVtx_Zexc = newTH2D("ZNGoodJetsNVtx_Zexc","NVtx vs Jet Counter (excl.)", 11, -0.5, 10.5, 45, 0.5, 45.5);
    ZNGoodJetsNVtx_Zexc->GetXaxis()->SetBinLabel(1, "= 0");
    ZNGoodJetsNVtx_Zexc->GetXaxis()->SetBinLabel(2, "= 1");
    ZNGoodJetsNVtx_Zexc->GetXaxis()->SetBinLabel(3, "= 2");
    ZNGoodJetsNVtx_Zexc->GetXaxis()->SetBinLabel(4, "= 3");
    ZNGoodJetsNVtx_Zexc->GetXaxis()->SetBinLabel(5, "= 4");
    ZNGoodJetsNVtx_Zexc->GetXaxis()->SetBinLabel(6, "= 5");
    ZNGoodJetsNVtx_Zexc->GetXaxis()->SetBinLabel(7, "= 6");
    ZNGoodJetsNVtx_Zexc->GetXaxis()->SetBinLabel(8, "= 7");
    ZNGoodJetsNVtx_Zexc->GetXaxis()->SetBinLabel(9, "= 8");
    ZNGoodJetsNVtx_Zexc->GetXaxis()->SetBinLabel(10,"= 9");
    ZNGoodJetsNVtx_Zexc->GetXaxis()->SetBinLabel(11,"= 10");
    
    ZNGoodJets_Zexc_check = newTH1D("ZNGoodJets_Zexc_check","Jet Counter (incl.) CHECK", "N_{jets}", 8, -0.5, 7.5);
    ZNGoodJets_Zexc_check->GetXaxis()->SetBinLabel(1,"#geq 0");
    ZNGoodJets_Zexc_check->GetXaxis()->SetBinLabel(2,"#geq 1");
    ZNGoodJets_Zexc_check->GetXaxis()->SetBinLabel(3,"#geq 2");
    ZNGoodJets_Zexc_check->GetXaxis()->SetBinLabel(4,"#geq 3");
    ZNGoodJets_Zexc_check->GetXaxis()->SetBinLabel(5,"#geq 4");
    ZNGoodJets_Zexc_check->GetXaxis()->SetBinLabel(6,"#geq 5");
    ZNGoodJets_Zexc_check->GetXaxis()->SetBinLabel(7,"#geq 6");
    ZNGoodJets_Zexc_check->GetXaxis()->SetBinLabel(8,"#geq 7");

    ZNGoodJets_Zexc_NoWeight = newTH1D("ZNGoodJets_Zexc_NoWeight","Unweighted jet Counter (excl.)", "N_{jets}", 8, -0.5, 7.5);
    ZNGoodJets_Zexc_NoWeight->GetXaxis()->SetBinLabel(1,"= 0");
    ZNGoodJets_Zexc_NoWeight->GetXaxis()->SetBinLabel(2,"= 1");
    ZNGoodJets_Zexc_NoWeight->GetXaxis()->SetBinLabel(3,"= 2");
    ZNGoodJets_Zexc_NoWeight->GetXaxis()->SetBinLabel(4,"= 3");
    ZNGoodJets_Zexc_NoWeight->GetXaxis()->SetBinLabel(5,"= 4");
    ZNGoodJets_Zexc_NoWeight->GetXaxis()->SetBinLabel(6,"= 5");
    ZNGoodJets_Zexc_NoWeight->GetXaxis()->SetBinLabel(7,"= 6");
    ZNGoodJets_Zexc_NoWeight->GetXaxis()->SetBinLabel(8,"= 7");

    ZNGoodJets_Zinc_NoWeight = newTH1D("ZNGoodJets_Zinc_NoWeight","Unweighted jet Counter (incl.)", "N_{jets}", 8, -0.5, 7.5);
    ZNGoodJets_Zinc_NoWeight->GetXaxis()->SetBinLabel(1,"#geq 0");
    ZNGoodJets_Zinc_NoWeight->GetXaxis()->SetBinLabel(2,"#geq 1");
    ZNGoodJets_Zinc_NoWeight->GetXaxis()->SetBinLabel(3,"#geq 2");
    ZNGoodJets_Zinc_NoWeight->GetXaxis()->SetBinLabel(4,"#geq 3");
    ZNGoodJets_Zinc_NoWeight->GetXaxis()->SetBinLabel(5,"#geq 4");
    ZNGoodJets_Zinc_NoWeight->GetXaxis()->SetBinLabel(6,"#geq 5");
    ZNGoodJets_Zinc_NoWeight->GetXaxis()->SetBinLabel(7,"#geq 6");
    ZNGoodJets_Zinc_NoWeight->GetXaxis()->SetBinLabel(8,"#geq 7");

    //DPS histograms
    //binning 
    int nbinSpt=21;
    double binSpt[22]={0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,.45,.5,.55,.6,.65,0.7,0.75,0.8,0.85,0.9,0.94,0.98,1};

    //-- jets and Z
    TwoJetsPtDiff_Zexc2jet        = newTH1D("TwoJetsPtDiff_Zexc2jet",        "pT diff of the two highest jet (N_{jets} = 2)",                                "#Delta pT(j_{1}j_{2}) [GeV]",      10,  0, 100);
    genTwoJetsPtDiff_Zexc2jet     = newTH1D("genTwoJetsPtDiff_Zexc2jet",     "gen pT diff of the two highest jet (N_{jets} = 2)",                            "#Delta pT(j_{1}j_{2}) [GeV]",      10,  0, 100);
    JetsMass_Zexc2jet             = newTH1D("JetsMass_Zexc2jet",             "2Jets Invariant Mass (N_{jets} = 2)",                                          Mjj, 24, 20, 620);
    genJetsMass_Zexc2jet          = newTH1D("genJetsMass_Zexc2jet",          "gen 2Jets Invariant Mass (N_{jets} = 2)",                                      Mjj, 24, 20, 620);
    ptBal_Zexc2jet                = newTH1D("ptBal_Zexc2jet",                "Vectorial pT sum: Z_{pT} + DiJet_{pT} (N_{jets} = 2)",                          "#Sigma pT [GeV]",      50,  0, 100);
    genptBal_Zexc2jet             = newTH1D("genptBal_Zexc2jet",             "gen Vectorial pT sum: Z_{pT} + DiJet_{pT} (N_{jets} = 2)",                      "#Sigma pT [GeV]",      50,  0, 100);
    dPhiJets_Zexc2jet             = newTH1D("dPhiJets_Zexc2jet",             "#Delta#phi btwn jets (N_{jets} = 2)",                                          jdPhi,           20,  0, PI);
    gendPhiJets_Zexc2jet          = newTH1D("gendPhiJets_Zexc2jet",          "gen #Delta#phi btwn jets (N_{jets} = 2)",                                      jdPhi,           20,  0, PI);
    dEtaJets_Zexc2jet             = newTH1D("dEtaJets_Zexc2jet",             "#Delta#eta btwn jets (N_{jets} = 2)",                                          jdEta,           48, 0, 4.8);
    gendEtaJets_Zexc2jet          = newTH1D("gendEtaJets_Zexc2jet",          "gen #Delta#eta btwn jets (N_{jets} = 2)",                                      jdEta,           48, 0, 4.8);
    dEtaFirstJetZ_Zexc2jet        = newTH1D("dEtaFirstJetZ_Zexc2jet",        "#Delta#eta btwn Jet_{1} and Z (N_{jets} = 2)",                                 "#Delta#eta(j_{1}Z)",           50, -6, 6);
    gendEtaFirstJetZ_Zexc2jet     = newTH1D("gendEtaFirstJetZ_Zexc2jet",     "gen #Delta#eta btwn Jet_{1} and Z (N_{jets} = 2)",                             "#Delta#eta(j_{1}Z)",           50, -6, 6);
    dEtaSecondJetZ_Zexc2jet       = newTH1D("dEtaSecondJetZ_Zexc2jet",       "#Delta#eta btwn Jet_{2} and Z (N_{jets} = 2)",                                 "#Delta#eta(j_{2}Z)",           50, -6, 6);
    gendEtaSecondJetZ_Zexc2jet    = newTH1D("gendEtaSecondJetZ_Zexc2jet",    "gen #Delta#eta btwn Jet_{2} and Z (N_{jets} = 2)",                             "#Delta#eta(j_{2}Z)",           50, -6, 6);
    dEtaJet1Plus2Z_Zexc2jet       = newTH1D("dEtaJet1Plus2Z_Zexc2jet",       "#Delta#eta btwn jets and Z (N_{jets} = 2)",                                    "#Delta#eta(j_{12}Z)",           120, -6, 6);
    gendEtaJet1Plus2Z_Zexc2jet    = newTH1D("gendEtaJet1Plus2Z_Zexc2jet",    "gen #Delta#eta btwn jets and Z (N_{jets} = 2)",                                "#Delta#eta(j_{12}Z)",           120, -6, 6);
    PHI_Zexc2jet                  = newTH1D("PHI_Zexc2jet",                  "#phi: Angle btwn the two subsystems planes (N_{jets} = 2)",                    "#phi(j_{12}Z)",                 25,  0, PI);
    genPHI_Zexc2jet               = newTH1D("genPHI_Zexc2jet",               "gen #phi: Angle btwn the two subsystems planes (N_{jets} = 2)",                "#phi(j_{12}Z)",                 25,  0, PI);
    PHI_T_Zexc2jet                = newTH1D("PHI_T_Zexc2jet",                "#Delta S Angle btwn lep and jet pair in T-plane (N_{jets} = 2)",            "#Delta S(j_{12}Z)",             10,  0, PI);
    genPHI_T_Zexc2jet             = newTH1D("genPHI_T_Zexc2jet",             "gen #Delta S Angle btwn lep and jet pair in T-plane (N_{jets} = 2)",        "#Delta S(j_{12}Z)",             10,  0, PI);
    SpT_Zexc2jet                  = newTH1D("SpT_Zexc2jet",                  "#Delta_{pT}^{rel} lep and jets combined (N_{jets} = 2)",                   Spt,    20,  0, 1);
    genSpT_Zexc2jet               = newTH1D("genSpT_Zexc2jet",               "gen #Delta_{pT}^{rel} lep and jets combined (N_{jets} = 2)",               Spt,    20,  0, 1);
    SpTJets_Zexc2jet              = newTH1D("SpTJets_Zexc2jet",              "#Delta_{pT}^{rel} jets (N_{jets} = 2)",                                  jSpt,   20,  0, 1);
    genSpTJets_Zexc2jet           = newTH1D("genSpTJets_Zexc2jet",           "gen #Delta_{pT}^{rel} jets (N_{jets} = 2)",                              jSpt,   20,  0, 1);
    SPhi_Zexc2jet                 = newTH1D("SPhi_Zexc2jet",                 "S_{#phi} lep and jets combined (N_{jets} = 2)",                            Sphi,   50,  0, PI);
    genSPhi_Zexc2jet              = newTH1D("genSPhi_Zexc2jet",              "gen S_{#phi} lep and jets combined (N_{jets} = 2)",                        Sphi,   50,  0, PI);

    TwoJetsPtDiff_Zinc2jet        = newTH1D("TwoJetsPtDiff_Zinc2jet",        "pT diff of the two highest jet (N_{jets} #geq 2)",                             "#Delta pT(j_{1}j_{2}) [GeV]",      10,  0, 100);
    genTwoJetsPtDiff_Zinc2jet     = newTH1D("genTwoJetsPtDiff_Zinc2jet",     "gen pT diff of the two highest jet (N_{jets} #geq 2)",                         "#Delta pT(j_{1}j_{2}) [GeV]",      10,  0, 100);
    BestTwoJetsPtDiff_Zinc2jet    = newTH1D("BestTwoJetsPtDiff_Zinc2jet",    "Best pT diff of the two highest jet (N_{jets} #geq 2)",                        "#Delta pT(j_{1}j_{2}) [GeV]",      10,  0, 100);
    genBestTwoJetsPtDiff_Zinc2jet = newTH1D("genBestTwoJetsPtDiff_Zinc2jet", "gen Best pT diff of the two highest jet (N_{jets} #geq 2)",                    "#Delta pT(j_{1}j_{2}) [GeV]",      10,  0, 100);
    JetsMass_Zinc2jet             = newTH1D("JetsMass_Zinc2jet",             "2Jets Invariant Mass (N_{jets} #geq 2)",                                       Mjj, 24, 20, 620);
    genJetsMass_Zinc2jet          = newTH1D("genJetsMass_Zinc2jet",          "gen 2Jets Invariant Mass (N_{jets} #geq 2)",                                   Mjj, 24, 20, 620);
    BestJetsMass_Zinc2jet         = newTH1D("BestJetsMass_Zinc2jet",         "Best 2Jets Invariant Mass (N_{jets} #geq 2)",                                  Mjj, 24, 20, 620);
    genBestJetsMass_Zinc2jet      = newTH1D("genBestJetsMass_Zinc2jet",      "gen Best 2Jets Invariant Mass (N_{jets} #geq 2)",                              Mjj, 24, 20, 620);
    ptBal_Zinc2jet                = newTH1D("ptBal_Zinc2jet",                "Vectorial pT sum: Z_{pT} + DiJet_{pT} (N_{jets} #geq 2)",                       "#Sigma pT [GeV]",      50,  0, 100);
    genptBal_Zinc2jet             = newTH1D("genptBal_Zinc2jet",             "gen Vectorial pT sum: Z_{pT} + DiJet_{pT} (N_{jets} #geq 2)",                   "#Sigma pT [GeV]",      50,  0, 100);
    
    BestdPhiJets_Zinc2jet         = newTH1D("BestdPhiJets_Zinc2jet",         "Best #Delta#phi btwn jets (N_{jets} #geq 2)",                                  jdPhi,           20,  0, PI);
    genBestdPhiJets_Zinc2jet      = newTH1D("genBestdPhiJets_Zinc2jet",      "gen Best #Delta#phi btwn jets (N_{jets} #geq 2)",                              jdPhi,           20,  0, PI);
    dEtaJets_Zinc2jet             = newTH1D("dEtaJets_Zinc2jet",             "#Delta#eta btwn jets (N_{jets} #geq 2)",                                       jdEta,           48, 0, 4.8);
    gendEtaJets_Zinc2jet          = newTH1D("gendEtaJets_Zinc2jet",          "gen #Delta#eta btwn jets (N_{jets} #geq 2)",                                   jdEta,           48, 0, 4.8);
    dEtaFirstJetZ_Zinc2jet        = newTH1D("dEtaFirstJetZ_Zinc2jet",        "#Delta#eta btwn Jet_{1} and Z (N_{jets} #geq 2)",                              "#Delta#eta(j_{1}Z)",           50, -6, 6);
    gendEtaFirstJetZ_Zinc2jet     = newTH1D("gendEtaFirstJetZ_Zinc2jet",     "gen #Delta#eta btwn Jet_{1} and Z (N_{jets} #geq 2)",                          "#Delta#eta(j_{1}Z)",           50, -6, 6);
    dEtaSecondJetZ_Zinc2jet       = newTH1D("dEtaSecondJetZ_Zinc2jet",       "#Delta#eta btwn Jet_{2} and Z (N_{jets} #geq 2)",                              "#Delta#eta(j_{2}Z)",           50, -6, 6);
    gendEtaSecondJetZ_Zinc2jet    = newTH1D("gendEtaSecondJetZ_Zinc2jet",    "gen #Delta#eta btwn Jet_{2} and Z (N_{jets} #geq 2)",                          "#Delta#eta(j_{2}Z)",           120, -6, 6);
    dEtaJet1Plus2Z_Zinc2jet       = newTH1D("dEtaJet1Plus2Z_Zinc2jet",       "#Delta#eta btwn jets and Z (N_{jets} #geq 2)",                                 "#Delta#eta(j_{12}Z)",          120, -6, 6);
    gendEtaJet1Plus2Z_Zinc2jet    = newTH1D("gendEtaJet1Plus2Z_Zinc2jet",    "gen #Delta#eta btwn jets and Z (N_{jets} #geq 2)",                             "#Delta#eta(j_{12}Z)",          120, -6, 6);
    PHI_Zinc2jet                  = newTH1D("PHI_Zinc2jet",                  "#phi: Angle btwn the two subsystems planes (N_{jets} #geq 2)",                 "#phi(j_{12}Z)",                 25,  0, PI);
    genPHI_Zinc2jet               = newTH1D("genPHI_Zinc2jet",               "gen #phi: Angle btwn the two subsystems planes (N_{jets} #geq 2)",             "#phi(j_{12}Z)",                 25,  0, PI);
    BestPHI_Zinc2jet              = newTH1D("BestPHI_Zinc2jet",              "Best #phi: Angle btwn the two subsystems planes (N_{jets} #geq 2)",            "#phi(j_{12}Z)",                 25,  0, PI);
    genBestPHI_Zinc2jet           = newTH1D("genBestPHI_Zinc2jet",           "gen Best #phi: Angle btwn the two subsystems planes (N_{jets} #geq 2)",        "#phi(j_{12}Z)",                 25,  0, PI);
    PHI_T_Zinc2jet                = newTH1D("PHI_T_Zinc2jet",                "#Delta S Angle btwn lep and jet pair in T-plane (N_{jets} #geq 2)",         "#Delta S(j_{12}Z)",             10,  0, PI);
    genPHI_T_Zinc2jet             = newTH1D("genPHI_T_Zinc2jet",             "gen #Delta S Angle btwn lep and jet pair in T-plane (N_{jets} #geq 2)",     "#Delta S(j_{12}Z)",             10,  0, PI);
    BestPHI_T_Zinc2jet            = newTH1D("BestPHI_T_Zinc2jet",            "Best #Delta S Angle btwn lep and jet pair in T-plane (N_{jets} #geq 2)",    "#Delta S(j_{12}Z)",             10,  0, PI);
    genBestPHI_T_Zinc2jet         = newTH1D("genBestPHI_T_Zinc2jet",         "gen Best #Delta S Angle btwn lep and jet pair in T-plane (N_{jets} #geq 2)","#Delta S(j_{12}Z)",             10,  0, PI);
    SpT_Zinc2jet                  = newTH1D("SpT_Zinc2jet",                  "#Delta_{pT}^{rel} lep and jets combined (N_{jets} #geq 2)",                 Spt,    20,  0, 1);
    genSpT_Zinc2jet               = newTH1D("genSpT_Zinc2jet",               "gen #Delta_{pT}^{rel} lep and jets combined (N_{jets} #geq 2)",             Spt,    20,  0, 1);
    BestSpT_Zinc2jet              = newTH1D("BestSpT_Zinc2jet",              "Best #Delta_{pT}^{rel} lep and jets combined (N_{jets} #geq 2)",            Spt,    20,  0, 1);
    genBestSpT_Zinc2jet           = newTH1D("genBestSpT_Zinc2jet",           "gen Best #Delta_{pT}^{rel} lep and jets combined (N_{jets} #geq 2)",         Spt,    20,  0, 1);
    SpTJets_Zinc2jet              = newTH1D("SpTJets_Zinc2jet",              "#Delta_{pT}^{rel} jets (N_{jets} #geq 2)",                                jSpt,   20,  0, 1);
    genSpTJets_Zinc2jet           = newTH1D("genSpTJets_Zinc2jet",           "gen #Delta_{pT}^{rel} jets (N_{jets} #geq 2)",                            jSpt,   20,  0, 1);
    BestSpTJets_Zinc2jet          = newTH1D("BestSpTJets_Zinc2jet",          "Best #Delta_{pT}^{rel} jets (N_{jets} #geq 2)",                           jSpt,   20,  0, 1);
    genBestSpTJets_Zinc2jet       = newTH1D("genBestSpTJets_Zinc2jet",       "gen Best #Delta_{pT}^{rel} jets (N_{jets} #geq 2)",                       jSpt,   20,  0, 1);
    SPhi_Zinc2jet                 = newTH1D("SPhi_Zinc2jet",                 "S_{#phi} lep and jets combined (N_{jets} #geq 2)",                          Sphi,   50,  0, PI);
    genSPhi_Zinc2jet              = newTH1D("genSPhi_Zinc2jet",              "gen S_{#phi} lep and jets combined (N_{jets} #geq 2)",                      Sphi,   50,  0, PI);
    BestSPhi_Zinc2jet             = newTH1D("BestSPhi_Zinc2jet",             "Best S_{#phi} lep and jets combined (N_{jets} #geq 2)",                     Sphi,   50,  0, PI);
    genBestSPhi_Zinc2jet          = newTH1D("genBestSPhi_Zinc2jet",          "gen Best S_{#phi} lep and jets combined (N_{jets} #geq 2)",                 Sphi,   50,  0, PI);

    //-- low Z pT
    TwoJetsPtDiff_LowPt_Zexc2jet  = newTH1D("TwoJetsPtDiff_LowPt_Zexc2jet",  "pT diff of the two highest jet at low Z_{pT} (N_{jets} = 2)",                  "#Delta pT(j_{1}j_{2}) [GeV]",      10,  0, 100);
    genTwoJetsPtDiff_LowPt_Zexc2jet = newTH1D("genTwoJetsPtDiff_LowPt_Zexc2jet", "gen pT diff of the two highest jet at low Z_{pT} (N_{jets} = 2)",          "#Delta pT(j_{1}j_{2}) [GeV]",      10,  0, 100);
    JetsMass_LowPt_Zexc2jet       = newTH1D("JetsMass_LowPt_Zexc2jet",       "2Jets Invariant Mass at low Z_{pT} (N_{jets} = 2)",                            Mjj, 24, 20, 620);
    genJetsMass_LowPt_Zexc2jet    = newTH1D("genJetsMass_LowPt_Zexc2jet",    "gen 2Jets Invariant Mass at low Z_{pT} (N_{jets} = 2)",                        Mjj, 24, 20, 620);
    ptBal_LowPt_Zexc2jet          = newTH1D("ptBal_LowPt_Zexc2jet",          "Vectorial pT sum: Z_{pT} + DiJet_{pT} at low Z_{pT} (N_{jets} = 2)",            "#Sigma pT [GeV]",      50, 0, 100);
    genptBal_LowPt_Zexc2jet       = newTH1D("genptBal_LowPt_Zexc2jet",       "gen Vectorial pT sum: Z_{pT} + DiJet_{pT} at low Z_{pT} (N_{jets} = 2)",        "#Sigma pT [GeV]",      50, 0, 100);
    dPhiJets_LowPt_Zexc2jet       = newTH1D("dPhiJets_LowPt_Zexc2jet",       "#Delta#phi btwn jets at low Z_{pT} (N_{jets} = 2)",                            jdPhi,           15, 0, PI);
    gendPhiJets_LowPt_Zexc2jet    = newTH1D("gendPhiJets_LowPt_Zexc2jet",    "gen #Delta#phi btwn jets at low Z_{pT} (N_{jets} = 2)",                        jdPhi,           15, 0, PI);
    dPhiLeptons_LowPt_Zexc2jet    = newTH1D("dPhiLeptons_LowPt_Zexc2jet",    "#Delta#phi btwn leptons at low Z_{pT} (N_{jets} = 2)",                         ldPhi,           50, 0, PI);
    gendPhiLeptons_LowPt_Zexc2jet = newTH1D("gendPhiLeptons_LowPt_Zexc2jet", "gen #Delta#phi btwn leptons at low Z_{pT} (N_{jets} = 2)",                     ldPhi,           50, 0, PI);
    PHI_LowPt_Zexc2jet            = newTH1D("PHI_LowPt_Zexc2jet",            "#phi: Angle btwn the two subsystems planes at low Z_{pT} (N_{jets} = 2)",      "#phi(j_{12}Z)", 25, 0, PI);
    genPHI_LowPt_Zexc2jet         = newTH1D("genPHI_LowPt_Zexc2jet",         "gen #phi: Angle btwn the two subsystems planes at low Z_{pT} (N_{jets} = 2)",  "#phi(j_{12}Z)", 25, 0, PI);
    PHI_T_LowPt_Zexc2jet          = newTH1D("PHI_T_LowPt_Zexc2jet",          "#Delta S Angle btwn lepton and jet pair in T-plane at low Z_{pT} (N_{jets} = 2)",    "#Delta S(j_{12}Z)",             10, 0, PI);
    genPHI_T_LowPt_Zexc2jet       = newTH1D("genPHI_T_LowPt_Zexc2jet",       "gen #Delta S Angle btwn lepton and jet pair in T-plane at low Z_{pT} (N_{jets} = 2)","#Delta S(j_{12}Z)",             10, 0, PI);
    SpT_LowPt_Zexc2jet            = newTH1D("SpT_LowPt_Zexc2jet",            "#Delta_{pT}^{rel} leptons and jets combined at low Z_{pT} (N_{jets} = 2)",     Spt,    25, 0, 1);
    genSpT_LowPt_Zexc2jet         = newTH1D("genSpT_LowPt_Zexc2jet",         "gen #Delta_{pT}^{rel} leptons and jets combined at low Z_{pT} (N_{jets} = 2)", Spt,    25, 0, 1);
    SpTJets_LowPt_Zexc2jet        = newTH1D("SpTJets_LowPt_Zexc2jet",        "#Delta_{pT}^{rel} jets at low Z_{pT} (N_{jets} = 2)",                    jSpt,   15, 0, 1);
    genSpTJets_LowPt_Zexc2jet     = newTH1D("genSpTJets_LowPt_Zexc2jet",     "gen #Delta_{pT}^{rel} jets at low Z_{pT} (N_{jets} = 2)",                jSpt,   15, 0, 1);
    SpTLeptons_LowPt_Zexc2jet     = newTH1D("SpTLeptons_LowPt_Zexc2jet",     "#Delta_{pT}^{rel} leptons at low Z_{pT} (N_{jets} = 2)",                 lSpt,   50, 0, 1);
    genSpTLeptons_LowPt_Zexc2jet  = newTH1D("genSpTLeptons_LowPt_Zexc2jet",  "gen #Delta_{pT}^{rel} leptons at low Z_{pT} (N_{jets} = 2)",             lSpt,   50, 0, 1);
    SPhi_LowPt_Zexc2jet           = newTH1D("SPhi_LowPt_Zexc2jet",           "S_{#phi}: leptons and jets combined at low Z_{pT} (N_{jets} = 2)",             Sphi,   50, 0, PI);
    genSPhi_LowPt_Zexc2jet        = newTH1D("genSPhi_LowPt_Zexc2jet",        "gen S_{#phi}: leptons and jets combined at low Z_{pT} (N_{jets} = 2)",         Sphi,   50, 0, PI);

    TwoJetsPtDiff_LowPt_Zinc2jet  = newTH1D("TwoJetsPtDiff_LowPt_Zinc2jet",  "pT diff of the two highest jet at low Z_{pT} (N_{jets} #geq 2)",                              "#Delta pT(j_{1}j_{2}) [GeV]",   10,  0, 100);
    genTwoJetsPtDiff_LowPt_Zinc2jet = newTH1D("genTwoJetsPtDiff_LowPt_Zinc2jet", "gen pT diff of the two highest jet at low Z_{pT}  (N_{jets} #geq 2)",                     "#Delta pT(j_{1}j_{2}) [GeV]",   10,  0, 100);
    BestTwoJetsPtDiff_LowPt_Zinc2jet = newTH1D("BestTwoJetsPtDiff_LowPt_Zinc2jet", "Best pT diff of the two highest jet at low Z_{pT} (N_{jets} #geq 2)",                   "#Delta pT(j_{1}j_{2}) [GeV]",   10,  0, 100);
    genBestTwoJetsPtDiff_LowPt_Zinc2jet = newTH1D("genBestTwoJetsPtDiff_LowPt_Zinc2jet", "gen Best pT diff of the two highest jet at low Z_{pT} (N_{jets} #geq 2)",         "#Delta pT(j_{1}j_{2}) [GeV]",   10,  0, 100);
    JetsMass_LowPt_Zinc2jet       = newTH1D("JetsMass_LowPt_Zinc2jet",       "2Jets Invariant Mass at low Z_{pT} (N_{jets} #geq 2)",                                        Mjj, 24, 20, 620);
    genJetsMass_LowPt_Zinc2jet    = newTH1D("genJetsMass_LowPt_Zinc2jet",    "gen 2Jets Invariant Mass at low Z_{pT} (N_{jets} #geq 2)",                                    Mjj, 24, 20, 620);
    BestJetsMass_LowPt_Zinc2jet   = newTH1D("BestJetsMass_LowPt_Zinc2jet",   "Best 2Jets Invariant Mass at low Z_{pT} (N_{jets} #geq 2)",                                   Mjj, 24, 20, 620);
    genBestJetsMass_LowPt_Zinc2jet = newTH1D("genBestJetsMass_LowPt_Zinc2jet", "gen Best 2Jets Invariant Mass at low Z_{pT} (N_{jets} #geq 2)",                             Mjj, 24, 20, 620);
    ptBal_LowPt_Zinc2jet          = newTH1D("ptBal_LowPt_Zinc2jet",          "Vectorial pT sum: Z_{pT} + DiJet_{pT} at low Z_{pT} (N_{jets} #geq 2)",                        "#Sigma pT [GeV]",      50, 0, 100);
    genptBal_LowPt_Zinc2jet       = newTH1D("genptBal_LowPt_Zinc2jet",       "gen Vectorial pT sum: Z_{pT} + DiJet_{pT} at low Z_{pT} (N_{jets} #geq 2)",                    "#Sigma pT [GeV]",      50, 0, 100);
    dPhiJets_LowPt_Zinc2jet       = newTH1D("dPhiJets_LowPt_Zinc2jet",       "#Delta#phi btwn jets at low Z_{pT} (N_{jets} #geq 2)",                                        jdPhi,           15, 0, PI);
    gendPhiJets_LowPt_Zinc2jet    = newTH1D("gendPhiJets_LowPt_Zinc2jet",    "gen#Delta#phi btwn jets at low Z_{pT} (N_{jets} #geq 2)",                                     jdPhi,           15, 0, PI);
    BestdPhiJets_LowPt_Zinc2jet   = newTH1D("BestdPhiJets_LowPt_Zinc2jet",   "Best #Delta#phi btwn jets at low Z_{pT} (N_{jets} #geq 2)",                                   jdPhi,           15, 0, PI);
    genBestdPhiJets_LowPt_Zinc2jet= newTH1D("genBestdPhiJets_LowPt_Zinc2jet","gen Best #Delta#phi btwn jets at low Z_{pT} (N_{jets} #geq 2)",                               jdPhi,           15, 0, PI);
    dPhiLeptons_LowPt_Zinc2jet    = newTH1D("dPhiLeptons_LowPt_Zinc2jet",    "#Delta #phi btwn leptons at low Z_{pT} (N_{jets} #geq 2)",                                    ldPhi,           50, 0, PI);
    gendPhiLeptons_LowPt_Zinc2jet = newTH1D("gendPhiLeptons_LowPt_Zinc2jet", "gen #Delta #phi btwn leptons at low Z_{pT} (N_{jets} #geq 2)",                                ldPhi,           50, 0, PI);
    PHI_LowPt_Zinc2jet            = newTH1D("PHI_LowPt_Zinc2jet",            "#phi: Angle btwn the two subsystems planes at low Z_{pT} (N_{jets} #geq 2)",                  "#phi(j_{12}Z)",        25, 0, PI);
    genPHI_LowPt_Zinc2jet         = newTH1D("genPHI_LowPt_Zinc2jet",         "gen #phi: Angle btwn the two subsystems planes at low Z_{pT} (N_{jets} #geq 2)",              "#phi(j_{12}Z)",        25, 0, PI);
    BestPHI_LowPt_Zinc2jet        = newTH1D("BestPHI_LowPt_Zinc2jet",        "Best #phi: Angle btwn the two subsystems planes at low Z_{pT} (N_{jets} #geq 2)",             "#phi(j_{12}Z)",        25, 0, PI);
    genBestPHI_LowPt_Zinc2jet     = newTH1D("genBestPHI_LowPt_Zinc2jet",     "gen Best #phi: Angle btwn the two subsystems planes at low Z_{pT} (N_{jets} #geq 2)",         "#phi(j_{12}Z)",        25, 0, PI);
    PHI_T_LowPt_Zinc2jet          = newTH1D("PHI_T_LowPt_Zinc2jet",          "#Delta S Angle btwn lepton and jet pair in T-plane at low Z_{pT} (N_{jets} #geq 2)",          "#Delta S(j_{12}Z)",    10, 0, PI);
    genPHI_T_LowPt_Zinc2jet       = newTH1D("genPHI_T_LowPt_Zinc2jet",       "gen #Delta S Angle btwn lepton and jet pair in T-plane at low Z_{pT} (N_{jets} #geq 2)",      "#Delta S(j_{12}Z)",    10, 0, PI);
    BestPHI_T_LowPt_Zinc2jet      = newTH1D("BestPHI_T_LowPt_Zinc2jet",      "Best #Delta S Angle btwn lepton and jet pair in T-plane at low Z_{pT} (N_{jets} #geq 2)",     "#Delta S(j_{12}Z)",    10, 0, PI);
    genBestPHI_T_LowPt_Zinc2jet   = newTH1D("genBestPHI_T_LowPt_Zinc2jet",   "gen Best #Delta S Angle btwn lepton and jet pair in T-plane at low Z_{pT} (N_{jets} #geq 2)", "#Delta S(j_{12}Z)",    10, 0, PI);
    SpT_LowPt_Zinc2jet            = newTH1D("SpT_LowPt_Zinc2jet",            "#Delta_{pT}^{rel} leptons and jets combined at low Z_{pT} (N_{jets} #geq 2)",                 Spt,    25, 0, 1);
    genSpT_LowPt_Zinc2jet         = newTH1D("genSpT_LowPt_Zinc2jet",         "gen #Delta_{pT}^{rel} leptons and jets combined at low Z_{pT} (N_{jets} #geq 2)",             Spt,    25, 0, 1);
    BestSpT_LowPt_Zinc2jet        = newTH1D("BestSpT_LowPt_Zinc2jet",        "Best #Delta_{pT}^{rel} leptons and jets combined at low Z_{pT} (N_{jets} #geq 2)",            Spt,    25, 0, 1);
    genBestSpT_LowPt_Zinc2jet     = newTH1D("genBestSpT_LowPt_Zinc2jet",     "gen Best #Delta_{pT}^{rel} leptons and jets combined at low Z_{pT} (N_{jets} #geq 2)",        Spt,    25, 0, 1);
    SpTJets_LowPt_Zinc2jet        = newTH1D("SpTJets_LowPt_Zinc2jet",        "#Delta_{pT}^{rel} jets at low Z_{pT} (N_{jets} #geq 2)",                                jSpt,   15, 0, 1);
    genSpTJets_LowPt_Zinc2jet     = newTH1D("genSpTJets_LowPt_Zinc2jet",     "gen #Delta_{pT}^{rel} jets at low Z_{pT} (N_{jets} #geq 2)",                            jSpt,   15, 0, 1);
    BestSpTJets_LowPt_Zinc2jet    = newTH1D("BestSpTJets_LowPt_Zinc2jet",    "Best #Delta_{pT}^{rel} jets at low Z_{pT} (N_{jets} #geq 2)",                           jSpt,   15, 0, 1);
    genBestSpTJets_LowPt_Zinc2jet = newTH1D("genBestSpTJets_LowPt_Zinc2jet", "gen Best #Delta_{pT}^{rel} jets at low Z_{pT} (N_{jets} #geq 2)",                       jSpt,   15, 0, 1);
    SpTLeptons_LowPt_Zinc2jet     = newTH1D("SpTLeptons_LowPt_Zinc2jet",     "#Delta_{pT}^{rel} leptons at low Z_{pT} (N_{jets} #geq 2)",                             lSpt,   50, 0, 1);
    genSpTLeptons_LowPt_Zinc2jet  = newTH1D("genSpTLeptons_LowPt_Zinc2jet",  "gen #Delta_{pT}^{rel} leptons at low Z_{pT} (N_{jets} #geq 2)",                         lSpt,   50, 0, 1);
    SPhi_LowPt_Zinc2jet           = newTH1D("SPhi_LowPt_Zinc2jet",           "S_{#phi}: leptons and jets combined at low Z_{pT} (N_{jets} #geq 2)",                         Sphi,   50, 0, PI);
    genSPhi_LowPt_Zinc2jet        = newTH1D("genSPhi_LowPt_Zinc2jet",        "gen S_{#phi}: leptons and jets combined at low Z_{pT} (N_{jets} #geq 2)",                     Sphi,   50, 0, PI);
    BestSPhi_LowPt_Zinc2jet       = newTH1D("BestSPhi_LowPt_Zinc2jet",       "Best S_{#phi}: leptons and jets combined at low Z_{pT} (N_{jets} #geq 2)",                    Sphi,   50, 0, PI);
    genBestSPhi_LowPt_Zinc2jet    = newTH1D("genBestSPhi_LowPt_Zinc2jet",    "gen Best S_{#phi}: leptons and jets combined at low Z_{pT} (N_{jets} #geq 2)",                Sphi,   50, 0, PI);

    //-- low Z pT and low SpT
    PHI_LowSpT_LowPt_Zexc2jet     = newTH1D("PHI_LowSpT_LowPt_Zexc2jet",     "#phi: Angle btwn the two subsystems planes at low #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} = 2)","#phi",     25,0.,PI);
    genPHI_LowSpT_LowPt_Zexc2jet  = newTH1D("genPHI_LowSpT_LowPt_Zexc2jet",  "gen #phi: Angle btwn the two subsystems planes at low #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} = 2)","#phi", 25,0.,PI);
    SPhi_LowSpT_LowPt_Zexc2jet    = newTH1D("SPhi_LowSpT_LowPt_Zexc2jet",    "S_{#phi}: leptons and jets combined at low #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} = 2)","S_{#phi}",           50,2.5,PI);
    genSPhi_LowSpT_LowPt_Zexc2jet = newTH1D("genSPhi_LowSpT_LowPt_Zexc2jet", "gen S_{#phi}: leptons and jets combined at low #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} = 2)","S_{#phi}",       50,2.5,PI);

    PHI_LowSpT_LowPt_Zinc2jet     = newTH1D("PHI_LowSpT_LowPt_Zinc2jet",     "#phi: Angle btwn the two subsystems planes at low #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} #geq 2)","#phi",     25,0.,PI);
    genPHI_LowSpT_LowPt_Zinc2jet  = newTH1D("genPHI_LowSpT_LowPt_Zinc2jet",  "gen #phi: Angle btwn the two subsystems planes at low #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} #geq 2)","#phi", 25,0.,PI);
    SPhi_LowSpT_LowPt_Zinc2jet    = newTH1D("SPhi_LowSpT_LowPt_Zinc2jet",    "S_{#phi}: leptons and jets combined at low #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} #geq 2)","S_{#phi}",           50,2.5,PI);
    genSPhi_LowSpT_LowPt_Zinc2jet = newTH1D("genSPhi_LowSpT_LowPt_Zinc2jet", "gen S_{#phi}: leptons and jets combined at low #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} #geq 2)","S_{#phi}",       50,2.5,PI);

    //-- low Z pT and high SpT
    PHI_HighSpT_LowPt_Zexc2jet    = newTH1D("PHI_HighSpT_LowPt_Zexc2jet",    "#phi: Angle btwn the two subsystems planes at high #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} = 2)","#phi",    50,0.,PI);
    genPHI_HighSpT_LowPt_Zexc2jet = newTH1D("genPHI_HighSpT_LowPt_Zexc2jet", "gen #phi: Angle btwn the two subsystems planes at high #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} = 2)","#phi",50,0.,PI);
    SPhi_HighSpT_LowPt_Zexc2jet   = newTH1D("SPhi_HighSpT_LowPt_Zexc2jet",   "S_{#phi}: leptons and jets combined at high #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} = 2)","S_{#phi}",          50,0.,PI);
    genSPhi_HighSpT_LowPt_Zexc2jet  = newTH1D("genSPhi_HighSpT_LowPt_Zexc2jet",   "gen S_{#phi}: leptons and jets combined at high #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} = 2)","S_{#phi}", 50,0.,PI);

    PHI_HighSpT_LowPt_Zinc2jet    = newTH1D("PHI_HighSpT_LowPt_Zinc2jet",    "#phi: Angle btwn the two subsystems planes at high #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} #geq 2)","#phi",    50,0.,PI);
    genPHI_HighSpT_LowPt_Zinc2jet = newTH1D("genPHI_HighSpT_LowPt_Zinc2jet", "gen #phi: Angle btwn the two subsystems planes at high #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} #geq 2)","#phi",50,0.,PI);
    SPhi_HighSpT_LowPt_Zinc2jet   = newTH1D("SPhi_HighSpT_LowPt_Zinc2jet",   "S_{#phi}: leptons and jets combined at high #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} #geq 2)","S_{#phi}",          50,0.,PI);
    genSPhi_HighSpT_LowPt_Zinc2jet  = newTH1D("genSPhi_HighSpT_LowPt_Zinc2jet",   "gen S_{#phi}: leptons and jets combined at high #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} #geq 2)","S_{#phi}", 50,0.,PI);

    //-- low Z pT and low SPhi
    SpT_LowSPhi_LowPt_Zexc2jet    = newTH1D("SpT_LowSPhi_LowPt_Zexc2jet",    "#Delta_{pT}^{rel} leptons and jets combined at low S_{#phi} and low Z_{pT} (N_{jets} = 2)","#Delta_{pT}^{rel}",             50,0.,1.);
    genSpT_LowSPhi_LowPt_Zexc2jet    = newTH1D("genSpT_LowSPhi_LowPt_Zexc2jet",    "gen #Delta_{pT}^{rel} leptons and jets combined at low S_{#phi} and low Z_{pT} (N_{jets} = 2)","#Delta_{pT}^{rel}",   50,0.,1.);

    SpT_LowSPhi_LowPt_Zinc2jet    = newTH1D("SpT_LowSPhi_LowPt_Zinc2jet",    "#Delta_{pT}^{rel} leptons and jets combined at low S_{#phi} and low Z_{pT} (N_{jets} #geq 2)","#Delta_{pT}^{rel}",             50,0.,1.);
    genSpT_LowSPhi_LowPt_Zinc2jet    = newTH1D("genSpT_LowSPhi_LowPt_Zinc2jet",    "gen #Delta_{pT}^{rel} leptons and jets combined at low S_{#phi} and low Z_{pT} (N_{jets} #geq 2)","#Delta_{pT}^{rel}",   50,0.,1.);

    //-- low Z pT and high SPhi
    SpT_HighSPhi_LowPt_Zexc2jet   = newTH1D("SpT_HighSPhi_LowPt_Zexc2jet",   "#Delta_{pT}^{rel} leptons and jets combined at high S_{#phi} and low Z_{pT} (N_{jets} = 2)","#Delta_{pT}^{rel}",            50,0.,1.);
    genSpT_HighSPhi_LowPt_Zexc2jet   = newTH1D("genSpT_HighSPhi_LowPt_Zexc2jet",   "gen #Delta_{pT}^{rel} leptons and jets combined at high S_{#phi} and low Z_{pT} (N_{jets} = 2)","#Delta_{pT}^{rel}",  50,0.,1.);

    SpT_HighSPhi_LowPt_Zinc2jet   = newTH1D("SpT_HighSPhi_LowPt_Zinc2jet",   "#Delta_{pT}^{rel} leptons and jets combined at high S_{#phi} and low Z_{pT} (N_{jets} #geq 2)","#Delta_{pT}^{rel}",            50,0.,1.);
    genSpT_HighSPhi_LowPt_Zinc2jet   = newTH1D("genSpT_HighSPhi_LowPt_Zinc2jet",   "gen #Delta_{pT}^{rel} leptons and jets combined at high S_{#phi} and low Z_{pT} (N_{jets} #geq 2)","#Delta_{pT}^{rel}",  50,0.,1.);

    //-- high Z pT
    ptBal_HighPt_Zexc2jet         = newTH1D("ptBal_HighPt_Zexc2jet",         "Vectorial pT sum: Z_{pT} + DiJet_{pT} at high Z_{pT} (N_{jets} = 2)","#Sigma pT [GeV]",                 50,0.,100.);
    genptBal_HighPt_Zexc2jet      = newTH1D("genptBal_HighPt_Zexc2jet",      "gen Vectorial pT sum: Z_{pT} + DiJet_{pT} at high Z_{pT} (N_{jets} = 2)","#Sigma pT [GeV]",             50,0.,100.);
    dPhiJets_HighPt_Zexc2jet      = newTH1D("dPhiJets_HighPt_Zexc2jet",      "#Delta#phi btwn jets at high Z_{pT} (N_{jets} = 2)",                                          jdPhi,            15, 0, PI);
    gendPhiJets_HighPt_Zexc2jet   = newTH1D("gendPhiJets_HighPt_Zexc2jet",   "gen #Delta#phi btwn jets at high Z_{pT} (N_{jets} = 2)",                                      jdPhi,            15, 0, PI);
    dPhiLeptons_HighPt_Zexc2jet   = newTH1D("dPhiLeptons_HighPt_Zexc2jet",   "#Delta#phi btwn leptons at high Z_{pT} (N_{jets} = 2)",                                       ldPhi,            50,0.,PI);
    gendPhiLeptons_HighPt_Zexc2jet = newTH1D("gendPhiLeptons_HighPt_Zexc2jet",   "gen #Delta#phi btwn leptons at high Z_{pT} (N_{jets} = 2)",                               ldPhi,            50,0.,PI);
    PHI_HighPt_Zexc2jet           = newTH1D("PHI_HighPt_Zexc2jet",           "#phi: Angle btwn the two subsystems planes at high Z_{pT} (N_{jets} = 2)","#phi",                   50,0.,PI);
    genPHI_HighPt_Zexc2jet        = newTH1D("genPHI_HighPt_Zexc2jet",        "gen #phi: Angle btwn the two subsystems planes at high Z_{pT} (N_{jets} = 2)","#phi",               50,0.,PI);
    PHI_T_HighPt_Zexc2jet         = newTH1D("PHI_T_HighPt_Zexc2jet",         "#Delta S Angle btwn lepton and jet pair in T-plane at high Z_{pT} (N_{jets} = 2)","#Delta S",                 10,0.,PI);
    genPHI_T_HighPt_Zexc2jet      = newTH1D("genPHI_T_HighPt_Zexc2jet",      "gen #Delta S Angle btwn lepton and jet pair in T-plane at high Z_{pT} (N_{jets} = 2)","#Delta S",             10,0.,PI);
    SpT_HighPt_Zexc2jet           = newTH1D("SpT_HighPt_Zexc2jet",           "#Delta_{pT}^{rel} leptons and jets combined at high Z_{pT} (N_{jets} = 2)","#Delta_{pT}^{rel}",                             50,0.,1.);
    genSpT_HighPt_Zexc2jet        = newTH1D("genSpT_HighPt_Zexc2jet",        "gen #Delta_{pT}^{rel} leptons and jets combined at high Z_{pT} (N_{jets} = 2)","#Delta_{pT}^{rel}",                         50,0.,1.);
    SpTJets_HighPt_Zexc2jet       = newTH1D("SpTJets_HighPt_Zexc2jet",       "#Delta_{pT}^{rel} jets at high Z_{pT} (N_{jets} = 2)","#Delta_{pT}^{rel}",                                            15,0.,1.);
    genSpTJets_HighPt_Zexc2jet    = newTH1D("genSpTJets_HighPt_Zexc2jet",    "gen #Delta_{pT}^{rel} jets at high Z_{pT} (N_{jets} = 2)","#Delta_{pT}^{rel}",                                        15,0.,1.);
    SpTLeptons_HighPt_Zexc2jet    = newTH1D("SpTLeptons_HighPt_Zexc2jet",    "#Delta_{pT}^{rel} leptons at high Z_{pT} (N_{jets} = 2)","#Delta_{pT}^{rel}",                                         50,0.,1.);
    genSpTLeptons_HighPt_Zexc2jet = newTH1D("genSpTLeptons_HighPt_Zexc2jet", "gen #Delta_{pT}^{rel} leptons at high Z_{pT} (N_{jets} = 2)","#Delta_{pT}^{rel}",                                     50,0.,1.);
    SPhi_HighPt_Zexc2jet          = newTH1D("SPhi_HighPt_Zexc2jet",          "S_{#phi}: leptons and jets combined at high Z_{pT} (N_{jets} = 2)","S_{#phi}",                         50,0.,PI);
    genSPhi_HighPt_Zexc2jet       = newTH1D("genSPhi_HighPt_Zexc2jet",       "gen S_{#phi}: leptons and jets combined at high Z_{pT} (N_{jets} = 2)","S_{#phi}",                     50,0.,PI);

    ptBal_HighPt_Zinc2jet         = newTH1D("ptBal_HighPt_Zinc2jet",         "Vectorial pT sum: Z_{pT} + DiJet_{pT} at high Z_{pT} (N_{jets} #geq 2)","#Sigma pT [GeV]",                 50,0.,100.);
    genptBal_HighPt_Zinc2jet      = newTH1D("genptBal_HighPt_Zinc2jet",      "gen Vectorial pT sum: Z_{pT} + DiJet_{pT} at high Z_{pT} (N_{jets} #geq 2)","#Sigma pT [GeV]",             50,0.,100.);
    dPhiJets_HighPt_Zinc2jet      = newTH1D("dPhiJets_HighPt_Zinc2jet",      "#Delta#phi btwn jets at high Z_{pT} (N_{jets} #geq 2)",                                       jdPhi,      15, 0, PI);
    gendPhiJets_HighPt_Zinc2jet   = newTH1D("gendPhiJets_HighPt_Zinc2jet",   "gen #Delta#phi btwn jets at high Z_{pT} (N_{jets} #geq 2)",                                   jdPhi,      15, 0, PI);
    dPhiLeptons_HighPt_Zinc2jet   = newTH1D("dPhiLeptons_HighPt_Zinc2jet",   "#Delta#phi btwn leptons at high Z_{pT (N_{jets} #geq 2)}",                                    ldPhi,      50,0.,PI);
    gendPhiLeptons_HighPt_Zinc2jet   = newTH1D("gendPhiLeptons_HighPt_Zinc2jet",   "gen #Delta#phi btwn leptons at high Z_{pT} (N_{jets} #geq 2)",                          ldPhi,      50,0.,PI);
    PHI_HighPt_Zinc2jet           = newTH1D("PHI_HighPt_Zinc2jet",           "#phi: Angle btwn the two subsystems planes at high Z_{pT} (N_{jets} #geq 2)","#phi",                   50,0.,PI);
    genPHI_HighPt_Zinc2jet        = newTH1D("genPHI_HighPt_Zinc2jet",        "gen #phi: Angle btwn the two subsystems planes at high Z_{pT} (N_{jets} #geq 2)","#phi",               50,0.,PI);
    PHI_T_HighPt_Zinc2jet         = newTH1D("PHI_T_HighPt_Zinc2jet",         "#Delta S Angle btwn lepton and jet pair in T-plane at high Z_{pT} (N_{jets} #geq 2)","#Delta S",                 10,0.,PI);
    genPHI_T_HighPt_Zinc2jet      = newTH1D("genPHI_T_HighPt_Zinc2jet",      "gen#Delta S Angle btwn lepton and jet pair in T-plane at high Z_{pT} (N_{jets} #geq 2)","#Delta S",              10,0.,PI);
    SpT_HighPt_Zinc2jet           = newTH1D("SpT_HighPt_Zinc2jet",           "#Delta_{pT}^{rel} leptons and jets combined at high Z_{pT} (N_{jets} #geq 2)","#Delta_{pT}^{rel}",                             50,0.,1.);
    genSpT_HighPt_Zinc2jet        = newTH1D("genSpT_HighPt_Zinc2jet",        "gen #Delta_{pT}^{rel} leptons and jets combined at high Z_{pT} (N_{jets} #geq 2)","#Delta_{pT}^{rel}",                         50,0.,1.);
    SpTJets_HighPt_Zinc2jet       = newTH1D("SpTJets_HighPt_Zinc2jet",       "#Delta_{pT}^{rel} jets at high Z_{pT} (N_{jets} #geq 2)","#Delta_{pT}^{rel}",                                            15,0.,1.);
    genSpTJets_HighPt_Zinc2jet    = newTH1D("genSpTJets_HighPt_Zinc2jet",    "gen #Delta_{pT}^{rel} jets at high Z_{pT} (N_{jets} #geq 2)","#Delta_{pT}^{rel}",                                        15,0.,1.);
    SpTLeptons_HighPt_Zinc2jet    = newTH1D("SpTLeptons_HighPt_Zinc2jet",    "#Delta_{pT}^{rel} leptons at high Z_{pT} (N_{jets} #geq 2)","#Delta_{pT}^{rel}",                                         50,0.,1.);
    genSpTLeptons_HighPt_Zinc2jet = newTH1D("genSpTLeptons_HighPt_Zinc2jet", "gen #Delta_{pT}^{rel} leptons at high Z_{pT} (N_{jets} #geq 2)","#Delta_{pT}^{rel}",                                     50,0.,1.);
    SPhi_HighPt_Zinc2jet          = newTH1D("SPhi_HighPt_Zinc2jet",          "S_{#phi}: leptons and jets combined at high Z_{pT} (N_{jets} #geq 2)","S_{#phi}",                         50,0.,PI);
    genSPhi_HighPt_Zinc2jet       = newTH1D("genSPhi_HighPt_Zinc2jet",       "gen S_{#phi}: leptons and jets combined at high Z_{pT} (N_{jets} #geq 2)","S_{#phi}",                     50,0.,PI);

    //-- high Z pT and low SpT
    PHI_LowSpT_HighPt_Zexc2jet    = newTH1D("PHI_LowSpT_HighPt_Zexc2jet",    "#phi: Angle btwn the two subsystems planes at low #Delta_{pT}^{rel} and high Z_{pT} (N_{jets} = 2)","#Phi",    50,0.,PI);
    SPhi_LowSpT_HighPt_Zexc2jet   = newTH1D("SPhi_LowSpT_HighPt_Zexc2jet",   "S_{#phi}: leptons and jets combined at low #Delta_{pT}^{rel} and high Z_{pT} (N_{jets} = 2)","S_{#phi}",          50,2.5,PI);

    PHI_LowSpT_HighPt_Zinc2jet    = newTH1D("PHI_LowSpT_HighPt_Zinc2jet",    "#phi: Angle btwn the two subsystems planes at low #Delta_{pT}^{rel} and high Z_{pT} (N_{jets} #geq 2)","#Phi",    50,0.,PI);
    SPhi_LowSpT_HighPt_Zinc2jet   = newTH1D("SPhi_LowSpT_HighPt_Zinc2jet",   "S_{#phi}: leptons and jets combined at low #Delta_{pT}^{rel} and high Z_{pT} (N_{jets} #geq 2)","S_{#phi}",          50,2.5,PI);

    //-- high Z pT and high SpT
    PHI_HighSpT_HighPt_Zexc2jet   = newTH1D("PHI_HighSpT_HighPt_Zexc2jet",   "#phi: Angle btwn the two subsystems planes at high #Delta_{pT}^{rel} and high Z_{pT} (N_{jets} = 2)","#phi",   50,0.,PI);
    SPhi_HighSpT_HighPt_Zexc2jet  = newTH1D("SPhiHighSpT_HighPt_Zexc2jet",   "S_{#phi}: leptons and jets combined at high #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} = 2)","S_{#phi}",          50,0.,PI);

    PHI_HighSpT_HighPt_Zinc2jet   = newTH1D("PHI_HighSpT_HighPt_Zinc2jet",   "#phi: Angle btwn the two subsystems planes at high #Delta_{pT}^{rel} and high Z_{pT} (N_{jets} #geq 2)","#phi",   50,0.,PI);
    SPhi_HighSpT_HighPt_Zinc2jet  = newTH1D("SPhiHighSpT_HighPt_Zinc2jet",   "S_{#phi}: leptons and jets combined at high #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} #geq 2)","S_{#phi}",          50,0.,PI);

    //-- high Z pT and low SPhi
    SpT_LowSPhi_HighPt_Zexc2jet   = newTH1D("SpT_LowSPhi_HighPt_Zexc2jet",   "#Delta_{pT}^{rel} leptons and jets combined at low S_{#phi} and high Z_{pT} (N_{jets} = 2)","#Delta_{pT}^{rel}",            50,0.,1.);

    SpT_LowSPhi_HighPt_Zinc2jet   = newTH1D("SpT_LowSPhi_HighPt_Zinc2jet",   "#Delta_{pT}^{rel} leptons and jets combined at low S_{#phi} and high Z_{pT} (N_{jets} #geq 2)","#Delta_{pT}^{rel}",            50,0.,1.);

    //-- high Z pT and high SPhi
    SpT_HighSPhi_HighPt_Zexc2jet  = newTH1D("SpT_HighSPhi_HighPt_Zexc2jet",  "#Delta_{pT}^{rel} leptons and jets combined at high S_{#phi} and high Z_{pT} (N_{jets} = 2)","#Delta_{pT}^{rel}",           50,0.,1.);

    SpT_HighSPhi_HighPt_Zinc2jet  = newTH1D("SpT_HighSPhi_HighPt_Zinc2jet",  "#Delta_{pT}^{rel} leptons and jets combined at high S_{#phi} and high Z_{pT} (N_{jets} #geq 2)","#Delta_{pT}^{rel}",           50,0.,1.);

    //-- low SPhi
    SpT_LowSPhi_Zexc2jet          = newTH1D("SpT_LowSPhi_Zexc2jet",          "#Delta_{pT}^{rel} leptons and jets combined at low S_{#phi} (N_{jets} = 2)","#Delta_{pT}^{rel}",                            50,0.,1.);

    SpT_LowSPhi_Zinc2jet          = newTH1D("SpT_LowSPhi_Zinc2jet",          "#Delta_{pT}^{rel} leptons and jets combined at low S_{#phi} (N_{jets} #geq 2)","#Delta_{pT}^{rel}",                            50,0.,1.);

    //-- high SPhi
    SpT_HighSPhi_Zexc2jet         = newTH1D("SpT_HighSPhi_Zexc2jet",         "#Delta_{pT}^{rel} leptons and jets combined at high S_{#phi} (N_{jets} = 2)","#Delta_{pT}^{rel}",                           50,0.,1.);

    SpT_HighSPhi_Zinc2jet         = newTH1D("SpT_HighSPhi_Zinc2jet",         "#Delta_{pT}^{rel} leptons and jets combined at high S_{#phi} (N_{jets} #geq 2)","#Delta_{pT}^{rel}",                           50,0.,1.);

    //-- low SpT
    PHI_LowSpT_Zexc2jet           = newTH1D("PHI_LowSpT_Zexc2jet",           "#phi: Angle btwn the two subsystems planes at low #Delta_{pT}^{rel} (N_{jets} = 2)","#Phi",                    50,0.,PI);
    SPhi_LowSpT_Zexc2jet          = newTH1D("SPhi_LowSpT_Zexc2jet",          "S_{#phi}: leptons and jets combined at low #Delta_{pT}^{rel} (N_{jets} = 2)","S_{#phi}",                          50,2.5,PI);

    PHI_LowSpT_Zinc2jet           = newTH1D("PHI_LowSpT_Zinc2jet",           "#phi: Angle btwn the two subsystems planes at low #Delta_{pT}^{rel} (N_{jets} #geq 2)","#Phi",                    50,0.,PI);
    SPhi_LowSpT_Zinc2jet          = newTH1D("SPhi_LowSpT_Zinc2jet",          "S_{#phi}: leptons and jets combined at low #Delta_{pT}^{rel} (N_{jets} #geq 2)","S_{#phi}",                          50,2.5,PI);

    //-- high SpT
    PHI_HighSpT_Zexc2jet          = newTH1D("PHI_HighSpT_Zexc2jet",          "#phi: Angle btwn the two subsystems planes at high #Delta_{pT}^{rel} (N_{jets} = 2)","#Phi",                   50,0.,PI);
    SPhi_HighSpT_Zexc2jet         = newTH1D("SPhi_HighSpT_Zexc2jet",         "S_{#phi}: leptons and jets combined at high #Delta_{pT}^{rel} (N_{jets} = 2)","S_{#phi}",                         50,0.,PI);

    PHI_HighSpT_Zinc2jet          = newTH1D("PHI_HighSpT_Zinc2jet",          "#phi: Angle btwn the two subsystems planes at high #Delta_{pT}^{rel} (N_{jets} #geq 2)","#Phi",                   50,0.,PI);
    SPhi_HighSpT_Zinc2jet         = newTH1D("SPhi_HighSpT_Zinc2jet",         "S_{#phi}: leptons and jets combined at high #Delta_{pT}^{rel} (N_{jets} #geq 2)","S_{#phi}",                         50,0.,PI);

    //-- gen stuff
    gendPhiJetsDeltaR_Zexc2jet    = newTH1D("gendPhiJetsDeltaR_Zexc2jet",    "#Delta #phi btwn gen jets with #Delta R < 0.5 (N_{jets} = 2)","#Delta#phi",                         50,0.,PI);
    resdPhiJetsDeltaR_Zexc2jet    = newTH1D("resdPhiJetsDeltaR_Zexc2jet",    "#Delta #phi btwn gen jets with #Delta R < 0.5 (N_{jets} = 2)","#Delta#phi",                         50,-2.5,2.5);
    genPHI_TDeltaR_Zexc2jet       = newTH1D("genPHI_TDeltaR_Zexc2jet",       "#Delta S Angle btwn gen lep and gen jet pair in T-plane with #Delta R < 0.5 (N_{jets} = 2)","#Delta S",    50,0.,PI);
    resPHI_TDeltaR_Zexc2jet       = newTH1D("resPHI_TDeltaR_Zexc2jet",       "#Delta S Angle btwn gen lep and gen jet pair in T-plane with #Delta R < 0.5 (N_{jets} = 2)","#Delta S",    50,-2.5,2.5);
    genSpTJetsDeltaR_Zexc2jet     = newTH1D("genSpTJetsDeltaR_Zexc2jet",     "#Delta_{pT}^{rel} Gen jets with #Delta R < 0.5 (N_{jets} = 2)","#Delta_{pT}^{rel}",                                   50,0.,1.);
    resSpTJetsDeltaR_Zexc2jet     = newTH1D("resSpTJetsDeltaR_Zexc2jet",     "#Delta_{pT}^{rel} Gen jets with #Delta R < 0.5 (N_{jets} = 2)","#Delta_{pT}^{rel}",                                   50,-2.5,2.5);
    genSpTDeltaR_Zexc2jet         = newTH1D("genSpTDeltaR_Zexc2jet",         "#Delta_{pT}^{rel} with #Delta R < 0.5 (N_{jets} = 2)","#Delta_{pT}^{rel}",                                                   50,0.,1.);
    resSpTDeltaR_Zexc2jet         = newTH1D("resSpTDeltaR_Zexc2jet",         "#Delta_{pT}^{rel} with #Delta R < 0.5 (N_{jets} = 2)","#Delta_{pT}^{rel}",                                                   50,-2.5,2.5);

    gendPhiJetsDPS_Zexc2jet       = newTH1D("gendPhiJetsDPS_Zexc2jet",       "#Delta #phi btwn gen jets matching DPS parton (N_{jets} = 2)","#Delta#phi_{j_{1}j_{2}}",            50,0.,PI);
    gendPhiJetsDPSDeltaR_Zexc2jet = newTH1D("gendPhiJetsDPSDeltaR_Zexc2jet", "#Delta #phi btwn gen jets matching DPS parton with #Delta R < 0.5 (N_{jets} = 2)","#Delta#phi",     50,0.,PI);
    genPHI_TDPS_Zexc2jet          = newTH1D("genPHI_TDPS_Zexc2jet",          "#Delta S Angle btwn gen lepton and jet pair in T-plane (N_{jets} = 2)","#Delta S",                         50,0.,PI);
    genPHI_TDPSDeltaR_Zexc2jet    = newTH1D("genPHI_TDPSDeltaR_Zexc2jet",    "#Delta S Angle btwn gen lepton and jet pair in T-plane with #Delta R < 0.5 (N_{jets} = 2)","#Delta S",     50,0.,PI);
    genSpTJetsDPS_Zexc2jet        = newTH1D("genSpTJetsDPS_Zexc2jet",        "#Delta_{pT}^{rel} Gen jets matching DPS parton (N_{jets} = 2)","#Delta_{pT}^{rel}",nbinSpt,binSpt);
    genSpTJetsDPSDeltaR_Zexc2jet  = newTH1D("genSpTJetsDPSDeltaR_Zexc2jet",  "#Delta_{pT}^{rel} Gen jets matching DPS parton with #Delta R < 0.5 (N_{jets} = 2)","#Delta_{pT}^{rel}",nbinSpt,binSpt);
    genSpTDPS_Zexc2jet            = newTH1D("genSpTDPS_Zexc2jet",            "#Delta_{pT}^{rel} with gen jets matching DPS parton (N_{jets} = 2)","#Delta_{pT}^{rel}",nbinSpt,binSpt);
    genSpTDPSDeltaR_Zexc2jet      = newTH1D("genSpTDPSDeltaR_Zexc2jet",      "#Delta_{pT}^{rel} with gen jets matching DPS parton with #Delta R < 0.5 (N_{jets} = 2)","#Delta_{pT}^{rel}",nbinSpt,binSpt);
    genSpTDPSPartons_Zexc2jet     = newTH1D("genSpTDPSPartons_Zexc2jet",     "#Delta_{pT}^{rel} DPS partons (N_{jets} = 2)","#Delta_{pT}^{rel}",nbinSpt,binSpt);

    
    //Correlations

    gendPhiJetsDPSDeltaR_ZpT_Zexc2jet = newTH2D("gendPhiJetsDPSDeltaR_ZpT_Zexc2jet", "gendPhiJetsDPSDeltaR_ZpT_Zexc2jet", 50, 0, PI, 100, 0, 100);
    partonX2D                          = newTH2D("partonX2D","parton X: x1 vs x2",100,0.0001,0.2,100,0.0001,0.2);

    gendeltaRjetMu                     = newTH1D("gendeltaRjetMu", "gen delta R btwn jet and muon", "#R", 50, 0., 2.5);

    /// additional information
    // Muoisolation

    MuDetIsoRhoCorr            = newTH1D("MuDetIsoRhoCorr",  "Muon Detect. Iso #rho corr.",     "l_{Iso}^{Det.}", 30, 0, 1.5);
    MuPFIsoDBetaCorr           = newTH1D("MuPFIsoDBetaCorr", "Muon PF Iso DBeta corr.",         "l_{Iso}^{PF}",   30, 0, 1.5);
    
    MuPFIso_Zinc0jet           = newTH1D("MuPFIso_Zinc0jet",      "Muon PF Iso DBeta corr. Sig",     "l_{Iso}^{PF}",   30, 0, 1.5);
    MuPFIso_2ndZinc0jet        = newTH1D("MuPFIso_2ndZinc0jet",   "Muon PF Iso DBeta corr. Sig2",    "l_{Iso}^{PF}",   150, 0, 1.5);
    MuPFIso_3rdZinc0jet        = newTH1D("MuPFIso_3rdZinc0jet",   "Muon PF Iso DBeta corr. Sig3",    "l_{Iso}^{PF}",   200, 0, 2.0);
    
    deltaRjetMu                = newTH1D("deltaRjetMu", "delta R btwn jet and muon", "#R", 50, 0., 2.5);
    deltaPtjetMu               = newTH1D("deltaPtjetMu", "delta Pt btwn jet and muon if dR<0.5", "#R", 150, -75., 75.);

    //TH2D* jecVspt=newTH1D("jecVspt","jec Vs pt","jec","pt",80,0.,400,100,0,0.5);
    NVtx                          = newTH1D("NVtx","Number of vertices","#Vtx",45,0.5,45.5);
    ZNGoodJetsBeta_Zexc = newTH2D("ZNGoodJetsBeta_Zexc","Beta cut vs Jet Counter (excl.) ", 11, -0.5, 10.5, 10, -0.5, 9.5);
    ZNGoodJetsBeta_Zexc->GetXaxis()->SetBinLabel(1, "= 0");
    ZNGoodJetsBeta_Zexc->GetXaxis()->SetBinLabel(2, "= 1");
    ZNGoodJetsBeta_Zexc->GetXaxis()->SetBinLabel(3, "= 2");
    ZNGoodJetsBeta_Zexc->GetXaxis()->SetBinLabel(4, "= 3");
    ZNGoodJetsBeta_Zexc->GetXaxis()->SetBinLabel(5, "= 4");
    ZNGoodJetsBeta_Zexc->GetXaxis()->SetBinLabel(6, "= 5");
    ZNGoodJetsBeta_Zexc->GetXaxis()->SetBinLabel(7, "= 6");
    ZNGoodJetsBeta_Zexc->GetXaxis()->SetBinLabel(8, "= 7");
    ZNGoodJetsBeta_Zexc->GetXaxis()->SetBinLabel(9, "= 8");
    ZNGoodJetsBeta_Zexc->GetXaxis()->SetBinLabel(10,"= 9");
    ZNGoodJetsBeta_Zexc->GetXaxis()->SetBinLabel(11,"= 10");


    Beta                          = newTH1D("Beta","Jet PU variable Beta","Beta",50,0.,1.);
    BetaStar                      = newTH1D("BetaStar","Jet PU variable BetaStar","BetaStar",50,0.,1.);
    puBeta_JetsMatchGenJets       = newTH1D("puBeta_JetsMatchGenJets", "puBeta_JetsMatchGenJets", "Beta", 50, 0, 1);
    puBetaStar_JetsMatchGenJets   = newTH1D("puBetaStar_JetsMatchGenJets", "puBetaStar_JetsMatchGenJets", "Beta", 50, 0, 1);
    puBeta_JetsNoMatchGenJets     = newTH1D("puBeta_JetsNoMatchGenJets", "puBeta_JetsNoMatchGenJets", "Beta", 50, 0, 1);
    puBetaStar_JetsNoMatchGenJets = newTH1D("puBetaStar_JetsNoMatchGenJets", "puBetaStar_JetsNoMatchGenJets", "Beta", 50, 0, 1);
    puMVA                         = newTH1D("puMVA","Jet PU variable from MVA","puMVA",40,-1.,1.);
    puMVA_JetsMatchGenJets        = newTH1D("puMVA_JetsMatchGenJets","Jet PU variable from MVA for matching jets","puMVA",40,-1.,1.);
    puMVA_JetsNoMatchGenJets      = newTH1D("puMVA_JetsNoMatchGenJets","Jet PU variable from MVA for non matching jets","puMVA",40,-1.,1.);
    jetsEta_JetsMatchGenJets      = newTH1D("jetsEta_JetsMatchGenJets","Jet Eta for matching jets","puMVA",48,-2.4,2.4);
    jetsEta_JetsNoMatchGenJets    = newTH1D("jetsEta_JetsNoMatchGenJets","Jet Eta for non matching jets","puMVA",48,-2.4,2.4);
    FirstJetdEtaGenReco_Zinc1	= newTH1D("FirstJetdEtaGenReco_Zinc1","#delta#eta(gen,reco) for 1st leading jet","#delta#eta",300,0.,7.5);
    FourthJetdEtaGenReco_Zinc4	= newTH1D("FourthJetdEtaGenReco_Zinc4","#delta#eta(gen,reco) for 4th leading jet","#delta#eta",300,0.,7.5);
    puMVAvsBeta                   = newTH2D("puMVA vs beta","Jet PU variable from MVA vs Beta",50,-1.,1.,50,0.,1.);


    PUWeight   = newTH1D("PUWeight","PU weight Z all","PU weight Z all",500,0.,14.);
    PUWeight0  = newTH1D("PUWeight0","PU weight Z+0jet","PU weight Z+0jet",500,0.,14.);
    PUWeight1  = newTH1D("PUWeigh1","PU weight Z+jet>0 ","PU weight Z+jet>0",500,0.,14.);
    MuPlusPt   = newTH1D("MuPlusPt","Pt of positive muon","pT [GeV]",150,10.,160.);
    MuMinusPt  = newTH1D("MuMinusPt","Pt of negative muon","pT [GeV]",150,10.,160.);
    MuPlusEta  = newTH1D("MuPlusEta","#eta of positive muon","#eta",250,-2.5,2.5);
    MuMinusEta = newTH1D("MuMinusEta","#eta of negative muon","#eta",250,-2.5,2.5);

    /// additional MET histograms
    fullMET                       = newTH1D("MET","MET for all passing leptons","MET for all passing leptons",200,0.,400.);
    fullMET_pfMETPFlow            = newTH1D("fullMET_pfMETPFlow","fullMET_pfMETPFlow            for all passing leptons","MET for all passing leptons",200,0.,400.);
    fullMET_pfMet                 = newTH1D("fullMET_pfMet","fullMET_pfMet                 for all passing leptons","MET for all passing leptons",200,0.,400.);
    fullMET_pfType1CorrectedMet   = newTH1D("fullMET_pfType1CorrectedMet","fullMET_pfType1CorrectedMet   for all passing leptons","MET for all passing leptons",200,0.,400.);
    fullMET_pfType1p2CorrectedMet = newTH1D("fullMET_pfType1p2CorrectedMet","fullMET_pfType1p2CorrectedMet for all passing leptons","MET for all passing leptons",200,0.,400.);
    fullMT          = newTH1D("MT" ,"MT for all passing leptons" ,"MT for all passing leptons",200,0.,400.);
    METvslepIso     = newTH2D("MET vs lep Iso" ,"MET vs leptons Iso for all passing lepton",100,0.,300.,100, 0., 1 );
    MTvslepIso      = newTH2D("MT vs lep Iso" ,"MT vs leptons Iso for all passing lepton",100,0.,300.,100, 0., 1 );
    
    MET_Zinc0jet                      = newTH1D("MET_Zinc0jet",                      "MET (N_{jets} #geq 0)",  "MET [GeV]",      200,0.,400 );
    MET_Zinc1jet                      = newTH1D("MET_Zinc1jet",                      "MET (N_{jets} #geq 1)",  "MET [GeV]",      200,0.,400 );
    MET_Zinc2jet                      = newTH1D("MET_Zinc2jet",                      "MET (N_{jets} #geq 2)",  "MET [GeV]",      200,0.,400 );
    MET_Zinc3jet                      = newTH1D("MET_Zinc3jet",                      "MET (N_{jets} #geq 3)",  "MET [GeV]",      200,0.,400 );

    METphi_Zinc0jet                      = newTH1D("METphi_Zinc0jet",                      "MET #phi (N_{jets} #geq 0)",  "#phi(MET)",      100,-PI ,PI );
    METphi_Zinc1jet                      = newTH1D("METphi_Zinc1jet",                      "MET #phi (N_{jets} #geq 1)",  "#phi(MET)",      100,-PI ,PI );
    METphi_Zinc2jet                      = newTH1D("METphi_Zinc2jet",                      "MET #phi (N_{jets} #geq 2)",  "#phi(MET)",      100,-PI ,PI );
    METphi_Zinc3jet                      = newTH1D("METphi_Zinc3jet",                      "MET #phi (N_{jets} #geq 3)",  "#phi(MET)",      100,-PI ,PI );

    MT_Zinc0jet                      = newTH1D("MT_Zinc0jet",                      "MT (N_{jets} #geq 0)",    "MT [GeV]",    200,0.,400 );
    MT_Zinc1jet                      = newTH1D("MT_Zinc1jet",                      "MT (N_{jets} #geq 1)",    "MT [GeV]",    200,0.,400 );
    MT_Zinc2jet                      = newTH1D("MT_Zinc2jet",                      "MT (N_{jets} #geq 2)",    "MT [GeV]",    200,0.,400 );
    MT_Zinc3jet                      = newTH1D("MT_Zinc3jet",                      "MT (N_{jets} #geq 3)",    "MT [GeV]",    200,0.,400 );

    ZNGoodJetsEWK_Zexc = newTH1D("ZNGoodJetsEWK_Zexc","Jet Counter (excl.)", "N_{jets}", 11, -0.5, 10.5);
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(1, "= 0");
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(2, "= 1");
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(3, "= 2");
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(4, "= 3");
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(5, "= 4");
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(6, "= 5");
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(7, "= 6");
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(8, "= 7");
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(9, "= 8");
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(10,"= 9");
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(11,"= 10");
    ZNGoodJetsEWKbtw_Zexc = newTH1D("ZNGoodJetsEWKbtw_Zexc","Between: Jet Counter (excl.)", "N_{jets}", 11, -0.5, 10.5);
    ZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(1, "= 0");
    ZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(2, "= 1");
    ZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(3, "= 2");
    ZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(4, "= 3");
    ZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(5, "= 4");
    ZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(6, "= 5");
    ZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(7, "= 6");
    ZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(8, "= 7");
    ZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(9, "= 8");
    ZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(10,"= 9");
    ZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(11,"= 10");

    // MET properties
    METEWK_Zinc2jet                      = newTH1D("METEWK_Zinc2jet",                      "MET (N_{jets} #geq 2)",  "MET [GeV]",      200,0.,500 );
    METEWK_Zinc3jet                      = newTH1D("METEWK_Zinc3jet",                      "MET (N_{jets} #geq 3)",  "MET [GeV]",      200,0.,500 );

    METphiEWK_Zinc2jet                      = newTH1D("METphiEWK_Zinc2jet",                      "MET #phi (N_{jets} #geq 2)",  "#phi(MET)",      100,-PI ,PI );
    METphiEWK_Zinc3jet                      = newTH1D("METphiEWK_Zinc3jet",                      "MET #phi (N_{jets} #geq 3)",  "#phi(MET)",      100,-PI ,PI );

    MTEWK_Zinc2jet                      = newTH1D("MTEWK_Zinc2jet",                      "MT (N_{jets} #geq 2)",    "MT [GeV]",    200,0.,400 );
    MTEWK_Zinc3jet                      = newTH1D("MTEWK_Zinc3jet",                      "MT (N_{jets} #geq 3)",    "MT [GeV]",    200,0.,400 );



    /// jet properties
    int nJetPtEWK_Zinc2jet(15);
    double jetPtEWK_Zinc2jet[16] = {15, 20, 24, 30, 38, 50, 68, 88, 113, 144, 184, 234, 297, 377, 480, 700 };
    int nJetPtEWKbtw_Zinc2jet(12);
    double jetPtEWKbtw_Zinc2jet[13] = {15, 20, 24, 30, 38, 50, 68, 88, 113, 144, 184, 234, 297 };
    int nJetHTEWK_Zinc2jet(15);
    double jetHTEWK_Zinc2jet[16] = {60, 80, 95, 121, 155, 200, 260, 336, 436, 566, 737, 962, 1200, 1500, 2000, 2500};

    FirstJetPtEWK_Zinc2jet                 = newTH1D("FirstJetPtEWK_Zinc2jet",                 "1st jet p_{T} (N_{jets} #geq 2)",             "p_{T}(j_{1}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);
    SecondJetPtEWK_Zinc2jet                = newTH1D("SecondJetPtEWK_Zinc2jet",                "2nd jet p_{T} (N_{jets} #geq 2)",             "p_{T}(j_{2}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);

    FirstJetEtaEWK_Zinc2jet                = newTH1D("FirstJetEtaEWK_Zinc2jet",                "1st jet #eta (N_{jets} #geq 2)",              "#eta(j_{1})",  24,-4.8, 4.8);
    SecondJetEtaEWK_Zinc2jet               = newTH1D("SecondJetEtaEWK_Zinc2jet",               "2nd jet #eta (N_{jets} #geq 2)",              "#eta(j_{2})",  24,-4.8, 4.8);
    FirstJetPhiEWK_Zinc2jet                = newTH1D("FirstJetPhiEWK_Zinc2jet",                "1st jet #phi (N_{jets} #geq 2)",              "#phi(j_{1})",  30,-PI, PI);
    SecondJetPhiEWK_Zinc2jet               = newTH1D("SecondJetPhiEWK_Zinc2jet",               "2nd jet #phi (N_{jets} #geq 2)",              "#phi(j_{2})",  30,-PI, PI);

    ForwardJetPtEWK_Zinc2jet                 = newTH1D("ForwardJetPtEWK_Zinc2jet",                 "Fwd jet p_{T} (N_{jets} #geq 2)",             "p_{T}(j_{1}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);
    ForwardJetEtaEWK_Zinc2jet                = newTH1D("ForwardJetEtaEWK_Zinc2jet",                "Fwd jet #eta (N_{jets} #geq 2)",              "#eta(j_{1})",  24,-4.8, 4.8);
    ForwardJetPhiEWK_Zinc2jet                = newTH1D("ForwardJetPhiEWK_Zinc2jet",                "Fwd jet #phi (N_{jets} #geq 2)",              "#phi(j_{1})",  30,-PI, PI);

    CentralJetPtEWK_Zinc2jet                 = newTH1D("CentralJetPtEWK_Zinc2jet",                 "Cnt jet p_{T} (N_{jets} #geq 2)",             "p_{T}(j_{1}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);
    CentralJetEtaEWK_Zinc2jet                = newTH1D("CentralJetEtaEWK_Zinc2jet",                "Cnt jet #eta (N_{jets} #geq 2)",              "#eta(j_{1})",  24,-4.8, 4.8);
    CentralJetPhiEWK_Zinc2jet                = newTH1D("CentralJetPhiEWK_Zinc2jet",                "Cnt jet #phi (N_{jets} #geq 2)",              "#phi(j_{1})",  30,-PI, PI);



    ThirdJetPtEWK_Zinc2jet                = newTH1D("ThirdJetPtEWK_Zinc2jet",                "Ext 3nd jet p_{T} (N_{jets} #geq 2)",             "p_{T}(j_{3}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);
    ThirdJetPhiEWK_Zinc2jet                = newTH1D("ThirdJetPhiEWK_Zinc2jet",              "Ext 3nd jet #Phi (N_{jets} #geq 2)",             "#Phi(j_{3}) ",     30, -PI, PI);
    ThirdJetEtaEWK_Zinc2jet               = newTH1D("ThirdJetEtaEWK_Zinc2jet",               "Ext 3nd jet #eta (N_{jets} #geq 2)",              "#eta(j_{3})",  24,-4.8, 4.8);
    /// additional activity > 15 GeV full space
    ThirdJetPtEWKadd_Zinc2jet                = newTH1D("ThirdJetPtEWKadd_Zinc2jet",                "Ext pt#ge 15 3nd jet p_{T} (N_{jets} #geq 2)",             "p_{T}(j_{3}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);
    ThirdJetPhiEWKadd_Zinc2jet                = newTH1D("ThirdJetPhiEWKadd_Zinc2jet",                "Ext 3nd pt#ge 15 jet p_{T} (N_{jets} #geq 2)",             "#Phi(j_{3}) ",     30, -PI, PI);
    ThirdJetEtaEWKadd_Zinc2jet               = newTH1D("ThirdJetEtaEWKadd_Zinc2jet",               "Ext 3nd pt#ge 15 jet #eta (N_{jets} #geq 2)",              "#eta(j_{3})",  24,-4.8, 4.8);


    /////  dijet properties
    string jSEta = "#eta(j_{1})+ #eta(j_{2})";
    SumEtaJetsEWK_Zinc2jet             = newTH1D("SumEtaJetsEWK_Zinc2jet",             "|#Sigma| #eta of j1,j2 jets (N_{jets} #geq 2)",                                       jSEta,           50, 0, 10);
    AbsSumEtaJetsEWK_Zinc2jet             = newTH1D("AbsSumEtaJetsEWK_Zinc2jet",       "#Sigma |#eta| of j1,j2 jets (N_{jets} #geq 2)",                                       jSEta,           50, 0, 10 );
    SumEtaJetsEWK_Zinc3jet             = newTH1D("SumEtaJetsEWK_Zinc3jet",             "sum of #eta of j1,j2 jets (N_{jets} #geq 3)",                                       jSEta,           50, -8, 8);


    dEtaJetsEWK_Zinc2jet             = newTH1D("dEtaJetsEWK_Zinc2jet",             "#Delta#eta btwn jets (N_{jets} #geq 2)",                                       jdEta,           50, 0, 10);

    TwoJetsPtDiffEWK_Zinc2jet        = newTH1D("TwoJetsPtDiffEWK_Zinc2jet",        "pT diff of the two highest jet (N_{jets} #geq 2)",                             "#Delta pT(j_{1}j_{2}) [GeV]",      50,  0, 500);


    ptBalEWK_Zinc2jet                = newTH1D("ptBalEWK_Zinc2jet",                "Vectorial pT sum: Z_{pT} + DiJet_{pT} (N_{jets} #geq 2)",                       "#Sigma pT [GeV]",      50,  0, 500);
    SpTJetsEWK_Zinc2jet              = newTH1D("SpTJetsEWK_Zinc2jet",              "#Delta_{pT}^{rel} jets (N_{jets} #geq 2)",                                jSpt,   50,  0, 1);

    dPhiJetsEWK_Zinc2jet             = newTH1D("dPhiJetsEWK_Zinc2jet",             "#Delta#phi btwn jets (N_{jets} #geq 2)",                                       jdPhi,           100,  0, PI);


    JetsHTEWK_Zinc2jet                     = newTH1D("JetsHTEWK_Zinc2jet",                     "Scalar sum jets p_{T} (N_{jets} #geq 2)",     HT,     nJetHTEWK_Zinc2jet, jetHTEWK_Zinc2jet);


    JetsMassEWK_Zinc2jet             = newTH1D("JetsMassEWK_Zinc2jet",             "2Jets Invariant Mass (N_{jets} #geq 2)",                                       Mjj, 50, 20, 2620);


    //// third jet properties
    string  j3Eta = "#eta(j_{3})";
    EtaThirdJetsEWK_Zinc3jet             = newTH1D("EtaThirdJetsEWK_Zinc3jet",             "Third jet #eta in dijet frame (N_{jets} #geq 3)",                                       j3Eta,           50, -5, 5);

    /// all jets between leading two jets 
    AllJetPtEWKbtw_Zinc2jet                = newTH1D("AllJetPtEWKbtw_Zinc2jet",                "Ext,btw All jet p_{T} (N_{jets} #geq 2)",             "p_{T}(j_{3}) [GeV]",     nJetPtEWKbtw_Zinc2jet, jetPtEWKbtw_Zinc2jet);
    AllJetPhiEWKbtw_Zinc2jet               = newTH1D("AllJetPhiEWKbtw_Zinc2jet",               "Ext,btw All jet #phi (N_{jets} #geq 2)",              "#phi(j_{3})",  30,-PI, PI);
    AllJetEtaEWKbtw_Zinc2jet               = newTH1D("AllJetEtaEWKbtw_Zinc2jet",               "Ext,btw All jet #eta (N_{jets} #geq 2)",              "#eta(j_{3})",  24,-4.8, 4.8);
    double jetHTEWKbtw_Zinc2jet[16] = {0,15, 20, 30, 42, 60, 80, 95, 121, 155, 200, 260, 336, 436, 566, 737};
    JetsHTEWKbtw_Zinc2jet                  = newTH1D("JetsHTEWKbtw_Zinc2jet",                  "Ext,btw Scalar sum jets p_{T} (N_{jets} #geq 2)",     HT,     nJetHTEWK_Zinc2jet, jetHTEWKbtw_Zinc2jet);
    //TH1D *JetsHTEWKbtw_Zinc2jet                     = newTH1D("JetsHTEWKbtw_Zinc2jet",                     "Ext,btw Scalar sum jets p_{T} (N_{jets} #geq 2)",     HT,     nJetHTEWK_Zinc2jet, jetHTEWK_Zinc2jet);

    // third jet between
    ThirdJetPtEWKbtw_Zinc2jet                = newTH1D("ThirdJetPtEWKbtw_Zinc2jet",                "Ext,btw 3nd jet p_{T} (N_{jets} #geq 2)",             "p_{T}(j_{3}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);
    ThirdJetPhiEWKbtw_Zinc2jet               = newTH1D("ThirdJetPhiEWKbtw_Zinc2jet",               "Ext,btw 3nd jet #phi (N_{jets} #geq 2)",              "#phi(j_{3})",  30,-PI, PI);
    ThirdJetEtaEWKbtw_Zinc2jet               = newTH1D("ThirdJetEtaEWKbtw_Zinc2jet",               "Ext,btw 3nd jet #eta (N_{jets} #geq 2)",              "#eta(j_{3})",  24,-4.8, 4.8);


    genZNGoodJetsEWK_Zexc = newTH1D("genZNGoodJetsEWK_Zexc","Jet Counter (excl.)", "N_{jets}", 11, -0.5, 10.5);
    genZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(1, "= 0");
    genZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(2, "= 1");
    genZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(3, "= 2");
    genZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(4, "= 3");
    genZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(5, "= 4");
    genZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(6, "= 5");
    genZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(7, "= 6");
    genZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(8, "= 7");
    genZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(9, "= 8");
    genZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(10,"= 9");
    genZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(11,"= 10");
    genZNGoodJetsEWKbtw_Zexc = newTH1D("genZNGoodJetsEWKbtw_Zexc","Between: Jet Counter (excl.)", "N_{jets}", 11, -0.5, 10.5);
    genZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(1, "= 0");
    genZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(2, "= 1");
    genZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(3, "= 2");
    genZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(4, "= 3");
    genZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(5, "= 4");
    genZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(6, "= 5");
    genZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(7, "= 6");
    genZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(8, "= 7");
    genZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(9, "= 8");
    genZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(10,"= 9");
    genZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(11,"= 10");


    genFirstJetPtEWK_Zinc2jet                 = newTH1D("genFirstJetPtEWK_Zinc2jet",                 "Gen:1st jet p_{T} (N_{jets} #geq 2)",             "Gen:p_{T}(j_{1}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);
    genSecondJetPtEWK_Zinc2jet                = newTH1D("genSecondJetPtEWK_Zinc2jet",                "Gen:2nd jet p_{T} (N_{jets} #geq 2)",             "Gen:p_{T}(j_{2}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);

    genFirstJetEtaEWK_Zinc2jet                = newTH1D("genFirstJetEtaEWK_Zinc2jet",                "Gen:1st jet #eta (N_{jets} #geq 2)",              "Gen:#eta(j_{1})",  24,-4.8, 4.8);
    genSecondJetEtaEWK_Zinc2jet               = newTH1D("genSecondJetEtaEWK_Zinc2jet",               "Gen:2nd jet #eta (N_{jets} #geq 2)",              "Gen:#eta(j_{2})",  24,-4.8, 4.8);
    genFirstJetPhiEWK_Zinc2jet                = newTH1D("genFirstJetPhiEWK_Zinc2jet",                "Gen:1st jet #phi (N_{jets} #geq 2)",              "Gen:#phi(j_{1})",  30,-PI, PI);
    genSecondJetPhiEWK_Zinc2jet               = newTH1D("genSecondJetPhiEWK_Zinc2jet",               "Gen:2nd jet #phi (N_{jets} #geq 2)",              "Gen:#phi(j_{2})",  30,-PI, PI);

    genForwardJetPtEWK_Zinc2jet                 = newTH1D("genForwardJetPtEWK_Zinc2jet",                 "Gen:Fwd jet p_{T} (N_{jets} #geq 2)",             "Gen:p_{T}(j_{1}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);
    genForwardJetEtaEWK_Zinc2jet                = newTH1D("genForwardJetEtaEWK_Zinc2jet",                "Gen:Fwd jet #eta (N_{jets} #geq 2)",              "Gen:#eta(j_{1})",  24,-4.8, 4.8);
    genForwardJetPhiEWK_Zinc2jet                = newTH1D("genForwardJetPhiEWK_Zinc2jet",                "Gen:Fwd jet #phi (N_{jets} #geq 2)",              "Gen:#phi(j_{1})",  30,-PI, PI);

    genCentralJetPtEWK_Zinc2jet                 = newTH1D("genCentralJetPtEWK_Zinc2jet",                 "Gen:Cnt jet p_{T} (N_{jets} #geq 2)",             "Gen:p_{T}(j_{1}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);
    genCentralJetEtaEWK_Zinc2jet                = newTH1D("genCentralJetEtaEWK_Zinc2jet",                "Gen:Cnt jet #eta (N_{jets} #geq 2)",              "Gen:#eta(j_{1})",  24,-4.8, 4.8);
    genCentralJetPhiEWK_Zinc2jet                = newTH1D("genCentralJetPhiEWK_Zinc2jet",                "Gen:Cnt jet #phi (N_{jets} #geq 2)",              "Gen:#phi(j_{1})",  30,-PI, PI);





    genThirdJetPtEWK_Zinc2jet                = newTH1D("genThirdJetPtEWK_Zinc2jet",                "Gen:Ext 3nd jet p_{T} (N_{jets} #geq 2)",             "Gen:p_{T}(j_{3}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);
    genThirdJetPhiEWK_Zinc2jet                = newTH1D("genThirdJetPhiEWK_Zinc2jet",              "Gen:Ext 3nd jet #Phi (N_{jets} #geq 2)",             "Gen:#Phi(j_{3}) ",     30, -PI, PI);
    genThirdJetEtaEWK_Zinc2jet               = newTH1D("genThirdJetEtaEWK_Zinc2jet",               "Gen:Ext 3nd jet #eta (N_{jets} #geq 2)",              "Gen:#eta(j_{3})",  24,-4.8, 4.8);
    /// additional activity > 15 GeV full space
    genThirdJetPtEWKadd_Zinc2jet                = newTH1D("genThirdJetPtEWKadd_Zinc2jet",                "Gen:Ext pt#ge 15 3nd jet p_{T} (N_{jets} #geq 2)",             "Gen:p_{T}(j_{3}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);
    genThirdJetPhiEWKadd_Zinc2jet                = newTH1D("genThirdJetPhiEWKadd_Zinc2jet",                "Gen:Ext 3nd pt#ge 15 jet p_{T} (N_{jets} #geq 2)",             "Gen:#Phi(j_{3}) ",     30, -PI, PI);
    genThirdJetEtaEWKadd_Zinc2jet               = newTH1D("genThirdJetEtaEWKadd_Zinc2jet",               "Gen:Ext 3nd pt#ge 15 jet #eta (N_{jets} #geq 2)",              "Gen:#eta(j_{3})",  24,-4.8, 4.8);


    /////  dijet properties
    //string jSEta = "#eta(j_{1})+ #eta(j_{2})";
    genSumEtaJetsEWK_Zinc2jet             = newTH1D("genSumEtaJetsEWK_Zinc2jet",             "Gen:|#Sigma| #eta of j1,j2 jets (N_{jets} #geq 2)",                                       jSEta,           50, 0, 10);
    genAbsSumEtaJetsEWK_Zinc2jet             = newTH1D("genAbsSumEtaJetsEWK_Zinc2jet",       "Gen:#Sigma |#eta| of j1,j2 jets (N_{jets} #geq 2)",                                       jSEta,           50, 0, 10 );
    genSumEtaJetsEWK_Zinc3jet             = newTH1D("genSumEtaJetsEWK_Zinc3jet",             "Gen:sum of #eta of j1,j2 jets (N_{jets} #geq 3)",                                       jSEta,           50, -8, 8);


    gendEtaJetsEWK_Zinc2jet             = newTH1D("gendEtaJetsEWK_Zinc2jet",             "Gen:#Delta#eta btwn jets (N_{jets} #geq 2)",                                       jdEta,           50, 0, 10);

    genTwoJetsPtDiffEWK_Zinc2jet        = newTH1D("genTwoJetsPtDiffEWK_Zinc2jet",        "Gen:pT diff of the two highest jet (N_{jets} #geq 2)",                             "Gen:#Delta pT(j_{1}j_{2}) [GeV]",      50,  0, 500);


    genptBalEWK_Zinc2jet                = newTH1D("genptBalEWK_Zinc2jet",                "Gen:Vectorial pT sum: Z_{pT} + DiJet_{pT} (N_{jets} #geq 2)",                       "Gen:#Sigma pT [GeV]",      50,  0, 500);
    genSpTJetsEWK_Zinc2jet              = newTH1D("genSpTJetsEWK_Zinc2jet",              "Gen:#Delta_{pT}^{rel} jets (N_{jets} #geq 2)",                                jSpt,   50,  0, 1);

    gendPhiJetsEWK_Zinc2jet             = newTH1D("gendPhiJetsEWK_Zinc2jet",             "Gen:#Delta#phi btwn jets (N_{jets} #geq 2)",                                       jdPhi,           100,  0, PI);


    genJetsHTEWK_Zinc2jet                     = newTH1D("genJetsHTEWK_Zinc2jet",                     "Gen:Scalar sum jets p_{T} (N_{jets} #geq 2)",     HT,     nJetHTEWK_Zinc2jet, jetHTEWK_Zinc2jet);


    genJetsMassEWK_Zinc2jet             = newTH1D("genJetsMassEWK_Zinc2jet",             "Gen:2Jets Invariant Mass (N_{jets} #geq 2)",                                       Mjj, 50, 20, 2620);


    //// third jet properties
    //string  j3Eta = "#eta(j_{3})";
    genEtaThirdJetsEWK_Zinc3jet             = newTH1D("genEtaThirdJetsEWK_Zinc3jet",             "Gen:Third jet #eta in dijet frame (N_{jets} #geq 3)",                                       j3Eta,           50, -5, 5);

    /// all jets between leading two jets 
    genAllJetPtEWKbtw_Zinc2jet                = newTH1D("genAllJetPtEWKbtw_Zinc2jet",                "Gen:Ext,btw All jet p_{T} (N_{jets} #geq 2)",             "Gen:p_{T}(j_{3}) [GeV]",     nJetPtEWKbtw_Zinc2jet, jetPtEWKbtw_Zinc2jet);
    genAllJetPhiEWKbtw_Zinc2jet               = newTH1D("genAllJetPhiEWKbtw_Zinc2jet",               "Gen:Ext,btw All jet #phi (N_{jets} #geq 2)",              "Gen:#phi(j_{3})",  30,-PI, PI);
    genAllJetEtaEWKbtw_Zinc2jet               = newTH1D("genAllJetEtaEWKbtw_Zinc2jet",               "Gen:Ext,btw All jet #eta (N_{jets} #geq 2)",              "Gen:#eta(j_{3})",  24,-4.8, 4.8);


    //double jetHTEWKbtw_Zinc2jet[16] = {0,15, 20, 30, 42, 60, 80, 95, 121, 155, 200, 260, 336, 436, 566, 737};
    genJetsHTEWKbtw_Zinc2jet                  = newTH1D("genJetsHTEWKbtw_Zinc2jet",                  "Gen:Ext,btw Scalar sum jets p_{T} (N_{jets} #geq 2)",     HT,     nJetHTEWK_Zinc2jet, jetHTEWKbtw_Zinc2jet);
    //TH1D *genJetsHTEWKbtw_Zinc2jet                     = newTH1D("genJetsHTEWKbtw_Zinc2jet",                     "Gen:Ext,btw Scalar sum jets p_{T} (N_{jets} #geq 2)",     HT,     nJetHTEWK_Zinc2jet, jetHTEWK_Zinc2jet);

    // third jet between
    genThirdJetPtEWKbtw_Zinc2jet                = newTH1D("genThirdJetPtEWKbtw_Zinc2jet",                "Gen:Ext,btw 3nd jet p_{T} (N_{jets} #geq 2)",             "Gen:p_{T}(j_{3}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);
    genThirdJetPhiEWKbtw_Zinc2jet               = newTH1D("genThirdJetPhiEWKbtw_Zinc2jet",               "Gen:Ext,btw 3nd jet #phi (N_{jets} #geq 2)",              "Gen:#phi(j_{3})",  30,-PI, PI);
    genThirdJetEtaEWKbtw_Zinc2jet               = newTH1D("genThirdJetEtaEWKbtw_Zinc2jet",               "Gen:Ext,btw 3nd jet #eta (N_{jets} #geq 2)",              "Gen:#eta(j_{3})",  24,-4.8, 4.8);



    //// at least one forward jet
    ZNGoodJetsEWKfwd_Zexc = newTH1D("ZNGoodJetsEWKfwd_Zexc"," 1 Fwd jet: Jet Counter (excl.)", "N_{jets}", 11, -0.5, 10.5);
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(1, "= 0");
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(2, "= 1");
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(3, "= 2");
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(4, "= 3");
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(5, "= 4");
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(6, "= 5");
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(7, "= 6");
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(8, "= 7");
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(9, "= 8");
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(10,"= 9");
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(11,"= 10");

    // met properties
    METEWKfwd_Zinc2jet                      = newTH1D("METEWKfwd_Zinc2jet",                   " 1 Fwd jet: MET (N_{jets} #geq 2)",  "MET [GeV]",      200,0.,500 );
    METEWKfwd_Zinc3jet                      = newTH1D("METEWKfwd_Zinc3jet",                   " 1 Fwd jet: MET (N_{jets} #geq 3)",  "MET [GeV]",      200,0.,500 );

    METphiEWKfwd_Zinc2jet                      = newTH1D("METphiEWKfwd_Zinc2jet",                   " 1 Fwd jet: MET #phi (N_{jets} #geq 2)",  "#phi(MET)",      100,-PI ,PI );
    METphiEWKfwd_Zinc3jet                      = newTH1D("METphiEWKfwd_Zinc3jet",                   " 1 Fwd jet: MET #phi (N_{jets} #geq 3)",  "#phi(MET)",      100,-PI ,PI );

    MTEWKfwd_Zinc2jet                      = newTH1D("MTEWKfwd_Zinc2jet",                   " 1 Fwd jet: MT (N_{jets} #geq 2)",    "MT [GeV]",    200,0.,400 );
    MTEWKfwd_Zinc3jet                      = newTH1D("MTEWKfwd_Zinc3jet",                   " 1 Fwd jet: MT (N_{jets} #geq 3)",    "MT [GeV]",    200,0.,400 );

    // jet hisotgrams
    FirstJetPtEWKfwd_Zinc2jet                 = newTH1D("FirstJetPtEWKfwd_Zinc2jet",              " 1 Fwd jet: 1st jet p_{T} (N_{jets} #geq 2)",             "p_{T}(j_{1}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);
    SecondJetPtEWKfwd_Zinc2jet                = newTH1D("SecondJetPtEWKfwd_Zinc2jet",             " 1 Fwd jet: 2nd jet p_{T} (N_{jets} #geq 2)",             "p_{T}(j_{2}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);

    FirstJetEtaEWKfwd_Zinc2jet                = newTH1D("FirstJetEtaEWKfwd_Zinc2jet",             " 1 Fwd jet: 1st jet #eta (N_{jets} #geq 2)",              "#eta(j_{1})",  24,-4.8, 4.8);
    SecondJetEtaEWKfwd_Zinc2jet               = newTH1D("SecondJetEtaEWKfwd_Zinc2jet",            " 1 Fwd jet: 2nd jet #eta (N_{jets} #geq 2)",              "#eta(j_{2})",  24,-4.8, 4.8);

    FirstJetPhiEWKfwd_Zinc2jet                = newTH1D("FirstJetPhiEWKfwd_Zinc2jet",             " 1 Fwd jet: 1st jet #phi (N_{jets} #geq 2)",              "#phi(j_{1})",  30,-PI, PI);
    SecondJetPhiEWKfwd_Zinc2jet               = newTH1D("SecondJetPhiEWKfwd_Zinc2jet",            " 1 Fwd jet: 2nd jet #phi (N_{jets} #geq 2)",              "#phi(j_{2})",  30,-PI, PI);


    SumEtaJetsEWKfwd_Zinc2jet             = newTH1D("SumEtaJetsEWKfwd_Zinc2jet",          " 1 Fwd jet: sum of #eta of j1,j2 jets (N_{jets} #geq 2)",                                       jSEta,           50, -8, 8);
    SumEtaJetsEWKfwd_Zinc3jet             = newTH1D("SumEtaJetsEWKfwd_Zinc3jet",          " 1 Fwd jet: sum of #eta of j1,j2 jets (N_{jets} #geq 3)",                                       jSEta,           50, -8, 8);
    genSumEtaJetsEWKfwd_Zinc2jet          = newTH1D("genSumEtaJetsEWKfwd_Zinc2jet",       " 1 Fwd jet: gen sum of #eta of j1,j2 jets (N_{jets} #geq 2)",                                   jSEta,           50, -8, 8);


    dEtaJetsEWKfwd_Zinc2jet             = newTH1D("dEtaJetsEWKfwd_Zinc2jet",          " 1 Fwd jet: #Delta#eta btwn jets (N_{jets} #geq 2)",                                       jdEta,           50,  0, 10);
    gendEtaJetsEWKfwd_Zinc2jet          = newTH1D("gendEtaJetsEWKfwd_Zinc2jet",       " 1 Fwd jet: gen #Delta#eta btwn jets (N_{jets} #geq 2)",                                   jdEta,           50,  0, 10);

    TwoJetsPtDiffEWKfwd_Zinc2jet        = newTH1D("TwoJetsPtDiffEWKfwd_Zinc2jet",     " 1 Fwd jet: pT diff of the two highest jet (N_{jets} #geq 2)",                             "#Delta pT(j_{1}j_{2}) [GeV]",      50,  0, 500);
    genTwoJetsPtDiffEWKfwd_Zinc2jet     = newTH1D("genTwoJetsPtDiffEWKfwd_Zinc2jet",  " 1 Fwd jet: gen pT diff of the two highest jet (N_{jets} #geq 2)",                         "#Delta pT(j_{1}j_{2}) [GeV]",      50,  0, 1000);


    ptBalEWKfwd_Zinc2jet                = newTH1D("ptBalEWKfwd_Zinc2jet",             " 1 Fwd jet: Vectorial pT sum: Z_{pT} + Jets_{pT} (N_{jets} #geq 2)",                       "#Sigma pT [GeV]",      50,  0, 500);
    SpTJetsEWKfwd_Zinc2jet              = newTH1D("SpTJetsEWKfwd_Zinc2jet",           " 1 Fwd jet: #Delta_{pT}^{rel} jets (N_{jets} #geq 2)",                                jSpt,   50,  0, 1);
    genSpTJetsEWKfwd_Zinc2jet           = newTH1D("genSpTJetsEWKfwd_Zinc2jet",        " 1 Fwd jet: gen #Delta_{pT}^{rel} jets (N_{jets} #geq 2)",                            jSpt,   50,  0, 1);

    dPhiJetsEWKfwd_Zinc2jet             = newTH1D("dPhiJetsEWKfwd_Zinc2jet",          " 1 Fwd jet: #Delta#phi btwn jets (N_{jets} #geq 2)",                                       jdPhi,           100,  0, PI);
    gendPhiJetsEWKfwd_Zinc2jet          = newTH1D("gendPhiJetsEWKfwd_Zinc2jet",       " 1 Fwd jet: gen #Delta#phi btwn jets (N_{jets} #geq 2)",                                   jdPhi,           100,  0, PI);


    JetsHTEWKfwd_Zinc2jet                     = newTH1D("JetsHTEWKfwd_Zinc2jet",                  " 1 Fwd jet: Scalar sum jets p_{T} (N_{jets} #geq 2)",     HT,     nJetHTEWK_Zinc2jet, jetHTEWK_Zinc2jet);
    genJetsHTEWKfwd_Zinc2jet                  = newTH1D("genJetsHTEWKfwd_Zinc2jet",               " 1 Fwd jet: Scalar sum jets p_{T} (N_{jets} #geq 2)",     HT,     nJetHTEWK_Zinc2jet, jetHTEWK_Zinc2jet);


    JetsMassEWKfwd_Zinc2jet             = newTH1D("JetsMassEWKfwd_Zinc2jet",          " 1 Fwd jet: 2Jets Invariant Mass (N_{jets} #geq 2)",                                       Mjj, 50, 20, 2620);
    genJetsMassEWKfwd_Zinc2jet          = newTH1D("genJetsMassEWKfwd_Zinc2jet",       " 1 Fwd jet: gen 2Jets Invariant Mass (N_{jets} #geq 2)",                                   Mjj, 50, 20, 2620);



    //// at least one forward jet
    ZNGoodJetsEWKmjj_Zexc = newTH1D("ZNGoodJetsEWKmjj_Zexc","M_{jj} #geq 1 TeV: Jet Counter (excl.)", "N_{jets}", 11, -0.5, 10.5);
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(1, "= 0");
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(2, "= 1");
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(3, "= 2");
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(4, "= 3");
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(5, "= 4");
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(6, "= 5");
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(7, "= 6");
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(8, "= 7");
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(9, "= 8");
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(10,"= 9");
    ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(11,"= 10");

    // met properties
    METEWKmjj_Zinc2jet                      = newTH1D("METEWKmjj_Zinc2jet",                   "M_{jj} #geq 1 TeV: MET (N_{jets} #geq 2)",  "MET [GeV]",      200,0.,500 );
    METEWKmjj_Zinc3jet                      = newTH1D("METEWKmjj_Zinc3jet",                   "M_{jj} #geq 1 TeV: MET (N_{jets} #geq 3)",  "MET [GeV]",      200,0.,500 );

    METphiEWKmjj_Zinc2jet                      = newTH1D("METphiEWKmjj_Zinc2jet",                   "M_{jj} #geq 1 TeV: MET #phi (N_{jets} #geq 2)",  "#phi(MET)",      100,-PI ,PI );
    ///TH1D *METphiEWKmjj_Zinc3jet                      = newTH1D("METphiEWKmjj_Zinc3jet",                   "M_{jj} #geq 1 TeV: MET #phi (N_{jets} #geq 3)",  "#phi(MET)",      100,-PI ,PI );

    MTEWKmjj_Zinc2jet                      = newTH1D("MTEWKmjj_Zinc2jet",                   "M_{jj} #geq 1 TeV: MT (N_{jets} #geq 2)",    "MT [GeV]",    200,0.,400 );
    //TH1D *MTEWKmjj_Zinc3jet                      = newTH1D("MTEWKmjj_Zinc3jet",                   "M_{jj} #geq 1 TeV: MT (N_{jets} #geq 3)",    "MT [GeV]",    200,0.,400 );

    // jet hisotgrams
    FirstJetPtEWKmjj_Zinc2jet                 = newTH1D("FirstJetPtEWKmjj_Zinc2jet",              "M_{jj} #geq 1 TeV: 1st jet p_{T} (N_{jets} #geq 2)",             "p_{T}(j_{1}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);
    SecondJetPtEWKmjj_Zinc2jet                = newTH1D("SecondJetPtEWKmjj_Zinc2jet",             "M_{jj} #geq 1 TeV: 2nd jet p_{T} (N_{jets} #geq 2)",             "p_{T}(j_{2}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);
    ThirdJetPtEWKmjj_Zinc3jet                 = newTH1D("ThirdJetPtEWKmjj_Zinc3jet",              "M_{jj} #geq 1 TeV: 3rd jet p_{T} (N_{jets} #geq 3)",             "p_{T}(j_{3}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);

    FirstJetEtaEWKmjj_Zinc2jet                = newTH1D("FirstJetEtaEWKmjj_Zinc2jet",             "M_{jj} #geq 1 TeV: 1st jet #eta (N_{jets} #geq 2)",              "#eta(j_{1})",  24,-4.8, 4.8);
    SecondJetEtaEWKmjj_Zinc2jet               = newTH1D("SecondJetEtaEWKmjj_Zinc2jet",            "M_{jj} #geq 1 TeV: 2nd jet #eta (N_{jets} #geq 2)",              "#eta(j_{2})",  24,-4.8, 4.8);

    FirstJetPhiEWKmjj_Zinc2jet                = newTH1D("FirstJetPhiEWKmjj_Zinc2jet",             "M_{jj} #geq 1 TeV: 1st jet #phi (N_{jets} #geq 2)",              "#phi(j_{1})",  30,-PI, PI);
    SecondJetPhiEWKmjj_Zinc2jet               = newTH1D("SecondJetPhiEWKmjj_Zinc2jet",            "M_{jj} #geq 1 TeV: 2nd jet #phi (N_{jets} #geq 2)",              "#phi(j_{2})",  30,-PI, PI);


    SumEtaJetsEWKmjj_Zinc2jet             = newTH1D("SumEtaJetsEWKmjj_Zinc2jet",          "M_{jj} #geq 1 TeV: sum of #eta of j1,j2 jets (N_{jets} #geq 2)",                                       jSEta,           50, -8, 8);
    SumEtaJetsEWKmjj_Zinc3jet             = newTH1D("SumEtaJetsEWKmjj_Zinc3jet",          "M_{jj} #geq 1 TeV: sum of #eta of j1,j2 jets (N_{jets} #geq 3)",                                       jSEta,           50, -8, 8);
    genSumEtaJetsEWKmjj_Zinc2jet          = newTH1D("genSumEtaJetsEWKmjj_Zinc2jet",       "M_{jj} #geq 1 TeV: gen sum of #eta of j1,j2 jets (N_{jets} #geq 2)",                                   jSEta,           50, -8, 8);


    dEtaJetsEWKmjj_Zinc2jet             = newTH1D("dEtaJetsEWKmjj_Zinc2jet",          "M_{jj} #geq 1 TeV: #Delta#eta btwn jets (N_{jets} #geq 2)",                                       jdEta,           50, 0, 10);

    gendEtaJetsEWKmjj_Zinc2jet          = newTH1D("gendEtaJetsEWKmjj_Zinc2jet",       "M_{jj} #geq 1 TeV: gen #Delta#eta btwn jets (N_{jets} #geq 2)",                                   jdEta,           50, 0, 10);

    TwoJetsPtDiffEWKmjj_Zinc2jet        = newTH1D("TwoJetsPtDiffEWKmjj_Zinc2jet",     "M_{jj} #geq 1 TeV: pT diff of the two highest jet (N_{jets} #geq 2)",                             "#Delta pT(j_{1}j_{2}) [GeV]",      50,  0, 500);
    genTwoJetsPtDiffEWKmjj_Zinc2jet     = newTH1D("genTwoJetsPtDiffEWKmjj_Zinc2jet",  "M_{jj} #geq 1 TeV: gen pT diff of the two highest jet (N_{jets} #geq 2)",                         "#Delta pT(j_{1}j_{2}) [GeV]",      50,  0, 1000);


    ptBalEWKmjj_Zinc2jet                = newTH1D("ptBalEWKmjj_Zinc2jet",             "M_{jj} #geq 1 TeV: Vectorial pT sum: Z_{pT} + Jets_{pT} (N_{jets} #geq 2)",                       "#Sigma pT [GeV]",      50,  0, 500);
    SpTJetsEWKmjj_Zinc2jet              = newTH1D("SpTJetsEWKmjj_Zinc2jet",           "M_{jj} #geq 1 TeV: #Delta_{pT}^{rel} jets (N_{jets} #geq 2)",                                jSpt,   50,  0, 1);
    genSpTJetsEWKmjj_Zinc2jet           = newTH1D("genSpTJetsEWKmjj_Zinc2jet",        "M_{jj} #geq 1 TeV: gen #Delta_{pT}^{rel} jets (N_{jets} #geq 2)",                            jSpt,   50,  0, 1);

    dPhiJetsEWKmjj_Zinc2jet             = newTH1D("dPhiJetsEWKmjj_Zinc2jet",          "M_{jj} #geq 1 TeV: #Delta#phi btwn jets (N_{jets} #geq 2)",                                       jdPhi,           100,  0, PI);
    gendPhiJetsEWKmjj_Zinc2jet          = newTH1D("gendPhiJetsEWKmjj_Zinc2jet",       "M_{jj} #geq 1 TeV: gen #Delta#phi btwn jets (N_{jets} #geq 2)",                                   jdPhi,           100,  0, PI);


    JetsHTEWKmjj_Zinc2jet                     = newTH1D("JetsHTEWKmjj_Zinc2jet",                  "M_{jj} #geq 1 TeV: Scalar sum jets p_{T} (N_{jets} #geq 2)",     HT,     nJetHTEWK_Zinc2jet, jetHTEWK_Zinc2jet);
    genJetsHTEWKmjj_Zinc2jet                  = newTH1D("genJetsHTEWKmjj_Zinc2jet",               "M_{jj} #geq 1 TeV: Scalar sum jets p_{T} (N_{jets} #geq 2)",     HT,     nJetHTEWK_Zinc2jet, jetHTEWK_Zinc2jet);

    JetsHTEWKmjjAdd_Zinc2jet                     = newTH1D("JetsHTEWKmjjAdd_Zinc2jet",                  "M_{jj} #geq 1 TeV: Scalar sum jets p_{T} , 15 GeV cut on third)",     HT,     nJetPtEWKbtw_Zinc2jet, jetHTEWKbtw_Zinc2jet);

    JetsMassEWKmjj_Zinc2jet             = newTH1D("JetsMassEWKmjj_Zinc2jet",          "M_{jj} #geq 1 TeV: 2Jets Invariant Mass (N_{jets} #geq 2)",                                       Mjj, 50, 1000, 2620);
    genJetsMassEWKmjj_Zinc2jet          = newTH1D("genJetsMassEWKmjj_Zinc2jet",       "M_{jj} #geq 1 TeV: gen 2Jets Invariant Mass (N_{jets} #geq 2)",                                   Mjj, 50, 1000, 2620);

    //// third jet properties
    ThirdJetEtaEWKmjj_Zinc3jet          = newTH1D("ThirdJetEtaEWKmjj_Zinc3jet",             "M_{jj} #geq 1 TeV: Third jet #eta in dijet frame (N_{jets} #geq 3)",                                       j3Eta,           50, -5, 5);


    partonsN          = newTH1D("partonsN","Number of ME partons ", "N_{partons}", 16, -0.5, 15.5);
    partonsNWeighted  = newTH1D("partonsNWeighted","Number of ME partons: weighted ", "N_{partons}", 16, -0.5, 15.5);
    partonsNAfterGenCut         = newTH1D("partonsNAfterGenCut","Number of ME partons passing the gen cut", "N_{partons}", 16, -0.5, 15.5);
    partonsNAfterGenCutWeighted = newTH1D("partonsNAfterGenCutWeighted","Number of ME partons passing the gen cut:weighted", "N_{partons}", 16, -0.5, 15.5);

    // vector boson and single jet
    dEtaBosonJet_Zexc1jet             = newTH1D("dEtaBosonJet_Zexc1",             "#Delta#eta btwn leading jet and V (N_{jets} #eq )) ",                                                   lJetdEta,           72, 0, 4.8);
    dEtaBosonJet_Zinc1jet             = newTH1D("dEtaBosonJet_Zinc1",             "#Delta#eta btwn leading jet and V (N_{jets} #geq )) ",                                                   lJetdEta,           72, 0, 4.8);
    gendEtaBosonJet_Zexc1jet             = newTH1D("gendEtaBosonJet_Zexc1",             "gen #Delta#eta btwn leading jet and V (N_{jets} #eq )) ",                                                   lJetdEta,           72, 0, 4.8);
    gendEtaBosonJet_Zinc1jet             = newTH1D("gendEtaBosonJet_Zinc1",             "gen #Delta#eta btwn leading jet and V (N_{jets} #geq )) ",                                                   lJetdEta,           72, 0, 4.8);





    //--- RooUnfoldResponses ---
    
    responseZMass                       = newResp(ZMass_Zinc0jet,                    genZMass_Zinc0jet);
    responseTwoJetsPtDiffExc            = newResp(TwoJetsPtDiff_Zexc2jet,            genTwoJetsPtDiff_Zexc2jet);
    responseTwoJetsPtDiffInc            = newResp(TwoJetsPtDiff_Zinc2jet,            genTwoJetsPtDiff_Zinc2jet);
    responseBestTwoJetsPtDiffInc        = newResp(BestTwoJetsPtDiff_Zinc2jet,        genBestTwoJetsPtDiff_Zinc2jet);
    responseTwoJetsPtDiffLowPtExc       = newResp(TwoJetsPtDiff_LowPt_Zexc2jet,      genTwoJetsPtDiff_LowPt_Zexc2jet);
    responseTwoJetsPtDiffLowPtInc       = newResp(TwoJetsPtDiff_LowPt_Zinc2jet,      genTwoJetsPtDiff_LowPt_Zinc2jet);
    responseBestTwoJetsPtDiffLowPtInc   = newResp(BestTwoJetsPtDiff_LowPt_Zinc2jet,  genBestTwoJetsPtDiff_LowPt_Zinc2jet);
    responseJetsMassExc                 = newResp(JetsMass_Zexc2jet,                 genJetsMass_Zexc2jet);
    responseJetsMassInc                 = newResp(JetsMass_Zinc2jet,                 genJetsMass_Zinc2jet);
    responseBestJetsMassInc             = newResp(BestJetsMass_Zinc2jet,             genBestJetsMass_Zinc2jet);
    responseJetsMassLowPtExc            = newResp(JetsMass_LowPt_Zexc2jet,           genJetsMass_LowPt_Zexc2jet);
    responseJetsMassLowPtInc            = newResp(JetsMass_LowPt_Zinc2jet,           genJetsMass_LowPt_Zinc2jet);
    responseBestJetsMassLowPtInc        = newResp(BestJetsMass_LowPt_Zinc2jet,       genBestJetsMass_LowPt_Zinc2jet);

    
    responseFirstJetPtEta               = newResp(FirstJetPtEta_Zinc1jet,            genFirstJetPtEta_Zinc1jet);
    responseSecondJetPtEta              = newResp(SecondJetPtEta_Zinc2jet,           genSecondJetPtEta_Zinc2jet);
    responseThirdJetPtEta               = newResp(ThirdJetPtEta_Zinc3jet,            genThirdJetPtEta_Zinc3jet);
    responseFourthJetPtEta              = newResp(FourthJetPtEta_Zinc4jet,           genFourthJetPtEta_Zinc4jet);
    responseFifthJetPtEta               = newResp(FifthJetPtEta_Zinc5jet,            genFifthJetPtEta_Zinc5jet);
    responseSixthJetPtEta               = newResp(SixthJetPtEta_Zinc6jet,            genSixthJetPtEta_Zinc6jet);

    
    responseFirstHighestJetPt_Zinc1jet  = newResp(FirstHighestJetPt_Zinc1jet,        genFirstHighestJetPt_Zinc1jet);
    responseFirstHighestJetPt_Zinc2jet  = newResp(FirstHighestJetPt_Zinc2jet,        genFirstHighestJetPt_Zinc2jet);
    responseFirstHighestJetPt_Zinc3jet  = newResp(FirstHighestJetPt_Zinc3jet,        genFirstHighestJetPt_Zinc3jet);
    responseFirstHighestJetPt_Zinc4jet  = newResp(FirstHighestJetPt_Zinc4jet,        genFirstHighestJetPt_Zinc4jet);
    responseFirstHighestJetPt_Zinc5jet  = newResp(FirstHighestJetPt_Zinc5jet,        genFirstHighestJetPt_Zinc5jet);
    responseFirstHighestJetPt_Zinc6jet  = newResp(FirstHighestJetPt_Zinc6jet,        genFirstHighestJetPt_Zinc6jet);
    responseSecondHighestJetPt_Zinc2jet = newResp(SecondHighestJetPt_Zinc2jet,       genSecondHighestJetPt_Zinc2jet);
    responseSecondHighestJetPt_Zinc3jet = newResp(SecondHighestJetPt_Zinc3jet,       genSecondHighestJetPt_Zinc3jet);
    responseSecondHighestJetPt_Zinc4jet = newResp(SecondHighestJetPt_Zinc4jet,       genSecondHighestJetPt_Zinc4jet);
    responseSecondHighestJetPt_Zinc5jet = newResp(SecondHighestJetPt_Zinc5jet,       genSecondHighestJetPt_Zinc5jet);
    responseSecondHighestJetPt_Zinc6jet = newResp(SecondHighestJetPt_Zinc6jet,       genSecondHighestJetPt_Zinc6jet);
    responseThirdHighestJetPt_Zinc3jet  = newResp(ThirdHighestJetPt_Zinc3jet,        genThirdHighestJetPt_Zinc3jet);
    responseThirdHighestJetPt_Zinc4jet  = newResp(ThirdHighestJetPt_Zinc4jet,        genThirdHighestJetPt_Zinc4jet);
    responseThirdHighestJetPt_Zinc5jet  = newResp(ThirdHighestJetPt_Zinc5jet,        genThirdHighestJetPt_Zinc5jet);
    responseThirdHighestJetPt_Zinc6jet  = newResp(ThirdHighestJetPt_Zinc6jet,        genThirdHighestJetPt_Zinc6jet);
    responseSpTJets_Zexc2jet            = newResp(SpTJets_Zexc2jet,                  genSpTJets_Zexc2jet);
    responseSpTJets_Zinc2jet            = newResp(SpTJets_Zinc2jet,                  genSpTJets_Zinc2jet);
    responseBestSpTJets_Zinc2jet        = newResp(BestSpTJets_Zinc2jet,              genBestSpTJets_Zinc2jet);
    responseSpTJets_LowPt_Zexc2jet      = newResp(SpTJets_LowPt_Zexc2jet,            genSpTJets_LowPt_Zexc2jet);
    responseSpTJets_LowPt_Zinc2jet      = newResp(SpTJets_LowPt_Zinc2jet,            genSpTJets_LowPt_Zinc2jet);
    responseBestSpTJets_LowPt_Zinc2jet  = newResp(BestSpTJets_LowPt_Zinc2jet,        genBestSpTJets_LowPt_Zinc2jet);
    responseSpT_Zexc2jet                = newResp(SpT_Zexc2jet,                      genSpT_Zexc2jet);
    responseSpT_Zinc2jet                = newResp(SpT_Zinc2jet,                      genSpT_Zinc2jet);
    responseBestSpT_Zinc2jet            = newResp(BestSpT_Zinc2jet,                  genBestSpT_Zinc2jet);
    responseSpT_LowPt_Zexc2jet          = newResp(SpT_LowPt_Zexc2jet,                genSpT_LowPt_Zexc2jet);
    responseSpT_LowPt_Zinc2jet          = newResp(SpT_LowPt_Zinc2jet,                genSpT_LowPt_Zinc2jet);
    responseBestSpT_LowPt_Zinc2jet      = newResp(BestSpT_LowPt_Zinc2jet,            genBestSpT_LowPt_Zinc2jet);
    responsedPhiJets_Zexc2jet           = newResp(dPhiJets_Zexc2jet,                 gendPhiJets_Zexc2jet);
    
    responseBestdPhiJets_Zinc2jet       = newResp(BestdPhiJets_Zinc2jet,             genBestdPhiJets_Zinc2jet);
    responsedPhiJets_LowPt_Zexc2jet     = newResp(dPhiJets_LowPt_Zexc2jet,           gendPhiJets_LowPt_Zexc2jet);
    responsedPhiJets_LowPt_Zinc2jet     = newResp(dPhiJets_LowPt_Zinc2jet,           gendPhiJets_LowPt_Zinc2jet);
    responseBestdPhiJets_LowPt_Zinc2jet = newResp(BestdPhiJets_LowPt_Zinc2jet,       genBestdPhiJets_LowPt_Zinc2jet);
    responsePHI_Zexc2jet                = newResp(PHI_Zexc2jet,                      genPHI_Zexc2jet);
    responsePHI_Zinc2jet                = newResp(PHI_Zinc2jet,                      genPHI_Zinc2jet);
    responseBestPHI_Zinc2jet            = newResp(BestPHI_Zinc2jet,                  genBestPHI_Zinc2jet);
    responsePHI_LowPt_Zexc2jet          = newResp(PHI_LowPt_Zexc2jet,                genPHI_LowPt_Zexc2jet);
    responsePHI_LowPt_Zinc2jet          = newResp(PHI_LowPt_Zinc2jet,                genPHI_LowPt_Zinc2jet);
    responseBestPHI_LowPt_Zinc2jet      = newResp(BestPHI_LowPt_Zinc2jet,            genBestPHI_LowPt_Zinc2jet);
    responsePHI_T_Zexc2jet              = newResp(PHI_T_Zexc2jet,                    genPHI_T_Zexc2jet);
    responsePHI_T_Zinc2jet              = newResp(PHI_T_Zinc2jet,                    genPHI_T_Zinc2jet);
    responseBestPHI_T_Zinc2jet          = newResp(BestPHI_T_Zinc2jet,                genBestPHI_T_Zinc2jet);
    responsePHI_T_LowPt_Zexc2jet        = newResp(PHI_T_LowPt_Zexc2jet,              genPHI_T_LowPt_Zexc2jet);
    responsePHI_T_LowPt_Zinc2jet        = newResp(PHI_T_LowPt_Zinc2jet,              genPHI_T_LowPt_Zinc2jet);
    responseBestPHI_T_LowPt_Zinc2jet    = newResp(BestPHI_T_LowPt_Zinc2jet,          genBestPHI_T_LowPt_Zinc2jet);
    responseSPhi_Zexc2jet               = newResp(SPhi_Zexc2jet,                     genSPhi_Zexc2jet);
    responseSPhi_Zinc2jet               = newResp(SPhi_Zinc2jet,                     genSPhi_Zinc2jet);
    responsedEtaJets_Zexc2jet           = newResp(dEtaJets_Zexc2jet,                 gendEtaJets_Zexc2jet);
    responsedEtaJets_Zinc2jet           = newResp(dEtaJets_Zinc2jet,                 gendEtaJets_Zinc2jet);
    responseBestSPhi_Zinc2jet           = newResp(BestSPhi_Zinc2jet,                 genBestSPhi_Zinc2jet);
    responseSPhi_LowPt_Zexc2jet         = newResp(SPhi_LowPt_Zexc2jet,               genSPhi_LowPt_Zexc2jet);
    responseSPhi_LowPt_Zinc2jet         = newResp(SPhi_LowPt_Zinc2jet,               genSPhi_LowPt_Zinc2jet);
    responseBestSPhi_LowPt_Zinc2jet     = newResp(BestSPhi_LowPt_Zinc2jet,           genBestSPhi_LowPt_Zinc2jet);

    // ratio plots  
    responseRatioJetPt21                = newResp(RatioJetPt21_Zinc2jet,               genRatioJetPt21_Zinc2jet);
    responseRatioJetPt32                = newResp(RatioJetPt32_Zinc3jet,               genRatioJetPt32_Zinc3jet);

    // vector boson and single jet
    responsedEtaBosonJet_Zexc1          = newResp(dEtaBosonJet_Zexc1jet,            gendEtaBosonJet_Zexc1jet);
    responsedEtaBosonJet_Zinc1          = newResp(dEtaBosonJet_Zinc1jet,            gendEtaBosonJet_Zinc1jet);


    // for 2D unfolding --- create 1D histograms for different rapidity ranges
    NbinsEta2Dtest = NbinsEta2D ;
    double j_pT_range1[NbinsEta2D]={30.,50.,70.,100.,130.,170.,220.,350.,1000.};
    double j_Y_range1[NbinsEta2D]={0.,0.5,1.0,1.5,2.0,2.5,3.0,3.2,4.7};
    for ( int i =0 ; i < NbinsEta2D  ; i++){
        j_pT_range[i] = j_pT_range1[i];
        j_Y_range[i] = j_Y_range1[i];
    }
    /// setup leading jet hitograms and response
    for ( int i =0 ; i < NbinsEta2D - 1 ; i++){
        char name[100];
        char name1[100];
        sprintf(name,"FirstJetPt_Zinc1jet_Eta_%i",i);
        sprintf(name1,"1st jet p_{T} (N_{jets} #geq 1): Rap %i", i );
        FirstJetPt_Zinc1jet_Eta[i]= newTH1D(name, name1,           "p_{T}(j_{1}) [GeV]",     NbinsEta2D - 1, j_pT_range);
        sprintf(name,"genFirstJetPt_Zinc1jet_Eta%i",i);
        sprintf(name1,"gen: 1st jet p_{T} (N_{jets} #geq 1): Rap %i", i );
        genFirstJetPt_Zinc1jet_Eta[i]= newTH1D(name, name1,           "p_{T}(j_{1}) [GeV]",     NbinsEta2D - 1, j_pT_range);
        responseFirstJetPt_Zinc1jet_Eta[i] = newResp( FirstJetPt_Zinc1jet_Eta[i], genFirstJetPt_Zinc1jet_Eta[i] );
    }
    // setup 2nd leading jet hitograms and response
    for ( int i =0 ; i < NbinsEta2D - 1 ; i++){
        char name[100];
        char name1[100];
        sprintf(name,"SecondJetPt_Zinc2jet_Eta_%i",i);
        sprintf(name1,"2nd jet p_{T} (N_{jets} #geq 2): Rap %i", i );
        SecondJetPt_Zinc2jet_Eta[i]= newTH1D(name, name1,           "p_{T}(j_{2}) [GeV]",     NbinsEta2D - 1, j_pT_range);
        sprintf(name,"genSecondJetPt_Zinc2jet_Eta%i",i);
        sprintf(name1,"gen: 2nd jet p_{T} (N_{jets} #geq 2): Rap %i", i );
        genSecondJetPt_Zinc2jet_Eta[i]= newTH1D(name, name1,           "p_{T}(j_{2}) [GeV]",     NbinsEta2D - 1, j_pT_range);
        responseSecondJetPt_Zinc2jet_Eta[i] = newResp( SecondJetPt_Zinc2jet_Eta[i], genSecondJetPt_Zinc2jet_Eta[i] );
    }

}
