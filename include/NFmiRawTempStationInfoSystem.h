// NFmiRawTempStationInfoSystem.h
//
// Luokka jossa on lista luotausasemia eri puolilta maailmaa.
// T‰t‰ k‰ytet‰‰n silloin kun k‰ytet‰‰n MetEditorin ominaisuutta,
// jolloin katsotaan raaka TEMP sanomista tulkattuja luotaus sanomia.
// N‰iss‰ sanomissa on vain aseman id. Tuon id:n avulla nimi ja 
// sijainti saadaan selville, jos se on asema lista tiedostossa.
// 
// Asemalista tiedoston formaatti:
// Kommentit sallittuja.
// Aseman tiedot esitet‰‰n aina yhdell‰ rivill‰, rivej‰ voi olla kuinka monta tahansa.
// Asema_is lon lat nimi(rivin loppuun asti)

#ifndef NFMIRAWTEMPSTATIONINFOSYSTEM_H
#define NFMIRAWTEMPSTATIONINFOSYSTEM_H

#include "NFmiHPlaceDescriptor.h"
#include "NFmiStation.h"

class NFmiRawTempStationInfoSystem
{
public:
	NFmiRawTempStationInfoSystem(void)
	:itsInitLogMessage()
	,itsLocations()
	{}

	void Init(const std::string &theInitFileName);
	NFmiHPlaceDescriptor& Locations(void) {return itsLocations;}
	const std::string& InitLogMessage(void) const {return itsInitLogMessage;}
private:

	std::string itsInitLogMessage; // onnistuneen initialisoinnin viesti, miss‰ voi olla varoituksia lokiin.
	NFmiHPlaceDescriptor itsLocations;
};

class AviationStation : public NFmiStation
{
public:
	AviationStation(void)
	:NFmiStation()
	,itsIcaoStr()
	{}

	AviationStation(unsigned long theIdent,
					const NFmiString & theName,
					double theLongitude,
					double theLatitude,
					const std::string &theIcaoStr)
	:NFmiStation(theIdent, theName, theLongitude, theLatitude, kFloatMissing, kWMO)
	,itsIcaoStr(theIcaoStr)
	{}

	~AviationStation(void){}

	const std::string& IcaoStr(void) const {return itsIcaoStr;}
	void IcaoStr(const std::string &newValue) {itsIcaoStr = newValue;}
	NFmiLocation * Clone(void) const 
	{
		return new AviationStation(*this);
	}

private:
	std::string itsIcaoStr; // icao tunnus (esim. EFHK)
};

class NFmiAviationStationInfoSystem
{
public:
	NFmiAviationStationInfoSystem(bool verboseMode = false)
	:itsInitLogMessage()
	,itsLocations()
	,fVerboseMode(verboseMode)
	{}

	NFmiHPlaceDescriptor& Locations(void) {return itsLocations;}
	const std::string& InitLogMessage(void) const {return itsInitLogMessage;}
	void Init(const std::string &theInitFileName);

	bool FindAviationStation(const std::string &theIcaoStr);
	bool FindStation(unsigned long theStationId);
	const NFmiLocation* CurrentLocation(const std::string &theIcaoStr);
private:

	double GetLatOrLonFromString(const std::string &theLatOrLonStr, const std::string &theLineStr, const std::string &theInitFileName, char posMark, char negMark);
	bool GetAviationStationFromString(const std::string &theStationStr, const std::string &theInitFileName, AviationStation &theStationOut);

	std::string itsInitLogMessage; // onnistuneen initialisoinnin viesti, miss‰ voi olla varoituksia lokiin.
	NFmiHPlaceDescriptor itsLocations;
	bool fVerboseMode;
};

class NFmiSilamStationList
{
public:
	class Station
	{
	public:
		NFmiPoint itsLatlon;
		std::string itsCountry;
		std::string itsType;
		std::string itsInfo;
	};


	NFmiSilamStationList(void)
	:itsInitLogMessage()
	,itsLocations()
	{}

	void Init(const std::string &theInitFileName);
	void Clear(void);
	checkedVector<NFmiSilamStationList::Station>& Locations(void) {return itsLocations;}
	const std::string& InitLogMessage(void) const {return itsInitLogMessage;}
private:

	std::string itsInitLogMessage; // onnistuneen initialisoinnin viesti, miss‰ voi olla varoituksia lokiin.
	checkedVector<NFmiSilamStationList::Station> itsLocations;
};


#endif // NFMIRAWTEMPSTATIONINFOSYSTEM_H
