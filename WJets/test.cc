{


    string srcdir = "Sources/";

    vector<string> sources;
    sources.push_back("getFilesAndHistograms");
    sources.push_back("functions");
    sources.push_back("HistoSet");
    sources.push_back("ZJetsAndDPS");

    ////--- Load shaared libraries ---
    //gSystem->Load("checkForCompilationNeeds.so");
    //checkForCompilationNeeds(srcdir, sources);
    unsigned int nSources = sources.size();
    gROOT->ProcessLine(".L /afs/cern.ch/cms/slc5_amd64_gcc434/external/lhapdf/5.8.5/lib/libLHAPDF.so");
    for (unsigned int i(0); i < nSources; i++){
        cout <<"Compiling " << srcdir + sources[i] << ".cc" << endl;
        gROOT->ProcessLine(string(".L " + srcdir + sources[i] + ".cc+").c_str());
    }

    double  muLumi(19.549); // DoubleMu number with pixelCalc
    double eleLumi(19.602); // DoubleEle number with pixelCalc


    int doRoch   = 0;
    int doFlat   = 0;
    bool doBJets = 0;
    int doMETcut = 20 ;
    int doQCD    = 0;
    int doPUStudy = -10 ; // default int the ZJets
    bool doSSign  =  0;   // contribution of QCD to emu in TTbar 
    bool doInvMassCut = 0 ; 
    int jetPtMin = 30;
    int jetPtMax = 0;
    int ZPtMin = 0 ;
    int ZEtaMin  = -999999;  // default value -999999       !!!!!!!  factor 100 to keep things integer ....    eta 2.4  = eta Cut 240 
    int  ZEtaMax  = 999999;  // default value  999999



//    string lepSelection = "DMu"; // default lumi is set for double muon dataset
    string lepSelection = "SMu"; // default lumi is set for double muon dataset
 
   if ( lepSelection.find("DE") == 0)   muLumi = 19.602 ;
    else if ( lepSelection.find("SMuE") == 0) muLumi = 19.673 ;
    else if ( lepSelection.find("SMu") == 0)  muLumi = 19.244 ;
    else if ( lepSelection.find("SE") == 0)   muLumi = 19.174 ;


    short dataSyst[5] = {0, 2,2,5,5};
    short dataDir[5]  = {0,-1,1,-1,1};

    short ttSyst[5]  = {0, 1, 1, 3, 3};
    short ttDir[5]   = {0,-1, 1,-1, 1};
    float ttScale[5] = {1, 1, 1, 0.07, 0.07};

    short tauSyst[5]  = {0, 1, 1, 3, 3};
    short tauDir[5]   = {0,-1, 1,-1, 1};
    float tauScale[5] = {1, 1, 1, 0.05, 0.05};

    short bgSyst[5]  = {0, 1, 1, 3, 3};
    short bgDir[5]   = {0,-1, 1,-1, 1};
    float bgScale[5] = {1, 1, 1, 0.15, 0.15};
    bool doDataEff(0);
    int NSystData(3),NSystMC(5);
    bool doSysRunning(0);
//    int doWhat   = -3 ; // 100 - all ; 0 - data, 1 - background , 2 - tau ???, 3 - DY, 4 - W+jets, 51 - MC gen
   int doWhat   = 100; //to run on background only

//   int doWhat   = 0;

    if ( lepSelection.find("DE") == 0)  NSystData = 5 ;

    if ( !doSysRunning ) {NSystData = 1; NSystMC = 1; }

    if (doWhat == -3) {
        ZJetsAndDPS DMuDYMixPDF(lepSelection+"_8TeV_DYJets_MIX_UNFOLDING_dR_5311_Inf3", muLumi*3531.8*1000/30459503., 1., 1, !doDataEff, 0, 0, 1, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax, -30, 1);
        //DMuDYMixPDF.Loop(0, 1,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, 0, 0, 1, 0, "CT10nlo.LHgrid", 0);
        //DMuDYMixPDF.Loop(1, 1,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, 0, 0, 0, 0);
        //
        ZJetsAndDPS DMuDYscaleUp(lepSelection+"_8TeV_DYJets_UNFOLDING_dR_5311_Inf3_scaleUp",  muLumi*3531.8*1000/(2*2170270.), 1., 1, !doDataEff, 0, 0, 1, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
        DMuDYscaleUp.Loop(1, 1,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
        ZJetsAndDPS DMuDYscaleDown(lepSelection+"_8TeV_DYJets_UNFOLDING_dR_5311_Inf3_scaleDown",  muLumi*3531.8*1000/(2*1934901.), 1., 1, !doDataEff, 0, 0, 1, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
        DMuDYscaleDown.Loop(1, 1,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
    
    }

    if ( doWhat == 222 ){
        ZJetsAndDPS DMuDY("DMu_8TeV_DYJets_UNFOLDING_dR_5311_Inf3",  muLumi*3531.8*1000/30459503., 1., 1, !doDataEff, 0, 0, 1,          jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
        //       DMuDY.Loop(1, 1,  doQCD,  doSSign, doInvMassCut,  doBJets, doPUStudy );
        //         DMuDY.Loop(1, 0,  1,  1, 1, 22, doBJets, 9 );
        ZJetsAndDPS DEDY("DE_8TeV_DYJets_UNFOLDING_dR_5311_Inf3",  muLumi*3531.8*1000/30459503., 1., 1, !doDataEff, 0, 0, 1,          jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
        ///          DEDY.Loop(1, 1,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
        ZJetsAndDPS DMuTTrew(lepSelection+"_8TeV_TTJets_dR_5311_TopReweighting",             muLumi*245.           *1000/6923652., 1, 1, !doDataEff, ttSyst[i], ttDir[i], ttScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
        DMuTTrew.Loop(1, 1, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );

    }
    if ( doWhat == 0 || doWhat == 100 ){
        for (unsigned int i(0); i < NSystData; i++){
            ZJetsAndDPS DMudata(lepSelection+"_8TeV_Data_dR_5311",             1.,                                   1, 1, doDataEff, dataSyst[i], dataDir[i], 1, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax );
//            DMudata.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
        }
    }
    if ( doWhat == 1 || doWhat == 100 ){
        for (unsigned int i(0); i < NSystMC; i++){ 
            ZJetsAndDPS DMuTT(lepSelection+"_8TeV_TTJets_dR_5311",             muLumi*245.           *1000/6923652., 1, 1, !doDataEff, ttSyst[i], ttDir[i], ttScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuTT.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );

            ZJetsAndDPS DMuTTrew(lepSelection+"_8TeV_TTJets_dR_5311_TopReweighting",             muLumi*245.           *1000/6923652., 1, 1, !doDataEff, ttSyst[i], ttDir[i], ttScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuTTrew.Loop(1, 1, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );

            if ( (lepSelection.find("SE") == -1 && lepSelection.find("SMu") == -1 )  ) {
                ZJetsAndDPS DMuWJ(lepSelection+"_8TeV_WJetsALL_UNFOLDING_dR_5311",           muLumi*36703.         *1000/76102995.,1, 1, !doDataEff, bgSyst[i], bgDir[i], bgScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
                DMuWJ.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );

                ZJetsAndDPS DMuWJmix(lepSelection+"_8TeV_WJetsALL_MIX_UNFOLDING_dR_5311",           muLumi*36703.         *1000/76102995.,1, 1, !doDataEff, bgSyst[i], bgDir[i], bgScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
                DMuWJmix.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            }
            ZJetsAndDPS DMuZZInc(lepSelection+"_8TeV_ZZ_dR_5311",              muLumi*17.654        *1000/9799908.,  1, 1, !doDataEff, bgSyst[i], bgDir[i], bgScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuZZInc.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            ZJetsAndDPS DMuWZInc(lepSelection+"_8TeV_WZ_dR_5311",              muLumi*33.21         *1000/10000283., 1, 1, !doDataEff, bgSyst[i], bgDir[i], bgScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuWZInc.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            ZJetsAndDPS DMuWWInc(lepSelection+"_8TeV_WW_dR_5311",              muLumi*54.838        *1000/10000431., 1, 1, !doDataEff, bgSyst[i], bgDir[i], bgScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuWWInc.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            ZJetsAndDPS DMuZZ(lepSelection+"_8TeV_ZZJets2L2Nu_dR_5311",        muLumi*17.654*0.04039 *1000/954911.,  1, 1, !doDataEff, bgSyst[i], bgDir[i], bgScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuZZ.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            ZJetsAndDPS DMuWW(lepSelection+"_8TeV_WWJets2L2Nu_dR_5311",        muLumi*54.838*0.10608 *1000/1933235., 1, 1, !doDataEff, bgSyst[i], bgDir[i], bgScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuWW.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            ZJetsAndDPS DMuZZ1(lepSelection+"_8TeV_ZZJets2L2Q_dR_5311",        muLumi*17.654*0.14118 *1000/1936727., 1, 1, !doDataEff, bgSyst[i], bgDir[i], bgScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuZZ1.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            ZJetsAndDPS DMuZZ2(lepSelection+"_8TeV_ZZJets4L_dR_5311",          muLumi*17.654*0.010196*1000/4807893., 1, 1, !doDataEff, bgSyst[i], bgDir[i], bgScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuZZ2.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            ZJetsAndDPS DMuWZ(lepSelection+"_8TeV_WZJets3LNu_dR_5311",         muLumi*33.21 *0.032887*1000/1995334., 1, 1, !doDataEff, bgSyst[i], bgDir[i], bgScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuWZ.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            ZJetsAndDPS DMuWZ1(lepSelection+"_8TeV_WZJets2L2Q_dR_5311",        muLumi*33.21 *0.068258*1000/3215990., 1, 1, !doDataEff, bgSyst[i], bgDir[i], bgScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuWZ1.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            ZJetsAndDPS DMuT(lepSelection+"_8TeV_T_s_channel_dR_5311",         muLumi*3.79           *1000/259961.,  1, 1, !doDataEff, bgSyst[i], bgDir[i], bgScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuT.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            ZJetsAndDPS DMuT1(lepSelection+"_8TeV_T_t_channel_dR_5311",        muLumi*56.4           *1000/3758227., 1, 1, !doDataEff, bgSyst[i], bgDir[i], bgScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuT1.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            ZJetsAndDPS DMuT2(lepSelection+"_8TeV_T_tW_channel_dR_5311",       muLumi*11.1           *1000/497658.,  1, 1, !doDataEff, bgSyst[i], bgDir[i], bgScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuT2.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            ZJetsAndDPS DMuTbar(lepSelection+"_8TeV_Tbar_s_channel_dR_5311",   muLumi*1.76           *1000/139974.,  1, 1, !doDataEff, bgSyst[i], bgDir[i], bgScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuTbar.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            ZJetsAndDPS DMuTbar1(lepSelection+"_8TeV_Tbar_t_channel_dR_5311",  muLumi*30.7           *1000/1903681., 1, 1, !doDataEff, bgSyst[i], bgDir[i], bgScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuTbar1.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            ZJetsAndDPS DMuTbar2(lepSelection+"_8TeV_Tbar_tW_channel_dR_5311", muLumi*11.1           *1000/493460.,  1, 1, !doDataEff, bgSyst[i], bgDir[i], bgScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuTbar2.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            ZJetsAndDPS DMuDY10(lepSelection+"_8TeV_DYJets10to50_dR_5311",    muLumi*860.5         *1000/11707222.,  1, 1, !doDataEff, bgSyst[i], bgDir[i], bgScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuDY10.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
        }
    }
    if ( doWhat == 2 || doWhat == 100 ){
        for (unsigned int i(3); i < 5; i++){ 
            ZJetsAndDPS DMuDYTau(lepSelection+"_8TeV_DYJets_FromTau_UNFOLDING_dR_5311_Inf3", muLumi*3531.8*1000/3045950, 1., 1,  !doDataEff, tauSyst[i], tauDir[i], tauScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            //	  DMuDYTau.Loop(1, 1,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
        }
    }

    //
    if ( doWhat == 3 || doWhat == 100 ){
        int doGen = 0 ;
        if ( lepSelection.find("DMu") == 0 || lepSelection.find("DE") == 0 )  doGen = 1 ;
        ZJetsAndDPS DMuDYTauS(lepSelection+"_8TeV_DYJets_FromTau_UNFOLDING_dR_5311_Inf3", muLumi*3531.8*1000/30459503., 1, 1, !doDataEff, 0, 0, 1, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
        	  DMuDYTauS.Loop(1, 1,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );

        ZJetsAndDPS DMuDY(lepSelection+"_8TeV_DYJets_UNFOLDING_dR_5311_Inf3",  muLumi*3531.8*1000/30459503., 1., 1, !doDataEff, 0, 0, 1, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
        	  DMuDY.Loop(1, doGen,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
        ZJetsAndDPS DMuDYMix(lepSelection+"_8TeV_DYJets_MIX_UNFOLDING_dR_5311_Inf3", muLumi*3531.8*1000/30459503., 1., 1, !doDataEff, 0, 0, 1, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax, 0);
        	  DMuDYMix.Loop(1, doGen,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
        // scale files
        ZJetsAndDPS DMuDYscaleUp(lepSelection+"_8TeV_DYJets_UNFOLDING_dR_5311_Inf3_scaleUp",  muLumi*3531.8*1000/2170270., 1.,  1, !doDataEff, 0, 0, 1, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
        DMuDYscaleUp.Loop(1, doGen,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
        ZJetsAndDPS DMuDYscaleDown(lepSelection+"_8TeV_DYJets_UNFOLDING_dR_5311_Inf3_scaleUp",  muLumi*3531.8*1000/1934901.,    1., 1, !doDataEff, 0, 0, 1, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
        DMuDYscaleDown.Loop(1, doGen,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );


        if ( doSysRunning){
            //PU
            ZJetsAndDPS DMuDYMix1(lepSelection+"_8TeV_DYJets_MIX_UNFOLDING_dR_5311_Inf3", muLumi*3531.8*1000/30459503., 1., 1, !doDataEff, 1, 1, 1, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax, 0);
            DMuDYMix1.Loop(1, doGen,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            ZJetsAndDPS DMuDYMix2(lepSelection+"_8TeV_DYJets_MIX_UNFOLDING_dR_5311_Inf3", muLumi*3531.8*1000/30459503., 1., 1, !doDataEff, 1,-1, 1, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax, 0);
            DMuDYMix2.Loop(1, doGen,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );

            // jet smearing part
            if ( doGen ) { 
                ZJetsAndDPS DMuDYMix3(lepSelection+"_8TeV_DYJets_MIX_UNFOLDING_dR_5311_Inf3", muLumi*3531.8*1000/30459503., 1., 1, !doDataEff, 4, 1, 1, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax, 0);
                DMuDYMix3.Loop(1, doGen,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            }
            else { // xsec offsets
                ZJetsAndDPS DMuDYMix4(lepSelection+"_8TeV_DYJets_MIX_UNFOLDING_dR_5311_Inf3", muLumi*3531.8*1000/30459503., 1., 1, !doDataEff, 3, 1, 0.04, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax, 0);
                DMuDYMix4.Loop(1, doGen,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );

                ZJetsAndDPS DMuDYMix5(lepSelection+"_8TeV_DYJets_MIX_UNFOLDING_dR_5311_Inf3", muLumi*3531.8*1000/30459503., 1., 1, !doDataEff, 3, -1, 0.04, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax, 0);
                DMuDYMix5.Loop(1, doGen,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );

            } 
            ZJetsAndDPS DMuDYMix6(lepSelection+"_8TeV_DYJets_MIX_UNFOLDING_dR_5311_Inf3", muLumi*3531.8*1000/30459503., 1., 1, !doDataEff, 1, 1, 1, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax, 0);
            DMuDYMix6.Loop(1, doGen,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            ZJetsAndDPS DMuDYMix7(lepSelection+"_8TeV_DYJets_MIX_UNFOLDING_dR_5311_Inf3", muLumi*3531.8*1000/30459503., 1., 1, !doDataEff, 1,-1, 1, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax, 0);
            DMuDYMix7.Loop(1, doGen,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
        }
    }
    /// this is unfolding part for W+jets 
    if ( doWhat == 4 || doWhat == 100 ){
        int doGen = 0 ;
        if ( (lepSelection.find("SE") == 0 || lepSelection.find("SMu") == 0 ) && lepSelection.find("SMuE") == -1 )  doGen = 1 ;

        ZJetsAndDPS DMuWJ(lepSelection+"_8TeV_WJetsALL_UNFOLDING_dR_5311",  muLumi*36703.         *1000/76102995., 1., 1, !doDataEff, 0, 0, 1, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
        DMuWJ.Loop(1, doGen,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
        ZJetsAndDPS DMuWJMix(lepSelection+"_8TeV_WJetsALL_MIX_UNFOLDING_dR_5311", muLumi*36703.         *1000/76102995., 1., 1, !doDataEff, 0, 0, 1, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax, 0);
        DMuWJMix.Loop(1, doGen,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );

        if ( doSysRunning){
            //PU
            ZJetsAndDPS DMuWJMix(lepSelection+"_8TeV_WJetsALL_UNFOLDING_dR_5311", muLumi*36703*1000/76102995., 1., 1, !doDataEff, 1, 1, 1, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax, 0);
            DMuWJMix.Loop(1, doGen,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            ZJetsAndDPS DMuWJMix(lepSelection+"_8TeV_WJetsALL_UNFOLDING_dR_5311", muLumi*36703*1000/76102995., 1., 1, !doDataEff, 1,-1, 1, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax, 0);
            DMuWJMix.Loop(1, doGen,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );

            // jet smearing part
            if ( doGen ) {
                ZJetsAndDPS DMuWJMix(lepSelection+"_8TeV_WJetsALL_UNFOLDING_dR_5311", muLumi*36703*1000/76102995., 1., 1, !doDataEff, 4, 1, 1, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax, 0);
                DMuWJMix.Loop(1, doGen,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            }
            else { // xsec offsets
                ZJetsAndDPS DMuWJMix(lepSelection+"_8TeV_WJetsALL_UNFOLDING_dR_5311", muLumi*36703*1000/76102995., 1., 1, !doDataEff, 3, 1, 0.04, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax, 0);
                DMuWJMix.Loop(1, doGen,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );

                ZJetsAndDPS DMuWJMix(lepSelection+"_8TeV_WJetsALL_UNFOLDING_dR_5311", muLumi*36703*1000/76102995., 1., 1, !doDataEff, 3, -1, 0.04, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax, 0);
                DMuWJMix.Loop(1, doGen,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );

            }
            ZJetsAndDPS DMuWJMix(lepSelection+"_8TeV_WJetsALL_UNFOLDING_dR_5311", muLumi*36703*1000/76102995., 1., 1, !doDataEff, 1, 1, 1, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax, 0);
            DMuWJMix.Loop(1, doGen,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            ZJetsAndDPS DMuWJMix(lepSelection+"_8TeV_WJetsALL_UNFOLDING_dR_5311", muLumi*36703*1000/76102995., 1., 1, !doDataEff, 1,-1, 1, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax, 0);
            DMuWJMix.Loop(1, doGen,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
        }


    }

    if ( doWhat == 51  ){

        // this is setup for sherpa NLO 
        ZJetsAndDPS DESherpaTest2NLO("DE_8TeV_DY_Sherpa_2NLO4_HepMC_dR_Full_List",  eleLumi         * 1000.          , 1.,    0,   0,     0,    0,     1.,  jetPtMin,  jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax );
        //	DESherpaTest2NLO.Loop(0, 1, 0, 0, 0);


        ZJetsAndDPS DESherpaTest1NLO("DE_8TeV_DY_Sherpa_1NLO4_HepMC_dR_Full_List",  eleLumi         * 1000.          , 1.,    0,   0,     0,    0,     1.,  jetPtMin,  jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax );
        DESherpaTest1NLO.Loop(0, 1, 0, 0, 0); 

        ZJetsAndDPS DESherpaTest1NLOScaleDown("DE_8TeV_DY_Sherpa_1NLO4_scaleDown_HepMC_dR_Full_List",  eleLumi         * 1000.          , 1.,    0,   0,     0,    0,     1.,  jetPtMin,  jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax );
        //	DESherpaTest1NLOScaleDown.Loop(0, 1, 0, 0); 

        ZJetsAndDPS DESherpaTest1NLOScaleUp("DE_8TeV_DY_Sherpa_1NLO4_scaleUp_HepMC_dR_Full_List",  eleLumi         * 1000.          , 1.,    0,   0,     0,    0,     1.,  jetPtMin,  jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax );
        //      DESherpaTest1NLOScaleUp.Loop(0, 1, 0, 0); 



        //ZJetsAndDPS DMuSherpa(lepSelection+"_DYJets_Sherpa_mcEveWeight",   muLumi * 3531.8           * 1000 / 30459503.,    1.,  0,  0,  0,  0,  1.,  jetPtMin,  jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax );
        //ZJetsAndDPS DESherpALL("DE_8TeV_Sherpa_HepMC_Z2jetNLO4jetLO_multithread_ALL_dR",  eleLumi         * 1000.          , 1.,    0,   0,     0,    0,     1.,  jetPtMin,  jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax );
        //DESherpALL.Loop(0, 1,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy ); 


        ZJetsAndDPS DMuPowMiNLO("DMu_8TeV_DYJets_PowhegZ2jMiNLO_dR_GEN_Cern",                             muLumi * 1.            * 1000 / 1964662.,    1.013,    0,   0,     0,    0,     1.,  jetPtMin,  jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax   ); 
        DMuPowMiNLO.Loop(0, 1, 0, 0, 0);
        //	ZJetsAndDPS DEPow("DE_8TeV_DYJets_PowhegNLO1Jet_dR_GEN",                             muLumi * 334.            * 1000 / 2948078.,    1.013,    0,   0,     0,    0,     1.,  jetPtMin,  jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax   ); 
        //	DEPow.Loop(0, 1, 0, 0);
        //	ZJetsAndDPS DEPowSU("DE_8TeV_DYJets_PowhegNLO1Jet_dR_ScaleUp_GEN",                             muLumi * 318.4            * 1000 / 5446372.,    1.013,    0,   0,     0,    0,     1.,  jetPtMin,  jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax   ); 
        //	DEPowSU.Loop(0, 1, 0, 0);
        //	ZJetsAndDPS DEPowSD("DE_8TeV_DYJets_PowhegNLO1Jet_dR_ScaleDown_GEN",                             muLumi * 357.2            * 1000 / 5856584.,    1.013,    0,   0,     0,    0,     1.,  jetPtMin,  jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax   ); 
        //	DEPowSD.Loop(0, 1, 0, 0);

        //ZJetsAndDPS DESherpaTest("DE_8TeV_DY_Sherpa_HepMC_dR_Full_List",  eleLumi         * 1000.          , 1.,    0,   0,     0,    0,     1.,  jetPtMin,  jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax );
        //ZJetsAndDPS DESherpaTest("Sherpa\/DE_8TeV_Sherpa_HepMC_num*",  eleLumi         * 1000.          , 1.,    0,   0,     0,    0,     1.,  jetPtMin,  jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax );
        //DESherpaTest.Loop(0, 1,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy ); 

    }

    if (doWhat == 90) {
        for (int pdfMember(1); pdfMember <= 5; pdfMember++) {
            ZJetsAndDPS DMuDYMixPDF(lepSelection+"_8TeV_DYJets_MIX_UNFOLDING_dR_5311_Inf3", muLumi*3531.8*1000/30459503., 1., 1, !doDataEff, 0, 0, 1, jetPtMin, jetPtMax, ZEtaMin,    ZEtaMax, -30);
            DMuDYMixPDF.Loop(0, 1,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, 0, 0, 1, 0, "NNPDF20_as_0118_100.LHgrid", pdfMember);
            //DMuDYMixPDF.Loop(0, 1,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, 0, 0, 1, 0);
        }
    }

}

