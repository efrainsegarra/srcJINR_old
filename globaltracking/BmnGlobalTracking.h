/**
 * \file BmnGlobalTracking.h
 * \author Sergey Merts <Sergey.Merts@gmail.com>
 * \date 2014
 * \brief BMN task for global track reconstruction.
 *
 * Output is reconstructed global tracks BmnGlobalTrack 
 * Hit-to-track merger attaches the TOF hit finally the track is refitted.
 **/

#ifndef BmnGlobalTracking_H_
#define BmnGlobalTracking_H_

#include "FairTask.h"
#include "BmnTrackFinder.h"
#include "BmnTrackFitter.h"
#include "BmnTrackPropagator.h"
#include "BmnHitToTrackMerger.h"
#include "BmnGlobalTrackingQA.h"
#include "BmnHitMatchingQA.h"
#include "TClonesArray.h"
#include "BmnGlobalTrack.h"
#include "CbmStsPoint.h"
#include "CbmTofPoint.h"
#include "BmnHit.h"
#include "CbmStripHit.h"
#include "CbmStsTrack.h"
#include "BmnGemTrack.h"
#include "CbmTofTrack.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairMCPoint.h"
#include "CbmStsHit.h"
#include "CbmMCTrack.h"
#include "TMath.h"
#include <vector>
#include <string>
#include "BmnDetectorSetup.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TGraph.h"

class TClonesArray;

using namespace std;

class BmnGlobalTracking : public FairTask {
public:
    /**
     * \brief Constructor.
     */
    BmnGlobalTracking();

    /**
     * \brief Destructor.
     */
    virtual ~BmnGlobalTracking();

    /**
     * \brief Inherited from FairTask.
     */
    virtual InitStatus Init();

    /**
     * \brief Inherited from FairTask.
     */
    virtual void Exec(Option_t* opt);

    /**
     * \brief Inherited from FairTask.
     */
    virtual void Finish();

    void SetRun1(Bool_t run) {
        isRUN1 = run;
    }

    void SetMakeQA(const Bool_t makeQA) {
        fMakeQA = makeQA;
    }

    void SetDetConf(Bool_t gem, Bool_t tof1, Bool_t dch1, Bool_t dch2, Bool_t tof2) {
        fDetConf = Short_t(gem) * 16 + Short_t(tof1) * 8 + Short_t(dch1) * 4 + Short_t(dch2) * 2 + Short_t(tof2) * 1;
    };

    const Short_t GetDetConf() const {
        return fDetConf;
    };

private:

    /*
     * \brief Calculate length of the global track
     */
    void CalculateLength();

    /**
     * \brief Run the track reconstruction
     */
    void RunTrackReconstruction();

    /**
     * \brief Select tracks for further merging with TOF.
     */
    void SelectTracksForTofMerging();

    // INPUT ARRAYS
    TClonesArray* fSeeds;
    TClonesArray* fGemTracks;
    TClonesArray* fGemHits;
    TClonesArray* fMwpc1Hits;
    TClonesArray* fMwpc2Hits;
    TClonesArray* fMwpc3Hits;
    TClonesArray* fDch1Hits;
    TClonesArray* fDch2Hits;
    TClonesArray* fTof1Hits;
    TClonesArray* fTof2Hits;

    // INPUT FOR CHECKING EFFICIENCY
    TClonesArray* fGemMcPoints;
    TClonesArray* fTof1McPoints;
    TClonesArray* fTof2McPoints;
    TClonesArray* fDch1McPoints;
    TClonesArray* fDch2McPoints;
    TClonesArray* fMcTracks;

    // OUTPUT ARRAYS

    TClonesArray* fGlobalTracks; //output BmnGlobalTrack array

    // Tools
    BmnTrackFinder* fFinder; // track finder
    BmnHitToTrackMerger* fMerger; // hit-to-track merger
    BmnTrackFitter* fFitter; // track fitter
    // track propagator
    // Used to propagate STS track to the last STS station!!!
    // Since this cannot be done in parallel mode!!!
    BmnTrackPropagator* fPropagator;
    BmnKalmanFilter* fUpdater;

    /*
     * Detector configuration, 5 bits:
     * 1 - GEM
     * 2 - TOF1
     * 3 - DCH1
     * 4 - DCH2
     * 5 - TOF2
     */
    Short_t fDetConf;
    BmnDetectorSetup fDet; // Detector presence information

    Int_t fEventNo; // event counter

    BmnGlobalTrackingQA* fGlobHisto; // pointer to object needed only for QA creating
    BmnHitMatchingQA* fTof1Histo; // pointer to object needed only for QA creating
    BmnHitMatchingQA* fTof2Histo; // pointer to object needed only for QA creating
    BmnHitMatchingQA* fDch1Histo; // pointer to object needed only for QA creating
    BmnHitMatchingQA* fDch2Histo; // pointer to object needed only for QA creating
    Bool_t fIsHistogramsInitialized; // is QA histograms initialized or not
    Bool_t fMakeQA; // create or not in output tree branch with QA histograms

    Bool_t isRUN1; // temporary flag for switching between full GEM-geometry and prototype

    Int_t fPDG; // PDG hypothesis
    Float_t fChiSqCut; // Chi square cut for hit to be attached to track.

    BmnStatus NearestHitMergeGEM(BmnGlobalTrack* tr);
    BmnStatus NearestHitMergeTOF(BmnGlobalTrack* tr, Int_t num);
    BmnStatus NearestHitMergeDCH(BmnGlobalTrack* tr, Int_t num);
    
    BmnStatus Refit(BmnGlobalTrack* tr);
    BmnStatus EfficiencyCalculation();
    
    BmnStatus Run1GlobalTrackFinder();
//    BmnStatus FillHoughHistogram(TH2F* hough, TGraph* orig, TH2F* cm, TGraph* seeds, TClonesArray* arr);
    BmnStatus FillHoughHistogram(TH1F* h, TGraph* orig, TH2F* cm, TGraph* seeds, TClonesArray* arr);

    void IdChecker(Int_t refId, Int_t hitId, TVector3 pos, BmnHitMatchingQA* hist);
    void FillIndexMap(map<Int_t, Int_t> &indexes, Int_t id);
    void FillGlobHistoQA(BmnGlobalTrack* tr, Int_t node, TVector3 pos); //filling histograms for global tracking QA
    void FillMatchHistoQA(BmnGlobalTrack* tr, Int_t nodeId, TVector3 pos, BmnHitMatchingQA* hist); //filling histograms for hits matching QA

    BmnGlobalTracking(const BmnGlobalTracking&);
    //    BmnGlobalTracking& operator=(const BmnGlobalTracking&);
    BmnStatus RefitToDetector(BmnGlobalTrack* tr, Int_t hitId, TClonesArray* hitArr, FairTrackParam* par, Int_t* nodeIdx, vector<BmnFitNode>* nodes);

    ClassDef(BmnGlobalTracking, 1);
};

#endif /* BmnGlobalTracking_H_ */
