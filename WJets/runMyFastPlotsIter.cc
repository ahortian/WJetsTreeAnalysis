{
    string srcdir = "Sources/";

    vector<string> sources;
    sources.push_back("getFilesAndHistograms");
    sources.push_back("functions");
    sources.push_back("writeFastPlotsTex");
    sources.push_back("unfoldingFunctions");
    sources.push_back("myFastPlotsIter");

    //--- Load shaared libraries ---
    unsigned int nSources = sources.size();
    gROOT->ProcessLine(".L /afs/cern.ch/cms/slc5_amd64_gcc434/external/lhapdf/5.8.5/lib/libLHAPDF.so");
    for (unsigned int i(0); i < nSources; i++){
        cout <<"Compiling " << srcdir + sources[i] << ".cc" << endl;
        gROOT->ProcessLine(string(".L " + srcdir + sources[i] + ".cc++").c_str());
    }
        
        
        myFastPlotsIter("SMu",  "FirstJetPt_Zinc1jet",  "FirstJetPt_2_Zinc1jet", false);
        myFastPlotsIter("SMu", "SecondJetPt_Zinc2jet", "SecondJetPt_2_Zinc2jet", false);
        myFastPlotsIter("SMu",  "ThirdJetPt_Zinc3jet",  "ThirdJetPt_2_Zinc3jet", false);
        myFastPlotsIter("SMu", "FourthJetPt_Zinc4jet", "FourthJetPt_2_Zinc4jet", false);
        myFastPlotsIter("SMu",  "FifthJetPt_Zinc5jet",  "FifthJetPt_2_Zinc5jet", false);
        
        myFastPlotsIter("SMu", "JetsHT_Zinc1jet", "JetsHT_2_Zinc1jet", false);
        myFastPlotsIter("SMu", "JetsHT_Zinc2jet", "JetsHT_2_Zinc2jet", false);
        myFastPlotsIter("SMu", "JetsHT_Zinc3jet", "JetsHT_2_Zinc3jet", false);
        myFastPlotsIter("SMu", "JetsHT_Zinc4jet", "JetsHT_2_Zinc4jet", false);
        myFastPlotsIter("SMu", "JetsHT_Zinc5jet", "JetsHT_2_Zinc5jet", false);
        
        myFastPlotsIter("SMu",  "FirstJetEta_Zinc1jet",  "FirstJetEta_2_Zinc1jet", false);
        myFastPlotsIter("SMu", "SecondJetEta_Zinc2jet", "SecondJetEta_2_Zinc2jet", false);
        myFastPlotsIter("SMu",  "ThirdJetEta_Zinc3jet",  "ThirdJetEta_2_Zinc3jet", false);
        myFastPlotsIter("SMu", "FourthJetEta_Zinc4jet", "FourthJetEta_2_Zinc4jet", false);
        myFastPlotsIter("SMu",  "FifthJetEta_Zinc5jet",  "FifthJetEta_2_Zinc5jet", false);
        
        
        myFastPlotsIter("SMu",  "dRapidityJets_Zinc2jet",  "dRapidityJets_2_Zinc2jet", false);
        myFastPlotsIter("SMu",  "dRapidityJets_Zinc3jet",  "dRapidityJets_2_Zinc3jet", false);
        myFastPlotsIter("SMu",  "dRapidityJets_Zinc4jet",  "dRapidityJets_2_Zinc4jet", false);
        
        myFastPlotsIter("SMu",  "dRapidityJetsFB_Zinc2jet",  "dRapidityJetsFB_2_Zinc2jet", false);
        myFastPlotsIter("SMu",  "dRapidityJetsFB_Zinc3jet",  "dRapidityJetsFB_2_Zinc3jet", false);
        myFastPlotsIter("SMu",  "dRapidityJetsFB_Zinc4jet",  "dRapidityJetsFB_2_Zinc4jet", false);
        
        myFastPlotsIter("SMu",   "dRapidityJets_First_Third_Zinc3jet",   "dRapidityJets_2_First_Third_Zinc3jet", false);
        myFastPlotsIter("SMu",  "dRapidityJets_Second_Third_Zinc3jet",  "dRapidityJets_2_Second_Third_Zinc3jet", false);
        
        myFastPlotsIter("SMu",    "dPhiJets_Zinc2jet",    "dPhiJets_2_Zinc2jet", false);
        myFastPlotsIter("SMu",  "dPhiJetsFB_Zinc2jet",  "dPhiJetsFB_2_Zinc2jet", false);
        
        myFastPlotsIter("SMu",  "dRJets_Zinc2jet",  "dRJets_2_Zinc2jet", false);
        
        myFastPlotsIter("SMu",  "diJetMass_Zinc2jet",  "diJetMass_2_Zinc2jet", false);
        myFastPlotsIter("SMu",  "diJetMass_Zinc3jet",  "diJetMass_2_Zinc3jet", false);
        myFastPlotsIter("SMu",  "diJetMass_Zinc4jet",  "diJetMass_2_Zinc4jet", false);
        
        myFastPlotsIter("SMu",  "diJetPt_Zinc2jet",  "diJetPt_2_Zinc2jet", false);
        myFastPlotsIter("SMu",  "diJetPt_Zinc3jet",  "diJetPt_2_Zinc3jet", false);
        myFastPlotsIter("SMu",  "diJetPt_Zinc4jet",  "diJetPt_2_Zinc4jet", false);
        

        
    //--- clean the *_cc.d and *_cc.so files ---
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
