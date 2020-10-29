TH2* jsonToHist(string sview, string base) {
  const string myname = "jsonToHist: ";
  using Index = unsigned int;
  float normfac = 1.0;
  normfac = 0.1/(-0.0993845);
  string sifil = base;
  ifstream fin(sifil);
  if ( ! fin ) {
    cout << "Unable to open " << sifil << endl;
    return nullptr;
  }
  int nlin = 0;
  int nele = 0;
  int ioff = 105;
  vector<int> nfill(210, 0);   // # histo fills for each z bin
  vector<float> vmins(210, 0);
  vector<float> vmaxs(210, 0);
  string line;
  int ipla = -1;
  string hnam;
  string httl;
  int mypla = -2;
  if ( sview == "x" ) {
    hnam = "FieldRes_W";
    mypla = 2;
  } else if ( sview == "u" ) {
    hnam = "FieldRes_U";
    mypla = 0;
  } else if ( sview == "v" ) {
    hnam = "FieldRes_V";
    mypla = 1;
  } else {
    cout << "ERROR: Invalid view " << sview << endl;
    return nullptr;
  }
  TH2* ph = new TH2F(hnam.c_str(), httl.c_str(), 210, -105, 105, 1000, 0, 100);
  while ( ! fin.eof() ) {
    getline(fin, line);
    if ( fin.eof() ) cout << line << endl;
    if ( line.find("PlaneResponse") != string::npos ) {
      ++ipla;
    }
    if ( ipla != mypla ) continue;
    if ( line.find("PathResponse") != string::npos ) {
      if ( ipla < 0 || ipla > 2 ) {
        cout << myname << "ERROR: Invalid plane: " << ipla << endl;
        return nullptr;
      }
      ++nele;
      string::size_type ipos = string::npos;
      vector<float> vals;
      while ( true ) {
        getline(fin, line);
        ipos = line.find("pitchpos");
        float z = -9999;
        if ( ipos != string::npos ) {
          ipos = line.find(":", ipos);
          istringstream ssz(line.substr(ipos+1));
          ssz >> z;
          int izoff = ioff + z/0.471 + 0.1;
          int iz = izoff - ioff;
          if ( izoff < 0 || izoff >= nfill.size() ) {
            cout << myname << "ERROR: Invalid izoff: " << izoff << endl;
          } else {
            cout << myname << "izoff: " << izoff << endl;
            vector<int> izfills(1, izoff);
            // Add mirror zbin where needed.
            Index izrem = izoff%10;
            if ( izrem >=1 && izrem <= 4 ) {
              int jz = -iz;
              int jzoff = ioff + jz;
              izfills.push_back(jzoff);
            }
            // Fill histogram.
            if ( vals.size() != 1000 ) {
              cout << myname << "Unexpected value count: " << vals.size() << endl;
              return nullptr;
            }
            for ( Index izfill : izfills ) {
              if ( nfill[izfill] ) {
                cout << myname << "ERROR: Duplicate fill of z bin " << izfill << endl;
                return nullptr;
              }
              ++nfill[izfill];
              for ( Index itim=0; itim<1000; ++itim ) {
                int bin = ph->GetBin(izfill+1, itim+1);
                float val = normfac*vals[itim];
                ph->SetBinContent(bin, val);
              }
            }
          }
          vals.clear();
          break;
        }
        ipos = line.find("elements");
        double val;
        double valmin = 999;
        double valmax = -999;
        Index itimmin = 0;
        Index itimmax = 0;
        if ( ipos != string::npos ) {
          Index itim = 0;
          while ( ! fin.eof() ) {
            getline(fin, line);
            ipos = line.find("]");
            if ( ipos != string::npos ) break;
            istringstream ssval(line);
            ssval >> val;
            if ( val > valmax ) { valmax = val; itimmax = itim; }
            if ( val < valmin ) { valmin = val; itimmin = itim; }
            vals.push_back(val);
            ++itim;
          }
          if ( itim != 1000 ) {
            cout << myname << "ERROR: Invalid # time samples: " << itim << endl;
            return nullptr;
          }
        }
        if ( fin.eof() ) {
          cout << myname << "ERROR: Premature end of file." << endl;
          break;
        }
      }
    }
    ++nlin;
  }
  Index nerr = 0;
  for ( unsigned int ifil=0; ifil<nfill.size(); ++ifil ) {
    Index nzfill = nfill[ifil];
    if ( nzfill != 1 ) {
      int iz = int(ifil) - ioff;
      cout << myname << "ERROR: " << setw(5) << ifil << " --> " << setw(3) << iz << " has fill count " << nzfill << endl;
    }
  }
  if ( nerr ) return nullptr;
  cout << myname << "Element count: " << nele << endl;
  cout << myname << "Line count: " << nlin << endl;
  cout << myname << "Plane count: " << ipla + 1  << endl;
  ph->Print();
  return ph;
}
