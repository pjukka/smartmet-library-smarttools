// ======================================================================
/*!
 * \file NFmiSoundingIndexCalculator.h
 *
 * Tämä luokka laskee erilaisia luotausi ndeksejä annettujen querinfojen avulla.
 * Mm. CAPE, CIN, LCL, BulkShear StormRelatedHellicity jne.
 */
// ======================================================================

#ifndef NFMISOUNDINGINDEXCALCULATOR_H
#define NFMISOUNDINGINDEXCALCULATOR_H

#include "NFmiDataMatrix.h"

class NFmiSmartInfo;
class NFmiFastQueryInfo;
class NFmiQueryInfo;
class NFmiSoundingData;
class NFmiDrawParam;
class NFmiMetTime;
class NFmiInfoOrganizer;
class NFmiLocation;
class NFmiPoint;

// Nämä parametrit ovat niitä joita lasketaan luotausnäytössä.
// Tarkoitus on nyt pystyä laskemaan niitä myös hilamuodossa karttanäytölle.
// Sitä varten tein oman parametrilistan, joka kuuluisi NFmiParameterName-enumiin.
// Mutta en halua sotkea sitä näillä, joten tein oman listan, jota kuitenkin käytetään
// kuin se olisi originaali NFmiParameterName-enumissa.
typedef enum
{
	kSoundingParNone = 4619, // aloitan luvut jostain numerosta mikä ei mene sekaisin originaali param-listan kanssa
	// aluksi surface (sur) arvojen avulla lasketut parametrit
	kSoundingParLCLSur = 4720,
	kSoundingParLFCSur,
	kSoundingParELSur,
	kSoundingParCAPESur,
	kSoundingParCAPE0_3kmSur,
	kSoundingParCINSur,
	kSoundingParLCLHeightSur,
	kSoundingParLFCHeightSur,
	kSoundingParELHeightSur,
	// sitten 500m arvojen avulla lasketut parametrit
	kSoundingParLCL500m = 4730,
	kSoundingParLFC500m,
	kSoundingParEL500m,
	kSoundingParCAPE500m,
	kSoundingParCAPE0_3km500m,
	kSoundingParCIN500m,
	kSoundingParLCLHeight500m,
	kSoundingParLFCHeight500m,
	kSoundingParELHeight500m,
	// sitten mostunstable (MostUn) arvojen avulla lasketut parametrit
	kSoundingParLCLMostUn = 4740,
	kSoundingParLFCMostUn,
	kSoundingParELMostUn,
	kSoundingParCAPEMostUn,
	kSoundingParCAPE0_3kmMostUn,
	kSoundingParCINMostUn,
	kSoundingParLCLHeightMostUn,
	kSoundingParLFCHeightMostUn,
	kSoundingParELHeightMostUn,
	// sitten normaaleja yleis indeksejä
	kSoundingParSHOW = 4750,
	kSoundingParLIFT,
	kSoundingParKINX,
	kSoundingParCTOT,
	kSoundingParVTOT,
	kSoundingParTOTL,
	// sitten vielä 'speciaali' paramereja
	kSoundingParBS0_6km = 4770, // BS=bulkshear
	kSoundingParBS0_1km,
	kSoundingParSRH0_3km, // SRH=storm related helicity
	kSoundingParSRH0_1km,
	kSoundingParWS1500m,
	kSoundingParThetaE0_3km
} FmiSoundingParameters;

class NFmiSoundingIndexCalculator
{
public:

	static bool FillSoundingData(NFmiFastQueryInfo *theInfo, NFmiSoundingData &theSoundingData, const NFmiMetTime &theTime, const NFmiLocation &theLocation);
	static void Calc(NFmiSmartInfo *theBaseInfo, NFmiInfoOrganizer *theInfoOrganizer, NFmiDrawParam *theDrawParam, NFmiDataMatrix<float> &theValues, const NFmiMetTime &theTime);
	static float Calc(NFmiSoundingData &theSoundingData, FmiSoundingParameters theParam);
	static float Calc(NFmiQueryInfo *theInfo, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, FmiSoundingParameters theParam);
};

#endif // NFMISOUNDINGINDEXCALCULATOR_H
