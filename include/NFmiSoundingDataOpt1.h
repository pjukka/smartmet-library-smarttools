// ======================================================================
/*!
 * \file NFmiSoundingDataOpt1.h
 *
 * Apuluokka laskemaan ja tutkimaan luotaus dataa. Osaa täyttää itsensä
 * mm. mallipinta QueryDatasta (infosta).
 */
// ======================================================================

#ifndef NFMISOUNDINGDATAOPT1_H
#define NFMISOUNDINGDATAOPT1_H

#include "NFmiMetTime.h"
#include "NFmiLocation.h"
#include "NFmiParameterName.h"
#include "NFmiDataMatrix.h"
#include "NFmiDataMatrix.h"
#include "NFmiSoundingData.h"

class NFmiFastQueryInfo;

class NFmiSoundingDataOpt1
{
public:
	// Miten LCL lasketaan, pinta-arvojen vai mixed layer arvojen avulla, vai most unstable?
/*
	typedef enum
	{
		kLCLCalcNone = 0,
		kLCLCalcSurface = 1,
		kLCLCalc500m = 2,
		kLCLCalc500m2 = 3, // lasketaan Tpot ja w keskiarvojen ja 1. hPa kerroksin laskien
		kLCLCalcMostUnstable = 4 // etsi maksimi theta-e arvon avulla most unstable tapaus
	} FmiLCLCalcType;
*/
	class LFCIndexCache
	{
	public:
		LFCIndexCache(void)
		:itsSurfaceValue(kFloatMissing)
		,itsSurfaceELValue(kFloatMissing)
		,its500mValue(kFloatMissing)
		,its500mELValue(kFloatMissing)
		,its500m2Value(kFloatMissing)
		,its500m2ELValue(kFloatMissing)
		,itsMostUnstableValue(kFloatMissing)
		,itsMostUnstableELValue(kFloatMissing)
		,fSurfaceValueInitialized(false)
		,f500mValueInitialized(false)
		,f500m2ValueInitialized(false)
		,fMostUnstableValueInitialized(false)
		{
		}

		void Clear(void)
		{
			*this = LFCIndexCache();
		}

		double itsSurfaceValue;
		double itsSurfaceELValue;
		double its500mValue;
		double its500mELValue;
		double its500m2Value;
		double its500m2ELValue;
		double itsMostUnstableValue;
		double itsMostUnstableELValue;
		bool fSurfaceValueInitialized;
		bool f500mValueInitialized;
		bool f500m2ValueInitialized;
		bool fMostUnstableValueInitialized;
	};

	NFmiSoundingDataOpt1(void){};

	// TODO Fill-metodeille pitää laittaa haluttu parametri-lista parametriksi (jolla täytetään sitten dynaamisesti NFmiDataMatrix-otus)
	bool FillSoundingData(NFmiFastQueryInfo* theInfo, const NFmiMetTime& theTime, const NFmiMetTime& theOriginTime, const NFmiLocation& theLocation, int useStationIdOnly = false);
	bool FillSoundingData(NFmiFastQueryInfo* theInfo, const NFmiMetTime& theTime, const NFmiMetTime& theOriginTime, const NFmiPoint& theLatlon, const NFmiString &theName, NFmiFastQueryInfo* theGroundDataInfo, bool useFastFill = false);
	void CutEmptyData(void); // tämä leikkaa Fill.. -metodeissa laskettuja data vektoreita niin että pelkät puuttuvat kerrokset otetaan pois
	static bool HasRealSoundingData(NFmiFastQueryInfo &theSoundingLevelInfo);

	// FillSoundingData-metodeilla täytetään kunkin parametrin vektorit ja tällä saa haluamansa parametrin vektorin käyttöön
	checkedVector<float>& GetParamData(FmiParameterName theId);
	const NFmiLocation& Location(void) const {return itsLocation;}
	void Location(const NFmiLocation &newValue) {itsLocation = newValue;}
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
	bool IsSameSounding(const NFmiSoundingDataOpt1 &theOtherSounding);
	bool GetTandTdValuesFromNearestPressureLevel(double P, double &theFoundP, double &theT, double &theTd);
	bool SetValueToPressureLevel(float P, float theParamValue, FmiParameterName theId);
	bool FindHighestThetaE(double &T, double &Td, double &P, double &theMaxThetaE, double theMinP);
	float FindPressureWhereHighestValue(FmiParameterName theId, float theMaxP, float theMinP);
	bool ModifyT2DryAdiapaticBelowGivenP(double P, double T);
	bool ModifyTd2MoistAdiapaticBelowGivenP(double P, double Td);
	bool ModifyTd2MixingRatioBelowGivenP(double P, double T, double Td);
	bool Add2ParamAtNearestP(float P, FmiParameterName parId, float addValue, float minValue, float maxValue, bool fCircularValue);
	void UpdateUandVParams(void);
	bool PressureDataAvailable(void) const {return fPressureDataAvailable;}
	bool HeightDataAvailable(void) const {return fHeightDataAvailable;}
	void SetTandTdSurfaceValues(float T, float Td);

	double CalcSHOWIndex(void);
	double CalcLIFTIndex(void);
	double CalcKINXIndex(void);
	double CalcCTOTIndex(void);
	double CalcVTOTIndex(void);
	double CalcTOTLIndex(void);
	double CalcLCLPressureLevel(FmiLCLCalcType theLCLCalcType);
	double CalcLCLIndex(FmiLCLCalcType theLCLCalcType);
	double CalcLCLHeightIndex(FmiLCLCalcType theLCLCalcType);
	double CalcLFCIndex(FmiLCLCalcType theLCLCalcType, double &EL);
	double CalcLFCHeightIndex(FmiLCLCalcType theLCLCalcType, double &ELheigth);
	double CalcCAPE500Index(FmiLCLCalcType theLCLCalcType, double theHeightLimit = kFloatMissing);
	double CalcCAPE_TT_Index(FmiLCLCalcType theLCLCalcType, double Thigh, double Tlow);
	double CalcCINIndex(FmiLCLCalcType theLCLCalcType);
	double CalcBulkShearIndex(double startH, double endH);
	double CalcSRHIndex(double startH, double endH);
	double CalcThetaEDiffIndex(double startH, double endH);
	double CalcWSatHeightIndex(double theH);
	bool GetValuesNeededInLCLCalculations(FmiLCLCalcType theLCLCalcType, double &T, double &Td, double &P);
	NFmiString Get_U_V_ID_IndexText(const NFmiString &theText, FmiDirection theStormDirection);
	void Calc_U_and_V_IDs_left(double &u_ID, double &v_ID);
	void Calc_U_and_V_IDs_right(double &u_ID, double &v_ID);
	void Calc_U_and_V_mean_0_6km(double &u0_6, double &v0_6);
	double CalcWindBulkShearComponent(double startH, double endH, FmiParameterName theParId);
	double CalcBulkShearIndex(double startH, double endH, FmiParameterName theParId);
	void Calc_U_V_helpers(double &shr_0_6_u_n, double &shr_0_6_v_n, double &u0_6, double &v0_6);
	double CalcTOfLiftedAirParcel(double T, double Td, double fromP, double toP);

private:
	bool CheckLFCIndexCache(FmiLCLCalcType theLCLCalcTypeIn, double &theLfcIndexValueOut, double &theELValueOut);
	void FillLFCIndexCache(FmiLCLCalcType theLCLCalcType, double theLfcIndexValue, double theELValue);
	void FixPressureDataSoundingWithGroundData(NFmiFastQueryInfo* theGroundDataInfo);
	unsigned int GetHighestNonMissingValueLevelIndex(FmiParameterName theParaId);
	float GetPressureAtHeight(double H);
	void ClearDatas(void);
	bool FillParamData(NFmiFastQueryInfo* theInfo, FmiParameterName theId);
	bool FillParamData(NFmiFastQueryInfo* theInfo, FmiParameterName theId, const NFmiMetTime& theTime, const NFmiPoint& theLatlon);
	bool FastFillParamData(NFmiFastQueryInfo* theInfo, FmiParameterName theId);
	void InitZeroHeight(void); // tätä kutsutaan FillParamData-metodeista
	void CalculateHumidityData(void);

	NFmiLocation itsLocation;
	NFmiMetTime	itsTime;
	NFmiMetTime	itsOriginTime; // tämä otetaan talteen IsSameSounding-metodia varten

	// TODO Laita käyttämään NFmiDataMatrix-luokkaa dynaamista datalistaa varten. Laita myös
	// param-lista (joka annetaan fillData-metodeissa) data osaksi
	checkedVector<float> itsTemperatureData;
	checkedVector<float> itsDewPointData;
	checkedVector<float> itsHumidityData;
	checkedVector<float> itsPressureData;
	checkedVector<float> itsGeomHeightData; // tämä on korkeus dataa metreissä
	checkedVector<float> itsWindSpeedData;
	checkedVector<float> itsWindDirectionData;
	checkedVector<float> itsWindComponentUData;
	checkedVector<float> itsWindComponentVData;
	checkedVector<float> itsWindVectorData;

	float itsZeroHeight; // tältä korkeudelta alkaa luotauksen 0-korkeus, eli vuoristossa luotaus alkaa oikeasti korkeammalta ja se korkeus pitää käsitellä pintakorkeutena
	int itsZeroHeightIndex; // edellisen indeksi (paikka vektorissa). Arvo on -1 jos ei löytynyt kunnollista 0-korkeutta
	bool fObservationData;
	bool fPressureDataAvailable;
	bool fHeightDataAvailable;

	LFCIndexCache itsLFCIndexCache;
};

#endif // NFMISOUNDINGDATAOPT1_H
