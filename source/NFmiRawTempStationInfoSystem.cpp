// NFmiRawTempStationInfoSystem.cpp
//

#include "NFmiRawTempStationInfoSystem.h"
#include "NFmiCommentStripper.h"
#include "NFmiStation.h"
#include "NFmiLocationBag.h"

// purkaa rivin
// 2963 23.5 60.8167 JOKIOINEN OBSERVATORIO
// osiin, ja rakentaan NFmiStation-otuksen, niin ett‰ 3. spacen j‰lkeen kaikki on nime‰.
static bool GetStationFromString(const std::string &theStationStr, const std::string &theInitFileName, NFmiStation &theStationOut)
{
	std::vector<std::string> strVector = NFmiStringTools::Split(theStationStr, " ");
	if(strVector.size() > 1 && strVector.size() < 4)
	{
		std::string errStr("GetStationFromString - station info had too few parts on line\n");
		errStr += theStationStr;
		errStr += "\n in file: ";
		errStr += theInitFileName;
		throw std::runtime_error(errStr);
	}
	else if(strVector.size() <= 1)
		return false; // en ole varma splitterist‰, mutta ignoorataan vain tyhj‰/yhden osion rivit

	theStationOut.SetIdent(NFmiStringTools::Convert<unsigned long>(strVector[0]));
	theStationOut.SetLongitude(NFmiStringTools::Convert<double>(strVector[1]));
	theStationOut.SetLatitude(NFmiStringTools::Convert<double>(strVector[2]));
	std::string name;
	for(unsigned int i = 3; i<strVector.size(); i++)
	{
		if(i != 3)
			name += " ";
		name += strVector[i];
	}
	theStationOut.SetName(NFmiString(name));
	return true;
}

// poistaa alku+loppu spacet ja tabulaattorit
// sek‰ sanojen v‰list‰ muut paitsi spacet ja tabit paitsi yhden spacen.
// Jos sanojen v‰lill‰ on ainoastaan tabulaattoreita, korvaa ne yhdell‰ spacella.
// <cr> ja <lf> ei poisteta eik‰ huomioida (paitsi alusta ja lopusta)!!
static std::string RemoveExtraSpaces(const std::string &theStr)
{
	std::string tmp(theStr);

	// trimmataan stringin alusta ja lopusta whitespacet pois
	NFmiStringTools::Trim(tmp, ' ');
	NFmiStringTools::Trim(tmp, '\t');
	NFmiStringTools::Trim(tmp, '\t');
	NFmiStringTools::Trim(tmp, '\n');
	std::string result;
	bool wordBreak = false;
	for(unsigned int i=0; i<tmp.size(); i++)
	{
		if(tmp[i] == ' ' || tmp[i] == '\t')
		{
			if(wordBreak == false)
			{
				wordBreak = true;
				result.push_back(' '); // laitetaan aina sanan per‰‰n yksi space
			}
		}
		else
		{
			wordBreak = false;
			result.push_back(tmp[i]);
		}
	}

	return result;
}

// Lukee initialisointi tiedostosta asema tiedot, heitt‰‰ poikkeuksen jos ongelmia.
// Asema tiedoston formaatti on seuraava (kommentit sallittuja, mutta asema rivit 
// ilman kommentti merkkej‰):
// 
// // id  lon  lat      name (with spaces to the end of line)
// 2963 23.5 60.8167 JOKIOINEN OBSERVATORIO
// 3005 -1.18333 60.1333 LERWICK
void NFmiRawTempStationInfoSystem::Init(const std::string &theInitFileName)
{
	itsInitLogMessage = "";
	itsLocations = NFmiHPlaceDescriptor(); // tyhjennet‰‰n ensin asemalista
	NFmiLocationBag locBag;

	if(theInitFileName.empty())
		throw std::runtime_error("NFmiRawTempStationInfoSystem::Init - empty settings filename given.");

	NFmiCommentStripper stripComments;
	if(stripComments.ReadAndStripFile(theInitFileName))
	{
#ifdef OLDGCC
		std::istrstream in(stripComments.GetString().c_str());
#else
		std::stringstream in(stripComments.GetString());
#endif

		const int maxBufferSize = 1024+1; // kuinka pitk‰ yhden rivin maksimissaan oletetaan olevan
		std::string buffer;
		NFmiStation station;
		int i = 0;
		int counter = 0;
		do
		{
			buffer.resize(maxBufferSize);
			in.getline(&buffer[0], maxBufferSize);

//			if(i >= 9320)
//				int jj=0;
			int realSize = strlen(buffer.c_str());
			buffer.resize(realSize);
			buffer = ::RemoveExtraSpaces(buffer);
			if(::GetStationFromString(buffer, theInitFileName, station))
			{
				counter++;
				locBag.AddLocation(station, true);
			}
			i++;
		}while(in.good());
		itsLocations = NFmiHPlaceDescriptor(locBag); // tyhjennet‰‰n ensin asemalista
		itsInitLogMessage = "Initializing sounding station data went OK, from file: ";
		itsInitLogMessage += theInitFileName;
		if(itsLocations.Size() == 0)
			itsInitLogMessage += "\nWarning: 0 sounding stations found.";
		else
			itsInitLogMessage += std::string("\nInfo: ") + NFmiStringTools::Convert<int>(itsLocations.Size()) + " sounding stations found.";
		int diff = counter - itsLocations.Size();
		if(diff > 0)
		{
			itsInitLogMessage += "\nWarning: there were probably ";
			itsInitLogMessage += NFmiStringTools::Convert<int>(diff);
			itsInitLogMessage += " 'extra' stations in file, meaning that there were more station lines than different station were found, use unique stations.";
		}
	}
	else
		throw std::runtime_error(std::string("NFmiRawTempStationInfoSystem::Init - trouble reading file: ") + theInitFileName);
}
