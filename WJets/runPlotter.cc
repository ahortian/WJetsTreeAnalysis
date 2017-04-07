{
    string srcdir = "Sources/";

    vector<string> sources;
    sources.push_back("getFilesAndHistograms");
    sources.push_back("Plotter");

    //--- Load shared libraries ---
    unsigned int nSources = sources.size();
    for (unsigned int i(0); i < nSources; i++){
        cout << "Compiling " << srcdir + sources[i] << ".cc" << endl;
        gROOT->ProcessLine(string(".L " + srcdir + sources[i] + ".cc++").c_str());
    }

    Plotter("Muons", 30);
    //Plotter("Electrons", 20, 0);
    //Plotter("Muons", 30, 0);
    //Plotter("Electrons", 20, 0);

    //Plotter("Muons",30,0,0);
    //Plotter("SMuE",30,0);
    //Plotter("Electrons");
    cout << " Plotter done" << endl;
    // you can immediately produce also the tables for jet multiplicity
    //getStatistics("SMuE", 30);
    //getStatistics("Muons", 30);
    //getStatistics("Electrons", 30);


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
