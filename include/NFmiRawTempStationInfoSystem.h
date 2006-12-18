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

#endif // NFMIRAWTEMPSTATIONINFOSYSTEM_H
