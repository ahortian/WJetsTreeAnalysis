#ifndef __fileNames__
#define __fileNames__

//-- directory of input root files --------------------
const string FILESDIRECTORY("HistoFiles/");
//---------- lets add basic information on samples inot common struct -------------------------------------------
struct processInfoStruct{
    string filename;
    double NEvents, xsec, xsecFrac, xsecError;
    int color;
    string legend;
};

const int NFILESWJETS(10); 
const int NFilesFull(28); 
const processInfoStruct ProcessInfo[] = {
    //--  Name  ---------------------------- #events ---- xsec - branch - xsec error (%) -- color for plot -- name on legend
    {"Data_dR_5311_List",                          1,          1.,      1,         1,             kBlack,      " Data"},   // 0
    {"WJetsALL_MIX_UNFOLDING_dR_5311_List",        76102995.,  36864.,  1,         0.03,          kOrange,       " W #rightarrow #mu#nu"},
    {"ZZ_dR_5311_List",	                          9799908.,   17.654,  1,         0.04,          kOrange,     " ZZ"},
    {"WZ_dR_5311_List",	                          10000283.,  33.21,   1,         0.05,          kRed+1,      " WZ"},	
    {"WW_dR_5311_List",	                          10000431.,  54.838,  1,         0.05,          kViolet+5,   " WW"},
    {"VV_dR_5311_List",	                          10000431.,  54.838,  1,         0.05,          kRed+1,   " VV"},

    {"ST_s_channel_dR_5311_List",	                  259961.,    3.79,    1,         0.10,          kMagenta,    " STs"},     // 6
    {"ST_tW_top_channel_dR_5311_List",	                  3758227.,   56.4,    1,         0.10,          kMagenta,    " STtWtop"},
    {"ST_tW_antitop_channel_dR_5311_List",	              497658.,    11.1,    1,         0.10,          kMagenta,    " STtWantitop"},
    {"TTJets_dR_5311_List",		                  6923652.,   234.,    1,         0.10,          kBlue,       " t#bar{t}"}, // 9
    {"DYJets50toInf_dR_5311_List",     30459503.,  3531.8,  1,         0.04,          kGreen-8,   " DYJets"}, // 10 /// up to this line files are set for W+jet s and TTbar
    {"Tbar_s_channel_dR_5311_List",                139974.,    1.76,    1,         0.10,          kMagenta,    " Tbars"},
    {"Tbar_t_channel_dR_5311_List",                1903681.,   30.7,    1,         0.10,          kMagenta,    " Tbart"},
    {"Tbar_tW_channel_dR_5311_List",               493460.,    11.1,    1,         0.10,          kMagenta,    " TbartW"},
  
    {"DYJets10to50_dR_5311_List",	              11707222.,  860.5,   1,         0.04,          kAzure-4,    " DY"},
    
    {"ZZJets2L2Nu_dR_5311_List",		              954911.,    17.654,  0.04039,   0.04,          kSpring+5,   " ZZJets2L2Nu"},
    {"ZZJets4L_dR_5311_List",		              4807893.,   17.654,  0.010196,  0.04,          kOrange,     " ZZJets4L"},
    {"ZZJets2L2Q_dR_5311_List",		              1936727.,   17.654,  0.14118,   0.04,          kOrange-3,   " ZZJets2L2Q"},
    {"WZJets3LNu_dR_5311_List",		              1995334.,   33.21,   0.032887,  0.04,          kRed+1,      " WZJets3LNu"},
    {"WZJets2L2Q_dR_5311_List",		              3215990.,   33.21,   0.068258,  0.04,          kPink-6,     " WZJets2L2Q"},
    {"WWJets2L2Nu_dR_5311_List",		              1933235.,   54.838,  0.10608 ,  0.04,          kViolet+5,   " WWJets2L2Nu"},
    {"Top_dR_5311_List",		                      1.,         1,       1,         0.04,          kMagenta,    " Single top"},  // 21

    {"DYJets_FromTau_UNFOLDING_dR_5311_List_Inf3", 30459503.,  3531.8,  1,         0.033,         kAzure+4,    " DYtautau"},
    {"DYJets10toInf3_dR_5311_List",	              1.,         1,       1,         0.04,          kGreen-8,   " DYJets"},          // 23
    {"DataQCD_dR_5311_List",	                      1.,         1,       1,         0.04,          kGreen+3,     " QCD"},         // 24
    {"WJetsALL_MIX_UNFOLDING_dR_5311_List",        76102995.,  36864.,  1,         0.03,          kOrange,       " W #rightarrow #mu#nu"},       // 25
    {"WJetsALL_MIX_dR_5311_List",                  76102995.,  36864.,  1,         0.03,          kPink,       " WJets"},  // relative weight for mixed DY and WJ files are set inthe code
    {"WJetsALL_dR_5311_List",                      76102995.,  36864.,  1,         0.03,          kPink,       " WJets"},
    {"DYJets_UNFOLDING_dR_5311_List_Inf3",         30459503.,  3531.8,  1,         0.04,          kAzure+10,   " DYJets"}, /// up to this line files are set for W+jet s and TTbar
};

//--- first element must point to the data
//--- last element must point to the MC Signal
const int NFILESDYJETS(12);
//const int FilesDYJets[NFILESDYJETS] = {0, 1, 21, 15, 20, 14, 16, 19, 18, 17, 11, 13};
const int FilesDYJets[NFILESDYJETS] = {0, 1, 21, 15, 20, 14, 16, 19, 18, 17, 11, 27};
const int NFILESTTBAR(7);
const int FilesTTbarWJets[NFILESTTBAR] = {0, 5, 24, 21, 10, 9, 25};
const int FilesTTbar[NFILESTTBAR] = {0, 5, 24, 21, 10, 9, 25};
const int NFILESWJETS_M(11);
const int FilesWJets_M[NFILESWJETS_M] = {0, 2, 3, 4, 23, 5, 6, 7, 8, 9, 24};
const int DATAFILENAME(0);
const int DYMADGRAPHFILENAME(13);

//-- file names ---------------------------------------
const string FILENAMESTTbar[NFILESTTBAR] = {
    "Data_dR_5311_List",
    "VV_dR_5311_List",
    "DataQCD_dR_5311_List",
    "Top_dR_5311_List",
    "DYJets10toInf3_dR_5311_List",
    "TTJets_dR_5311_List",
    "WJetsALL_UNFOLDING_dR_5311_List",
};
const int NFILESVJETS(14);
const string FILENAMES[NFILESVJETS] = {
    "Data_dR_5311_List",
    "WJetsALL_MIX_dR_5311_List",
    "DYJets_FromTau_UNFOLDING_dR_5311_List_Inf3",
    "Top_dR_5311_List",
    "ZZJets2L2Nu_dR_5311_List",
    "ZZJets4L_dR_5311_List",
    "ZZJets2L2Q_dR_5311_List",
    "WZJets3LNu_dR_5311_List",
    "WZJets2L2Q_dR_5311_List",
    "WWJets2L2Nu_dR_5311_List",
    "TTJets_dR_5311_List",
    //"DYJets_UNFOLDING_NOLEPTON_5311_List",
    //"DYJets_UNFOLDING_dR_5311_List_Inf3",
    "DYJets_MIX_UNFOLDING_dR_5311_List_Inf3",
    // "DY01234Jets_UNFOLDING_dR_5311_List_Inf3",
    //"DYJets_PowhegNLO1Jet_GEN",
    "DYJets_PowhegZ2jMiNLO_dR_GEN_CernMOJ",
    //"DYJets_UNFOLDING_Sherpa",
    "DY_Sherpa_1NLO4_HepMC_dR_Full_List"
}; 
//-----------------------------------------------------


const string DYPOWHEGFILENAME = FILENAMES[12];
const string DYSHERPAFILENAME = FILENAMES[13];
const string DYSHERPA2NLOFILENAME ="DY_Sherpa_2NLO4_HepMC_dR_Full_ListALL";

const string BGFILENAMES[9] = {
    FILENAMES[1],
    FILENAMES[2],
    FILENAMES[3],
    FILENAMES[4],
    FILENAMES[5],
    FILENAMES[6],
    FILENAMES[7],
    FILENAMES[8],
    FILENAMES[9],
};
const string BGFILENAMESGrouped[8] = {
    FILENAMES[1],
    FILENAMES[2],
    FILENAMES[3],
    FILENAMES[4],
    FILENAMES[5],
    FILENAMES[6],
    FILENAMES[7],
    FILENAMES[9],
};

const string GenMCFILENAMES[5] = {
    "DYJets_GEN_Z2_MPIoff",
    "DYJets_GEN_Z2Star",
    "DYJets_GEN_Z2Star_MPIoff",
    "DYJets_P8_ZmumJets_MPIon_GEN_Tune4C",
    "DYJets_PowhegZjjMiNLO_GEN"
};
const string DYPOWHEGZjjFILENAME = GenMCFILENAMES[4];
const string DYSHERPAZjFILENAME("DYJets_Sherpa_UNFOLDING_dR_5311_List_Inf");

#endif


