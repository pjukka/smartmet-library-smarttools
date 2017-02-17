#include "NFmiFastInfoUtils.h"
#include "NFmiFastQueryInfo.h"
#include "NFmiProducerName.h"

namespace NFmiFastInfoUtils
{
    bool IsInfoShipTypeData(NFmiFastQueryInfo &theInfo)
    {
        if(theInfo.IsGrid() == false)
        {
            FmiProducerName prodId = static_cast<FmiProducerName>(theInfo.Producer()->GetIdent());
            if(prodId == kFmiSHIP || prodId == kFmiBUOY)
                return true;
        }
        return false;
    }

    // synop-dataa ei tarvitsee asettaa mihink‰‰n, mutta
    // mm. luotaus plotin yhteydess‰ pit‰‰ etsi‰ oikea korkeus/level
    void SetSoundingDataLevel(const NFmiLevel &theWantedSoundingPressureLevel, NFmiFastQueryInfo &info)
    {
        if(theWantedSoundingPressureLevel.LevelType() == kFmiSoundingLevel)
        { // sounding plotin yhteydess‰ pit‰‰ asettaa level haluttuun korkeuteen
            float levelValue = theWantedSoundingPressureLevel.LevelValue();
            FmiParameterName parName = static_cast<FmiParameterName>(info.Param().GetParamIdent());
            unsigned long parIndex = info.ParamIndex();
            bool subParaUsed = info.IsSubParamUsed();
            if(info.Param(kFmiPressure))
            {
                for(info.ResetLevel(); info.NextLevel(); )
                    if(info.FloatValue() == levelValue)
                        break;
            }
            if(subParaUsed)
                info.Param(parName); // pakko vet‰‰ t‰m‰ hitaalla tavalla jostain syyst‰
            else
                info.ParamIndex(parIndex);
        }
    }
}
