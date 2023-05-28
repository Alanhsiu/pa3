#define _GLIBCXX_USE_CXX11_ABI 0
#ifndef GLOBALPLACER_H
#define GLOBALPLACER_H

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <limits>
#include <random>
#include <set>
#include <vector>
#include "ParamPlacement.h"
#include "Placement.h"

class GlobalPlacer {
   public:
    GlobalPlacer(Placement& placement);
    void place();
    void plotPlacementResult(const string outfilename, bool isPrompt = false);

   private:
    Placement& _placement;
    void plotBoxPLT(ofstream& stream, double x1, double y1, double x2, double y2);
};

#endif  // GLOBALPLACER_H
