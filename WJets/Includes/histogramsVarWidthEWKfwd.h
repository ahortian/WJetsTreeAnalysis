//// at least one forward jet
TH1D *ZNGoodJetsEWKfwd_Zexc = newHisto("ZNGoodJetsEWKfwd_Zexc"," 1 Fwd jet: Jet Counter (excl.)", "N_{jets}", 11, -0.5, 10.5);
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
TH1D *METEWKfwd_Zinc2jet                      = newHisto("METEWKfwd_Zinc2jet",                   " 1 Fwd jet: MET (N_{jets} #geq 2)",  "MET [GeV]",      200,0.,500 );
TH1D *METEWKfwd_Zinc3jet                      = newHisto("METEWKfwd_Zinc3jet",                   " 1 Fwd jet: MET (N_{jets} #geq 3)",  "MET [GeV]",      200,0.,500 );

TH1D *METphiEWKfwd_Zinc2jet                      = newHisto("METphiEWKfwd_Zinc2jet",                   " 1 Fwd jet: MET #phi (N_{jets} #geq 2)",  "#phi(MET)",      100,-PI ,PI );
TH1D *METphiEWKfwd_Zinc3jet                      = newHisto("METphiEWKfwd_Zinc3jet",                   " 1 Fwd jet: MET #phi (N_{jets} #geq 3)",  "#phi(MET)",      100,-PI ,PI );

TH1D *MTEWKfwd_Zinc2jet                      = newHisto("MTEWKfwd_Zinc2jet",                   " 1 Fwd jet: MT (N_{jets} #geq 2)",    "MT [GeV]",    200,0.,400 );
TH1D *MTEWKfwd_Zinc3jet                      = newHisto("MTEWKfwd_Zinc3jet",                   " 1 Fwd jet: MT (N_{jets} #geq 3)",    "MT [GeV]",    200,0.,400 );

// jet hisotgrams
TH1D *FirstJetPtEWKfwd_Zinc2jet                 = newHisto("FirstJetPtEWKfwd_Zinc2jet",              " 1 Fwd jet: 1st jet p_{T} (N_{jets} #geq 2)",             "p_{T}(j_{1}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);
TH1D *SecondJetPtEWKfwd_Zinc2jet                = newHisto("SecondJetPtEWKfwd_Zinc2jet",             " 1 Fwd jet: 2nd jet p_{T} (N_{jets} #geq 2)",             "p_{T}(j_{2}) [GeV]",     nJetPtEWK_Zinc2jet, jetPtEWK_Zinc2jet);

TH1D *FirstJetEtaEWKfwd_Zinc2jet                = newHisto("FirstJetEtaEWKfwd_Zinc2jet",             " 1 Fwd jet: 1st jet #eta (N_{jets} #geq 2)",              "#eta(j_{1})",  24,-4.8, 4.8);
TH1D *SecondJetEtaEWKfwd_Zinc2jet               = newHisto("SecondJetEtaEWKfwd_Zinc2jet",            " 1 Fwd jet: 2nd jet #eta (N_{jets} #geq 2)",              "#eta(j_{2})",  24,-4.8, 4.8);

TH1D *FirstJetPhiEWKfwd_Zinc2jet                = newHisto("FirstJetPhiEWKfwd_Zinc2jet",             " 1 Fwd jet: 1st jet #phi (N_{jets} #geq 2)",              "#phi(j_{1})",  30,-PI, PI);
TH1D *SecondJetPhiEWKfwd_Zinc2jet               = newHisto("SecondJetPhiEWKfwd_Zinc2jet",            " 1 Fwd jet: 2nd jet #phi (N_{jets} #geq 2)",              "#phi(j_{2})",  30,-PI, PI);


TH1D *SumEtaJetsEWKfwd_Zinc2jet             = newHisto("SumEtaJetsEWKfwd_Zinc2jet",          " 1 Fwd jet: sum of #eta of j1,j2 jets (N_{jets} #geq 2)",                                       jSEta,           50, -8, 8);
TH1D *SumEtaJetsEWKfwd_Zinc3jet             = newHisto("SumEtaJetsEWKfwd_Zinc3jet",          " 1 Fwd jet: sum of #eta of j1,j2 jets (N_{jets} #geq 3)",                                       jSEta,           50, -8, 8);
TH1D *genSumEtaJetsEWKfwd_Zinc2jet          = newHisto("genSumEtaJetsEWKfwd_Zinc2jet",       " 1 Fwd jet: gen sum of #eta of j1,j2 jets (N_{jets} #geq 2)",                                   jSEta,           50, -8, 8);


TH1D *dEtaJetsEWKfwd_Zinc2jet             = newHisto("dEtaJetsEWKfwd_Zinc2jet",          " 1 Fwd jet: #Delta#eta btwn jets (N_{jets} #geq 2)",                                       jdEta,           50,  0, 10);
TH1D *gendEtaJetsEWKfwd_Zinc2jet          = newHisto("gendEtaJetsEWKfwd_Zinc2jet",       " 1 Fwd jet: gen #Delta#eta btwn jets (N_{jets} #geq 2)",                                   jdEta,           50,  0, 10);

TH1D *TwoJetsPtDiffEWKfwd_Zinc2jet        = newHisto("TwoJetsPtDiffEWKfwd_Zinc2jet",     " 1 Fwd jet: pT diff of the two highest jet (N_{jets} #geq 2)",                             "#Delta pT(j_{1}j_{2}) [GeV]",      50,  0, 500);
TH1D *genTwoJetsPtDiffEWKfwd_Zinc2jet     = newHisto("genTwoJetsPtDiffEWKfwd_Zinc2jet",  " 1 Fwd jet: gen pT diff of the two highest jet (N_{jets} #geq 2)",                         "#Delta pT(j_{1}j_{2}) [GeV]",      50,  0, 1000);


TH1D *ptBalEWKfwd_Zinc2jet                = newHisto("ptBalEWKfwd_Zinc2jet",             " 1 Fwd jet: Vectorial pT sum: Z_{pT} + Jets_{pT} (N_{jets} #geq 2)",                       "#Sigma pT [GeV]",      50,  0, 500);
TH1D* SpTJetsEWKfwd_Zinc2jet              = newHisto("SpTJetsEWKfwd_Zinc2jet",           " 1 Fwd jet: #Delta_{pT}^{rel} jets (N_{jets} #geq 2)",                                jSpt,   50,  0, 1);
TH1D *genSpTJetsEWKfwd_Zinc2jet           = newHisto("genSpTJetsEWKfwd_Zinc2jet",        " 1 Fwd jet: gen #Delta_{pT}^{rel} jets (N_{jets} #geq 2)",                            jSpt,   50,  0, 1);

TH1D *dPhiJetsEWKfwd_Zinc2jet             = newHisto("dPhiJetsEWKfwd_Zinc2jet",          " 1 Fwd jet: #Delta#phi btwn jets (N_{jets} #geq 2)",                                       jdPhi,           100,  0, PI);
TH1D *gendPhiJetsEWKfwd_Zinc2jet          = newHisto("gendPhiJetsEWKfwd_Zinc2jet",       " 1 Fwd jet: gen #Delta#phi btwn jets (N_{jets} #geq 2)",                                   jdPhi,           100,  0, PI);


TH1D *JetsHTEWKfwd_Zinc2jet                     = newHisto("JetsHTEWKfwd_Zinc2jet",                  " 1 Fwd jet: Scalar sum jets p_{T} (N_{jets} #geq 2)",     HT,     nJetHTEWK_Zinc2jet, jetHTEWK_Zinc2jet);
TH1D *genJetsHTEWKfwd_Zinc2jet                  = newHisto("genJetsHTEWKfwd_Zinc2jet",               " 1 Fwd jet: Scalar sum jets p_{T} (N_{jets} #geq 2)",     HT,     nJetHTEWK_Zinc2jet, jetHTEWK_Zinc2jet);


TH1D *JetsMassEWKfwd_Zinc2jet             = newHisto("JetsMassEWKfwd_Zinc2jet",          " 1 Fwd jet: 2Jets Invariant Mass (N_{jets} #geq 2)",                                       Mjj, 50, 20, 2620);
TH1D *genJetsMassEWKfwd_Zinc2jet          = newHisto("genJetsMassEWKfwd_Zinc2jet",       " 1 Fwd jet: gen 2Jets Invariant Mass (N_{jets} #geq 2)",                                   Mjj, 50, 20, 2620);

