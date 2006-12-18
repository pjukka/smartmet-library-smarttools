
#ifndef NFMITEMPCODE_H
#define NFMITEMPCODE_H

#include <string>
#include <vector>
#include <map>
#include "NFmiMetTime.h"
#include "NFmiStation.h"

class NFmiQueryData;
class NFmiHPlaceDescriptor;
class NFmiProducer;

struct TEMPLevelData
{
	void Clear(void)
	{
		itsPressure = kFloatMissing;
		itsHeight = kFloatMissing;
		itsTemperature = kFloatMissing;
		itsDewPoint = kFloatMissing;
		itsWS = kFloatMissing;
		itsWD = kFloatMissing;
	}

	double itsPressure;
	double itsHeight;
	double itsTemperature;
	double itsDewPoint;
	double itsWS;
	double itsWD;
};

namespace DecodeTEMP
{
	// T‰lle funktiolle annetaan haluttu l‰j‰ TEMP koodia 
	// (stringiss‰ voi olla vaikka kuinka monta eri TEMP sanomaa per‰kk‰in).
	// Funktio purkaa ne ja laittaa datan new:lla luotuun QueryData-olioon.
	NFmiQueryData* MakeNewDataFromTEMPStr(const std::string &theTEMPStr, std::string &theCheckReportStr, NFmiHPlaceDescriptor *theTempStations, const NFmiPoint &theUnknownStationLocation, const NFmiProducer &theWantedProducer, bool fRoundTimesToNearestSynopticTimes);
}

// Luokka joka saa 4 stringi‰, miss‰ on koko temp-koodi.
// Purkaa koodin ja laskee leveleiksi ja muuksi dataksi.
class NFmiTEMPCode
{
public:
	NFmiTEMPCode(void);
	NFmiTEMPCode(NFmiHPlaceDescriptor *theTempStations, const NFmiPoint &theUnknownStationLocation);
	~NFmiTEMPCode(void);

	// Saa A,B,C ja D koodit. Tarkistaa ne ja purkaa. Palauttaa luvun, joka kertoo kuinka monta 
	// osaa kelpasi (eli oli samalta asemalta, samaan aikaan jne. )
	int InsertCodeStrings(const std::string &theCodeAStr, const std::string &theCodeBStr, const std::string &theCodeCStr, const std::string &theCodeDStr, bool fNoEqualSignInCode);

	void Time(const NFmiMetTime &theTime) {itsTime = theTime;}
	const NFmiMetTime& Time(void) const {return itsTime;}
	const NFmiStation& Station(void) const {return itsStation;}
//	const std::vector<TEMPLevelData>& LevelData(void) const {return itsLevels;}
	const std::map<double, TEMPLevelData>& LevelData(void) const {return itsLevels;}

private:
	int Decode(void);
	bool DecodeA(void);
	bool DecodeB(void);
	bool DecodeC(void);
	bool DecodeD(void);
//	void SortLevels(void);
	void AddData(const TEMPLevelData &theLevelData);

	std::string itsOriginalCodeAStr; // t‰h‰n talletetaan koko koodi osa A
	std::string itsOriginalCodeBStr; // t‰h‰n talletetaan koko koodi osa B
	std::string itsOriginalCodeCStr; // t‰h‰n talletetaan koko koodi osa C
	std::string itsOriginalCodeDStr; // t‰h‰n talletetaan koko koodi osa D

	NFmiMetTime itsTime;
	NFmiStation itsStation;
	bool fTempMobil; // onko kyseess‰ temp mobil koodia, joka pit‰‰ purkaa hieman erilailla
	NFmiHPlaceDescriptor *itsTempStations; // Ei omista.  Jos t‰ss‰ on luotausinfo, sit‰ voidaan pit‰‰ luotaus asemien "tietokantana"
	NFmiPoint itsUnknownStationLocation;

//	std::vector<TEMPLevelData> itsLevels;
	std::map<double, TEMPLevelData> itsLevels;

};

#endif // NFMITEMPCODE_H
