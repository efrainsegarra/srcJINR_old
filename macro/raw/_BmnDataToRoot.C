#include "/nica/user/s/segarrae/software/bmnroot/macro/run/bmnloadlibs.C"
#include "/nica/user/s/segarrae/software/bmnroot/bmndata/BmnEnums.h"

//file: full path to raw-file
//nEvents: if 0 then decode all events
//doConvert: convert RAW --> ROOT before decoding or use file converted before

void BmnDataToRoot(TString file="/nica/mpd16/basalaev/src_raw_data2196-2311/mpd_run_trigCode_FILENAME.data", Long_t nEvents, Bool_t doConvert = kTRUE)
{
#if ROOT_VERSION_CODE < ROOT_VERSION(5,99,99)
    gROOT->LoadMacro("$VMCWORKDIR/macro/run/bmnloadlibs.C");
#endif
    bmnloadlibs(); // load BmnRoot libraries
    UInt_t period = 7;
    BmnSetup stp = kSRCSETUP; // use kSRCSETUP for Short-Range Correlation program and kBMNSETUP otherwise
    BmnRawDataDecoder* decoder = new BmnRawDataDecoder(file, nEvents, period);
    decoder->SetBmnSetup(stp);

    Bool_t setup[10]; //array of flags to determine BM@N setup
    //Just put "0" to exclude detector from decoding
    setup[0] = 1; // TRIGGERS
    setup[1] = 1; // MWPC
    setup[2] = 0; // SILICON
    setup[3] = 1; // GEM
    setup[4] = 1; // TOF-400
    setup[5] = 0; // TOF-700
    setup[6] = 0; // DCH
    setup[7] = 0; // ZDC
    setup[8] = 0; // ECAL
    setup[9] = 1; // LAND
    decoder->SetDetectorSetup(setup);

    TString PeriodSetupExt = Form("%d%s.txt", period, ((stp == kBMNSETUP) ? "" : "_SRC"));
    decoder->SetTrigMapping(TString("Trig_map_Run") + PeriodSetupExt);
    decoder->SetTrigINLFile("TRIG_INL.txt");
    decoder->SetSiliconMapping("SILICON_map_run7.txt");
    decoder->SetGemMapping(TString("GEM_map_run") + PeriodSetupExt);
    // in case comment out the line decoder->SetTof400Mapping("...")  
    // the maps of TOF400 will be read from DB (only for JINR network)
    decoder->SetTof400Mapping("TOF400_PlaceMap_RUN7.txt", "TOF400_StripMap_RUN7.txt");
    decoder->SetTof700Mapping("TOF700_map_period_7.txt");
    decoder->SetZDCMapping("ZDC_map_period_5.txt");
    decoder->SetZDCCalibration("zdc_muon_calibration.txt");
    decoder->SetECALMapping("ECAL_map_period_5.txt");
    decoder->SetECALCalibration("");
    decoder->SetMwpcMapping(TString("MWPC_map_period") + PeriodSetupExt);
    decoder->SetLANDMapping("land_mapping_jinr_triplex.txt");
    decoder->SetLANDPedestal("r0030_land_clock.hh");
    decoder->SetLANDTCal("r0030_land_tcal.hh");
    decoder->SetLANDDiffSync("r352_cosmic1.hh");
    decoder->SetLANDVScint("neuland_sync_2.txt");
    decoder->InitMaps();
    if (doConvert) decoder->ConvertRawToRoot(); // Convert raw data in .data format into adc-,tdc-, ..., sync-digits in .root format
    decoder->DecodeDataToDigi(); // Decode data into detector-digits using current mappings.

    delete decoder;
}
