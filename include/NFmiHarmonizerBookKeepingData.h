#pragma once

#include <set>
#include "NFmiMetTime.h"
#include "NFmiParamBag.h"

// smartinfon pit‰‰ pit‰‰ kirjaa harmonisaattoriin liittyvist‰ asioista
// mitk‰ parametrit ja ajat ovat olleet mill‰kin hetkell‰ muokatttuina
class NFmiHarmonizerBookKeepingData
{
public:
	NFmiHarmonizerBookKeepingData(void);
	NFmiHarmonizerBookKeepingData(const std::set<NFmiMetTime> &theHarmonizerTimesSet,
								  bool foobar,
								  const NFmiParamBag &theHarmonizerParams);
	~NFmiHarmonizerBookKeepingData(void);

	NFmiParamBag& HarmonizerParams(void) {return itsHarmonizerParams;}
	void HarmonizerParams(const NFmiParamBag &theParamBag) {itsHarmonizerParams = theParamBag;}
	void InsertTime(const NFmiMetTime &theTime);
	bool HarmonizeAllTimes(void) const {return fHarmonizeAllTimes;}
	void HarmonizeAllTimes(bool newValue) {fHarmonizeAllTimes = newValue;}
	std::set<NFmiMetTime>& HarmonizerTimesSet(void) {return itsHarmonizerTimesSet;}

private:
	std::set<NFmiMetTime> itsHarmonizerTimesSet; // t‰nne laitetaan kaikki muokatut ajat jotta voidaan
												// harmonisoinnin yhteydess‰ tehd‰ timedesc, jonka avulla ajetaan
												// harmonisaattori skripti
	bool fHarmonizeAllTimes; // apuna edellisen set:in lis‰ksi, ett‰ jos k‰yd‰‰n l‰pi koko data
	NFmiParamBag itsHarmonizerParams; // t‰h‰n merkit‰‰n parametrit, joita on muokattu eri tyˆkaluilla.
};
