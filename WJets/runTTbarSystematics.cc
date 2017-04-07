
{
    string srcdir = "Sources/";
    
    vector<string> sources;
    sources.push_back("getFilesAndHistograms");
    sources.push_back("functions");
    sources.push_back("unfoldingFunctions");
    sources.push_back("TTbarSystematics");
    
    //--- Load shared libraries ---
    unsigned int nSources = sources.size();
    gROOT->ProcessLine(".L /afs/cern.ch/cms/slc5_amd64_gcc434/external/lhapdf/5.8.5/lib/libLHAPDF.so");
    for (unsigned int i(0); i < nSources; i++) {
        cout <<"Compiling " << srcdir + sources[i] << ".cc" << endl;
        gROOT->ProcessLine(string(".L " + srcdir + sources[i] + ".cc++").c_str());
    }
        
    computeTTbarSyst("SMu", "ZNGoodJets_Zexc", false);
    computeTTbarSyst("SMu", "FirstJetPt_Zinc1jet", false);
        
        computeTTbarSyst("SMu", "SecondJetPt_Zinc2jet", false);
        computeTTbarSyst("SMu", "ThirdJetPt_Zinc3jet", false);
        computeTTbarSyst("SMu", "FourthJetPt_Zinc4jet", false);
        computeTTbarSyst("SMu", "FifthJetPt_Zinc5jet", false);
        
        computeTTbarSyst("SMu", "JetsHT_Zinc1jet", false);
        computeTTbarSyst("SMu", "JetsHT_Zinc2jet", false);
        computeTTbarSyst("SMu", "JetsHT_Zinc3jet", false);
        computeTTbarSyst("SMu", "JetsHT_Zinc4jet", false);
        computeTTbarSyst("SMu", "JetsHT_Zinc5jet", false);
        
        computeTTbarSyst("SMu", "FirstJetEta_Zinc1jet", false);
        computeTTbarSyst("SMu", "SecondJetEta_Zinc2jet", false);
        computeTTbarSyst("SMu", "ThirdJetEta_Zinc3jet", false);
        computeTTbarSyst("SMu", "FourthJetEta_Zinc4jet", false);
        computeTTbarSyst("SMu", "FifthJetEta_Zinc5jet", false);
        
        computeTTbarSyst("SMu", "dRapidityJets_Zinc2jet", false);
        computeTTbarSyst("SMu", "dRapidityJets_Zinc3jet", false);
        computeTTbarSyst("SMu", "dRapidityJets_Zinc4jet", false);
        computeTTbarSyst("SMu", "dRapidityJetsFB_Zinc2jet", false);
        computeTTbarSyst("SMu", "dRapidityJetsFB_Zinc3jet", false);
        computeTTbarSyst("SMu", "dRapidityJetsFB_Zinc4jet", false);
        
        computeTTbarSyst("SMu", "dRapidityJets_First_Third_Zinc3jet", false);
        computeTTbarSyst("SMu", "dRapidityJets_Second_Third_Zinc3jet", false);
        
        computeTTbarSyst("SMu", "dPhiJets_Zinc2jet", false);
        computeTTbarSyst("SMu", "dPhiJetsFB_Zinc2jet", false);
        computeTTbarSyst("SMu", "dRJets_Zinc2jet", false);
        
        computeTTbarSyst("SMu", "diJetMass_Zinc2jet", false);
        computeTTbarSyst("SMu", "diJetMass_Zinc3jet", false);
        computeTTbarSyst("SMu", "diJetMass_Zinc4jet", false);
        computeTTbarSyst("SMu", "diJetPt_Zinc2jet", false);
        computeTTbarSyst("SMu", "diJetPt_Zinc3jet", false);
        computeTTbarSyst("SMu", "diJetPt_Zinc4jet", false);
    
    
    //getStatisticsVar("SMu", "ZNGoodJets_Zexc", 30, 0, false, true, 0, false, false, 0, 2);
    //getStatisticsVar("SMu", "ZNGoodJets_Zinc", 30, 0, false, true, 0, false, false, 0, 2);
        
    //analyzeTTbar("SMu", "ZNGoodJets_Zexc", 30, 0, false, true, 0, false, false, 0, 2);
    
    
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
