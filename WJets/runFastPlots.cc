{
    string srcdir = "Sources/";

    vector<string> sources;
    sources.push_back("getFilesAndHistograms");
    sources.push_back("functions");
    sources.push_back("writeFastPlotsTex");
    sources.push_back("unfoldingFunctions");
    sources.push_back("FastPlots");

    //--- Load shaared libraries ---
    unsigned int nSources = sources.size();
    for (unsigned int i(0); i < nSources; i++){
        cout <<"Compiling " << srcdir + sources[i] << ".cc" << endl;
        gROOT->ProcessLine(string(".L " + srcdir + sources[i] + ".cc++").c_str());
        //gROOT->LoadMacro(string(srcdir + sources[i] + ".cc++").c_str());
    }

    //FastPlots("DMu", "FirstJetPt_Zinc1jet");
    //FastPlots("DMu", "FirstJetPt_2_Zinc1jet");
    FastPlots("DE", "SecondJetPt_2_Zinc2jet");
    FastPlots("DE", "JetsHT_2_Zinc1jet");
    FastPlots("DE", "JetsHT_2_Zinc2jet");
    //FastPlots("DE");
    //FastPlots("FirstJetPt_1_Zinc1jet");
    //FastPlots("FirstJetPt_2_Zinc1jet");
    //FastPlots("FirstJetPt_3_Zinc1jet");
    //FastPlots("FirstJetPt_4_Zinc1jet");

    /*
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
    */

}
