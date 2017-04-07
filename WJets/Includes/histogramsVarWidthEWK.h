TH1::SetDefaultSumw2();
TH2::SetDefaultSumw2();

//string jSpt = "#Delta_{pT}^{rel}(j_{1}j_{2})";

TH1D *ZNGoodJetsEWK_Zexc = newHisto("ZNGoodJetsEWK_Zexc","Jet Counter (excl.)", "N_{jets}", 11, -0.5, 10.5);
ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(1,"N_{jets} = 0");
ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(2,"N_{jets} = 1");
ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(3,"N_{jets} = 2");
ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(4,"N_{jets} = 3");
ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(5,"N_{jets} = 4");
ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(6,"N_{jets} = 5");
ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(7,"N_{jets} = 6");
ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(8,"N_{jets} = 7");
ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(9,"N_{jets} = 8");
ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(10,"N_{jets} = 9");
ZNGoodJetsEWK_Zexc->GetXaxis()->SetBinLabel(11,"N_{jets} = 10");
TH1D *ZNGoodJetsEWKbtw_Zexc = newHisto("ZNGoodJetsEWKbtw_Zexc","Between: Jet Counter (excl.)", "N_{jets}", 11, -0.5, 10.5);
ZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(1,"N_{jets} = 0");
ZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(2,"N_{jets} = 1");
ZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(3,"N_{jets} = 2");
ZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(4,"N_{jets} = 3");
ZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(5,"N_{jets} = 4");
ZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(6,"N_{jets} = 5");
ZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(7,"N_{jets} = 6");
ZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(8,"N_{jets} = 7");
ZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(9,"N_{jets} = 8");
ZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(10,"N_{jets} = 9");
ZNGoodJetsEWKbtw_Zexc->GetXaxis()->SetBinLabel(11,"N_{jets} = 10");

// MET properties
TH1D *METEWK_Zinc2jet                      = newHisto("METEWK_Zinc2jet",                      "MET (N_{jets} #geq 2)",  "MET [GeV]",      200,0.,500 );
TH1D *METEWK_Zinc3jet                      = newHisto("METEWK_Zinc3jet",                      "MET (N_{jets} #geq 3)",  "MET [GeV]",      200,0.,500 );

TH1D *METphiEWK_Zinc2jet                      = newHisto("METphiEWK_Zinc2jet",                      "MET #phi (N_{jets} #geq 2)",  "#phi(MET)",      100,-PI ,PI );
TH1D *METphiEWK_Zinc3jet                      = newHisto("METphiEWK_Zinc3jet",                      "MET #phi (N_{jets} #geq 3)",  "#phi(MET)",      100,-PI ,PI );

TH1D *MTEWK_Zinc2jet                      = newHisto("MTEWK_Zinc2jet",                      "MT (N_{jets} #geq 2)",    "MT [GeV]",    200,0.,400 );
TH1D *MTEWK_Zinc3jet                      = newHisto("MTEWK_Zinc3jet",                      "MT (N_{jets} #geq 3)",    "MT [GeV]",    200,0.,400 );



/// jet properties
int nJetPtEWK_Zinc2jet(15);
double jetPtEWK_Zinc2jet[16] = {15, 20, 24, 30, 38, 50, 68, 88, 113, 144, 184, 234, 297, 377, 480, 700 };
int nJetPtEWKbtw_Zinc2jet(12);
double jetPtEWKbtw_Zinc2jet[13] = {15, 20, 24, 30, 38, 50, 68, 88, 113, 144, 184, 234, 297 };
int nJetHTEWK_Zinc2jet(15);
double jetHTEWK_Zinc2jet[16] = {60, 80, 95, 121, 155, 200, 260, 336, 436, 566, 737, 962, 1200, 1500, 2000, 2500};

TH1D *FirstJetPtEWK_Zinc2jet                 = newHisto("FirstJetPtEWK_Zinc2jet",                 "1st jet p_{T} (N_{jets} #geq 2)",             "p_{T}(j_{1}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);
TH1D *SecondJetPtEWK_Zinc2jet                = newHisto("SecondJetPtEWK_Zinc2jet",                "2nd jet p_{T} (N_{jets} #geq 2)",             "p_{T}(j_{2}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);

TH1D *FirstJetEtaEWK_Zinc2jet                = newHisto("FirstJetEtaEWK_Zinc2jet",                "1st jet #eta (N_{jets} #geq 2)",              "#eta(j_{1})",  24,-4.8, 4.8);
TH1D *SecondJetEtaEWK_Zinc2jet               = newHisto("SecondJetEtaEWK_Zinc2jet",               "2nd jet #eta (N_{jets} #geq 2)",              "#eta(j_{2})",  24,-4.8, 4.8);
TH1D *FirstJetPhiEWK_Zinc2jet                = newHisto("FirstJetPhiEWK_Zinc2jet",                "1st jet #phi (N_{jets} #geq 2)",              "#phi(j_{1})",  30,-PI, PI);
TH1D *SecondJetPhiEWK_Zinc2jet               = newHisto("SecondJetPhiEWK_Zinc2jet",               "2nd jet #phi (N_{jets} #geq 2)",              "#phi(j_{2})",  30,-PI, PI);

TH1D *ForwardJetPtEWK_Zinc2jet                 = newHisto("ForwardJetPtEWK_Zinc2jet",                 "Fwd jet p_{T} (N_{jets} #geq 2)",             "p_{T}(j_{1}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);
TH1D *ForwardJetEtaEWK_Zinc2jet                = newHisto("ForwardJetEtaEWK_Zinc2jet",                "Fwd jet #eta (N_{jets} #geq 2)",              "#eta(j_{1})",  24,-4.8, 4.8);
TH1D *ForwardJetPhiEWK_Zinc2jet                = newHisto("ForwardJetPhiEWK_Zinc2jet",                "Fwd jet #phi (N_{jets} #geq 2)",              "#phi(j_{1})",  30,-PI, PI);

TH1D *CentralJetPtEWK_Zinc2jet                 = newHisto("CentralJetPtEWK_Zinc2jet",                 "Cnt jet p_{T} (N_{jets} #geq 2)",             "p_{T}(j_{1}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);
TH1D *CentralJetEtaEWK_Zinc2jet                = newHisto("CentralJetEtaEWK_Zinc2jet",                "Cnt jet #eta (N_{jets} #geq 2)",              "#eta(j_{1})",  24,-4.8, 4.8);
TH1D *CentralJetPhiEWK_Zinc2jet                = newHisto("CentralJetPhiEWK_Zinc2jet",                "Cnt jet #phi (N_{jets} #geq 2)",              "#phi(j_{1})",  30,-PI, PI);



TH1D *ThirdJetPtEWK_Zinc2jet                = newHisto("ThirdJetPtEWK_Zinc2jet",                "Ext 3nd jet p_{T} (N_{jets} #geq 2)",             "p_{T}(j_{3}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);
TH1D *ThirdJetPhiEWK_Zinc2jet                = newHisto("ThirdJetPhiEWK_Zinc2jet",              "Ext 3nd jet #Phi (N_{jets} #geq 2)",             "#Phi(j_{3}) ",     30, -PI, PI);
TH1D *ThirdJetEtaEWK_Zinc2jet               = newHisto("ThirdJetEtaEWK_Zinc2jet",               "Ext 3nd jet #eta (N_{jets} #geq 2)",              "#eta(j_{3})",  24,-4.8, 4.8);
/// additional activity > 15 GeV full space
TH1D *ThirdJetPtEWKadd_Zinc2jet                = newHisto("ThirdJetPtEWKadd_Zinc2jet",                "Ext pt#ge 15 3nd jet p_{T} (N_{jets} #geq 2)",             "p_{T}(j_{3}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);
TH1D *ThirdJetPhiEWKadd_Zinc2jet                = newHisto("ThirdJetPhiEWKadd_Zinc2jet",                "Ext 3nd pt#ge 15 jet p_{T} (N_{jets} #geq 2)",             "#Phi(j_{3}) ",     30, -PI, PI);
TH1D *ThirdJetEtaEWKadd_Zinc2jet               = newHisto("ThirdJetEtaEWKadd_Zinc2jet",               "Ext 3nd pt#ge 15 jet #eta (N_{jets} #geq 2)",              "#eta(j_{3})",  24,-4.8, 4.8);


/////  dijet properties
string jSEta = "#eta(j_{1})+ #eta(j_{2})";
TH1D *SumEtaJetsEWK_Zinc2jet             = newHisto("SumEtaJetsEWK_Zinc2jet",             "|#Sigma| #eta of j1,j2 jets (N_{jets} #geq 2)",                                       jSEta,           50, 0, 10);
TH1D *AbsSumEtaJetsEWK_Zinc2jet             = newHisto("AbsSumEtaJetsEWK_Zinc2jet",       "#Sigma |#eta| of j1,j2 jets (N_{jets} #geq 2)",                                       jSEta,           50, 0, 10 );
TH1D *SumEtaJetsEWK_Zinc3jet             = newHisto("SumEtaJetsEWK_Zinc3jet",             "sum of #eta of j1,j2 jets (N_{jets} #geq 3)",                                       jSEta,           50, -8, 8);


TH1D *dEtaJetsEWK_Zinc2jet             = newHisto("dEtaJetsEWK_Zinc2jet",             "#Delta#eta btwn jets (N_{jets} #geq 2)",                                       jdEta,           50, 0, 10);

TH1D *TwoJetsPtDiffEWK_Zinc2jet        = newHisto("TwoJetsPtDiffEWK_Zinc2jet",        "pT diff of the two highest jet (N_{jets} #geq 2)",                             "#Delta pT(j_{1}j_{2}) [GeV]",      50,  0, 500);


TH1D *ptBalEWK_Zinc2jet                = newHisto("ptBalEWK_Zinc2jet",                "Vectorial pT sum: Z_{pT} + DiJet_{pT} (N_{jets} #geq 2)",                       "#Sigma pT [GeV]",      50,  0, 500);
TH1D* SpTJetsEWK_Zinc2jet              = newHisto("SpTJetsEWK_Zinc2jet",              "#Delta_{pT}^{rel} jets (N_{jets} #geq 2)",                                jSpt,   50,  0, 1);

TH1D *dPhiJetsEWK_Zinc2jet             = newHisto("dPhiJetsEWK_Zinc2jet",             "#Delta#phi btwn jets (N_{jets} #geq 2)",                                       jdPhi,           100,  0, PI);


TH1D *JetsHTEWK_Zinc2jet                     = newHisto("JetsHTEWK_Zinc2jet",                     "Scalar sum jets p_{T} (N_{jets} #geq 2)",     HT,     nJetHTEWK_Zinc2jet, jetHTEWK_Zinc2jet);


TH1D *JetsMassEWK_Zinc2jet             = newHisto("JetsMassEWK_Zinc2jet",             "2Jets Invariant Mass (N_{jets} #geq 2)",                                       Mjj, 50, 20, 2620);


//// third jet properties
string  j3Eta = "#eta(j_{3})";
TH1D *EtaThirdJetsEWK_Zinc3jet             = newHisto("EtaThirdJetsEWK_Zinc3jet",             "Third jet #eta in dijet frame (N_{jets} #geq 3)",                                       j3Eta,           50, -5, 5);

/// all jets between leading two jets 
TH1D *AllJetPtEWKbtw_Zinc2jet                = newHisto("AllJetPtEWKbtw_Zinc2jet",                "Ext,btw All jet p_{T} (N_{jets} #geq 2)",             "p_{T}(j_{3}) [GeV]",     nJetPtEWKbtw_Zinc2jet, jetPtEWKbtw_Zinc2jet);
TH1D *AllJetPhiEWKbtw_Zinc2jet               = newHisto("AllJetPhiEWKbtw_Zinc2jet",               "Ext,btw All jet #phi (N_{jets} #geq 2)",              "#phi(j_{3})",  30,-PI, PI);
TH1D *AllJetEtaEWKbtw_Zinc2jet               = newHisto("AllJetEtaEWKbtw_Zinc2jet",               "Ext,btw All jet #eta (N_{jets} #geq 2)",              "#eta(j_{3})",  24,-4.8, 4.8);
double jetHTEWKbtw_Zinc2jet[16] = {0,15, 20, 30, 42, 60, 80, 95, 121, 155, 200, 260, 336, 436, 566, 737};
TH1D *JetsHTEWKbtw_Zinc2jet                  = newHisto("JetsHTEWKbtw_Zinc2jet",                  "Ext,btw Scalar sum jets p_{T} (N_{jets} #geq 2)",     HT,     nJetHTEWK_Zinc2jet, jetHTEWKbtw_Zinc2jet);
//TH1D *JetsHTEWKbtw_Zinc2jet                     = newHisto("JetsHTEWKbtw_Zinc2jet",                     "Ext,btw Scalar sum jets p_{T} (N_{jets} #geq 2)",     HT,     nJetHTEWK_Zinc2jet, jetHTEWK_Zinc2jet);

// third jet between
TH1D *ThirdJetPtEWKbtw_Zinc2jet                = newHisto("ThirdJetPtEWKbtw_Zinc2jet",                "Ext,btw 3nd jet p_{T} (N_{jets} #geq 2)",             "p_{T}(j_{3}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);
TH1D *ThirdJetPhiEWKbtw_Zinc2jet               = newHisto("ThirdJetPhiEWKbtw_Zinc2jet",               "Ext,btw 3nd jet #phi (N_{jets} #geq 2)",              "#phi(j_{3})",  30,-PI, PI);
TH1D *ThirdJetEtaEWKbtw_Zinc2jet               = newHisto("ThirdJetEtaEWKbtw_Zinc2jet",               "Ext,btw 3nd jet #eta (N_{jets} #geq 2)",              "#eta(j_{3})",  24,-4.8, 4.8);

