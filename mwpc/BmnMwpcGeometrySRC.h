// @(#)bmnroot/mwpc:$Id$
// Author: Maria Patsyuk <mpatsyuk@mit.edu> 2018-02-10

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// BmnMwpcGeometrySRC                                                         //
//                                                                            //
// A class containing geometry params. of                                     //
// the Multi Wire Prop. Chambers of the SRC at BM@N experiment                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef BMNMWPCGEOMETRYSRC_H
#define BMNMWPCGEOMETRYSRC_H

#include <iostream>
#include <TNamed.h>
#include <TMath.h>
#include <TVector3.h>

using namespace std;
using namespace TMath;

class BmnMwpcGeometrySRC : public TNamed {
public:

    /** Default constructor **/
    BmnMwpcGeometrySRC();

    /** Destructor **/
    virtual ~BmnMwpcGeometrySRC();
    
    Short_t GetNChambers() {
        return fNChambers;
    }

    Short_t GetNPlanes() {
        return fNPlanes;
    }
    
    Short_t GetTimeBin() {
        return fTimeBin;
    }
    
    Short_t GetNWires() {
        return fNWires;
    }
    
    Double_t GetAngleStep() {
        return fAngleStep;
    }
    
    Double_t GetWireStep() {
        return fWireStep;
    }
    
    Double_t GetPlaneStep() {
        return fPlaneStep;
    }
    
    Double_t GetPlaneHeight() {
        return fPlaneHeight;
    }
    
    Double_t GetPlaneWidth() {
        return fPlaneWidth;
    }
    
    Double_t GetZPlanePos(Int_t chamber, Int_t plane) {
        return zPlanePos[chamber][plane];
    }
    
    TVector3 GetChamberCenter(Int_t chamber);
    TVector3 GetAxisPrime(Int_t chamber, Int_t axis);

    Double_t GetAngleX(Int_t chamber){
      return fAngleX[chamber];
    }

    Double_t GetAngleY(Int_t chamber){
      return fAngleY[chamber];
    }
    
    Double_t GetZRight(Int_t chamber) {
        return zRight[chamber];
    }
    
    Double_t GetZLeft(Int_t chamber) {
        return zLeft[chamber];
    }

    Double_t GetXcenter(Int_t chamber){
      return fX[chamber];
    }

    Double_t GetYcenter(Int_t chamber){
      return fY[chamber];
    }
 
private:

    Short_t fNChambers;
    Short_t fNPlanes;
    Short_t fTimeBin;  // in ns
    Short_t fNWires;   //in one plane
    Double_t fAngleStep;
    Double_t fWireStep; // in cm
    Double_t fPlaneStep; // in cm
    Double_t fPlaneHeight; // in cm
    Double_t fPlaneWidth; //in cm
    Double_t fSpaceLeft; //in cm
    Double_t fSpaceRight; //in cm
 
    Double_t fAngleX[4];
    Double_t fAngleY[4];
   
    Double_t zPlanePos[4][6];
    Double_t zRight[4];
    Double_t zLeft[4];
    Double_t fX[4];
    Double_t fY[4];
    
    Double_t fChamberWidth;
    //    Double_t fDistanceBetweenMWPC;
    Double_t fGlobalZdiff;
    
    Bool_t fDebug;

    TVector3 fOXprime[4];
    TVector3 fOYprime[4];
    TVector3 fOZprime[4];
    
    ClassDef(BmnMwpcGeometrySRC, 1);
};

#endif
