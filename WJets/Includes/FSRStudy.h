//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Dec 12 10:27:21 2012 by ROOT version 5.32/01
// from TTree tree/tree
// found on file: ../Data/DMu_DYJets_2012_UNFOLDING_FNAL.root
//////////////////////////////////////////////////////////

#ifndef FSRStudy_h
#define FSRStudy_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <vector>

// Fixed size dimensions of array or collections stored in the TTree if any.
const Int_t kMaxgenLepPt = 1;
const Int_t kMaxgenLepEta = 1;
const Int_t kMaxgenLepPhi = 1;
const Int_t kMaxgenLepE = 1;
const Int_t kMaxgenLepQ = 1;
const Int_t kMaxgenLepId = 1;
const Int_t kMaxgenPhoE = 1;
const Int_t kMaxgenPhoPt = 1;
const Int_t kMaxgenPhoEta = 1;
const Int_t kMaxgenPhoPhi = 1;

class FSRStudy {
   public :
      TTree          *fChain;   //!pointer to the analyzed TTree or TChain
      Int_t           fCurrent; //!current Tree number in a TChain

      // Declaration of leaf types
      Double_t        PU_weight;
      Double_t        PU_npT;
      Double_t        PU_npIT;
      Int_t           EvtInfo_NumVtx;
      vector<double>  *genLepPt_;
      vector<double>  *genLepEta_;
      vector<double>  *genLepPhi_;
      vector<double>  *genLepE_;
      vector<double>  *genLepQ_;
      vector<double>  *genLepSt_;
      vector<double>  *genLepId_;
      vector<double>  *genPhoE_;
      vector<double>  *genPhoPt_;
      vector<double>  *genPhoEta_;
      vector<double>  *genPhoPhi_;
      // List of branches
      TBranch        *b_PU_weight;   //!
      TBranch        *b_PU_npT;   //!
      TBranch        *b_PU_npIT;   //!
      TBranch        *b_EvtInfo_NumVtx;   //!
      TBranch        *b_genLepPt_;   //!
      TBranch        *b_genLepEta_;   //!
      TBranch        *b_genLepPhi_;   //!
      TBranch        *b_genLepE_;   //!
      TBranch        *b_genLepQ_;   //!
      TBranch        *b_genLepSt_;   //!
      TBranch        *b_genLepId_;   //!
      TBranch        *b_genPhoE_;   //!
      TBranch        *b_genPhoPt_;   //!
      TBranch        *b_genPhoEta_;   //!
      TBranch        *b_genPhoPhi_;   //!

      FSRStudy(string fileName_ = "DE_8TeV_DYJets_UNFOLDING_dR_5311_Inf3", TTree *tree=0);
      virtual ~FSRStudy();
      virtual Int_t    Cut(Long64_t entry);
      virtual Int_t    GetEntry(Long64_t entry);
      virtual Long64_t LoadTree(Long64_t entry);
      virtual void     Init(TTree *tree);
      virtual void     Loop();
      virtual Bool_t   Notify();
      virtual void     Show(Long64_t entry = -1);
      string fileName;
      string leptonFlavor;
};

#endif

#ifdef FSRStudy_cxx
FSRStudy::FSRStudy(string fileName_, TTree *tree) : fChain(0), fileName(fileName_)
{
   //-- determine the flavor of the file
   if (fileName.find("DMu") == 0) leptonFlavor = "Muons";
   else if (fileName.find("DE") == 0) leptonFlavor = "Electrons";

   //-- open the file and initialize the tree
   string FILENAME = "../Data_Z_5311/" + fileName + ".root";
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(FILENAME.c_str());
      if (!f || !f->IsOpen()) {
	 f = new TFile(FILENAME.c_str());
      }
      FILENAME = FILENAME + ":/tree";
      TDirectory * dir = (TDirectory*)f->Get(FILENAME.c_str());
      dir->GetObject("tree",tree);

   }
   Init(tree);
}

FSRStudy::~FSRStudy()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t FSRStudy::GetEntry(Long64_t entry)
{
   // Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t FSRStudy::LoadTree(Long64_t entry)
{
   // Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void FSRStudy::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   genLepPt_ = 0;
   genLepEta_ = 0;
   genLepPhi_ = 0;
   genLepE_ = 0;
   genLepQ_ = 0;
   genLepSt_ = 0;
   genLepId_ = 0;
   genPhoE_ = 0;
   genPhoPt_ = 0;
   genPhoEta_ = 0;
   genPhoPhi_ = 0;

  // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("PU_weight", &PU_weight, &b_PU_weight);
   fChain->SetBranchAddress("PU_npT", &PU_npT, &b_PU_npT);
   fChain->SetBranchAddress("PU_npIT", &PU_npIT, &b_PU_npIT);
   fChain->SetBranchAddress("genLepPt_", &genLepPt_, &b_genLepPt_);
   fChain->SetBranchAddress("genLepEta_", &genLepEta_, &b_genLepEta_);
   fChain->SetBranchAddress("genLepPhi_", &genLepPhi_, &b_genLepPhi_);
   fChain->SetBranchAddress("genLepE_", &genLepE_, &b_genLepE_);
   fChain->SetBranchAddress("genLepQ_", &genLepQ_, &b_genLepQ_);
   fChain->SetBranchAddress("genLepSt_", &genLepSt_, &b_genLepSt_);
   fChain->SetBranchAddress("genLepId_", &genLepId_, &b_genLepId_);
   fChain->SetBranchAddress("genPhoE_", &genPhoE_, &b_genPhoE_);
   fChain->SetBranchAddress("genPhoPt_", &genPhoPt_, &b_genPhoPt_);
   fChain->SetBranchAddress("genPhoEta_", &genPhoEta_, &b_genPhoEta_);
   fChain->SetBranchAddress("genPhoPhi_", &genPhoPhi_, &b_genPhoPhi_);
   
   Notify();
}

Bool_t FSRStudy::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void FSRStudy::Show(Long64_t entry)
{
   // Print contents of entry.
   // If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t FSRStudy::Cut(Long64_t entry)
{
   entry++;
   entry--;
   // This function may be called from Loop.
   // returns  1 if entry is accepted.
   // returns -1 otherwise.
   return 1;
}
#endif // #ifdef FSRStudy_cxx
