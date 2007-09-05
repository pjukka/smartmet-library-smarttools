//© Ilmatieteenlaitos/Marko.
//Original 09.10.2006
//
// Luokka pitää MetEditoriin imagine contourauksessa käytettyjen
// xy-pisteiden cachea.
//---------------------------------------------------------- NFmiGridPointCache.cpp

#include "NFmiGridPointCache.h"
#include "NFmiGrid.h"

void NFmiGridPointCache::Add(const std::string &theGridStr, const NFmiGridPointCache::Data &theData)
{
	pointMap::iterator it = itsPointCache.find(theGridStr);
	if(it == itsPointCache.end())
		itsPointCache.insert(std::make_pair(theGridStr, theData));
	else // jos löytyi jo cachesta, korvataan arvot
	{
		(*it).second = theData;
	}
}

const std::string NFmiGridPointCache::MakeGridCacheStr( const NFmiGrid &theGrid )
{
	std::string str(theGrid.Area()->AreaStr());
	str += ":";
	str += NFmiStringTools::Convert(theGrid.XNumber());
	str += ",";
	str += NFmiStringTools::Convert(theGrid.YNumber());

	return str;
}
