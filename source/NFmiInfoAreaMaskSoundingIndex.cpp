// ======================================================================
/*!
 * \file NFmiInfoAreaMaskSoundingIndex.cpp
 * \brief Implementation of class NFmiInfoAreaMaskSoundingIndex
 */
// ======================================================================

#include "NFmiInfoAreaMaskSoundingIndex.h"
#include "NFmiFastQueryInfo.h"

NFmiInfoAreaMaskSoundingIndex::~NFmiInfoAreaMaskSoundingIndex(void)
{
}

NFmiInfoAreaMaskSoundingIndex::NFmiInfoAreaMaskSoundingIndex(boost::shared_ptr<NFmiFastQueryInfo> &theInfo, FmiSoundingParameters theSoundingParam)
:NFmiInfoAreaMask(theInfo, kNoValue)
,itsSoundingParam(theSoundingParam)
{
}

// tätä kaytetaan smarttool-modifierin yhteydessä
double NFmiInfoAreaMaskSoundingIndex::Value(const NFmiPoint & theLatlon, const NFmiMetTime & theTime, int /* theTimeIndex */ , bool /* fUseTimeInterpolationAlways */ )
{
	// RUMAA CAST-koodia!!!!!
	return NFmiSoundingIndexCalculator::Calc(itsInfo.get(), theLatlon, theTime, itsSoundingParam);
}
