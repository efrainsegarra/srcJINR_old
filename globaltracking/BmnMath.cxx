#include "BmnMath.h"

#include "FairTrackParam.h"
#include "CbmHit.h"
#include "BmnHit.h"
#include "CbmStripHit.h"
#include "CbmPixelHit.h"
#include "CbmGlobalTrack.h"
#include "BmnGemStripHit.h"
#include "TMath.h"
#include "TF1.h"

#include <iostream>
#include <cmath>

using namespace TMath;

namespace lit {

    Float_t ChiSq(const FairTrackParam* par, const CbmHit* hit) {
        Float_t chisq = 0.; //FIXME
        //   if (hit->GetType() == kLITSTRIPHIT) {
        //      chisq = ChiSq(par, (CbmStripHit*)hit);
        //   } else if (hit->GetType() == kLITPIXELHIT) {
        //      chisq = ChiSq(par, (CbmPixelHit*) hit);
        //   }
        return chisq;
    }

    Float_t ChiSq(//FIXME
            const FairTrackParam* par,
            const CbmStripHit* hit) {
        //   Float_t duu = hit->GetDu() * hit->GetDu();
        //   Float_t phiCos = 0;//hit->GetCosPhi();                                       //FIXME
        //   Float_t phiSin = 1;//hit->GetSinPhi();                                       //FIXME
        //   Float_t phiCosSq = phiCos * phiCos;
        //   Float_t phiSinSq = phiSin * phiSin;
        //   Float_t phi2SinCos = 2 * phiCos * phiSin;
        //   Float_t C0 = par->GetCovariance(0);
        //   Float_t C1 = par->GetCovariance(1);
        //   Float_t C5 = par->GetCovariance(5);
        //
        //   Float_t ru = hit->GetU() - par->GetX() * phiCos - par->GetY() * phiSin;
        //
        //   return (ru * ru) / (duu - phiCosSq*C0 - phi2SinCos*C1 - phiSinSq*C5);
        return 0;
    }

    Float_t ChiSq(//FIXME
            const FairTrackParam* par,
            const CbmPixelHit* hit) {
        //   Float_t dxx = hit->GetDx() * hit->GetDx();
        //   Float_t dxy = hit->GetDxy();
        //   Float_t dyy = hit->GetDy() * hit->GetDy();
        //   Float_t xmx = hit->GetX() - par->GetX();
        //   Float_t ymy = hit->GetY() - par->GetY();
        //   Float_t C0 = par->GetCovariance(0);
        //   Float_t C1 = par->GetCovariance(1);
        //   Float_t C5 = par->GetCovariance(5);
        //
        //   Float_t norm = dxx * dyy - dxx * C5 - dyy * C0 + C0 * C5
        //              - dxy * dxy + 2 * dxy * C1 - C1 * C1;
        //   if (norm == 0.) { norm = 1e-10; }
        //   return ((xmx * (dyy - C5) - ymy * (dxy - C1)) * xmx
        //           +(-xmx * (dxy - C1) + ymy * (dxx - C0)) * ymy) / norm;
        return 0;
    }

    Int_t NDF(const CbmGlobalTrack* track) {
        Int_t ndf = 0;
        //   for (Int_t i = 0; i < track->GetNofHits(); i++) {
        //      if (track->GetHit(i)->GetType() == kLITPIXELHIT) { ndf += 2; }
        //      else if (track->GetHit(i)->GetType() == kLITSTRIPHIT) { ndf++; }
        //   }
        //   ndf -= 5;
        ndf = track->GetNofHits();
        if (ndf > 0) {
            return ndf;
        } else {
            return 1;
        }
    }

}

TVector3 SpiralFit(const BmnGemTrack* tr, const TClonesArray* arr) {

    const Float_t kN = tr->GetNHits();

    Float_t sumRTheta = 0.0;
    Float_t sumTheta = 0.0;
    Float_t sumTheta2 = 0.0;
    Float_t sumR = 0.0;
    for (Int_t i = 0; i < kN; ++i) {
        BmnGemStripHit* hit = (BmnGemStripHit*) arr->At(tr->GetHitIndex(i));
        if (hit == NULL) continue;
        Float_t x = hit->GetX();
        Float_t z = hit->GetZ();
        Float_t r = Sqrt(x * x + z * z);
        Float_t Theta = ATan2(x, z);
        //        cout << Theta << " " << r << endl;
        sumTheta += Theta;
        sumR += r;
        sumTheta2 += Theta * Theta;
        sumRTheta += r * Theta;
    }
    Float_t b = (kN * sumRTheta - sumTheta * sumR) / (kN * sumTheta2 - sumTheta * sumTheta);
    Float_t a = (sumR - b * sumTheta) / kN;

    //calculate curvature at first point
    //    Float_t z0 = ((BmnGemStripHit*) arr->At(tr->GetHitIndex(0)))->GetZ();
    //    Float_t x0 = ((BmnGemStripHit*) arr->At(tr->GetHitIndex(0)))->GetX();
    //    Float_t theta0 = ATan2(x0, z0);
    //    Float_t tmp2 = (a + b * theta0) * (a + b * theta0);
    //Float_t k = (tmp2 + 2 * b * b) / Sqrt(Power((tmp2 + b * b), 3));

    Float_t k = Abs(2 / b); //curvature for r == 0
    //    cout << a << " " << b << " " << 1 / k << endl;
    return TVector3(a, b, 1 / k);
}

Bool_t IsParCorrect(const FairTrackParam* par) {
    const Float_t maxSlope = 5.;
    const Float_t minSlope = 1e-10;
    const Float_t maxQp = 1000.; // p = 10 MeV

    if (abs(par->GetTx()) > maxSlope || abs(par->GetTy()) > maxSlope || abs(par->GetTx()) < minSlope || abs(par->GetTy()) < minSlope || abs(par->GetQp()) > maxQp) return kFALSE;
    if (isnan(par->GetX()) || isnan(par->GetY()) || isnan(par->GetTx()) || isnan(par->GetTy()) || isnan(par->GetQp())) return kFALSE;

    return kTRUE;
}

Float_t ChiSq(const TVector3 par, const BmnGemTrack* tr, const TClonesArray* arr) {
    Float_t a = par.X();
    Float_t b = par.Y();
    Float_t sum = 0.0;
    for (Int_t i = 0; i < tr->GetNHits(); ++i) {
        BmnHit* hit = (BmnHit*) arr->At(tr->GetHitIndex(i));
        Float_t x = hit->GetX();
        Float_t z = hit->GetZ();
        Float_t r_hit = Sqrt(x * x + z * z);
        Float_t theta = ATan2(x, z);
        Float_t r_fit = a + b * theta;
        sum += (r_hit - r_fit) * (r_hit - r_fit) / r_hit;
    }
    return sum;
}

Int_t stationNumber(const string& detName, const Float_t z) {
    Int_t station = -1;
    if (detName == "Gem") {
        const Float_t delta = 6.;
        if (Abs(30 - z) < delta) station = 0;
        else if (Abs(45 - z) < delta) station = 1;
        else if (Abs(60 - z) < delta) station = 2;
        else if (Abs(80 - z) < delta) station = 3;
        else if (Abs(100 - z) < delta) station = 4;
        else if (Abs(130 - z) < delta) station = 5;
        else if (Abs(160 - z) < delta) station = 6;
        else if (Abs(190 - z) < delta) station = 7;
        else if (Abs(230 - z) < delta) station = 8;
        else if (Abs(270 - z) < delta) station = 9;
        else if (Abs(315 - z) < delta) station = 10;
        else if (Abs(360 - z) < delta) station = 11;
        else station = -1;
    } else if (detName == "Tof1") {
        station = 12;
    } else if (detName == "Dch1") {
        station = 13;
    } else if (detName == "Dch2") {
        station = 14;
    } else if (detName == "Tof2") {
        station = 15;
    }
    return station;
}
