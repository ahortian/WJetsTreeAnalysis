{
    string srcdir = "Sources/";

    vector<string> sources;
    sources.push_back("getFilesAndHistograms");
    sources.push_back("functions");
    sources.push_back("writeFastPlotsTex");
    sources.push_back("unfoldingFunctions");
    sources.push_back("myFastPlots");

    //--- Load shaared libraries ---
    unsigned int nSources = sources.size();
    gROOT->ProcessLine(".L /afs/cern.ch/cms/slc5_amd64_gcc434/external/lhapdf/5.8.5/lib/libLHAPDF.so");
    for (unsigned int i(0); i < nSources; i++){
        cout <<"Compiling " << srcdir + sources[i] << ".cc" << endl;
        gROOT->ProcessLine(string(".L " + srcdir + sources[i] + ".cc++").c_str());
        //gROOT->LoadMacro(string(srcdir + sources[i] + ".cc++").c_str());
    }

        

    myFastPlots("SMu", "ZNGoodJets_Zexc", false);
    //myFastPlots("SMu", "ZNGoodJets_Zexc", true);
 
    myFastPlots("SMu", "FirstJetPt_Zinc1jet", false);
    //myFastPlots("SMu", "FirstJetPt_Zinc1jet", true);
    myFastPlots("SMu", "SecondJetPt_Zinc2jet", false);
    //myFastPlots("SMu", "SecondJetPt_Zinc2jet", true);
    myFastPlots("SMu", "ThirdJetPt_Zinc3jet", false);
    //myFastPlots("SMu", "ThirdJetPt_Zinc3jet", true);
    myFastPlots("SMu", "FourthJetPt_Zinc4jet", false);
    //myFastPlots("SMu", "FourthJetPt_Zinc4jet", true);

        
    myFastPlots("SMu", "JetsHT_Zinc1jet", false);
    //myFastPlots("SMu", "JetsHT_Zinc1jet", true);
    myFastPlots("SMu", "JetsHT_Zinc2jet", false);
    //myFastPlots("SMu", "JetsHT_Zinc2jet", true);
    myFastPlots("SMu", "JetsHT_Zinc3jet", false);
    //myFastPlots("SMu", "JetsHT_Zinc3jet", true);
    myFastPlots("SMu", "JetsHT_Zinc4jet", false);
    //myFastPlots("SMu", "JetsHT_Zinc4jet", true);

        
    myFastPlots("SMu", "FirstJetEta_Zinc1jet", false);
    //myFastPlots("SMu", "FirstJetEta_Zinc1jet", true);
    myFastPlots("SMu", "SecondJetEta_Zinc2jet", false);
    //myFastPlots("SMu", "SecondJetEta_Zinc2jet", true);
    myFastPlots("SMu", "ThirdJetEta_Zinc3jet", false);
    //myFastPlots("SMu", "ThirdJetEta_Zinc3jet", true);
    myFastPlots("SMu", "FourthJetEta_Zinc4jet", false);
    //myFastPlots("SMu", "FourthJetEta_Zinc4jet", true);
    
    
    myFastPlots("SMu", "FifthJetPt_Zinc5jet", false);
    //myFastPlots("SMu", "FifthJetPt_Zinc5jet", true);
    myFastPlots("SMu", "JetsHT_Zinc5jet", false);
    //myFastPlots("SMu", "JetsHT_Zinc5jet", true);
    myFastPlots("SMu", "FifthJetEta_Zinc5jet", false);
    //myFastPlots("SMu", "FifthJetEta_Zinc5jet", true);

        
    //--
    myFastPlots("SMu", "dRapidityJets_Zinc2jet", false);
    //myFastPlots("SMu", "dRapidityJets_Zinc2jet", true);
    myFastPlots("SMu", "dRapidityJets_Zinc3jet", false);
    //myFastPlots("SMu", "dRapidityJets_Zinc3jet", true);
    myFastPlots("SMu", "dRapidityJets_Zinc4jet", false);
    //myFastPlots("SMu", "dRapidityJets_Zinc4jet", true);
    
    //--
    myFastPlots("SMu", "dRapidityJetsFB_Zinc2jet", false);
    //myFastPlots("SMu", "dRapidityJetsFB_Zinc2jet", true);
    myFastPlots("SMu", "dRapidityJetsFB_Zinc3jet", false);
    //myFastPlots("SMu", "dRapidityJetsFB_Zinc3jet", true);
    myFastPlots("SMu", "dRapidityJetsFB_Zinc4jet", false);
    //myFastPlots("SMu", "dRapidityJetsFB_Zinc4jet", true);
    
    //--
    myFastPlots("SMu", "dRapidityJets_First_Third_Zinc3jet", false);
    //myFastPlots("SMu", "dRapidityJets_First_Third_Zinc3jet", true);
        
    //--
    myFastPlots("SMu", "dRapidityJets_Second_Third_Zinc3jet", false);
    //myFastPlots("SMu", "dRapidityJets_Second_Third_Zinc3jet", true);
    
    //--
    myFastPlots("SMu", "dPhiJets_Zinc2jet", false);
    //myFastPlots("SMu", "dPhiJets_Zinc2jet", true);
    myFastPlots("SMu", "dPhiJetsFB_Zinc2jet", false);
    //myFastPlots("SMu", "dPhiJetsFB_Zinc2jet", true);
    myFastPlots("SMu", "dRJets_Zinc2jet", false);
    //myFastPlots("SMu", "dRJets_Zinc2jet", true);
    
    //--
    myFastPlots("SMu", "diJetMass_Zinc2jet", false);
    //myFastPlots("SMu", "diJetMass_Zinc2jet", true);
    myFastPlots("SMu", "diJetMass_Zinc3jet", false);
    //myFastPlots("SMu", "diJetMass_Zinc3jet", true);
    myFastPlots("SMu", "diJetMass_Zinc4jet", false);
    //myFastPlots("SMu", "diJetMass_Zinc4jet", true);
    
    //--
    myFastPlots("SMu", "diJetPt_Zinc2jet", false);
    //myFastPlots("SMu", "diJetPt_Zinc2jet", true);
    myFastPlots("SMu", "diJetPt_Zinc3jet", false);
    //myFastPlots("SMu", "diJetPt_Zinc3jet", true);
    myFastPlots("SMu", "diJetPt_Zinc4jet", false);
    //myFastPlots("SMu", "diJetPt_Zinc4jet", true);
        /*
    //--
    myFastPlots("SMu", "dPhiLepJet1_Zinc1jet", false);
    //myFastPlots("SMu", "dPhiLepJet1_Zinc1jet", true);
    myFastPlots("SMu", "dPhiLepJet2_Zinc2jet", false);
    //myFastPlots("SMu", "dPhiLepJet2_Zinc2jet", true);
    myFastPlots("SMu", "dPhiLepJet3_Zinc3jet", false);
    //myFastPlots("SMu", "dPhiLepJet3_Zinc3jet", true);
    myFastPlots("SMu", "dPhiLepJet4_Zinc4jet", false);
    //myFastPlots("SMu", "dPhiLepJet4_Zinc4jet", true);
    myFastPlots("SMu", "dPhiLepJet5_Zinc5jet", false);
    //myFastPlots("SMu", "dPhiLepJet5_Zinc5jet", true);
        */ //Kadir
        
        //myFastPlots("SMu", "ZNGoodJets_Zinc", false);
        //myFastPlots("SMu", "ZNGoodJets_Zinc", true);
        
        /*
         myFastPlots("SMu", "FirstJetPt_1_Zinc1jet", false);
         myFastPlots("SMu", "SecondJetPt_1_Zinc2jet", false);
         myFastPlots("SMu", "ThirdJetPt_1_Zinc3jet", false);
         myFastPlots("SMu", "FourthJetPt_1_Zinc4jet", false);
         myFastPlots("SMu", "FifthJetPt_1_Zinc5jet", false);
         
         myFastPlots("SMu", "JetsHT_1_Zinc1jet", false);
         myFastPlots("SMu", "JetsHT_1_Zinc2jet", false);
         myFastPlots("SMu", "JetsHT_1_Zinc3jet", false);
         myFastPlots("SMu", "JetsHT_1_Zinc4jet", false);
         myFastPlots("SMu", "JetsHT_1_Zinc5jet", false);
         */
        
    
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
