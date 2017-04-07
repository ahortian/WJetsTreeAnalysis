#include "unfoldingFunctions.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <TArrow.h>
#include <TLatex.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TMath.h>
#include <RooUnfoldResponse.h>
#include <RooUnfoldSvd.h>
#include <RooUnfoldBayes.h>
#include <RooUnfoldBinByBin.h>
#include <RooUnfoldInvert.h>

#include "writeFastPlotsTex.h"
#include "getFilesAndHistograms.h"
#include "functions.h"

using namespace std;
///////////////////////////////////////////////////////////////////////////////////////////////////////
TH1D* combinedHisto(TH1D* hMuon, TH1D* hEle, int NBinsOrig){
    int NBins = 2 * NBinsOrig ;
    cout << "combinedHisto  :"<< NBinsOrig <<"  " << NBins << endl;
    TH1D* hOut;
    double binWidth = (hMuon->GetXaxis()->GetXmax()-hMuon->GetXaxis()->GetXmin()) / hMuon->GetXaxis()->GetNbins() ;
    double binMax = hMuon->GetXaxis()->GetXmin() + binWidth * NBins ;
    cout << " merging histograms :" << hMuon->GetXaxis()->GetXbins()->fN << " Nbins = " << binWidth << "  " << binMax <<"  " << hMuon->GetXaxis()->GetXmax() <<  endl;
    /// for fixed bin width
    if ( !hMuon->GetXaxis()->GetXbins()->fN  ) {
        hOut = newTH1D((string ) hMuon->GetName(), (string) hMuon->GetTitle(),    (string) hMuon->GetYaxis()->GetTitle(),   NBins, hMuon->GetXaxis()->GetXmin(), hMuon->GetXaxis()->GetXmin() + binWidth * NBins );
        int countBins = 1 ;
        for ( int j = 0;  j <  NBins/2    ; j++){
            hOut->SetBinContent(countBins, hMuon->GetBinContent(j + 1));
            hOut->SetBinError(countBins, hMuon->GetBinError(j + 1));
            countBins++;
        }
        for ( int j = 0;  j <  NBins/2    ; j++){  
            hOut->SetBinContent(countBins, hEle->GetBinContent(j + 1));
            hOut->SetBinError(countBins, hEle->GetBinError(j + 1));
            countBins++;
        }        
        for ( int j = 0;  j <  NBins    ; j++){
            cout << " combine:  " << j <<"    " << hOut->GetBinContent(j + 1)<< endl;

        }
    }
    else{
        cout << " GO IN THE PART WITH NON FIXED BIN WIDTH " << endl;
        // let's create array needed for bin definition 
        //Double_t*  test;
        double*  test;
        test = (double *) hMuon->GetXaxis()->GetXbins()->GetArray() ;
        double bins[NBins + 1 ];
        int count = 0 ; 
        for ( int j = 0 ; j < 2 ; j++){
            for ( int i = 0 ; i < NBinsOrig + 1   ; i++){
                bins[count] = test[i];
                if ( j == 1 && i == 0 ) continue;  
                if ( j == 1 ) bins[count] = bins[count-1]+test[i] - test[i-1];
                if ( i == NBinsOrig && NBinsOrig != hMuon->GetXaxis()->GetNbins()  ) bins[count] = bins[count-1]+test[NBinsOrig-1] - test[NBinsOrig-2];

                cout << " TEST:" <<NBins <<"   " << count<<"   "<< i << "  " << test[i] <<"    " << bins[count]<<"  " << test[i-1]-test[i-2]<< "    " <<  test[i-1]<<"   " << test[i-2] << endl;
                count++;
            }
        }
        for ( int i = 0 ; i < NBins + 1  ; i++){
            cout << " bins:" << i<<"   " << bins[i]<< endl;
        }
        hOut = newTH1D((string ) hMuon->GetName(), (string) hMuon->GetTitle(),    (string) hMuon->GetYaxis()->GetTitle(),   NBins, bins );
        // now fill histo
        int countBins = 1 ;
        for ( int j = 0 ; j < NBinsOrig  ; j++){
            hOut->SetBinContent(countBins, hMuon->GetBinContent(j + 1));
            hOut->SetBinError(countBins, hMuon->GetBinError(j + 1));
            countBins++;
            cout << "reco muon: "<< j+1 <<"   " << hMuon->GetBinContent(j + 1) << endl; 
        }
        for ( int j = 0;  j <  NBinsOrig    ; j++){
            hOut->SetBinContent(countBins, hEle->GetBinContent(j + 1));
            hOut->SetBinError(countBins, hEle->GetBinError(j + 1));
            countBins++;
        }

    }
    for ( int j = 0 ; j < NBins  ; j++){
        cout << " both leptons reco : " << j+1 <<"   " <<  hOut->GetBinContent(j +  1)<< endl;

    }
    
    hOut->Draw();
    return hOut;

}
///////////////////////////////////////////////////////////////////////////////////////////////////////
TH1D* mergeGenHisto(TH1D* hMuon, TH1D* hEle, int NBins){

    TH1D* hOut;
    double binWidth = (hMuon->GetXaxis()->GetXmax()-hMuon->GetXaxis()->GetXmin()) / hMuon->GetXaxis()->GetNbins() ;
    double binMax = hMuon->GetXaxis()->GetXmin() + binWidth*2 * NBins ;
    if ( !hMuon->GetXaxis()->GetXbins()->fN  ) { hOut = newTH1D((string ) hMuon->GetName(), (string) hMuon->GetTitle(),    (string) hMuon->GetYaxis()->GetTitle(),   NBins, hMuon->GetXaxis()->GetXmin(), hMuon->GetXaxis()->GetXmin() + binWidth * NBins );
    }
    else {
        double*  test;
        test = (double *) hMuon->GetXaxis()->GetXbins()->GetArray() ;
        double bins[NBins + 1 ];
        int count = 0 ;
            for ( int i = 0 ; i < NBins + 1   ; i++){
                bins[count] = test[i];
                if ( i == NBins && NBins != hMuon->GetXaxis()->GetNbins()   ) bins[count] = bins[count-1]+test[NBins-1] - test[NBins-2];

                count++;
        }
        for ( int i = 0 ; i < NBins + 1  ; i++){
            cout << " merged 1D bins:" << i<<"   " << bins[i]<< endl;
        }
        hOut = newTH1D((string ) hMuon->GetName(), (string) hMuon->GetTitle(),    (string) hMuon->GetYaxis()->GetTitle(),   NBins,  bins );


    }
    for ( int j = 0;  j <  NBins    ; j++){
        double Mu = hMuon->GetBinContent(j + 1) ;
        double MuE = hMuon->GetBinError(j + 1) ;
        double E = hEle->GetBinContent(j + 1) ;
        double EE = hEle->GetBinError(j + 1) ;
        double sig2 =1 / ( 1/(EE*EE) + 1/(MuE*MuE) );
        //double mean = (Mu /( MuE * MuE ) + E /(EE* EE) ) * sig2;
        double mean = (E* ( MuE * MuE ) + Mu *(EE* EE) ) /(  MuE * MuE + EE* EE);
        hOut->SetBinContent( j + 1, mean );
        hOut->SetBinError(j+1, sqrt(sig2));
        cout << " merge gen histos: " << j<<"   " << mean << " mu : " << Mu <<"  Ele:  " << E << endl;
    }
    return hOut;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
TH2D* combineTH2DRes(TH2D* hMuon, TH2D* hEle, int NBinsOrig){

    int NBins = 2 * NBinsOrig ;
//    hMuon->Scale(2.27008258178896);
//    hEle->Scale(2.27251754624995);

    cout << "creating response matrix   :"<< NBinsOrig <<"  " << NBins << endl;
    TH2D* hOut;
    TH2D* hOutTest;
    double binWidth = (hMuon->GetXaxis()->GetXmax()-hMuon->GetXaxis()->GetXmin()) / hMuon->GetXaxis()->GetNbins() ;
    double binMax = hMuon->GetXaxis()->GetXmin() + binWidth*2 * NBins ;
    if ( !hMuon->GetXaxis()->GetXbins()->fN  ) {
        hOut = newTH2D((string ) hMuon->GetName(), (string) hMuon->GetTitle(),    NBins, hMuon->GetXaxis()->GetXmin(), hMuon->GetXaxis()->GetXmin() + binWidth * NBins, NBinsOrig, (double) hMuon->GetXaxis()->GetXmin(), (double ) hMuon->GetXaxis()->GetXmin() +  binWidth * NBinsOrig  );
        hOutTest = newTH2D((string ) hMuon->GetName(), (string) hMuon->GetTitle(),  NBinsOrig, (double) hMuon->GetXaxis()->GetXmin(), (double ) hMuon->GetXaxis()->GetXmin() +  binWidth * NBinsOrig, NBins, hMuon->GetXaxis()->GetXmin(), hMuon->GetXaxis()->GetXmin() + binWidth *       NBins  );
    }
    else {
        cout << " we need a vector of bins to initilaize histogram :"<< endl;
        double*  test;
        test = (double *) hMuon->GetXaxis()->GetXbins()->GetArray() ;
        double binsY[NBinsOrig+1];
        double binsX[NBins + 1];
        int count = 0 ;
        for ( int j = 0 ; j < 2 ; j++){
            for ( int i = 0 ; i < NBinsOrig + 1  ; i++){
                binsX[count] = test[i];
                if ( j == 1 && i == 0 ) continue;
                if ( test[i] == 0  ) binsX[count] = binsX[count-1]+test[i-1] - test[i-2];
                if ( j == 1 ) binsX[count] = binsX[count-1]+test[i] - test[i-1];
                if ( i == NBinsOrig && hMuon->GetXaxis()->GetNbins()  ) binsX[count] = binsX[count-1]+test[NBinsOrig-1] - test[NBinsOrig-2];

                //            cout << " TEST:" <<count<<"   "<< i << "  " << test[i] <<"    " << bins[count]<<"  " << test[i]-test[i-1]<< endl;
                count++;
            }
        }
        for ( int i = 0 ; i < NBinsOrig + 1  ; i++){
            binsY[i] = binsX[i] ;
        }

        hOut = newTH2D((string ) hMuon->GetName(), (string) hMuon->GetTitle(),  NBins, binsX, NBinsOrig, binsY  );
        hOutTest = newTH2D((string ) hMuon->GetName(), (string) hMuon->GetTitle(),  NBinsOrig, binsY, NBins, binsX  );
        // now fill histo
        //
    }
    cout << " Filling combined response : " << hOut->GetXaxis()->GetNbins()<<"   " << hOut->GetYaxis()->GetNbins()<<"   " << NBins + 1 << endl;
    cout << " Filling combined response : " << hOutTest->GetXaxis()->GetNbins()<<"   " << hOutTest->GetYaxis()->GetNbins()<<"   " << NBins + 1 << endl;
    int countBins = 1 ;
    for ( int i = 0;  i <  NBinsOrig    ; i++){
        for ( int j = 0;  j <  NBinsOrig    ; j++){
            hOut->SetBinContent(countBins, j+1, hMuon->GetBinContent(i + 1 , j + 1));
            hOut->SetBinError(countBins , j+1 , hMuon->GetBinError(i + 1 , j + 1));
            //hOut->SetBinContent(j+1 , countBins,  hMuon->GetBinContent(i + 1 , j + 1));
            //hOut->SetBinError( j+1 , countBins ,  hMuon->GetBinError(i + 1 , j + 1));
          
        }
        countBins++;
    }
    for ( int i = 0;  i <  NBinsOrig    ; i++){
        for ( int j = 0;  j <  NBinsOrig    ; j++){
            hOut->SetBinContent(countBins, j+1, hEle->GetBinContent(i + 1 ,j + 1));
            hOut->SetBinError(countBins, j+1 , hEle->GetBinError(i + 1 ,j + 1));
            //hOut->SetBinContent(j+1, countBins, hEle->GetBinContent(i + 1 ,j + 1));
            //hOut->SetBinError(j+1,  countBins,  hEle->GetBinError(i + 1 ,j + 1));
        }
        countBins++;
    }

    cout << " Elements of response:" << endl;
    for ( int i = 0;  i <  2*NBinsOrig    ; i++){
                cout << i <<"  " ;
                for ( int j = 0;  j <  NBinsOrig    ; j++){
                        cout <<   hOut->GetBinContent(i+1,j+1)<<"   ";
                }

            cout << endl;
    }

    //hOut->Draw();
    return hOut;
    //return hOutTest;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
TCanvas * createCanvas(TH1D* h2Dtemp11 , string name ){
    TCanvas *canA = new TCanvas(name.c_str(),name.c_str(),700,700);
    canA->cd();h2Dtemp11->Draw("TEXT");
    return canA;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
void normalizeTH2D(TH2D *h)
{
    int xbin(h->GetNbinsX()), ybin(h->GetNbinsY());
    for (int i(0); i <= ybin + 1; i++){
        double sum(0.);
        for (int j(0); j <= xbin + 1; j++){
            sum += h->GetBinContent(j,i);
        }
        for (int j(0); j <= xbin + 1; j++){
            if (sum > 0) h->SetBinContent(j, i, h->GetBinContent(j, i) / sum );
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////

void plotHNormResp(TH2D *hNormResp_, string leptonFlavor, string variable, string energy, string outputDirectory, TFile *outputRootFile, bool closureTest, bool save)
{
    if (leptonFlavor == "Muons" || leptonFlavor == "DMu") leptonFlavor = "DMu_";
    else if (leptonFlavor == "Electrons" || leptonFlavor == "DE") leptonFlavor = "DE_";
    else if (leptonFlavor == "Muon" || leptonFlavor == "SE") leptonFlavor = "SE_";
    else if (leptonFlavor == "Electron" || leptonFlavor == "SMu") leptonFlavor = "SMu_";


    TH2D *hNormResp = (TH2D*) hNormResp_->Clone();
    normalizeTH2D(hNormResp);

    string canNameRespMatrix = leptonFlavor + energy + "_" + variable + "_ResponseMatrix" ;

    if (closureTest) canNameRespMatrix += "_CT";
    string YTitle = hNormResp->GetYaxis()->GetTitle();  YTitle = "reco " + YTitle;
    string XTitle = hNormResp->GetYaxis()->GetTitle();  XTitle = "gen " + XTitle;
    string respTitle = hNormResp->GetTitle();
    string niceTitle = respTitle.substr(0, respTitle.find("#right"));
    niceTitle = niceTitle.substr(niceTitle.find("Response")+9);

    if (niceTitle.find("gen") != string::npos) niceTitle = niceTitle.substr(niceTitle.find("gen")+4);
    niceTitle = "Response Matrix for " + niceTitle;

    if (closureTest) niceTitle += " C.T.";
    if (niceTitle.find("Counter") != string::npos) hNormResp->GetYaxis()->SetLabelSize(0);

    hNormResp->SetTitle(niceTitle.c_str());
    hNormResp->GetYaxis()->SetTitle(YTitle.c_str());
    hNormResp->GetYaxis()->SetTitleSize(0.04);
    hNormResp->GetYaxis()->SetTitleOffset(1.10);
    hNormResp->GetXaxis()->SetTitle(XTitle.c_str());
    hNormResp->GetXaxis()->SetTitleSize(0.04);
    hNormResp->GetXaxis()->SetTitleOffset(1.10);
    hNormResp->GetZaxis()->SetRangeUser(0,1);

    TCanvas *canRespMatrix = new TCanvas(canNameRespMatrix.c_str(), canNameRespMatrix.c_str(), 700, 700);
    canRespMatrix->cd();
    hNormResp->SetContour(90);
    hNormResp->DrawCopy("COLZ");
    hNormResp->DrawCopy("TEXT SAME");
    string outputFileNamePNGResp = outputDirectory + canNameRespMatrix + ".pdf";
    if (save){
        canRespMatrix->Print(outputFileNamePNGResp.c_str());
        outputRootFile->cd();
        canRespMatrix->Write();
    }

}

void plotDVector(TH1D *hmodD_, int kterm, TH2D *hNormResp, string leptonFlavor, string variable, string energy, string outputDirectory, TFile *outputRootFile, bool closureTest, bool save)
{
    if (leptonFlavor == "Muons" || leptonFlavor == "DMu") leptonFlavor = "DMu_";
    else if (leptonFlavor == "Electrons" || leptonFlavor == "DE") leptonFlavor = "DE_";
    else if (leptonFlavor == "Muon" || leptonFlavor == "SE") leptonFlavor = "SE_";
    else if (leptonFlavor == "Electron" || leptonFlavor == "SMu") leptonFlavor = "SMu_";

    TH1D *hmodD = (TH1D*) hmodD_->Clone();
    int NBins(hmodD->GetNbinsX());

    double hmax(hmodD->GetMaximum());
    double kCont(hmodD->GetBinContent(kterm));
    kCont *= 5;

    TArrow *arrow = new TArrow(kterm+0.5, hmax, kterm+0.5, kCont, 0.05, "|>");
    arrow->SetAngle(40);
    arrow->SetLineWidth(2);
    arrow->SetLineColor(kRed+2);
    arrow->SetFillColor(kRed+2);

    string respTitle = hNormResp->GetTitle();
    string niceTitle = respTitle.substr(0, respTitle.find("#right"));
    niceTitle = niceTitle.substr(niceTitle.find("Response")+9);
    if (niceTitle.find("gen") != string::npos) niceTitle = niceTitle.substr(niceTitle.find("gen")+4);
    string modDTitle = "Rotated measured vector for " + niceTitle;
    if (closureTest) modDTitle += " C.T.";
    string canNameD = leptonFlavor + energy + "_" + variable + "_D";
    if (closureTest) canNameD += "_CT";

    hmodD->SetTitle(modDTitle.c_str());
    hmodD->SetLineWidth(3);
    hmodD->SetLineColor(kBlack);
    hmodD->GetXaxis()->SetTitle("Bin Numb.");
    hmodD->GetXaxis()->SetTitleSize(0.04);
    hmodD->GetXaxis()->SetTitleOffset(1.0);
    hmodD->GetYaxis()->SetTitleSize(0.04);
    hmodD->GetYaxis()->SetTitleOffset(1.15);
    hmodD->GetXaxis()->CenterLabels();
    hmodD->GetXaxis()->SetLabelSize(0.1/sqrt(NBins));
    hmodD->GetXaxis()->SetNdivisions(NBins, 0, 0);
    hmodD->GetYaxis()->SetTitle("|d|");
    TCanvas *canD = new TCanvas(canNameD.c_str(), canNameD.c_str(), 700, 700);
    canD->cd();
    canD->SetLeftMargin(0.1);
    canD->SetRightMargin(0.05);
    canD->SetLogy();
    canD->SetGrid();
    canD->SetTicks();
    hmodD->DrawCopy("TEXT60");
    arrow->Draw();
    string outputFileNamePNG = outputDirectory + canNameD + ".pdf";
    if (save){
        canD->Print(outputFileNamePNG.c_str());
        outputRootFile->cd();
        canD->Write();
    }
}


void plotSVVector(TH1D *hSV_, TH2D *hNormResp, string leptonFlavor, string variable, string energy, string outputDirectory, TFile *outputRootFile, bool closureTest, bool save)
{
    if (leptonFlavor == "Muons" || leptonFlavor == "DMu") leptonFlavor = "DMu_";
    else if (leptonFlavor == "Electrons" || leptonFlavor == "DE") leptonFlavor = "DE_";
    else if (leptonFlavor == "Muon" || leptonFlavor == "SE") leptonFlavor = "SE_";
    else if (leptonFlavor == "Electron" || leptonFlavor == "SMu") leptonFlavor = "SMu_";

    TH1D *hSV = (TH1D*) hSV_->Clone();
    int NBins(hSV->GetNbinsX());

    string respTitle = hNormResp->GetTitle();
    string niceTitle = respTitle.substr(0, respTitle.find("#right"));
    niceTitle = niceTitle.substr(niceTitle.find("Response")+9);
    if (niceTitle.find("gen") != string::npos) niceTitle = niceTitle.substr(niceTitle.find("gen")+4);
    string SVTitle = "Singular Values of AC^{-1} for " + niceTitle;
    if (closureTest) SVTitle += " C.T.";
    string canNameSV = leptonFlavor + energy + "_" + variable + "_SV";
    if (closureTest) canNameSV += "_CT"; 

    hSV->SetTitle(SVTitle.c_str());
    hSV->SetLineWidth(3);
    hSV->SetLineColor(kBlack);
    hSV->GetXaxis()->SetTitle("Bin Numb.");
    hSV->GetXaxis()->SetTitleSize(0.04);
    hSV->GetXaxis()->SetTitleOffset(1.0);
    hSV->GetYaxis()->SetTitleSize(0.04);
    hSV->GetYaxis()->SetTitleOffset(1.15);
    hSV->GetXaxis()->CenterLabels();
    //hSV->GetXaxis()->SetLabelSize(0.02);
    hSV->GetXaxis()->SetLabelSize(0.1/sqrt(NBins));
    hSV->GetXaxis()->SetNdivisions(NBins,0,0);
    hSV->GetYaxis()->SetTitle("SV");
    TCanvas *canSV = new TCanvas(canNameSV.c_str(), canNameSV.c_str(), 700, 700);
    canSV->cd();
    canSV->SetLeftMargin(0.1);
    canSV->SetRightMargin(0.05);
    canSV->SetLogy();
    canSV->SetGrid();
    canSV->SetTicks();
    hSV->DrawCopy("TEXT60");
    string outputFileNamePNG = outputDirectory + canNameSV + ".pdf";
    if (save){
        canSV->Print(outputFileNamePNG.c_str());
        outputRootFile->cd();
        canSV->Write();
    }
}

void plotSVDUnfoldedHistograms(RooUnfoldResponse *response, TH1D *hData, TH1D *hBG[], int nBG, string leptonFlavor, string variable, string energy, bool logZ, bool decrease, string outputDirectory, TFile *outputRootFile, bool closureTest, bool save)
{
    if (leptonFlavor == "Muons" || leptonFlavor == "DMu") leptonFlavor = "DMu_";
    else if (leptonFlavor == "Electrons" || leptonFlavor == "DE") leptonFlavor = "DE_";
    else if (leptonFlavor == "Muon" || leptonFlavor == "SE") leptonFlavor = "SE_";
    else if (leptonFlavor == "Electron" || leptonFlavor == "SMu") leptonFlavor = "SMu_";

    TH2D *hResponse = (TH2D*) response->Hresponse();
    TH1D *MCreco    = (TH1D*) response->Hmeasured();
    string yTitle = MCreco->GetXaxis()->GetTitle();
    string respTitle = hResponse->GetTitle();
    string niceTitle = respTitle.substr(0, respTitle.find("#right"));
    niceTitle = niceTitle.substr(niceTitle.find("Response")+9);
    if (niceTitle.find("gen") != string::npos) niceTitle = niceTitle.substr(niceTitle.find("gen")+4);
    string title = "Unfolded Distributions for " + niceTitle + " vs. kTerm (SVD)";
    if (closureTest) title += " C.T.";
    string name = leptonFlavor + energy + "_" + variable + "_Unfolded_Distributions_SVD";
    if (closureTest) name += "_CT";


    TH1D *hMeas = (TH1D*) hData->Clone();
    double xFrom(hMeas->GetXaxis()->GetXmin());
    double xTo(hMeas->GetXaxis()->GetXmax());
    int nBins(hMeas->GetNbinsX());

    //string hName = leptonFlavor + " for " + variable + " vs. kterm (SVD)";
    TH2D *unfoldedDistributions = new TH2D("SVDUnfoldedDistributions", title.c_str(), nBins, xFrom, xTo, nBins, 0.5, nBins + 0.5);
    for (int i(0); i < nBG; i++) {
        if (i != 6) hMeas->Add(hBG[i], -1);
    }
    for (int kterm(1); kterm <= nBins; kterm++){ 
        RooUnfoldSvd unfoldSVD(response, hMeas, kterm, 1);
        unfoldSVD.SetVerbose(0);
        TH1D *hUnfolded = (TH1D*) unfoldSVD.Hreco();
        for (int bins(1); bins <= nBins; bins++){
            unfoldedDistributions->SetBinContent(bins, kterm, hUnfolded->GetBinContent(bins));
            unfoldedDistributions->SetBinError(bins, kterm, hUnfolded->GetBinError(bins));
        }
    }

    TCanvas *canUnfoldedDistributions = new TCanvas(name.c_str(), name.c_str(), 700, 700);
    canUnfoldedDistributions->cd();
    canUnfoldedDistributions->SetLeftMargin(0.12);
    canUnfoldedDistributions->SetRightMargin(0.04);
    if (decrease) canUnfoldedDistributions->SetPhi(-50);
    canUnfoldedDistributions->SetTheta(20);
    if (logZ) canUnfoldedDistributions->SetLogz();
    unfoldedDistributions->SetContour(90);
    unfoldedDistributions->GetZaxis()->SetTitle("#Events");
    unfoldedDistributions->GetZaxis()->SetTitleSize(0.04);
    unfoldedDistributions->GetZaxis()->SetTitleOffset(1.55);
    unfoldedDistributions->GetZaxis()->SetLabelSize(0.02);
    unfoldedDistributions->GetYaxis()->SetLabelSize(0.08/sqrt(nBins));
    unfoldedDistributions->GetYaxis()->SetNdivisions(nBins, 0, 0);
    unfoldedDistributions->GetYaxis()->SetTitle("kTerm");
    unfoldedDistributions->GetYaxis()->SetTitleOffset(1.5);
    unfoldedDistributions->GetXaxis()->SetTitle(yTitle.c_str());
    unfoldedDistributions->GetXaxis()->SetTitleOffset(1.5);
    unfoldedDistributions->GetXaxis()->SetLabelSize(0.025);
    unfoldedDistributions->DrawCopy("lego2");
    string outputFileNamePNG = outputDirectory + name.c_str() + ".pdf";
    if (save){
        canUnfoldedDistributions->Print(outputFileNamePNG.c_str());
        outputRootFile->cd();
        canUnfoldedDistributions->Write();
    }
}

/*
void plotChi2OfChange(RooUnfoldResponse *response, int kterm, TH1D *hData, TH1D *hBG[], int nBG, string leptonFlavor, string variable, string energy, string outputDirectory, TFile *outputRootFile, bool closureTest, bool save)
{
    if (leptonFlavor == "Muons" || leptonFlavor == "DMu") leptonFlavor = "DMu_";
    else if (leptonFlavor == "Electrons" || leptonFlavor == "DE") leptonFlavor = "DE_";
    else if (leptonFlavor == "Muon" || leptonFlavor == "SE") leptonFlavor = "SE_";
    else if (leptonFlavor == "Electron" || leptonFlavor == "SMu") leptonFlavor = "SMu_";

    TH2D *hResponse = (TH2D*) response->Hresponse();
    TH1D *hMeas = (TH1D*) hData->Clone();
    int nBins(hMeas->GetNbinsX()  );

    for (int i(0); i < nBG; i++){
        if (i != 6) hMeas->Add(hBG[i], -1);
    }
    RooUnfoldBayes unfoldBayesChi2(response, hMeas, nBins); 
    unfoldBayesChi2.SetVerbose(0);
    TH1D *tmp = (TH1D*) unfoldBayesChi2.Hreco();
    nBins = tmp->GetNbinsX();
    TH1D *Chi2OfChange = unfoldBayesChi2.Chi2OfChange;

    double hmax(Chi2OfChange->GetMaximum());
    double kCont(Chi2OfChange->GetBinContent(kterm));
    kCont *= 2;

    TArrow *arrow = new TArrow(kterm+0.5, hmax, kterm+0.5, kCont, 0.03, "|>");
    arrow->SetAngle(40);
    arrow->SetLineWidth(2);
    arrow->SetLineColor(kRed+2);
    arrow->SetFillColor(kRed+2);


    string respTitle = hResponse->GetTitle();
    string niceTitle = respTitle.substr(0, respTitle.find("#right"));
    niceTitle = niceTitle.substr(niceTitle.find("Response")+9);
    if (niceTitle.find("gen") != string::npos) niceTitle = niceTitle.substr(niceTitle.find("gen")+4);
    string chi2Title = "#chi^{2} of change for " + niceTitle;
    if (closureTest) chi2Title += " C.T.";
    string canNameChi2 = leptonFlavor + energy + "_" + variable + "_Chi2";
    if (closureTest) canNameChi2 += "_CT";

    Chi2OfChange->SetTitle(chi2Title.c_str());
    Chi2OfChange->SetLineWidth(3);
    Chi2OfChange->SetLineColor(kBlack);
    Chi2OfChange->GetXaxis()->SetTitle("Bin Numb.");
    Chi2OfChange->GetXaxis()->SetTitleSize(0.04);
    Chi2OfChange->GetXaxis()->SetTitleOffset(1.0);
    Chi2OfChange->GetYaxis()->SetTitleSize(0.04);
    Chi2OfChange->GetYaxis()->SetTitleOffset(1.15);
    Chi2OfChange->GetXaxis()->CenterLabels();
    Chi2OfChange->GetXaxis()->SetLabelSize(0.1/sqrt(nBins));
    Chi2OfChange->GetXaxis()->SetNdivisions(nBins, 0, 0);
    Chi2OfChange->GetYaxis()->SetTitle("#chi^{2}");
    TCanvas *canChi2 = new TCanvas(canNameChi2.c_str(), canNameChi2.c_str(), 700, 700);
    canChi2->cd();
    canChi2->SetLeftMargin(0.1);
    canChi2->SetRightMargin(0.05);
    canChi2->SetLogy();
    canChi2->SetGrid();
    canChi2->SetTicks();
    Chi2OfChange->DrawCopy("TEXT60");
    arrow->Draw();
    string outputFileNamePNG = outputDirectory + canNameChi2 + ".pdf";
    if (save){
        canChi2->Print(outputFileNamePNG.c_str());
        outputRootFile->cd();
        canChi2->Write();
    }
}
*/
 
void plotBayesUnfoldedHistograms(RooUnfoldResponse *response, TH1D *hData, TH1D *hBG[], int nBG, string leptonFlavor, string variable, string energy, bool logZ, bool decrease, string outputDirectory, TFile *outputRootFile, bool closureTest, bool save)
{
    if (leptonFlavor == "Muons" || leptonFlavor == "DMu") leptonFlavor = "DMu_";
    else if (leptonFlavor == "Electrons" || leptonFlavor == "DE") leptonFlavor = "DE_";
    else if (leptonFlavor == "Muon" || leptonFlavor == "SE") leptonFlavor = "SE_";
    else if (leptonFlavor == "Electron" || leptonFlavor == "SMu") leptonFlavor = "SMu_";

    TH2D *hResponse = (TH2D*) response->Hresponse();
    TH1D *MCreco    = (TH1D*) response->Hmeasured();
    string yTitle = MCreco->GetXaxis()->GetTitle();
    string respTitle = hResponse->GetTitle();
    string niceTitle = respTitle.substr(0, respTitle.find("#right"));
    niceTitle = niceTitle.substr(niceTitle.find("Response")+9);
    if (niceTitle.find("gen") != string::npos) niceTitle = niceTitle.substr(niceTitle.find("gen")+4);
    string title = "Unfolded Distributions for " + niceTitle + " vs. kTerm (Bayes)";
    if (closureTest) title += " C.T.";
    string name = leptonFlavor + energy + "_" + variable + "_Unfolded_Distributions_Bayes";
    if (closureTest) name += "_CT";


    TH1D *hMeas = (TH1D*) hData->Clone();
    double xFrom(hMeas->GetXaxis()->GetXmin());
    double xTo(hMeas->GetXaxis()->GetXmax());
    int nBins(hMeas->GetNbinsX());

    //string hName = leptonFlavor + " for " + variable + " vs. kterm (Bayes)";
    TH2D *unfoldedDistributions = new TH2D("BayesUnfoldedDistributions", title.c_str(), nBins, xFrom, xTo, nBins, 0.5, nBins + 0.5);
    for (int i(0); i < nBG; i++) {
        if (i != 6) hMeas->Add(hBG[i], -1);
    }

    for (int kterm(1); kterm <= nBins; kterm++){ 
        RooUnfoldBayes unfoldBayes(response, hMeas, kterm); 
        unfoldBayes.SetVerbose(0);
        TH1D *hUnfolded = (TH1D*) unfoldBayes.Hreco();
        for (int bins(1); bins <= nBins; bins++){
            unfoldedDistributions->SetBinContent(bins, kterm, hUnfolded->GetBinContent(bins));
            unfoldedDistributions->SetBinError(bins, kterm, hUnfolded->GetBinError(bins));
        }
    }

    TCanvas *canUnfoldedDistributions = new TCanvas(name.c_str(), name.c_str(), 700, 700);
    canUnfoldedDistributions->cd();
    canUnfoldedDistributions->SetLeftMargin(0.12);
    canUnfoldedDistributions->SetRightMargin(0.04);
    if (decrease) canUnfoldedDistributions->SetPhi(-50);
    canUnfoldedDistributions->SetTheta(20);
    if (logZ) canUnfoldedDistributions->SetLogz();
    unfoldedDistributions->SetContour(90);
    unfoldedDistributions->GetZaxis()->SetTitle("#Events");
    unfoldedDistributions->GetZaxis()->SetTitleSize(0.04);
    unfoldedDistributions->GetZaxis()->SetTitleOffset(1.55);
    unfoldedDistributions->GetZaxis()->SetLabelSize(0.02);
    unfoldedDistributions->GetYaxis()->SetLabelSize(0.08/sqrt(nBins));
    unfoldedDistributions->GetYaxis()->SetNdivisions(nBins, 0, 0);
    unfoldedDistributions->GetYaxis()->SetTitle("kTerm");
    unfoldedDistributions->GetYaxis()->SetTitleOffset(1.5);
    unfoldedDistributions->GetXaxis()->SetTitle(yTitle.c_str());
    unfoldedDistributions->GetXaxis()->SetTitleOffset(1.5);
    unfoldedDistributions->GetXaxis()->SetLabelSize(0.025);
    unfoldedDistributions->DrawCopy("lego2");
    string outputFileNamePNG = outputDirectory + name.c_str() + ".pdf";
    if (save){
        canUnfoldedDistributions->Print(outputFileNamePNG.c_str());
        outputRootFile->cd();
        canUnfoldedDistributions->Write();
    }
}

void plotSelectedMethod(string method, RooUnfoldResponse *response, TH1D *genMad, int kterm, TH1D *hData, TH1D *hBG[], int nBG, string leptonFlavor, string variable, bool logZ, bool decrease, string outputDirectory, TFile *outputRootFile, bool closureTest, bool save)
{
    if (leptonFlavor == "Muons" || leptonFlavor == "DMu") leptonFlavor = "DMu_";
    else if (leptonFlavor == "Electrons" || leptonFlavor == "DE") leptonFlavor = "DE_";
    else if (leptonFlavor == "Muon" || leptonFlavor == "SE") leptonFlavor = "SE_";
    else if (leptonFlavor == "Electron" || leptonFlavor == "SMu") leptonFlavor = "SMu_";

    string energy = getEnergy();
    if (kterm < 1) kterm = 2;
    ostringstream ktermStr; ktermStr << kterm;
    TH2D *hResponse = (TH2D*) response->Hresponse();
    TH1D *MCreco    = (TH1D*) response->Hmeasured();
    TH1D *gen       = (TH1D*) genMad->Clone();
    string yTitle = MCreco->GetXaxis()->GetTitle();
    string respTitle = hResponse->GetTitle();
    string niceTitle = respTitle.substr(0, respTitle.find("#right"));
    niceTitle = niceTitle.substr(niceTitle.find("Response")+9);
    if (niceTitle.find("gen") != string::npos) niceTitle = niceTitle.substr(niceTitle.find("gen")+4);
    string title = method + " Unfolded Distributions for " + niceTitle + " (kTerm = " + ktermStr.str() + " #pm 1)";
    if (closureTest) title += " C.T.";
    string name = leptonFlavor + energy + "_" + variable + "_Unfolded_Distribution_" + method;
    if (closureTest) name += "_CT";

    TH1D *hMeas = (TH1D*) hData->Clone();
    int nBins(hMeas->GetNbinsX());

    for (int i(0); i < nBG; i++) {
        if (i != 6) hMeas->Add(hBG[i], -1);
    }


    TH1D *hUnfoldedC = NULL, *hUnfoldedD = NULL, *hUnfoldedU = NULL;
    if (method == "SVD") {
        RooUnfoldSvd unfoldC(response, hMeas, kterm); 
        unfoldC.SetVerbose(0);
        hUnfoldedC = (TH1D*) unfoldC.Hreco();
        if (kterm > 1){
            RooUnfoldSvd unfoldD(response, hMeas, kterm - 1); 
            unfoldD.SetVerbose(0);
            hUnfoldedD = (TH1D*) unfoldD.Hreco();
        }
        else cout << "       Impossible to show kterm - 1" << endl;

        if (kterm < nBins){
            RooUnfoldSvd unfoldU(response, hMeas, kterm + 1); 
            unfoldU.SetVerbose(0);
            hUnfoldedU = (TH1D*) unfoldU.Hreco();
        }
        else cout << "       Impossible to show kterm + 1" << endl;

    }
    else if (method == "Bayes") {
        RooUnfoldBayes unfoldC(response, hMeas, kterm); 
        unfoldC.SetVerbose(0);
        hUnfoldedC = (TH1D*) unfoldC.Hreco();
        if (kterm > 1){
            RooUnfoldBayes unfoldD(response, hMeas, kterm - 1); 
            unfoldD.SetVerbose(0);
            hUnfoldedD = (TH1D*) unfoldD.Hreco();
        }
        else cout << "       Impossible to show kterm - 1" << endl;
        if (kterm < nBins){
            RooUnfoldBayes unfoldU(response, hMeas, kterm + 1); 
            unfoldU.SetVerbose(0);
            hUnfoldedU = (TH1D*) unfoldU.Hreco();
        }
        else cout << "       Impossible to show kterm + 1" << endl;
    }

    TLatex *cmsColl = new TLatex();
    cmsColl->SetTextSize(0.025);
    cmsColl->SetTextFont(42);
    cmsColl->SetLineWidth(2);
    cmsColl->SetTextColor(kBlack);
    cmsColl->SetNDC();
    cmsColl->SetTextAlign(11);

    TLatex *sqrtXTeV = new TLatex();
    sqrtXTeV->SetTextSize(0.025);
    sqrtXTeV->SetTextFont(42);
    sqrtXTeV->SetLineWidth(2);
    sqrtXTeV->SetTextColor(kBlack);
    sqrtXTeV->SetNDC();
    sqrtXTeV->SetTextAlign(21);

    TLatex *intLumi = new TLatex();
    intLumi->SetTextSize(0.022);
    intLumi->SetTextFont(42);
    intLumi->SetLineWidth(2);
    intLumi->SetTextColor(kBlack);
    intLumi->SetNDC();
    intLumi->SetTextAlign(31);


    TCanvas *canUnfoldedDistribution = new TCanvas(name.c_str(), name.c_str(), 700, 900);
    canUnfoldedDistribution->cd();
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1);
    pad1->SetTopMargin(0.11);
    pad1->SetBottomMargin(0);
    pad1->SetRightMargin(0.02);
    pad1->Draw();
    pad1->cd();
    if (logZ) pad1->SetLogy();
    gen->SetTitle(title.c_str());
    gen->GetYaxis()->SetTitleOffset(1.4);
    gen->SetLineColor(kBlack);
    gen->SetLineWidth(2);
    gen->SetFillStyle(3003);
    gen->SetFillColor(kGray+2);
    gen->DrawCopy("HIST");
    if (kterm > 1) {
        hUnfoldedD->SetTitle("");
        hUnfoldedD->SetMarkerStyle(32);
        hUnfoldedD->SetMarkerColor(kRed+1);
        hUnfoldedD->SetLineColor(kRed+1);
        hUnfoldedD->SetLineWidth(2);
        hUnfoldedD->DrawCopy("E1same");
    }  
    if (kterm < nBins) {
        hUnfoldedU->SetTitle("");
        hUnfoldedU->SetMarkerStyle(26);
        hUnfoldedU->SetMarkerColor(kGreen+2);
        hUnfoldedU->SetLineColor(kGreen+2);
        hUnfoldedU->SetLineWidth(2);
        hUnfoldedU->DrawCopy("E1same");
    }
    hUnfoldedC->SetMarkerStyle(24);
    hUnfoldedC->SetMarkerColor(kBlue+1);
    hUnfoldedC->SetLineColor(kBlue+1);
    hUnfoldedC->SetLineWidth(2);
    hUnfoldedC->DrawCopy("E1same");

    double xLowLeg(0.76), xHighLeg(0.95);
    if (!decrease){ 
        xLowLeg = 0.15;
        xHighLeg = 0.34;
    }

    TLegend *leg = new TLegend(xLowLeg, 0.63, xHighLeg, 0.89); 
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    if (!closureTest) leg->AddEntry(gen, "MC Gen.", "fl");
    else leg->AddEntry(gen, "MC Gen. Even", "fl");
    string legEntry = "kTerm = " + ktermStr.str();
    leg->AddEntry(hUnfoldedC, legEntry.c_str(), "pl");
    if (kterm < nBins) leg->AddEntry(hUnfoldedU, "kTerm + 1", "pl");
    if (kterm > 1) leg->AddEntry(hUnfoldedD, "kTerm - 1", "pl");
    leg->Draw();
    if (!closureTest){
        cmsColl->DrawLatex(0.11,0.905, "CMS Preliminary");
        if (energy == "7TeV") sqrtXTeV->DrawLatex(0.54,0.905, "#sqrt{s} = 7 TeV");
        if (energy == "8TeV") sqrtXTeV->DrawLatex(0.54,0.905, "#sqrt{s} = 8 TeV");
        if (energy == "7TeV") intLumi->DrawLatex(0.98,0.91, "#int L dt = 5.05 fb^{-1}");
        if (energy == "8TeV") intLumi->DrawLatex(0.98,0.91, "#int L dt = 19.6 fb^{-1}");
    }
    pad1->Draw();
    canUnfoldedDistribution->cd();

    TPad *pad2 = new TPad("pad2", "pad2", 0, 0, 1, 0.3);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.3);
    pad2->SetRightMargin(0.02);
    pad2->SetTicks();
    pad2->SetGridy();
    pad2->Draw();
    pad2->cd();
    hUnfoldedC->Divide(genMad);
    if (kterm > 1) hUnfoldedD->Divide(genMad);
    if (kterm < nBins) hUnfoldedU->Divide(genMad);

    hUnfoldedC->SetTitle("");
    hUnfoldedC->GetXaxis()->SetTickLength(0.03);
    hUnfoldedC->GetXaxis()->SetTitleSize(0.1);
    hUnfoldedC->GetXaxis()->SetLabelSize(0.1);
    hUnfoldedC->GetXaxis()->SetLabelOffset(0.018);
    hUnfoldedC->GetXaxis()->SetTitleOffset(1.2);

    hUnfoldedC->GetYaxis()->SetRangeUser(0.51, 1.49);
    hUnfoldedC->GetYaxis()->SetNdivisions(5,5,0);
    if (!closureTest) {
        hUnfoldedC->GetYaxis()->SetTitle("Gen / Unfolded");
        hUnfoldedC->GetYaxis()->SetTitleSize(0.1);
        hUnfoldedC->GetYaxis()->SetTitleOffset(0.5);
    }
    else {
        hUnfoldedC->GetYaxis()->SetTitle("Unf. Even / Gen. Even");
        hUnfoldedC->GetYaxis()->SetTitleSize(0.07);
        hUnfoldedC->GetYaxis()->SetTitleOffset(0.7);
    }
    hUnfoldedC->GetYaxis()->CenterTitle();
    hUnfoldedC->GetYaxis()->SetLabelSize(0.08);

    for (int j(1); j <= nBins; j++){
        double content(hUnfoldedC->GetBinContent(j));
        if (content > 0){
            hUnfoldedC->SetBinContent(j, 1./content);
        }
        content = (hUnfoldedD->GetBinContent(j));
        if (content > 0){
            hUnfoldedD->SetBinContent(j, 1./content);
        }
        content = (hUnfoldedU->GetBinContent(j));
        if (content > 0){
            hUnfoldedU->SetBinContent(j, 1./content);
        }
    }
    hUnfoldedC->DrawCopy("E1");
    if (kterm > 1) hUnfoldedD->DrawCopy("E1same");
    if (kterm < nBins) hUnfoldedU->DrawCopy("E1same");
    pad2->Draw();

    string outputFileNamePNG = outputDirectory + name.c_str() + ".pdf";
    if (save){
        canUnfoldedDistribution->Print(outputFileNamePNG.c_str());
        outputRootFile->cd();
        canUnfoldedDistribution->Write();
    }
}

void plotComparisonMadPowShe(string method, TH2D *hRespPow, TH2D *hRespShe, RooUnfoldResponse *response, int kterm, TH1D *hData, TH1D *hBG[], int nBG, string leptonFlavor, string variable, bool logZ, bool decrease, string outputDirectory, TFile *outputRootFile, bool closureTest, bool save)
{
    if (leptonFlavor == "Muons" || leptonFlavor == "DMu") leptonFlavor = "DMu_";
    else if (leptonFlavor == "Electrons" || leptonFlavor == "DE") leptonFlavor = "DE_";
    else if (leptonFlavor == "Muon" || leptonFlavor == "SE") leptonFlavor = "SE_";
    else if (leptonFlavor == "Electron" || leptonFlavor == "SMu") leptonFlavor = "SMu_";

    string energy = getEnergy();
    ostringstream ktermStr; ktermStr << kterm;
    TH2D *hResponse = (TH2D*) response->Hresponse();
    TH1D *MCreco    = (TH1D*) response->Hmeasured();
    string yTitle = MCreco->GetXaxis()->GetTitle();
    string respTitle = hResponse->GetTitle();
    string niceTitle = respTitle.substr(0, respTitle.find("#right"));
    niceTitle = niceTitle.substr(niceTitle.find("Response")+9);
    if (niceTitle.find("gen") != string::npos) niceTitle = niceTitle.substr(niceTitle.find("gen")+4);
    string title = method + " Comparison Mad, Pow, She for " + niceTitle + " (kTerm = " + ktermStr.str() + ")";
    if (closureTest) title += " C.T.";
    string name = leptonFlavor + energy + "_" + variable + "_Comparison_Mad_Pow_She_" + method;
    if (closureTest) name += "_CT";


    //-- get truth and reco from Madgraph response object
    TH1D *hTruth = (TH1D*) response->Htruth();
    TH1D *hReco  = (TH1D*) response->Hmeasured();

    //-- build new RooUnfoldResponse from Powheg TH2D and MadGraph TH1Ds
    RooUnfoldResponse *madPowResp = new RooUnfoldResponse(hReco, hTruth, hRespPow);
    madPowResp->UseOverflow();
    //-- build new RooUnfoldResponse from Sherpa TH2D and MadGraph TH1Ds
    RooUnfoldResponse *madSheResp = new RooUnfoldResponse(hReco, hTruth, hRespShe);
    madSheResp->UseOverflow();

    TH1D *hMeas = (TH1D*) hData->Clone();
    int nBins(hMeas->GetNbinsX());

    for (int i(0); i < nBG; i++){
        if (i != 6) hMeas->Add(hBG[i], -1);
    }

    TH1D *hUnfoldedMad = NULL, *hUnfoldedPow = NULL, *hUnfoldedShe = NULL;
    if (method == "SVD") {
        //-- unfold with full MadGraph object
        RooUnfoldSvd unfoldMad(response, hMeas, kterm); 
        unfoldMad.SetVerbose(0);
        hUnfoldedMad = (TH1D*) unfoldMad.Hreco();
        //-- unfold with mixed Mad Pow object
        RooUnfoldSvd unfoldPow(madPowResp, hMeas, kterm); 
        unfoldPow.SetVerbose(0);
        hUnfoldedPow = (TH1D*) unfoldPow.Hreco();
        //-- unfold with mixed Mad She object
        RooUnfoldSvd unfoldShe(madSheResp, hMeas, kterm); 
        unfoldShe.SetVerbose(0);
        hUnfoldedShe = (TH1D*) unfoldShe.Hreco();
    }
    else if (method == "Bayes") {
        //-- unfold with full MadGraph object
        RooUnfoldBayes unfoldMad(response, hMeas, kterm); 
        unfoldMad.SetVerbose(0);
        hUnfoldedMad = (TH1D*) unfoldMad.Hreco();
        //-- unfold with mixed Mad Pow object
        RooUnfoldBayes unfoldPow(madPowResp, hMeas, kterm); 
        unfoldPow.SetVerbose(0);
        hUnfoldedPow = (TH1D*) unfoldPow.Hreco();
        //-- unfold with mixed Mad She object
        RooUnfoldBayes unfoldShe(madSheResp, hMeas, kterm); 
        unfoldShe.SetVerbose(0);
        hUnfoldedShe = (TH1D*) unfoldShe.Hreco();
    }

    TLatex *cmsColl = new TLatex();
    cmsColl->SetTextSize(0.025);
    cmsColl->SetTextFont(42);
    cmsColl->SetLineWidth(2);
    cmsColl->SetTextColor(kBlack);
    cmsColl->SetNDC();
    cmsColl->SetTextAlign(11);

    TLatex *sqrtXTeV = new TLatex();
    sqrtXTeV->SetTextSize(0.025);
    sqrtXTeV->SetTextFont(42);
    sqrtXTeV->SetLineWidth(2);
    sqrtXTeV->SetTextColor(kBlack);
    sqrtXTeV->SetNDC();
    sqrtXTeV->SetTextAlign(21);

    TLatex *intLumi = new TLatex();
    intLumi->SetTextSize(0.022);
    intLumi->SetTextFont(42);
    intLumi->SetLineWidth(2);
    intLumi->SetTextColor(kBlack);
    intLumi->SetNDC();
    intLumi->SetTextAlign(31);

    TCanvas *canUnfoldedDistribution = new TCanvas(name.c_str(), name.c_str(), 700, 900);
    canUnfoldedDistribution->cd();
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1);
    pad1->SetTopMargin(0.11);
    pad1->SetBottomMargin(0);
    pad1->SetRightMargin(0.02);
    pad1->Draw();
    pad1->cd();
    if (logZ) pad1->SetLogy();
    hUnfoldedMad->SetTitle(title.c_str());
    hUnfoldedMad->GetYaxis()->SetTitleOffset(1.4);
    hUnfoldedMad->SetFillStyle(3003);
    hUnfoldedMad->SetFillColor(kBlue+1);
    hUnfoldedMad->SetMarkerStyle(24);
    hUnfoldedMad->SetMarkerColor(kBlue+1);
    hUnfoldedMad->SetLineColor(kBlue+1);
    hUnfoldedMad->SetLineWidth(2);
    hUnfoldedPow->SetTitle("");
    hUnfoldedPow->SetMarkerStyle(32);
    hUnfoldedPow->SetMarkerColor(kGreen+2);
    hUnfoldedPow->SetLineColor(kGreen+2);
    hUnfoldedPow->SetLineWidth(2);
    hUnfoldedShe->SetTitle("");
    hUnfoldedShe->SetMarkerStyle(26);
    hUnfoldedShe->SetMarkerColor(kRed+1);
    hUnfoldedShe->SetLineColor(kRed+1);
    hUnfoldedShe->SetLineWidth(2);
    hUnfoldedMad->DrawCopy("E1HIST");
    hUnfoldedPow->DrawCopy("E1same");
    hUnfoldedShe->DrawCopy("E1same");

    double xLowLeg(0.76), xHighLeg(0.95);
    if (!decrease){ 
        xLowLeg = 0.15;
        xHighLeg = 0.34;
    }

    TLegend *leg = new TLegend(xLowLeg, 0.63, xHighLeg, 0.89); 
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    if (!closureTest) leg->AddEntry(hUnfoldedMad, "Unf. MadGraph", "fpl");
    else leg->AddEntry(hUnfoldedMad, "MC Gen. Even", "fl");
    leg->AddEntry(hUnfoldedPow, "Unf. Powheg", "pl");
    leg->AddEntry(hUnfoldedShe, "Unf. Sherpa", "pl");
    leg->Draw();
    if (!closureTest){
        cmsColl->DrawLatex(0.11,0.905, "CMS Preliminary");
        if (energy == "7TeV") sqrtXTeV->DrawLatex(0.54,0.905, "#sqrt{s} = 7 TeV");
        if (energy == "8TeV") sqrtXTeV->DrawLatex(0.54,0.905, "#sqrt{s} = 8 TeV");
        if (energy == "7TeV") intLumi->DrawLatex(0.98,0.91, "#int L dt = 5.05 fb^{-1}");
        if (energy == "8TeV") intLumi->DrawLatex(0.98,0.91, "#int L dt = 19.6 fb^{-1}");
    }
    pad1->Draw();
    canUnfoldedDistribution->cd();

    TPad *pad2 = new TPad("pad2", "pad2", 0, 0, 1, 0.3);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.3);
    pad2->SetRightMargin(0.02);
    pad2->SetTicks();
    pad2->SetGridy();
    pad2->Draw();
    pad2->cd();
    hUnfoldedPow->Divide(hUnfoldedMad);
    hUnfoldedShe->Divide(hUnfoldedMad);

    hUnfoldedPow->GetXaxis()->SetTickLength(0.03);
    hUnfoldedPow->GetXaxis()->SetTitleSize(0.1);
    hUnfoldedPow->GetXaxis()->SetLabelSize(0.1);
    hUnfoldedPow->GetXaxis()->SetLabelOffset(0.018);
    hUnfoldedPow->GetXaxis()->SetTitleOffset(1.2);

    hUnfoldedPow->GetYaxis()->SetRangeUser(0.51, 1.49);
    hUnfoldedPow->GetYaxis()->SetNdivisions(5,5,0);
    if (!closureTest) {
        hUnfoldedPow->GetYaxis()->SetTitle("Ratio to Unf. MG");
        hUnfoldedPow->GetYaxis()->SetTitleSize(0.1);
        hUnfoldedPow->GetYaxis()->SetTitleOffset(0.5);
    }
    else {
        hUnfoldedPow->GetYaxis()->SetTitle("Ratio to Unf. MG");
        hUnfoldedPow->GetYaxis()->SetTitleSize(0.07);
        hUnfoldedPow->GetYaxis()->SetTitleOffset(0.7);
    }
    hUnfoldedPow->GetYaxis()->CenterTitle();
    hUnfoldedPow->GetYaxis()->SetLabelSize(0.08);

    for (int j(1); j <= nBins; j++){
        double content(hUnfoldedPow->GetBinContent(j));
        if (content > 0){
            hUnfoldedPow->SetBinContent(j, 1./content);
        }
        content = (hUnfoldedShe->GetBinContent(j));
        if (content > 0){
            hUnfoldedShe->SetBinContent(j, 1./content);
        }
    }
    hUnfoldedPow->DrawCopy("E1");
    hUnfoldedShe->DrawCopy("E1same");
    pad2->Draw();

    string outputFileNamePNG = outputDirectory + name.c_str() + ".pdf";
    if (save){
        canUnfoldedDistribution->Print(outputFileNamePNG.c_str());
        outputRootFile->cd();
        canUnfoldedDistribution->Write();
    }

}



void plotSVDvsBayesvsBBB(RooUnfoldResponse *response, TH1D *genMad, int SVDkterm, int Bayeskterm, TH1D *hData, TH1D *hBG[], int nBG, string leptonFlavor, string variable, bool logZ, bool decrease, string outputDirectory, TFile *outputRootFile, bool closureTest, bool save)
{
    if (leptonFlavor == "Muons" || leptonFlavor == "DMu") leptonFlavor = "DMu_";
    else if (leptonFlavor == "Electrons" || leptonFlavor == "DE") leptonFlavor = "DE_";
    else if (leptonFlavor == "Muon" || leptonFlavor == "SE") leptonFlavor = "SE_";
    else if (leptonFlavor == "Electron" || leptonFlavor == "SMu") leptonFlavor = "SMu_";

    string energy = getEnergy();
    if (SVDkterm   < 1) SVDkterm = 2;
    if (Bayeskterm < 1) Bayeskterm = 2;
    ostringstream SVDktermStr; SVDktermStr << SVDkterm;
    ostringstream BayesktermStr; BayesktermStr << Bayeskterm;
    TH2D *hResponse = (TH2D*) response->Hresponse();
    TH1D *MCreco    = (TH1D*) response->Hmeasured();
    TH1D *gen       = (TH1D*) genMad->Clone();
    string yTitle = MCreco->GetXaxis()->GetTitle();
    string respTitle = hResponse->GetTitle();
    string niceTitle = respTitle.substr(0, respTitle.find("#right"));
    niceTitle = niceTitle.substr(niceTitle.find("Response")+9);
    if (niceTitle.find("gen") != string::npos) niceTitle = niceTitle.substr(niceTitle.find("gen")+4);
    string title = "Unfolding Comparison for " + niceTitle;
    if (closureTest) title += " C.T.";
    string name = leptonFlavor + energy + "_" + variable + "_Unfolding_Comparison";
    if (closureTest) name += "_CT";

    TH1D *hMeas = (TH1D*) hData->Clone();
    int nBins(hMeas->GetNbinsX());

    //string hName = leptonFlavor + " Unfolding Comparison for " + variable;
    for (int i(0); i < nBG; i++){
        if (i != 6) hMeas->Add(hBG[i], -1);
    }

    //-- SVD method
    RooUnfoldSvd unfoldSVD(response, hMeas, SVDkterm); 
    unfoldSVD.SetVerbose(0);
    TH1D *hUnfoldedSVD = (TH1D*) unfoldSVD.Hreco(RooUnfold::kCovariance);
    //-- Bayes method
    RooUnfoldBayes unfoldBayes(response, hMeas, Bayeskterm); 
    unfoldBayes.SetVerbose(0);
    TH1D *hUnfoldedBayes = (TH1D*) unfoldBayes.Hreco(RooUnfold::kCovariance);
    //-- Bin by Bin method
    RooUnfoldBinByBin unfoldBBB(response, hMeas); 
    unfoldBBB.SetVerbose(0);
    TH1D *hUnfoldedBBB = (TH1D*) unfoldBBB.Hreco(RooUnfold::kCovariance);
    //-- Full Inversion method
    RooUnfoldInvert unfoldInvert(response,hMeas);
    unfoldInvert.SetVerbose(0);
    TH1D *hUnfoldedInvert = (TH1D*) unfoldInvert.Hreco(RooUnfold::kCovariance);


    TLatex *cmsColl = new TLatex();
    cmsColl->SetTextSize(0.025);
    cmsColl->SetTextFont(42);
    cmsColl->SetLineWidth(2);
    cmsColl->SetTextColor(kBlack);
    cmsColl->SetNDC();
    cmsColl->SetTextAlign(11);

    TLatex *sqrtXTeV = new TLatex();
    sqrtXTeV->SetTextSize(0.025);
    sqrtXTeV->SetTextFont(42);
    sqrtXTeV->SetLineWidth(2);
    sqrtXTeV->SetTextColor(kBlack);
    sqrtXTeV->SetNDC();
    sqrtXTeV->SetTextAlign(21);

    TLatex *intLumi = new TLatex();
    intLumi->SetTextSize(0.022);
    intLumi->SetTextFont(42);
    intLumi->SetLineWidth(2);
    intLumi->SetTextColor(kBlack);
    intLumi->SetNDC();
    intLumi->SetTextAlign(31);

    TCanvas *canUnfoldedDistribution = new TCanvas(name.c_str(), name.c_str(), 700, 900);
    canUnfoldedDistribution->cd();
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1);
    pad1->SetTopMargin(0.11);
    pad1->SetBottomMargin(0);
    pad1->SetRightMargin(0.02);
    pad1->Draw();
    pad1->cd();
    if (logZ) pad1->SetLogy();
    gen->SetTitle(title.c_str());
    gen->GetYaxis()->SetTitleOffset(1.4);
    gen->SetLineColor(kBlack);
    gen->SetLineWidth(2);
    gen->SetFillStyle(3003);
    gen->SetFillColor(kGray+2);
    hUnfoldedSVD->SetMarkerStyle(24);
    hUnfoldedSVD->SetMarkerColor(kBlue+1);
    hUnfoldedSVD->SetLineColor(kBlue+1);
    hUnfoldedSVD->SetLineWidth(2);
    hUnfoldedBayes->SetTitle("");
    hUnfoldedBayes->SetMarkerStyle(24);
    hUnfoldedBayes->SetMarkerColor(kRed+1);
    hUnfoldedBayes->SetLineColor(kRed+1);
    hUnfoldedBayes->SetLineWidth(2);
    hUnfoldedBBB->SetTitle("");
    hUnfoldedBBB->SetMarkerStyle(24);
    hUnfoldedBBB->SetMarkerColor(kGreen+2);
    hUnfoldedBBB->SetLineColor(kGreen+2);
    hUnfoldedBBB->SetLineWidth(2);
    hUnfoldedInvert->SetTitle("");
    hUnfoldedInvert->SetMarkerStyle(24);
    hUnfoldedInvert->SetMarkerColor(kOrange-1);
    hUnfoldedInvert->SetLineColor(kOrange-1);
    hUnfoldedInvert->SetLineWidth(2);
    gen->DrawCopy("HIST");
    hUnfoldedInvert->DrawCopy("E1same");
    hUnfoldedBBB->DrawCopy("E1same");
    hUnfoldedBayes->DrawCopy("E1same");
    hUnfoldedSVD->DrawCopy("E1same");
    double xLowLeg(0.76), xHighLeg(0.95);
    if (!decrease){ 
        xLowLeg = 0.12;
        xHighLeg = 0.29;
    }

    TLegend *leg = new TLegend(xLowLeg, 0.60, xHighLeg, 0.89); 
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    if (!closureTest) leg->AddEntry(gen, "MC Gen.", "fl");
    else leg->AddEntry(gen, "MC Gen. Even", "fl");
    leg->AddEntry(hUnfoldedInvert, "Full Invers.", "pl");
    leg->AddEntry(hUnfoldedBBB, "BinByBin", "pl");
    string legEntry = "Bayes (" + BayesktermStr.str() + ")";
    leg->AddEntry(hUnfoldedBayes, legEntry.c_str(), "pl");
    legEntry = "SVD (" + SVDktermStr.str() + ")";
    leg->AddEntry(hUnfoldedSVD, legEntry.c_str(), "pl");
    leg->Draw();
    if (!closureTest) {
        cmsColl->DrawLatex(0.11,0.905, "CMS Preliminary");
        if (energy == "7TeV") sqrtXTeV->DrawLatex(0.54,0.905, "#sqrt{s} = 7 TeV");
        if (energy == "8TeV") sqrtXTeV->DrawLatex(0.54,0.905, "#sqrt{s} = 8 TeV");
        if (energy == "7TeV") intLumi->DrawLatex(0.98,0.91, "#int L dt = 5.05 fb^{-1}");
        if (energy == "8TeV") intLumi->DrawLatex(0.98,0.91, "#int L dt = 19.6 fb^{-1}");
    }

    pad1->Draw();
    canUnfoldedDistribution->cd();

    TPad *pad2 = new TPad("pad2", "pad2", 0, 0, 1, 0.3);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.3);
    pad2->SetRightMargin(0.02);
    pad2->SetTicks();
    pad2->SetGridy();
    pad2->Draw();
    pad2->cd();
    hUnfoldedSVD->Divide(genMad);
    hUnfoldedBayes->Divide(genMad);
    hUnfoldedBBB->Divide(genMad);
    hUnfoldedInvert->Divide(genMad);

    hUnfoldedInvert->GetXaxis()->SetTickLength(0.03);
    hUnfoldedInvert->GetXaxis()->SetTitleSize(0.1);
    hUnfoldedInvert->GetXaxis()->SetLabelSize(0.1);
    hUnfoldedInvert->GetXaxis()->SetLabelOffset(0.018);
    hUnfoldedInvert->GetXaxis()->SetTitleOffset(1.2);

    hUnfoldedInvert->GetYaxis()->SetRangeUser(0.51, 1.49);
    hUnfoldedInvert->GetYaxis()->SetNdivisions(5,5,0);
    if (!closureTest) {
        hUnfoldedInvert->GetYaxis()->SetTitle("Gen / Unfolded");
        hUnfoldedInvert->GetYaxis()->SetTitleSize(0.1);
        hUnfoldedInvert->GetYaxis()->SetTitleOffset(0.5);
    }
    else {
        hUnfoldedInvert->GetYaxis()->SetTitle("Unf. Even / Gen. Even");
        hUnfoldedInvert->GetYaxis()->SetTitleSize(0.07);
        hUnfoldedInvert->GetYaxis()->SetTitleOffset(0.7);
    }
    hUnfoldedInvert->GetYaxis()->CenterTitle();
    hUnfoldedInvert->GetYaxis()->SetLabelSize(0.08);

    for (int j(1); j <= nBins; j++){
        double content(hUnfoldedInvert->GetBinContent(j));
        if (content > 0){
            hUnfoldedInvert->SetBinContent(j, 1./content);
        }
        content = (hUnfoldedBBB->GetBinContent(j));
        if (content > 0){
            hUnfoldedBBB->SetBinContent(j, 1./content);
        }
        content = (hUnfoldedBayes->GetBinContent(j));
        if (content > 0){
            hUnfoldedBayes->SetBinContent(j, 1./content);
        }
        content = (hUnfoldedSVD->GetBinContent(j));
        if (content > 0){
            hUnfoldedSVD->SetBinContent(j, 1./content);
        }
    }
    hUnfoldedInvert->DrawCopy("E1");
    hUnfoldedBBB->DrawCopy("E1same");
    hUnfoldedBayes->DrawCopy("E1same");
    hUnfoldedSVD->DrawCopy("E1same");
    pad2->Draw();

    string outputFileNamePNG = outputDirectory + name.c_str() + ".pdf";
    if (save){
        canUnfoldedDistribution->Print(outputFileNamePNG.c_str());
        outputRootFile->cd();
        canUnfoldedDistribution->Write();
    }
}
