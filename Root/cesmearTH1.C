#include "dune/DuneCommon/coldelecResponse.h"

void smearVector(const vector<double>& data, const vector<double>& smear, unsigned int ioff, vector<double>& res) {
  using Index = unsigned int;
  Index ndat = data.size();
  Index nsme = smear.size();
  res.resize(ndat, 0.0);
  for ( Index ires=0; ires<ndat; ++ires ) {
    double resval = 0.0;
    Index idat = ioff + ires > nsme ? ires + ioff - nsme : 0;
    while ( idat < ndat ) {
      if ( idat > ioff + ires ) break;
      Index isme = ioff + ires - idat;
      resval += data[idat]*smear[isme];
      //cout << "XXX " << ires << "=" << idat << "+" << isme << ": " << resval << endl;
      ++idat;
    }
    res[ires] = resval;
  }
}
  
// Smear a vector with one entry per dx ticks.
void cesmearVector(vector<double>& data, double dx =1) {
  using Index = unsigned int;
  Index ndat = data.size();
  double shaping = 2.2;
  double gain = 1.0;
  vector<double> smear;
  double sum = 0.0;
  for ( Index isme=0; isme<1000; ++isme ) {
    double xsme = isme*dx;
    double val = coldelecResponse(xsme, gain, shaping);
    sum += val;
    smear.push_back(val);
    if ( val == 0.0 && xsme > 2.0*shaping ) break;
  }
  Index nsme = smear.size();
  double suminv = 1.0/sum;
  for ( Index isme=0; isme<nsme; ++isme ) smear[isme] *= suminv;
  Index ioff = 1.3*shaping/dx;
  vector<double> res;
  smearVector(data, smear, ioff, res);
  data = res;
}

// Smear a histogram.
void cesmearTH1(TH1* ph) {
  using Index = unsigned int;
  Index ndat = ph->GetNbinsX();
  vector<double> data(ndat);
  for ( Index idat=0; idat<ndat; ++idat ) {
    data[idat] = ph->GetBinContent(idat+1);
  }
  double dx = ph->GetBinWidth(1);
  cesmearVector(data, dx);
  for ( Index idat=0; idat<ndat; ++idat ) {
    ph->SetBinContent(idat+1, data[idat]);
  }
}

// Test function.
// Smear delta function and check we get CE shaping.
void cesmearTH1() {
  using Index = unsigned int;
  cout << "Testing smearTH1" << endl;
  TH1* ph = new TH1F("hce", "CE smearing", 1000, -20, 10);
  Index ndat = ph->GetNbinsX();
  ph->Fill(0);
  //ph->Fill(-30, -1.0);
  ph->Print();
  cesmearTH1(ph);
  for ( Index idat=0; idat<ndat; ++idat ) {
    cout << "  " << idat << ": " << ph->GetBinContent(idat+1) << endl;
  }
  ph->Print();
  cout << "Area: " << ph->Integral() << endl;
  ph->Draw();
}


