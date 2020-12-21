#include "getWirecellFile.C"

void draw2dResponse(string sview ="x") {
  string myname = "draw2d: ";
  string fnam = getWirecellFile();
  cout << myname << "Hist file: " << fnam << endl;
  TFile* pfil = TFile::Open(fnam.c_str());
  pfil->ls();
  string svh;
  float xmax = 2.0;
  float ymin = -6.0;
  float ymax =  6.0;
  float zmax = 0.0004;
  if ( sview == "x" ) {
    svh = "W";
    //zmax = 0.002;
  } else if ( sview == "u" ) {
    svh = "U";
  } else if ( sview == "v" ) {
    svh = "V";
  } else {
    cout << myname << "Invalid view: " << sview << endl;
    return;
  }
  string hnam = "FieldRes_" + svh;
  TH2* ph = dynamic_cast<TH2*>(pfil->Get(hnam.c_str()));
  if ( ph == nullptr ) {
    cout << myname << "Unable to find histogram " << hnam << endl;
    return;
  }
  ph->Print();
  ph->SetContour(200);
  TPadManipulator man(1400, 1000);
  man.add(ph, "colz");
  man.setRangeX(-xmax, xmax);
  man.setRangeY(ymin, ymax);
  man.setRangeZ(-zmax, zmax);
  man.setPalette(2021);
  man.addVerticalModLines(1.0, -0.05, 1, 1);
  man.addVerticalModLines(1.0, 0.45);
  man.addAxis();
  string fout = "resp2d" + sview + ".png";
  cout << myname << "Plot is at " << fout << endl;
  man.print(fout);
}
