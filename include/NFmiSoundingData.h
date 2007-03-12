// ======================================================================
/*!
 * \file NFmiSoundingData.h
 *
 * Apuluokka laskemaan ja tutkimaan luotaus dataa. Osaa täyttää itsensä
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

	// TODO Fill-metodeille pitää laittaa haluttu parametri-lista parametriksi (jolla täytetään sitten dynaamisesti NFmiDataMatrix-otus)
	bool FillSoundingData(NFmiQueryInfo* theInfo, const NFmiMetTime& theTime, const NFmiMetTime& theOriginTime, const NFmiLocation& theLocation, int useStationIdOnly = false);
	bool FillSoundingData(NFmiQueryInfo* theInfo, const NFmiMetTime& theTime, const NFmiMetTime& theOriginTime, const NFmiPoint& theLatlon, const NFmiString &theName);
	void CutEmptyData(void); // tämä leikkaa Fill.. -metodeissa laskettuja data vektoreita niin että pelkät puuttuvat kerrokset otetaan pois

	// FillSoundingData-metodeilla täytetään kunkin parametrin vektorit ja tällä saa haluamansa parametrin vektorin käyttöön
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
	bool ModifyT2DryAdiapaticBelowGivenP(double P, double T);
	bool ModifyTd2MoistAdiapaticBelowGivenP(double P, double Td);
	bool ModifyTd2MixingRatioBelowGivenP(double P, double T, double Td);
	bool Add2ParamAtNearestP(float P, FmiParameterName parId, float addValue, float minValue, float maxValue, bool fCircularValue);
	void UpdateUandVParams(void);
private:
	unsigned int GetHighestNonMissingValueLevelIndex(FmiParameterName theParaId);
	float GetPressureAtHeight(double H);
	void ClearDatas(void);
	bool FillParamData(NFmiQueryInfo* theInfo, FmiParameterName theId);
	bool FillParamData(NFmiQueryInfo* theInfo, FmiParameterName theId, const NFmiMetTime& theTime, const NFmiPoint& theLatlon);
	void InitZeroHeight(void); // tätä kutsutaan FillParamData-metodeista

	NFmiLocation itsLocation;
	NFmiMetTime	itsTime;
	NFmiMetTime	itsOriginTime; // tämä otetaan talteen IsSameSounding-metodia varten

	// TODO Laita käyttämään NFmiDataMatrix-luokkaa dynaamista datalistaa varten. Laita myös
	// param-lista (joka annetaan fillData-metodeissa) data osaksi
	checkedVector<float> itsTemperatureData;
	checkedVector<float> itsDewPointData;
	checkedVector<float> itsPressureData;
	checkedVector<float> itsGeomHeightData; // tämä on korkeus dataa metreissä
	checkedVector<float> itsWindSpeedData;
	checkedVector<float> itsWindDirectionData;
	checkedVector<float> itsWindComponentUData;
	checkedVector<float> itsWindComponentVData;

	float itsZeroHeight; // tältä korkeudelta alkaa luotauksen 0-korkeus, eli vuoristossa luotaus alkaa oikeasti korkeammalta ja se korkeus pitää käsitellä pintakorkeutena
	int itsZeroHeightIndex; // edellisen indeksi (paikka vektorissa). Arvo on -1 jos ei löytynyt kunnollista 0-korkeutta
	bool fObservationData;
};

#endif // NFMISOUNDINGDATA_H
