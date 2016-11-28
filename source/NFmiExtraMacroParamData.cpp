#include "NFmiExtraMacroParamData.h"
#include "NFmiFastQueryInfo.h"
#include "NFmiInfoOrganizer.h"

NFmiExtraMacroParamData::NFmiExtraMacroParamData()
:itsGivenResolutionInKm(kFloatMissing)
,itsProducer()
,itsLevelType(kFmiNoLevelType)
,itsDataBasedResolutionInKm(kFloatMissing)
,itsResolutionMacroParamData()
{
}

void NFmiExtraMacroParamData::FinalizeData(NFmiInfoOrganizer &theInfoOrganizer)
{
    if(itsGivenResolutionInKm != kFloatMissing)
        InitializeResolutionData(theInfoOrganizer, itsGivenResolutionInKm);
    //else if(itsProducer.GetIdent() != 0)
    //{
    //    // Alusta datapohjainen resoluutio
    //}

}

bool NFmiExtraMacroParamData::UseSpecialResolution() const
{
    return itsResolutionMacroParamData != 0;
}

void NFmiExtraMacroParamData::SetUsedAreaForData(boost::shared_ptr<NFmiFastQueryInfo> &theData, const NFmiArea *theUsedArea)
{
    if(theData->Grid() && theUsedArea)
    {
        try
        {
            NFmiGrid grid(theUsedArea, theData->Grid()->XNumber(), theData->Grid()->YNumber());
            grid.Area()->SetXYArea(NFmiRect(0, 0, 1, 1)); // t‰ss‰ pit‰‰ laittaa xy-alue 0,0 - 1,1 :ksi, koska macroParam datat zoomataan sitten erikseen
            NFmiHPlaceDescriptor hplace(grid);
            theData->SetHPlaceDescriptor(hplace);
        }
        catch(...)
        {
            // ei tehd‰ mit‰‰n, otetaan vain poikkeukset kiinni
        }
    }
}

static void CalcUsedGridSize(const NFmiArea *usedArea, int &gridSizeX, int &gridSizeY, float usedResolutionInKm)
{
    if(usedArea)
    {
        gridSizeX = boost::math::iround(usedArea->WorldXYWidth() / (usedResolutionInKm*1000.));
        gridSizeY = boost::math::iround(usedArea->WorldXYHeight() / (usedResolutionInKm*1000.));
    }
}

void NFmiExtraMacroParamData::InitializeResolutionData(NFmiInfoOrganizer &theInfoOrganizer, float usedResolutionInKm)
{
    int gridSizeX = 0;
    int gridSizeY = 0;
    const NFmiArea *usedArea = theInfoOrganizer.MacroParamData()->Area();
    ::CalcUsedGridSize(usedArea, gridSizeX, gridSizeY, usedResolutionInKm);

    itsResolutionMacroParamData = theInfoOrganizer.CreateNewMacroParamData(gridSizeX, gridSizeY, NFmiInfoData::kMacroParam);
    // Pit‰‰ viel‰ s‰‰t‰‰ datan alue kartan zoomaus alueeseen. Se saadaan infoOrganizerin omasta macroParamDatasta.
    SetUsedAreaForData(itsResolutionMacroParamData, usedArea);
}

void NFmiExtraMacroParamData::AdjustValueMatrixToMissing(const boost::shared_ptr<NFmiFastQueryInfo> &theData, NFmiDataMatrix<float> &theValueMatrix)
{
    if(theData)
    {
        const NFmiGrid *grid = theData->Grid();
        if(grid)
        {
            theValueMatrix.Resize(grid->XNumber(),grid->YNumber());
            theValueMatrix = kFloatMissing;
        }
    }
}
