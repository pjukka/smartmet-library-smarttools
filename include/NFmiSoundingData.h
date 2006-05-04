// ======================================================================
/*!
 * \file NFmiSoundingData.h
 *
 * Apuluokka laskemaan ja tutkimaan luotaus dataa. Osaa t�ytt�� itsens�
 * mm. mallipinta QueryDatasta (infosta).
 */
// ======================================================================

#ifndef NFMISOUNDINGDATA_H
#define NFMISOUNDINGDATA_H

#include "NFmiMetTime.h"
#include "NFmiLocation.h"
#include "NFmiParameterName.h"
#include "NFmiDataMatrix.h"

class NFmiFastQueryInfo;
class NFmiQueryInfo;

class NFmiSoundingData
{
public:
	NFmiSoundingData(void){};

	bool FillSoundingData(NFmiQueryInfo* theInfo, const NFmiMetTime& theTime, const NFmiMetTime& theOriginTime, const NFmiLocation& theLocation);
	bool FillSoundingData(NFmiQueryInfo* theInfo, const NFmiMetTime& theTime, const NFmiMetTime& theOriginTime, const NFmiPoint& theLatlon, const NFmiString &theName);

	// FillSoundingData-metodeilla t�ytet��n kunkin parametrin vektorit ja t�ll� saa haluamansa parametrin vektorin k�ytt��n
	checkedVector<float>& GetParamData(FmiParameterName theId);
	const NFmiLocation& Location(void) const {return itsLocation;}
	const NFmiMetTime& Time(void) const {return itsTime;}
	const NFmiMetTime& OriginTime(void) const {return itsOriginTime;}
	bool GetValuesStartingLookingFromPressureLevel(double &T, double &Td, double &P);
	float GetValueAtPressure(FmiParameterName theId, float P);
	float GetValueAtHeight(FmiParameterName theId, float H);
	bool CalcLCLAvgValues(double fromZ, double toZ, double &T, double &Td, double &P, bool fUsePotTandMix);
	bool CalcAvgWindComponentValues(double fromZ, double toZ, double &u, double &v);
	bool ObservationData(void) const {return fObservationData;}
	bool GetLowestNonMissingValues(float &H, float &U, float &V);
	float ZeroHeight(void) const {return itsZeroHeight;}
	int ZeroHeightIndex(void) const {return itsZeroHeightIndex;}
	bool IsSameSounding(const NFmiSoundingData &theOtherSounding);
	bool GetTandTdValuesFromNearestPressureLevel(double P, double &theFoundP, double &theT, double &theTd);
	bool SetValueToPressureLevel(float P, float theParamValue, FmiParameterName theId);
	bool FindHighestThetaE(double &T, double &Td, double &P, double &theMaxThetaE, double theMinP);
	float FindPressureWhereHighestValue(FmiParameterName theId, float theMaxP, float theMinP);
private:
	float GetPressureAtHeight(double H);
	void ClearDatas(void);
	bool FillParamData(NFmiQueryInfo* theInfo, FmiParameterName theId);
	bool FillParamData(NFmiQueryInfo* theInfo, FmiParameterName theId, const NFmiMetTime& theTime, const NFmiPoint& theLatlon);
	void InitZeroHeight(void); // t�t� kutsutaan FillParamData-metodeista

	NFmiLocation itsLocation;
	NFmiMetTime	itsTime;
	NFmiMetTime	itsOriginTime; // t�m� otetaan talteen IsSameSounding-metodia varten

	checkedVector<float> itsTemperatureData;
	checkedVector<float> itsDewPointData;
	checkedVector<float> itsPressureData;
	checkedVector<float> itsGeomHeightData; // t�m� on korkeus dataa metreiss�
	checkedVector<float> itsWindSpeedData;
	checkedVector<float> itsWindDirectionData;
	checkedVector<float> itsWindComponentUData;
	checkedVector<float> itsWindComponentVData;

	float itsZeroHeight; // t�lt� korkeudelta alkaa luotauksen 0-korkeus, eli vuoristossa luotaus alkaa oikeasti korkeammalta ja se korkeus pit�� k�sitell� pintakorkeutena
	int itsZeroHeightIndex; // edellisen indeksi (paikka vektorissa). Arvo on -1 jos ei l�ytynyt kunnollista 0-korkeutta
	bool fObservationData;
};

#endif // NFMISOUNDINGDATA_H