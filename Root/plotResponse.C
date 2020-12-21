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

#include "plotResponseInternal.C"

void plotResponse(double zc, double dz, double ymin, double ymax, string fnamin ="") {
  plotResponseInternal(false, zc, dz, ymin, ymax, fnamin);
}

void plotResponse(string fnam ="garfield-1d-boundary-path-rev-dune.root") {
  const string myname = "makeResponsePlot: ";
  string sdir = gSystem->Getenv("WIRECELL_KERNEL_DATA");
  string sfil = sdir + "/" + fnam;
  plotResponse(0.0, 1.0, -0.25, 1.4, sfil);
}

