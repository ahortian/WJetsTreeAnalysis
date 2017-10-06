//---- 2015_06_13
// Add sys LepSF
//---- 2015_04_09
// Fixing to be able to run Sherpa2

{
    string srcdir = "Sources/";

    vector<string> sources;
    sources.push_back("getFilesAndHistograms");
    sources.push_back("functions");
    sources.push_back("funcReweightResp");
    sources.push_back("HistoSet");
    sources.push_back("ZJetsAndDPS");

    ////--- Load shaared libraries ---
    unsigned int nSources = sources.size();
    gSystem->AddIncludePath("-D__USE_XOPEN2K8");
    //gROOT->ProcessLine(".L /usr/local/lib/libLHAPDF.dylib");
    gROOT->ProcessLine(".L /cvmfs/cms.cern.ch/slc5_amd64_gcc434/external/lhapdf/5.8.5/lib/libLHAPDF.so");
    for (unsigned int i(0); i < nSources; i++) {
        std::cout << "Compiling " << srcdir + sources[i] << ".cc" << std::endl;
        gROOT->ProcessLine(string(".L " + srcdir + sources[i] + ".cc+").c_str());
    }
        
    //------ External configurations (change these if you wish) ---
    int doWhat       = 100;
                              // 100 - all ; 0 - data, 1 - background , 2 - tau ?, 3 - DY, 4 - W+jets,
                              // 51 - MC gen, 90 - PDF Syst., 1001 - do pull DY samples
        
    int doSysRunning = 0;
                             // 0 - no syst running, 100 - all systematic runnings,
                             // 1 - PU, 2 - JES, 3 - XSEC, 4 - JER, 5 - LepSF,
                             // 6 - BtagSF, 7 - MES, 8 - MER, 9 - WB, 10 - RESP
        
    int doQCD        = 0;
                             // 0-3 : 4 combination between isolation/anti-isolation and MT cuts
        
    int doBJets      = -1;
                            // 0 - no infor on B-jets will be used ;
                            // 1, 2 .. require at least 1, 2, .. ; use 2 for ttbar systmatics;
                            // -1, -2, .. veto the event if you have 1 or more, 2 or more .. b-jets ;
                            // 101 - require exactly 1 b-jet
    
		
		
    //--- Internal configuration (no need to change this) ---
    
    string lepSelection = "SMu"; // default lumi is set for double muon dataset
    //double muLumi(2503.96046); //before approval
    //double muLumi(2466.72186918); //after approval
    double muLumi(2239.351439352); //76x data bonzai golden json v2
    //double muLumi(2671.381641216); //76x data bonzai silver json v2

    int doRoch   = 0;
    int doFlat   = 0;
    int doPUStudy = -10 ;  // default int the ZJets
    bool doSSign  =  0;     // contribution of QCD to emu in TTbar
    bool doInvMassCut = 0 ;
    bool doDataEff(0);
        
    int doMETcut = 0 ;     // if you want to apply MET cut
    int jetPtMin = 30;
    int jetPtMax = 0;      // 0 - for no jetPtMax cut
    int ZPtMin = 0 ;
    int ZEtaMin  = -999999;  // default value -999999  !!!!!!!  factor 100 to keep things integer .... eta 2.4  = eta Cut 240
    int ZEtaMax  = 999999;   // default value  999999

    const int NSystData(3), NSystMC(12); // set NSystMC to 5 if you want to do only PU, XSEC
    const int NSystWJets(16), NSystDYJets(14);
    
    short dataSyst[NSystData]  = {0, 2, 2};
    short dataDir[NSystData]   = {0,-1, 1};

    short ttSyst[NSystMC]      = {0, 1, 1,    3,    3,  5, 5, 6, 6, 7, 7, 8};
    short ttDir[NSystMC]       = {0,-1, 1,   -1,    1, -1, 1,-1, 1,-1, 1, 1};
    float ttScale[NSystMC]     = {1, 1, 1,  0.06,  0.06,  1, 1, 1, 1, 1, 1, 1};

    short tauSyst[NSystMC]     = {0, 1, 1,    3,    3,  5, 5, 6, 6, 7, 7, 8};
    short tauDir[NSystMC]      = {0,-1, 1,   -1,    1, -1, 1,-1, 1,-1, 1, 1};
    float tauScale[NSystMC]    = {1, 1, 1,   0.,   0.,  1, 1, 1, 1, 1, 1, 1};

    short bgSyst[NSystMC]      = {0, 1, 1,    3,    3,  5, 5, 6, 6, 7, 7, 8};
    short bgDir[NSystMC]       = {0,-1, 1,   -1,    1, -1, 1,-1, 1,-1, 1, 1};
    float bgScale[NSystMC]     = {1, 1, 1,   0.,   0.,  1, 1, 1, 1, 1, 1, 1};
    
    short zzSyst[NSystMC]      = {0, 1, 1,    3,    3,  5, 5, 6, 6, 7, 7, 8};
    short zzDir[NSystMC]       = {0,-1, 1,   -1,    1, -1, 1,-1, 1,-1, 1, 1};
    float zzScale[NSystMC]     = {1, 1, 1, 0.07, 0.07,  1, 1, 1, 1, 1, 1, 1};
        
    short wzSyst[NSystMC]      = {0, 1, 1,    3,    3,  5, 5, 6, 6, 7, 7, 8};
    short wzDir[NSystMC]       = {0,-1, 1,   -1,    1, -1, 1,-1, 1,-1, 1, 1};
    float wzScale[NSystMC]     = {1, 1, 1, 0.07, 0.07,  1, 1, 1, 1, 1, 1, 1};
    
    short wwSyst[NSystMC]      = {0, 1, 1,    3,    3,  5, 5, 6, 6, 7, 7, 8};
    short wwDir[NSystMC]       = {0,-1, 1,   -1,    1, -1, 1,-1, 1,-1, 1, 1};
    float wwScale[NSystMC]     = {1, 1, 1, 0.06, 0.06,  1, 1, 1, 1, 1, 1, 1};
        
    short tcsSyst[NSystMC]     = {0, 1, 1,    3,    3,  5, 5, 6, 6, 7, 7, 8};
    short tcsDir[NSystMC]      = {0,-1, 1,   -1,    1, -1, 1,-1, 1,-1, 1, 1};
    float tcsScale[NSystMC]    = {1, 1, 1, 0.04, 0.04,  1, 1, 1, 1, 1, 1, 1};
    
    short tctSyst[NSystMC]     = {0, 1, 1,    3,    3,  5, 5, 6, 6, 7, 7, 8};
    short tctDir[NSystMC]      = {0,-1, 1,   -1,    1, -1, 1,-1, 1,-1, 1, 1};
    float tctScale[NSystMC]    = {1, 1, 1, 0.05, 0.05,  1, 1, 1, 1, 1, 1, 1};
    
    short tcwSyst[NSystMC]     = {0, 1, 1,    3,    3,  5, 5, 6, 6, 7, 7, 8};
    short tcwDir[NSystMC]      = {0,-1, 1,   -1,    1, -1, 1,-1, 1,-1, 1, 1};
    float tcwScale[NSystMC]    = {1, 1, 1, 0.06, 0.06,  1, 1, 1, 1, 1, 1, 1};
        
    short wjSyst[NSystWJets]   = {0, 1, 1,    3,    3,  5, 5, 6, 6, 7, 7, 4, 4, 8, 9, 10};
    short wjDir[NSystWJets]    = {0,-1, 1,   -1,    1, -1, 1,-1, 1,-1, 1,-1, 1, 1, 1,  1};
    float wjScale[NSystWJets]  = {1, 1, 1, 0.04, 0.04,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1};
        
    short dySyst[NSystDYJets]  = {0, 1, 1,    3,    3,  5, 5, 6, 6, 7, 7, 4, 4, 8};
    short dyDir[NSystDYJets]   = {0,-1, 1,   -1,    1, -1, 1,-1, 1,-1, 1,-1, 1, 1};
    float dyScale[NSystDYJets] = {1, 1, 1, 0.04, 0.04,  1, 1, 1, 1, 1, 1, 1, 1, 1};
        

    // Data
    if ( doWhat == 0 || doWhat == 100) {
        for (unsigned int i(0); i < NSystData; i++) {
        //for (unsigned int i(1); i < 3; i++) {
            if (dataSyst[i] != doSysRunning && doSysRunning != 100) continue;
            
            ZJetsAndDPS DMudata(lepSelection+"_13TeV_Data_dR_5311_List", 1., 1, 1, doDataEff, dataSyst[i], dataDir[i], 1, jetPtMin, jetPtMax, ZPtMin, ZEtaMin, ZEtaMax);
            DMudata.Loop(1, 0, doQCD, doSSign, doInvMassCut, doBJets, doPUStudy);
        }
    }
        
    // Background
    if ( doWhat == 1 || doWhat == 100 ){
        for (unsigned int i(0); i < NSystMC; i++){
            if (bgSyst[i] != doSysRunning && doSysRunning != 100) continue;
            
            ZJetsAndDPS DMuTT(lepSelection+"_13TeV_TTJets_dR_5311_List",             muLumi * 831.7  , 1, 1, !doDataEff, ttSyst[i], ttDir[i], ttScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuTT.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );

            //ZJetsAndDPS DMuTT1(lepSelection+"_13TeV_TTJets_amcatnlo_dR_5311_List",             muLumi * 831.7  , 1, 1, !doDataEff, ttSyst[i], ttDir[i], ttScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            //DMuTT1.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );  // open if you want to run on ttbar by amcatnlo sample as well

            ZJetsAndDPS DMuZZInc(lepSelection+"_13TeV_ZZ_dR_5311_List",              muLumi * 15.4 ,  1, 1, !doDataEff, zzSyst[i], zzDir[i], zzScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuZZInc.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            ZJetsAndDPS DMuWZInc(lepSelection+"_13TeV_WZ_dR_5311_List",              muLumi * 23.5 , 1, 1, !doDataEff, wzSyst[i], wzDir[i], wzScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuWZInc.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            ZJetsAndDPS DMuWWInc(lepSelection+"_13TeV_WW_dR_5311_List",              muLumi * 12.21  , 1, 1, !doDataEff, wwSyst[i], wwDir[i], wwScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuWWInc.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            ZJetsAndDPS DMuT(lepSelection+"_13TeV_ST_s_channel_dR_5311_List",         muLumi * 3.35  ,  1, 1, !doDataEff, tcsSyst[i], tcsDir[i], tcsScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuT.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            ZJetsAndDPS DMuT1(lepSelection+"_13TeV_ST_t_channel_dR_5311_List",         muLumi * 70.30  ,  1, 1, !doDataEff, tcsSyst[i], tcsDir[i], tcsScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuT1.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            ZJetsAndDPS DMuT2(lepSelection+"_13TeV_ST_tW_top_channel_dR_5311_List",        muLumi * 35.6  , 1, 1, !doDataEff, tcwSyst[i], tcwDir[i], tcwScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuT2.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            ZJetsAndDPS DMuT3(lepSelection+"_13TeV_ST_tW_antitop_channel_dR_5311_List",       muLumi * 35.6 ,  1, 1, !doDataEff, tcwSyst[i], tcwDir[i], tcwScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuT3.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );

            
            if ( (lepSelection.find("SE") == -1 && lepSelection.find("SMu") == -1 )  ) {
                // for Z plus jets: WJets is background
				ZJetsAndDPS DMuWJMix(lepSelection+"_13TeV_WJetsALL_MIX_UNFOLDING_dR_5311", muLumi* 60290.0 , 1., 1, !doDataEff, wjSyst[i], wjDir[i], wjScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax, 0);
				DMuWJMix.Loop(1, 0,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy ); //FxFx with NLO normalization
				
				ZJetsAndDPS DMuWJMix1(lepSelection+"_13TeV_WJetsALL_MLM_UNFOLDING_dR_5311", muLumi* 61526.7 , 1., 1, !doDataEff, wjSyst[i], wjDir[i], wjScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax, 0);
				DMuWJMix1.Loop(1, 0,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy ); //open if you want to run on wjets by mg lo mlm sample as well
				
				//--- the following Bg are not used for WJets analysis ---//
            }
        }
    }
    
    // DYJets_MIX_UNFOLDING_dR_5311_Inf3
    if ( doWhat == 3 || doWhat == 100 ){
        int doGen = 0 ;
        if ( lepSelection.find("DMu") == 0 || lepSelection.find("DE") == 0 )  doGen = 1 ;
        
        for (unsigned int i(0); i < NSystDYJets; i++){
            if ( ( lepSelection.find("SMu") == 0 || lepSelection.find("SE") == 0 ) && dySyst[i] == 4 ) continue; // jet smearing part -- not done for SMu ---
            if ( ( lepSelection.find("DMu") == 0 || lepSelection.find("DE") == 0 ) && dySyst[i] == 3 ) continue; // xsec -- not done for Z+jets ---
            if (dySyst[i] != doSysRunning && doSysRunning != 100) continue;
            
            ZJetsAndDPS DMuDYMix(lepSelection+"_13TeV_DYJets50toInf_dR_5311_List", muLumi * 6025.2 , 1., 1, !doDataEff, dySyst[i], dyDir[i], dyScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax, 0);
            DMuDYMix.Loop(1, doGen,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
        }
        
        if ( (lepSelection.find("SE") == -1 && lepSelection.find("SMu") == -1 )  ) {
            // these files are not needed for W+jets
            ZJetsAndDPS DMuDY(lepSelection+"_13TeV_DYJets_UNFOLDING_dR_5311_Inf3",  muLumi*3531.8*1000/30459503., 1., 1, !doDataEff, 0, 0, 1, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuDY.Loop(1, doGen,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
        }
    }
        
    // This is unfolding part for W+jets 
    if ( doWhat == 4 || doWhat == 100 ){
        int doGen = 0 ;
        if ( (lepSelection.find("SE") == 0 || lepSelection.find("SMu") == 0 ) && lepSelection.find("SMuE") == -1 )  doGen = 1 ;
        
        for (unsigned int i(0); i < NSystWJets; i++){
            if (!doGen ) continue;
            if ( ( lepSelection.find("SMu") == 0 || lepSelection.find("SE") == 0 ) && wjSyst[i] == 3) continue; // xsec -- not done for SMu ---
            if (wjSyst[i] != doSysRunning && doSysRunning != 100) continue;
 
            ZJetsAndDPS DMuWJMix(lepSelection+"_13TeV_WJetsALL_MIX_UNFOLDING_dR_5311_List", muLumi* 60290.0 , 1., 1, !doDataEff, wjSyst[i], wjDir[i], wjScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax, 0);
            DMuWJMix.Loop(1, doGen,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy ); //FxFx with NLO normalization

            ZJetsAndDPS DMuWJMix1(lepSelection+"_13TeV_WJetsALL_MLM_UNFOLDING_dR_5311_List", muLumi* 61526.7 , 1., 1, !doDataEff, wjSyst[i], wjDir[i], wjScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax, 0);
            DMuWJMix1.Loop(1, doGen,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy ); //open if you want to run on wjets by mg lo mlm sample as well
        }
    }
    
    // Sherpa2
    if ( doWhat == 51){
        // this is setup for sherpa NLO
        ZJetsAndDPS DESherpaTest2NLO("SMu_13TeV_WToLNu_Sherpa2jNLO4jLO_v5",  muLumi         * 1000.          , 1.,    0,   0,     0,    0,     1.,  jetPtMin,  jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax );
        DESherpaTest2NLO.Loop(0, 1, 0, 0, 0);
    }
    
    
    // skip this part
    // now produce files for pulls
    if ( doWhat == 1001 ){
        int doGen = 1 ;
        doDataEff = 1 ;
        int NPulls = 25 ;
        for (int loopPull = 0 ; loopPull < NPulls ; loopPull++){
            if ( lepSelection.find("DMu") == 0 ||  lepSelection.find("DE") == 0 ) {
                ZJetsAndDPS DMuDYMix(lepSel+"_13TeV_DYJets_MIX_UNFOLDING_dR_5311_Inf3", muLumi*3531.8*1000/30459503., 1., 1, !doDataEff, 0, 0, 1,   jetPtMin,         jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax, 0);
                DMuDYMix.Loop(1, 1,  0,  0, 0, 0, -10, 0, 0, 1, 0, "", 0, loopPull, NPulls  );
            }
        }
    }
        
        
        
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

