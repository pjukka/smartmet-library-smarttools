// ==========================================================================
/*!
 * \file
 * \brief Implementation of NFmiAviationStation and NFmiAviationStationInfoSystem -classes
 */
// ==========================================================================


#include "NFmiAviationStationInfoSystem.h"
#include "NFmiFileSystem.h"
#include "NFmiStringTools.h"

#include <sstream>
#include <vector>

NFmiAviationStation* NFmiAviationStationInfoSystem::FindStation(const std::string &theIcaoId)
{
	if(fWmoStationsWanted == false)
	{
		std::map<std::string, NFmiAviationStation>::iterator it = itsIcaoStations.find(theIcaoId);
		if(it != itsIcaoStations.end())
		{
			return &(it->second);
		}
	}
	return 0;
}

NFmiAviationStation* NFmiAviationStationInfoSystem::FindStation(unsigned long theWmoId)
{
	if(fWmoStationsWanted)
	{
		std::map<unsigned long, NFmiAviationStation>::iterator it = itsWmoStations.find(theWmoId);
		if(it != itsWmoStations.end())
		{
			return &(it->second);
		}
	}
	return 0;
}

// country3;country2;country;region;subregion;city;station_name_current;station_name_special;stn_key;icao_xref;national_id_xref;wmo_xref;wban_xref;iata_xref;status;icao;icao_quality;icao_source;national_id;national_quality;national_id_source;wmo;wmo_quality;wmo_source;maslib;maslib_source;wban;wban_source;special;lat_prp;lon_prp;ll_quality;llsrc;elev_prp;elev_prp_quality;elev_aerodrome_source;elev_baro;elev_baro_quality;elev_baro_source;tz;postal_code;start;start_source;end;end_source;remarks;latgrp;longrp
// FIN;FI;Finland;-;;Helsinki;Vantaa;;FIaaEFHK;EFHK;;2974;;HEL;m;EFHK;A;ICA09;;;;2974;A;WMO11;29740;;;;;60.31666667;24.96666667;G;WMO11 WMO10;51;C;WMO11 WMO10;56;C;WMO11 WMO10;Europe/Helsinki;FI-01560;;;;;;60;24
static bool GetAviationStationFromCsvString(const std::string &theStationStr, NFmiAviationStation &theStationOut, bool fIcaoNeeded, bool fWmoNeeded)
{
	static unsigned long currentWmoIdCounter = 128000;
	if(theStationStr.size() > 2)
	{
		if(theStationStr[0] == '#')
			return false;
		if(theStationStr[0] == '/' && theStationStr[1] == '/')
			return false;

		std::vector<std::string> stationParts = NFmiStringTools::Split(theStationStr, ";");
		if(stationParts.size() >= 30)
		{
			bool latlonOk = false;
			bool icaoOk = false;
			bool wmoOk = false;
			std::string nameStr = stationParts[5];
			std::string icaoStr = stationParts[15];
			const unsigned long missingWmoId = 9999999;
			unsigned long wmoId = missingWmoId;
			double lat = -9999;
			double lon = -9999;
			if(icaoStr.size() == 4)
				icaoOk = true;
			try
			{
				wmoId = NFmiStringTools::Convert<unsigned long>(stationParts[21]);
				wmoOk = true;
			}
			catch(...)
			{}

			try
			{
				lat = NFmiStringTools::Convert<double>(stationParts[29]);
				lon = NFmiStringTools::Convert<double>(stationParts[30]);
				latlonOk = true;
			}
			catch(...)
			{}

			if(latlonOk && (fIcaoNeeded == false || fIcaoNeeded && icaoOk) && (fWmoNeeded == false || fWmoNeeded && wmoOk))
			{
				if(wmoId == missingWmoId)
					wmoId = currentWmoIdCounter++;
				theStationOut.SetIdent(wmoId);
				theStationOut.SetLatitude(lat);
				theStationOut.SetLongitude(lon);
				theStationOut.IcaoStr(icaoStr);
				theStationOut.SetName(nameStr);
				return true;
			}
		}
	}
	return false;
}

void NFmiAviationStationInfoSystem::InitFromMasterTableCsv(const std::string &theInitFileName)
{
	itsInitLogMessage = "";
	 // tyhjennet‰‰n ensin asemalistat
	itsIcaoStations.clear();
	itsWmoStations.clear();

	if(theInitFileName.empty())
		throw std::runtime_error("NFmiAviationStationInfoSystem::InitFromMasterTableCsv - empty settings filename given.");

	std::string fileContent;
	if(NFmiFileSystem::ReadFile2String(theInitFileName, fileContent))
	{
		std::stringstream in(fileContent);

		const int maxBufferSize = 1024+1; // kuinka pitk‰ yhden rivin maksimissaan oletetaan olevan
		std::string buffer;
		int i = 0;
		int counter = 0;
		do
		{
			buffer.resize(maxBufferSize);
			in.getline(&buffer[0], maxBufferSize);
			size_t realSize = strlen(buffer.c_str());
			buffer.resize(realSize);

			try
			  {
				NFmiAviationStation station;
				if(::GetAviationStationFromCsvString(buffer, station, fWmoStationsWanted == false, fWmoStationsWanted == true))
				  {
					counter++;
					if(fWmoStationsWanted)
						itsWmoStations.insert(std::make_pair(station.GetIdent(), station));
					else
						itsIcaoStations.insert(std::make_pair(station.IcaoStr(), station));
				  }
			  }
			catch(std::runtime_error & /* e */ )
			  {
			  }

			i++;
		}while(in.good());

		if(fVerboseMode)
		{
			int totalStationCount = static_cast<int>(fWmoStationsWanted ? itsWmoStations.size() : itsIcaoStations.size());
			itsInitLogMessage = "Initializing sounding station data went OK, from file: ";
			itsInitLogMessage += theInitFileName;
			if(totalStationCount == 0)
				itsInitLogMessage += "\nWarning: 0 icao stations found.";
			else
				itsInitLogMessage += std::string("\nInfo: ") + NFmiStringTools::Convert<int>(totalStationCount) + " icao stations found.";
			int diff = counter - totalStationCount;
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


// ======================================================================
