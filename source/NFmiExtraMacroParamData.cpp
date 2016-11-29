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
    {
        InitializeResolutionData(theInfoOrganizer, itsGivenResolutionInKm);
    }
    else if(itsProducer.GetIdent() != 0)
    {
        InitializeDataBasedResolutionData(theInfoOrganizer, itsProducer, itsLevelType);
    }
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

static bool IsPrimarySurfaceDataType(boost::shared_ptr<NFmiFastQueryInfo> &info)
{
    NFmiInfoData::Type dataType = info->DataType();
    if(dataType == NFmiInfoData::kViewable || dataType == NFmiInfoData::kObservations || dataType == NFmiInfoData::kKepaData || dataType == NFmiInfoData::kAnalyzeData)
        return true;
    else
        return false;
}

static bool IsPrimaryLevelDataType(boost::shared_ptr<NFmiFastQueryInfo> &info)
{
    NFmiInfoData::Type dataType = info->DataType();
    if(dataType == NFmiInfoData::kViewable || dataType == NFmiInfoData::kHybridData || dataType == NFmiInfoData::kSingleStationRadarData || dataType == NFmiInfoData::kAnalyzeData)
        return true;
    else
        return false;
}

static boost::shared_ptr<NFmiFastQueryInfo> FindWantedInfo(checkedVector<boost::shared_ptr<NFmiFastQueryInfo> > &theInfos, FmiLevelType theLevelType)
{
    boost::shared_ptr<NFmiFastQueryInfo> backupData; // T‰h‰n laitetaan talteen ei prim‰‰ri datatyyppi varmuuden varalle
    bool searchSingleLevelData = (theLevelType == kFmiMeanSeaLevel);
    for(size_t i = 0; i < theInfos.size(); i++)
    {
        boost::shared_ptr<NFmiFastQueryInfo> &info = theInfos[i];
        // Vain hiladatat kelpaavat tarkasteluissa
        if(info->Grid()) 
        {
            if(searchSingleLevelData)
            {
                if(info->SizeLevels() == 1)
                {
                    if(::IsPrimarySurfaceDataType(info))
                        return info; // Palautetaan surface tapauksessa 1. yksi tasoinen 'prim‰‰ri' data
                    else
                        backupData = info;
                }
            }
            else
            {
                if(theLevelType == info->LevelType())
                {
                    if(::IsPrimaryLevelDataType(info))
                        return info; // Palautetaan level tapauksessa 1. 'prim‰‰ri' data
                    else
                        backupData = info;
                }
            }
        }
    }
    return backupData;
}

static std::string GetProducerInfoForResolutionError(const NFmiProducer &theProducer, FmiLevelType theLevelType)
{
    std::string str = theProducer.GetName();
    str += " ";
    if(theLevelType == kFmiMeanSeaLevel)
        str += "surface";
    else if(theLevelType == kFmiPressureLevel)
        str += "pressure";
    else if(theLevelType == kFmiHybridLevel)
        str += "hybrid";
    else if(theLevelType == kFmiHeight)
        str += "height";

    return str;
}

// Oletus theInfo on hilamuotoista dataa, eli silt‰ lˆytyy area.
// Lasketaan hilan x- ja y-resoluutioiden keskiarvo kilometreissa.
static float CalcDataBasedResolutionInKm(boost::shared_ptr<NFmiFastQueryInfo> &theInfo)
{
    double resolutionX = theInfo->Area()->WorldXYWidth() / theInfo->GridXNumber();
    double resolutionY = theInfo->Area()->WorldXYHeight() / theInfo->GridYNumber();
    return static_cast<float>((resolutionX + resolutionY) / (2. * 1000.));
}

void NFmiExtraMacroParamData::InitializeDataBasedResolutionData(NFmiInfoOrganizer &theInfoOrganizer, const NFmiProducer &theProducer, FmiLevelType theLevelType)
{
    checkedVector<boost::shared_ptr<NFmiFastQueryInfo> > infos = theInfoOrganizer.GetInfos(theProducer.GetIdent());
    boost::shared_ptr<NFmiFastQueryInfo> info = ::FindWantedInfo(infos, theLevelType);
    if(info)
    {
        itsDataBasedResolutionInKm = CalcDataBasedResolutionInKm(info);
        InitializeResolutionData(theInfoOrganizer, itsDataBasedResolutionInKm);
    }
    else
        throw std::runtime_error(std::string("Could not find the given 'resolution' data for ") + ::GetProducerInfoForResolutionError(theProducer, theLevelType));
}
