string getWirecellFile();

// Return wirecell fill from data area.
string getWirecellFile(string fnam) {
  if ( fnam.size() == 0 ) return getWirecellFile();
  string sdir = gSystem->Getenv("WIRECELL_KERNEL_DATA");
  return sdir + "/" + fnam;
}

// Return the default wirecell response histogram file.
string getWirecellFile() {
  //string fnam = "garfield-1d-boundary-path-rev-dune.root";
  string fnam = "dune-garfield-1d565.root";     // June 2020
  return getWirecellFile(fnam);
}

