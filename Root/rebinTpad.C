#include "getResponseLabel.C"

// Rebin the response histograms histograms in a tpad file.
// Input binning is 100 ns and output is ticks (500 ns).
//
// The ouput is ntick bins starting at tick1.
// The input data if offset by ioff bins. e.g. tick 0 includes
// time samples -ioff through -ioff+4.

void rebinTpad(string sfbase, int ioff, int tick1, int ntick) {
  const string myname = "rebinTpad: ";
  string sfin = sfbase + ".tpad";
  string soff = to_string(abs(ioff));
  while ( soff.size() < 3 ) soff = "0" + soff;
  string ssgn = ioff < 0 ? "m" : "p";
  string sfout = sfbase + "RebinOff" + ssgn + soff + ".{png,tpad}";
  TPadManipulator* pman = TPadManipulator::read(sfin);
  TPadManipulator* pmano = new TPadManipulator;
  vector<TH1*> hists;
  hists.push_back(pman->hist());
  vector<TObject*> objs = pman->objects();
  for ( TObject* pobj : objs ) {
    pobj->Print();
    TH1* ph = dynamic_cast<TH1*>(pobj);
    if ( ph != nullptr ) hists.push_back(ph);
  }
  TLegend* pleg = pmano->addLegend(0.12, 0.70, 0.60, 0.88);
  pleg->SetMargin(0.05);
  int rebin = 5;
  string hopt = "hist";
  for ( TH1* phi : hists ) {
    string hnam = phi->GetName();
    hnam += "Tick";
    string httl = phi->GetTitle();
    httl += ", offset " + to_string(ioff);
    httl += "; Tick; Response [/tick]";
    int tick2 = tick1 + ntick;
    cout << "Histogram has " << ntick << " ticks: [" << tick1 << ", " << tick2 << ")" << endl;
    TH1* pho = new TH1F(hnam.c_str(), httl.c_str(), ntick, tick1, tick2);
    pho->SetStats(0);
    pho->SetLineColor(phi->GetLineColor());
    pho->SetLineWidth(2);
    int ibin = -10*lround(phi->GetXaxis()->GetXmin());
    ibin += 5*tick1;
    ibin -= ioff;
    ++ibin;
    int itik = 0;
    // Create fcl.
    string soff = string("off") + ( ioff < 0 ? "m" : "p" ) + to_string(abs(ioff));
    string stout = sfbase + "-" + soff + "-" + hnam.substr(0,1) + ".txt";
    ofstream ftout(stout.c_str());
    ftout << "  ResponseVector: [";
    int ftmod = 10;
    cout << myname << "Rebin factor: " << rebin << endl;
    cout << myname << "Input bin count: " << phi->GetNbinsX() << endl;
    cout << myname << "Starting input bin: " << ibin << endl;
    for ( int itik=0; itik<ntick; ++itik ) {
      double val = 0.0;
      for ( int ireb=0; ireb<rebin; ++ireb ) {
        val += phi->GetBinContent(ibin++);
      }
      pho->SetBinContent(itik+1, val);
      if ( itik ) ftout << ",";
      ftout << (itik%ftmod == 0 ? "\n   " : "");
      ftout << setw(12) << val;
    }
    cout << myname << "Final input bin: " << ibin-1 << endl;
    ftout << "\n  ]\n";
    cout << myname << "Created " << stout << endl;
    pmano->add(pho, hopt);
    hopt = "same";
    string slab = "?: ";
    slab[0] = toupper(hnam[0]);
    getResponseLabel(pho, slab);
    cout << myname << slab << endl;
    pleg->AddEntry(pho, slab.c_str(), "l");
  }
  pmano->addHorizontalLine(0.0);
  string sttl = pman->getTitle();
  sttl += ", offset " + to_string(ioff);
  cout << myname << sttl << endl;
  pmano->setTitle(sttl);
  //pman->setRangeX(-12, 5);
  double ymin = rebin*pman->ymin();
  double ymax = rebin*pman->ymax();
  pmano->setRangeY(ymin, ymax);
  pmano->addAxis();
  cout << myname << "Printing " << sfout << endl;
  pmano->print(sfout);
}

