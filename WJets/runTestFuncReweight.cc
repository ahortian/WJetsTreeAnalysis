{
    string srcdir = "Sources/";
    vector<string> sources;
    sources.push_back("functions");
    sources.push_back("funcReweightResp");
    
    //--- Load shaared libraries ---
    unsigned int nSources = sources.size();
    gROOT->ProcessLine(".L /afs/cern.ch/cms/slc5_amd64_gcc434/external/lhapdf/5.8.5/lib/libLHAPDF.so");
    for (unsigned int i(0); i < nSources; i++){
        cout <<"Compiling " << srcdir + sources[i] << ".cc" << endl;
        gROOT->ProcessLine(string(".L " + srcdir + sources[i] + ".cc++").c_str());
        }
        
        double weight = 1.02;
        string variable1 = "MeanNJetsHT_Zinc1jet";
        //string variable2 = "MeanNJetsHT_Zinc2jet";
        //string variable3 = "MeanNJetsdRapidity_Zinc2jet";
        //string variable4 = "MeanNJetsdRapidityFB_Zinc2jet";
        
        vector<double> vecFwHT1;
        vecFwHT1 = getFW(variable1.c_str());
        
        vector<double> binEdgeHT1;
        binEdgeHT1 = getXBin(variable1.c_str());
        
        double WforMeanNHT1 = ReweightForResp(vecFwHT1, binEdgeHT1, weight, 100.0);
        //double WforMeanNHT2 = ReweightForResp(vecFw, binEdge, weight, 100.0);
        //double WforMeanNdRap12 = ReweightForResp(vecFw, binEdge, weight, 2.0);
        //double WforMeanNdRapFB = ReweightForResp(vecFw, binEdge, weight, 2.0);
        
        
        
        cout << "initial : " << weight << "  new weight : " << WforMeanNHT1 << endl;
        
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

