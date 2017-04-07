TH1::SetDefaultSumw2();
TH2::SetDefaultSumw2();

string ZpT = "p_{T}(Z) [GeV]", Zrap = "y(Z)", Zeta = "#eta(Z)";
string HT = "H_{T}(jets) [GeV]", Mjj = "M_{j_{1}j_{2}} [GeV]", jSpt = "#Delta_{pT}^{rel}(j_{1}j_{2})", jdPhi = "#Delta#phi(j_{1}j_{2})", jdEta = "#Delta#eta(j_{1}j_{2})";
string Mll = "M_{#mu#mu} [GeV]", leta = "#eta(#mu)", lpT = "p_{T}(#mu) [GeV]", ldPhi = "#Delta#phi(#mu_{1}#mu_{2})", ldEta = "#Delta#eta(#mu_{1}#mu_{2})", ldR = "#DeltaR(#mu_{1}#mu_{2})";
string lSpt = "#Delta_{pT}^{rel}(#mu_{1}#mu_{2})";
string Spt = "#Delta_{pT}^{rel}(j_{1}j_{2}#mu_{1}#mu_{2})";
string Sphi = "Sphi(j_{1}j_{2}#mu_{1}#mu_{2})";

if (leptonFlavor == "Electrons" || leptonFlavor == "DE" || leptonFlavor == "DE_") {
   Mll = "M_{ee} [GeV]";
   leta = "#eta(e)";
   lpT = "p_{T}(e) [GeV]";
   ldPhi = "#Delta#phi(e_{1}e_{2})";
   ldEta = "#Delta#eta(e_{1}e_{2})";
   ldR = "#DeltaR(e_{1}e_{2})";
   lSpt = "#Delta_{pT}^{rel}(e_{1}e_{2})";
   Spt = "#Delta_{pT}^{rel}(j_{1}j_{2}e_{1}e_{2})";
   Sphi = "Sphi(j_{1}j_{2}e_{1}e_{2})";
}

TH1D *ZMass_lowDeltaR                     = newHisto("ZMass_lowDeltaR",                     "ZMass_lowDeltaR",                             Mll,    60, 60, 120);

TH1D *ZMass_Zinc0jet_test                 = newHisto("ZMass_Zinc0jet_test",                 "Z Invariant Mass (N_{jets} #geq 0)",          Mll,    40, 71, 111);
TH1D *ZMass_Zinc0jet                      = newHisto("ZMass_Zinc0jet",                      "Z Invariant Mass (N_{jets} #geq 0)",          Mll,    40, 71, 111);
TH1D *ZMass_Zinc1jet                      = newHisto("ZMass_Zinc1jet",                      "Z Invariant Mass (N_{jets} #geq 1)",          Mll,    40, 71, 111);
TH1D *ZMass_Zinc2jet                      = newHisto("ZMass_Zinc2jet",                      "Z Invariant Mass (N_{jets} #geq 2)",          Mll,    40, 71, 111);
TH1D *ZMass_Zinc3jet                      = newHisto("ZMass_Zinc3jet",                      "Z Invariant Mass (N_{jets} #geq 3)",          Mll,    40, 71, 111);
TH1D *ZMass_Zinc4jet                      = newHisto("ZMass_Zinc4jet",                      "Z Invariant Mass (N_{jets} #geq 4)",          Mll,    40, 71, 111);
TH1D *ZMass_Zinc5jet                      = newHisto("ZMass_Zinc5jet",                      "Z Invariant Mass (N_{jets} #geq 5)",          Mll,    40, 71, 111);
TH1D *ZMass_Zinc6jet                      = newHisto("ZMass_Zinc6jet",                      "Z Invariant Mass (N_{jets} #geq 6)",          Mll,    40, 71, 111);

TH1D *genZMass_Zinc0jet_test              = newHisto("genZMass_Zinc0jet_test",              "Z Invariant Mass (N_{jets} #geq 0)",          Mll,    40, 71, 111);
TH1D *genZMass_Zinc0jet                   = newHisto("genZMass_Zinc0jet",                   "Z Invariant Mass (N_{jets} #geq 0)",          Mll,    40, 71, 111);
TH1D *genZMass_Zinc1jet                   = newHisto("genZMass_Zinc1jet",                   "Z Invariant Mass (N_{jets} #geq 1)",          Mll,    40, 71, 111);
TH1D *genZMass_Zinc2jet                   = newHisto("genZMass_Zinc2jet",                   "Z Invariant Mass (N_{jets} #geq 2)",          Mll,    40, 71, 111);
TH1D *genZMass_Zinc3jet                   = newHisto("genZMass_Zinc3jet",                   "Z Invariant Mass (N_{jets} #geq 3)",          Mll,    40, 71, 111);
TH1D *genZMass_Zinc4jet                   = newHisto("genZMass_Zinc4jet",                   "Z Invariant Mass (N_{jets} #geq 4)",          Mll,    40, 71, 111);
TH1D *genZMass_Zinc5jet                   = newHisto("genZMass_Zinc5jet",                   "Z Invariant Mass (N_{jets} #geq 5)",          Mll,    40, 71, 111);
TH1D *genZMass_Zinc6jet                   = newHisto("genZMass_Zinc6jet",                   "Z Invariant Mass (N_{jets} #geq 6)",          Mll,    40, 71, 111);

TH1D *ZMass_Zexc0jet                      = newHisto("ZMass_Zexc0jet",                      "Z Invariant Mass (N_{jets} = 0)",             Mll,    40, 71, 111);
TH1D *ZMass_Zexc1jet                      = newHisto("ZMass_Zexc1jet",                      "Z Invariant Mass (N_{jets} = 1)",             Mll,    40, 71, 111);
TH1D *ZMass_Zexc2jet                      = newHisto("ZMass_Zexc2jet",                      "Z Invariant Mass (N_{jets} = 2)",             Mll,    40, 71, 111);
TH1D *ZMass_Zexc3jet                      = newHisto("ZMass_Zexc3jet",                      "Z Invariant Mass (N_{jets} = 3)",             Mll,    40, 71, 111);
TH1D *ZMass_Zexc4jet                      = newHisto("ZMass_Zexc4jet",                      "Z Invariant Mass (N_{jets} = 4)",             Mll,    40, 71, 111);
TH1D *ZMass_Zexc5jet                      = newHisto("ZMass_Zexc5jet",                      "Z Invariant Mass (N_{jets} = 5)",             Mll,    40, 71, 111);
TH1D *ZMass_Zexc6jet                      = newHisto("ZMass_Zexc6jet",                      "Z Invariant Mass (N_{jets} = 6)",             Mll,    40, 71, 111);

TH1D *ZPt_Zinc0jet                        = newHisto("ZPt_Zinc0jet",                        "Z p_{T} (N_{jets} #geq 0)",                   ZpT,    40, 0, 400);
TH1D *ZPt_Zinc1jet                        = newHisto("ZPt_Zinc1jet",                        "Z p_{T} (N_{jets} #geq 1)",                   ZpT,    40, 0, 400);
TH1D *ZPt_Zinc2jet                        = newHisto("ZPt_Zinc2jet",                        "Z p_{T} (N_{jets} #geq 2)",                   ZpT,    40, 0, 400);
TH1D *ZPt_Zinc3jet                        = newHisto("ZPt_Zinc3jet",                        "Z p_{T} (N_{jets} #geq 3)",                   ZpT,    40, 0, 300);
TH1D *ZPt_Zinc4jet                        = newHisto("ZPt_Zinc4jet",                        "Z p_{T} (N_{jets} #geq 4)",                   ZpT,    40, 0, 200);
TH1D *ZPt_Zinc5jet                        = newHisto("ZPt_Zinc5jet",                        "Z p_{T} (N_{jets} #geq 5)",                   ZpT,    40, 0, 200);
TH1D *ZPt_Zinc6jet                        = newHisto("ZPt_Zinc6jet",                        "Z p_{T} (N_{jets} #geq 6)",                   ZpT,    40, 0, 200);

TH1D *genZPt_Zinc0jet                     = newHisto("genZPt_Zinc0jet",                     "gen Z p_{T} (N_{jets} #geq 0)",               ZpT,    40, 0, 200);
TH1D *genZPt_Zinc1jet                     = newHisto("genZPt_Zinc1jet",                     "gen Z p_{T} (N_{jets} #geq 1)",               ZpT,    40, 0, 400);
TH1D *genZPt_Zinc2jet                     = newHisto("genZPt_Zinc2jet",                     "gen Z p_{T} (N_{jets} #geq 2)",               ZpT,    40, 0, 400);
TH1D *genZPt_Zinc3jet                     = newHisto("genZPt_Zinc3jet",                     "gen Z p_{T} (N_{jets} #geq 3)",               ZpT,    40, 0, 300);
TH1D *genZPt_Zinc4jet                     = newHisto("genZPt_Zinc4jet",                     "gen Z p_{T} (N_{jets} #geq 4)",               ZpT,    40, 0, 200);
TH1D *genZPt_Zinc5jet                     = newHisto("genZPt_Zinc5jet",                     "gen Z p_{T} (N_{jets} #geq 5)",               ZpT,    40, 0, 200);
TH1D *genZPt_Zinc6jet                     = newHisto("genZPt_Zinc6jet",                     "gen Z p_{T} (N_{jets} #geq 6)",               ZpT,    40, 0, 200);

TH1D *ZPt_Zexc0jet                        = newHisto("ZPt_Zexc0jet",                        "Z p_{T} (N_{jets} = 0)",                      ZpT,    40, 0, 400);
TH1D *ZPt_Zexc1jet                        = newHisto("ZPt_Zexc1jet",                        "Z p_{T} (N_{jets} = 1)",                      ZpT,    40, 0, 400);
TH1D *ZPt_Zexc2jet                        = newHisto("ZPt_Zexc2jet",                        "Z p_{T} (N_{jets} = 2)",                      ZpT,    40, 0, 400);
TH1D *ZPt_Zexc3jet                        = newHisto("ZPt_Zexc3jet",                        "Z p_{T} (N_{jets} = 3)",                      ZpT,    40, 0, 300);
TH1D *ZPt_Zexc4jet                        = newHisto("ZPt_Zexc4jet",                        "Z p_{T} (N_{jets} = 4)",                      ZpT,    40, 0, 200);
TH1D *ZPt_Zexc5jet                        = newHisto("ZPt_Zexc5jet",                        "Z p_{T} (N_{jets} = 5)",                      ZpT,    40, 0, 200);
TH1D *ZPt_Zexc6jet                        = newHisto("ZPt_Zexc6jet",                        "Z p_{T} (N_{jets} = 6)",                      ZpT,    40, 0, 200);

TH1D *ZRapidity_Zinc0jet                  = newHisto("ZRapidity_Zinc0jet",                  "Z Rapidity (N_{jets} #geq 0)",                Zrap,   30,-3, 3);
TH1D *ZRapidity_Zinc1jet                  = newHisto("ZRapidity_Zinc1jet",                  "Z Rapidity (N_{jets} #geq 1)",                Zrap,   30,-3, 3);
TH1D *ZRapidity_Zinc2jet                  = newHisto("ZRapidity_Zinc2jet",                  "Z Rapidity (N_{jets} #geq 2)",                Zrap,   30,-3, 3);
TH1D *ZRapidity_Zinc3jet                  = newHisto("ZRapidity_Zinc3jet",                  "Z Rapidity (N_{jets} #geq 3)",                Zrap,   30,-3, 3);
TH1D *ZRapidity_Zinc4jet                  = newHisto("ZRapidity_Zinc4jet",                  "Z Rapidity (N_{jets} #geq 4)",                Zrap,   30,-3, 3);
TH1D *ZRapidity_Zinc5jet                  = newHisto("ZRapidity_Zinc5jet",                  "Z Rapidity (N_{jets} #geq 5)",                Zrap,   30,-3, 3);
TH1D *ZRapidity_Zinc6jet                  = newHisto("ZRapidity_Zinc6jet",                  "Z Rapidity (N_{jets} #geq 6)",                Zrap,   30,-3, 3);

TH1D *genZRapidity_Zinc0jet               = newHisto("genZRapidity_Zinc0jet",               "gen Z Rapidity (N_{jets} #geq 0)",            Zrap,   30,-3, 3);
TH1D *genZRapidity_Zinc1jet               = newHisto("genZRapidity_Zinc1jet",               "gen Z Rapidity (N_{jets} #geq 1)",            Zrap,   30,-3, 3);
TH1D *genZRapidity_Zinc2jet               = newHisto("genZRapidity_Zinc2jet",               "gen Z Rapidity (N_{jets} #geq 2)",            Zrap,   30,-3, 3);
TH1D *genZRapidity_Zinc3jet               = newHisto("genZRapidity_Zinc3jet",               "gen Z Rapidity (N_{jets} #geq 3)",            Zrap,   30,-3, 3);
TH1D *genZRapidity_Zinc4jet               = newHisto("genZRapidity_Zinc4jet",               "gen Z Rapidity (N_{jets} #geq 4)",            Zrap,   30,-3, 3);
TH1D *genZRapidity_Zinc5jet               = newHisto("genZRapidity_Zinc5jet",               "gen Z Rapidity (N_{jets} #geq 5)",            Zrap,   30,-3, 3);
TH1D *genZRapidity_Zinc6jet               = newHisto("genZRapidity_Zinc6jet",               "gen Z Rapidity (N_{jets} #geq 6)",            Zrap,   30,-3, 3);

TH1D *ZRapidity_Zexc0jet                  = newHisto("ZRapidity_Zexc0jet",                  "Z Rapidity (N_{jets} = 0)",                   Zrap,   30,-3, 3);
TH1D *ZRapidity_Zexc1jet                  = newHisto("ZRapidity_Zexc1jet",                  "Z Rapidity (N_{jets} = 1)",                   Zrap,   30,-3, 3);
TH1D *ZRapidity_Zexc2jet                  = newHisto("ZRapidity_Zexc2jet",                  "Z Rapidity (N_{jets} = 2)",                   Zrap,   30,-3, 3);
TH1D *ZRapidity_Zexc3jet                  = newHisto("ZRapidity_Zexc3jet",                  "Z Rapidity (N_{jets} = 3)",                   Zrap,   30,-3, 3);
TH1D *ZRapidity_Zexc4jet                  = newHisto("ZRapidity_Zexc4jet",                  "Z Rapidity (N_{jets} = 4)",                   Zrap,   30,-3, 3);
TH1D *ZRapidity_Zexc5jet                  = newHisto("ZRapidity_Zexc5jet",                  "Z Rapidity (N_{jets} = 5)",                   Zrap,   30,-3, 3);
TH1D *ZRapidity_Zexc6jet                  = newHisto("ZRapidity_Zexc6jet",                  "Z Rapidity (N_{jets} = 6)",                   Zrap,   30,-3, 3);

TH1D *ZEta_Zinc0jet                       = newHisto("ZEta_Zinc0jet",                       "Z #eta (N_{jets} #geq 0)",                    Zeta,   30,-3, 3);
TH1D *ZEta_Zinc1jet                       = newHisto("ZEta_Zinc1jet",                       "Z #eta (N_{jets} #geq 1)",                    Zeta,   30,-3, 3);
TH1D *ZEta_Zinc2jet                       = newHisto("ZEta_Zinc2jet",                       "Z #eta (N_{jets} #geq 2)",                    Zeta,   30,-3, 3);
TH1D *ZEta_Zinc3jet                       = newHisto("ZEta_Zinc3jet",                       "Z #eta (N_{jets} #geq 3)",                    Zeta,   30,-3, 3);
TH1D *ZEta_Zinc4jet                       = newHisto("ZEta_Zinc4jet",                       "Z #eta (N_{jets} #geq 4)",                    Zeta,   30,-3, 3);
TH1D *ZEta_Zinc5jet                       = newHisto("ZEta_Zinc5jet",                       "Z #eta (N_{jets} #geq 5)",                    Zeta,   30,-3, 3);
TH1D *ZEta_Zinc6jet                       = newHisto("ZEta_Zinc6jet",                       "Z #eta (N_{jets} #geq 6)",                    Zeta,   30,-3, 3);

TH1D *genZEta_Zinc0jet                    = newHisto("genZEta_Zinc0jet",                    "gen Z #eta (N_{jets} #geq 0)",                Zeta,   30,-3, 3);
TH1D *genZEta_Zinc1jet                    = newHisto("genZEta_Zinc1jet",                    "gen Z #eta (N_{jets} #geq 1)",                Zeta,   30,-3, 3);
TH1D *genZEta_Zinc2jet                    = newHisto("genZEta_Zinc2jet",                    "gen Z #eta (N_{jets} #geq 2)",                Zeta,   30,-3, 3);
TH1D *genZEta_Zinc3jet                    = newHisto("genZEta_Zinc3jet",                    "gen Z #eta (N_{jets} #geq 3)",                Zeta,   30,-3, 3);
TH1D *genZEta_Zinc4jet                    = newHisto("genZEta_Zinc4jet",                    "gen Z #eta (N_{jets} #geq 4)",                Zeta,   30,-3, 3);
TH1D *genZEta_Zinc5jet                    = newHisto("genZEta_Zinc5jet",                    "gen Z #eta (N_{jets} #geq 5)",                Zeta,   30,-3, 3);
TH1D *genZEta_Zinc6jet                    = newHisto("genZEta_Zinc6jet",                    "gen Z #eta (N_{jets} #geq 6)",                Zeta,   30,-3, 3);

TH1D *ZEta_Zexc0jet                       = newHisto("ZEta_Zexc0jet",                       "Z #eta (N_{jets} = 0)",                       Zeta,   30,-3, 3);
TH1D *ZEta_Zexc1jet                       = newHisto("ZEta_Zexc1jet",                       "Z #eta (N_{jets} = 1)",                       Zeta,   30,-3, 3);
TH1D *ZEta_Zexc2jet                       = newHisto("ZEta_Zexc2jet",                       "Z #eta (N_{jets} = 2)",                       Zeta,   30,-3, 3);
TH1D *ZEta_Zexc3jet                       = newHisto("ZEta_Zexc3jet",                       "Z #eta (N_{jets} = 3)",                       Zeta,   30,-3, 3);
TH1D *ZEta_Zexc4jet                       = newHisto("ZEta_Zexc4jet",                       "Z #eta (N_{jets} = 4)",                       Zeta,   30,-3, 3);
TH1D *ZEta_Zexc5jet                       = newHisto("ZEta_Zexc5jet",                       "Z #eta (N_{jets} = 5)",                       Zeta,   30,-3, 3);
TH1D *ZEta_Zexc6jet                       = newHisto("ZEta_Zexc6jet",                       "Z #eta (N_{jets} = 6)",                       Zeta,   30,-3, 3);

TH1D *lepEta_Zinc0jet                     = newHisto("lepEta_Zinc0jet",                     "1st & 2nd lep #eta (N_{jets} #geq 0)",        leta,   24,-2.4, 2.4);
TH1D *lepEta_Zinc1jet                     = newHisto("lepEta_Zinc1jet",                     "1st & 2nd lep #eta (N_{jets} #geq 1)",        leta,   24,-2.4, 2.4);
TH1D *lepEta_Zinc2jet                     = newHisto("lepEta_Zinc2jet",                     "1st & 2nd lep #eta (N_{jets} #geq 2)",        leta,   24,-2.4, 2.4);
TH1D *lepEta_Zinc3jet                     = newHisto("lepEta_Zinc3jet",                     "1st & 2nd lep #eta (N_{jets} #geq 3)",        leta,   24,-2.4, 2.4);
TH1D *lepEta_Zinc4jet                     = newHisto("lepEta_Zinc4jet",                     "1st & 2nd lep #eta (N_{jets} #geq 4)",        leta,   24,-2.4, 2.4);
TH1D *lepEta_Zinc5jet                     = newHisto("lepEta_Zinc5jet",                     "1st & 2nd lep #eta (N_{jets} #geq 5)",        leta,   24,-2.4, 2.4);

TH1D *genlepEta_Zinc0jet                  = newHisto("genlepEta_Zinc0jet",                  "1st & 2nd lep #eta (N_{jets} #geq 0)",        leta,   24,-2.4, 2.4);  
TH1D *genlepEta_Zinc1jet                  = newHisto("genlepEta_Zinc1jet",                  "1st & 2nd lep #eta (N_{jets} #geq 1)",        leta,   24,-2.4, 2.4);  
TH1D *genlepEta_Zinc2jet                  = newHisto("genlepEta_Zinc2jet",                  "1st & 2nd lep #eta (N_{jets} #geq 2)",        leta,   24,-2.4, 2.4);  

TH1D *lepEta_Zexc0jet                     = newHisto("lepEta_Zexc0jet",                     "1st & 2nd lep #eta (N_{jets} = 0)",           leta,   24,-2.4, 2.4);  
TH1D *lepEta_Zexc1jet                     = newHisto("lepEta_Zexc1jet",                     "1st & 2nd lep #eta (N_{jets} = 1)",           leta,   24,-2.4, 2.4);  
TH1D *lepEta_Zexc2jet                     = newHisto("lepEta_Zexc2jet",                     "1st & 2nd lep #eta (N_{jets} = 2)",           leta,   24,-2.4, 2.4);  
TH1D *lepEta_Zexc3jet                     = newHisto("lepEta_Zexc3jet",                     "1st & 2nd lep #eta (N_{jets} = 3)",           leta,   24,-2.4, 2.4);  
TH1D *lepEta_Zexc4jet                     = newHisto("lepEta_Zexc4jet",                     "1st & 2nd lep #eta (N_{jets} = 4)",           leta,   24,-2.4, 2.4);  
TH1D *lepEta_Zexc5jet                     = newHisto("lepEta_Zexc5jet",                     "1st & 2nd lep #eta (N_{jets} = 5)",           leta,   24,-2.4, 2.4);  

TH1D *FirstJetEta_Zinc1jet                = newHisto("FirstJetEta_Zinc1jet",                "1st jet #eta (N_{jets} #geq 1)",              "#eta(j_{1})",  24,-2.4, 2.4);  
TH1D *SecondJetEta_Zinc2jet               = newHisto("SecondJetEta_Zinc2jet",               "2nd jet #eta (N_{jets} #geq 2)",              "#eta(j_{2})",  24,-2.4, 2.4);  
TH1D *ThirdJetEta_Zinc3jet                = newHisto("ThirdJetEta_Zinc3jet",                "3rd jet #eta (N_{jets} #geq 3)",              "#eta(j_{3})",  24,-2.4, 2.4);  
TH1D *FourthJetEta_Zinc4jet               = newHisto("FourthJetEta_Zinc4jet",               "4th jet #eta (N_{jets} #geq 4)",              "#eta(j_{4})",  12,-2.4, 2.4);  
TH1D *FifthJetEta_Zinc5jet                = newHisto("FifthJetEta_Zinc5jet",                "5th jet #eta (N_{jets} #geq 5)",              "#eta(j_{5})",   6,-2.4, 2.4);  
TH1D *SixthJetEta_Zinc6jet                = newHisto("SixthJetEta_Zinc6jet",                "#geq 6th jets #eta (N_{jets} #geq 6)",        "#eta(j_{6})",   4,-2.4, 2.4);  

TH1D *genFirstJetEta_Zinc1jet             = newHisto("genFirstJetEta_Zinc1jet",             "gen 1st jet #eta (N_{jets} #geq 1)",          "#eta(j_{1})",  24,-2.4, 2.4);
TH1D *genSecondJetEta_Zinc2jet            = newHisto("genSecondJetEta_Zinc2jet",            "gen 2nd jet #eta (N_{jets} #geq 2)",          "#eta(j_{2})",  24,-2.4, 2.4);
TH1D *genThirdJetEta_Zinc3jet             = newHisto("genThirdJetEta_Zinc3jet",             "gen 3rd jet #eta (N_{jets} #geq 3)",          "#eta(j_{3})",  24,-2.4, 2.4);
TH1D *genFourthJetEta_Zinc4jet            = newHisto("genFourthJetEta_Zinc4jet",            "gen 4th jet #eta (N_{jets} #geq 4)",          "#eta(j_{4})",  12,-2.4, 2.4);
TH1D *genFifthJetEta_Zinc5jet             = newHisto("genFifthJetEta_Zinc5jet",             "gen 5th jet #eta (N_{jets} #geq 5)",          "#eta(j_{5})",   6,-2.4, 2.4);
TH1D *genSixthJetEta_Zinc6jet             = newHisto("genSixthJetEta_Zinc6jet",             "gen #geq 6th jets #eta (N_{jets} #geq 6)",    "#eta(j_{6})",   4,-2.4, 2.4);

TH1D *FirstJetEta_Zexc1jet                = newHisto("FirstJetEta_Zexc1jet",                "1st jet #eta (N_{jets} = 1)",                 "#eta(j_{1})",  24,-2.4, 2.4);  
TH1D *SecondJetEta_Zexc2jet               = newHisto("SecondJetEta_Zexc2jet",               "2nd jet #eta (N_{jets} = 2)",                 "#eta(j_{2})",  24,-2.4, 2.4);  

TH1D *AllJetEta_Zinc1jet                  = newHisto("AllJetEta_Zinc1jet",                  "All jets #eta (N_{jets} #geq 1)",             "#eta(jets)",   24,-2.4, 2.4);  
TH1D *AllJetEta_Zinc2jet                  = newHisto("AllJetEta_Zinc2jet",                  "All jets #eta (N_{jets} #geq 2)",             "#eta(jets)",   24,-2.4, 2.4);  
TH1D *AllJetEta_Zinc3jet                  = newHisto("AllJetEta_Zinc3jet",                  "All jets #eta (N_{jets} #geq 3)",             "#eta(jets)",   24,-2.4, 2.4);  
TH1D *AllJetEta_Zinc4jet                  = newHisto("AllJetEta_Zinc4jet",                  "All jets #eta (N_{jets} #geq 4)",             "#eta(jets)",   24,-2.4, 2.4);  

TH1D *FirstJetPhi_Zinc1jet                = newHisto("FirstJetPhi_Zinc1jet",                "1st jet #phi (N_{jets} #geq 1)",              "#phi(j_{1})",  30,-3, 3);
TH1D *SecondJetPhi_Zinc2jet               = newHisto("SecondJetPhi_Zinc2jet",               "2nd jet #phi (N_{jets} #geq 2)",              "#phi(j_{2})",  30,-3, 3);
TH1D *ThirdJetPhi_Zinc3jet                = newHisto("ThirdJetPhi_Zinc3jet",                "3rd jet #phi (N_{jets} #geq 3)",              "#phi(j_{3})",  30,-3, 3);
TH1D *FourthJetPhi_Zinc4jet               = newHisto("FourthJetPhi_Zinc4jet",               "4th jet #phi (N_{jets} #geq 4)",              "#phi(j_{4})",  30,-3, 3);
TH1D *FifthJetPhi_Zinc5jet                = newHisto("FifthJetPhi_Zinc5jet",                "5th jet #phi (N_{jets} #geq 5)",              "#phi(j_{5})",  30,-3, 3);
TH1D *SixthJetPhi_Zinc6jet                = newHisto("SixthJetPhi_Zinc6jet",                "6th jet #phi (N_{jets} #geq 6)",              "#phi(j_{6})",  30,-3, 3);

TH1D *FirstJetPhi_Zexc1jet                = newHisto("FirstJetPhi_Zexc1jet",                "1st jet #phi (N_{jets} = 1)",                 "#phi(j_{1})",  30,-3, 3);
TH1D *SecondJetPhi_Zexc2jet               = newHisto("SecondJetPhi_Zexc2jet",               "2nd jet #phi (N_{jets} = 2)",                 "#phi(j_{2})",  30,-3, 3);

TH1D *AllJetPhi_Zinc1jet                  = newHisto("AllJetPhi_Zinc1jet",                  "All jets #phi (N_{jets} #geq 1)",             "#phi(jets)",   30,-3, 3);
TH1D *AllJetPhi_Zinc2jet                  = newHisto("AllJetPhi_Zinc2jet",                  "All jets #phi (N_{jets} #geq 2)",             "#phi(jets)",   30,-3, 3);
TH1D *AllJetPhi_Zinc3jet                  = newHisto("AllJetPhi_Zinc3jet",                  "All jets #phi (N_{jets} #geq 3)",             "#phi(jets)",   30,-3, 3);
TH1D *AllJetPhi_Zinc4jet                  = newHisto("AllJetPhi_Zinc4jet",                  "All jets #phi (N_{jets} #geq 4)",             "#phi(jets)",   30,-3, 3);

TH1D *lepPt_Zinc0jet                      = newHisto("lepPt_Zinc0jet",                      "1st & 2nd lep p_{T} (N_{jets} #geq 0)",       lpT,     40, 0, 200);
TH1D *lepPt_Zinc1jet                      = newHisto("lepPt_Zinc1jet",                      "1st & 2nd lep p_{T} (N_{jets} #geq 1)",       lpT,     40, 0, 200);
TH1D *lepPt_Zinc2jet                      = newHisto("lepPt_Zinc2jet",                      "1st & 2nd lep p_{T} (N_{jets} #geq 2)",       lpT,     40, 0, 200);
TH1D *lepPt_Zinc3jet                      = newHisto("lepPt_Zinc3jet",                      "1st & 2nd lep p_{T} (N_{jets} #geq 3)",       lpT,     40, 0, 200);
TH1D *lepPt_Zinc4jet                      = newHisto("lepPt_Zinc4jet",                      "1st & 2nd lep p_{T} (N_{jets} #geq 4)",       lpT,     40, 0, 200);
TH1D *lepPt_Zinc5jet                      = newHisto("lepPt_Zinc5jet",                      "1st & 2nd lep p_{T} (N_{jets} #geq 5)",       lpT,     40, 0, 200);

TH1D *genlepPt_Zinc0jet                   = newHisto("genlepPt_Zinc0jet",                   "gen 1st & 2nd lep p_{T} (N_{jets} #geq 0)",   lpT,     40, 0, 200);
TH1D *genlepPt_Zinc1jet                   = newHisto("genlepPt_Zinc1jet",                   "gen 1st & 2nd lep p_{T} (N_{jets} #geq 1)",   lpT,     40, 0, 200);
TH1D *genlepPt_Zinc2jet                   = newHisto("genlepPt_Zinc2jet",                   "gen 1st & 2nd lep p_{T} (N_{jets} #geq 2)",   lpT,     40, 0, 200);

TH1D *lepPt_Zexc0jet                      = newHisto("lepPt_Zexc0jet",                      "1st & 2nd lep p_{T} (N_{jets} = 0)",          lpT,     40, 0, 200);
TH1D *lepPt_Zexc1jet                      = newHisto("lepPt_Zexc1jet",                      "1st & 2nd lep p_{T} (N_{jets} = 1)",          lpT,     40, 0, 200);
TH1D *lepPt_Zexc2jet                      = newHisto("lepPt_Zexc2jet",                      "1st & 2nd lep p_{T} (N_{jets} = 2)",          lpT,     40, 0, 200);
TH1D *lepPt_Zexc3jet                      = newHisto("lepPt_Zexc3jet",                      "1st & 2nd lep p_{T} (N_{jets} = 3)",          lpT,     40, 0, 200);
TH1D *lepPt_Zexc4jet                      = newHisto("lepPt_Zexc4jet",                      "1st & 2nd lep p_{T} (N_{jets} = 4)",          lpT,     40, 0, 200);
TH1D *lepPt_Zexc5jet                      = newHisto("lepPt_Zexc5jet",                      "1st & 2nd lep p_{T} (N_{jets} = 5)",          lpT,     40, 0, 200);

TH1D *dPhiLeptons_Zexc0jet                = newHisto("dPhiLeptons_Zexc0jet",                "#Delta #phi btw lep (N_{jets} = 0)",          ldPhi,     50, 0, PI);
TH1D *dPhiLeptons_Zexc1jet                = newHisto("dPhiLeptons_Zexc1jet",                "#Delta #phi btw lep (N_{jets} = 1)",          ldPhi,     50, 0, PI);
TH1D *dPhiLeptons_Zexc2jet                = newHisto("dPhiLeptons_Zexc2jet",                "#Delta #phi btw lep (N_{jets} = 2)",          ldPhi,     50, 0, PI);
TH1D *dPhiLeptons_Zexc3jet                = newHisto("dPhiLeptons_Zexc3jet",                "#Delta #phi btw lep (N_{jets} = 3)",          ldPhi,     50, 0, PI);
TH1D *dPhiLeptons_Zexc4jet                = newHisto("dPhiLeptons_Zexc4jet",                "#Delta #phi btw lep (N_{jets} = 4)",          ldPhi,     50, 0, PI);
TH1D *dPhiLeptons_Zexc5jet                = newHisto("dPhiLeptons_Zexc5jet",                "#Delta #phi btw lep (N_{jets} = 5)",          ldPhi,     50, 0, PI);

TH1D *dPhiLeptons_Zinc0jet                = newHisto("dPhiLeptons_Zinc0jet",                "#Delta #phi btw lep (N_{jets} #geq 0)",       ldPhi,     50, 0, PI);
TH1D *dPhiLeptons_Zinc1jet                = newHisto("dPhiLeptons_Zinc1jet",                "#Delta #phi btw lep (N_{jets} #geq 1)",       ldPhi,     50, 0, PI);
TH1D *dPhiLeptons_Zinc2jet                = newHisto("dPhiLeptons_Zinc2jet",                "#Delta #phi btw lep (N_{jets} #geq 2)",       ldPhi,     50, 0, PI);
TH1D *dPhiLeptons_Zinc3jet                = newHisto("dPhiLeptons_Zinc3jet",                "#Delta #phi btw lep (N_{jets} #geq 3)",       ldPhi,     50, 0, PI);
TH1D *dPhiLeptons_Zinc4jet                = newHisto("dPhiLeptons_Zinc4jet",                "#Delta #phi btw lep (N_{jets} #geq 4)",       ldPhi,     50, 0, PI);
TH1D *dPhiLeptons_Zinc5jet                = newHisto("dPhiLeptons_Zinc5jet",                "#Delta #phi btw lep (N_{jets} #geq 5)",       ldPhi,     50, 0, PI);

TH1D *dEtaLeptons_Zexc0jet                = newHisto("dEtaLeptons_Zexc0jet",                "#Delta #eta btw lep (N_{jets} = 0)",          ldEta,      50,-5, 5);
TH1D *dEtaLeptons_Zexc1jet                = newHisto("dEtaLeptons_Zexc1jet",                "#Delta #eta btw lep (N_{jets} = 1)",          ldEta,      50,-5, 5);
TH1D *dEtaLeptons_Zexc2jet                = newHisto("dEtaLeptons_Zexc2jet",                "#Delta #eta btw lep (N_{jets} = 2)",          ldEta,      50,-5, 5);
TH1D *dEtaLeptons_Zexc3jet                = newHisto("dEtaLeptons_Zexc3jet",                "#Delta #eta btw lep (N_{jets} = 3)",          ldEta,      50,-5, 5);
TH1D *dEtaLeptons_Zexc4jet                = newHisto("dEtaLeptons_Zexc4jet",                "#Delta #eta btw lep (N_{jets} = 4)",          ldEta,      50,-5, 5);
TH1D *dEtaLeptons_Zexc5jet                = newHisto("dEtaLeptons_Zexc5jet",                "#Delta #eta btw lep (N_{jets} = 5)",          ldEta,      50,-5, 5);

TH1D *dEtaLeptons_Zinc0jet                = newHisto("dEtaLeptons_Zinc0jet",                "#Delta #eta btw lep (N_{jets} #geq 0)",       ldEta,      50,-5, 5);
TH1D *dEtaLeptons_Zinc1jet                = newHisto("dEtaLeptons_Zinc1jet",                "#Delta #eta btw lep (N_{jets} #geq 1)",       ldEta,      50,-5, 5);
TH1D *dEtaLeptons_Zinc2jet                = newHisto("dEtaLeptons_Zinc2jet",                "#Delta #eta btw lep (N_{jets} #geq 2)",       ldEta,      50,-5, 5);
TH1D *dEtaLeptons_Zinc3jet                = newHisto("dEtaLeptons_Zinc3jet",                "#Delta #eta btw lep (N_{jets} #geq 3)",       ldEta,      50,-5, 5);
TH1D *dEtaLeptons_Zinc4jet                = newHisto("dEtaLeptons_Zinc4jet",                "#Delta #eta btw lep (N_{jets} #geq 4)",       ldEta,      50,-5, 5);
TH1D *dEtaLeptons_Zinc5jet                = newHisto("dEtaLeptons_Zinc5jet",                "#Delta #eta btw lep (N_{jets} #geq 5)",       ldEta,      50,-5, 5);

TH1D *dRLeptons_Zinc0jet                  = newHisto("dRLeptons_Zinc0jet",                  "#Delta R btw lep (N_{jets} #geq 0)",          ldR,        50, 0, 5);
TH1D *dRLeptons_Zinc1jet                  = newHisto("dRLeptons_Zinc1jet",                  "#Delta R btw lep (N_{jets} #geq 1)",          ldR,        50, 0, 5);
TH1D *dRLeptons_Zinc2jet                  = newHisto("dRLeptons_Zinc2jet",                  "#Delta R btw lep (N_{jets} #geq 2)",          ldR,        50, 0, 5);
TH1D *dRLeptons_Zinc3jet                  = newHisto("dRLeptons_Zinc3jet",                  "#Delta R btw lep (N_{jets} #geq 3)",          ldR,        50, 0, 5);
TH1D *dRLeptons_Zinc4jet                  = newHisto("dRLeptons_Zinc4jet",                  "#Delta R btw lep (N_{jets} #geq 4)",          ldR,        50, 0, 5);
TH1D *dRLeptons_Zinc5jet                  = newHisto("dRLeptons_Zinc5jet",                  "#Delta R btw lep (N_{jets} #geq 5)",          ldR,        50, 0, 5);

TH1D *SpTLeptons_Zexc0jet                 = newHisto("SpTLeptons_Zexc0jet",                 "#Delta_{pT}^{rel} lep (N_{jets} = 0)",            lSpt,          50, 0, 1);
TH1D *SpTLeptons_Zexc1jet                 = newHisto("SpTLeptons_Zexc1jet",                 "#Delta_{pT}^{rel} lep (N_{jets} = 1)",            lSpt,          50, 0, 1);
TH1D *SpTLeptons_Zexc2jet                 = newHisto("SpTLeptons_Zexc2jet",                 "#Delta_{pT}^{rel} lep (N_{jets} = 2)",            lSpt,          50, 0, 1);
TH1D *SpTLeptons_Zexc3jet                 = newHisto("SpTLeptons_Zexc3jet",                 "#Delta_{pT}^{rel} lep (N_{jets} = 3)",            lSpt,          50, 0, 1);
TH1D *SpTLeptons_Zexc4jet                 = newHisto("SpTLeptons_Zexc4jet",                 "#Delta_{pT}^{rel} lep (N_{jets} = 4)",            lSpt,          50, 0, 1);
TH1D *SpTLeptons_Zexc5jet                 = newHisto("SpTLeptons_Zexc5jet",                 "#Delta_{pT}^{rel} lep (N_{jets} = 5)",            lSpt,          50, 0, 1);

TH1D *genSpTLeptons_Zexc2jet              = newHisto("genSpTLeptons_Zexc2jet",              "gen #Delta_{pT}^{rel} lep (N_{jets} = 2)",        lSpt,          50,0.,1.);

TH1D *SpTLeptons_Zinc0jet                 = newHisto("SpTLeptons_Zinc0jet",                 "#Delta_{pT}^{rel} lep (N_{jets} #geq 0)",         lSpt,          50, 0, 1);
TH1D *SpTLeptons_Zinc1jet                 = newHisto("SpTLeptons_Zinc1jet",                 "#Delta_{pT}^{rel} lep (N_{jets} #geq 1)",         lSpt,          50, 0, 1);
TH1D *SpTLeptons_Zinc2jet                 = newHisto("SpTLeptons_Zinc2jet",                 "#Delta_{pT}^{rel} lep (N_{jets} #geq 2)",         lSpt,          50, 0, 1);
TH1D *SpTLeptons_Zinc3jet                 = newHisto("SpTLeptons_Zinc3jet",                 "#Delta_{pT}^{rel} lep (N_{jets} #geq 3)",         lSpt,          50, 0, 1);
TH1D *SpTLeptons_Zinc4jet                 = newHisto("SpTLeptons_Zinc4jet",                 "#Delta_{pT}^{rel} lep (N_{jets} #geq 4)",         lSpt,          50, 0, 1);
TH1D *SpTLeptons_Zinc5jet                 = newHisto("SpTLeptons_Zinc5jet",                 "#Delta_{pT}^{rel} lep (N_{jets} #geq 5)",         lSpt,          50, 0, 1);

TH1D *genSpTLeptons_Zinc2jet              = newHisto("genSpTLeptons_Zinc2jet",              "gen #Delta_{pT}^{rel} lep (N_{jets} #geq 2)",     lSpt,          50, 0, 1);

TH1D *JetsHT_Zinc1jet                     = newHisto("JetsHT_Zinc1jet",                     "Scalar sum jets p_{T} (N_{jets} #geq 1)",     HT,     19,  1*lowPt, 1*lowPt + 19 * 50);  
TH1D *JetsHT_Zinc2jet                     = newHisto("JetsHT_Zinc2jet",                     "Scalar sum jets p_{T} (N_{jets} #geq 2)",     HT,     19,  2*lowPt, 2*lowPt + 19 * 50);  
TH1D *JetsHT_Zinc3jet                     = newHisto("JetsHT_Zinc3jet",                     "Scalar sum jets p_{T} (N_{jets} #geq 3)",     HT,     18,  3*lowPt, 3*lowPt + 18 * 50);  
TH1D *JetsHT_Zinc4jet                     = newHisto("JetsHT_Zinc4jet",                     "Scalar sum jets p_{T} (N_{jets} #geq 4)",     HT,     12,  4*lowPt, 4*lowPt + 12 * 75);  
TH1D *JetsHT_Zinc5jet                     = newHisto("JetsHT_Zinc5jet",                     "Scalar sum jets p_{T} (N_{jets} #geq 5)",     HT,      6,  5*lowPt, 5*lowPt + 6 * 155);  
TH1D *JetsHT_Zinc6jet                     = newHisto("JetsHT_Zinc6jet",                     "Scalar sum jets p_{T} (N_{jets} #geq 6)",     HT,      3,  6*lowPt, 6*lowPt + 3 * 200);  

TH1D *genJetsHT_Zinc1jet                  = newHisto("genJetsHT_Zinc1jet",                  "Scalar sum jets p_{T} (N_{jets} #geq 1)",     HT,     19,  1*lowPt, 1*lowPt + 19 * 50);  
TH1D *genJetsHT_Zinc2jet                  = newHisto("genJetsHT_Zinc2jet",                  "Scalar sum jets p_{T} (N_{jets} #geq 2)",     HT,     19,  2*lowPt, 2*lowPt + 19 * 50);  
TH1D *genJetsHT_Zinc3jet                  = newHisto("genJetsHT_Zinc3jet",                  "Scalar sum jets p_{T} (N_{jets} #geq 3)",     HT,     18,  3*lowPt, 3*lowPt + 18 * 50);  
TH1D *genJetsHT_Zinc4jet                  = newHisto("genJetsHT_Zinc4jet",                  "Scalar sum jets p_{T} (N_{jets} #geq 4)",     HT,     12,  4*lowPt, 4*lowPt + 12 * 75);  
TH1D *genJetsHT_Zinc5jet                  = newHisto("genJetsHT_Zinc5jet",                  "Scalar sum jets p_{T} (N_{jets} #geq 5)",     HT,      6,  5*lowPt, 5*lowPt + 6 * 155);  
TH1D *genJetsHT_Zinc6jet                  = newHisto("genJetsHT_Zinc6jet",                  "Scalar sum jets p_{T} (N_{jets} #geq 6)",     HT,      3,  6*lowPt, 6*lowPt + 3 * 200);  

TH1D *FirstJetPt_Zinc1jet                 = newHisto("FirstJetPt_Zinc1jet",                 "1st jet p_{T} (N_{jets} #geq 1)",             "p_{T}(j_{1}) [GeV]",     30, lowPt, lowPt + 30 * 15); 
TH1D *SecondJetPt_Zinc2jet                = newHisto("SecondJetPt_Zinc2jet",                "2nd jet p_{T} (N_{jets} #geq 2)",             "p_{T}(j_{2}) [GeV]",     22, lowPt, lowPt + 22 * 15); 
TH1D *ThirdJetPt_Zinc3jet                 = newHisto("ThirdJetPt_Zinc3jet",                 "3rd jet p_{T} (N_{jets} #geq 3)",             "p_{T}(j_{3}) [GeV]",     11, lowPt, lowPt + 11 * 15); 
TH1D *FourthJetPt_Zinc4jet                = newHisto("FourthJetPt_Zinc4jet",                "4th jet p_{T} (N_{jets} #geq 4)",             "p_{T}(j_{4}) [GeV]",      6, lowPt, lowPt +  6 * 15); 
TH1D *FifthJetPt_Zinc5jet                 = newHisto("FifthJetPt_Zinc5jet",                 "5th jet p_{T} (N_{jets} #geq 5)",             "p_{T}(j_{5}) [GeV]",      4, lowPt, lowPt +  4 * 15); 
TH1D *SixthJetPt_Zinc6jet                 = newHisto("SixthJetPt_Zinc6jet",                 "#geq 6th jets p_{T} (N_{jets} #geq 6)",       "p_{T}(j_{6}) [GeV]",      3, lowPt, lowPt +  3 * 15); 

TH1D *genFirstJetPt_Zinc1jet              = newHisto("genFirstJetPt_Zinc1jet",              "gen 1st jet p_{T} (N_{jets} #geq 1)",         "p_{T}(j_{1}) [GeV]",     30, lowPt, lowPt + 30 * 15);
TH1D *genSecondJetPt_Zinc2jet             = newHisto("genSecondJetPt_Zinc2jet",             "gen 2nd jet p_{T} (N_{jets} #geq 2)",         "p_{T}(j_{2}) [GeV]",     22, lowPt, lowPt + 22 * 15);
TH1D *genThirdJetPt_Zinc3jet              = newHisto("genThirdJetPt_Zinc3jet",              "gen 3rd jet p_{T} (N_{jets} #geq 3)",         "p_{T}(j_{3}) [GeV]",     11, lowPt, lowPt + 11 * 15);
TH1D *genFourthJetPt_Zinc4jet             = newHisto("genFourthJetPt_Zinc4jet",             "gen 4th jet p_{T} (N_{jets} #geq 4)",         "p_{T}(j_{4}) [GeV]",      6, lowPt, lowPt +  6 * 15);
TH1D *genFifthJetPt_Zinc5jet              = newHisto("genFifthJetPt_Zinc5jet",              "gen 5th jet p_{T} (N_{jets} #geq 5)",         "p_{T}(j_{5}) [GeV]",      4, lowPt, lowPt +  4 * 15);
TH1D *genSixthJetPt_Zinc6jet              = newHisto("genSixthJetPt_Zinc6jet",              "gen #geq 6th jets p_{T} (N_{jets} #geq 6)",   "p_{T}(j_{6}) [GeV]",      3, lowPt, lowPt +  3 * 15);

TH1D *FirstJetPt_Zinc1jet_test            = newHisto("FirstJetPt_Zinc1jet_test",            "1st jet p_{T} (N_{jets} #geq 1)",             "p_{T}(j_{1}) [GeV]",     30, lowPt, lowPt + 30 * 15);
TH1D *SecondJetPt_Zinc2jet_test           = newHisto("SecondJetPt_Zinc2jet_test",           "2nd jet p_{T} (N_{jets} #geq 2)",             "p_{T}(j_{2}) [GeV]",     22, lowPt, lowPt + 22 * 15);
TH1D *ThirdJetPt_Zinc3jet_test            = newHisto("ThirdJetPt_Zinc3jet_test",            "3rd jet p_{T} (N_{jets} #geq 3)",             "p_{T}(j_{3}) [GeV]",     11, lowPt, lowPt + 11 * 15);
TH1D *FourthJetPt_Zinc4jet_test           = newHisto("FourthJetPt_Zinc4jet_test",           "4th jet p_{T} (N_{jets} #geq 4)",             "p_{T}(j_{4}) [GeV]",      6, lowPt, lowPt +  6 * 15);
TH1D *FifthJetPt_Zinc5jet_test            = newHisto("FifthJetPt_Zinc5jet_test",            "5th jet p_{T} (N_{jets} #geq 5)",             "p_{T}(j_{5}) [GeV]",      4, lowPt, lowPt +  4 * 15);
TH1D *SixthJetPt_Zinc6jet_test            = newHisto("SixthJetPt_Zinc6jet_test",            "#geq 6th jets p_{T} (N_{jets} #geq 6)",       "p_{T}(j_{6}) [GeV]",      3, lowPt, lowPt +  3 * 15);

TH1D *genFirstJetPt_Zinc1jet_test         = newHisto("genFirstJetPt_Zinc1jet_test",         "gen 1st jet p_{T} (N_{jets} #geq 1)",         "p_{T}(j_{1}) [GeV]",     30, lowPt, lowPt + 30 * 15);
TH1D *genSecondJetPt_Zinc2jet_test        = newHisto("genSecondJetPt_Zinc2jet_test",        "gen 2nd jet p_{T} (N_{jets} #geq 2)",         "p_{T}(j_{2}) [GeV]",     22, lowPt, lowPt + 22 * 15);
TH1D *genThirdJetPt_Zinc3jet_test         = newHisto("genThirdJetPt_Zinc3jet_test",         "gen 3rd jet p_{T} (N_{jets} #geq 3)",         "p_{T}(j_{3}) [GeV]",     11, lowPt, lowPt + 11 * 15);
TH1D *genFourthJetPt_Zinc4jet_test        = newHisto("genFourthJetPt_Zinc4jet_test",        "gen 4th jet p_{T} (N_{jets} #geq 4)",         "p_{T}(j_{4}) [GeV]",      6, lowPt, lowPt +  6 * 15);
TH1D *genFifthJetPt_Zinc5jet_test         = newHisto("genFifthJetPt_Zinc5jet_test",         "gen 5th jet p_{T} (N_{jets} #geq 5)",         "p_{T}(j_{5}) [GeV]",      4, lowPt, lowPt +  4 * 15);
TH1D *genSixthJetPt_Zinc6jet_test         = newHisto("genSixthJetPt_Zinc6jet_test",         "gen #geq 6th jets p_{T} (N_{jets} #geq 6)",   "p_{T}(j_{6}) [GeV]",      3, lowPt, lowPt +  3 * 15);

TH1D *FirstJetPt_Zexc1jet                 = newHisto("FirstJetPt_Zexc1jet",                 "1st jet p_{T} (N_{jets} = 1)",                "p_{T}(j_{1}) [GeV]",     30, lowPt, lowPt + 30 * 15); 
TH1D *SecondJetPt_Zexc2jet                = newHisto("SecondJetPt_Zexc2jet",                "2nd jet p_{T} (N_{jets} = 2)",                "p_{T}(j_{2}) [GeV]",     22, lowPt, lowPt + 22 * 15); 

TH1D *genFirstJetPt_Zexc1jet              = newHisto("genFirstJetPt_Zexc1jet",              "gen 1st jet p_{T} (N_{jets} = 1)",            "p_{T}(j_{1}) [GeV]",     30, lowPt, lowPt + 30 * 15);
TH1D *genSecondJetPt_Zexc2jet             = newHisto("genSecondJetPt_Zexc2jet",             "gen 2nd jet p_{T} (N_{jets} = 2)",            "p_{T}(j_{2}) [GeV]",     22, lowPt, lowPt + 22 * 15);


TH1D *FirstHighestJetPt_Zinc1jet          = newHisto("FirstHighestJetPt_Zinc1jet",          "1st Highest jet p_{T} (N_{jets} #geq 1)",     "p_{T}(j_{1}) [GeV]",     30, lowPt, lowPt + 30 * 15); 
TH1D *FirstHighestJetPt_Zinc2jet          = newHisto("FirstHighestJetPt_Zinc2jet",          "1st Highest jet p_{T} (N_{jets} #geq 2)",     "p_{T}(j_{1}) [GeV]",     22, lowPt, lowPt + 22 * 15); 
TH1D *FirstHighestJetPt_Zinc3jet          = newHisto("FirstHighestJetPt_Zinc3jet",          "1st Highest jet p_{T} (N_{jets} #geq 3)",     "p_{T}(j_{1}) [GeV]",     11, lowPt, lowPt + 11 * 15); 
TH1D *FirstHighestJetPt_Zinc4jet          = newHisto("FirstHighestJetPt_Zinc4jet",          "1st Highest jet p_{T} (N_{jets} #geq 4)",     "p_{T}(j_{1}) [GeV]",      6, lowPt, lowPt +  6 * 15); 
TH1D *FirstHighestJetPt_Zinc5jet          = newHisto("FirstHighestJetPt_Zinc5jet",          "1st Highest jet p_{T} (N_{jets} #geq 5)",     "p_{T}(j_{1}) [GeV]",      4, lowPt, lowPt +  4 * 15); 
TH1D *FirstHighestJetPt_Zinc6jet          = newHisto("FirstHighestJetPt_Zinc6jet",          "1st Highest jet p_{T} (N_{jets} #geq 6)",     "p_{T}(j_{1}) [GeV]",      3, lowPt, lowPt +  3 * 15); 

TH1D *genFirstHighestJetPt_Zinc1jet       = newHisto("genFirstHighestJetPt_Zinc1jet",       "gen 1st Highest jet p_{T} (N_{jets} #geq 1)", "p_{T}(j_{1}) [GeV]",     30, lowPt, lowPt + 30 * 15);  
TH1D *genFirstHighestJetPt_Zinc2jet       = newHisto("genFirstHighestJetPt_Zinc2jet",       "gen 1st Highest jet p_{T} (N_{jets} #geq 2)", "p_{T}(j_{1}) [GeV]",     22, lowPt, lowPt + 22 * 15);  
TH1D *genFirstHighestJetPt_Zinc3jet       = newHisto("genFirstHighestJetPt_Zinc3jet",       "gen 1st Highest jet p_{T} (N_{jets} #geq 3)", "p_{T}(j_{1}) [GeV]",     11, lowPt, lowPt + 11 * 15);  
TH1D *genFirstHighestJetPt_Zinc4jet       = newHisto("genFirstHighestJetPt_Zinc4jet",       "gen 1st Highest jet p_{T} (N_{jets} #geq 4)", "p_{T}(j_{1}) [GeV]",      6, lowPt, lowPt +  6 * 15);  
TH1D *genFirstHighestJetPt_Zinc5jet       = newHisto("genFirstHighestJetPt_Zinc5jet",       "gen 1st Highest jet p_{T} (N_{jets} #geq 5)", "p_{T}(j_{1}) [GeV]",      4, lowPt, lowPt +  4 * 15);  
TH1D *genFirstHighestJetPt_Zinc6jet       = newHisto("genFirstHighestJetPt_Zinc6jet",       "gen 1st Highest jet p_{T} (N_{jets} #geq 6)", "p_{T}(j_{1}) [GeV]",      3, lowPt, lowPt +  3 * 15);  

TH1D *FirstHighestJetPt_Zinc1jet_test     = newHisto("FirstHighestJetPt_Zinc1jet_test",     "1st Highest jet p_{T} (N_{jets} #geq 1)",     "p_{T}(j_{1}) [GeV]",     30, lowPt, lowPt + 30 * 15);  
TH1D *FirstHighestJetPt_Zinc2jet_test     = newHisto("FirstHighestJetPt_Zinc2jet_test",     "1st Highest jet p_{T} (N_{jets} #geq 2)",     "p_{T}(j_{1}) [GeV]",     22, lowPt, lowPt + 22 * 15); 
TH1D *FirstHighestJetPt_Zinc3jet_test     = newHisto("FirstHighestJetPt_Zinc3jet_test",     "1st Highest jet p_{T} (N_{jets} #geq 3)",     "p_{T}(j_{1}) [GeV]",     11, lowPt, lowPt + 11 * 15); 
TH1D *FirstHighestJetPt_Zinc4jet_test     = newHisto("FirstHighestJetPt_Zinc4jet_test",     "1st Highest jet p_{T} (N_{jets} #geq 4)",     "p_{T}(j_{1}) [GeV]",      6, lowPt, lowPt +  6 * 15); 
TH1D *FirstHighestJetPt_Zinc5jet_test     = newHisto("FirstHighestJetPt_Zinc5jet_test",     "1st Highest jet p_{T} (N_{jets} #geq 5)",     "p_{T}(j_{1}) [GeV]",      4, lowPt, lowPt +  4 * 15); 
TH1D *FirstHighestJetPt_Zinc6jet_test     = newHisto("FirstHighestJetPt_Zinc6jet_test",     "1st Highest jet p_{T} (N_{jets} #geq 6)",     "p_{T}(j_{1}) [GeV]",      3, lowPt, lowPt +  3 * 15); 

TH1D *genFirstHighestJetPt_Zinc1jet_test  = newHisto("genFirstHighestJetPt_Zinc1jet_test",  "gen 1st Highest jet p_{T} (N_{jets} #geq 1)", "p_{T}(j_{1}) [GeV]",     30, lowPt, lowPt + 30 * 15); 
TH1D *genFirstHighestJetPt_Zinc2jet_test  = newHisto("genFirstHighestJetPt_Zinc2jet_test",  "gen 1st Highest jet p_{T} (N_{jets} #geq 2)", "p_{T}(j_{1}) [GeV]",     22, lowPt, lowPt + 22 * 15); 
TH1D *genFirstHighestJetPt_Zinc3jet_test  = newHisto("genFirstHighestJetPt_Zinc3jet_test",  "gen 1st Highest jet p_{T} (N_{jets} #geq 3)", "p_{T}(j_{1}) [GeV]",     11, lowPt, lowPt + 11 * 15); 
TH1D *genFirstHighestJetPt_Zinc4jet_test  = newHisto("genFirstHighestJetPt_Zinc4jet_test",  "gen 1st Highest jet p_{T} (N_{jets} #geq 4)", "p_{T}(j_{1}) [GeV]",      6, lowPt, lowPt +  6 * 15); 
TH1D *genFirstHighestJetPt_Zinc5jet_test  = newHisto("genFirstHighestJetPt_Zinc5jet_test",  "gen 1st Highest jet p_{T} (N_{jets} #geq 5)", "p_{T}(j_{1}) [GeV]",      4, lowPt, lowPt +  4 * 15); 
TH1D *genFirstHighestJetPt_Zinc6jet_test  = newHisto("genFirstHighestJetPt_Zinc6jet_test",  "gen 1st Highest jet p_{T} (N_{jets} #geq 6)", "p_{T}(j_{1}) [GeV]",      3, lowPt, lowPt +  3 * 15); 

TH1D *SecondHighestJetPt_Zinc2jet         = newHisto("SecondHighestJetPt_Zinc2jet",         "2nd Highest jet p_{T} (N_{jets} #geq 2)",     "p_{T}(j_{2}) [GeV]",     22, lowPt, lowPt + 22 * 15); 
TH1D *SecondHighestJetPt_Zinc3jet         = newHisto("SecondHighestJetPt_Zinc3jet",         "2nd Highest jet p_{T} (N_{jets} #geq 3)",     "p_{T}(j_{2}) [GeV]",     11, lowPt, lowPt + 11 * 15); 
TH1D *SecondHighestJetPt_Zinc4jet         = newHisto("SecondHighestJetPt_Zinc4jet",         "2nd Highest jet p_{T} (N_{jets} #geq 4)",     "p_{T}(j_{2}) [GeV]",      6, lowPt, lowPt +  6 * 15); 
TH1D *SecondHighestJetPt_Zinc5jet         = newHisto("SecondHighestJetPt_Zinc5jet",         "2nd Highest jet p_{T} (N_{jets} #geq 5)",     "p_{T}(j_{2}) [GeV]",      4, lowPt, lowPt +  4 * 15); 
TH1D *SecondHighestJetPt_Zinc6jet         = newHisto("SecondHighestJetPt_Zinc6jet",         "2nd Highest jet p_{T} (N_{jets} #geq 6)",     "p_{T}(j_{2}) [GeV]",      3, lowPt, lowPt +  3 * 15); 

TH1D *genSecondHighestJetPt_Zinc2jet      = newHisto("genSecondHighestJetPt_Zinc2jet",      "gen 2nd Highest jet p_{T} (N_{jets} #geq 2)", "p_{T}(j_{2}) [GeV]",     22, lowPt, lowPt + 22 * 15); 
TH1D *genSecondHighestJetPt_Zinc3jet      = newHisto("genSecondHighestJetPt_Zinc3jet",      "gen 2nd Highest jet p_{T} (N_{jets} #geq 3)", "p_{T}(j_{2}) [GeV]",     11, lowPt, lowPt + 11 * 15); 
TH1D *genSecondHighestJetPt_Zinc4jet      = newHisto("genSecondHighestJetPt_Zinc4jet",      "gen 2nd Highest jet p_{T} (N_{jets} #geq 4)", "p_{T}(j_{2}) [GeV]",      6, lowPt, lowPt +  6 * 15); 
TH1D *genSecondHighestJetPt_Zinc5jet      = newHisto("genSecondHighestJetPt_Zinc5jet",      "gen 2nd Highest jet p_{T} (N_{jets} #geq 5)", "p_{T}(j_{2}) [GeV]",      4, lowPt, lowPt +  4 * 15); 
TH1D *genSecondHighestJetPt_Zinc6jet      = newHisto("genSecondHighestJetPt_Zinc6jet",      "gen 2nd Highest jet p_{T} (N_{jets} #geq 6)", "p_{T}(j_{2}) [GeV]",      3, lowPt, lowPt +  3 * 15); 

TH1D *SecondHighestJetPt_Zinc2jet_test    = newHisto("SecondHighestJetPt_Zinc2jet_test",    "2nd Highest jet p_{T} (N_{jets} #geq 2)",     "p_{T}(j_{2}) [GeV]",     22, lowPt, lowPt + 22 * 15); 
TH1D *SecondHighestJetPt_Zinc3jet_test    = newHisto("SecondHighestJetPt_Zinc3jet_test",    "2nd Highest jet p_{T} (N_{jets} #geq 3)",     "p_{T}(j_{2}) [GeV]",     11, lowPt, lowPt + 11 * 15); 
TH1D *SecondHighestJetPt_Zinc4jet_test    = newHisto("SecondHighestJetPt_Zinc4jet_test",    "2nd Highest jet p_{T} (N_{jets} #geq 4)",     "p_{T}(j_{2}) [GeV]",      6, lowPt, lowPt +  6 * 15); 
TH1D *SecondHighestJetPt_Zinc5jet_test    = newHisto("SecondHighestJetPt_Zinc5jet_test",    "2nd Highest jet p_{T} (N_{jets} #geq 5)",     "p_{T}(j_{2}) [GeV]",      4, lowPt, lowPt +  4 * 15); 
TH1D *SecondHighestJetPt_Zinc6jet_test    = newHisto("SecondHighestJetPt_Zinc6jet_test",    "2nd Highest jet p_{T} (N_{jets} #geq 6)",     "p_{T}(j_{2}) [GeV]",      3, lowPt, lowPt +  3 * 15); 

TH1D *genSecondHighestJetPt_Zinc2jet_test = newHisto("genSecondHighestJetPt_Zinc2jet_test", "gen 2nd Highest jet p_{T} (N_{jets} #geq 2)", "p_{T}(j_{2}) [GeV]",     22, lowPt, lowPt + 22 * 15); 
TH1D *genSecondHighestJetPt_Zinc3jet_test = newHisto("genSecondHighestJetPt_Zinc3jet_test", "gen 2nd Highest jet p_{T} (N_{jets} #geq 3)", "p_{T}(j_{2}) [GeV]",     11, lowPt, lowPt + 11 * 15); 
TH1D *genSecondHighestJetPt_Zinc4jet_test = newHisto("genSecondHighestJetPt_Zinc4jet_test", "gen 2nd Highest jet p_{T} (N_{jets} #geq 4)", "p_{T}(j_{2}) [GeV]",      6, lowPt, lowPt +  6 * 15); 
TH1D *genSecondHighestJetPt_Zinc5jet_test = newHisto("genSecondHighestJetPt_Zinc5jet_test", "gen 2nd Highest jet p_{T} (N_{jets} #geq 5)", "p_{T}(j_{2}) [GeV]",      4, lowPt, lowPt +  4 * 15); 
TH1D *genSecondHighestJetPt_Zinc6jet_test = newHisto("genSecondHighestJetPt_Zinc6jet_test", "gen 2nd Highest jet p_{T} (N_{jets} #geq 6)", "p_{T}(j_{2}) [GeV]",      3, lowPt, lowPt +  3 * 15); 

TH1D *ThirdHighestJetPt_Zinc3jet          = newHisto("ThirdHighestJetPt_Zinc3jet",          "3rd Highest jet p_{T} (N_{jets} #geq 3)",     "p_{T}(j_{3}) [GeV]",     11, lowPt, lowPt + 11 * 15);
TH1D *ThirdHighestJetPt_Zinc4jet          = newHisto("ThirdHighestJetPt_Zinc4jet",          "3rd Highest jet p_{T} (N_{jets} #geq 4)",     "p_{T}(j_{3}) [GeV]",      6, lowPt, lowPt +  6 * 15);
TH1D *ThirdHighestJetPt_Zinc5jet          = newHisto("ThirdHighestJetPt_Zinc5jet",          "3rd Highest jet p_{T} (N_{jets} #geq 5)",     "p_{T}(j_{3}) [GeV]",      4, lowPt, lowPt +  4 * 15);
TH1D *ThirdHighestJetPt_Zinc6jet          = newHisto("ThirdHighestJetPt_Zinc6jet",          "3rd Highest jet p_{T} (N_{jets} #geq 6)",     "p_{T}(j_{3}) [GeV]",      3, lowPt, lowPt +  3 * 15);

TH1D *genThirdHighestJetPt_Zinc3jet       = newHisto("genThirdHighestJetPt_Zinc3jet",       "gen 3rd Highest jet p_{T} (N_{jets} #geq 3)", "p_{T}(j_{3}) [GeV]",     11, lowPt, lowPt + 11 * 15); 
TH1D *genThirdHighestJetPt_Zinc4jet       = newHisto("genThirdHighestJetPt_Zinc4jet",       "gen 3rd Highest jet p_{T} (N_{jets} #geq 4)", "p_{T}(j_{3}) [GeV]",      6, lowPt, lowPt +  6 * 15); 
TH1D *genThirdHighestJetPt_Zinc5jet       = newHisto("genThirdHighestJetPt_Zinc5jet",       "gen 3rd Highest jet p_{T} (N_{jets} #geq 5)", "p_{T}(j_{3}) [GeV]",      4, lowPt, lowPt +  4 * 15); 
TH1D *genThirdHighestJetPt_Zinc6jet       = newHisto("genThirdHighestJetPt_Zinc6jet",       "gen 3rd Highest jet p_{T} (N_{jets} #geq 6)", "p_{T}(j_{3}) [GeV]",      3, lowPt, lowPt +  3 * 15); 

TH1D *ThirdHighestJetPt_Zinc3jet_test     = newHisto("ThirdHighestJetPt_Zinc3jet_test",     "3rd Highest jet p_{T} (N_{jets} #geq 3)",     "p_{T}(j_{3}) [GeV]",     11, lowPt, lowPt + 11 * 15); 
TH1D *ThirdHighestJetPt_Zinc4jet_test     = newHisto("ThirdHighestJetPt_Zinc4jet_test",     "3rd Highest jet p_{T} (N_{jets} #geq 4)",     "p_{T}(j_{3}) [GeV]",      6, lowPt, lowPt +  6 * 15); 
TH1D *ThirdHighestJetPt_Zinc5jet_test     = newHisto("ThirdHighestJetPt_Zinc5jet_test",     "3rd Highest jet p_{T} (N_{jets} #geq 5)",     "p_{T}(j_{3}) [GeV]",      4, lowPt, lowPt +  4 * 15); 
TH1D *ThirdHighestJetPt_Zinc6jet_test     = newHisto("ThirdHighestJetPt_Zinc6jet_test",     "3rd Highest jet p_{T} (N_{jets} #geq 6)",     "p_{T}(j_{3}) [GeV]",      3, lowPt, lowPt +  3 * 15); 

TH1D *genThirdHighestJetPt_Zinc3jet_test  = newHisto("genThirdHighestJetPt_Zinc3jet_test",  "gen 3rd Highest jet p_{T} (N_{jets} #geq 3)", "p_{T}(j_{3}) [GeV]",     11, lowPt, lowPt + 11 * 15); 
TH1D *genThirdHighestJetPt_Zinc4jet_test  = newHisto("genThirdHighestJetPt_Zinc4jet_test",  "gen 3rd Highest jet p_{T} (N_{jets} #geq 4)", "p_{T}(j_{3}) [GeV]",      6, lowPt, lowPt +  6 * 15); 
TH1D *genThirdHighestJetPt_Zinc5jet_test  = newHisto("genThirdHighestJetPt_Zinc5jet_test",  "gen 3rd Highest jet p_{T} (N_{jets} #geq 5)", "p_{T}(j_{3}) [GeV]",      4, lowPt, lowPt +  4 * 15); 
TH1D *genThirdHighestJetPt_Zinc6jet_test  = newHisto("genThirdHighestJetPt_Zinc6jet_test",  "gen 3rd Highest jet p_{T} (N_{jets} #geq 6)", "p_{T}(j_{3}) [GeV]",      3, lowPt, lowPt +  3 * 15); 

TH1D *AllJetPt_Zinc1jet                   = newHisto("AllJetPt_Zinc1jet",                   "All jets (N_{jets} #geq 1)",                  "p_{T}(jets) [GeV]",     80, 0, 400);
TH1D *AllJetPt_Zinc2jet                   = newHisto("AllJetPt_Zinc2jet",                   "All jets (N_{jets} #geq 2)",                  "p_{T}(jets) [GeV]",     80, 0, 400);
TH1D *AllJetPt_Zinc3jet                   = newHisto("AllJetPt_Zinc3jet",                   "All jets (N_{jets} #geq 3)",                  "p_{T}(jets) [GeV]",     80, 0, 400);
TH1D *AllJetPt_Zinc4jet                   = newHisto("AllJetPt_Zinc4jet",                   "All jets (N_{jets} #geq 4)",                  "p_{T}(jets) [GeV]",     80, 0, 400);

TH2D *FirstJetPtEta_Zinc1jet              = new TH2D("FirstJetPtEta_Zinc1jet",              "1st jet p_{T} #eta (N_{jets} #geq 1)",        30, 15, 465,      24,-2.4, 2.4);
TH2D *SecondJetPtEta_Zinc2jet             = new TH2D("SecondJetPtEta_Zinc2jet",             "2nd jet p_{T} #eta (N_{jets} #geq 2)",        30, 15, 465,      24,-2.4, 2.4);
TH2D *ThirdJetPtEta_Zinc3jet              = new TH2D("ThirdJetPtEta_Zinc3jet",              "3rd jet p_{T} #eta (N_{jets} #geq 3)",        30, 15, 465,      24,-2.4, 2.4);
TH2D *FourthJetPtEta_Zinc4jet             = new TH2D("FourthJetPtEta_Zinc4jet",             "4th jet p_{T} #eta (N_{jets} #geq 4)",        30, 15, 465,      24,-2.4, 2.4);
TH2D *FifthJetPtEta_Zinc5jet              = new TH2D("FifthJetPtEta_Zinc5jet",              "5th jet p_{T} #eta (N_{jets} #geq 5)",        30, 15, 465,      24,-2.4, 2.4);
TH2D *SixthJetPtEta_Zinc6jet              = new TH2D("SixthJetPtEta_Zinc6jet",              "6th jet p_{T} #eta (N_{jets} #geq 6)",        30, 15, 465,      24,-2.4, 2.4);

TH2D *genFirstJetPtEta_Zinc1jet           = new TH2D("genFirstJetPtEta_Zinc1jet",           "gen 1st jet p_{T} #eta (N_{jets} #geq 1)",    30, 15, 465,      24,-2.4, 2.4);
TH2D *genSecondJetPtEta_Zinc2jet          = new TH2D("genSecondJetPtEta_Zinc2jet",          "gen 2nd jet p_{T} #eta (N_{jets} #geq 2)",    30, 15, 465,      24,-2.4, 2.4);
TH2D *genThirdJetPtEta_Zinc3jet           = new TH2D("genThirdJetPtEta_Zinc3jet",           "gen 3rd jet p_{T} #eta (N_{jets} #geq 3)",    30, 15, 465,      24,-2.4, 2.4);
TH2D *genFourthJetPtEta_Zinc4jet          = new TH2D("genFourthJetPtEta_Zinc4jet",          "gen 4th jet p_{T} #eta (N_{jets} #geq 4)",    30, 15, 465,      24,-2.4, 2.4);
TH2D *genFifthJetPtEta_Zinc5jet           = new TH2D("genFifthJetPtEta_Zinc5jet",           "gen 5th jet p_{T} #eta (N_{jets} #geq 5)",    30, 15, 465,      24,-2.4, 2.4);
TH2D *genSixthJetPtEta_Zinc6jet           = new TH2D("genSixthJetPtEta_Zinc6jet",           "gen 6th jet p_{T} #eta (N_{jets} #geq 6)",    30, 15, 465,      24,-2.4, 2.4);

TH1D *ZNGoodJets_Zexc = newHisto("ZNGoodJets_Zexc","Jet Counter (excl.)", "N_{jets}", 8, -0.5, 7.5);
ZNGoodJets_Zexc->GetXaxis()->SetBinLabel(1,"N_{jets} = 0");
ZNGoodJets_Zexc->GetXaxis()->SetBinLabel(2,"N_{jets} = 1");
ZNGoodJets_Zexc->GetXaxis()->SetBinLabel(3,"N_{jets} = 2");
ZNGoodJets_Zexc->GetXaxis()->SetBinLabel(4,"N_{jets} = 3");
ZNGoodJets_Zexc->GetXaxis()->SetBinLabel(5,"N_{jets} = 4");
ZNGoodJets_Zexc->GetXaxis()->SetBinLabel(6,"N_{jets} = 5");
ZNGoodJets_Zexc->GetXaxis()->SetBinLabel(7,"N_{jets} = 6");
ZNGoodJets_Zexc->GetXaxis()->SetBinLabel(8,"N_{jets} = 7");

TH1D *ZNGoodJets_Zexc_test = newHisto("ZNGoodJets_Zexc_test","Jet Counter (excl.)", "N_{jets}", 8, -0.5, 7.5);
ZNGoodJets_Zexc_test->GetXaxis()->SetBinLabel(1,"N_{jets} = 0");
ZNGoodJets_Zexc_test->GetXaxis()->SetBinLabel(2,"N_{jets} = 1");
ZNGoodJets_Zexc_test->GetXaxis()->SetBinLabel(3,"N_{jets} = 2");
ZNGoodJets_Zexc_test->GetXaxis()->SetBinLabel(4,"N_{jets} = 3");
ZNGoodJets_Zexc_test->GetXaxis()->SetBinLabel(5,"N_{jets} = 4");
ZNGoodJets_Zexc_test->GetXaxis()->SetBinLabel(6,"N_{jets} = 5");
ZNGoodJets_Zexc_test->GetXaxis()->SetBinLabel(7,"N_{jets} = 6");
ZNGoodJets_Zexc_test->GetXaxis()->SetBinLabel(8,"N_{jets} = 7");

TH1D *ZNGoodJets_Zinc_test = newHisto("ZNGoodJets_Zinc_test","Jet Counter (incl.)", "N_{jets}", 8, -0.5, 7.5);
ZNGoodJets_Zinc_test->GetXaxis()->SetBinLabel(1,"N_{jets} #geq 0");
ZNGoodJets_Zinc_test->GetXaxis()->SetBinLabel(2,"N_{jets} #geq 1");
ZNGoodJets_Zinc_test->GetXaxis()->SetBinLabel(3,"N_{jets} #geq 2");
ZNGoodJets_Zinc_test->GetXaxis()->SetBinLabel(4,"N_{jets} #geq 3");
ZNGoodJets_Zinc_test->GetXaxis()->SetBinLabel(5,"N_{jets} #geq 4");
ZNGoodJets_Zinc_test->GetXaxis()->SetBinLabel(6,"N_{jets} #geq 5");
ZNGoodJets_Zinc_test->GetXaxis()->SetBinLabel(7,"N_{jets} #geq 6");
ZNGoodJets_Zinc_test->GetXaxis()->SetBinLabel(8,"N_{jets} #geq 7");

TH1D *ZNGoodJets_Zinc = newHisto("ZNGoodJets_Zinc","Jet Counter (incl.)", "N_{jets}", 8, -0.5, 7.5);
ZNGoodJets_Zinc->GetXaxis()->SetBinLabel(1,"N_{jets} #geq 0");
ZNGoodJets_Zinc->GetXaxis()->SetBinLabel(2,"N_{jets} #geq 1");
ZNGoodJets_Zinc->GetXaxis()->SetBinLabel(3,"N_{jets} #geq 2");
ZNGoodJets_Zinc->GetXaxis()->SetBinLabel(4,"N_{jets} #geq 3");
ZNGoodJets_Zinc->GetXaxis()->SetBinLabel(5,"N_{jets} #geq 4");
ZNGoodJets_Zinc->GetXaxis()->SetBinLabel(6,"N_{jets} #geq 5");
ZNGoodJets_Zinc->GetXaxis()->SetBinLabel(7,"N_{jets} #geq 6");
ZNGoodJets_Zinc->GetXaxis()->SetBinLabel(8,"N_{jets} #geq 7");

TH1D *ZNGoodJets_Zexc_check = newHisto("ZNGoodJets_Zexc_check","Jet Counter (incl.) CHECK", "N_{jets}", 8, -0.5, 7.5);
ZNGoodJets_Zexc_check->GetXaxis()->SetBinLabel(1,"N_{jets} #geq 0");
ZNGoodJets_Zexc_check->GetXaxis()->SetBinLabel(2,"N_{jets} #geq 1");
ZNGoodJets_Zexc_check->GetXaxis()->SetBinLabel(3,"N_{jets} #geq 2");
ZNGoodJets_Zexc_check->GetXaxis()->SetBinLabel(4,"N_{jets} #geq 3");
ZNGoodJets_Zexc_check->GetXaxis()->SetBinLabel(5,"N_{jets} #geq 4");
ZNGoodJets_Zexc_check->GetXaxis()->SetBinLabel(6,"N_{jets} #geq 5");
ZNGoodJets_Zexc_check->GetXaxis()->SetBinLabel(7,"N_{jets} #geq 6");
ZNGoodJets_Zexc_check->GetXaxis()->SetBinLabel(8,"N_{jets} #geq 7");

TH1D *ZNGoodJets_Zexc_NoWeight = newHisto("ZNGoodJets_Zexc_NoWeight","Unweighted jet Counter (excl.)", "N_{jets}", 8, -0.5, 7.5);
ZNGoodJets_Zexc_NoWeight->GetXaxis()->SetBinLabel(1,"N_{jets} = 0");
ZNGoodJets_Zexc_NoWeight->GetXaxis()->SetBinLabel(2,"N_{jets} = 1");
ZNGoodJets_Zexc_NoWeight->GetXaxis()->SetBinLabel(3,"N_{jets} = 2");
ZNGoodJets_Zexc_NoWeight->GetXaxis()->SetBinLabel(4,"N_{jets} = 3");
ZNGoodJets_Zexc_NoWeight->GetXaxis()->SetBinLabel(5,"N_{jets} = 4");
ZNGoodJets_Zexc_NoWeight->GetXaxis()->SetBinLabel(6,"N_{jets} = 5");
ZNGoodJets_Zexc_NoWeight->GetXaxis()->SetBinLabel(7,"N_{jets} = 6");
ZNGoodJets_Zexc_NoWeight->GetXaxis()->SetBinLabel(8,"N_{jets} = 7");

TH1D *ZNGoodJets_Zinc_NoWeight = newHisto("ZNGoodJets_Zinc_NoWeight","Unweighted jet Counter (incl.)", "N_{jets}", 8, -0.5, 7.5);
ZNGoodJets_Zinc_NoWeight->GetXaxis()->SetBinLabel(1,"N_{jets} #geq 0");
ZNGoodJets_Zinc_NoWeight->GetXaxis()->SetBinLabel(2,"N_{jets} #geq 1");
ZNGoodJets_Zinc_NoWeight->GetXaxis()->SetBinLabel(3,"N_{jets} #geq 2");
ZNGoodJets_Zinc_NoWeight->GetXaxis()->SetBinLabel(4,"N_{jets} #geq 3");
ZNGoodJets_Zinc_NoWeight->GetXaxis()->SetBinLabel(5,"N_{jets} #geq 4");
ZNGoodJets_Zinc_NoWeight->GetXaxis()->SetBinLabel(6,"N_{jets} #geq 5");
ZNGoodJets_Zinc_NoWeight->GetXaxis()->SetBinLabel(7,"N_{jets} #geq 6");
ZNGoodJets_Zinc_NoWeight->GetXaxis()->SetBinLabel(8,"N_{jets} #geq 7");

//DPS histograms
//binning 
int nbinSpt=21;
double binSpt[22]={0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,.45,.5,.55,.6,.65,0.7,0.75,0.8,0.85,0.9,0.94,0.98,1};

//-- jets and Z
TH1D *TwoJetsPtDiff_Zexc2jet        = newHisto("TwoJetsPtDiff_Zexc2jet",        "pT diff of the two highest jet (N_{jets} = 2)",                                "#Delta pT(j_{1}j_{2}) [GeV]",      10,  0, 100);
TH1D *genTwoJetsPtDiff_Zexc2jet     = newHisto("genTwoJetsPtDiff_Zexc2jet",     "gen pT diff of the two highest jet (N_{jets} = 2)",                            "#Delta pT(j_{1}j_{2}) [GeV]",      10,  0, 100);
TH1D *JetsMass_Zexc2jet             = newHisto("JetsMass_Zexc2jet",             "2Jets Invariant Mass (N_{jets} = 2)",                                          Mjj, 24, 20, 620);
TH1D *genJetsMass_Zexc2jet          = newHisto("genJetsMass_Zexc2jet",          "gen 2Jets Invariant Mass (N_{jets} = 2)",                                      Mjj, 24, 20, 620);
TH1D *JetsMass_Zexc2jet_test        = newHisto("JetsMass_Zexc2jet_test",        "2Jets Invariant Mass (N_{jets} = 2)",                                          Mjj, 24, 20, 620);
TH1D *genJetsMass_Zexc2jet_test     = newHisto("genJetsMass_Zexc2jet_test",     "gen 2Jets Invariant Mass (N_{jets} = 2)",                                      Mjj, 24, 20, 620);
TH1D *ptBal_Zexc2jet                = newHisto("ptBal_Zexc2jet",                "Vectorial pT sum: Z_{pT} + Jets_{pT} (N_{jets} = 2)",                          "#Sigma pT [GeV]",      50,  0, 100);
TH1D *genptBal_Zexc2jet             = newHisto("genptBal_Zexc2jet",             "gen Vectorial pT sum: Z_{pT} + Jets_{pT} (N_{jets} = 2)",                      "#Sigma pT [GeV]",      50,  0, 100);
TH1D *dPhiJets_Zexc2jet             = newHisto("dPhiJets_Zexc2jet",             "#Delta#phi btwn jets (N_{jets} = 2)",                                          jdPhi,           20,  0, PI);
TH1D *gendPhiJets_Zexc2jet          = newHisto("gendPhiJets_Zexc2jet",          "gen #Delta#phi btwn jets (N_{jets} = 2)",                                      jdPhi,           20,  0, PI);
TH1D *dEtaJets_Zexc2jet             = newHisto("dEtaJets_Zexc2jet",             "#Delta#eta btwn jets (N_{jets} = 2)",                                          jdEta,           50, -5, 5);
TH1D *gendEtaJets_Zexc2jet          = newHisto("gendEtaJets_Zexc2jet",          "gen #Delta#eta btwn jets (N_{jets} = 2)",                                      jdEta,           50, -5, 5);
TH1D *dEtaFirstJetZ_Zexc2jet        = newHisto("dEtaFirstJetZ_Zexc2jet",        "#Delta#eta btwn Jet_{1} and Z (N_{jets} = 2)",                                 "#Delta#eta(j_{1}Z)",           50, -5, 5);
TH1D *gendEtaFirstJetZ_Zexc2jet     = newHisto("gendEtaFirstJetZ_Zexc2jet",     "gen #Delta#eta btwn Jet_{1} and Z (N_{jets} = 2)",                             "#Delta#eta(j_{1}Z)",           50, -5, 5);
TH1D *dEtaSecondJetZ_Zexc2jet       = newHisto("dEtaSecondJetZ_Zexc2jet",       "#Delta#eta btwn Jet_{2} and Z (N_{jets} = 2)",                                 "#Delta#eta(j_{2}Z)",           50, -5, 5);
TH1D *gendEtaSecondJetZ_Zexc2jet    = newHisto("gendEtaSecondJetZ_Zexc2jet",    "gen #Delta#eta btwn Jet_{2} and Z (N_{jets} = 2)",                             "#Delta#eta(j_{2}Z)",           50, -5, 5);
TH1D *dEtaJet1Plus2Z_Zexc2jet       = newHisto("dEtaJet1Plus2Z_Zexc2jet",       "#Delta#eta btwn jets and Z (N_{jets} = 2)",                                    "#Delta#eta(j_{12}Z)",           50, -5, 5);
TH1D *gendEtaJet1Plus2Z_Zexc2jet    = newHisto("gendEtaJet1Plus2Z_Zexc2jet",    "gen #Delta#eta btwn jets and Z (N_{jets} = 2)",                                "#Delta#eta(j_{12}Z)",           50, -5, 5);
TH1D *PHI_Zexc2jet                  = newHisto("PHI_Zexc2jet",                  "#phi: Angle btwn the two subsystems planes (N_{jets} = 2)",                    "#phi(j_{12}Z)",                 25,  0, PI);
TH1D *genPHI_Zexc2jet               = newHisto("genPHI_Zexc2jet",               "gen #phi: Angle btwn the two subsystems planes (N_{jets} = 2)",                "#phi(j_{12}Z)",                 25,  0, PI);
TH1D *PHI_T_Zexc2jet                = newHisto("PHI_T_Zexc2jet",                "#Delta S Angle btwn lep and jet pair in T-plane (N_{jets} = 2)",            "#Delta S(j_{12}Z)",             10,  0, PI);
TH1D *genPHI_T_Zexc2jet             = newHisto("genPHI_T_Zexc2jet",             "gen #Delta S Angle btwn lep and jet pair in T-plane (N_{jets} = 2)",        "#Delta S(j_{12}Z)",             10,  0, PI);
TH1D *SpT_Zexc2jet                  = newHisto("SpT_Zexc2jet",                  "#Delta_{pT}^{rel} lep and jets combined (N_{jets} = 2)",                   Spt,    20,  0, 1);
TH1D *genSpT_Zexc2jet               = newHisto("genSpT_Zexc2jet",               "gen #Delta_{pT}^{rel} lep and jets combined (N_{jets} = 2)",               Spt,    20,  0, 1);
TH1D *SpTJets_Zexc2jet              = newHisto("SpTJets_Zexc2jet",              "#Delta_{pT}^{rel} jets (N_{jets} = 2)",                                  jSpt,   20,  0, 1);
TH1D *genSpTJets_Zexc2jet           = newHisto("genSpTJets_Zexc2jet",           "gen #Delta_{pT}^{rel} jets (N_{jets} = 2)",                              jSpt,   20,  0, 1);
TH1D *SPhi_Zexc2jet                 = newHisto("SPhi_Zexc2jet",                 "S_{#phi} lep and jets combined (N_{jets} = 2)",                            Sphi,   50,  0, PI);
TH1D *genSPhi_Zexc2jet              = newHisto("genSPhi_Zexc2jet",              "gen S_{#phi} lep and jets combined (N_{jets} = 2)",                        Sphi,   50,  0, PI);

TH1D *TwoJetsPtDiff_Zinc2jet        = newHisto("TwoJetsPtDiff_Zinc2jet",        "pT diff of the two highest jet (N_{jets} #geq 2)",                             "#Delta pT(j_{1}j_{2}) [GeV]",      10,  0, 100);
TH1D *genTwoJetsPtDiff_Zinc2jet     = newHisto("genTwoJetsPtDiff_Zinc2jet",     "gen pT diff of the two highest jet (N_{jets} #geq 2)",                         "#Delta pT(j_{1}j_{2}) [GeV]",      10,  0, 100);
TH1D *BestTwoJetsPtDiff_Zinc2jet    = newHisto("BestTwoJetsPtDiff_Zinc2jet",    "Best pT diff of the two highest jet (N_{jets} #geq 2)",                        "#Delta pT(j_{1}j_{2}) [GeV]",      10,  0, 100);
TH1D *genBestTwoJetsPtDiff_Zinc2jet = newHisto("genBestTwoJetsPtDiff_Zinc2jet", "gen Best pT diff of the two highest jet (N_{jets} #geq 2)",                    "#Delta pT(j_{1}j_{2}) [GeV]",      10,  0, 100);
TH1D *JetsMass_Zinc2jet             = newHisto("JetsMass_Zinc2jet",             "2Jets Invariant Mass (N_{jets} #geq 2)",                                       Mjj, 24, 20, 620);
TH1D *genJetsMass_Zinc2jet          = newHisto("genJetsMass_Zinc2jet",          "gen 2Jets Invariant Mass (N_{jets} #geq 2)",                                   Mjj, 24, 20, 620);
TH1D *BestJetsMass_Zinc2jet         = newHisto("BestJetsMass_Zinc2jet",         "Best 2Jets Invariant Mass (N_{jets} #geq 2)",                                  Mjj, 24, 20, 620);
TH1D *genBestJetsMass_Zinc2jet      = newHisto("genBestJetsMass_Zinc2jet",      "gen Best 2Jets Invariant Mass (N_{jets} #geq 2)",                              Mjj, 24, 20, 620);
TH1D *JetsMass_Zinc2jet_test        = newHisto("JetsMass_Zinc2jet_test",        "2Jets Invariant Mass (N_{jets} #geq 2)",                                       Mjj, 24, 20, 620);
TH1D *genJetsMass_Zinc2jet_test     = newHisto("genJetsMass_Zinc2jet_test",     "gen 2Jets Invariant Mass (N_{jets} #geq 2)",                                   Mjj, 24, 20, 620);
TH1D *ptBal_Zinc2jet                = newHisto("ptBal_Zinc2jet",                "Vectorial pT sum: Z_{pT} + Jets_{pT} (N_{jets} #geq 2)",                       "#Sigma pT [GeV]",      50,  0, 100);
TH1D *genptBal_Zinc2jet             = newHisto("genptBal_Zinc2jet",             "gen Vectorial pT sum: Z_{pT} + Jets_{pT} (N_{jets} #geq 2)",                   "#Sigma pT [GeV]",      50,  0, 100);
TH1D *dPhiJets_Zinc2jet             = newHisto("dPhiJets_Zinc2jet",             "#Delta#phi btwn jets (N_{jets} #geq 2)",                                       jdPhi,           20,  0, PI);
TH1D *gendPhiJets_Zinc2jet          = newHisto("gendPhiJets_Zinc2jet",          "gen #Delta#phi btwn jets (N_{jets} #geq 2)",                                   jdPhi,           20,  0, PI);
TH1D *BestdPhiJets_Zinc2jet         = newHisto("BestdPhiJets_Zinc2jet",         "Best #Delta#phi btwn jets (N_{jets} #geq 2)",                                  jdPhi,           20,  0, PI);
TH1D *genBestdPhiJets_Zinc2jet      = newHisto("genBestdPhiJets_Zinc2jet",      "gen Best #Delta#phi btwn jets (N_{jets} #geq 2)",                              jdPhi,           20,  0, PI);
TH1D *dEtaJets_Zinc2jet             = newHisto("dEtaJets_Zinc2jet",             "#Delta#eta btwn jets (N_{jets} #geq 2)",                                       jdEta,           50, -5, 5);
TH1D *gendEtaJets_Zinc2jet          = newHisto("gendEtaJets_Zinc2jet",          "gen #Delta#eta btwn jets (N_{jets} #geq 2)",                                   jdEta,           50, -5, 5);
TH1D *dEtaFirstJetZ_Zinc2jet        = newHisto("dEtaFirstJetZ_Zinc2jet",        "#Delta#eta btwn Jet_{1} and Z (N_{jets} #geq 2)",                              "#Delta#eta(j_{1}Z)",           50, -5, 5);
TH1D *gendEtaFirstJetZ_Zinc2jet     = newHisto("gendEtaFirstJetZ_Zinc2jet",     "gen #Delta#eta btwn Jet_{1} and Z (N_{jets} #geq 2)",                          "#Delta#eta(j_{1}Z)",           50, -5, 5);
TH1D *dEtaSecondJetZ_Zinc2jet       = newHisto("dEtaSecondJetZ_Zinc2jet",       "#Delta#eta btwn Jet_{2} and Z (N_{jets} #geq 2)",                              "#Delta#eta(j_{2}Z)",           50, -5, 5);
TH1D *gendEtaSecondJetZ_Zinc2jet    = newHisto("gendEtaSecondJetZ_Zinc2jet",    "gen #Delta#eta btwn Jet_{2} and Z (N_{jets} #geq 2)",                          "#Delta#eta(j_{2}Z)",           50, -5, 5);
TH1D *dEtaJet1Plus2Z_Zinc2jet       = newHisto("dEtaJet1Plus2Z_Zinc2jet",       "#Delta#eta btwn jets and Z (N_{jets} #geq 2)",                                 "#Delta#eta(j_{12}Z)",           50, -5, 5);
TH1D *gendEtaJet1Plus2Z_Zinc2jet    = newHisto("gendEtaJet1Plus2Z_Zinc2jet",    "gen #Delta#eta btwn jets and Z (N_{jets} #geq 2)",                             "#Delta#eta(j_{12}Z)",           50, -5, 5);
TH1D *PHI_Zinc2jet                  = newHisto("PHI_Zinc2jet",                  "#phi: Angle btwn the two subsystems planes (N_{jets} #geq 2)",                 "#phi(j_{12}Z)",                 25,  0, PI);
TH1D *genPHI_Zinc2jet               = newHisto("genPHI_Zinc2jet",               "gen #phi: Angle btwn the two subsystems planes (N_{jets} #geq 2)",             "#phi(j_{12}Z)",                 25,  0, PI);
TH1D *BestPHI_Zinc2jet              = newHisto("BestPHI_Zinc2jet",              "Best #phi: Angle btwn the two subsystems planes (N_{jets} #geq 2)",            "#phi(j_{12}Z)",                 25,  0, PI);
TH1D *genBestPHI_Zinc2jet           = newHisto("genBestPHI_Zinc2jet",           "gen Best #phi: Angle btwn the two subsystems planes (N_{jets} #geq 2)",        "#phi(j_{12}Z)",                 25,  0, PI);
TH1D *PHI_T_Zinc2jet                = newHisto("PHI_T_Zinc2jet",                "#Delta S Angle btwn lep and jet pair in T-plane (N_{jets} #geq 2)",         "#Delta S(j_{12}Z)",             10,  0, PI);
TH1D *genPHI_T_Zinc2jet             = newHisto("genPHI_T_Zinc2jet",             "gen #Delta S Angle btwn lep and jet pair in T-plane (N_{jets} #geq 2)",     "#Delta S(j_{12}Z)",             10,  0, PI);
TH1D *BestPHI_T_Zinc2jet            = newHisto("BestPHI_T_Zinc2jet",            "Best #Delta S Angle btwn lep and jet pair in T-plane (N_{jets} #geq 2)",    "#Delta S(j_{12}Z)",             10,  0, PI);
TH1D *genBestPHI_T_Zinc2jet         = newHisto("genBestPHI_T_Zinc2jet",         "gen Best #Delta S Angle btwn lep and jet pair in T-plane (N_{jets} #geq 2)","#Delta S(j_{12}Z)",             10,  0, PI);
TH1D *SpT_Zinc2jet                  = newHisto("SpT_Zinc2jet",                  "#Delta_{pT}^{rel} lep and jets combined (N_{jets} #geq 2)",                 Spt,    20,  0, 1);
TH1D *genSpT_Zinc2jet               = newHisto("genSpT_Zinc2jet",               "gen #Delta_{pT}^{rel} lep and jets combined (N_{jets} #geq 2)",             Spt,    20,  0, 1);
TH1D *BestSpT_Zinc2jet              = newHisto("BestSpT_Zinc2jet",              "Best #Delta_{pT}^{rel} lep and jets combined (N_{jets} #geq 2)",            Spt,    20,  0, 1);
TH1D *genBestSpT_Zinc2jet           = newHisto("genBestSpT_Zinc2jet",           "gen Best #Delta_{pT}^{rel} lep and jets combined (N_{jets} #geq 2)",         Spt,    20,  0, 1);
TH1D *SpTJets_Zinc2jet              = newHisto("SpTJets_Zinc2jet",              "#Delta_{pT}^{rel} jets (N_{jets} #geq 2)",                                jSpt,   20,  0, 1);
TH1D *genSpTJets_Zinc2jet           = newHisto("genSpTJets_Zinc2jet",           "gen #Delta_{pT}^{rel} jets (N_{jets} #geq 2)",                            jSpt,   20,  0, 1);
TH1D *BestSpTJets_Zinc2jet          = newHisto("BestSpTJets_Zinc2jet",          "Best #Delta_{pT}^{rel} jets (N_{jets} #geq 2)",                           jSpt,   20,  0, 1);
TH1D *genBestSpTJets_Zinc2jet       = newHisto("genBestSpTJets_Zinc2jet",       "gen Best #Delta_{pT}^{rel} jets (N_{jets} #geq 2)",                       jSpt,   20,  0, 1);
TH1D *SPhi_Zinc2jet                 = newHisto("SPhi_Zinc2jet",                 "S_{#phi} lep and jets combined (N_{jets} #geq 2)",                          Sphi,   50,  0, PI);
TH1D *genSPhi_Zinc2jet              = newHisto("genSPhi_Zinc2jet",              "gen S_{#phi} lep and jets combined (N_{jets} #geq 2)",                      Sphi,   50,  0, PI);
TH1D *BestSPhi_Zinc2jet             = newHisto("BestSPhi_Zinc2jet",             "Best S_{#phi} lep and jets combined (N_{jets} #geq 2)",                     Sphi,   50,  0, PI);
TH1D *genBestSPhi_Zinc2jet          = newHisto("genBestSPhi_Zinc2jet",          "gen Best S_{#phi} lep and jets combined (N_{jets} #geq 2)",                 Sphi,   50,  0, PI);

//-- low Z pT
TH1D *TwoJetsPtDiff_LowPt_Zexc2jet  = newHisto("TwoJetsPtDiff_LowPt_Zexc2jet",  "pT diff of the two highest jet at low Z_{pT} (N_{jets} = 2)",                  "#Delta pT(j_{1}j_{2}) [GeV]",      10,  0, 100);
TH1D *genTwoJetsPtDiff_LowPt_Zexc2jet = newHisto("genTwoJetsPtDiff_LowPt_Zexc2jet", "gen pT diff of the two highest jet at low Z_{pT} (N_{jets} = 2)",          "#Delta pT(j_{1}j_{2}) [GeV]",      10,  0, 100);
TH1D *JetsMass_LowPt_Zexc2jet       = newHisto("JetsMass_LowPt_Zexc2jet",       "2Jets Invariant Mass at low Z_{pT} (N_{jets} = 2)",                            Mjj, 24, 20, 620);
TH1D *genJetsMass_LowPt_Zexc2jet    = newHisto("genJetsMass_LowPt_Zexc2jet",    "gen 2Jets Invariant Mass at low Z_{pT} (N_{jets} = 2)",                        Mjj, 24, 20, 620);
TH1D *ptBal_LowPt_Zexc2jet          = newHisto("ptBal_LowPt_Zexc2jet",          "Vectorial pT sum: Z_{pT} + Jets_{pT} at low Z_{pT} (N_{jets} = 2)",            "#Sigma pT [GeV]",      50, 0, 100);
TH1D *genptBal_LowPt_Zexc2jet       = newHisto("genptBal_LowPt_Zexc2jet",       "gen Vectorial pT sum: Z_{pT} + Jets_{pT} at low Z_{pT} (N_{jets} = 2)",        "#Sigma pT [GeV]",      50, 0, 100);
TH1D *dPhiJets_LowPt_Zexc2jet       = newHisto("dPhiJets_LowPt_Zexc2jet",       "#Delta#phi btwn jets at low Z_{pT} (N_{jets} = 2)",                            jdPhi,           15, 0, PI);
TH1D *gendPhiJets_LowPt_Zexc2jet    = newHisto("gendPhiJets_LowPt_Zexc2jet",    "gen #Delta#phi btwn jets at low Z_{pT} (N_{jets} = 2)",                        jdPhi,           15, 0, PI);
TH1D *dPhiLeptons_LowPt_Zexc2jet    = newHisto("dPhiLeptons_LowPt_Zexc2jet",    "#Delta#phi btwn leptons at low Z_{pT} (N_{jets} = 2)",                         ldPhi,           50, 0, PI);
TH1D *gendPhiLeptons_LowPt_Zexc2jet = newHisto("gendPhiLeptons_LowPt_Zexc2jet", "gen #Delta#phi btwn leptons at low Z_{pT} (N_{jets} = 2)",                     ldPhi,           50, 0, PI);
TH1D *PHI_LowPt_Zexc2jet            = newHisto("PHI_LowPt_Zexc2jet",            "#phi: Angle btwn the two subsystems planes at low Z_{pT} (N_{jets} = 2)",      "#phi(j_{12}Z)", 25, 0, PI);
TH1D *genPHI_LowPt_Zexc2jet         = newHisto("genPHI_LowPt_Zexc2jet",         "gen #phi: Angle btwn the two subsystems planes at low Z_{pT} (N_{jets} = 2)",  "#phi(j_{12}Z)", 25, 0, PI);
TH1D *PHI_T_LowPt_Zexc2jet          = newHisto("PHI_T_LowPt_Zexc2jet",          "#Delta S Angle btwn lepton and jet pair in T-plane at low Z_{pT} (N_{jets} = 2)",    "#Delta S(j_{12}Z)",             10, 0, PI);
TH1D *genPHI_T_LowPt_Zexc2jet       = newHisto("genPHI_T_LowPt_Zexc2jet",       "gen #Delta S Angle btwn lepton and jet pair in T-plane at low Z_{pT} (N_{jets} = 2)","#Delta S(j_{12}Z)",             10, 0, PI);
TH1D *SpT_LowPt_Zexc2jet            = newHisto("SpT_LowPt_Zexc2jet",            "#Delta_{pT}^{rel} leptons and jets combined at low Z_{pT} (N_{jets} = 2)",     Spt,    25, 0, 1);
TH1D *genSpT_LowPt_Zexc2jet         = newHisto("genSpT_LowPt_Zexc2jet",         "gen #Delta_{pT}^{rel} leptons and jets combined at low Z_{pT} (N_{jets} = 2)", Spt,    25, 0, 1);
TH1D *SpTJets_LowPt_Zexc2jet        = newHisto("SpTJets_LowPt_Zexc2jet",        "#Delta_{pT}^{rel} jets at low Z_{pT} (N_{jets} = 2)",                    jSpt,   15, 0, 1);
TH1D *genSpTJets_LowPt_Zexc2jet     = newHisto("genSpTJets_LowPt_Zexc2jet",     "gen #Delta_{pT}^{rel} jets at low Z_{pT} (N_{jets} = 2)",                jSpt,   15, 0, 1);
TH1D *SpTLeptons_LowPt_Zexc2jet     = newHisto("SpTLeptons_LowPt_Zexc2jet",     "#Delta_{pT}^{rel} leptons at low Z_{pT} (N_{jets} = 2)",                 lSpt,   50, 0, 1);
TH1D *genSpTLeptons_LowPt_Zexc2jet  = newHisto("genSpTLeptons_LowPt_Zexc2jet",  "gen #Delta_{pT}^{rel} leptons at low Z_{pT} (N_{jets} = 2)",             lSpt,   50, 0, 1);
TH1D *SPhi_LowPt_Zexc2jet           = newHisto("SPhi_LowPt_Zexc2jet",           "S_{#phi}: leptons and jets combined at low Z_{pT} (N_{jets} = 2)",             Sphi,   50, 0, PI);
TH1D *genSPhi_LowPt_Zexc2jet        = newHisto("genSPhi_LowPt_Zexc2jet",        "gen S_{#phi}: leptons and jets combined at low Z_{pT} (N_{jets} = 2)",         Sphi,   50, 0, PI);

TH1D *TwoJetsPtDiff_LowPt_Zinc2jet  = newHisto("TwoJetsPtDiff_LowPt_Zinc2jet",  "pT diff of the two highest jet at low Z_{pT} (N_{jets} #geq 2)",                              "#Delta pT(j_{1}j_{2}) [GeV]",   10,  0, 100);
TH1D *genTwoJetsPtDiff_LowPt_Zinc2jet = newHisto("genTwoJetsPtDiff_LowPt_Zinc2jet", "gen pT diff of the two highest jet at low Z_{pT}  (N_{jets} #geq 2)",                     "#Delta pT(j_{1}j_{2}) [GeV]",   10,  0, 100);
TH1D *BestTwoJetsPtDiff_LowPt_Zinc2jet = newHisto("BestTwoJetsPtDiff_LowPt_Zinc2jet", "Best pT diff of the two highest jet at low Z_{pT} (N_{jets} #geq 2)",                   "#Delta pT(j_{1}j_{2}) [GeV]",   10,  0, 100);
TH1D *genBestTwoJetsPtDiff_LowPt_Zinc2jet = newHisto("genBestTwoJetsPtDiff_LowPt_Zinc2jet", "gen Best pT diff of the two highest jet at low Z_{pT} (N_{jets} #geq 2)",         "#Delta pT(j_{1}j_{2}) [GeV]",   10,  0, 100);
TH1D *JetsMass_LowPt_Zinc2jet       = newHisto("JetsMass_LowPt_Zinc2jet",       "2Jets Invariant Mass at low Z_{pT} (N_{jets} #geq 2)",                                        Mjj, 24, 20, 620);
TH1D *genJetsMass_LowPt_Zinc2jet    = newHisto("genJetsMass_LowPt_Zinc2jet",    "gen 2Jets Invariant Mass at low Z_{pT} (N_{jets} #geq 2)",                                    Mjj, 24, 20, 620);
TH1D *BestJetsMass_LowPt_Zinc2jet   = newHisto("BestJetsMass_LowPt_Zinc2jet",   "Best 2Jets Invariant Mass at low Z_{pT} (N_{jets} #geq 2)",                                   Mjj, 24, 20, 620);
TH1D *genBestJetsMass_LowPt_Zinc2jet = newHisto("genBestJetsMass_LowPt_Zinc2jet", "gen Best 2Jets Invariant Mass at low Z_{pT} (N_{jets} #geq 2)",                             Mjj, 24, 20, 620);
TH1D *ptBal_LowPt_Zinc2jet          = newHisto("ptBal_LowPt_Zinc2jet",          "Vectorial pT sum: Z_{pT} + Jets_{pT} at low Z_{pT} (N_{jets} #geq 2)",                        "#Sigma pT [GeV]",      50, 0, 100);
TH1D *genptBal_LowPt_Zinc2jet       = newHisto("genptBal_LowPt_Zinc2jet",       "gen Vectorial pT sum: Z_{pT} + Jets_{pT} at low Z_{pT} (N_{jets} #geq 2)",                    "#Sigma pT [GeV]",      50, 0, 100);
TH1D *dPhiJets_LowPt_Zinc2jet       = newHisto("dPhiJets_LowPt_Zinc2jet",       "#Delta#phi btwn jets at low Z_{pT} (N_{jets} #geq 2)",                                        jdPhi,           15, 0, PI);
TH1D *gendPhiJets_LowPt_Zinc2jet    = newHisto("gendPhiJets_LowPt_Zinc2jet",    "gen#Delta#phi btwn jets at low Z_{pT} (N_{jets} #geq 2)",                                     jdPhi,           15, 0, PI);
TH1D *BestdPhiJets_LowPt_Zinc2jet   = newHisto("BestdPhiJets_LowPt_Zinc2jet",   "Best #Delta#phi btwn jets at low Z_{pT} (N_{jets} #geq 2)",                                   jdPhi,           15, 0, PI);
TH1D *genBestdPhiJets_LowPt_Zinc2jet= newHisto("genBestdPhiJets_LowPt_Zinc2jet","gen Best #Delta#phi btwn jets at low Z_{pT} (N_{jets} #geq 2)",                               jdPhi,           15, 0, PI);
TH1D *dPhiLeptons_LowPt_Zinc2jet    = newHisto("dPhiLeptons_LowPt_Zinc2jet",    "#Delta #phi btwn leptons at low Z_{pT} (N_{jets} #geq 2)",                                    ldPhi,           50, 0, PI);
TH1D *gendPhiLeptons_LowPt_Zinc2jet = newHisto("gendPhiLeptons_LowPt_Zinc2jet", "gen #Delta #phi btwn leptons at low Z_{pT} (N_{jets} #geq 2)",                                ldPhi,           50, 0, PI);
TH1D *PHI_LowPt_Zinc2jet            = newHisto("PHI_LowPt_Zinc2jet",            "#phi: Angle btwn the two subsystems planes at low Z_{pT} (N_{jets} #geq 2)",                  "#phi(j_{12}Z)",        25, 0, PI);
TH1D *genPHI_LowPt_Zinc2jet         = newHisto("genPHI_LowPt_Zinc2jet",         "gen #phi: Angle btwn the two subsystems planes at low Z_{pT} (N_{jets} #geq 2)",              "#phi(j_{12}Z)",        25, 0, PI);
TH1D *BestPHI_LowPt_Zinc2jet        = newHisto("BestPHI_LowPt_Zinc2jet",        "Best #phi: Angle btwn the two subsystems planes at low Z_{pT} (N_{jets} #geq 2)",             "#phi(j_{12}Z)",        25, 0, PI);
TH1D *genBestPHI_LowPt_Zinc2jet     = newHisto("genBestPHI_LowPt_Zinc2jet",     "gen Best #phi: Angle btwn the two subsystems planes at low Z_{pT} (N_{jets} #geq 2)",         "#phi(j_{12}Z)",        25, 0, PI);
TH1D *PHI_T_LowPt_Zinc2jet          = newHisto("PHI_T_LowPt_Zinc2jet",          "#Delta S Angle btwn lepton and jet pair in T-plane at low Z_{pT} (N_{jets} #geq 2)",          "#Delta S(j_{12}Z)",    10, 0, PI);
TH1D *genPHI_T_LowPt_Zinc2jet       = newHisto("genPHI_T_LowPt_Zinc2jet",       "gen #Delta S Angle btwn lepton and jet pair in T-plane at low Z_{pT} (N_{jets} #geq 2)",      "#Delta S(j_{12}Z)",    10, 0, PI);
TH1D *BestPHI_T_LowPt_Zinc2jet      = newHisto("BestPHI_T_LowPt_Zinc2jet",      "Best #Delta S Angle btwn lepton and jet pair in T-plane at low Z_{pT} (N_{jets} #geq 2)",     "#Delta S(j_{12}Z)",    10, 0, PI);
TH1D *genBestPHI_T_LowPt_Zinc2jet   = newHisto("genBestPHI_T_LowPt_Zinc2jet",   "gen Best #Delta S Angle btwn lepton and jet pair in T-plane at low Z_{pT} (N_{jets} #geq 2)", "#Delta S(j_{12}Z)",    10, 0, PI);
TH1D *SpT_LowPt_Zinc2jet            = newHisto("SpT_LowPt_Zinc2jet",            "#Delta_{pT}^{rel} leptons and jets combined at low Z_{pT} (N_{jets} #geq 2)",                 Spt,    25, 0, 1);
TH1D *genSpT_LowPt_Zinc2jet         = newHisto("genSpT_LowPt_Zinc2jet",         "gen #Delta_{pT}^{rel} leptons and jets combined at low Z_{pT} (N_{jets} #geq 2)",             Spt,    25, 0, 1);
TH1D *BestSpT_LowPt_Zinc2jet        = newHisto("BestSpT_LowPt_Zinc2jet",        "Best #Delta_{pT}^{rel} leptons and jets combined at low Z_{pT} (N_{jets} #geq 2)",            Spt,    25, 0, 1);
TH1D *genBestSpT_LowPt_Zinc2jet     = newHisto("genBestSpT_LowPt_Zinc2jet",     "gen Best #Delta_{pT}^{rel} leptons and jets combined at low Z_{pT} (N_{jets} #geq 2)",        Spt,    25, 0, 1);
TH1D *SpTJets_LowPt_Zinc2jet        = newHisto("SpTJets_LowPt_Zinc2jet",        "#Delta_{pT}^{rel} jets at low Z_{pT} (N_{jets} #geq 2)",                                jSpt,   15, 0, 1);
TH1D *genSpTJets_LowPt_Zinc2jet     = newHisto("genSpTJets_LowPt_Zinc2jet",     "gen #Delta_{pT}^{rel} jets at low Z_{pT} (N_{jets} #geq 2)",                            jSpt,   15, 0, 1);
TH1D *BestSpTJets_LowPt_Zinc2jet    = newHisto("BestSpTJets_LowPt_Zinc2jet",    "Best #Delta_{pT}^{rel} jets at low Z_{pT} (N_{jets} #geq 2)",                           jSpt,   15, 0, 1);
TH1D *genBestSpTJets_LowPt_Zinc2jet = newHisto("genBestSpTJets_LowPt_Zinc2jet", "gen Best #Delta_{pT}^{rel} jets at low Z_{pT} (N_{jets} #geq 2)",                       jSpt,   15, 0, 1);
TH1D *SpTLeptons_LowPt_Zinc2jet     = newHisto("SpTLeptons_LowPt_Zinc2jet",     "#Delta_{pT}^{rel} leptons at low Z_{pT} (N_{jets} #geq 2)",                             lSpt,   50, 0, 1);
TH1D *genSpTLeptons_LowPt_Zinc2jet  = newHisto("genSpTLeptons_LowPt_Zinc2jet",  "gen #Delta_{pT}^{rel} leptons at low Z_{pT} (N_{jets} #geq 2)",                         lSpt,   50, 0, 1);
TH1D *SPhi_LowPt_Zinc2jet           = newHisto("SPhi_LowPt_Zinc2jet",           "S_{#phi}: leptons and jets combined at low Z_{pT} (N_{jets} #geq 2)",                         Sphi,   50, 0, PI);
TH1D *genSPhi_LowPt_Zinc2jet        = newHisto("genSPhi_LowPt_Zinc2jet",        "gen S_{#phi}: leptons and jets combined at low Z_{pT} (N_{jets} #geq 2)",                     Sphi,   50, 0, PI);
TH1D *BestSPhi_LowPt_Zinc2jet       = newHisto("BestSPhi_LowPt_Zinc2jet",       "Best S_{#phi}: leptons and jets combined at low Z_{pT} (N_{jets} #geq 2)",                    Sphi,   50, 0, PI);
TH1D *genBestSPhi_LowPt_Zinc2jet    = newHisto("genBestSPhi_LowPt_Zinc2jet",    "gen Best S_{#phi}: leptons and jets combined at low Z_{pT} (N_{jets} #geq 2)",                Sphi,   50, 0, PI);

//-- low Z pT and low SpT
TH1D *PHI_LowSpT_LowPt_Zexc2jet     = newHisto("PHI_LowSpT_LowPt_Zexc2jet",     "#phi: Angle btwn the two subsystems planes at low #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} = 2)","#phi",     25,0.,PI);
TH1D *genPHI_LowSpT_LowPt_Zexc2jet  = newHisto("genPHI_LowSpT_LowPt_Zexc2jet",  "gen #phi: Angle btwn the two subsystems planes at low #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} = 2)","#phi", 25,0.,PI);
TH1D *SPhi_LowSpT_LowPt_Zexc2jet    = newHisto("SPhi_LowSpT_LowPt_Zexc2jet",    "S_{#phi}: leptons and jets combined at low #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} = 2)","S_{#phi}",           50,2.5,PI);
TH1D *genSPhi_LowSpT_LowPt_Zexc2jet = newHisto("genSPhi_LowSpT_LowPt_Zexc2jet", "gen S_{#phi}: leptons and jets combined at low #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} = 2)","S_{#phi}",       50,2.5,PI);

TH1D *PHI_LowSpT_LowPt_Zinc2jet     = newHisto("PHI_LowSpT_LowPt_Zinc2jet",     "#phi: Angle btwn the two subsystems planes at low #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} #geq 2)","#phi",     25,0.,PI);
TH1D *genPHI_LowSpT_LowPt_Zinc2jet  = newHisto("genPHI_LowSpT_LowPt_Zinc2jet",  "gen #phi: Angle btwn the two subsystems planes at low #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} #geq 2)","#phi", 25,0.,PI);
TH1D *SPhi_LowSpT_LowPt_Zinc2jet    = newHisto("SPhi_LowSpT_LowPt_Zinc2jet",    "S_{#phi}: leptons and jets combined at low #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} #geq 2)","S_{#phi}",           50,2.5,PI);
TH1D *genSPhi_LowSpT_LowPt_Zinc2jet = newHisto("genSPhi_LowSpT_LowPt_Zinc2jet", "gen S_{#phi}: leptons and jets combined at low #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} #geq 2)","S_{#phi}",       50,2.5,PI);

//-- low Z pT and high SpT
TH1D *PHI_HighSpT_LowPt_Zexc2jet    = newHisto("PHI_HighSpT_LowPt_Zexc2jet",    "#phi: Angle btwn the two subsystems planes at high #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} = 2)","#phi",    50,0.,PI);
TH1D *genPHI_HighSpT_LowPt_Zexc2jet = newHisto("genPHI_HighSpT_LowPt_Zexc2jet", "gen #phi: Angle btwn the two subsystems planes at high #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} = 2)","#phi",50,0.,PI);
TH1D *SPhi_HighSpT_LowPt_Zexc2jet   = newHisto("SPhi_HighSpT_LowPt_Zexc2jet",   "S_{#phi}: leptons and jets combined at high #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} = 2)","S_{#phi}",          50,0.,PI);
TH1D *genSPhi_HighSpT_LowPt_Zexc2jet  = newHisto("genSPhi_HighSpT_LowPt_Zexc2jet",   "gen S_{#phi}: leptons and jets combined at high #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} = 2)","S_{#phi}", 50,0.,PI);

TH1D *PHI_HighSpT_LowPt_Zinc2jet    = newHisto("PHI_HighSpT_LowPt_Zinc2jet",    "#phi: Angle btwn the two subsystems planes at high #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} #geq 2)","#phi",    50,0.,PI);
TH1D *genPHI_HighSpT_LowPt_Zinc2jet = newHisto("genPHI_HighSpT_LowPt_Zinc2jet", "gen #phi: Angle btwn the two subsystems planes at high #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} #geq 2)","#phi",50,0.,PI);
TH1D *SPhi_HighSpT_LowPt_Zinc2jet   = newHisto("SPhi_HighSpT_LowPt_Zinc2jet",   "S_{#phi}: leptons and jets combined at high #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} #geq 2)","S_{#phi}",          50,0.,PI);
TH1D *genSPhi_HighSpT_LowPt_Zinc2jet  = newHisto("genSPhi_HighSpT_LowPt_Zinc2jet",   "gen S_{#phi}: leptons and jets combined at high #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} #geq 2)","S_{#phi}", 50,0.,PI);

//-- low Z pT and low SPhi
TH1D *SpT_LowSPhi_LowPt_Zexc2jet    = newHisto("SpT_LowSPhi_LowPt_Zexc2jet",    "#Delta_{pT}^{rel} leptons and jets combined at low S_{#phi} and low Z_{pT} (N_{jets} = 2)","#Delta_{pT}^{rel}",             50,0.,1.);
TH1D *genSpT_LowSPhi_LowPt_Zexc2jet    = newHisto("genSpT_LowSPhi_LowPt_Zexc2jet",    "gen #Delta_{pT}^{rel} leptons and jets combined at low S_{#phi} and low Z_{pT} (N_{jets} = 2)","#Delta_{pT}^{rel}",   50,0.,1.);

TH1D *SpT_LowSPhi_LowPt_Zinc2jet    = newHisto("SpT_LowSPhi_LowPt_Zinc2jet",    "#Delta_{pT}^{rel} leptons and jets combined at low S_{#phi} and low Z_{pT} (N_{jets} #geq 2)","#Delta_{pT}^{rel}",             50,0.,1.);
TH1D *genSpT_LowSPhi_LowPt_Zinc2jet    = newHisto("genSpT_LowSPhi_LowPt_Zinc2jet",    "gen #Delta_{pT}^{rel} leptons and jets combined at low S_{#phi} and low Z_{pT} (N_{jets} #geq 2)","#Delta_{pT}^{rel}",   50,0.,1.);

//-- low Z pT and high SPhi
TH1D *SpT_HighSPhi_LowPt_Zexc2jet   = newHisto("SpT_HighSPhi_LowPt_Zexc2jet",   "#Delta_{pT}^{rel} leptons and jets combined at high S_{#phi} and low Z_{pT} (N_{jets} = 2)","#Delta_{pT}^{rel}",            50,0.,1.);
TH1D *genSpT_HighSPhi_LowPt_Zexc2jet   = newHisto("genSpT_HighSPhi_LowPt_Zexc2jet",   "gen #Delta_{pT}^{rel} leptons and jets combined at high S_{#phi} and low Z_{pT} (N_{jets} = 2)","#Delta_{pT}^{rel}",  50,0.,1.);

TH1D *SpT_HighSPhi_LowPt_Zinc2jet   = newHisto("SpT_HighSPhi_LowPt_Zinc2jet",   "#Delta_{pT}^{rel} leptons and jets combined at high S_{#phi} and low Z_{pT} (N_{jets} #geq 2)","#Delta_{pT}^{rel}",            50,0.,1.);
TH1D *genSpT_HighSPhi_LowPt_Zinc2jet   = newHisto("genSpT_HighSPhi_LowPt_Zinc2jet",   "gen #Delta_{pT}^{rel} leptons and jets combined at high S_{#phi} and low Z_{pT} (N_{jets} #geq 2)","#Delta_{pT}^{rel}",  50,0.,1.);

//-- high Z pT
TH1D *ptBal_HighPt_Zexc2jet         = newHisto("ptBal_HighPt_Zexc2jet",         "Vectorial pT sum: Z_{pT} + Jets_{pT} at high Z_{pT} (N_{jets} = 2)","#Sigma pT [GeV]",                 50,0.,100.);
TH1D *genptBal_HighPt_Zexc2jet      = newHisto("genptBal_HighPt_Zexc2jet",      "gen Vectorial pT sum: Z_{pT} + Jets_{pT} at high Z_{pT} (N_{jets} = 2)","#Sigma pT [GeV]",             50,0.,100.);
TH1D *dPhiJets_HighPt_Zexc2jet      = newHisto("dPhiJets_HighPt_Zexc2jet",      "#Delta#phi btwn jets at high Z_{pT} (N_{jets} = 2)",                                          jdPhi,            15, 0, PI);
TH1D *gendPhiJets_HighPt_Zexc2jet   = newHisto("gendPhiJets_HighPt_Zexc2jet",   "gen #Delta#phi btwn jets at high Z_{pT} (N_{jets} = 2)",                                      jdPhi,            15, 0, PI);
TH1D *dPhiLeptons_HighPt_Zexc2jet   = newHisto("dPhiLeptons_HighPt_Zexc2jet",   "#Delta#phi btwn leptons at high Z_{pT} (N_{jets} = 2)",                                       ldPhi,            50,0.,PI);
TH1D *gendPhiLeptons_HighPt_Zexc2jet = newHisto("gendPhiLeptons_HighPt_Zexc2jet",   "gen #Delta#phi btwn leptons at high Z_{pT} (N_{jets} = 2)",                               ldPhi,            50,0.,PI);
TH1D *PHI_HighPt_Zexc2jet           = newHisto("PHI_HighPt_Zexc2jet",           "#phi: Angle btwn the two subsystems planes at high Z_{pT} (N_{jets} = 2)","#phi",                   50,0.,PI);
TH1D *genPHI_HighPt_Zexc2jet        = newHisto("genPHI_HighPt_Zexc2jet",        "gen #phi: Angle btwn the two subsystems planes at high Z_{pT} (N_{jets} = 2)","#phi",               50,0.,PI);
TH1D *PHI_T_HighPt_Zexc2jet         = newHisto("PHI_T_HighPt_Zexc2jet",         "#Delta S Angle btwn lepton and jet pair in T-plane at high Z_{pT} (N_{jets} = 2)","#Delta S",                 10,0.,PI);
TH1D *genPHI_T_HighPt_Zexc2jet      = newHisto("genPHI_T_HighPt_Zexc2jet",      "gen #Delta S Angle btwn lepton and jet pair in T-plane at high Z_{pT} (N_{jets} = 2)","#Delta S",             10,0.,PI);
TH1D *SpT_HighPt_Zexc2jet           = newHisto("SpT_HighPt_Zexc2jet",           "#Delta_{pT}^{rel} leptons and jets combined at high Z_{pT} (N_{jets} = 2)","#Delta_{pT}^{rel}",                             50,0.,1.);
TH1D *genSpT_HighPt_Zexc2jet        = newHisto("genSpT_HighPt_Zexc2jet",        "gen #Delta_{pT}^{rel} leptons and jets combined at high Z_{pT} (N_{jets} = 2)","#Delta_{pT}^{rel}",                         50,0.,1.);
TH1D *SpTJets_HighPt_Zexc2jet       = newHisto("SpTJets_HighPt_Zexc2jet",       "#Delta_{pT}^{rel} jets at high Z_{pT} (N_{jets} = 2)","#Delta_{pT}^{rel}",                                            15,0.,1.);
TH1D *genSpTJets_HighPt_Zexc2jet    = newHisto("genSpTJets_HighPt_Zexc2jet",    "gen #Delta_{pT}^{rel} jets at high Z_{pT} (N_{jets} = 2)","#Delta_{pT}^{rel}",                                        15,0.,1.);
TH1D *SpTLeptons_HighPt_Zexc2jet    = newHisto("SpTLeptons_HighPt_Zexc2jet",    "#Delta_{pT}^{rel} leptons at high Z_{pT} (N_{jets} = 2)","#Delta_{pT}^{rel}",                                         50,0.,1.);
TH1D *genSpTLeptons_HighPt_Zexc2jet = newHisto("genSpTLeptons_HighPt_Zexc2jet", "gen #Delta_{pT}^{rel} leptons at high Z_{pT} (N_{jets} = 2)","#Delta_{pT}^{rel}",                                     50,0.,1.);
TH1D *SPhi_HighPt_Zexc2jet          = newHisto("SPhi_HighPt_Zexc2jet",          "S_{#phi}: leptons and jets combined at high Z_{pT} (N_{jets} = 2)","S_{#phi}",                         50,0.,PI);
TH1D *genSPhi_HighPt_Zexc2jet       = newHisto("genSPhi_HighPt_Zexc2jet",       "gen S_{#phi}: leptons and jets combined at high Z_{pT} (N_{jets} = 2)","S_{#phi}",                     50,0.,PI);

TH1D *ptBal_HighPt_Zinc2jet         = newHisto("ptBal_HighPt_Zinc2jet",         "Vectorial pT sum: Z_{pT} + Jets_{pT} at high Z_{pT} (N_{jets} #geq 2)","#Sigma pT [GeV]",                 50,0.,100.);
TH1D *genptBal_HighPt_Zinc2jet      = newHisto("genptBal_HighPt_Zinc2jet",      "gen Vectorial pT sum: Z_{pT} + Jets_{pT} at high Z_{pT} (N_{jets} #geq 2)","#Sigma pT [GeV]",             50,0.,100.);
TH1D *dPhiJets_HighPt_Zinc2jet      = newHisto("dPhiJets_HighPt_Zinc2jet",      "#Delta#phi btwn jets at high Z_{pT} (N_{jets} #geq 2)",                                       jdPhi,      15, 0, PI);
TH1D *gendPhiJets_HighPt_Zinc2jet   = newHisto("gendPhiJets_HighPt_Zinc2jet",   "gen #Delta#phi btwn jets at high Z_{pT} (N_{jets} #geq 2)",                                   jdPhi,      15, 0, PI);
TH1D *dPhiLeptons_HighPt_Zinc2jet   = newHisto("dPhiLeptons_HighPt_Zinc2jet",   "#Delta#phi btwn leptons at high Z_{pT (N_{jets} #geq 2)}",                                    ldPhi,      50,0.,PI);
TH1D *gendPhiLeptons_HighPt_Zinc2jet   = newHisto("gendPhiLeptons_HighPt_Zinc2jet",   "gen #Delta#phi btwn leptons at high Z_{pT} (N_{jets} #geq 2)",                          ldPhi,      50,0.,PI);
TH1D *PHI_HighPt_Zinc2jet           = newHisto("PHI_HighPt_Zinc2jet",           "#phi: Angle btwn the two subsystems planes at high Z_{pT} (N_{jets} #geq 2)","#phi",                   50,0.,PI);
TH1D *genPHI_HighPt_Zinc2jet        = newHisto("genPHI_HighPt_Zinc2jet",        "gen #phi: Angle btwn the two subsystems planes at high Z_{pT} (N_{jets} #geq 2)","#phi",               50,0.,PI);
TH1D *PHI_T_HighPt_Zinc2jet         = newHisto("PHI_T_HighPt_Zinc2jet",         "#Delta S Angle btwn lepton and jet pair in T-plane at high Z_{pT} (N_{jets} #geq 2)","#Delta S",                 10,0.,PI);
TH1D *genPHI_T_HighPt_Zinc2jet      = newHisto("genPHI_T_HighPt_Zinc2jet",      "gen#Delta S Angle btwn lepton and jet pair in T-plane at high Z_{pT} (N_{jets} #geq 2)","#Delta S",              10,0.,PI);
TH1D *SpT_HighPt_Zinc2jet           = newHisto("SpT_HighPt_Zinc2jet",           "#Delta_{pT}^{rel} leptons and jets combined at high Z_{pT} (N_{jets} #geq 2)","#Delta_{pT}^{rel}",                             50,0.,1.);
TH1D *genSpT_HighPt_Zinc2jet        = newHisto("genSpT_HighPt_Zinc2jet",        "gen #Delta_{pT}^{rel} leptons and jets combined at high Z_{pT} (N_{jets} #geq 2)","#Delta_{pT}^{rel}",                         50,0.,1.);
TH1D *SpTJets_HighPt_Zinc2jet       = newHisto("SpTJets_HighPt_Zinc2jet",       "#Delta_{pT}^{rel} jets at high Z_{pT} (N_{jets} #geq 2)","#Delta_{pT}^{rel}",                                            15,0.,1.);
TH1D *genSpTJets_HighPt_Zinc2jet    = newHisto("genSpTJets_HighPt_Zinc2jet",    "gen #Delta_{pT}^{rel} jets at high Z_{pT} (N_{jets} #geq 2)","#Delta_{pT}^{rel}",                                        15,0.,1.);
TH1D *SpTLeptons_HighPt_Zinc2jet    = newHisto("SpTLeptons_HighPt_Zinc2jet",    "#Delta_{pT}^{rel} leptons at high Z_{pT} (N_{jets} #geq 2)","#Delta_{pT}^{rel}",                                         50,0.,1.);
TH1D *genSpTLeptons_HighPt_Zinc2jet = newHisto("genSpTLeptons_HighPt_Zinc2jet", "gen #Delta_{pT}^{rel} leptons at high Z_{pT} (N_{jets} #geq 2)","#Delta_{pT}^{rel}",                                     50,0.,1.);
TH1D *SPhi_HighPt_Zinc2jet          = newHisto("SPhi_HighPt_Zinc2jet",          "S_{#phi}: leptons and jets combined at high Z_{pT} (N_{jets} #geq 2)","S_{#phi}",                         50,0.,PI);
TH1D *genSPhi_HighPt_Zinc2jet       = newHisto("genSPhi_HighPt_Zinc2jet",       "gen S_{#phi}: leptons and jets combined at high Z_{pT} (N_{jets} #geq 2)","S_{#phi}",                     50,0.,PI);

//-- high Z pT and low SpT
TH1D *PHI_LowSpT_HighPt_Zexc2jet    = newHisto("PHI_LowSpT_HighPt_Zexc2jet",    "#phi: Angle btwn the two subsystems planes at low #Delta_{pT}^{rel} and high Z_{pT} (N_{jets} = 2)","#Phi",    50,0.,PI);
TH1D *SPhi_LowSpT_HighPt_Zexc2jet   = newHisto("SPhi_LowSpT_HighPt_Zexc2jet",   "S_{#phi}: leptons and jets combined at low #Delta_{pT}^{rel} and high Z_{pT} (N_{jets} = 2)","S_{#phi}",          50,2.5,PI);

TH1D *PHI_LowSpT_HighPt_Zinc2jet    = newHisto("PHI_LowSpT_HighPt_Zinc2jet",    "#phi: Angle btwn the two subsystems planes at low #Delta_{pT}^{rel} and high Z_{pT} (N_{jets} #geq 2)","#Phi",    50,0.,PI);
TH1D *SPhi_LowSpT_HighPt_Zinc2jet   = newHisto("SPhi_LowSpT_HighPt_Zinc2jet",   "S_{#phi}: leptons and jets combined at low #Delta_{pT}^{rel} and high Z_{pT} (N_{jets} #geq 2)","S_{#phi}",          50,2.5,PI);

//-- high Z pT and high SpT
TH1D *PHI_HighSpT_HighPt_Zexc2jet   = newHisto("PHI_HighSpT_HighPt_Zexc2jet",   "#phi: Angle btwn the two subsystems planes at high #Delta_{pT}^{rel} and high Z_{pT} (N_{jets} = 2)","#phi",   50,0.,PI);
TH1D *SPhi_HighSpT_HighPt_Zexc2jet  = newHisto("SPhiHighSpT_HighPt_Zexc2jet",   "S_{#phi}: leptons and jets combined at high #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} = 2)","S_{#phi}",          50,0.,PI);

TH1D *PHI_HighSpT_HighPt_Zinc2jet   = newHisto("PHI_HighSpT_HighPt_Zinc2jet",   "#phi: Angle btwn the two subsystems planes at high #Delta_{pT}^{rel} and high Z_{pT} (N_{jets} #geq 2)","#phi",   50,0.,PI);
TH1D *SPhi_HighSpT_HighPt_Zinc2jet  = newHisto("SPhiHighSpT_HighPt_Zinc2jet",   "S_{#phi}: leptons and jets combined at high #Delta_{pT}^{rel} and low Z_{pT} (N_{jets} #geq 2)","S_{#phi}",          50,0.,PI);

//-- high Z pT and low SPhi
TH1D *SpT_LowSPhi_HighPt_Zexc2jet   = newHisto("SpT_LowSPhi_HighPt_Zexc2jet",   "#Delta_{pT}^{rel} leptons and jets combined at low S_{#phi} and high Z_{pT} (N_{jets} = 2)","#Delta_{pT}^{rel}",            50,0.,1.);

TH1D *SpT_LowSPhi_HighPt_Zinc2jet   = newHisto("SpT_LowSPhi_HighPt_Zinc2jet",   "#Delta_{pT}^{rel} leptons and jets combined at low S_{#phi} and high Z_{pT} (N_{jets} #geq 2)","#Delta_{pT}^{rel}",            50,0.,1.);

//-- high Z pT and high SPhi
TH1D *SpT_HighSPhi_HighPt_Zexc2jet  = newHisto("SpT_HighSPhi_HighPt_Zexc2jet",  "#Delta_{pT}^{rel} leptons and jets combined at high S_{#phi} and high Z_{pT} (N_{jets} = 2)","#Delta_{pT}^{rel}",           50,0.,1.);

TH1D *SpT_HighSPhi_HighPt_Zinc2jet  = newHisto("SpT_HighSPhi_HighPt_Zinc2jet",  "#Delta_{pT}^{rel} leptons and jets combined at high S_{#phi} and high Z_{pT} (N_{jets} #geq 2)","#Delta_{pT}^{rel}",           50,0.,1.);

//-- low SPhi
TH1D *SpT_LowSPhi_Zexc2jet          = newHisto("SpT_LowSPhi_Zexc2jet",          "#Delta_{pT}^{rel} leptons and jets combined at low S_{#phi} (N_{jets} = 2)","#Delta_{pT}^{rel}",                            50,0.,1.);

TH1D *SpT_LowSPhi_Zinc2jet          = newHisto("SpT_LowSPhi_Zinc2jet",          "#Delta_{pT}^{rel} leptons and jets combined at low S_{#phi} (N_{jets} #geq 2)","#Delta_{pT}^{rel}",                            50,0.,1.);

//-- high SPhi
TH1D *SpT_HighSPhi_Zexc2jet         = newHisto("SpT_HighSPhi_Zexc2jet",         "#Delta_{pT}^{rel} leptons and jets combined at high S_{#phi} (N_{jets} = 2)","#Delta_{pT}^{rel}",                           50,0.,1.);

TH1D *SpT_HighSPhi_Zinc2jet         = newHisto("SpT_HighSPhi_Zinc2jet",         "#Delta_{pT}^{rel} leptons and jets combined at high S_{#phi} (N_{jets} #geq 2)","#Delta_{pT}^{rel}",                           50,0.,1.);

//-- low SpT
TH1D *PHI_LowSpT_Zexc2jet           = newHisto("PHI_LowSpT_Zexc2jet",           "#phi: Angle btwn the two subsystems planes at low #Delta_{pT}^{rel} (N_{jets} = 2)","#Phi",                    50,0.,PI);
TH1D *SPhi_LowSpT_Zexc2jet          = newHisto("SPhi_LowSpT_Zexc2jet",          "S_{#phi}: leptons and jets combined at low #Delta_{pT}^{rel} (N_{jets} = 2)","S_{#phi}",                          50,2.5,PI);

TH1D *PHI_LowSpT_Zinc2jet           = newHisto("PHI_LowSpT_Zinc2jet",           "#phi: Angle btwn the two subsystems planes at low #Delta_{pT}^{rel} (N_{jets} #geq 2)","#Phi",                    50,0.,PI);
TH1D *SPhi_LowSpT_Zinc2jet          = newHisto("SPhi_LowSpT_Zinc2jet",          "S_{#phi}: leptons and jets combined at low #Delta_{pT}^{rel} (N_{jets} #geq 2)","S_{#phi}",                          50,2.5,PI);

//-- high SpT
TH1D *PHI_HighSpT_Zexc2jet          = newHisto("PHI_HighSpT_Zexc2jet",          "#phi: Angle btwn the two subsystems planes at high #Delta_{pT}^{rel} (N_{jets} = 2)","#Phi",                   50,0.,PI);
TH1D *SPhi_HighSpT_Zexc2jet         = newHisto("SPhi_HighSpT_Zexc2jet",         "S_{#phi}: leptons and jets combined at high #Delta_{pT}^{rel} (N_{jets} = 2)","S_{#phi}",                         50,0.,PI);

TH1D *PHI_HighSpT_Zinc2jet          = newHisto("PHI_HighSpT_Zinc2jet",          "#phi: Angle btwn the two subsystems planes at high #Delta_{pT}^{rel} (N_{jets} #geq 2)","#Phi",                   50,0.,PI);
TH1D *SPhi_HighSpT_Zinc2jet         = newHisto("SPhi_HighSpT_Zinc2jet",         "S_{#phi}: leptons and jets combined at high #Delta_{pT}^{rel} (N_{jets} #geq 2)","S_{#phi}",                         50,0.,PI);

//-- gen stuff
TH1D *genPDFweight                  = newHisto("genPDFweight",                  "event PDF weight all gen events","Count",                                               100,0.5,1.5);
TH1D *genPDFweightPassReco          = newHisto("genPDFweightPassReco",          "event PDF weight:pass reco and gen selection","Count",                                  100,0.5,1.5);


TH1D *gendPhiJetsDeltaR_Zexc2jet    = newHisto("gendPhiJetsDeltaR_Zexc2jet",    "#Delta #phi btwn gen jets with #Delta R < 0.5 (N_{jets} = 2)","#Delta#phi",                         50,0.,PI);
TH1D *resdPhiJetsDeltaR_Zexc2jet    = newHisto("resdPhiJetsDeltaR_Zexc2jet",    "#Delta #phi btwn gen jets with #Delta R < 0.5 (N_{jets} = 2)","#Delta#phi",                         50,-2.5,2.5);
TH1D *genPHI_TDeltaR_Zexc2jet       = newHisto("genPHI_TDeltaR_Zexc2jet",       "#Delta S Angle btwn gen lep and gen jet pair in T-plane with #Delta R < 0.5 (N_{jets} = 2)","#Delta S",    50,0.,PI);
TH1D *resPHI_TDeltaR_Zexc2jet       = newHisto("resPHI_TDeltaR_Zexc2jet",       "#Delta S Angle btwn gen lep and gen jet pair in T-plane with #Delta R < 0.5 (N_{jets} = 2)","#Delta S",    50,-2.5,2.5);
TH1D *genSpTJetsDeltaR_Zexc2jet     = newHisto("genSpTJetsDeltaR_Zexc2jet",     "#Delta_{pT}^{rel} Gen jets with #Delta R < 0.5 (N_{jets} = 2)","#Delta_{pT}^{rel}",                                   50,0.,1.);
TH1D *resSpTJetsDeltaR_Zexc2jet     = newHisto("resSpTJetsDeltaR_Zexc2jet",     "#Delta_{pT}^{rel} Gen jets with #Delta R < 0.5 (N_{jets} = 2)","#Delta_{pT}^{rel}",                                   50,-2.5,2.5);
TH1D *genSpTDeltaR_Zexc2jet         = newHisto("genSpTDeltaR_Zexc2jet",         "#Delta_{pT}^{rel} with #Delta R < 0.5 (N_{jets} = 2)","#Delta_{pT}^{rel}",                                                   50,0.,1.);
TH1D *resSpTDeltaR_Zexc2jet         = newHisto("resSpTDeltaR_Zexc2jet",         "#Delta_{pT}^{rel} with #Delta R < 0.5 (N_{jets} = 2)","#Delta_{pT}^{rel}",                                                   50,-2.5,2.5);

TH1D *gendPhiJetsDPS_Zexc2jet       = newHisto("gendPhiJetsDPS_Zexc2jet",       "#Delta #phi btwn gen jets matching DPS parton (N_{jets} = 2)","#Delta#phi_{j_{1}j_{2}}",            50,0.,PI);
TH1D *gendPhiJetsDPSDeltaR_Zexc2jet = newHisto("gendPhiJetsDPSDeltaR_Zexc2jet", "#Delta #phi btwn gen jets matching DPS parton with #Delta R < 0.5 (N_{jets} = 2)","#Delta#phi",     50,0.,PI);
TH1D *genPHI_TDPS_Zexc2jet          = newHisto("genPHI_TDPS_Zexc2jet",          "#Delta S Angle btwn gen lepton and jet pair in T-plane (N_{jets} = 2)","#Delta S",                         50,0.,PI);
TH1D *genPHI_TDPSDeltaR_Zexc2jet    = newHisto("genPHI_TDPSDeltaR_Zexc2jet",    "#Delta S Angle btwn gen lepton and jet pair in T-plane with #Delta R < 0.5 (N_{jets} = 2)","#Delta S",     50,0.,PI);
TH1D *genSpTJetsDPS_Zexc2jet        = newHisto("genSpTJetsDPS_Zexc2jet",        "#Delta_{pT}^{rel} Gen jets matching DPS parton (N_{jets} = 2)","#Delta_{pT}^{rel}",nbinSpt,binSpt);
TH1D *genSpTJetsDPSDeltaR_Zexc2jet  = newHisto("genSpTJetsDPSDeltaR_Zexc2jet",  "#Delta_{pT}^{rel} Gen jets matching DPS parton with #Delta R < 0.5 (N_{jets} = 2)","#Delta_{pT}^{rel}",nbinSpt,binSpt);
TH1D *genSpTDPS_Zexc2jet            = newHisto("genSpTDPS_Zexc2jet",            "#Delta_{pT}^{rel} with gen jets matching DPS parton (N_{jets} = 2)","#Delta_{pT}^{rel}",nbinSpt,binSpt);
TH1D *genSpTDPSDeltaR_Zexc2jet      = newHisto("genSpTDPSDeltaR_Zexc2jet",      "#Delta_{pT}^{rel} with gen jets matching DPS parton with #Delta R < 0.5 (N_{jets} = 2)","#Delta_{pT}^{rel}",nbinSpt,binSpt);
TH1D *genSpTDPSPartons_Zexc2jet     = newHisto("genSpTDPSPartons_Zexc2jet",     "#Delta_{pT}^{rel} DPS partons (N_{jets} = 2)","#Delta_{pT}^{rel}",nbinSpt,binSpt);


TH1D *genZNGoodJets_Zinc_test = newHisto("genZNGoodJets_Zinc_test","Jet Counter (incl.)", "N_{jets}", 8, -0.5, 7.5);
genZNGoodJets_Zinc_test->GetXaxis()->SetBinLabel(1,"N_{jets} #geq 0");
genZNGoodJets_Zinc_test->GetXaxis()->SetBinLabel(2,"N_{jets} #geq 1");
genZNGoodJets_Zinc_test->GetXaxis()->SetBinLabel(3,"N_{jets} #geq 2");
genZNGoodJets_Zinc_test->GetXaxis()->SetBinLabel(4,"N_{jets} #geq 3");
genZNGoodJets_Zinc_test->GetXaxis()->SetBinLabel(5,"N_{jets} #geq 4");
genZNGoodJets_Zinc_test->GetXaxis()->SetBinLabel(6,"N_{jets} #geq 5");
genZNGoodJets_Zinc_test->GetXaxis()->SetBinLabel(7,"N_{jets} #geq 6");
genZNGoodJets_Zinc_test->GetXaxis()->SetBinLabel(8,"N_{jets} #geq 7");

TH1D *genZNGoodJets_Zinc = newHisto("genZNGoodJets_Zinc","Jet Counter (incl.)", "N_{jets}", 8, -0.5, 7.5);
genZNGoodJets_Zinc->GetXaxis()->SetBinLabel(1,"N_{jets} #geq 0");
genZNGoodJets_Zinc->GetXaxis()->SetBinLabel(2,"N_{jets} #geq 1");
genZNGoodJets_Zinc->GetXaxis()->SetBinLabel(3,"N_{jets} #geq 2");
genZNGoodJets_Zinc->GetXaxis()->SetBinLabel(4,"N_{jets} #geq 3");
genZNGoodJets_Zinc->GetXaxis()->SetBinLabel(5,"N_{jets} #geq 4");
genZNGoodJets_Zinc->GetXaxis()->SetBinLabel(6,"N_{jets} #geq 5");
genZNGoodJets_Zinc->GetXaxis()->SetBinLabel(7,"N_{jets} #geq 6");
genZNGoodJets_Zinc->GetXaxis()->SetBinLabel(8,"N_{jets} #geq 7");

TH1D *genZNGoodJets_Zexc_test = newHisto("genZNGoodJets_Zexc_test","Jet Counter (excl.)", "N_{jets}", 8, -0.5, 7.5);
genZNGoodJets_Zexc_test->GetXaxis()->SetBinLabel(1,"N_{jets} = 0");
genZNGoodJets_Zexc_test->GetXaxis()->SetBinLabel(2,"N_{jets} = 1");
genZNGoodJets_Zexc_test->GetXaxis()->SetBinLabel(3,"N_{jets} = 2");
genZNGoodJets_Zexc_test->GetXaxis()->SetBinLabel(4,"N_{jets} = 3");
genZNGoodJets_Zexc_test->GetXaxis()->SetBinLabel(5,"N_{jets} = 4");
genZNGoodJets_Zexc_test->GetXaxis()->SetBinLabel(6,"N_{jets} = 5");
genZNGoodJets_Zexc_test->GetXaxis()->SetBinLabel(7,"N_{jets} = 6");
genZNGoodJets_Zexc_test->GetXaxis()->SetBinLabel(8,"N_{jets} = 7");

TH1D *genZNGoodJets_Zexc = newHisto("genZNGoodJets_Zexc","Jet Counter (excl.)", "N_{jets}", 8, -0.5, 7.5);
genZNGoodJets_Zexc->GetXaxis()->SetBinLabel(1,"N_{jets} = 0");
genZNGoodJets_Zexc->GetXaxis()->SetBinLabel(2,"N_{jets} = 1");
genZNGoodJets_Zexc->GetXaxis()->SetBinLabel(3,"N_{jets} = 2");
genZNGoodJets_Zexc->GetXaxis()->SetBinLabel(4,"N_{jets} = 3");
genZNGoodJets_Zexc->GetXaxis()->SetBinLabel(5,"N_{jets} = 4");
genZNGoodJets_Zexc->GetXaxis()->SetBinLabel(6,"N_{jets} = 5");
genZNGoodJets_Zexc->GetXaxis()->SetBinLabel(7,"N_{jets} = 6");
genZNGoodJets_Zexc->GetXaxis()->SetBinLabel(8,"N_{jets} = 7");

//Correlations

TH2D *gendPhiJetsDPSDeltaR_ZpT_Zexc2jet = new TH2D("gendPhiJetsDPSDeltaR_ZpT_Zexc2jet", "gendPhiJetsDPSDeltaR_ZpT_Zexc2jet", 50, 0, PI, 100, 0, 100);
TH2D *partonX2D = new TH2D("partonX2D","parton X: x1 vs x2",100,0.0001,0.2,100,0.0001,0.2);

TH1D *gendeltaRjetMu = newHisto("gendeltaRjetMu", "gen delta R btwn jet and muon", "#R", 50, 0., 2.5);

/// additional information
// Muoisolation

TH1D *MuDetIsoRhoCorr  = newHisto("MuDetIsoRhoCorr",  "Muon Detect. Iso #rho corr.", "l_{Iso}^{Det.}", 30, 0, 1.5);
TH1D *MuPFIsoDBetaCorr = newHisto("MuPFIsoDBetaCorr", "Muon PF Iso DBeta corr.",     "l_{Iso}^{PF}",   30, 0, 1.5);

TH1D *deltaRjetMu = newHisto("deltaRjetMu", "delta R btwn jet and muon", "#R", 50, 0., 2.5);
TH1D *deltaPtjetMu = newHisto("deltaPtjetMu", "delta Pt btwn jet and muon if dR<0.5", "#R", 150, -75., 75.);

//TH2D* jecVspt=newHisto("jecVspt","jec Vs pt","jec","pt",80,0.,400,100,0,0.5);
TH1D *NVtx = newHisto("NVtx","Number of vertices","#Vtx",60,0.,60.);

TH1D *Beta = newHisto("Beta","Jet PU variable Beta","Beta",50,0.,1.);
TH1D *BetaStar = newHisto("BetaStar","Jet PU variable BetaStar","BetaStar",50,0.,1.);
TH1D *puBeta_JetsMatchGenJets = newHisto("puBeta_JetsMatchGenJets", "puBeta_JetsMatchGenJets", "Beta", 50, 0, 1);
TH1D *puBetaStar_JetsMatchGenJets = newHisto("puBetaStar_JetsMatchGenJets", "puBetaStar_JetsMatchGenJets", "Beta", 50, 0, 1);
TH1D *puBeta_JetsNoMatchGenJets = newHisto("puBeta_JetsNoMatchGenJets", "puBeta_JetsNoMatchGenJets", "Beta", 50, 0, 1);
TH1D *puBetaStar_JetsNoMatchGenJets = newHisto("puBetaStar_JetsNoMatchGenJets", "puBetaStar_JetsNoMatchGenJets", "Beta", 50, 0, 1);
TH1D *puMVA = newHisto("puMVA","Jet PU variable from MVA","puMVA",40,-1.,1.);
TH1D *puMVA_JetsMatchGenJets = newHisto("puMVA_JetsMatchGenJets","Jet PU variable from MVA for matching jets","puMVA",40,-1.,1.);
TH1D *puMVA_JetsNoMatchGenJets = newHisto("puMVA_JetsNoMatchGenJets","Jet PU variable from MVA for non matching jets","puMVA",40,-1.,1.);
TH2D *puMVAvsBeta = new TH2D("puMVA vs beta","Jet PU variable from MVA vs Beta",50,-1.,1.,50,0.,1.);


TH1D *PUWeight = newHisto("PUWeight","PU weight Z all","PU weight Z all",500,0.,14.);
TH1D *PUWeight0 = newHisto("PUWeight0","PU weight Z+0jet","PU weight Z+0jet",500,0.,14.);
TH1D *PUWeight1 = newHisto("PUWeigh1","PU weight Z+jet>0 ","PU weight Z+jet>0",500,0.,14.);
TH1D *MuPlusPt = newHisto("MuPlusPt","Pt of positive muon","pT [GeV]",150,10.,160.);
TH1D *MuMinusPt = newHisto("MuMinusPt","Pt of negative muon","pT [GeV]",150,10.,160.);
TH1D *MuPlusEta = newHisto("MuPlusEta","#eta of positive muon","#eta",250,-2.5,2.5);
TH1D *MuMinusEta = newHisto("MuMinusEta","#eta of negative muon","#eta",250,-2.5,2.5);



