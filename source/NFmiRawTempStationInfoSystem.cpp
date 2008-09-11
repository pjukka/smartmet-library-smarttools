// NFmiRawTempStationInfoSystem.cpp
//

#include "NFmiRawTempStationInfoSystem.h"
#include "NFmiCommentStripper.h"
#include "NFmiStation.h"
#include "NFmiLocationBag.h"

using namespace std;

static void ThrowLatLonProblemException(const std::string &theFunctionNameStr, const std::string &theProblemStr, const std::string &theLatLonStr, const std::string &theLineStr, const std::string &theInitFileName)
{
	std::string errStr;
	errStr += theFunctionNameStr;
	errStr += " - ";
	errStr += theProblemStr;
	errStr += ": ";
	errStr += theLatLonStr;
	errStr += "\non line:\n";
	errStr += theLineStr;
	errStr += "\nin file: ";
	errStr += theInitFileName;
	throw std::runtime_error(errStr);
}

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

// ***********************************************************************
// *****************  AviationStationInfoSystem  *************************
// ***********************************************************************

// Lukee initialisointi tiedostosta asema tiedot, heitt‰‰ poikkeuksen jos ongelmia.
// Lukee asema tiedot jotka saadaan webist‰ paikasta http://weather.noaa.gov/data/nsd_bbsss.txt
// Asema tiedoston formaatti on seuraava (kommentit sallittuja, mutta asema rivit 
// ilman kommentti merkkej‰):
// 
// Keyed by Index (Block and Station) Number
// This index table from http://weather.noaa.gov/tg/site.shtml
// and the station table from http://weather.noaa.gov/data/nsd_bbsss.txt
// Last Updated
// All stations that have an index number assigned by the WMO are available in this file. The ASCII file contains the following fields in the order specified, delimited by semi-colons. Fields that are empty will be coded as no characters (adjacent delimiters) except where otherwise noted.
//
// Block Number 		2 digits representing the WMO-assigned block.
// Station Number  		3 digits representing the WMO-assigned station.
// ICAO Location Indicator 	4 alphanumeric characters, not all stations in this file have an assigned location indicator. The value "----" is used for stations that do not have an assigned location indicator.
// Place Name 			Common name of station location.
// State 			2 character abbreviation (included for stations located in the United States only).
// Country Name 		Country name is ISO short English form.
// WMO Region 			digits 1 through 6 representing the corresponding WMO region, 7 stands for the WMO Antarctic region.
// Station Latitude 		DD-MM-SSH where DD is degrees, MM is minutes, SS is seconds and H is N for northern hemisphere or S for southern hemisphere. The seconds value is omitted for those stations where the seconds value is unknown.
// Station Longitude 		DDD-MM-SSH where DDD is degrees, MM is minutes, SS is seconds and H is E for eastern hemisphere or W for western hemisphere. The seconds value is omitted for those stations where the seconds value is unknown.
// Upper Air Latitude 		DD-MM-SSH where DD is degrees, MM is minutes, SS is seconds and H is N for northern hemisphere or S for southern hemisphere. The seconds value is omitted for those stations where the seconds value is unknown.
// Upper Air Longitude 		DDD-MM-SSH where DDD is degrees, MM is minutes, SS is seconds and H is E for eastern hemisphere or W for western hemisphere. The seconds value is omitted for those stations where the seconds value is unknown.
// Station Elevation (Ha) 	The station elevation in meters. Value is omitted if unknown.
// Upper Air Elevation (Hp) 	The upper air elevation in meters. Value is omitted if unknown.
// RBSN indicator 		P if station is defined by the WMO as belonging to the Regional Basic Synoptic Network, omitted otherwise.

// 00;000;PABL;Buckland, Buckland Airport;AK;United States;4;65-58-56N;161-09-07W;;;7;;
// 01;001;ENJA;Jan Mayen;;Norway;6;70-56N;008-40W;70-56N;008-40W;10;9;P
// 01;002;----;Grahuken;;Norway;6;79-47N;014-28E;;;;15;
// 01;003;----;Hornsund;;Norway;6;77-00N;015-30E;;;10;12;
// 01;004;ENAS;Ny-Alesund Ii;;Norway;6;78-55N;011-56E;78-55N;011-56E;8;8;
// 01;006;----;Edgeoya;;Norway;6;78-14N;022-47E;;;;14;
// 01;007;----;Ny Alesund;;Norway;6;78-55N;011-56E;;;;12;
// 01;008;ENSB;Svalbard Lufthavn;;Norway;6;78-15N;015-28E;;;2;29;P
// 01;009;----;Phippsoya;;Norway;6;80-41N;020-52E;;;4;;
// 01;010;ENAN;Andoya;;Norway;6;69-18N;016-09E;69-18N;016-09E;1;14;P
// 01;011;----;Kvitoya;;Norway;6;80-06N;031-24E;80-06N;031-24E;16;16;
// 01;015;----;Hekkingen Fyr;;Norway;6;69-36N;017-50E;;;14;;
// 01;016;----;Senja-Grasmyrskogen;;Norway;6;69-17N;017-46E;;;50;;
// 01;023;ENDU;Bardufoss;;Norway;6;69-04N;018-32E;;;7;79;P
// jne....
void NFmiAviationStationInfoSystem::Init(const std::string &theInitFileName)
{
	itsInitLogMessage = "";
	itsLocations = NFmiHPlaceDescriptor(); // tyhjennet‰‰n ensin asemalista
	NFmiLocationBag locBag;

	if(theInitFileName.empty())
		throw std::runtime_error("NFmiAviationStationInfoSystem::Init - empty settings filename given.");

	NFmiCommentStripper stripComments(false); // ei stripata # eli pound kommentteja, koska stripperiss‰ on bugi
	if(stripComments.ReadAndStripFile(theInitFileName))
	{
#ifdef OLDGCC
		std::istrstream in(stripComments.GetString().c_str());
#else
		std::stringstream in(stripComments.GetString());
#endif

		const int maxBufferSize = 1024+1; // kuinka pitk‰ yhden rivin maksimissaan oletetaan olevan
		std::string buffer;
		AviationStation station;
		int i = 0;
		int counter = 0;
		do
		{
			buffer.resize(maxBufferSize);
			in.getline(&buffer[0], maxBufferSize);
			size_t realSize = strlen(buffer.c_str());
			buffer.resize(realSize);
			buffer = ::RemoveExtraSpaces(buffer);
			if(GetAviationStationFromString(buffer, theInitFileName, station))
			{
				counter++;
				unsigned long oldSize = locBag.GetSize();
				locBag.AddLocation(station, false); // AddLocation ongelma, k‰ytt‰‰ NFmiLocation::operator< -metodia, joka ei vertaa station-id:t‰ vaan nimi ja lat+lon. T‰llˆin jotkut noaa listan asemista menev‰t p‰‰llekk‰in.
				unsigned long newSize = locBag.GetSize();
				if(fVerboseMode && oldSize == newSize)
						cerr << "Duplicate station info:" << endl << buffer.c_str() << endl;
			}
			i++;
		}while(in.good());
		itsLocations = NFmiHPlaceDescriptor(locBag); // tyhjennet‰‰n ensin asemalista
		if(fVerboseMode)
		{
			itsInitLogMessage = "Initializing NFmiAviationStationInfoSystem data went OK, from file: ";
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
	}
	else
		throw std::runtime_error(std::string("NFmiAviationStationInfoSystem::Init - trouble reading file: ") + theInitFileName);
}

// esim. 70-56N (ilmeisesti myˆs sekunnit voi olla mukana, jolloin olisi 70-56-24N)
// Station Latitude 		DD-MM-SSH where DD is degrees, MM is minutes, SS is seconds and H is N for northern hemisphere or S for southern hemisphere. The seconds value is omitted for those stations where the seconds value is unknown.
// tai
// esim. 008-40W (ilmeisesti myˆs sekunnit voi olla mukana, jolloin olisi esim. 008-40-15W)
// Station Longitude 		DDD-MM-SSH where DDD is degrees, MM is minutes, SS is seconds and H is E for eastern hemisphere or W for western hemisphere. The seconds value is omitted for those stations where the seconds value is unknown.
double NFmiAviationStationInfoSystem::GetLatOrLonFromString(const std::string &theLatOrLonStr, const std::string &theLineStr, const std::string &theInitFileName, char posMark, char negMark)
{
	std::vector<std::string> strVector = NFmiStringTools::Split(theLatOrLonStr, "-");
	if(strVector.size() < 2 || strVector.size() > 3)
		ThrowLatLonProblemException("GetLatOrLonFromString", "Station lat/lon had illegal form", theLatOrLonStr, theLineStr, theInitFileName);

	int degrees = NFmiStringTools::Convert<int>(strVector[0]);
	int minutes = 0;
	int seconds = 0;
	bool posSign = true;
	if(strVector.size() == 2)
	{
		char ch = static_cast<char>(::toupper(strVector[1].operator[](strVector[1].size()-1))); // t‰h‰n N, S, W tai E
		if(ch == posMark)
			posSign = true;
		else if(ch == negMark)
			posSign = false;
		else
			ThrowLatLonProblemException("GetLatOrLonFromString", "The station's lat/lon string didn't have N, S, W or E at end", theLatOrLonStr, theLineStr, theInitFileName);
		strVector[1].resize(strVector[1].size()-1); // lyhennet‰‰n t‰t‰ stringi‰, niin ett‰ viimeinen merkki j‰‰ pois
	}
	else if(strVector.size() == 3)
	{
		char ch = static_cast<char>(::toupper(strVector[2].operator[](strVector[2].size()-1))); // t‰h‰n N, S, W tai E
		if(ch == posMark)
			posSign = true;
		else if(ch == negMark)
			posSign = false;
		else
			ThrowLatLonProblemException("GetLatOrLonFromString", "The station's lat/lon string didn't have N, S, W or E at end", theLatOrLonStr, theLineStr, theInitFileName);
		strVector[2].resize(strVector[2].size()-1); // lyhennet‰‰n t‰t‰ stringi‰, niin ett‰ viimeinen merkki j‰‰ pois
		seconds = NFmiStringTools::Convert<int>(strVector[2]);
	}
	minutes = NFmiStringTools::Convert<int>(strVector[1]);
	double value = degrees;
	value += minutes/60.;
	value += seconds/3600.;
	if(posSign == false)
		value = -value;
	return value;
}

// purkaa rivin
// 01;001;ENJA;Jan Mayen;;Norway;6;70-56N;008-40W;70-56N;008-40W;10;9;P
// osiin, ja rakentaan AviationStation-otuksen.
bool NFmiAviationStationInfoSystem::GetAviationStationFromString(const std::string &theStationStr, const std::string &theInitFileName, AviationStation &theStationOut)
{
	std::vector<std::string> strVector = NFmiStringTools::Split(theStationStr, ";");
	if(strVector.size() > 1 && strVector.size() < 13)
	{
		std::string errStr("GetAviationStationFromString - station info had too few parts (");
		errStr +=  NFmiStringTools::Convert<int>(static_cast<int>(strVector.size()-1));
		errStr +=  " ';' separators, 13 is right amount)";
		errStr +=  " on line\n";
		errStr += theStationStr;
		errStr += "\nin file: ";
		errStr += theInitFileName;
		throw std::runtime_error(errStr);
	}
	else if(strVector.size() == 13)
	{
		static int counter = 1;
		if(fVerboseMode)
		{
			if(counter == 1)
			{
				std::string errStr("Warning: GetAviationStationFromString - station info had too few parts (");
				errStr +=  NFmiStringTools::Convert<int>(static_cast<int>(strVector.size()-1));
				errStr +=  " ';' separators, 13 is right amount, but 12 is accepted)";
				errStr +=  " on line\n";
				errStr += theStationStr;
				errStr += "\nin file: ";
				errStr += theInitFileName;
				cerr << errStr << endl;
			}
			else if(counter == 2)
			{
				cerr << "Also these station lines had only 12 separators:" << endl;
				cerr << theStationStr << endl;
			}
			else
				cerr << theStationStr << endl;
		}
		counter++;
		return false;
	}
	else if(strVector.size() <= 1)
		return false; // en ole varma splitterist‰, mutta ignoorataan vain tyhj‰/yhden osion rivit

	std::string icaoId = strVector[2];
	if(icaoId.empty())
		return false; // otetaan vain asemat miss‰ icao id

	theStationOut.IcaoStr(icaoId);
	unsigned long stationId = NFmiStringTools::Convert<unsigned long>(strVector[0])*1000 + NFmiStringTools::Convert<unsigned long>(strVector[1]);
	theStationOut.SetIdent(stationId);
	double lat = GetLatOrLonFromString(strVector[7], theStationStr, theInitFileName, 'N', 'S');
	double lon = GetLatOrLonFromString(strVector[8], theStationStr, theInitFileName, 'E', 'W');
	theStationOut.SetLongitude(lon);
	theStationOut.SetLatitude(lat);
	std::string name = strVector[3];
	theStationOut.SetName(NFmiString(name));
	return true;
}

const NFmiLocation* NFmiAviationStationInfoSystem::CurrentLocation(const std::string &theIcaoStr)
{
	return itsLocations.Location();
}

// HUOM!!! Oletus hplaceDesc sis‰lt‰‰ AviationStation:eita.
bool NFmiAviationStationInfoSystem::FindAviationStation(const std::string &theIcaoStr)
{
	for(itsLocations.Reset(); itsLocations.Next(); )
	{
		const AviationStation &aviStation = *static_cast<const AviationStation*>(itsLocations.Location());
		if(aviStation.IcaoStr() == theIcaoStr)
			return true;
	}
	return false;
}

bool NFmiAviationStationInfoSystem::FindStation(unsigned long theStationId)
{
	return itsLocations.Location(theStationId);
}

// ***********************************************************************
// *****************  AviationStationInfoSystem  *************************
// ***********************************************************************


// ***********************************************************************
// *****************  NFmiSilamStationList  *************************
// ***********************************************************************

// t‰‰lt‰ heitet‰‰n aina poikkeus kun on virhe tai halutaan skipata rivi
static NFmiSilamStationList::Station GetSilamStationFromString(const std::string &lineStr)
{
	if(lineStr.empty())
		throw std::runtime_error("GetSilamStationFromString - empty line string."); // viestill‰ ei ole oikeastaan v‰li‰, l‰hinn‰ kommentti

	if(lineStr[0] == '!')
		throw std::runtime_error("GetSilamStationFromString - comment line string."); // viestill‰ ei ole oikeastaan v‰li‰, l‰hinn‰ kommentti

	std::stringstream in(lineStr);
	NFmiSilamStationList::Station station;

	double value = 0;
	in >> value; // latitude
	if(in.fail())
		throw std::runtime_error("GetSilamStationFromString - error in line string."); // viestill‰ ei ole oikeastaan v‰li‰, l‰hinn‰ kommentti
	std::string str;
	in >> str; // latitude sign
	if(in.fail())
		throw std::runtime_error("GetSilamStationFromString - error in line string."); // viestill‰ ei ole oikeastaan v‰li‰, l‰hinn‰ kommentti

	station.itsLatlon.Y(value); // latitude signilla ei ilmeisesti merkityst‰, ainakin longitude sign on merkityksetˆn


	in >> value; // longtitude
	if(in.fail())
		throw std::runtime_error("GetSilamStationFromString - error in line string."); // viestill‰ ei ole oikeastaan v‰li‰, l‰hinn‰ kommentti
	in >> str; // longtitude sign
	if(in.fail())
		throw std::runtime_error("GetSilamStationFromString - error in line string."); // viestill‰ ei ole oikeastaan v‰li‰, l‰hinn‰ kommentti

	station.itsLatlon.X(value); // longtitude signilla ei ilmeisesti merkityst‰, ainakin longitude sign on merkityksetˆn

	// sitten luetaan maa (joka voi olla useassa osassa)
	do
	{
		in >> str;
		if(in.fail())
			throw std::runtime_error("GetSilamStationFromString - error in line string."); // viestill‰ ei ole oikeastaan v‰li‰, l‰hinn‰ kommentti
		if(str.size() >= 2 && (str[0] == 'O' && str[1] == 'T') || (str[0] == 'P' && str[1] == 'L')) // tyyppi ilmeisesti alkaa OT tai PL alulla
			break;
		station.itsCountry += str;
	}while(true);

	if(in.fail())
		throw std::runtime_error("GetSilamStationFromString - error in line string."); // viestill‰ ei ole oikeastaan v‰li‰, l‰hinn‰ kommentti

	station.itsType = str;

	int maxBufferSize = 1024+1; // kuinka pitk‰ yhden rivin maksimissaan oletetaan olevan
	std::string buffer;
	buffer.resize(maxBufferSize);
	in.getline(&buffer[0], maxBufferSize);
	size_t length = buffer.length();
	size_t length2 = ::strlen(&buffer[0]);
	buffer.resize(length2);
	station.itsInfo = buffer;

	return station;
}


// Lukee Silam tyyppisest‰ asemalista tiedostosta asema tiedot, heitt‰‰ poikkeuksen jos ongelmia.
// Asema tiedoston formaatti on seuraava (!-merkki aloittaa kommentti rivin):
// 
// !    latitude        longitude        valtio                    tyyppi               info
// !--------------------------------------------------------------------------------------------------------------------
//
// 40.600   N   44.350   E   ARMENIA   PL_PWR   Ararat, Armenia teho 2x376 MW, Shut down 1989 
// 48.300   N   15.800   E   AUSTRIA   PL   Zwentendorf, Tullnerfeld 1
void NFmiSilamStationList::Init(const std::string &theInitFileName)
{
	itsInitLogMessage = "";
	itsLocations.clear(); // tyhjennet‰‰n ensin asemalista

	if(theInitFileName.empty())
		throw std::runtime_error("NFmiSilamStationList::Init - empty settings filename given.");

	ifstream in(theInitFileName.c_str());
	if(in)
	{
		const int maxBufferSize = 1024+1; // kuinka pitk‰ yhden rivin maksimissaan oletetaan olevan
		std::string buffer;
		Station station;
		do
		{
			buffer.resize(maxBufferSize);
			in.getline(&buffer[0], maxBufferSize);

			int realSize = strlen(buffer.c_str());
			buffer.resize(realSize);
			buffer = ::RemoveExtraSpaces(buffer);

			try
			{
				station = ::GetSilamStationFromString(buffer);
				itsLocations.push_back(station);
			}
			catch(...)
			{
				// ei tehd‰ mit‰‰n, jatketaan seuraavalla rivill‰ niin kauan kuin rivej‰ riitt‰‰
			}

		}while(in.good());
		itsInitLogMessage = "Initializing silam station data went OK, from file: ";
		itsInitLogMessage += theInitFileName;
/*
		if(itsLocations.Size() == 0)
			itsInitLogMessage += "\nWarning: 0 sounding stations found.";
		else
			itsInitLogMessage += std::string("\nInfo: ") + NFmiStringTools::Convert<int>(itsLocations.Size()) + " sounding stations found.";
*/
	}
	else
		throw std::runtime_error(std::string("NFmiSilamStationList::Init - trouble reading file: ") + theInitFileName);
}

void NFmiSilamStationList::Clear(void)
{
	itsLocations.clear();
}
