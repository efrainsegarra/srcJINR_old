// @(#)bmnroot/mwpc:$Id$
// Author: Maria Patsyuk <mpatsyuk@mit.ru> 2018-02-10

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// BmnMwpcGeometrySRC                                                         //
//                                                                            //
// A class containing geometry params. of                                     //
// the Multi Wire Prop. Chambers of the SRC at BM@N experiment                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
#include "BmnMwpcGeometrySRC.h"

BmnMwpcGeometrySRC::BmnMwpcGeometrySRC(Int_t periodNum) :
fDebug(kFALSE) {
  if(periodNum == 7){
    fNChambers = 4;
  }else if (periodNum == 6){
    fNChambers = 2;
  }
  fNPlanes = 6;
  
  fX = new Double_t [fNChambers];
  fY = new Double_t [fNChambers];
  fZleft = new Double_t [fNChambers];
  fZright = new Double_t [fNChambers];
  fAngleX = new Double_t [fNChambers];
  fAngleY = new Double_t [fNChambers];
  /*  fZPlanePos = new Double_t*[fNChambers];
  for(Int_t i=0; i<fNChambers; i++){
    fZPlanePos[i] = new Double_t[fNPlanes];
    }*/
  
  fTimeBin = 8;
  fNWires = 96;
  fAngleStep = 60 * DegToRad();
  fWireStep = 0.25;
  fPlaneStep = 1.0015; // According to schemes given by M. Rumyantsev

  fPlaneHeight = 43.3;
  fPlaneWidth = fNWires * fWireStep;
  
  fSpaceLeft = 2.1338; // According to schemes given by M. Rumyantsev
  fSpaceRight = 2.1328; // According to schemes given by M. Rumyantsev
  
  fChamberWidth = fSpaceLeft + fSpaceLeft + (fNPlanes - 1) * fPlaneStep;
  fGlobalZdiff = -647.476; // location of SRC Z=0 relative to BM@N Z=0

  // MWPCs along the beam: 0, 1, 2, 3
  
  /*
    Reference frame is related to left side of the magnetic pole (z = 0, zC = 124.5 cm) 
    -----> Z -----> (MWPC -> GEM -> DCH)
    MWPC1                MWPC2
    
    fZleft[0] (fSpaceLeft | | | | | | fSpaceRight) fZright[0]  fZleft[1] (fSpaceLeft | | | | | | fSpaceRight) fZright[1]
                         1 2 3 4 5 6                                              1 2 3 4 5 6
    */
    if(periodNum == 6){
      fX[0] = 0.;
      fY[0] = 0.;
      fZright[0] = 0.;
      fZleft[0] = 0.;
      
      fX[1] = 0.;
      fY[1] = 0.;
      fZright[1] = 0.;
      fZleft[1] = 0.;
    }else if(periodNum == 7){
      fX[0] = -0.24;
      fY[0] = -3.342;
      fZright[0] = fGlobalZdiff - 206.042;
      fZleft[0] = fZright[0] - fChamberWidth;
      
      fX[1] = -0.109;
      fY[1] = -3.601;
      fZright[1] = fGlobalZdiff - 107.363;
      fZleft[1] = fZright[1] - fChamberWidth;
      
      fX[2] = 0.271;
      fY[2] = 6.038;
      fZright[2] = fGlobalZdiff + 287.858;
      fZleft[2] = fZright[2] - fChamberWidth;
      
      fX[3] = 0.234;
      fY[3] = 6.140;
      fZright[3] = fGlobalZdiff + 437.568;
      fZleft[3] = fZright[3] - fChamberWidth;
    }
    // location of the MWPC body in 3d.
    // Calculate angles based on the precise measurements done by Alexander Kolesnikov.
    // Define space positions of 2 points along x axis (a, b) and 2 points along y axis (c, d) for each chamber:

    //                  |c
    //                  |
    //           a _____|_____ b
    //                  |
    //                  |
    //                  |d 
    /*    if(periodNum == 7){
      TVector3 a[4], b[4], c[4], d[4];
      a[0].SetXYZ(-23.431, 6.064, 287.816);
      b[0].SetXYZ(23.961, 5.995, 287.9);
      c[0].SetXYZ(0.285, 26.583, 287.688);
      d[0].SetXYZ(0.262, -14.499, 288.028);
      
      a[1].SetXYZ(-23.485, 6.183, 437.581);
      b[1].SetXYZ(23.996, 6.078, 437.554);
      c[1].SetXYZ(0.292, 26.676, 437.655);
      d[1].SetXYZ(0.155, -14.388, 437.480);
      
      a[2].SetXYZ(-23.924, 0.00, -205.972);
      b[2].SetXYZ(23.453, 0.00, -206.061);
      c[2].SetXYZ(0.00, 17.198, -205.886);
      d[2].SetXYZ(0.00, -23.867, -206.199);
      
      a[3].SetXYZ(-23.781, 0.00, -107.469);
      b[3].SetXYZ(23.576, 0.00, -107.323);
      c[3].SetXYZ(0.00, 17.038, -107.552);
      d[3].SetXYZ(0.00, -24.220, -107.174);
    }
    // Defined using rotation constructor a la Geant3.
    // Angles theta(i), phi(i) are the polar and azimuthal angles of the (i) axis of the rotated system
    // with respect to the initial non-rotated system
    // (no rotation is composed by theta1 = 90, phi1 = 0, theta2 = 90, phi2 = 90, theta3 = 0, phi3 = 0)

    // construct oy' using points a and b, construct ox' using points c and d, and construct oz' using (a-b) x (d-c)

    for(Int_t iChamber = 0; iChamber < fNChambers; iChamber++){
      fOXprime[iChamber] = b[iChamber] - a[iChamber];
      fOYprime[iChamber] = d[iChamber] - c[iChamber];
      fOZprime[iChamber] = (a[iChamber] - b[iChamber]).Cross(d[iChamber] - c[iChamber]);
      }*/
    
    for (Int_t iChamber = 0; iChamber < fNChambers; iChamber++)
        for (Int_t iPlane = 0; iPlane < fNPlanes; iPlane++)
            fZPlanePos[iChamber][iPlane] = fZleft[iChamber] + fSpaceLeft + iPlane * fPlaneStep;


    //    cout<<"it was: fZright = "<< -(186.+22.)<<", fZright1 = "<<-(186.+22.)-fChamberWidth-140.5<<endl;
    // Check built geometry
    if (fDebug)
        for (Int_t iChamber = 0; iChamber < fNChambers; iChamber++) {
            cout << "MWPC" << iChamber + 1 << endl;
            cout << "Zleft = " << fZleft[iChamber] << " Zright = " << fZright[iChamber] << endl;
            for (Int_t iPlane = 0; iPlane < fNPlanes; iPlane++)
                cout << "zPlanePos " << iPlane + 1 << " " << fZPlanePos[iChamber][iPlane] << endl;
            cout << endl;
        }
}

TVector3 BmnMwpcGeometrySRC::GetChamberCenter(Int_t chamber) {
  return TVector3(fX[chamber], fY[chamber], (fZright[chamber] + fZleft[chamber]) * 0.5);
}

TVector3 BmnMwpcGeometrySRC::GetAxisPrime(Int_t chamber, Int_t axis){
  if(axis == 0)  return fOXprime[chamber];
  if(axis == 1)  return fOYprime[chamber];
  if(axis == 2)  return fOZprime[chamber];
}

BmnMwpcGeometrySRC::~BmnMwpcGeometrySRC() {

}
