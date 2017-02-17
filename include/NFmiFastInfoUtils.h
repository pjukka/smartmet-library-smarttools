#pragma once

class NFmiFastQueryInfo;
class NFmiLevel;

namespace NFmiFastInfoUtils
{
	bool IsInfoShipTypeData(NFmiFastQueryInfo &theInfo);
    void SetSoundingDataLevel(const NFmiLevel &theWantedSoundingPressureLevel, NFmiFastQueryInfo &info);
}
