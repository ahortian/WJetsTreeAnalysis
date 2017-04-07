class MyPDF {
    public:
      MyPDF( const char* pdfname) {
            //LHAPDF::setVerbosity(LHAPDF::SILENT);
           LHAPDF::initPDFSet(1,"cteq6ll.LHpdf");
            //LHAPDF::initPDFSet(1,"cteq6ll.LHgrid");
            // Typical pdfnames to reweight to are:
           //     CT10.LHgrid (this gives 90% CL uncertainties)
            //    MSTW2008nlo68cl.LHgrid, MSTW2008nlo90cl.LHgrid
            //    NNPDF21_100.LHgrid (this gives 68% CL uncertainties)
            LHAPDF::initPDFSet(2,pdfname);
      }
      virtual ~MyPDF(){};
      // This reweights the event to reference to set "pdfname", member "member"
      //
      // For NNPDF2x_100 one should just use all 100 members and calculate 
      // the r.m.s. of the 100 values of the physical observable obtained
      // via reweighting
      //
      // For CTEQ or MSTW, in order to assign systematics one should 
      // use the master formulae. See for instance:
      // http://cmsdoc.cern.ch/cms/PRS/gentools/www/pdfuncert/uncert.html
      double weight( int id1,int id2, double x1,double x2,double Q ,unsigned int member) {
//      double weight( int id1 ,unsigned int member) {
//            int id1 = 1;
            /*int id2 = 2;
            double x1 = 0.1;
            double x2 = 0.01;
            double Q = 100;*/
            if (id1==21) id1=0;
            if (id2==21) id2=0;

            LHAPDF::usePDFMember(1,0);
            double pdf1 = LHAPDF::xfx(1,x1,Q,id1)*LHAPDF::xfx(1,x2,Q,id2);
            LHAPDF::usePDFMember(2,member);
            double pdf2 = LHAPDF::xfx(2,x1,Q,id1)*LHAPDF::xfx(2,x2,Q,id2);
            if (pdf1>0) {
//                cout<<"PDF weight valu:"<<pdf2/pdf1<<endl;
                  return pdf2/pdf1;
            } else {
            cout<<"ID ="<<id1<<"   "<<id2<<"X:  "<<x1<<"   "<<x2<<" Q:  "<<Q<<endl;
            cout<<" x pdf1:"<< LHAPDF::xfx(1,x1,Q,id1)<<"  x pdf2:"<< LHAPDF::xfx(1,x2,Q,id2)<<endl;
            cout<<" ratio:  "<<pdf2/pdf1<<endl;
                  printf ("pdf1 = %e, pdf2 = %e\n", pdf1, pdf2);
                  return 1.;

            }
      }

};
