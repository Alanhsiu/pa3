#ifndef PARAMPLACEMENT_H
#define PARAMPLACEMENT_H
#include <string>

#include "arghandler.h"
#include "limits"

enum SCALE_TYPE
{
SCALE_TO_LEFT,
SCALE_TO_RIGHT,
SCALE_TO_MIDLINE,
SCALE_TO_LEFT_BETWEEN_MACRO,
SCALE_TO_RIGHT_BETWEEN_MACRO,
SCALE_TO_MIDLINE_BETWEEN_MACRO
};

string TrueFalse( bool );

class CParamPlacement
{
    public:

    CParamPlacement();

    bool bRunGlobal;
    bool bRunLegal;
    bool bRunDetail;
    int threadNum;
    string auxFilename;
    string plFilename;

    int best_seed;
    int best_num_section;
    double best_HPWL;
};

extern CParamPlacement param;	// global variable

#endif
