// ======================================================================
/*!
 * \file NFmiSoundingData.cpp
 *
 * Apuluokka laskemaan ja tutkimaan luotaus dataa. Osaa täyttää itsensä
 * mm. mallipinta QueryDatasta (infosta).
 */
// ======================================================================

#include "NFmiSoundingData.h"
#include "NFmiFastQueryInfo.h"
#include "NFmiDataModifierAvg.h"
#include "NFmiInterpolation.h"
#include "NFmiSoundingFunctions.h"

using namespace NFmiSoundingFunctions;

// hakee lähimmän sopivan painepinnan, mistä löytyy halutuille parametreille arvot
// Mutta ei sallita muokkausta ennen 1. validia leveliä!
bool NFmiSoundingData::GetTandTdValuesFromNearestPressureLevel(double P, double &theFoundP, double &theT, double &theTd)
{
	if(P != kFloatMissing)
	{
		checkedVector<float>&pV = GetParamData(kFmiPressure);
		checkedVector<float>&tV = GetParamData(kFmiTemperature);
		checkedVector<float>&tdV = GetParamData(kFmiDewPoint);
		if(pV.size() > 0) // oletus että parV on saman kokoinen kuin pV -vektori
		{
			double minDiffP = 999999;
			theFoundP = 999999;
			theT = kFloatMissing;
			theTd = kFloatMissing;
			bool foundLevel = false;
//			for(int i=ZeroHeightIndex(); i<static_cast<int>(pV.size()); i++)
			for(int i=0; i<static_cast<int>(pV.size()); i++)
			{
				if(i < 0)
					return false; // jos 'tyhjä' luotaus, on tässä aluksi -1 indeksinä
				if(pV[i] != kFloatMissing)
				{
					double pDiff = ::fabs(pV[i] - P);
					if(pDiff < minDiffP)
					{
						theFoundP = pV[i];
						minDiffP = pDiff;
						theT = tV[i];
						theTd = tdV[i];
						foundLevel = true;
					}
				}
			}
			if(foundLevel)
				return true;

			theFoundP = kFloatMissing; // 'nollataan' tämä vielä varmuuden vuoksi
		}
	}
	return false;
}

// hakee lähimmän sopivan painepinnan, mistä löytyy halutulle parametrille ei-puuttuva arvo
bool NFmiSoundingData::SetValueToPressureLevel(float P, float theParamValue, FmiParameterName theId)
{
	if(P != kFloatMissing)
	{
		checkedVector<float>&pV = GetParamData(kFmiPressure);
		checkedVector<float>&parV = GetParamData(theId);
		if(pV.size() > 0) // oletus että parV on saman kokoinen kuin pV -vektori
		{
			checkedVector<float>::iterator it = std::find(pV.begin(), pV.end(), P);
			if(it != pV.end())
			{
				int index = static_cast<int>(std::distance(pV.begin(), it));
				parV[index] = theParamValue;
				return true;
			}
		}
	}
	return false;
}

void NFmiSoundingData::SetTandTdSurfaceValues(float T, float Td)
{
	checkedVector<float>&tV = GetParamData(kFmiTemperature);
	checkedVector<float>&tdV = GetParamData(kFmiDewPoint);
	tV[0] = T;
	tdV[0] = Td;
}

// paluttaa paine arvon halutulle metri korkeudelle
float NFmiSoundingData::GetPressureAtHeight(double H)
{
	if(H == kFloatMissing)
		return kFloatMissing;

	double maxDiffInH = 100; // jos ei voi interpoloida, pitää löydetyn arvon olla vähintäin näin lähellä, että hyväksytään
	checkedVector<float>&pV = GetParamData(kFmiPressure);
	checkedVector<float>&hV = GetParamData(kFmiGeomHeight);
	float value = kFloatMissing;
	if(hV.size() > 0 && pV.size() == hV.size())
	{
		unsigned int ssize = static_cast<unsigned int>(pV.size());
		float lastP = kFloatMissing;
		float lastH = kFloatMissing;
		float currentP = kFloatMissing;
		float currentH = kFloatMissing;
		bool goneOverWantedHeight = false;
		for(unsigned int i=0; i<ssize; i++)
		{
			currentP = pV[i];
			currentH = hV[i];
			if(currentH != kFloatMissing)
			{
				if(currentH > H)
				{
					if(currentP != kFloatMissing)
					{
						goneOverWantedHeight = true;
						break;
					}
				}
				if(currentP != kFloatMissing)
				{
					lastP = currentP;
					lastH = currentH;
					if(currentH == H)
						return currentP; // jos oli tarkka osuma, turha jatkaa
				}
			}
		}
		if(goneOverWantedHeight && lastP != kFloatMissing && currentP != kFloatMissing && lastH != kFloatMissing && currentH != kFloatMissing)
		{ // interpoloidaan arvo kun löytyi kaikki arvot
			value = static_cast<float>(NFmiInterpolation::Linear(H, currentH, lastH, currentP, lastP));
		}
		else if(lastP != kFloatMissing && lastH != kFloatMissing && ::fabs(lastH - H) < maxDiffInH)
			value = lastP;
		else if(currentP != kFloatMissing && currentH != kFloatMissing && ::fabs(currentH - H) < maxDiffInH)
			value = currentP;
	}
	return value;
}

// Luotausten muokkausta varten pitää tietään onko ensimmäinen luotaus vaihtunut (paikka, aika, origin-aika).
// Jos ei ole ja luotausta on modifioitu, piirretään modifioitu, muuten nollataan 'modifioitu'
// luotaus ja täytetään se tällä uudella datalla.
bool NFmiSoundingData::IsSameSounding(const NFmiSoundingData &theOtherSounding)
{
	if(Location() == theOtherSounding.Location())
		if(Time() == theOtherSounding.Time())
			if(OriginTime() == theOtherSounding.OriginTime())
				return true;
	return false;
}

// hakee ne arvot h, u ja v parametreista, mitkä ovat samalta korkeudelta ja mitkä
// eivät ole puuttuvia. Haetaan alhaalta ylös päin arvoja
bool NFmiSoundingData::GetLowestNonMissingValues(float &H, float &U, float &V)
{
	checkedVector<float>&hV = GetParamData(kFmiGeomHeight);
	checkedVector<float>&uV = GetParamData(kFmiWindUMS);
	checkedVector<float>&vV = GetParamData(kFmiWindVMS);
	if(hV.size() > 0 && hV.size() == uV.size() && hV.size() == vV.size())
	{
		for(int i=0; i<static_cast<int>(hV.size()); i++)
		{
			if(hV[i] != kFloatMissing && uV[i] != kFloatMissing && vV[i] != kFloatMissing)
			{
				H = hV[i];
				U = uV[i];
				V = vV[i];
				return true;
			}
		}
	}
	return false;
}

// laskee halutun parametrin arvon haluttuun metri korkeuteen
float NFmiSoundingData::GetValueAtHeight(FmiParameterName theId, float H)
{
	float P = GetPressureAtHeight(H);
	if(P == kFloatMissing)
		return kFloatMissing;

	return GetValueAtPressure(theId, P);
}

// Hakee halutun parametrin arvon halutulta painekorkeudelta.
float NFmiSoundingData::GetValueAtPressure(FmiParameterName theId, float P)
{
	if(P == kFloatMissing)
		return kFloatMissing;

	checkedVector<float>&pV = GetParamData(kFmiPressure);
	checkedVector<float>&paramV = GetParamData(theId);
	float value = kFloatMissing;
	if(paramV.size() > 0 && pV.size() == paramV.size())
	{
		unsigned int ssize = static_cast<unsigned int>(pV.size());
		float lastP = kFloatMissing;
		float lastValue = kFloatMissing;
		float currentP = kFloatMissing;
		float currentValue = kFloatMissing;
		for(unsigned int i=0; i<ssize; i++)
		{
			currentP = pV[i];
			currentValue = paramV[i];
			if(currentP != kFloatMissing)
			{
				if(currentP < P)
				{
					if(currentValue != kFloatMissing)
						break;
				}
				if(currentValue != kFloatMissing)
				{
					lastP = currentP;
					lastValue = currentValue;
					if(currentP == P)
						return currentValue; // jos oli tarkka osuma, turha jatkaa
				}
			}
		}
		float maxPDiff = 10.; // suurin sallittu ero, että arvo hyväksytään, jos pyydetty paine on 'asteikon ulkona'
		if(lastP != kFloatMissing && currentP != kFloatMissing && lastValue != kFloatMissing && currentValue != kFloatMissing)
		{ // interpoloidaan arvo kun löytyi kaikki arvot
			if(theId == kFmiWindVectorMS)
				value = CalcLogInterpolatedWindWectorValue(lastP, currentP, P, lastValue, currentValue);
			else
				value = CalcLogInterpolatedValue(lastP, currentP, P, lastValue, currentValue);
		}
		else if(lastP != kFloatMissing && lastValue != kFloatMissing)
		{
			if(::fabs(lastP - P) < maxPDiff)
				value = lastValue;
		}
		else if(currentP != kFloatMissing && currentValue != kFloatMissing)
		{
			if(::fabs(currentP - P) < maxPDiff)
				value = currentValue;
		}
	}
	return value;
}

// Laskee u ja v komponenttien keskiarvot halutulla välillä
// Huom! z korkeudet interpoloidaan, koska havaituissa luotauksissa niitä ei ole aina ja varsinkaan samoissa
// väleissä kuin tuulia
bool NFmiSoundingData::CalcAvgWindComponentValues(double fromZ, double toZ, double &u, double &v)
{
	u = kFloatMissing;
	v = kFloatMissing;
	checkedVector<float>&pV = GetParamData(kFmiPressure);
	if(pV.size() > 0)
	{
		double hStep = 100; // käydään dataa läpi 100 metrin välein

		NFmiDataModifierAvg uAvg;
		NFmiDataModifierAvg vAvg;
		for(double h = fromZ; h < toZ + hStep/2.; h += hStep) // käydää layeria 100 metrin välein
		{
			double pressure = GetPressureAtHeight(h);
			float tmpU = GetValueAtPressure(kFmiWindUMS, static_cast<float>(pressure));
			float tmpV = GetValueAtPressure(kFmiWindVMS, static_cast<float>(pressure));
			if(tmpU != kFloatMissing && tmpV != kFloatMissing)
			{
				uAvg.Calculate(tmpU);
				vAvg.Calculate(tmpV);
			}
		}
		u = uAvg.CalculationResult();
		v = vAvg.CalculationResult();
		return (u != kFloatMissing && v != kFloatMissing);
	}
	return false;
}

float NFmiSoundingData::FindPressureWhereHighestValue(FmiParameterName theId, float theMaxP, float theMinP)
{
	float maxValuePressure = kFloatMissing;
	checkedVector<float>&pV = GetParamData(kFmiPressure);
	checkedVector<float>&paramV = GetParamData(theId);
	if(pV.size() > 0)
	{
		float maxValue = -99999999.f;
		for(unsigned int i=0; i<pV.size(); i++)
		{
			float tmpP = pV[i];
			float tmpParam = paramV[i];
			if(tmpP != kFloatMissing && tmpParam != kFloatMissing)
			{
				if(tmpP <= theMaxP && tmpP >= theMinP) // eli ollaanko haluttujen pintojen välissä
				{
					if(tmpParam > maxValue)
					{
						maxValue = tmpParam;
						maxValuePressure = tmpP;
					}
				}

				if(tmpP < theMinP) // jos oltiin jo korkeammalla kuin minimi paine, voidaan lopettaa
					break;
			}
		}
	}
	return maxValuePressure;
}

// Käy läpi luotausta ja etsi sen kerroksen arvot, jolta löytyy suurin theta-E ja
// palauta sen kerroksen T, Td ja P ja laskettu max Theta-e.
// Etsitään arvoja jos pinta on alle theMinP-tason (siis alle tuon tason fyysisesti).
// HUOM! theMinP ei voi olla kFloatMissing, jos haluat että kaikki levelit käydään läpi laita sen arvoksi 0.
bool NFmiSoundingData::FindHighestThetaE(double &T, double &Td, double &P, double &theMaxThetaE, double theMinP)
{
	T = kFloatMissing;
	Td = kFloatMissing;
	P = kFloatMissing;
	theMaxThetaE = kFloatMissing;
	checkedVector<float>&pV = GetParamData(kFmiPressure);
	checkedVector<float>&tV = GetParamData(kFmiTemperature);
	checkedVector<float>&tdV = GetParamData(kFmiDewPoint);
	if(pV.size() > 0)
	{
		theMaxThetaE = -99999999;
		for(unsigned int i=0; i<pV.size(); i++)
		{
			float tmpP = pV[i];
			float tmpT = tV[i];
			float tmpTd = tdV[i];
			if(tmpP != kFloatMissing && tmpT != kFloatMissing && tmpTd != kFloatMissing)
			{
				if(tmpP >= theMinP) // eli ollaanko lähempana maanpintaa kuin raja paine pinta on
				{
					double thetaE = CalcThetaE(tmpT, tmpTd, tmpP);
					if(thetaE != kFloatMissing && thetaE > theMaxThetaE)
					{
						theMaxThetaE = thetaE;
						T = tmpT;
						Td = tmpTd;
						P = tmpP;
					}
				}
				else // jos oltiin korkeammalla, voidaan lopettaa
					break;
			}
		}
	}
	return theMaxThetaE != kFloatMissing;
}

// Tämä on Pieter Groenemeijerin ehdottama tapa laskea LCL-laskuihin tarvittavia T, Td ja P arvoja yli halutun layerin.
// Laskee keskiarvot T:lle, Td:lle ja P:lle haluttujen korkeuksien välille.
// Eli laskee keskiarvon lämpötilalle potentiaali lömpötilojen avulla.
// Laskee kastepisteen keskiarvon mixing valueiden avulla.
// Näille lasketaan keskiarvot laskemalla halutun layerin läpi 1 hPa askelissa, ettei epämääräiset
// näyte valit painota mitenkaan laskuja.
// Paineelle otetaan suoraan pohja kerroksen arvo.
// Oletus fromZ ja toZ eivät ole puuttuvia.
bool NFmiSoundingData::CalcLCLAvgValues(double fromZ, double toZ, double &T, double &Td, double &P, bool fUsePotTandMix)
{
	T = kFloatMissing;
	Td = kFloatMissing;
	P = kFloatMissing;
	checkedVector<float>&pV = GetParamData(kFmiPressure);
	checkedVector<float>&tV = GetParamData(kFmiTemperature);
	checkedVector<float>&tdV = GetParamData(kFmiDewPoint);
	if(pV.size() > 0)
	{
		for(unsigned int i=0; i<pV.size(); i++)
			if(pV[i] != kFloatMissing && tV[i] != kFloatMissing && tdV[i] != kFloatMissing) // etsitään 1. ei puuttuva paine arvo eli alin paine arvo (missä myös T ja Td arvot)
			{
				P = pV[i];
				break;
			}
		if(P == kFloatMissing)
			return false;
		int startP = static_cast<int>(round(GetPressureAtHeight(fromZ)));
		int endP = static_cast<int>(round(GetPressureAtHeight(toZ)));
		if(startP == kFloatMissing || endP == kFloatMissing || startP <= endP)
			return false;
		NFmiDataModifierAvg avgT; // riippuen moodista tässä lasketaan T tai Tpot keskiarvoa
		NFmiDataModifierAvg avgTd; // riippuen moodista tässä lasketaan Td tai w keskiarvoa
		for(int pressure = startP; pressure > endP; pressure--) // käydää layeria yhden hPa:n välein läpi
		{
			float temperature = GetValueAtPressure(kFmiTemperature, static_cast<float>(pressure));
			float dewpoint = GetValueAtPressure(kFmiDewPoint, static_cast<float>(pressure));
			if(temperature != kFloatMissing && dewpoint != kFloatMissing)
			{
				if(fUsePotTandMix)
				{
					avgT.Calculate(static_cast<float>(T2tpot(temperature, pressure)));
					avgTd.Calculate(static_cast<float>(CalcMixingRatio(temperature, dewpoint, pressure)));
				}
				else
				{
					avgT.Calculate(temperature);
					avgTd.Calculate(dewpoint);
				}
			}
		}
		if(avgT.CalculationResult() != kFloatMissing && avgTd.CalculationResult() != kFloatMissing )
		{
			if(fUsePotTandMix)
			{
				T = Tpot2t(avgT.CalculationResult(), P);
				Td = CalcDewPoint(T, avgTd.CalculationResult(), P);
			}
			else
			{
				T = avgT.CalculationResult();
				Td = avgTd.CalculationResult();
			}
			return (P != kFloatMissing && T != kFloatMissing && Td != kFloatMissing);
		}
	}
	return false;
}

// Tämä hakee annettua painearvoa lähimmät arvot, jotka löytyvät kaikille halutuille parametreille.
// Palauttaa true, jos löytyy dataa ja false jos ei löydy.
// OLETUS: maanpinta arvot ovat vektorin alussa, pitäisi tarkistaa??
bool NFmiSoundingData::GetValuesStartingLookingFromPressureLevel(double &T, double &Td, double &P)
{
	checkedVector<float>&pV = GetParamData(kFmiPressure);
	checkedVector<float>&tV = GetParamData(kFmiTemperature);
	checkedVector<float>&tdV = GetParamData(kFmiDewPoint);
	if(pV.size() > 0)
	{
		for(unsigned int i=0; i<pV.size(); i++)
		{
			if(pV[i] != kFloatMissing)// && tV[i] != kFloatMissing && tdV[i] != kFloatMissing)
			{
				if(P >= pV[i] && tV[i] != kFloatMissing && tdV[i] != kFloatMissing)
				{
					T = tV[i];
					Td = tdV[i];
					P = pV[i];
					return true;
				}
			}
		}
	}
	return false;
}

// oletuksia paljon:
// theInfo on validi, aika ja paikka on jo asetettu
bool NFmiSoundingData::FillParamData(NFmiFastQueryInfo* theInfo, FmiParameterName theId)
{
	try
	{
		checkedVector<float>&data = GetParamData(theId);
		data.resize(theInfo->SizeLevels(), kFloatMissing); // alustetaan vektori puuttuvalla
		bool paramFound = theInfo->Param(theId);
		if(paramFound == false && theId == kFmiDewPoint)
			paramFound = theInfo->Param(kFmiDewPoint2M); // kastepiste data voi tulla luotausten yhteydessä tällä parametrilla ja mallidatan yhteydessä toisella
		if(paramFound)
		{
			int i = 0;
			for(theInfo->ResetLevel(); theInfo->NextLevel(); i++)
				data[i] = theInfo->FloatValue();
			if(theInfo->HeightParamIsRising() == false) // jos ei nousevassa järjestyksessä, käännetään vektorissa olevat arvot
				std::reverse(data.begin(), data.end());
			if(theId == kFmiPressure)
				fPressureDataAvailable = true;
			if(theId == kFmiGeomHeight || theId == kFmiGeopHeight || theId == kFmiFlAltitude)
				fHeightDataAvailable = true;
			return true;
		}
	}
	catch(std::exception & /* e */)
	{
	}
	return false;
}

bool NFmiSoundingData::FillParamData(NFmiFastQueryInfo* theInfo, FmiParameterName theId, const NFmiMetTime& theTime, const NFmiPoint& theLatlon)
{
	bool status = false;
	checkedVector<float>&data = GetParamData(theId);
	data.resize(theInfo->SizeLevels(), kFloatMissing); // alustetaan vektori puuttuvalla
	if(theInfo->Param(theId))
	{
		int i = 0;
		for(theInfo->ResetLevel(); theInfo->NextLevel(); i++)
			data[i] = theInfo->InterpolatedValue(theLatlon, theTime); // varmuuden vuoksi kaikki interpoloinnit päälle, se funktio tarkistaa tarvitseeko sitä tehdä
		status = true;
	}
	else if(theId == kFmiDewPoint && theInfo->Param(kFmiHumidity))
	{
		unsigned int RHindex = theInfo->ParamIndex();
		if(!theInfo->Param(kFmiTemperature))
			return false;
		unsigned int Tindex = theInfo->ParamIndex();
		float T = 0;
		float RH = 0;
		int i = 0;
		for(theInfo->ResetLevel(); theInfo->NextLevel(); i++)
		{
			theInfo->ParamIndex(Tindex);
			T = theInfo->InterpolatedValue(theLatlon, theTime); // varmuuden vuoksi kaikki interpoloinnit päälle, se funktio tarkistaa tarvitseeko sitä tehdä
			theInfo->ParamIndex(RHindex);
			RH = theInfo->InterpolatedValue(theLatlon, theTime); // varmuuden vuoksi kaikki interpoloinnit päälle, se funktio tarkistaa tarvitseeko sitä tehdä
			data[i] = static_cast<float>(CalcDP(T, RH));
		}
		status = true;
	}
	else if(theId == kFmiPressure)
	{ // jos halutaan paine dataa ja parametria ei ollut datassa, oliko kyseessa painepinta data, jolloin paine pitää irroittaa level-tiedosta
		if(theInfo->FirstLevel())
		{
			if(theInfo->Level()->LevelType() == kFmiPressureLevel)
			{
				int i = 0;
				for(theInfo->ResetLevel(); theInfo->NextLevel(); i++)
					data[i] = static_cast<float>(theInfo->Level()->LevelValue());
				status = true;
			}
		}
	}

	if(theInfo->HeightParamIsRising() == false) // jos ei nousevassa järjestyksessä, käännetään vektorissa olevat arvot
		std::reverse(data.begin(), data.end());

	return status;
}

unsigned int NFmiSoundingData::GetHighestNonMissingValueLevelIndex(FmiParameterName theParaId)
{
	checkedVector<float> &vec = GetParamData(theParaId);
	checkedVector<float>::size_type ssize = vec.size();
	unsigned int index = 0;
	for(unsigned int i = 0; i < ssize; i++)
		if(vec[i] != kFloatMissing)
			index = i;
	return index;
}

// tämä leikkaa Fill.. -metodeissa laskettuja data vektoreita niin että pelkät puuttuvat kerrokset otetaan pois
void NFmiSoundingData::CutEmptyData(void)
{
	std::vector<FmiParameterName> itsSoundingParameters;
	itsSoundingParameters.push_back(kFmiPressure);
	itsSoundingParameters.push_back(kFmiTemperature);
	itsSoundingParameters.push_back(kFmiDewPoint);
	itsSoundingParameters.push_back(kFmiWindSpeedMS);
	itsSoundingParameters.push_back(kFmiWindDirection);
	itsSoundingParameters.push_back(kFmiGeomHeight);

	unsigned int greatestNonMissingLevelIndex = 0;
	unsigned int maxLevelIndex = static_cast<unsigned int>(GetParamData(itsSoundingParameters[0]).size());
	for(unsigned int i = 0; i < itsSoundingParameters.size(); i++)
	{
		unsigned int currentIndex = GetHighestNonMissingValueLevelIndex(itsSoundingParameters[i]);
		if(currentIndex > greatestNonMissingLevelIndex)
			greatestNonMissingLevelIndex = currentIndex;
		if(greatestNonMissingLevelIndex >= maxLevelIndex)
			return ; // ei tarvitse leikata, kun dataa löytyy korkeimmaltakin leveliltä
	}

	// tässä pitää käydä läpi kaikki data vektorit!!!! Oikeasti nämä datavektori pitäisi laittaa omaan vektoriin että sitä voitaisiin iteroida oikein!
	itsTemperatureData.resize(greatestNonMissingLevelIndex);
	itsDewPointData.resize(greatestNonMissingLevelIndex);
	itsHumidityData.resize(greatestNonMissingLevelIndex);
	itsPressureData.resize(greatestNonMissingLevelIndex);
	itsGeomHeightData.resize(greatestNonMissingLevelIndex);
	itsWindSpeedData.resize(greatestNonMissingLevelIndex);
	itsWindDirectionData.resize(greatestNonMissingLevelIndex);
	itsWindComponentUData.resize(greatestNonMissingLevelIndex);
	itsWindComponentVData.resize(greatestNonMissingLevelIndex);
	itsWindVectorData.resize(greatestNonMissingLevelIndex);

}

static bool FindTimeIndexies(NFmiFastQueryInfo* theInfo, const NFmiMetTime &theStartTime, long minuteRange, unsigned long &timeIndex1, unsigned long &timeIndex2)
{
	theInfo->FindNearestTime(theStartTime, kBackward);
	timeIndex1 = theInfo->TimeIndex();
	NFmiMetTime endTime(theStartTime);
	endTime.ChangeByMinutes(minuteRange);
	theInfo->FindNearestTime(endTime, kBackward);
	timeIndex2 = theInfo->TimeIndex();

	if(timeIndex1 == timeIndex2) // pitää testata erikois tapaus, koska TimeToNearestStep-palauttaa aina jotain, jos on dataa
	{
		theInfo->TimeIndex(timeIndex1);
		NFmiMetTime foundTime(theInfo->Time());
		if(foundTime > endTime || foundTime < theStartTime) // jos löydetty aika on alku ja loppu ajan ulkopuolella ei piirretä salamaa
			return false;
	}
	return true;
}

static bool FindAmdarSoundingTime(NFmiFastQueryInfo* theInfo, const NFmiMetTime &theTime, NFmiLocation &theLocation)
{
	theInfo->FirstLocation();  // amdareissa vain yksi dummy paikka, laitetaan se päälle
	NFmiMetTime timeStart(theTime);
	timeStart.ChangeByMinutes(-30);
	unsigned long timeIndex1 = 0;
	unsigned long timeIndex2 = 0;
	if(::FindTimeIndexies(theInfo, timeStart, 60, timeIndex1, timeIndex2) == false)
		return false;

	float lat = 0;
	float lon = 0;
	theInfo->Param(kFmiLatitude);
	unsigned long latIndex = theInfo->ParamIndex();
	theInfo->Param(kFmiLongitude);
	unsigned long lonIndex = theInfo->ParamIndex();
	double minDistance = 99999999;
	unsigned long minDistTimeInd = static_cast<unsigned long>(-1);
	for(unsigned long i=timeIndex1; i<=timeIndex2; i++)
	{
		theInfo->TimeIndex(i);

		for(theInfo->ResetLevel() ; theInfo->NextLevel(); )
		{
			theInfo->ParamIndex(latIndex);
			lat = theInfo->FloatValue();
			theInfo->ParamIndex(lonIndex);
			lon = theInfo->FloatValue();

			if(lat != kFloatMissing && lon != kFloatMissing)
			{
				NFmiLocation loc(NFmiPoint(lon, lat));
				double currDist = theLocation.Distance(loc);
				if(currDist < minDistance)
				{
					minDistance = currDist;
					minDistTimeInd = i;
				}
			}
		}
	}
	if(minDistance < 1000 * 1000) // jos lento löytyi vähintäin 1000 km säteeltä hiiren klikkauspaikasta, otetaan kyseinen amdar piirtoon
	{
		theInfo->TimeIndex(minDistTimeInd);
		// pitää lisäksi asettaa locationiksi luotauksen alkupiste
		theInfo->FirstLevel();
		theInfo->ParamIndex(latIndex);
		lat = theInfo->FloatValue();
		theInfo->ParamIndex(lonIndex);
		lon = theInfo->FloatValue();
		theLocation.SetLatitude(lat);
		theLocation.SetLongitude(lon);

		return true;
	}

	return false;
}

// Tälle anntaan asema dataa ja ei tehdä minkäänlaisia interpolointeja.
bool NFmiSoundingData::FillSoundingData(NFmiFastQueryInfo* theInfo, const NFmiMetTime& theTime, const NFmiMetTime& theOriginTime, const NFmiLocation& theLocation, int useStationIdOnly)
{
	NFmiMetTime usedTime = theTime;
	NFmiLocation usedLocation(theLocation);
	ClearDatas();
	if(theInfo && !theInfo->IsGrid())
	{
		fObservationData = true;
		theInfo->FirstLevel();
		bool amdarSounding = (theInfo->Level()->LevelType() == kFmiAmdarLevel);
		bool timeOk = false;
		if(amdarSounding)
		{
			timeOk = ::FindAmdarSoundingTime(theInfo, usedTime, usedLocation);
			usedTime = theInfo->Time();
		}
		else
			timeOk = theInfo->Time(usedTime);
		if(timeOk)
		{
			bool stationOk = false;
			if(amdarSounding)
				stationOk = true; // asemaa ei etsitä, jokainen lento liittyy tiettyyn aikaa
			else
				stationOk = (useStationIdOnly ? theInfo->Location(usedLocation.GetIdent()) : theInfo->Location(usedLocation));
			if(stationOk)
			{
				itsLocation = usedLocation;
				itsTime = usedTime;
				itsOriginTime = theOriginTime;

				FillParamData(theInfo, kFmiTemperature);
				FillParamData(theInfo, kFmiDewPoint);
				FillParamData(theInfo, kFmiPressure);
				if(!FillParamData(theInfo, kFmiGeopHeight))
					if(!FillParamData(theInfo, kFmiGeomHeight))
					FillParamData(theInfo, kFmiFlAltitude); // eri datoissa on geom ja geop heightia, kokeillaan molempia tarvittaessa
				FillParamData(theInfo, kFmiWindSpeedMS);
				FillParamData(theInfo, kFmiWindDirection);
				FillParamData(theInfo, kFmiWindUMS);
				FillParamData(theInfo, kFmiWindVMS);
				FillParamData(theInfo, kFmiWindVectorMS);
				CalculateHumidityData();
				InitZeroHeight();
				return true;
			}
		}
	}
	return false;
}

// Tälle annetaan hiladataa, ja interpolointi tehdään tarvittaessa ajassa ja paikassa.
bool NFmiSoundingData::FillSoundingData(NFmiFastQueryInfo* theInfo, const NFmiMetTime& theTime, const NFmiMetTime& theOriginTime, const NFmiPoint& theLatlon, const NFmiString &theName, NFmiFastQueryInfo* theGroundDataInfo)
{
	ClearDatas();
	if(theInfo && theInfo->IsGrid())
	{
		fObservationData = false;
		itsLocation = NFmiLocation(theLatlon);
		itsLocation.SetName(theName);
		itsTime = theTime;
		itsOriginTime = theOriginTime;

		FillParamData(theInfo, kFmiTemperature, theTime, theLatlon);
		FillParamData(theInfo, kFmiDewPoint, theTime, theLatlon);
		FillParamData(theInfo, kFmiPressure, theTime, theLatlon);
		if(!FillParamData(theInfo, kFmiGeopHeight, theTime, theLatlon))
			FillParamData(theInfo, kFmiGeomHeight, theTime, theLatlon); // eri datoissa on geom ja geop heightia, kokeillaan molempia tarvittaessa
		FillParamData(theInfo, kFmiWindSpeedMS, theTime, theLatlon);
		FillParamData(theInfo, kFmiWindDirection, theTime, theLatlon);
		FillParamData(theInfo, kFmiWindUMS, theTime, theLatlon);
		FillParamData(theInfo, kFmiWindVMS, theTime, theLatlon);
		FillParamData(theInfo, kFmiWindVectorMS, theTime, theLatlon);
		CalculateHumidityData();
		InitZeroHeight();
		FixPressureDataSoundingWithGroundData(theGroundDataInfo);
		return true;
	}
	return false;
}

// theVec is vector that is to be erased from the start.
// theCutIndex is the index that ends the cutting operation. If 0 then nothing is done.
template <class vectorContainer>
static void CutStartOfVector(vectorContainer &theVec, int theCutIndex)
{
	if(theVec.size() > 0 && theCutIndex > 0 && theCutIndex < theVec.size() - 1)
		theVec.erase(theVec.begin(), theVec.begin() + theCutIndex);
}

// Jos kyseessä on painepinta dataa, mistä löytyy myös siihen liittyvä pinta data, jossa on
// mukana parametri 472 eli paine aseman korkeudella, laitetaan tämä uudeksi ala-paineeksi luotaus-dataan
// ja laitetaan pinta-arvot muutenkin alimmalle tasolle pinta-datan mukaisiksi.
// HUOM! Oletus että löytyi ainakin yksi kerros, joka oli alle tämän pintakerroksen, koska
// en tee taulukkojen resize:a ainakaan nyt, eli taulukossa pitää olla tilaa tälle uudelle
// pintakerrokselle.
void NFmiSoundingData::FixPressureDataSoundingWithGroundData(NFmiFastQueryInfo* theGroundDataInfo)
{
	if(theGroundDataInfo)
	{
		NFmiPoint wantedLatlon(itsLocation.GetLocation());
		theGroundDataInfo->Param(kFmiTemperature);
		float groundT = theGroundDataInfo->InterpolatedValue(wantedLatlon, itsTime);
		theGroundDataInfo->Param(kFmiDewPoint);
		float groundTd = theGroundDataInfo->InterpolatedValue(wantedLatlon, itsTime);
		theGroundDataInfo->Param(kFmiWindSpeedMS);
		float groundWS = theGroundDataInfo->InterpolatedValue(wantedLatlon, itsTime);
		theGroundDataInfo->Param(kFmiWindDirection);
		float groundWD = theGroundDataInfo->InterpolatedValue(wantedLatlon, itsTime);
		theGroundDataInfo->Param(kFmiWindUMS);
		float groundU = theGroundDataInfo->InterpolatedValue(wantedLatlon, itsTime);
		theGroundDataInfo->Param(kFmiWindVMS);
		float groundV = theGroundDataInfo->InterpolatedValue(wantedLatlon, itsTime);
		theGroundDataInfo->Param(kFmiWindVectorMS);
		float groundWv = theGroundDataInfo->InterpolatedValue(wantedLatlon, itsTime);
		theGroundDataInfo->Param(kFmiPressureAtStationLevel);
		float groundStationPressure = theGroundDataInfo->InterpolatedValue(wantedLatlon, itsTime);
		theGroundDataInfo->Param(kFmiHumidity);
		float groundRH = theGroundDataInfo->InterpolatedValue(wantedLatlon, itsTime);

		if(groundStationPressure != kFloatMissing)
		{
			// oletus, kaikki vektorit on alustettu saman kokoisiksi kuin paine vektori
			if(theGroundDataInfo && itsPressureData.size() > 0)
			{
				// HUOM! luotausdatat ovat aina 'nousevassa' järjestyksessä eli maanpinta on taulukoissa ensimmäisenä
				for(int i=0; i < static_cast<int>(itsPressureData.size()); i++)
				{
					float currentPressure = itsPressureData[i];
					if(currentPressure != kFloatMissing && groundStationPressure >= currentPressure)
					{ // Eli nyt luotausdata taulukosta löytynyt paine on pienempi kuin paineasemakorkeudella (eli ollaan juuri menty
						// groundStationPressure-lukeman yli siis pinnasta avaruutta kohden, ei isompaan paine lukemaan), eli 
						// uusi pinta on laitettava ennen tätä kerrosta (jos kyseessä on alin pinta taulukosta)
						if(i > 0)
						{
							// mennään siis edelliseen tasoon ja laitetaan se uudeksi pinta kerrokseksi
							i--; 
							itsZeroHeightIndex = i;
							itsZeroHeight = 2;

							itsGeomHeightData[i] = 2; // Oletus: pinta data on 2 metrin korkeudella
							itsPressureData[i] = groundStationPressure;
							itsTemperatureData[i] = groundT;
							itsDewPointData[i] = groundTd;
							itsWindSpeedData[i] = groundWS;
							itsWindDirectionData[i] = groundWD;
							itsWindVectorData[i] = groundWv;
							itsWindComponentUData[i] = groundU;
							itsWindComponentVData[i] = groundV;
							itsHumidityData[i] = groundRH;

							// pitää ottaa vektoreista alkuosa pois, kun tuota itsZeroHeightIndex -dataosaa
							// ei näemmä käytetäkään missään
							if(itsZeroHeightIndex > 0)
							{	// huonoa koodia pitäisi olla vektorin vektori, että ei tarvitse aina muistaa kuinka monta vektoria on olemassa
								::CutStartOfVector(itsGeomHeightData, itsZeroHeightIndex);
								::CutStartOfVector(itsPressureData, itsZeroHeightIndex);
								::CutStartOfVector(itsTemperatureData, itsZeroHeightIndex);
								::CutStartOfVector(itsDewPointData, itsZeroHeightIndex);
								::CutStartOfVector(itsWindSpeedData, itsZeroHeightIndex);
								::CutStartOfVector(itsWindDirectionData, itsZeroHeightIndex);
								::CutStartOfVector(itsWindVectorData, itsZeroHeightIndex);
								::CutStartOfVector(itsWindComponentUData, itsZeroHeightIndex);
								::CutStartOfVector(itsWindComponentVData, itsZeroHeightIndex);
								::CutStartOfVector(itsHumidityData, itsZeroHeightIndex);
								itsZeroHeightIndex = 0;
							}

							break;
						}
					}
				}
			}
		}
	}
}

// laskee jo laskettujen T ja Td avulla RH
void NFmiSoundingData::CalculateHumidityData(void)
{
	size_t tVectorSize = itsTemperatureData.size();
	if(tVectorSize > 0 && itsDewPointData.size() == tVectorSize)
	{
		itsHumidityData.resize(tVectorSize);
		for(size_t i = 0; i < tVectorSize; i++)
		{
			if(itsTemperatureData[i] != kFloatMissing && itsDewPointData[i] != kFloatMissing)
				itsHumidityData[i] = static_cast<float>(NFmiSoundingFunctions::CalcRH(itsTemperatureData[i], itsDewPointData[i]));
		}
	}
}

// tätä kutsutaan FillParamData-metodeista
// sillä katsotaan mistä korkeudesta luotaus oikeasti alkaa
// Tehdään aloitus korkeus seuraavasti:
// Se pinta, miltä kaikki arvot löytyvät 1. kertaa ei puuttuvana
// tai jos sellaista ei löydy asetetaan arvoksi 0.
void NFmiSoundingData::InitZeroHeight(void)
{
	itsZeroHeight = 0;
	itsZeroHeightIndex = -1;
	// oletus, kaikki vektorit on alustettu saman kokoisiksi kuin paine vektori
	if(itsPressureData.size() > 0)
	{
		for(int i=0; i<static_cast<int>(itsPressureData.size()); i++)
		{
			if(itsPressureData[i] != kFloatMissing &&
//				itsTemperatureData[i] != kFloatMissing && // lämpötilaa ei ehkä tarvitse olla etsittäessä ensimmäistä validia kerrosta
//				itsDewPointData[i] != kFloatMissing && // kastepistettä ei ehkä tarvitse olla etsittäessä ensimmäistä validia kerrosta
				itsGeomHeightData[i] != kFloatMissing
//				&& itsWindSpeedData[i] != kFloatMissing // tuulta ei tarvitse olla etsittäessä ensimmäistä validia kerrosta
				)
			{
				itsZeroHeight = itsGeomHeightData[i];
				itsZeroHeightIndex = i;
				break; // lopetetaan kun 1. löytyi
			}
		}
	}
}

checkedVector<float>& NFmiSoundingData::GetParamData(FmiParameterName theId)
{
	static checkedVector<float> dummy;
	switch(theId)
	{
	case kFmiTemperature:
		return itsTemperatureData;
	case kFmiDewPoint:
		return itsDewPointData;
	case kFmiHumidity:
		return itsHumidityData;
	case kFmiPressure:
		return itsPressureData;
	case kFmiGeopHeight:
	case kFmiGeomHeight:
	case kFmiFlAltitude:
		return itsGeomHeightData;
	case kFmiWindSpeedMS:
		return itsWindSpeedData;
	case kFmiWindDirection:
		return itsWindDirectionData;
	case kFmiWindUMS:
		return itsWindComponentUData;
	case kFmiWindVMS:
		return itsWindComponentVData;
	case kFmiWindVectorMS:
		return itsWindVectorData;
	default:
	  throw std::runtime_error(std::string("NFmiSoundingData::GetParamData - wrong paramId given (Error in Program?): ") + NFmiStringTools::Convert<int>(theId));
	}
}

void NFmiSoundingData::ClearDatas(void)
{
	checkedVector<float>().swap(itsTemperatureData);
	checkedVector<float>().swap(itsDewPointData);
	checkedVector<float>().swap(itsHumidityData);
	checkedVector<float>().swap(itsPressureData);
	checkedVector<float>().swap(itsGeomHeightData);
	checkedVector<float>().swap(itsWindSpeedData);
	checkedVector<float>().swap(itsWindDirectionData);
	checkedVector<float>().swap(itsWindComponentUData);
	checkedVector<float>().swap(itsWindComponentVData);
	checkedVector<float>().swap(itsWindVectorData);

	fPressureDataAvailable = false;
	fHeightDataAvailable = false;
}

bool NFmiSoundingData::ModifyT2DryAdiapaticBelowGivenP(double P, double T)
{
	if(P == kFloatMissing || T == kFloatMissing)
		return false;

	checkedVector<float>&pV = GetParamData(kFmiPressure);
	checkedVector<float>&tV = GetParamData(kFmiTemperature);
	if(pV.size() > 0 && pV.size() == tV.size())
	{
		unsigned int ssize = static_cast<unsigned int>(pV.size());
		float wantedTPot = static_cast<float>(::T2tpot(T, P));
		float currentP = 1000;
		for(unsigned int i=0; i<ssize; i++)
		{
			currentP = pV[i];
			if(currentP >= P)
			{ // muutoksia siis tehtiin niin kauan kuin oltiin alle annetun paineen
				float wantedT = static_cast<float>(::Tpot2t(wantedTPot, currentP));
				tV[i] = wantedT;
			}
			else
				break;
		}
		return true;
	}
	return false;
}

bool NFmiSoundingData::ModifyTd2MixingRatioBelowGivenP(double P, double T, double Td)
{
	if(P == kFloatMissing || Td == kFloatMissing)
		return false;

	checkedVector<float>&pV = GetParamData(kFmiPressure);
	checkedVector<float>&tV = GetParamData(kFmiTemperature);
	checkedVector<float>&tdV = GetParamData(kFmiDewPoint);
	if(pV.size() > 0 && pV.size() == tV.size() && pV.size() == tdV.size())
	{
		unsigned int ssize = static_cast<unsigned int>(pV.size());

		float wantedMixRatio = static_cast<float>(::CalcMixingRatio(T, Td, P));
		for(unsigned int i=0; i<ssize; i++)
		{
			float currentP = pV[i];
			if(currentP >= P)
			{ // muutoksia siis tehtiin niin kauan kuin oltiin alle annetun paineen
				float wantedTd = static_cast<float>(::CalcDewPoint(tV[i], wantedMixRatio, currentP));
				tdV[i] = wantedTd;
			}
			else
				break;
		}
		return true;
	}
	return false;
}

bool NFmiSoundingData::ModifyTd2MoistAdiapaticBelowGivenP(double P, double Td)
{
	if(P == kFloatMissing || Td == kFloatMissing)
		return false;

	checkedVector<float>&pV = GetParamData(kFmiPressure);
	checkedVector<float>&tdV = GetParamData(kFmiDewPoint);
	if(pV.size() > 0 && pV.size() == tdV.size())
	{
		unsigned int ssize = static_cast<unsigned int>(pV.size());

//		float wantedTPot = static_cast<float>(::T2tpot(Td, P));
		float AOS = static_cast<float>(NFmiSoundingFunctions::OS(Td, P));
		float currentP = 1000;
		for(unsigned int i=0; i<ssize; i++)
		{
			currentP = pV[i];
			if(currentP >= P)
			{ // muutoksia siis tehtiin niin kauan kuin oltiin alle annetun paineen
				float ATSA  = static_cast<float>(NFmiSoundingFunctions::TSA(AOS, currentP));
//				float wantedT = static_cast<float>(::Tpot2t(wantedTPot, currentP));
				tdV[i] = ATSA;
			}
			else
				break;
		}
		return true;
	}
	return false;
}

static float FixCircularValues(float theValue, float modulorValue)
{
	if(theValue != kFloatMissing)
	{
		if(theValue < 0)
			return modulorValue - ::fmod(-theValue, modulorValue);
		else
			return ::fmod(theValue, modulorValue);
	}
	return theValue;
}

static float FixValuesWithLimits(float theValue, float minValue, float maxValue)
{
	if(theValue != kFloatMissing)
	{
		if(minValue != kFloatMissing)
			theValue = FmiMax(theValue, minValue);
		if(maxValue != kFloatMissing)
			theValue = FmiMin(theValue, maxValue);
	}
	return theValue;
}

bool NFmiSoundingData::Add2ParamAtNearestP(float P, FmiParameterName parId, float addValue, float minValue, float maxValue, bool fCircularValue)
{
	if(P == kFloatMissing)
		return false;

	checkedVector<float>&pV = GetParamData(kFmiPressure);
	checkedVector<float>&paramV = GetParamData(parId);
	if(pV.size() > 0 && pV.size() == paramV.size())
	{
		float currentP = kFloatMissing;
		float currentParam = kFloatMissing;
		float closestPdiff = kFloatMissing;
		unsigned int closestIndex = 0;
		unsigned int ssize = static_cast<unsigned int>(pV.size());
		for(unsigned int i=0; i<ssize; i++)
		{
			currentP = pV[i];
			currentParam = paramV[i];
			if(currentP != kFloatMissing && currentParam != kFloatMissing)
			{
				if(closestPdiff > ::fabs(P-currentP))
				{
					closestPdiff = ::fabs(P-currentP);
					closestIndex = i;
				}
			}
			if(currentP < P && closestPdiff != kFloatMissing)
				break;
		}

		if(closestPdiff != kFloatMissing)
		{
			float closestParamValue = paramV[closestIndex];
			closestParamValue += addValue;
			if(fCircularValue)
				closestParamValue = ::FixCircularValues(closestParamValue, maxValue);
			else
				closestParamValue = ::FixValuesWithLimits(closestParamValue, minValue, maxValue);

			paramV[closestIndex] = closestParamValue;
			return true;
		}
	}
	return false;
}

static float CalcU(float WS, float WD)
{
	if(WD == 999) // jos tuulensuunta on vaihtelevaa (999), palautetaan 0 arvo (voisi olla myös puuttuvaa)
		return 0.f;
	float value = kFloatMissing;
	if(WS != kFloatMissing && WD != kFloatMissing)
	{
		value = WS * sin(((static_cast<int>(180 + WD) % 360)/360.f)*(2.f*static_cast<float>(kPii))); // huom! tuulen suunta pitää ensin kääntää 180 astetta ja sitten muuttaa radiaaneiksi kulma/360 * 2*pii
	}
	return value;
}

static float CalcV(float WS, float WD)
{
	if(WD == 999) // jos tuulensuunta on vaihtelevaa (999), palautetaan 0 arvo (voisi olla myös puuttuvaa)
		return 0;
	float value = kFloatMissing;
	if(WS != kFloatMissing && WD != kFloatMissing)
	{
		value = WS * cos(((static_cast<int>(180 + WD) % 360)/360.f)*(2.f*static_cast<float>(kPii))); // huom! tuulen suunta pitää ensin kääntää 180 astetta ja sitten muuttaa radiaaneiksi kulma/360 * 2*pii
	}
	return value;
}

void NFmiSoundingData::UpdateUandVParams(void)
{
	checkedVector<float>&wsV = GetParamData(kFmiWindSpeedMS);
	checkedVector<float>&wdV = GetParamData(kFmiWindDirection);
	checkedVector<float>&uV = GetParamData(kFmiWindUMS);
	checkedVector<float>&vV = GetParamData(kFmiWindVMS);
	if(wsV.size() > 0 && wsV.size() == wdV.size() && wsV.size() == uV.size() && wsV.size() == vV.size())
	{
		unsigned int ssize = static_cast<unsigned int>(wsV.size());
		for(unsigned int i=0; i<ssize; i++)
		{
			uV[i] = ::CalcU(wsV[i], wdV[i]);
			vV[i] = ::CalcV(wsV[i], wdV[i]);
		}
	}
}

// tarkistaa onko kyseisellä ajanhetkellä ja asemalla ei puuttuvaa luotaus-dataa
bool NFmiSoundingData::HasRealSoundingData(NFmiFastQueryInfo &theSoundingLevelInfo)
{
//	theSoundingLevelInfo.First();
	if(theSoundingLevelInfo.Param(kFmiPressure) || theSoundingLevelInfo.Param(kFmiGeomHeight) || theSoundingLevelInfo.Param(kFmiGeopHeight))
	{
		int cc = 0;
		for(theSoundingLevelInfo.ResetLevel(); theSoundingLevelInfo.NextLevel(); cc++)
		{
			if(theSoundingLevelInfo.FloatValue() != kFloatMissing)
				return true; // jos miltään alku leveliltä löytyy yhtään korkeusdataa, on käyrä 'piirrettävissä'
			if(cc > 10) // pitää löytyä dataa 10 ensimmäisen kerroksen aikana
				break;
		}
		cc = 0; // käydään dataa läpi myös toisesta päästä, jos ei löytynyt
		for(theSoundingLevelInfo.LastLevel(); theSoundingLevelInfo.PreviousLevel(); cc++)
		{
			if(theSoundingLevelInfo.FloatValue() != kFloatMissing)
				return true; // jos miltään alku leveliltä löytyy yhtään korkeusdataa, on käyrä 'piirrettävissä'
			if(cc > 10) // pitää löytyä dataa 10 ensimmäisen kerroksen aikana
				break;
		}
	}
	return false;
}
