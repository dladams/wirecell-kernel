// plotResponse.C
//
// David Adams
// June 2020
//
// Make protoDUNE APA response histograms from the 2D reponse histograms
// provided by Wenqiang. The response is the current generated in a wire
// (x, v or u) at z = 0 and fixed x (10 cm?) by charge deposited uniformly
// over the range zc - dz/2 < z < zc + dz/2. The charge is deposited at
// rate of 1 ke/(wire spacing).

#include "getWirecellFile.C"
#include "makeResponseHisto.C"
#include "getResponseLabel.C"

void plotResponse(double zc, double dz, double ymin, double ymax, string fnamin ="") {
  const string myname = "makeResponsePlot: ";
  string fnam = getWirecellFile(fnamin);
  cout << myname << "Input file: " << fnam << endl;
  TH1* phkx = makeResponseHisto("x", fnam, zc, dz);
  TH1* phku = makeResponseHisto("u", fnam, zc, dz);
  TH1* phkv = makeResponseHisto("v", fnam, zc, dz);
  vector<TH1*> hists = {phkx, phkv, phku};
  vector<string> sviews = {"x", "v", "u"};
  vector<string> svcaps = {"X", "V", "U"};
  vector<string> slabs;
  for ( int iview=0; iview<3; ++iview ) {
    TH1* phk = hists[iview];
    string slab = svcaps[iview] + ": ";
    getResponseLabel(phk, slab);
    cout << myname << slab << endl;
    slabs.push_back(slab);
  }
  if ( false ) {
    phkx->Rebin(5);
    phku->Rebin(5);
    phkv->Rebin(5);
  }
  LineColors lc;
  phkx->SetLineColor(lc.blue());
  phkv->SetLineColor(lc.green());
  phku->SetLineColor(lc.red());
  TPadManipulator man;
  TLegend* pleg = man.addLegend(0.12, 0.70, 0.60, 0.88);
  pleg->SetMargin(0.05);
  for ( int ilab=0; ilab<3; ++ilab ) {
    pleg->AddEntry(hists[ilab], slabs[ilab].c_str(), "l");
  }
  man.add(phkx);
  man.add(phkv, "same");
  man.add(phku, "same");
  double xmin = -10.0;    // was -13.2
  double xmax = xmin + 20.0;
  man.setRangeX(xmin, xmax);
  man.setRangeY(ymin, ymax);
  man.addHorizontalLine(0.0);
  man.addAxis();
  ostringstream ssval;
  ssval << int(1000*zc);
  string szc = ssval.str();
  while( szc.size() < 4 ) szc = "0" + szc;
  ssval.str("");
  ssval << int(1000*dz);
  string sdz = ssval.str();
  while( sdz.size() < 4 ) sdz = "0" + sdz;
  string fout = "response" + szc + "w" + sdz + ".{png,tpad}";
  cout << myname << "Printing " << fout << endl;
  man.print(fout);
}

void plotResponse(string fnam ="garfield-1d-boundary-path-rev-dune.root") {
  const string myname = "makeResponsePlot: ";
  string sdir = gSystem->Getenv("WIRECELL_KERNEL_DATA");
  string sfil = sdir + "/" + fnam;
  plotResponse(0.0, 1.0, -0.25, 1.4, sfil);
}
