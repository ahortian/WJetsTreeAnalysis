TH1::SetDefaultSumw2();
TH2::SetDefaultSumw2();

//string jSpt = "#Delta_{pT}^{rel}(j_{1}j_{2})";

TH1D *genZNGoodJetsEWK_Zexc = newHisto("genZNGoodJetsEWK_Zexc","Jet Counter (excl.)", "N_{jets}", 11, -0.5, 10.5);
genZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(1,"N_{jets} = 0");
genZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(2,"N_{jets} = 1");
genZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(3,"N_{jets} = 2");
genZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(4,"N_{jets} = 3");
genZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(5,"N_{jets} = 4");
genZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(6,"N_{jets} = 5");
genZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(7,"N_{jets} = 6");
genZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(8,"N_{jets} = 7");
genZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(9,"N_{jets} = 8");
genZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(10,"N_{jets} = 9");
genZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(11,"N_{jets} = 10");
TH1D *genZNGoodJetsEWKbtw_Zexc = newHisto("genZNGoodJetsEWKbtw_Zexc","Between: Jet Counter (excl.)", "N_{jets}", 11, -0.5, 10.5);
genZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(1,"N_{jets} = 0");
genZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(2,"N_{jets} = 1");
genZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(3,"N_{jets} = 2");
genZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(4,"N_{jets} = 3");
genZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(5,"N_{jets} = 4");
genZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(6,"N_{jets} = 5");
genZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(7,"N_{jets} = 6");
genZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(8,"N_{jets} = 7");
genZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(9,"N_{jets} = 8");
genZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(10,"N_{jets} = 9");
genZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(11,"N_{jets} = 10");

// MET properties
//TH1D *genMETEWK_Zinc2jet                      = newHisto("genMETEWK_Zinc2jet",                      "Gen:MET (N_{jets} #geq 2)",  "Gen:MET [GeV]",      200,0.,500 );
//TH1D *genMETEWK_Zinc3jet                      = newHisto("genMETEWK_Zinc3jet",                      "Gen:MET (N_{jets} #geq 3)",  "Gen:MET [GeV]",      200,0.,500 );

//TH1D *genMETphiEWK_Zinc2jet                      = newHisto("genMETphiEWK_Zinc2jet",                      "Gen:MET #phi (N_{jets} #geq 2)",  "Gen:#phi(MET)",      100,-PI ,PI );
//TH1D *genMETphiEWK_Zinc3jet                      = newHisto("genMETphiEWK_Zinc3jet",                      "Gen:MET #phi (N_{jets} #geq 3)",  "Gen:#phi(MET)",      100,-PI ,PI );

//TH1D *genMTEWK_Zinc2jet                      = newHisto("genMTEWK_Zinc2jet",                      "Gen:MT (N_{jets} #geq 2)",    "Gen:MT [GeV]",    200,0.,400 );
//TH1D *genMTEWK_Zinc3jet                      = newHisto("genMTEWK_Zinc3jet",                      "Gen:MT (N_{jets} #geq 3)",    "Gen:MT [GeV]",    200,0.,400 );



/// jet properties
/*int nJetPtEWK_Zinc2jet(15);
double jetPtEWK_Zinc2jet[16] = {15, 20, 24, 30, 38, 50, 68, 88, 113, 144, 184, 234, 297, 377, 480, 700 };
int nJetPtEWKbtw_Zinc2jet(12);
double jetPtEWKbtw_Zinc2jet[13] = {15, 20, 24, 30, 38, 50, 68, 88, 113, 144, 184, 234, 297 };
int nJetHTEWK_Zinc2jet(15);
double jetHTEWK_Zinc2jet[16] = {60, 80, 95, 121, 155, 200, 260, 336, 436, 566, 737, 962, 1200, 1500, 2000, 2500};
*/
TH1D *genFirstJetPtEWK_Zinc2jet                 = newHisto("genFirstJetPtEWK_Zinc2jet",                 "Gen:1st jet p_{T} (N_{jets} #geq 2)",             "Gen:p_{T}(j_{1}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);
TH1D *genSecondJetPtEWK_Zinc2jet                = newHisto("genSecondJetPtEWK_Zinc2jet",                "Gen:2nd jet p_{T} (N_{jets} #geq 2)",             "Gen:p_{T}(j_{2}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);

TH1D *genFirstJetEtaEWK_Zinc2jet                = newHisto("genFirstJetEtaEWK_Zinc2jet",                "Gen:1st jet #eta (N_{jets} #geq 2)",              "Gen:#eta(j_{1})",  24,-4.8, 4.8);
TH1D *genSecondJetEtaEWK_Zinc2jet               = newHisto("genSecondJetEtaEWK_Zinc2jet",               "Gen:2nd jet #eta (N_{jets} #geq 2)",              "Gen:#eta(j_{2})",  24,-4.8, 4.8);
TH1D *genFirstJetPhiEWK_Zinc2jet                = newHisto("genFirstJetPhiEWK_Zinc2jet",                "Gen:1st jet #phi (N_{jets} #geq 2)",              "Gen:#phi(j_{1})",  30,-PI, PI);
TH1D *genSecondJetPhiEWK_Zinc2jet               = newHisto("genSecondJetPhiEWK_Zinc2jet",               "Gen:2nd jet #phi (N_{jets} #geq 2)",              "Gen:#phi(j_{2})",  30,-PI, PI);

TH1D *genForwardJetPtEWK_Zinc2jet                 = newHisto("genForwardJetPtEWK_Zinc2jet",                 "Gen:Fwd jet p_{T} (N_{jets} #geq 2)",             "Gen:p_{T}(j_{1}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);
TH1D *genForwardJetEtaEWK_Zinc2jet                = newHisto("genForwardJetEtaEWK_Zinc2jet",                "Gen:Fwd jet #eta (N_{jets} #geq 2)",              "Gen:#eta(j_{1})",  24,-4.8, 4.8);
TH1D *genForwardJetPhiEWK_Zinc2jet                = newHisto("genForwardJetPhiEWK_Zinc2jet",                "Gen:Fwd jet #phi (N_{jets} #geq 2)",              "Gen:#phi(j_{1})",  30,-PI, PI);

TH1D *genCentralJetPtEWK_Zinc2jet                 = newHisto("genCentralJetPtEWK_Zinc2jet",                 "Gen:Cnt jet p_{T} (N_{jets} #geq 2)",             "Gen:p_{T}(j_{1}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);
TH1D *genCentralJetEtaEWK_Zinc2jet                = newHisto("genCentralJetEtaEWK_Zinc2jet",                "Gen:Cnt jet #eta (N_{jets} #geq 2)",              "Gen:#eta(j_{1})",  24,-4.8, 4.8);
TH1D *genCentralJetPhiEWK_Zinc2jet                = newHisto("genCentralJetPhiEWK_Zinc2jet",                "Gen:Cnt jet #phi (N_{jets} #geq 2)",              "Gen:#phi(j_{1})",  30,-PI, PI);





TH1D *genThirdJetPtEWK_Zinc2jet                = newHisto("genThirdJetPtEWK_Zinc2jet",                "Gen:Ext 3nd jet p_{T} (N_{jets} #geq 2)",             "Gen:p_{T}(j_{3}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);
TH1D *genThirdJetPhiEWK_Zinc2jet                = newHisto("genThirdJetPhiEWK_Zinc2jet",              "Gen:Ext 3nd jet #Phi (N_{jets} #geq 2)",             "Gen:#Phi(j_{3}) ",     30, -PI, PI);
TH1D *genThirdJetEtaEWK_Zinc2jet               = newHisto("genThirdJetEtaEWK_Zinc2jet",               "Gen:Ext 3nd jet #eta (N_{jets} #geq 2)",              "Gen:#eta(j_{3})",  24,-4.8, 4.8);
/// additional activity > 15 GeV full space
TH1D *genThirdJetPtEWKadd_Zinc2jet                = newHisto("genThirdJetPtEWKadd_Zinc2jet",                "Gen:Ext pt#ge 15 3nd jet p_{T} (N_{jets} #geq 2)",             "Gen:p_{T}(j_{3}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);
TH1D *genThirdJetPhiEWKadd_Zinc2jet                = newHisto("genThirdJetPhiEWKadd_Zinc2jet",                "Gen:Ext 3nd pt#ge 15 jet p_{T} (N_{jets} #geq 2)",             "Gen:#Phi(j_{3}) ",     30, -PI, PI);
TH1D *genThirdJetEtaEWKadd_Zinc2jet               = newHisto("genThirdJetEtaEWKadd_Zinc2jet",               "Gen:Ext 3nd pt#ge 15 jet #eta (N_{jets} #geq 2)",              "Gen:#eta(j_{3})",  24,-4.8, 4.8);


/////  dijet properties
//string jSEta = "#eta(j_{1})+ #eta(j_{2})";
TH1D *genSumEtaJetsEWK_Zinc2jet             = newHisto("genSumEtaJetsEWK_Zinc2jet",             "Gen:|#Sigma| #eta of j1,j2 jets (N_{jets} #geq 2)",                                       jSEta,           50, 0, 10);
TH1D *genAbsSumEtaJetsEWK_Zinc2jet             = newHisto("genAbsSumEtaJetsEWK_Zinc2jet",       "Gen:#Sigma |#eta| of j1,j2 jets (N_{jets} #geq 2)",                                       jSEta,           50, 0, 10 );
TH1D *genSumEtaJetsEWK_Zinc3jet             = newHisto("genSumEtaJetsEWK_Zinc3jet",             "Gen:sum of #eta of j1,j2 jets (N_{jets} #geq 3)",                                       jSEta,           50, -8, 8);


TH1D *gendEtaJetsEWK_Zinc2jet             = newHisto("gendEtaJetsEWK_Zinc2jet",             "Gen:#Delta#eta btwn jets (N_{jets} #geq 2)",                                       jdEta,           50, 0, 10);

TH1D *genTwoJetsPtDiffEWK_Zinc2jet        = newHisto("genTwoJetsPtDiffEWK_Zinc2jet",        "Gen:pT diff of the two highest jet (N_{jets} #geq 2)",                             "Gen:#Delta pT(j_{1}j_{2}) [GeV]",      50,  0, 500);


TH1D *genptBalEWK_Zinc2jet                = newHisto("genptBalEWK_Zinc2jet",                "Gen:Vectorial pT sum: Z_{pT} + DiJet_{pT} (N_{jets} #geq 2)",                       "Gen:#Sigma pT [GeV]",      50,  0, 500);
TH1D* genSpTJetsEWK_Zinc2jet              = newHisto("genSpTJetsEWK_Zinc2jet",              "Gen:#Delta_{pT}^{rel} jets (N_{jets} #geq 2)",                                jSpt,   50,  0, 1);

TH1D *gendPhiJetsEWK_Zinc2jet             = newHisto("gendPhiJetsEWK_Zinc2jet",             "Gen:#Delta#phi btwn jets (N_{jets} #geq 2)",                                       jdPhi,           100,  0, PI);


TH1D *genJetsHTEWK_Zinc2jet                     = newHisto("genJetsHTEWK_Zinc2jet",                     "Gen:Scalar sum jets p_{T} (N_{jets} #geq 2)",     HT,     nJetHTEWK_Zinc2jet, jetHTEWK_Zinc2jet);


TH1D *genJetsMassEWK_Zinc2jet             = newHisto("genJetsMassEWK_Zinc2jet",             "Gen:2Jets Invariant Mass (N_{jets} #geq 2)",                                       Mjj, 50, 20, 2620);


//// third jet properties
//string  j3Eta = "#eta(j_{3})";
TH1D *genEtaThirdJetsEWK_Zinc3jet             = newHisto("genEtaThirdJetsEWK_Zinc3jet",             "Gen:Third jet #eta in dijet frame (N_{jets} #geq 3)",                                       j3Eta,           50, -5, 5);

/// all jets between leading two jets 
TH1D *genAllJetPtEWKbtw_Zinc2jet                = newHisto("genAllJetPtEWKbtw_Zinc2jet",                "Gen:Ext,btw All jet p_{T} (N_{jets} #geq 2)",             "Gen:p_{T}(j_{3}) [GeV]",     nJetPtEWKbtw_Zinc2jet, jetPtEWKbtw_Zinc2jet);
TH1D *genAllJetPhiEWKbtw_Zinc2jet               = newHisto("genAllJetPhiEWKbtw_Zinc2jet",               "Gen:Ext,btw All jet #phi (N_{jets} #geq 2)",              "Gen:#phi(j_{3})",  30,-PI, PI);
TH1D *genAllJetEtaEWKbtw_Zinc2jet               = newHisto("genAllJetEtaEWKbtw_Zinc2jet",               "Gen:Ext,btw All jet #eta (N_{jets} #geq 2)",              "Gen:#eta(j_{3})",  24,-4.8, 4.8);


//double jetHTEWKbtw_Zinc2jet[16] = {0,15, 20, 30, 42, 60, 80, 95, 121, 155, 200, 260, 336, 436, 566, 737};
TH1D *genJetsHTEWKbtw_Zinc2jet                  = newHisto("genJetsHTEWKbtw_Zinc2jet",                  "Gen:Ext,btw Scalar sum jets p_{T} (N_{jets} #geq 2)",     HT,     nJetHTEWK_Zinc2jet, jetHTEWKbtw_Zinc2jet);
//TH1D *genJetsHTEWKbtw_Zinc2jet                     = newHisto("genJetsHTEWKbtw_Zinc2jet",                     "Gen:Ext,btw Scalar sum jets p_{T} (N_{jets} #geq 2)",     HT,     nJetHTEWK_Zinc2jet, jetHTEWK_Zinc2jet);

// third jet between
TH1D *genThirdJetPtEWKbtw_Zinc2jet                = newHisto("genThirdJetPtEWKbtw_Zinc2jet",                "Gen:Ext,btw 3nd jet p_{T} (N_{jets} #geq 2)",             "Gen:p_{T}(j_{3}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);
TH1D *genThirdJetPhiEWKbtw_Zinc2jet               = newHisto("genThirdJetPhiEWKbtw_Zinc2jet",               "Gen:Ext,btw 3nd jet #phi (N_{jets} #geq 2)",              "Gen:#phi(j_{3})",  30,-PI, PI);
TH1D *genThirdJetEtaEWKbtw_Zinc2jet               = newHisto("genThirdJetEtaEWKbtw_Zinc2jet",               "Gen:Ext,btw 3nd jet #eta (N_{jets} #geq 2)",              "Gen:#eta(j_{3})",  24,-4.8, 4.8);

