// History
//---- 2015_05_17
// Also merge top and DY (and generate outputs) for the regions of QCD1,2,3.

#include <iostream>
#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <sstream>
#include <RooUnfoldResponse.h>

void runMergeVV_BVeto(string lepSelection = "DE", int systematics =0  , int jetPtCutMin = 30 , int doQCD = 0 );

void MergeVV_BVeto(){
    
    runMergeVV_BVeto("SMu",0,30,0);
    //runMergeTop_BVeto("SMu",0,30,1);
    //runMergeTop_BVeto("SMu",0,30,2);
    //runMergeTop_BVeto("SMu",0,30,3);

    //runMergeTop("DE",0,20,0);
    //runMergeTop("DE",1,20,0);
    //runMergeTop("DE",-1,20,0);
    //runMergeTop("DE",3,20,0);
    //runMergeTop("DE",-3,20,0);
    

}

void runMergeVV_BVeto(string lepSelection, int systematics, int jetPtCutMin, int doQCD)
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
    cout << doQCD << endl;
    //string str1, str2, str3, str4, str5, str6, strf;
    string str1, str2, str3, strf;
    int nDYfiles = 2 ;
    string sstrDY[10];
    
    if (doQCD == 0) {
        str1 = "HistoFiles/"+ lepSelection +  "_13TeV_WW_dR_5311_List_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth_BVeto.root";
        str2 = "HistoFiles/"+ lepSelection +  "_13TeV_WZ_dR_5311_List_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth_BVeto.root";
        str3 = "HistoFiles/"+ lepSelection +  "_13TeV_ZZ_dR_5311_List_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth_BVeto.root";
        //str4 = "HistoFiles/"+ lepSelection +  "_13TeV_ST_tW_antitop_channel_dR_5311_List_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth_BVeto.root";
       // str4 = "HistoFiles/"+ lepSelection +  "_13TeV_Tbar_s_channel_dR_5311_List_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth_BVeto.root";
       // str5 = "HistoFiles/"+ lepSelection +  "_13TeV_Tbar_t_channel_dR_5311_List_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth_BVeto.root";
       // str6 = "HistoFiles/"+ lepSelection +  "_13TeV_Tbar_tW_channel_dR_5311_List_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth_BVeto.root";
        strf = "HistoFiles/"+ lepSelection +  "_13TeV_VV_dR_5311_List_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth_BVeto.root";
        
        //--- DY Files
        //sstrDY[0] = "HistoFiles/"+ lepSelection +  "_13TeV_DYJets10to50_dR_5311_List_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth_BVeto.root";
        
        sstrDY[0] = "HistoFiles/"+ lepSelection +  "_13TeV_DYJets50toInf_dR_5311_List_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth_BVeto.root";
        
        sstrDY[1] = "HistoFiles/"+ lepSelection +  "_13TeV_DYJets10toInf3_dR_5311_List_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth_BVeto.root";
    }
    if (doQCD > 0){
        str1 = "HistoFiles/"+ lepSelection +  "_13TeV_WW_dR_5311_List_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth_BVeto_QCD" + doQCDStr.str() + ".root";
        str2 = "HistoFiles/"+ lepSelection +  "_13TeV_WZ_dR_5311_List_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth_BVeto_QCD" + doQCDStr.str() + ".root";
        str3 = "HistoFiles/"+ lepSelection +  "_13TeV_ZZ_dR_5311_List_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth_BVeto_QCD" + doQCDStr.str() + ".root";
        //str4 = "HistoFiles/"+ lepSelection +  "_13TeV_ST_tW_antitop_channel_dR_5311_List_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth_BVeto_QCD" + doQCDStr.str() + ".root";
       // str4 = "HistoFiles/"+ lepSelection +  "_13TeV_Tbar_s_channel_dR_5311_List_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth_BVeto_QCD" + doQCDStr.str() + ".root";
       // str5 = "HistoFiles/"+ lepSelection +  "_13TeV_Tbar_t_channel_dR_5311_List_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth_BVeto_QCD" + doQCDStr.str() + ".root";
       // str6 = "HistoFiles/"+ lepSelection +  "_13TeV_Tbar_tW_channel_dR_5311_List_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth_BVeto_QCD" + doQCDStr.str() + ".root";
        strf = "HistoFiles/"+ lepSelection +  "_13TeV_VV_dR_5311_List_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth_BVeto_QCD" + doQCDStr.str() + ".root";
        
        //--- DY Files
        //sstrDY[0] = "HistoFiles/"+ lepSelection +  "_13TeV_DYJets10to50_dR_5311_List_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth_BVeto_QCD" + doQCDStr.str() + ".root";
        
        sstrDY[0] = "HistoFiles/"+ lepSelection +  "_13TeV_DYJets50toInf_dR_5311_List_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str()+ "_VarWidth_BVeto_QCD" + doQCDStr.str() + ".root";
        
        sstrDY[1] = "HistoFiles/"+ lepSelection +  "_13TeV_DYJets10toInf3_dR_5311_List_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth_BVeto_QCD" + doQCDStr.str() + ".root";
    }

    cout << "Output file: " << strf << endl;
    TFile *f1 = new TFile(str1.c_str(), "read");
    TFile *f2 = new TFile(str2.c_str(), "read");
    TFile *f3 = new TFile(str3.c_str(), "read");
    //TFile *f4 = new TFile(str4.c_str(), "read");
   // TFile *f4 = new TFile(str4.c_str(), "read");
    //TFile *f5 = new TFile(str5.c_str(), "read");
    //TFile *f6 = new TFile(str6.c_str(), "read");
    TFile *ff = new TFile(strf.c_str(), "RECREATE");

    int nHist = f1->GetListOfKeys()->GetEntries();
    for (int i(0); i < nHist; i++){
        string hName = f1->GetListOfKeys()->At(i)->GetName();

        if (hName.find("hresponse") != string::npos){
            continue;
            /*
            TH2D *hr1 = (TH2D*) f1->Get(hName.c_str());
            TH2D *hr2 = (TH2D*) f2->Get(hName.c_str());
            TH2D *hr3 = (TH2D*) f3->Get(hName.c_str());
            TH2D *hr4 = (TH2D*) f4->Get(hName.c_str());
            TH2D *hr5 = (TH2D*) f5->Get(hName.c_str());
            TH2D *hr6 = (TH2D*) f6->Get(hName.c_str());

            TH2D *hrSum = (TH2D*) hr1->Clone();
            hrSum->Add(*hr2);
            hrSum->Add(*hr3);
            hrSum->Add(*hr4);
            hrSum->Add(*hr5);
            hrSum->Add(*hr6);
            ff->cd();
            hrSum->Write(hName.c_str());
             */
        }
        else {
            TH1D *h1 = (TH1D*) f1->Get(hName.c_str()); 
            TH1D *h2 = (TH1D*) f2->Get(hName.c_str()); 
            TH1D *h3 = (TH1D*) f3->Get(hName.c_str()); 
            //TH1D *h4 = (TH1D*) f4->Get(hName.c_str()); 
            //TH1D *h5 = (TH1D*) f5->Get(hName.c_str()); 
            //TH1D *h6 = (TH1D*) f6->Get(hName.c_str()); 

            TH1D *hSum = (TH1D*) h1->Clone();
            hSum->Add(h2);
            hSum->Add(h3);
            //hSum->Add(h4);
           // hSum->Add(h5);
           // hSum->Add(h6);
            ff->cd();
            hSum->Write();
        }
    }
    cout << "closing single top files" << endl;
    f1->Close();
    f2->Close();
    f3->Close();
    //f4->Close();
  //  f5->Close();
  //  f6->Close();
    ff->Close();

    //--- merge DY files
    int countHist(0);
    if (lepSelection == "SMu"){
        TFile *fDY[10] = {NULL};
        for ( int i = 0 ; i < nDYfiles ; i++){
            if ( i == nDYfiles - 1 )  fDY[i] =  new TFile(sstrDY[i].c_str(), "recreate");
            else fDY[i] =  new TFile(sstrDY[i].c_str(), "read");
        }
        cout << "Output file: " << sstrDY[nDYfiles - 1] << endl;

        nHist = fDY[0]->GetListOfKeys()->GetEntries();
        for (int i(0); i < nHist; i++){
            
            string hName = fDY[0]->GetListOfKeys()->At(i)->GetName();
            if (hName.find("hresponse") != string::npos){
                continue;
                /*
                cout << i << " TH2D " << hName << "  " << nHist << endl;
                TH2D* hrSum = NULL; TH2D* hrDY[10] = {NULL};
                for ( int j = 0 ; j < nDYfiles -1 ; j++){
                    hrDY[j] = (TH2D*) fDY[j]->Get(hName.c_str());
                    if ( j == 0 ) hrSum = (TH2D*) hrDY[j]->Clone();
                    else hrSum->Add(hrDY[j]);
                }
                fDY[nDYfiles -1]->cd();
                hrSum->Write();
                */
            }
            else {
                countHist++;
                cout << countHist << " " << i << " TH1D " << hName << "  " << nHist << endl;
                TH1D* hSum = NULL; TH1D* hDY[10] = {NULL};
                for ( int j = 0 ; j < nDYfiles -1 ; j++){
                    hDY[j] = (TH1D*) fDY[j]->Get(hName.c_str());
                    if ( j == 0 ) hSum = (TH1D*) hDY[j]->Clone();
                    else hSum->Add(hDY[j]);
                }
                fDY[nDYfiles -1]->cd();
                hSum->Write();
            }
        }
        
        cout << "closing DY files" << endl;
        for ( int i = 0 ; i < nDYfiles ; i++){
            fDY[i] ->Close();
        }
    }
    
}
