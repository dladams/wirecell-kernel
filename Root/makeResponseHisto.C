// makeResponseHisto.C
//
// David Adams
// June 2020
//
// Make a protoDUNE APA response histograms from the 2D reponse histograms
// provided by Wenqiang. The response is the current generated in a wire
// (x, v or u) at z = 0 and fixed x (10 cm?) by charge deposited uniformly
// over the range zc - dz/2 < z < zc + dz/2. The charge is deposited at
// rate of 1 ke/(wire spacing).

#include "jsonToHist.C"

TH1* makeResponseHisto(string sview, string fnamin, double zc, double dz) {
  const string myname = "makeResponseHisto: ";
  string fnam = fnamin;
  string sdir = gSystem->Getenv("WIRECELL_KERNEL_DATA");
  if ( fnam == "root" ) fnam = sdir + "dune-garfield-1d565.root";
  if ( fnam == "json" ) fnam = sdir + "dune-garfield-1d565.json";
  map<string, string> frnams;
  frnams["u"] = "FieldRes_U";
  frnams["v"] = "FieldRes_V";
  frnams["x"] = "FieldRes_W";
  if ( frnams.count(sview) == 0 ) {
    cout << myname << "Invalid view: " << sview << endl;
    return nullptr;
  }
  TH2* phr = nullptr;
  if ( fnam.find("root") != string::npos ) {
    TFile* pfil = TFile::Open(fnam.c_str());
    if ( ! pfil->IsOpen() ) {
      cout << myname << "Unable to open " << fnam << endl;
      return nullptr;
    }
    string hnamr = frnams[sview];
    phr = dynamic_cast<TH2*>(pfil->Get(hnamr.c_str()));
    if ( phr == nullptr ) {
      cout << myname << "Unable to open histogram " << hnamr << endl;
      pfil->ls();
      return nullptr;
    }
  } else {
    phr = jsonToHist(sview, fnam);
    if ( phr == nullptr ) return nullptr;
  }
  phr->Print();
  cout << phr->GetTitle() << endl;
  phr->GetXaxis()->Print();
  phr->GetYaxis()->Print();
  phr->GetZaxis()->Print();
  string hnamk = sview + "kernel0";
  TH1* phk = nullptr;
  double xbin0 = 106.5;    // Bin coordinate where x=0
  xbin0 = 106.0;    // oct20: More symmetric and area = 1.002 for center
  double wbin = 0.10;
  double xbin1 = xbin0 + (zc - 0.5*dz)/wbin;
  double xbin2 = xbin0 + (zc + 0.5*dz)/wbin;
  double qdep = 1.00*(xbin2 - xbin1);
  int ibin1 = xbin1;
  int ibin2 = xbin2;
  double wt1 = ibin1 + 1 - xbin1;
  double wt2 = xbin2 - ibin2;
  cout << myname << "Bin range: " << ibin1 << ", " << ibin2
       << " (" << wt1 << ", " << wt2 << ")" << endl;
  if ( ibin2 > ibin1 + 1 ) {
    phk = phr->ProjectionY(hnamk.c_str(), ibin1+1, ibin2-1);
    phk->Add(phr->ProjectionY("htmp", ibin1, ibin1), wt1);
    phk->Add(phr->ProjectionY("htmp", ibin2, ibin2), wt2);
  } else if ( ibin2 > ibin1 ) {
    phk = phr->ProjectionY(hnamk.c_str(), ibin1, ibin1);
    phk->Scale(wt1);
    phk->Add(phr->ProjectionY("htmp", ibin2, ibin2), wt2);
  } else {
    phk = phr->ProjectionY(hnamk.c_str(), ibin1, ibin1);
    phk->Scale(xbin2 - xbin1);
  }
  phk->Sumw2(false);
  ostringstream ssttl;
  ssttl << "Response for charge deposited at wire " << zc << " with width " << dz;
  string sttl = ssttl.str();
  phk->SetTitle(sttl.c_str());
  ostringstream ssytl;
  //ssytl << "Current [#muA/(" << qdep << " ke)]";
  ssytl << "Response [/(0.1 #mus)]";
  string sytl = ssytl.str();
  phk->GetYaxis()->SetTitle(sytl.c_str());
  // Scale to uA/(ke/wire)
  //double sfac = 1000.0/10.0;
  double sfac = 10.0;
  phk->Scale(sfac);
  phk->SetStats(0);
  phk->SetLineWidth(2);
  return phk;
}

TH1* makeSymmetricResponseHisto(string sview, string fnamin, double zc, double dz) {
  TH1* ph = makeResponseHisto(sview, fnamin, zc, dz);
  string sviewtmp = sview;
  TH1* phtmp = makeResponseHisto(sviewtmp, fnamin, -zc, dz);
  ph->Add(phtmp, 1.0);
  ph->Scale(0.5);
  ostringstream ssttl;
  ssttl << "Symmetrized response for charge deposited at wire " << zc << " with width " << dz;
  string sttl = ssttl.str();
  ph->SetTitle(sttl.c_str());
  return ph;
}
