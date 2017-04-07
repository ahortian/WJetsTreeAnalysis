#ifndef __variablesOfInterest__
#define __variablesOfInterest__

#include <TObject.h>
#include <TList.h>

struct variableStruct{
   string name;
   bool log, decrease;  
   int MuSVDkterm, MuBayeskterm, ESVDkterm, EBayeskterm;
};

int NVAROFINTEREST(54);

variableStruct VAROFINTEREST[] = {
   //--  Name  ------------------ log - decrease - Mu SVD kterm - Mu Bayes kterm - E SVD kterm - E Bayes kterm
   {"ZNGoodJets_Zexc",             1,      1,           4,              4,             4,            4},

   //-- jet Pt distribution
   {"FirstJetPt_Zinc1jet",         1,      1,          15,              4,            15,            4}, 
   {"SecondJetPt_Zinc2jet",        1,      1,          12,              4,            12,            5}, 
   {"ThirdJetPt_Zinc3jet",         1,      1,           8,              4,             6,            3}, 
   {"FourthJetPt_Zinc4jet",        1,      1,           5,              3,             3,            4}, 
   {"FifthJetPt_Zinc5jet",         1,      1,           4,              2,             3,            4},

   {"FirstHighestJetPt_Zinc1jet",  1,      1,          15,              4,            15,            4},
   {"FirstHighestJetPt_Zinc2jet",  1,      1,          13,              5,            12,            6},
   {"FirstHighestJetPt_Zinc3jet",  1,      1,           5,              4,             8,           10},
   {"FirstHighestJetPt_Zinc4jet",  1,      0,           3,              5,             2,            6},
   {"FirstHighestJetPt_Zinc5jet",  1,      0,           3,              1,             3,            4},

   {"SecondHighestJetPt_Zinc2jet", 1,      1,          12,              4,            12,            5},
   {"SecondHighestJetPt_Zinc3jet", 1,      1,           9,              7,             8,            5},
   {"SecondHighestJetPt_Zinc4jet", 1,      1,           3,              4,             5,            5},
   {"SecondHighestJetPt_Zinc5jet", 1,      0,           2,              4,             2,            4},

   {"ThirdHighestJetPt_Zinc3jet",  1,      1,           8,              4,             6,            8},
   {"ThirdHighestJetPt_Zinc4jet",  1,      1,           5,              4,             5,            4},
   {"ThirdHighestJetPt_Zinc5jet",  1,      1,           3,              4,             2,            4},

   //-- jet Eta distribution
   {"FirstJetEta_Zinc1jet",        0,      1,           9,              4,            16,            4},
   {"SecondJetEta_Zinc2jet",       0,      1,           5,              5,             2,            5}, 
   {"ThirdJetEta_Zinc3jet",        0,      1,           5,              5,             4,            6}, 
   {"FourthJetEta_Zinc4jet",       0,      1,           5,              6,             2,            6}, 
   { "FifthJetEta_Zinc5jet",       0,      1,           5,              6,             2,            6}, // check this 

   //-- jets HT distribution
   {"JetsHT_Zinc1jet",             1,      1,           9,              4,            10,            8}, 
   {"JetsHT_Zinc2jet",             1,      1,          10,              5,             8,            9},
   {"JetsHT_Zinc3jet",             1,      1,          10,              7,            11,            5},
   {"JetsHT_Zinc4jet",             1,      1,           7,              4,             7,            8},
   {"JetsHT_Zinc5jet",             1,      1,           4,              3,             3,            3},

   //-- Z Mass distribution
   {"ZMass_Zinc0jet",              0,      1,          19,             18,             7,           36},

   //-- Jets Mass distribution
   {"JetsMass_Zexc2jet",           0,      1,           7,              9,             7,            7},
   {"JetsMass_Zinc2jet",           0,      1,           7,              9,             7,            6},

   //-- DPS distribution
   {"SpTJets_Zexc2jet",            0,      0,           3,              5,             4,            8},
   {"SpTJets_Zinc2jet",            0,      0,           3,              6,             4,            7},
   {"SpT_Zexc2jet",                0,      0,           6,              6,             6,            5},
   {"SpT_Zinc2jet",                0,      0,           6,              6,             6,            4},
   {"dPhiJets_Zexc2jet",           0,      0,           5,              5,             5,            4},
   {"dPhiJets_Zinc2jet",           0,      0,           5,              5,             6,            4},
   {"PHI_Zexc2jet",                0,      0,           4,              6,             4,            7},
   {"PHI_Zinc2jet",                0,      0,           9,              6,             2,            6},
   {"PHI_T_Zexc2jet",              0,      0,           5,              9,             5,            7},
   {"PHI_T_Zinc2jet",              0,      0,           5,              9,             4,            6},
   {"SPhi_Zexc2jet",               0,      0,          18,              6,             3,            6},
   {"SPhi_Zinc2jet",               0,      0,           7,              6,             6,            6},

   {"SpTJets_LowPt_Zexc2jet",            0,      1,           4,              8,             3,           11},
   {"SpTJets_LowPt_Zinc2jet",            0,      1,           4,              9,             3,            9},
   {"SpT_LowPt_Zexc2jet",                0,      1,           4,              8,             3,            9},
   {"SpT_LowPt_Zinc2jet",                0,      1,           6,              9,             4,            8},
   {"dPhiJets_LowPt_Zexc2jet",           0,      0,           4,              7,             8,            7},
   {"dPhiJets_LowPt_Zinc2jet",           0,      0,           4,              8,             2,            7},
   {"PHI_LowPt_Zexc2jet",                0,      1,           7,              5,             4,           15},
   {"PHI_LowPt_Zinc2jet",                0,      1,           5,              5,             2,           15},
   {"PHI_T_LowPt_Zexc2jet",              0,      0,           3,              6,             3,            5},
   {"PHI_T_LowPt_Zinc2jet",              0,      0,           3,              5,             3,            5},
   {"SPhi_LowPt_Zexc2jet",               0,      0,          11,              9,             3,            4},
   {"SPhi_LowPt_Zinc2jet",               0,      0,           8,             10,             6,            6},
   // additional correlations  =----- NEED TP CHECK THE UNFOLDING PARAMTERS !!!!
   {"dEtaJets_Zinc2jet",                 0,      0,           8,             10,             6,            6},
   {"dEtaJets_Zexc2jet",                 0,      0,           8,             10,             6,            6},
   {"dEtaBosonJet_Zinc1",                 0,      0,           8,             10,             6,            6},
   {"dEtaBosonJet_Zexc1",                 0,      0,           8,             10,             6,            6},
   {"dEtaFirstJetZ_Zinc2jet",                 0,      0,           8,             10,             6,            6},
   {"dEtaSecondJetZ_Zinc2jet"                 0,      0,           8,             10,             6,            6},

}; 

TList *LISTOFVAROFINTEREST = new TList();

void makeLISTOFVAROFINTEREST(){
   for (int i(0); i < NVAROFINTEREST; i++){
      TObjString *name = new TObjString(VAROFINTEREST[i].name.c_str());
      LISTOFVAROFINTEREST->Add(name);
   }
}

#endif

