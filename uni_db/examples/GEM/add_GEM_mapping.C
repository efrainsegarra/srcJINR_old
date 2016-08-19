#include "../../db_structures.h"

int AssignMapStructure(GemMapStructure* pArray, int id, unsigned int ser, int ch_l, int ch_h, int gemId, int st, int hz) {
    pArray[id].serial = ser;
    pArray[id].id = gemId;
    pArray[id].station = st;
    pArray[id].hotZone = hz;
    pArray[id].channel_low = ch_l;
    pArray[id].channel_high = ch_h;
    return 0;
}

// macro for adding parameter value (if parameter exist - you could check existing parameters by 'UniDbParameter::PrintAll()' function)

void add_GEM_mapping() {
    gROOT->LoadMacro("$VMCWORKDIR/gconfig/basiclibs.C");
    basiclibs();
    gSystem->Load("libUniDb");

    bool return_error = false;

    const int kNitems = 17;

    GemMapStructure* pValues = new GemMapStructure[kNitems];

    AssignMapStructure(pValues, 0, 0x76CBA8B, 0, 2047, 2, 2, 0);
    AssignMapStructure(pValues, 1, 0x76CD410, 0, 2047, 4, 4, 0);
    AssignMapStructure(pValues, 2, 0x76C8320, 0, 2047, 3, 3, 0);
    AssignMapStructure(pValues, 3, 0x76CB9C0, 0, 2047, 1, 1, 0);
    AssignMapStructure(pValues, 4, 0x76CA266, 0, 2047, 5, 5, 0);
    AssignMapStructure(pValues, 5, 0x76D08B9, 0, 127, 2, 2, 0);
    AssignMapStructure(pValues, 6, 0x76D08B9, 128, 255, 4, 4, 0);
    AssignMapStructure(pValues, 7, 0x76D08B9, 256, 383, 3, 3, 0);
    AssignMapStructure(pValues, 8, 0x76D08B9, 384, 511, 1, 1, 0);
    AssignMapStructure(pValues, 9, 0x76D08B9, 896, 1023, 5, 5, 0);
    AssignMapStructure(pValues, 10, 0x76D08B9, 1024, 1536, 0, 0, 0);
    AssignMapStructure(pValues, 11, 0x76D08B9, 640, 767, 6, 6, 0);
    AssignMapStructure(pValues, 12, 0x76D08B9, 768, 895, 7, 6, 0);
    AssignMapStructure(pValues, 13, 0x76C8321, 0, 1023, 6, 6, 1);
    AssignMapStructure(pValues, 14, 0x76CE3EE, 0, 2047, 6, 6, 0);
    AssignMapStructure(pValues, 15, 0x76CE3E5, 0, 2047, 7, 6, 0);
    AssignMapStructure(pValues, 16, 0x4E983C1, 0, 1023, 7, 6, 1);

    UniDbDetectorParameter* pDetectorParameter = UniDbDetectorParameter::CreateDetectorParameter("GEM", "GEM_global_mapping", 4, 61, 4, 84, pValues, kNitems);
    if (pDetectorParameter == NULL)
        return_error = true;

    // clean memory after work
    delete [] pValues;
    if (pDetectorParameter)
        delete pDetectorParameter;

    if (return_error)
        cout << "\nMacro finished with errors" << endl;
    else
        cout << "\nMacro finished successfully" << endl;
}