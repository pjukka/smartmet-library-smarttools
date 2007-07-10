// ======================================================================
/*!
 * \file NFmiSoundingFunctions.h
 *
 * Namespace, jossa erilaisia irto funktioita, joiden avulla lasketaan
 * erilaisia mm. luotauksiin liittvi‰ juttuja.
 * Julkisia funktioita on vain osa, cpp-filessa on paljon lis‰‰ tavaraa.
 */
// ======================================================================

#ifndef NFMISOUNDINGFUNCTIONS_H
#define NFMISOUNDINGFUNCTIONS_H

#include "NFmiGlobals.h"
#include "NFmiParameterName.h"
#include "NFmiString.h"

class NFmiSoundingData;

// Miten LCL lasketaan, pinta-arvojen vai mixed layer arvojen avulla, vai most unstable?
typedef enum
{
	kLCLCalcNone = 0,
	kLCLCalcSurface = 1,
	kLCLCalc500m = 2,
	kLCLCalc500m2 = 3, // lasketaan Tpot ja w keskiarvojen ja 1. hPa kerroksin laskien
	kLCLCalcMostUnstable = 4 // etsi maksimi theta-e arvon avulla most unstable tapaus
} FmiLCLCalcType;

namespace NFmiSoundingFunctions
{
	double CalcSHOWIndex(NFmiSoundingData &theData);
	double CalcLIFTIndex(NFmiSoundingData &theData);
	double CalcKINXIndex(NFmiSoundingData &theData);
	double CalcCTOTIndex(NFmiSoundingData &theData);
	double CalcVTOTIndex(NFmiSoundingData &theData);
	double CalcTOTLIndex(NFmiSoundingData &theData);
	double CalcLCLPressureLevel(NFmiSoundingData &theData, FmiLCLCalcType theLCLCalcType);
	double CalcLCLIndex(NFmiSoundingData &theData, FmiLCLCalcType theLCLCalcType);
	double CalcLCLHeightIndex(NFmiSoundingData &theData, FmiLCLCalcType theLCLCalcType);
	double CalcLFCIndex(NFmiSoundingData &theData, FmiLCLCalcType theLCLCalcType, double &EL);
	double CalcLFCHeightIndex(NFmiSoundingData &theData, FmiLCLCalcType theLCLCalcType, double &ELheigth);
	double CalcCAPE500Index(NFmiSoundingData &theData, FmiLCLCalcType theLCLCalcType, double theHeightLimit = kFloatMissing);
	double CalcCAPE_TT_Index(NFmiSoundingData &theData, FmiLCLCalcType theLCLCalcType, double Thigh, double Tlow);
	double CalcCINIndex(NFmiSoundingData &theData, FmiLCLCalcType theLCLCalcType);
	double CalcBulkShearIndex(NFmiSoundingData &theData, double startH, double endH);
	double CalcSRHIndex(NFmiSoundingData &theData, double startH, double endH);
	double CalcThetaEDiffIndex(NFmiSoundingData &theData, double startH, double endH);
	double CalcWSatHeightIndex(NFmiSoundingData &theData, double theH);
	double FindNearestW(double T, double P);
	double Tpot2t(double tpot, double p);
	double CalcTOfLiftedAirParcel(double T, double Td, double fromP, double toP);
	double CalcRH(double T, double Td);
	double TMR(double W, double P);
	double OS(double T, double P);
	double TSA(double OS, double P);
	double T2tpot(double T, double P);
	double CalcMoistT(double T, double P);
	double CalcThetaE(double T, double Td, double P);
	double CalcMixingRatio(double T, double Td, double P);
	double CalcDewPoint(double T, double w, double P);
	double CalcDP(double T, double RH);

	float CalcLogInterpolatedValue(float x1, float x2, float x, float y1, float y2);
	float CalcLogInterpolatedWindWectorValue(float x1, float x2, float x, float wv1, float wv2);

	bool GetValuesNeededInLCLCalculations(NFmiSoundingData &theData, FmiLCLCalcType theLCLCalcType, double &T, double &Td, double &P);

	NFmiString Get_U_V_ID_IndexText(NFmiSoundingData &theData, const NFmiString &theText, FmiDirection theStormDirection);
	void Calc_U_and_V_IDs_left(NFmiSoundingData &theData, double &u_ID, double &v_ID);
	void Calc_U_and_V_IDs_right(NFmiSoundingData &theData, double &u_ID, double &v_ID);
	void Calc_U_and_V_mean_0_6km(NFmiSoundingData &theData, double &u0_6, double &v0_6);
}

#endif // NFMISOUNDINGDATA_H
