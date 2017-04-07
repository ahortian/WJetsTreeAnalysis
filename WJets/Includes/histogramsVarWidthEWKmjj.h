//// at least one forward jet
TH1D *ZNGoodJetsEWKmjj_Zexc = newHisto("ZNGoodJetsEWKmjj_Zexc","M_{jj} #geq 1 TeV: Jet Counter (excl.)", "N_{jets}", 11, -0.5, 10.5);
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

// met properties
TH1D *METEWKmjj_Zinc2jet                      = newHisto("METEWKmjj_Zinc2jet",                   "M_{jj} #geq 1 TeV: MET (N_{jets} #geq 2)",  "MET [GeV]",      200,0.,500 );
TH1D *METEWKmjj_Zinc3jet                      = newHisto("METEWKmjj_Zinc3jet",                   "M_{jj} #geq 1 TeV: MET (N_{jets} #geq 3)",  "MET [GeV]",      200,0.,500 );

TH1D *METphiEWKmjj_Zinc2jet                      = newHisto("METphiEWKmjj_Zinc2jet",                   "M_{jj} #geq 1 TeV: MET #phi (N_{jets} #geq 2)",  "#phi(MET)",      100,-PI ,PI );
///TH1D *METphiEWKmjj_Zinc3jet                      = newHisto("METphiEWKmjj_Zinc3jet",                   "M_{jj} #geq 1 TeV: MET #phi (N_{jets} #geq 3)",  "#phi(MET)",      100,-PI ,PI );

TH1D *MTEWKmjj_Zinc2jet                      = newHisto("MTEWKmjj_Zinc2jet",                   "M_{jj} #geq 1 TeV: MT (N_{jets} #geq 2)",    "MT [GeV]",    200,0.,400 );
//TH1D *MTEWKmjj_Zinc3jet                      = newHisto("MTEWKmjj_Zinc3jet",                   "M_{jj} #geq 1 TeV: MT (N_{jets} #geq 3)",    "MT [GeV]",    200,0.,400 );

// jet hisotgrams
TH1D *FirstJetPtEWKmjj_Zinc2jet                 = newHisto("FirstJetPtEWKmjj_Zinc2jet",              "M_{jj} #geq 1 TeV: 1st jet p_{T} (N_{jets} #geq 2)",             "p_{T}(j_{1}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);
TH1D *SecondJetPtEWKmjj_Zinc2jet                = newHisto("SecondJetPtEWKmjj_Zinc2jet",             "M_{jj} #geq 1 TeV: 2nd jet p_{T} (N_{jets} #geq 2)",             "p_{T}(j_{2}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);
TH1D *ThirdJetPtEWKmjj_Zinc3jet                 = newHisto("ThirdJetPtEWKmjj_Zinc3jet",              "M_{jj} #geq 1 TeV: 3rd jet p_{T} (N_{jets} #geq 3)",             "p_{T}(j_{3}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);

TH1D *FirstJetEtaEWKmjj_Zinc2jet                = newHisto("FirstJetEtaEWKmjj_Zinc2jet",             "M_{jj} #geq 1 TeV: 1st jet #eta (N_{jets} #geq 2)",              "#eta(j_{1})",  24,-4.8, 4.8);
TH1D *SecondJetEtaEWKmjj_Zinc2jet               = newHisto("SecondJetEtaEWKmjj_Zinc2jet",            "M_{jj} #geq 1 TeV: 2nd jet #eta (N_{jets} #geq 2)",              "#eta(j_{2})",  24,-4.8, 4.8);

TH1D *FirstJetPhiEWKmjj_Zinc2jet                = newHisto("FirstJetPhiEWKmjj_Zinc2jet",             "M_{jj} #geq 1 TeV: 1st jet #phi (N_{jets} #geq 2)",              "#phi(j_{1})",  30,-PI, PI);
TH1D *SecondJetPhiEWKmjj_Zinc2jet               = newHisto("SecondJetPhiEWKmjj_Zinc2jet",            "M_{jj} #geq 1 TeV: 2nd jet #phi (N_{jets} #geq 2)",              "#phi(j_{2})",  30,-PI, PI);


TH1D *SumEtaJetsEWKmjj_Zinc2jet             = newHisto("SumEtaJetsEWKmjj_Zinc2jet",          "M_{jj} #geq 1 TeV: sum of #eta of j1,j2 jets (N_{jets} #geq 2)",                                       jSEta,           50, -8, 8);
TH1D *SumEtaJetsEWKmjj_Zinc3jet             = newHisto("SumEtaJetsEWKmjj_Zinc3jet",          "M_{jj} #geq 1 TeV: sum of #eta of j1,j2 jets (N_{jets} #geq 3)",                                       jSEta,           50, -8, 8);
TH1D *genSumEtaJetsEWKmjj_Zinc2jet          = newHisto("genSumEtaJetsEWKmjj_Zinc2jet",       "M_{jj} #geq 1 TeV: gen sum of #eta of j1,j2 jets (N_{jets} #geq 2)",                                   jSEta,           50, -8, 8);


TH1D *dEtaJetsEWKmjj_Zinc2jet             = newHisto("dEtaJetsEWKmjj_Zinc2jet",          "M_{jj} #geq 1 TeV: #Delta#eta btwn jets (N_{jets} #geq 2)",                                       jdEta,           50, 0, 10);

TH1D *gendEtaJetsEWKmjj_Zinc2jet          = newHisto("gendEtaJetsEWKmjj_Zinc2jet",       "M_{jj} #geq 1 TeV: gen #Delta#eta btwn jets (N_{jets} #geq 2)",                                   jdEta,           50, 0, 10);

TH1D *TwoJetsPtDiffEWKmjj_Zinc2jet        = newHisto("TwoJetsPtDiffEWKmjj_Zinc2jet",     "M_{jj} #geq 1 TeV: pT diff of the two highest jet (N_{jets} #geq 2)",                             "#Delta pT(j_{1}j_{2}) [GeV]",      50,  0, 500);
TH1D *genTwoJetsPtDiffEWKmjj_Zinc2jet     = newHisto("genTwoJetsPtDiffEWKmjj_Zinc2jet",  "M_{jj} #geq 1 TeV: gen pT diff of the two highest jet (N_{jets} #geq 2)",                         "#Delta pT(j_{1}j_{2}) [GeV]",      50,  0, 1000);


TH1D *ptBalEWKmjj_Zinc2jet                = newHisto("ptBalEWKmjj_Zinc2jet",             "M_{jj} #geq 1 TeV: Vectorial pT sum: Z_{pT} + Jets_{pT} (N_{jets} #geq 2)",                       "#Sigma pT [GeV]",      50,  0, 500);
TH1D* SpTJetsEWKmjj_Zinc2jet              = newHisto("SpTJetsEWKmjj_Zinc2jet",           "M_{jj} #geq 1 TeV: #Delta_{pT}^{rel} jets (N_{jets} #geq 2)",                                jSpt,   50,  0, 1);
TH1D *genSpTJetsEWKmjj_Zinc2jet           = newHisto("genSpTJetsEWKmjj_Zinc2jet",        "M_{jj} #geq 1 TeV: gen #Delta_{pT}^{rel} jets (N_{jets} #geq 2)",                            jSpt,   50,  0, 1);

TH1D *dPhiJetsEWKmjj_Zinc2jet             = newHisto("dPhiJetsEWKmjj_Zinc2jet",          "M_{jj} #geq 1 TeV: #Delta#phi btwn jets (N_{jets} #geq 2)",                                       jdPhi,           100,  0, PI);
TH1D *gendPhiJetsEWKmjj_Zinc2jet          = newHisto("gendPhiJetsEWKmjj_Zinc2jet",       "M_{jj} #geq 1 TeV: gen #Delta#phi btwn jets (N_{jets} #geq 2)",                                   jdPhi,           100,  0, PI);


TH1D *JetsHTEWKmjj_Zinc2jet                     = newHisto("JetsHTEWKmjj_Zinc2jet",                  "M_{jj} #geq 1 TeV: Scalar sum jets p_{T} (N_{jets} #geq 2)",     HT,     nJetHTEWK_Zinc2jet, jetHTEWK_Zinc2jet);
TH1D *genJetsHTEWKmjj_Zinc2jet                  = newHisto("genJetsHTEWKmjj_Zinc2jet",               "M_{jj} #geq 1 TeV: Scalar sum jets p_{T} (N_{jets} #geq 2)",     HT,     nJetHTEWK_Zinc2jet, jetHTEWK_Zinc2jet);

TH1D *JetsHTEWKmjjAdd_Zinc2jet                     = newHisto("JetsHTEWKmjjAdd_Zinc2jet",                  "M_{jj} #geq 1 TeV: Scalar sum jets p_{T} , 15 GeV cut on third)",     HT,     nJetPtEWKbtw_Zinc2jet, jetHTEWKbtw_Zinc2jet);

TH1D *JetsMassEWKmjj_Zinc2jet             = newHisto("JetsMassEWKmjj_Zinc2jet",          "M_{jj} #geq 1 TeV: 2Jets Invariant Mass (N_{jets} #geq 2)",                                       Mjj, 50, 1000, 2620);
TH1D *genJetsMassEWKmjj_Zinc2jet          = newHisto("genJetsMassEWKmjj_Zinc2jet",       "M_{jj} #geq 1 TeV: gen 2Jets Invariant Mass (N_{jets} #geq 2)",                                   Mjj, 50, 1000, 2620);

//// third jet properties
TH1D *EtaThirdJetsEWKmjj_Zinc3jet             = newHisto("EtaThirdJetsEWKmjj_Zinc3jet",             "M_{jj} #geq 1 TeV: Third jet #eta in dijet frame (N_{jets} #geq 3)",                                       j3Eta,           50, -5, 5);

