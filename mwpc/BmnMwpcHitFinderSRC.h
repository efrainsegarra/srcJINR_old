// @(#)bmnroot/mwpc:$Id$
// Author: Pavel Batyuk (VBLHEP) <pavel.batyuk@jinr.ru> 2017-02-10

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// BmnMwpcHitFinderSRC                                                        //
//                                                                            //
// Implementation of an algorithm developed by                                // 
// Vasilisa Lenivenko                                                         //
// to the BmnRoot software                                                    //
//                                                                            //
// The algorithm serves for searching for hits                                //
// in the Multi Wire Prop. Chambers of the BM@N experiment                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef BMNMWPCHITFINDERSRC_H
#define	BMNMWPCHITFINDERSRC_H

#include <map>
#include <algorithm>
#include <Rtypes.h>
#include <TClonesArray.h>
#include <TVector3.h>
#include <TMath.h>
#include <TString.h>
#include  "FairTask.h"
#include  "BmnMwpcHit.h"
#include  "BmnMwpcDigit.h"
#include  "BmnMwpcGeometrySRC.h"
#include  "FairTask.h"
#include "TList.h"
#include "BmnTrack.h"

#include "TH1D.h"
#include "TH2D.h"

class TH1D;
class TH2D;

using namespace std;

class BmnMwpcHitFinderSRC : public FairTask {
public:
  /** Default constructor **/
  BmnMwpcHitFinderSRC() {};

  /** Constructor to be used **/
  BmnMwpcHitFinderSRC(Bool_t);

  /** Destructor **/
  virtual ~BmnMwpcHitFinderSRC();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  void SetUseDigitsInTimeBin(Bool_t flag) {
      fUseDigitsInTimeBin = flag;
  }

private:
  Bool_t expData;
  UInt_t fEventNo; // event counter

  TString fInputBranchName;
  TString fOutputBranchName;
  TString fOutputBranchName1;
  TString fOutputBranchName2;

  /** Input array of MWPC digits **/
  TClonesArray* fBmnMwpcDigitArray;

  /** Output array of MWPC hits **/
  TClonesArray* fBmnMwpcHitArray; 
  TClonesArray* fBmnMwpcSegmentsArray; 
  TClonesArray* fBmnMwpcTracksArray; 

  vector <BmnMwpcDigit*> CheckDigits(vector <BmnMwpcDigit*>);
  vector <TVector3> CreateHitsBy3Planes(vector <BmnMwpcDigit*>, vector <BmnMwpcDigit*>, vector <BmnMwpcDigit*>, Float_t);
  TVector3 CalcHitPosByTwoDigits(BmnMwpcDigit*, BmnMwpcDigit*);
  void FindNeighbour(BmnMwpcDigit*, vector <BmnMwpcDigit*>, vector<BmnMwpcDigit*>);
  void CreateMwpcHits(vector <TVector3>, TClonesArray*, Short_t);
  void DefineCoordinateAngle(Short_t, Double_t&, Double_t&);
  void FindPairs(vector <BmnMwpcDigit*>, vector <BmnMwpcDigit*>, vector <TVector3>&);
  
  Float_t thDist;      // distance between found hits [cm]
  Int_t nInputDigits;  // max. number of found digits per plane
  Int_t nTimeSamples;  // 

  Bool_t fUseDigitsInTimeBin; // use digits found in a time bin of width = kTimeBin (8 ns).

  BmnMwpcGeometrySRC* fMwpcGeometrySRC;

  TList fList,gList,tList;

  TH1D  *hNp_best_ch1, *hNp_best_ch2,  *hNbest_Ch1,  *hNbest_Ch2, *hChi2_ch1_2;

  Short_t kNChambers;
  Short_t kNPlanes;
  Short_t kNWires;
  Int_t kBig;

  Float_t *kZmid;// Float_t kZmid1;
  Float_t *ZCh;// Float_t ZCh1;
  Float_t **kZ_loc;// Float_t *kZ1_loc;
 
 Float_t **z_gl;// Float_t *z_gl1;
  Float_t **shift; //  Float_t *shift1;
  Float_t *shift1_2;

  Float_t kZ_to_pole;
  Int_t kMinHits;
  Double_t kChi2_Max;

  Float_t dw;
  Float_t dw_half;
  Double_t sq3;
  Double_t sq12;
  Double_t sigma;
  Short_t kMiddlePl;

  
  Int_t Nbest_Ch12_gl;

 
  Int_t **kPln;
  Int_t **iw;
  Int_t **iw_Ch;
  Int_t **Nhits_Ch;//  Int_t *Nhits_Ch1; 
  Int_t *Nseg_Ch; // Int_t Nseg_Ch1;
  Int_t *Nbest_Ch;//Int_t Nbest_Ch1;

  Int_t ***wire_Ch;  // Int_t **wire_Ch1;
  Float_t ***xuv_Ch;  //  Float_t **xuv_Ch1;
  Int_t ***Wires_Ch;// Int_t **Wires_Ch1;   
  Int_t ***clust_Ch;// Int_t **clust_Ch1;
  Float_t ***XVU_Ch;//  Float_t **XVU_Ch1;  

  Double_t ***par_ab_Ch;// Double_t **par_ab_Ch1;
  Float_t **XVU;// Float_t *XVU1;
  Float_t **XVU_cl;// Float_t *XVU_cl1;
 
 

  Int_t **ind_best_Ch;// Int_t *ind_best_Ch1;
  Int_t **best_Ch_gl;// Int_t *best_Ch1_gl;
  Double_t **Chi2_ndf_Ch;// Double_t *Chi2_ndf_Ch1;
  Double_t **Chi2_ndf_best_Ch;//Double_t *Chi2_ndf_best_Ch1;

  
  
  Int_t *ind_best_Ch1_2;  
  Double_t **par_ab_Ch1_2;
  
  
  Double_t *Chi2_match;
  Double_t *Chi2_ndf_Ch1_2;

  Float_t *sigm2;
  Int_t *ipl;
  Double_t **matrA;
  Double_t **matrb;
  Double_t **Amatr;
  Double_t **bmatr;
  Float_t *dX_i;
 

  Float_t *z2;

  //functions for Vasilisa method:
  void PrepareArraysToProcessEvent();
  void SegmentFinder(
   Int_t, Int_t***, Int_t***,  Float_t***, 
   Int_t**, Int_t** , 
   Int_t *, Int_t ***, Float_t ***, 
   Int_t, Short_t , Int_t);

  // void SegmentFinder(  
  //Int_t, Int_t**, Int_t**,  Float_t**,
  //Int_t*, Int_t* , 
  //Int_t &, Int_t **, Float_t **, 
  //Int_t, Short_t , Int_t);

  void ProcessSegments(
   Int_t, Double_t ,Float_t , 
   Float_t **, Int_t , Int_t * ,
   Int_t **,Int_t ***,Int_t ***,
   Float_t ***,Int_t * , Int_t **, 
   Double_t **, Double_t **, 
   Double_t ***, Int_t ,Int_t* , 
   Float_t**,  Float_t**, Double_t);

  void SegmentParamAlignment(Int_t &, Int_t *, Double_t **, Float_t *);
  void SegmentMatching(Int_t &, Int_t &, Double_t **, Double_t **, Float_t, Float_t, Int_t *, Int_t *,   Int_t *,  Int_t *, Int_t &, Double_t *);
  void SegmentFit(Float_t *, Float_t *,  Float_t*,  Int_t &, Int_t *, Int_t *, Int_t *, Int_t *,  
		  Double_t **, Double_t *, 
		  //  Int_t *, Int_t *, 
		  Int_t **,  Int_t **,
		  //  Double_t **, Double_t **,
		  Float_t **,Float_t **, 
		  // Float_t *, Float_t *, 
		  Int_t *, Int_t *, Int_t * );

  void FillFitMatrix(Int_t, Double_t **, Float_t **, Float_t *, Int_t *);

  void FillFreeCoefVectorXUV(Int_t, Double_t*, Float_t**, 
			     Float_t**, Float_t*, Int_t*);

  void FillFreeCoefVector(Int_t, Double_t*, Float_t**, Int_t, Float_t*, Float_t*, Int_t*, Int_t);

  
  void InverseMatrix(Double_t**, Double_t**);
  vector<vector<double>> vkFillFitMatrix(Float_t*, Float_t*, Int_t*);

  ClassDef(BmnMwpcHitFinderSRC, 1);
};

#endif	

