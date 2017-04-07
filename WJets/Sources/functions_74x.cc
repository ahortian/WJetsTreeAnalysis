// History
//---- 2015_06_13
// fix get efficiency
//---- 2015_06_12
// fix smear jet function

#include <iostream>
#include <sstream>
#include <fstream>
#include <TH1.h>
#include <TH2.h>
#include <TLorentzVector.h>
#include <vector>
#include <cstdarg>
#include "functions.h"

using namespace std;

void barre_de_progression(int pourcentage){
    string progression = "[";
    for (int i=0; i<pourcentage; i++){ progression += "="; }
    for (int i=0; i<(100-pourcentage); i++){ progression += " "; }
    progression += "]";
    ostringstream oss; oss << pourcentage;
    string pourcentage_str = oss.str();
    for (int i=0; i < (int) pourcentage_str.size(); i++){ progression[49+i] = pourcentage_str[i]; }
    if (pourcentage<100) cout << "\t" << progression << "\r" ;
    else cout << "\t" << progression << endl;
}

bool LepDescendingOrder(leptonStruct l1, leptonStruct l2){
    return (l1.pt > l2.pt);
}

bool JetDescendingOrder(jetStruct j1, jetStruct j2){
    return (j1.pt > j2.pt);
}

//***************************** begin edit ********************************************//
bool JetYDescendingOrder(TLorentzVector tj1, TLorentzVector tj2){
    return (tj1.Rapidity() > tj2.Rapidity());
}

double deltaRYPhi(TLorentzVector j1, TLorentzVector j2){
    double dY = j1.Rapidity() - j2.Rapidity();
    double dPhi = deltaPhi(j1, j2);
    return sqrt(dY * dY + dPhi * dPhi);
}
//***************************** end edit ********************************************//

vector<double> makeVector(int num, ...)
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

void insertVector(vector<double>& veca, int num, ...)
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

TH1D* newTH1D(string name, string title, string xTitle, int nBins, double *xBins)
{
    TH1D* hist = new TH1D(name.c_str(), title.c_str(), nBins, xBins);
    hist->GetXaxis()->SetTitle(xTitle.c_str());
    hist->GetYaxis()->SetTitle("# Events");
    return hist;
}

TH1D* newTH1D(string name, string title, string xTitle, vector<double>& xBinsVect)
{
    int nBins = xBinsVect.size()-1;
    double *xBins = new double[xBinsVect.size()];
    std::copy(xBinsVect.begin(), xBinsVect.end(), xBins);
    TH1D* hist = new TH1D(name.c_str(), title.c_str(), nBins, xBins);
    hist->GetXaxis()->SetTitle(xTitle.c_str());
    hist->GetYaxis()->SetTitle("# Events");
    delete [] xBins;
    return hist;
}


TH1D* newTH1D(string name, string title, string xTitle, int nBins, double xLow, double xUp){
    TH1D* hist = new TH1D(name.c_str(), title.c_str(), nBins, xLow, xUp);
    hist->GetXaxis()->SetTitle(xTitle.c_str());
    hist->GetYaxis()->SetTitle("# Events");
    hist->SetOption("HIST");
    return hist;
}

TH2D* newTH2D(string name, string title, int nBinsX, double *xBins, int nBinsY, double *yBinsY){
    TH2D* hist = new TH2D(name.c_str(), title.c_str(), nBinsX, xBins, nBinsY, yBinsY);
    hist->GetZaxis()->SetTitle("# Events");
    return hist;
}

TH2D* newTH2D(string name, string title, int nBinsX, double *xBins, int nBinsY, double yLow, double yUp){
    TH2D* hist = new TH2D(name.c_str(), title.c_str(), nBinsX, xBins, nBinsY, yLow, yUp);
    hist->GetZaxis()->SetTitle("# Events");
    return hist;
}

TH2D* newTH2D(string name, string title, int nBinsX, double xLow, double xUp, int nBinsY, double *yBins){
    TH2D* hist = new TH2D(name.c_str(), title.c_str(), nBinsX, xLow, xUp, nBinsY, yBins);
    hist->GetZaxis()->SetTitle("# Events");
    return hist;
}

TH2D* newTH2D(string name, string title, int nBinsX, double xLow, double xUp, int nBinsY, double yLow, double yUp){
    TH2D* hist = new TH2D(name.c_str(), title.c_str(), nBinsX, xLow, xUp, nBinsY, yLow, yUp);
    hist->GetZaxis()->SetTitle("# Events");
    hist->SetOption("HIST");
    return hist;
}

RooUnfoldResponse* newResp(TH1D* reco, TH1D* gen)
{
    RooUnfoldResponse *response = new RooUnfoldResponse(reco, gen);
    return response;
}

RooUnfoldResponse* newResp(TH2D* reco, TH2D* gen)
{
    RooUnfoldResponse *response = new RooUnfoldResponse(reco, gen);
    return response;
}

double phi0to2pi(double phi){
    double pi = 3.141592653589793238;
    while (phi >= 2.*pi) phi -= 2.*pi;
    while (phi < 0.) phi += 2.*pi;
    return phi;
}

double deltaPhi(TLorentzVector v1, TLorentzVector v2){
    // build the delta Phi angle between the two vectors
    double pi = 3.141592653589793238;
    double phi1 = phi0to2pi(v1.Phi());
    double phi2 = phi0to2pi(v2.Phi());
    double dPhi = phi0to2pi(phi1 - phi2);
    dPhi = (dPhi > (2*pi - dPhi)) ? 2*pi - dPhi : dPhi;
    return dPhi;
} 

double deltaPhi(double Phi1, double Phi2){
    // build the delta Phi angle between the two vectors
    double pi = 3.141592653589793238;
    double phi1 = phi0to2pi(Phi1);
    double phi2 = phi0to2pi(Phi2);
    double dPhi = phi0to2pi(phi1 - phi2);
    dPhi = (dPhi > (2*pi - dPhi)) ? 2*pi - dPhi : dPhi;
    //cout << "      DeltaPhi: " << endl;
    //cout << "      phi1 = " << phi1 << "  phi2 = " << phi2 << endl;
    //cout << "      DeltaPhi = " << dPhi << endl;
    return dPhi;
} 

double deltaR(TLorentzVector v1, TLorentzVector v2){
    double dEta = v1.Eta() - v2.Eta();
    double dPhi = deltaPhi(v1, v2);
    return sqrt(dEta * dEta + dPhi * dPhi);
}

double deltaR(double Phi1, double Eta1, double Phi2, double Eta2){
    //cout << "DeltaR:" << endl;
    //cout << "phi1 = " << Phi1 << "  eta1 = " << Eta1 << "  phi2 = " << Phi2 << "  eta2 = " << Eta2 << endl; 
    double dEta = Eta1 - Eta2;
    double dPhi = deltaPhi(Phi1, Phi2);
    //cout << "   deltaR = " << sqrt(dEta * dEta + dPhi * dPhi) << endl;
    return sqrt(dEta * dEta + dPhi * dPhi);
}

double PHI(TLorentzVector l1, TLorentzVector l2, TLorentzVector j1, TLorentzVector j2){
    // build the angle PHI between the two subsytems (l1+l2, j1+j2) vectors 
    double lPx = (l1.Px() + l2.Px());
    double lPy = (l1.Py() + l2.Py());
    double lPz = (l1.Pz() + l2.Pz());
    double lNorm = sqrt(lPx * lPx + lPy * lPy + lPz * lPz);
    double jPx = (j1.Px() + j2.Px());
    double jPy = (j1.Py() + j2.Py());
    double jPz = (j1.Pz() + j2.Pz());
    double jNorm = sqrt(jPx * jPx + jPy * jPy + jPz * jPz);
    return acos((jPx * lPx + jPy * lPy + jPz * lPz) / (jNorm * lNorm));
}

double PHI_T(TLorentzVector l1, TLorentzVector l2, TLorentzVector j1, TLorentzVector j2){
    // build the angle PHI between the two subsytems (l1+l2, j1+j2) vectors in the transverse plane
    double lPx = (l1.Px() + l2.Px());
    double lPy = (l1.Py() + l2.Py());
    double lNorm = sqrt(lPx * lPx + lPy * lPy);
    double jPx = (j1.Px() + j2.Px());
    double jPy = (j1.Py() + j2.Py());
    double jNorm = sqrt(jPx * jPx + jPy * jPy);
    return acos((jPx * lPx + jPy * lPy) / (jNorm * lNorm));
}

double SpTsub(TLorentzVector v1, TLorentzVector v2){
    return sqrt(pow(v1.Px() + v2.Px(), 2) + pow(v1.Py() + v2.Py(), 2)) / (v1.Pt() + v2.Pt());
}

double SpT(TLorentzVector l1, TLorentzVector l2, TLorentzVector j1, TLorentzVector j2){
    return sqrt( pow(SpTsub(l1, l2), 2) + pow(SpTsub(j1, j2), 2)  ) / sqrt(2.);
} 

double SPhi(TLorentzVector l1, TLorentzVector l2, TLorentzVector j1, TLorentzVector j2){
    return sqrt(deltaPhi(l1, l2) * deltaPhi(l1, l2) + deltaPhi(j1, j2) * deltaPhi(j1, j2)) / sqrt(2.);
}

record::record(): 
    ptLow(0), ptHi(0), etaLow(0), etaHi(0), effi(0),effiErrorLow(0), effiErrorHigh(0)
{
}

record::record(double pt1, double pt2, double eta1, double eta2, double eff, double effLow, double effHigh):
    ptLow(pt1), ptHi(pt2), etaLow(eta1), etaHi(eta2), effi(eff),effiErrorLow(effLow), effiErrorHigh(effHigh)
{
}

bool record::belongTo(double pt, double eta)
{
    return (pt < ptHi && pt >= ptLow) && (eta < etaHi && eta >= etaLow);
}

table::table()
{
}

table::table(string filename)
{
    ifstream file(filename.c_str());
    cout << filename << endl;
    if (file) cout << "OK" << endl;
    else cout << "Not OK" << endl;
    /*
       double  pt1, pt2, eta1, eta2, effi, effiErrorLow, effiErrorHigh ;
       while( file >> eta1 >> eta2 >> pt1 >> pt2 >> effi >> effiErrorLow >> effiErrorHigh){
    //    cout << eta1 << "  " << eta2 << "  " << pt1 << "  " << pt2 << endl;
    recd.push_back(record(pt1, pt2, eta1, eta2, effi,effiErrorLow, effiErrorHigh));
    //    cout << "ef " << effi << endl;
    }
    */
    double data[7];
    while( file ){
        for (int i=0;i<7;i++)
        {
            file>>data[i];
            cout << data[i] <<"  " ;
        }
        cout << endl;
        recd.push_back(record(data[2],data[3],data[0],data[1],data[4],data[5],data[6]));

    }

}

double table::getEfficiency(double pt, double eta, int sysLepSF){
    double hiPtBin= 0;
    if (sysLepSF == 0){
        for (unsigned int i=0; i != recd.size(); i++) {
            if((recd[i]).belongTo(pt, eta)) return recd[i].effi;
            if((recd[i]).belongTo(190, eta)) hiPtBin = recd[i].effi;
        }
        return hiPtBin;
    }
    else if (sysLepSF == 1){
        for (unsigned int i=0; i != recd.size(); i++) {
            if((recd[i]).belongTo(pt, eta)) return recd[i].effi+recd[i].effiErrorHigh;
            if((recd[i]).belongTo(190, eta)) hiPtBin = recd[i].effi;
        }
        return hiPtBin;
    }
    else if (sysLepSF == -1){
        for (unsigned int i=0; i != recd.size(); i++) {
            if((recd[i]).belongTo(pt, eta)) return recd[i].effi-recd[i].effiErrorLow;
            if((recd[i]).belongTo(190, eta)) hiPtBin = recd[i].effi;
        }
        return hiPtBin;
    }
    else return 1;
}
double table::getEfficiency(double pt, double eta){
    double hiPtBin= 0;
    for (unsigned int i=0; i != recd.size(); i++) {
        // if finds the proper bin, then return the efficiency
        if((recd[i]).belongTo(pt, eta)) return recd[i].effi;
        // else store the average pt of the current bin efficency but do not return and try the next bin
        if((recd[i]).belongTo(0.5*(recd[i].ptHi + recd[i].ptLow), eta)) hiPtBin = recd[i].effi;
    }
    return hiPtBin;
}
double table::getEfficiencyLow(double pt, double eta){
    double hiPtBin= 0;
    for (unsigned int i=0; i != recd.size(); i++) {
        if((recd[i]).belongTo(pt, eta)) return recd[i].effi-recd[i].effiErrorLow;
        if((recd[i]).belongTo(190, eta)) hiPtBin = recd[i].effi;
    }
    return hiPtBin;
}
double table::getEfficiencyHigh(double pt, double eta){
    double hiPtBin= 0;
    for (unsigned int i=0; i != recd.size(); i++) {
        if((recd[i]).belongTo(pt, eta)) return recd[i].effi+recd[i].effiErrorHigh;
        if((recd[i]).belongTo(190, eta)) hiPtBin = recd[i].effi;
    }
    return hiPtBin;
}


double SmearJetPt(double recoPt, double genPt, double eta, int smearJet){
    // Fall 2015 resolution scale factor
    // twiki.cern.ch/twiki/bin/view/CMS/JetResolution
    double centralSF(1.00);
    if      (fabs(eta) < 0.8) centralSF = 1.061;
    else if (fabs(eta) < 1.3) centralSF = 1.088;
    else if (fabs(eta) < 1.9) centralSF = 1.106;
    else if (fabs(eta) < 2.5) centralSF = 1.126;
    else if (fabs(eta) < 3.0) centralSF = 1.343;
    else if (fabs(eta) < 3.2) centralSF = 1.303;
    else if (fabs(eta) < 5.0) centralSF = 1.320;
    else centralSF = 1.320;
    
    double upSF(1.00);
    if      (fabs(eta) < 0.8) upSF = 1.084;
    else if (fabs(eta) < 1.3) upSF = 1.117;
    else if (fabs(eta) < 1.9) upSF = 1.136;
    else if (fabs(eta) < 2.5) upSF = 1.220;
    else if (fabs(eta) < 3.0) upSF = 1.466;
    else if (fabs(eta) < 3.2) upSF = 1.414;
    else if (fabs(eta) < 5.0) upSF = 1.606;
    else upSF = 1.606;
    
    double downSF(1.00);
    if      (fabs(eta) < 0.8) downSF = 1.038;
    else if (fabs(eta) < 1.3) downSF = 1.059;
    else if (fabs(eta) < 1.9) downSF = 1.076;
    else if (fabs(eta) < 2.5) downSF = 1.032;
    else if (fabs(eta) < 3.0) downSF = 1.220;
    else if (fabs(eta) < 3.2) downSF = 1.192;
    else if (fabs(eta) < 5.0) downSF = 1.034;
    else downSF = 1.034;


    double smearedPt(0);
    
    if (smearJet == 0) {
        smearedPt = std::max(0., genPt + centralSF*(recoPt - genPt));
    }
    else if (smearJet == 1) {
        smearedPt = std::max(0., genPt + upSF*(recoPt - genPt));
    }
    else if (smearJet == -1) {
        smearedPt = std::max(0., genPt + downSF*(recoPt - genPt));
    }
    
    return smearedPt;
}


void normalizeTH2D(TH2D *h)
{
    int xbin(h->GetNbinsX()), ybin(h->GetNbinsY());
    for (int i(1); i <= ybin; i++){
        double sum(0.);
        for (int j(1); j <= xbin; j++){
            sum += h->GetBinContent(j,i);
        }
        for (int j(1); j <= xbin; j++){
            if (sum > 0) h->SetBinContent(j, i, h->GetBinContent(j, i) / sum );
        }
    }
}


void bestTwoJetsCandidatesPt(vector<jetStruct> jets, pair<TLorentzVector, TLorentzVector>& bestTwoJets)
{
    int nGoodJets(jets.size());
    if (nGoodJets >= 2){
        //cout << "\nMore than 2 jets, selecting best pair" << endl;
        double minPt(999999.);
        for (int i(0); i < nGoodJets - 1; i++) {
            TLorentzVector jeti; jeti.SetPtEtaPhiE(jets[i].pt, jets[i].eta, jets[i].phi, jets[i].energy);
            for (int j(i + 1); j < nGoodJets; j++) {
                TLorentzVector jetj; jetj.SetPtEtaPhiE(jets[j].pt, jets[j].eta, jets[j].phi, jets[j].energy);
                TLorentzVector jetij = jeti + jetj;
                //cout << i << " " << j << ": Pair pt = " << jetij.Pt() << endl;
                if (jetij.Pt() < minPt){
                    bestTwoJets.first = jeti; 
                    bestTwoJets.second = jetj;
                    minPt = jetij.Pt();
                    //cout << "Smallest pt = " << jetij.Pt() << endl;
                }
            }
        } 
    }
}

void bestTwoJetsCandidatesPhi(vector<jetStruct> jets, pair<TLorentzVector, TLorentzVector>& bestTwoJets)
{
    int nGoodJets(jets.size());
    if (nGoodJets >= 2){
        //cout << "\nMore than 2 jets, selecting best pair" << endl;
        double maxdPhi(-0.0001);
        for (int i(0); i < nGoodJets - 1; i++) {
            TLorentzVector jeti; jeti.SetPtEtaPhiE(jets[i].pt, jets[i].eta, jets[i].phi, jets[i].energy);
            for (int j(i + 1); j < nGoodJets; j++) {
                TLorentzVector jetj; jetj.SetPtEtaPhiE(jets[j].pt, jets[j].eta, jets[j].phi, jets[j].energy);
                double dPhi = deltaPhi(jeti, jetj);
                //cout << i << " " << j << ": dPhi = " << dPhi << endl;
                if (dPhi > maxdPhi){
                    bestTwoJets.first = jeti; 
                    bestTwoJets.second = jetj;
                    maxdPhi = dPhi;
                    //cout << "Biggest dPhi = " << dPhi << endl;
                }
            }
        } 
    }
}
