#include "getWirecellFile.C"
#include "getResponseLabel.C"

// iwin = [0, 20], 10 is the diagonal
TH1* makeWirecellHisto(int iwin, string sview) {
  using Index = unsigned int;
  const string myname = "makeWirecell: ";
  string svf;
  if ( sview == "x" ) svf = "W";
  else if ( sview == "u" ) svf = "U";
  else if ( sview == "v" ) svf = "V";
  else {
    cout << myname << "Invalid view: " << sview << endl;
    return nullptr;
  }
  string sfil = getWirecellFile("FieldResp_" + svf + ".txt");
  cout << myname << "Reading file " << sfil << endl;
  ifstream fin(sfil.c_str());
  string slin;
  for ( int ilin=0; ilin<=iwin; ++ilin ) {
    getline(fin, slin);
    if ( fin.eof() ) {
      cout << myname << "End of file reached at line " << ilin << endl;
      return nullptr;
    }
  }
  istringstream sslin(slin);
  double val;
  Index ival = 0;
  Index nval = 300;
  double sum = 0.0;
  string hnam = "hwc" + svf;
  TH1* ph = new TH1F(hnam.c_str(), "; Tick; Response [A.U.]", nval, 0, nval);
  ph->SetStats(0);
  ph->SetLineWidth(2);
  while ( ! sslin.eof() ) {
    sslin >> val;
    //cout << myname << setw(3) << ival << ": " << val << endl;
    ph->SetBinContent(ival+1, val);
    sum += val;
    ++ival;
    if ( ival >= nval ) break;
  }
  cout << myname << "Value count: " << ival << endl;
  cout << myname << "Value sum: " << sum << endl;
  return ph;
}

void plotWirecell(int iwin, double xmin, double xmax, double ymin, double ymax) {
  const string myname = "plotWirecell: ";
  TH1* phkx = makeWirecellHisto(iwin, "x");
  TH1* phku = makeWirecellHisto(iwin, "u");
  TH1* phkv = makeWirecellHisto(iwin, "v");
  LineColors lc;
  phkx->SetLineColor(lc.blue());
  phkv->SetLineColor(lc.green());
  phku->SetLineColor(lc.red());
  TPadManipulator man;
  TLegend* pleg = man.addLegend(0.12, 0.70, 0.60, 0.88);
  pleg->SetMargin(0.05);
  vector<TH1*> hists = {phkx, phkv, phku};
  for ( int ihst=0; ihst<3; ++ihst ) {
    TH1* ph = hists[ihst];
    string slab = "?: ";
    string hnam = ph->GetName();
    slab[0] = toupper(hnam[3]);
    getResponseLabel(ph, slab);
    cout << myname << slab << endl;
    pleg->AddEntry(ph, slab.c_str(), "l");
    string smsg;
    getResponseLabel(ph, smsg, 0.0005);
    cout << myname << smsg << endl;
  }
  man.add(phkx);
  man.add(phku, "same");
  man.add(phkv, "same");
  man.setRangeX(xmin, xmax);
  man.setRangeY(ymin, ymax);
  man.addHorizontalLine(0.0);
  man.addAxis();
  string sttl = "Wirecell response from cell " + to_string(iwin - 10);
  man.setTitle(sttl);
  string swin = to_string(iwin);
  if ( swin.size() < 2 ) swin = "0" + swin;
  string sfout = "wirecellResponse" + swin + ".{png,tpad}";
  cout << myname << "Printing " << sfout << endl;
  man.print(sfout);
}
