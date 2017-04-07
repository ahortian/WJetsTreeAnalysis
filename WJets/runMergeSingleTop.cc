#include <iostream>
#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <sstream>
#include <RooUnfoldResponse.h>

void MergeSingleTop(string lepSelection = "SMu", int systematics = 0, int jetPtCutMin = 30, int doQCD = 0, int doBJets = -1);

void runMergeSingleTop(){
    
    //MergeSingleTop("SMu", 0, 30, 0, -1);
    //MergeSingleTop("SMu", 0, 30, 0,  0);
    MergeSingleTop("SMu", 0, 30, 0,  2);
    
    //MergeSingleTop("SMu",  1, 30, 0, -1);
    //MergeSingleTop("SMu", -1, 30, 0, -1);
    //MergeSingleTop("SMu",  3, 30, 0, -1);
    //MergeSingleTop("SMu", -3, 30, 0, -1);
    
    
    //--- clean the *_cc.d and *_cc.so files ---
    string srcdir = "Sources/";
    string cmd = "if ls *_cc.d &> .ls_tmp.list; then rm *_cc.d; fi";
    system(cmd.c_str());
    cmd = "if ls *_cc.so &> .ls_tmp.list; then rm *_cc.so; fi";
    system(cmd.c_str());
    cmd = "if ls " + srcdir + "*_cc.d &> .ls_tmp.list; then rm " + srcdir + "*_cc.d; fi";
    system(cmd.c_str());
    cmd = "if ls " + srcdir + "*_cc.so &> .ls_tmp.list; then rm " + srcdir + "*_cc.so; fi";
    system(cmd.c_str());
    system("rm .ls_tmp.list");
    
    
}

void MergeSingleTop(string lepSelection, int systematics, int jetPtCutMin, int doQCD, int doBJets)
{
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();
    
    cout << __FILE__ << endl;
    
    string histDir = "HistoFiles/";
    
    // We always use EffiCorr=1 and TrigCorr=1. These means corrections are applied to MC to match data.
    string dRstr = "_dR_5311_EffiCorr_1_TrigCorr_1";
    
    // For single top bg systematics, we have syst 1, -1, 3, -3.
    string syst;
    if (systematics == 0) syst = "_Syst_0";
    else if (systematics == 1) syst = "_Syst_1_Up";
    else if (systematics == -1) syst = "_Syst_1_Down";
    else if (systematics == 3) syst = "_Syst_3_Up";
    else if (systematics == -3) syst = "_Syst_3_Down";
    
    ostringstream strJetPtCutMin;
    strJetPtCutMin << "_JetPtMin_" << jetPtCutMin;
    
    string bjets;
    if (doBJets == 0) bjets = "";
    else if (doBJets > 0) bjets = "_BJets";
    else if (doBJets < 0) bjets = "_BVeto";
    
    ostringstream doQCDStr; doQCDStr << doQCD ;
    string qcd;
    if (doQCD == 0) qcd = "";
    else if (doQCD > 0) qcd = "_QCD" + doQCDStr.str();
    
    string str1 = histDir + lepSelection + "_8TeV_T_s_channel"     + dRstr + syst + strJetPtCutMin.str() + "_VarWidth" + bjets + qcd + ".root";
    string str2 = histDir + lepSelection + "_8TeV_T_t_channel"     + dRstr + syst + strJetPtCutMin.str() + "_VarWidth" + bjets + qcd + ".root";
    string str3 = histDir + lepSelection + "_8TeV_T_tW_channel"    + dRstr + syst + strJetPtCutMin.str() + "_VarWidth" + bjets + qcd + ".root";
    string str4 = histDir + lepSelection + "_8TeV_Tbar_s_channel"  + dRstr + syst + strJetPtCutMin.str() + "_VarWidth" + bjets + qcd + ".root";
    string str5 = histDir + lepSelection + "_8TeV_Tbar_t_channel"  + dRstr + syst + strJetPtCutMin.str() + "_VarWidth" + bjets + qcd + ".root";
    string str6 = histDir + lepSelection + "_8TeV_Tbar_tW_channel" + dRstr + syst + strJetPtCutMin.str() + "_VarWidth" + bjets + qcd + ".root";
    string strf = histDir + lepSelection + "_8TeV_Top"             + dRstr + syst + strJetPtCutMin.str() + "_VarWidth" + bjets + qcd + ".root";
    
    cout << strf<< endl;
    TFile *f1 = new TFile(str1.c_str());
    TFile *f2 = new TFile(str2.c_str());
    TFile *f3 = new TFile(str3.c_str());
    TFile *f4 = new TFile(str4.c_str());
    TFile *f5 = new TFile(str5.c_str());
    TFile *f6 = new TFile(str6.c_str());
    TFile *ff = new TFile(strf.c_str(), "RECREATE");
    
    int nHist = f1->GetListOfKeys()->GetEntries();
    for (int i(0); i < nHist; i++){
        string hName = f1->GetListOfKeys()->At(i)->GetName();
        
        //        cout << hName << endl;
        if (hName.find("response") != string::npos){
            /*
            RooUnfoldResponse *r1 = (RooUnfoldResponse*) f1->Get(hName.c_str());
            RooUnfoldResponse *r2 = (RooUnfoldResponse*) f2->Get(hName.c_str());
            RooUnfoldResponse *r3 = (RooUnfoldResponse*) f3->Get(hName.c_str());
            RooUnfoldResponse *r4 = (RooUnfoldResponse*) f4->Get(hName.c_str());
            RooUnfoldResponse *r5 = (RooUnfoldResponse*) f5->Get(hName.c_str());
            RooUnfoldResponse *r6 = (RooUnfoldResponse*) f6->Get(hName.c_str());
            
            RooUnfoldResponse *rSum = (RooUnfoldResponse*) r1->Clone();
            rSum->Add(*r2);
            rSum->Add(*r3);
            rSum->Add(*r4);
            rSum->Add(*r5);
            rSum->Add(*r6);
            ff->cd();
            rSum->Write(hName.c_str());
            */
            
        }
        else {
            TH1D *h1 = (TH1D*) f1->Get(hName.c_str());
            TH1D *h2 = (TH1D*) f2->Get(hName.c_str());
            TH1D *h3 = (TH1D*) f3->Get(hName.c_str());
            TH1D *h4 = (TH1D*) f4->Get(hName.c_str());
            TH1D *h5 = (TH1D*) f5->Get(hName.c_str());
            TH1D *h6 = (TH1D*) f6->Get(hName.c_str());
            
            TH1D *hSum = (TH1D*) h1->Clone();
            hSum->Add(h2);
            hSum->Add(h3);
            hSum->Add(h4);
            hSum->Add(h5);
            hSum->Add(h6);
            ff->cd();
            hSum->Write();
        }
    }
    f1->Close();
    f2->Close();
    f3->Close();
    f4->Close();
    f5->Close();
    f6->Close();
    ff->Close();
    
    
    //// merge DY files
    int nDYfiles = 3 ;
    string sstrDY[10];
    sstrDY[0] = histDir + lepSelection + "_8TeV_DYJets10to50" + dRstr + syst + strJetPtCutMin.str() + "_VarWidth" + bjets + qcd + ".root";
    sstrDY[1] = histDir + lepSelection + "_8TeV_DYJets_MIX_UNFOLDING_dR_5311_Inf3_EffiCorr_1_TrigCorr_1" + syst + strJetPtCutMin.str() + "_VarWidth" + bjets + qcd + ".root";
    sstrDY[2] = histDir + lepSelection + "_8TeV_DYJets10toInf3" + dRstr + syst + strJetPtCutMin.str() + "_VarWidth" + bjets + qcd + ".root";
    
    // merge DY only if we do SMu
    if (lepSelection == "SMu"){

        TFile *fDY[10];
        for ( int i = 0 ; i < nDYfiles ; i++){
            if ( i == nDYfiles - 1 )  fDY[i] =  new TFile(sstrDY[i].c_str(), "recreate");
            else fDY[i] =  new TFile(sstrDY[i].c_str(), "read");
            cout << sstrDY[i] << endl;
            
        }
        
        nHist = fDY[0]->GetListOfKeys()->GetEntries();
        for (int i(0); i < nHist; i++){
            string hName = fDY[0]->GetListOfKeys()->At(i)->GetName();
            cout << i << "  " << hName <<  "  " << nHist << endl;
            TH1D* hSumDY; TH1D* hDY[10];
            for ( int j = 0 ; j < nDYfiles -1 ; j++){
                hDY[j] = (TH1D*) fDY[j]->Get(hName.c_str());
                if ( j == 0 ) hSumDY = (TH1D*) hDY[j]->Clone();
                else hSumDY->Add(hDY[j]);
            }
            fDY[nDYfiles -1]->cd();
            hSumDY->Write();
        }
        for ( int i = 0 ; i < nDYfiles ; i++){
            fDY[i] ->Close();
        }
        
    } /// end if to merge DY files
    
}

