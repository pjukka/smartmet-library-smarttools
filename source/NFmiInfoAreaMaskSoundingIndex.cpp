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

NFmiInfoAreaMaskSoundingIndex::NFmiInfoAreaMaskSoundingIndex(NFmiQueryInfo * theInfo, FmiSoundingParameters theSoundingParam, bool ownsInfo, bool destroySmartInfoData)
:NFmiInfoAreaMask(theInfo, ownsInfo, kNoValue, destroySmartInfoData)
,itsSoundingParam(theSoundingParam)
{
}

// tätä kaytetaan smarttool-modifierin yhteydessä
double NFmiInfoAreaMaskSoundingIndex::Value(const NFmiPoint & theLatlon, const NFmiMetTime & theTime, int /* theTimeIndex */ , bool /* fUseTimeInterpolationAlways */ )
{
	// RUMAA CAST-koodia!!!!!
	return NFmiSoundingIndexCalculator::Calc(static_cast<NFmiFastQueryInfo*>(itsInfo), theLatlon, theTime, itsSoundingParam);
}
