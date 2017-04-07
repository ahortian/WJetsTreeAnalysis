{
  cout << "\nI'm executing rootlogon.C\n" << endl;
  string currentFile =  __FILE__;
  string currentWorkingDir = currentFile.substr(0, currentFile.find("./rootlogon.C"));
  cout << "The Working Direcotry is:\n\t" << currentWorkingDir << "\n"<< endl;
  gErrorIgnoreLevel = kError;
  string srcdir = currentWorkingDir + "Sources/";
  string incdir = currentWorkingDir + "Includes/";
  string lhapdfdir = "/cvmfs/cms.cern.ch/slc5_amd64_gcc434/external/lhapdf/5.8.5/include/";
  string roounfolddir = currentWorkingDir + "RooUnfold/";  

  cout << "--------------------------------------------------------------------------------\n" << endl; 
  cout << "Adding " << incdir << " to includes directories..." << endl;
  gSystem->AddIncludePath(string("-I" + incdir).c_str());
  cout << "Adding " << lhapdfdir << " to includes directories..." << endl;
  gSystem->AddIncludePath(string("-I" + lhapdfdir).c_str());
  cout << "Adding " << roounfolddir << "src" << " to includes directories..." << endl;
  gSystem->AddIncludePath(string("-I" + roounfolddir + "src").c_str());
  cout << "Include Path -D__USE_XOPEN2K8 to fix lxplus6 compatibility" << endl;
  gSystem->AddIncludePath("-D__USE_XOPEN2K8");
  cout << "Loading RooUnfold libraries..." << endl;
  gSystem->Load(string(roounfolddir + "libRooUnfold").c_str());
  cout << "\n";
  cout << "--------------------------------------------------------------------------------\n" << endl; 
}
