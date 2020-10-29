#include "cesmearTH1.C"
#include "getResponseLabel.C"

// Smear the histograms in a tpad file.
void cesmearTpad(string sfbase, double ymin, double ymax) {
  const string myname = "cesmearTpad: ";
  string sfin = sfbase + ".tpad";
  string sfout = sfbase + "Smeared.{png,tpad}";
  TPadManipulator* pman = TPadManipulator::read(sfin);
  vector<TH1*> hists;
  hists.push_back(pman->hist());
  vector<TObject*> objs = pman->objects();
  for ( TObject* pobj : objs ) {
    pobj->Print();
    TH1* ph = dynamic_cast<TH1*>(pobj);
    if ( ph != nullptr ) hists.push_back(ph);
  }
  TLegend* pleg = pman->getLegend();
  pleg->Clear();
  for ( TH1* ph : hists ) {
    cesmearTH1(ph);
    string hnam = ph->GetName();
    string slab = "?: ";
    slab[0] = toupper(hnam[0]);
    getResponseLabel(ph, slab);
    cout << myname << slab << endl;
    pleg->AddEntry(ph, slab.c_str(), "l");
  }
  string sttl = pman->getTitle();
  cout << myname << sttl << endl;
  sttl[0] = tolower(sttl[0]);
  sttl = "Smeared " + sttl;
  pman->setTitle(sttl);
  //pman->setRangeX(-12, 5);
  pman->setRangeY(ymin, ymax);
  cout << myname << "Printing " << sfout << endl;
  pman->print(sfout);
}

