#include <iostream>
#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <sstream>
#include <RooUnfoldResponse.h>

void runMergeTop(string lepSelection = "SMu", int systematics =0  , int jetPtCutMin = 30 , int doQCD = 0 );

void MergeTop(){
/////    runMergeTop("DMu",0,20,0);
//    runMergeTop("SMu",0,20,0);
    //runMergeTop("DMu",1,20,0);
    //runMergeTop("DMu",-1,20,0);
    //runMergeTop("DMu",3,20,0);
    //runMergeTop("DMu",-3,20,0);	        
/////    runMergeTop("DMu",0,30,0);
    runMergeTop("SMu",0,30,0);
    //runMergeTop("DMu",1,30,0);
    //runMergeTop("DMu",-1,30,0);
    //runMergeTop("DMu",3,30,0);
    //runMergeTop("DMu",-3,30,0);
}

void runMergeTop(string lepSelection, int systematics, int jetPtCutMin, int doQCD)
{
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();

    cout << __FILE__ << endl;
    ostringstream strJetPtCutMin; strJetPtCutMin << jetPtCutMin;
    ostringstream doQCDStr;     doQCDStr << doQCD ;
    string syst;
    if (systematics == 0) syst = "Syst_0_";
    else if (systematics == 1) syst = "Syst_1_Up_"; 
    else if (systematics == -1) syst = "Syst_1_Down_"; 
    else if (systematics == 3) syst = "Syst_3_Up_"; 
    else if (systematics == -3) syst = "Syst_3_Down_"; 


//SMu_8TeV_T_s_channel_dR_5311_EffiCorr_1_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth.root

    string str1 = "HistoFiles/"+ lepSelection +  "_8TeV_T_s_channel_dR_5311_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth.root";
    string str2 = "HistoFiles/"+ lepSelection +  "_8TeV_T_t_channel_dR_5311_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth.root";
    string str3 = "HistoFiles/"+ lepSelection +  "_8TeV_T_tW_channel_dR_5311_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth.root";
    string str4 = "HistoFiles/"+ lepSelection +  "_8TeV_Tbar_s_channel_dR_5311_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth.root";
    string str5 = "HistoFiles/"+ lepSelection +  "_8TeV_Tbar_t_channel_dR_5311_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth.root";
    string str6 = "HistoFiles/"+ lepSelection +  "_8TeV_Tbar_tW_channel_dR_5311_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth.root";
    string strf = "HistoFiles/"+ lepSelection +  "_8TeV_Top_dR_5311_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth.root";

    /// DY
    int nDYfiles = 3 ;

    string sstrDY[10];
//    sstrDY[0] = "HistoFiles/"+ lepSelection +  "_8TeV_DYJets10to50_dR_5311_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth_InvMass_SS.root";
//   sstrDY[1] = "HistoFiles/"+ lepSelection +  "_8TeV_DYJets_MIX_UNFOLDING_dR_5311_Inf3_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth_InvMass_SS.root";
//    sstrDY[2] = "HistoFiles/"+ lepSelection +  "_8TeV_DYJets10toInf3_dR_5311_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth_InvMass_SS.root";

 sstrDY[0] = "HistoFiles/"+ lepSelection +  "_8TeV_DYJets10to50_dR_5311_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth.root";
sstrDY[1] = "HistoFiles/"+ lepSelection +  "_8TeV_DYJets_MIX_UNFOLDING_dR_5311_Inf3_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth.root";
 sstrDY[2] = "HistoFiles/"+ lepSelection +  "_8TeV_DYJets10toInf3_dR_5311_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth.root";


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
    if (lepSelection == "SMu"){
        /// DY
        //   int nDYfiles = 3 ;
        //     string sstrDY[10];
        //     sstrDY[0] = "HistoFiles/"+ lepSelection +  "_8TeV_DYJets10to50_dR_5311_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth_InvMass_SS.root";
        //     sstrDY[1] = "HistoFiles/"+ lepSelection +  "_8TeV_DYJets_MIX_UNFOLDING_dR_5311_Inf3_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth_InvMass_SS.root";
        //     sstrDY[2] = "HistoFiles/"+ lepSelection +  "_8TeV_DYJets10toInf3_dR_5311_EffiCorr_1_TrigCorr_1" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth_InvMass_SS.root"; 
        TFile *fDY[10];
        for ( int i = 0 ; i < nDYfiles ; i++){
            if ( i == nDYfiles - 1 )  fDY[i] =  new TFile(sstrDY[i].c_str(), "recreate");
            else fDY[i] =  new TFile(sstrDY[i].c_str(), "read");
            cout << sstrDY[i] << endl;

        }

        nHist = fDY[0]->GetListOfKeys()->GetEntries();
        for (int i(0); i < nHist; i++){
            string hName = fDY[0]->GetListOfKeys()->At(i)->GetName();
            cout << i <<"  " << hName << nHist << endl;
            TH1D* hSum; TH1D* hDY[10];
            for ( int j = 0 ; j < nDYfiles -1 ; j++){
                hDY[j] = (TH1D*) fDY[j]->Get(hName.c_str());
                if ( j == 0 ) hSum = (TH1D*) hDY[j]->Clone();
                else hSum->Add(hDY[j]);
            }
            fDY[nDYfiles -1]->cd();
            hSum->Write();
        }
        for ( int i = 0 ; i < nDYfiles ; i++){
            fDY[i] ->Close();
        }

    } /// end if DY
  

}
