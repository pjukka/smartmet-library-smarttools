//� Ilmatieteenlaitos/Marko.
//Original 09.10.2006
//
// Luokka pit�� MetEditoriin imagine contourauksessa k�ytettyjen
// xy-pisteiden cachea.
//---------------------------------------------------------- NFmiGridPointCache.cpp

#include "NFmiGridPointCache.h"

void NFmiGridPointCache::Add(const std::string &theGridStr, const NFmiGridPointCache::Data &theData)
{
	pointMap::iterator it = itsPointCache.find(theGridStr);
	if(it == itsPointCache.end())
		itsPointCache.insert(std::make_pair(theGridStr, theData));
	else // jos l�ytyi jo cachesta, korvataan arvot
	{
		(*it).second = theData;
	}
}
