// NFmiHelpDataInfo.cpp

#ifdef _MSC_VER
#pragma warning(disable : 4996) // 4996 poistaa ep‰turvallisten string manipulaatio funktioiden k‰ytˆst‰ tulevat varoitukset. En aio k‰ytt‰‰ ehdotettuja turvallisia _s -funktioita (esim. sprintf_s), koska ne eiv‰t ole linux yhteensopivia.
#endif

#include "NFmiHelpDataInfo.h"
#include <NFmiArea.h>
#include <NFmiAreaFactory.h>
#include <NFmiStereographicArea.h>
#include <NFmiSettings.h>
#include <NFmiFileString.h>

using namespace std;

// ----------------------------------------------------------------------
/*!
 *  syˆ spacet pois streamista ja palauttaa true:n jos ei olla lopussa
 *
 * \param theInput The input stream
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiHelpDataInfo::NFmiHelpDataInfo(void)
:itsName()
,itsFileNameFilter()
,itsPartialDataCacheFileNameFilter()
,fForceFileFilterName(false)
,itsLatestFileName()
,itsLatestErroneousFileName()
,itsDataType(NFmiInfoData::kNoDataType)
,itsLatestFileTimeStamp(0)
,itsFakeProducerId(0)
,itsImageProjectionString()
,itsImageDataIdent()
,itsImageArea()
,fNotifyOnLoad(false)
,itsNotificationLabel()
,itsCustomMenuFolder()
,itsReportNewDataTimeStepInMinutes(0)
,itsReportNewDataLabel()
,itsCombineDataPathAndFileName()
,itsCombineDataMaxTimeSteps(0)
,fMakeSoundingIndexData(false)
,itsBaseNameSpace()
,itsAdditionalArchiveFileCount(0)
,fEnable(true)
,fNonFixedTimeGab(false)
,itsModelRunTimeGapInHours(0)
{}

NFmiHelpDataInfo::NFmiHelpDataInfo(const NFmiHelpDataInfo &theOther)
:itsName(theOther.itsName)
,itsFileNameFilter(theOther.itsFileNameFilter)
,itsPartialDataCacheFileNameFilter(theOther.itsPartialDataCacheFileNameFilter)
,fForceFileFilterName(theOther.fForceFileFilterName)
,itsLatestFileName(theOther.itsLatestFileName)
,itsLatestErroneousFileName(theOther.itsLatestErroneousFileName)
,itsDataType(theOther.itsDataType)
,itsLatestFileTimeStamp(theOther.itsLatestFileTimeStamp)
,itsFakeProducerId(theOther.itsFakeProducerId)
,itsImageProjectionString(theOther.itsImageProjectionString)
,itsImageDataIdent(theOther.itsImageDataIdent)
,itsImageArea(theOther.itsImageArea ? theOther.itsImageArea->Clone() : 0)
,fNotifyOnLoad(theOther.fNotifyOnLoad)
,itsNotificationLabel(theOther.itsNotificationLabel)
,itsCustomMenuFolder(theOther.itsCustomMenuFolder)
,itsReportNewDataTimeStepInMinutes(theOther.itsReportNewDataTimeStepInMinutes)
,itsReportNewDataLabel(theOther.itsReportNewDataLabel)
,itsCombineDataPathAndFileName(theOther.itsCombineDataPathAndFileName)
,itsCombineDataMaxTimeSteps(theOther.itsCombineDataMaxTimeSteps)
,fMakeSoundingIndexData(theOther.fMakeSoundingIndexData)
,itsBaseNameSpace(theOther.itsBaseNameSpace)
,itsAdditionalArchiveFileCount(theOther.itsAdditionalArchiveFileCount)
,fEnable(theOther.fEnable)
,fNonFixedTimeGab(theOther.fNonFixedTimeGab)
,itsModelRunTimeGapInHours(theOther.itsModelRunTimeGapInHours)
{}

NFmiHelpDataInfo& NFmiHelpDataInfo::operator=(const NFmiHelpDataInfo &theOther)
{
	if(this != &theOther)
	{
		Clear(); // l‰hinn‰ area-otuksen tuhoamista varten kutsutaan
		itsName = theOther.itsName;
		itsFileNameFilter = theOther.itsFileNameFilter;
		itsPartialDataCacheFileNameFilter = theOther.itsPartialDataCacheFileNameFilter;
		fForceFileFilterName = theOther.fForceFileFilterName;
		itsLatestFileName = theOther.itsLatestFileName;
		itsLatestErroneousFileName = theOther.itsLatestErroneousFileName;
		itsDataType = theOther.itsDataType;
		itsLatestFileTimeStamp = theOther.itsLatestFileTimeStamp;
		itsFakeProducerId = theOther.itsFakeProducerId;
		itsImageProjectionString = theOther.itsImageProjectionString;
		itsImageDataIdent = theOther.itsImageDataIdent;
		if(theOther.itsImageArea)
		  itsImageArea.reset(theOther.itsImageArea->Clone());
		fNotifyOnLoad = theOther.fNotifyOnLoad;
		itsNotificationLabel = theOther.itsNotificationLabel;
		itsCustomMenuFolder = theOther.itsCustomMenuFolder;
		itsReportNewDataTimeStepInMinutes = theOther.itsReportNewDataTimeStepInMinutes;
		itsReportNewDataLabel = theOther.itsReportNewDataLabel;
		itsCombineDataPathAndFileName = theOther.itsCombineDataPathAndFileName;
		itsCombineDataMaxTimeSteps = theOther.itsCombineDataMaxTimeSteps;
		fMakeSoundingIndexData = theOther.fMakeSoundingIndexData;
		itsAdditionalArchiveFileCount = theOther.itsAdditionalArchiveFileCount;
		fEnable = theOther.fEnable;
		fNonFixedTimeGab = theOther.fNonFixedTimeGab;
		itsModelRunTimeGapInHours = theOther.itsModelRunTimeGapInHours;

		itsBaseNameSpace = theOther.itsBaseNameSpace;
	}
	return *this;
}

void NFmiHelpDataInfo::Clear(void)
{
	itsName = "";
	itsFileNameFilter = "";
	itsPartialDataCacheFileNameFilter = "";
	fForceFileFilterName = false;
	itsLatestFileName = "";
	itsLatestErroneousFileName = "";
	itsDataType = NFmiInfoData::kNoDataType;
	itsLatestFileTimeStamp = 0;
	itsFakeProducerId = 0;
	itsImageProjectionString = "";
	itsImageDataIdent = NFmiDataIdent();
	itsImageArea.reset();
	fNotifyOnLoad = false;
	itsNotificationLabel = "";
	itsCustomMenuFolder = "";
	itsBaseNameSpace = "";
	itsReportNewDataTimeStepInMinutes = 0;
	itsReportNewDataLabel = "";
	itsCombineDataPathAndFileName = "";
	itsCombineDataMaxTimeSteps = 0;
	fMakeSoundingIndexData = false;
	itsAdditionalArchiveFileCount = 0;
	fEnable = true;
	fNonFixedTimeGab = false;
	itsModelRunTimeGapInHours = 0;
}

static void FixPathEndWithSeparator(std::string &theFixedPathStr)
{
	if(theFixedPathStr.empty() == false)
	{
        NFmiFileString tmpFileStr(theFixedPathStr);
		tmpFileStr.NormalizeDelimiter(); // varmistetaan myˆs ett‰ polun merkit ovat oikein p‰in
		theFixedPathStr = static_cast<char*>(tmpFileStr);

		std::string::value_type lastLetter = theFixedPathStr[theFixedPathStr.size()-1];
		if(lastLetter != kFmiDirectorySeparator)
			theFixedPathStr.push_back(kFmiDirectorySeparator);
	}
}

static void FixPatternSeparators(std::string &theFixedPatternStr)
{
	if(theFixedPatternStr.empty() == false)
	{
	    NFmiFileString tmpFileStr(theFixedPatternStr);
		tmpFileStr.NormalizeDelimiter(); // varmistetaan ett‰ polun merkit ovat oikein p‰in
		theFixedPatternStr = static_cast<char*>(tmpFileStr);
	}
}

static void MakeCombinedDataFilePattern(NFmiHelpDataInfo &theDataInfo, const NFmiHelpDataInfoSystem &theHelpDataSystem)
{
	// combineDataPattern += "FileNameFilter:ist‰ osa jossa on mukana ylin hakemistotaso"
	// esim. "P:\data\partial_data\laps\*_LAPS_finland.sqd" -> "laps\*_LAPS_finland.sqd"
	std::string lastDirFilePattern = theDataInfo.FileNameFilter();
	if(lastDirFilePattern.empty() == false)
	{
		int slashesFound = 0;
		size_t i = lastDirFilePattern.size()-1;
		for( ; i > 0; i--)
		{
			if(lastDirFilePattern[i] == '\\')
				slashesFound++;
			if(slashesFound > 1)
				break;
		}

		if(slashesFound > 1)
		{
			std::string combineDataPattern(theHelpDataSystem.CachePartialDataDirectory());
			combineDataPattern += std::string(lastDirFilePattern.begin() + i + 1, lastDirFilePattern.end());
			theDataInfo.PartialDataCacheFileNameFilter(combineDataPattern);
		}
	}
}

void NFmiHelpDataInfo::InitFromSettings(const std::string &theBaseKey, const std::string &theName, const NFmiHelpDataInfoSystem &theHelpDataSystem)
{
	fForceFileFilterName = false;
	itsName = theName;
	itsBaseNameSpace = theBaseKey + "::" + theName;

	std::string fileNameFilterKey = itsBaseNameSpace + "::FilenameFilter";
	if(NFmiSettings::IsSet(fileNameFilterKey))
	{
		// Read configuration
		itsFileNameFilter = NFmiSettings::Require<std::string>(fileNameFilterKey);
		::FixPatternSeparators(itsFileNameFilter);
		itsDataType = static_cast<NFmiInfoData::Type> (NFmiSettings::Require<int>(itsBaseNameSpace + "::DataType"));
		itsFakeProducerId = NFmiSettings::Optional<int>(itsBaseNameSpace + "::ProducerId", 0);
		fNotifyOnLoad = NFmiSettings::Optional<bool>(itsBaseNameSpace + "::NotifyOnLoad", false);
		itsNotificationLabel = NFmiSettings::Optional<string>(itsBaseNameSpace + "::NotificationLabel", "");
		itsCustomMenuFolder = NFmiSettings::Optional<string>(itsBaseNameSpace + "::CustomMenuFolder", "");
		itsReportNewDataTimeStepInMinutes = NFmiSettings::Optional<int>(itsBaseNameSpace + "::ReportNewDataTimeStepInMinutes", 0);
		itsReportNewDataLabel = NFmiSettings::Optional<string>(itsBaseNameSpace + "::ReportNewDataLabel", "");
		itsCombineDataPathAndFileName = NFmiSettings::Optional<string>(itsBaseNameSpace + "::CombineDataPathAndFileName", "");
		itsCombineDataMaxTimeSteps = NFmiSettings::Optional<int>(itsBaseNameSpace + "::CombineDataMaxTimeSteps", 0);
		fMakeSoundingIndexData = NFmiSettings::Optional<bool>(itsBaseNameSpace + "::MakeSoundingIndexData", false);
		itsAdditionalArchiveFileCount = NFmiSettings::Optional<int>(itsBaseNameSpace + "::AdditionalArchiveFileCount", 0);
		fEnable = NFmiSettings::Require<bool>(itsBaseNameSpace + "::Enable");
		fNonFixedTimeGab = NFmiSettings::Optional<bool>(itsBaseNameSpace + "::NonFixedTimeGab", false);
		itsModelRunTimeGapInHours = NFmiSettings::Optional<float>(itsBaseNameSpace + "::ModelRunTimeGapInHours", 0);

		if(IsCombineData())
			::MakeCombinedDataFilePattern(*this, theHelpDataSystem);

		std::string imageProjectionKey(itsBaseNameSpace + "::ImageProjection");
		if (NFmiSettings::IsSet(imageProjectionKey))
		{
		  boost::shared_ptr<NFmiArea> area = NFmiAreaFactory::Create(NFmiSettings::Require<std::string>(imageProjectionKey));
			if(area)
			{
				if(area->XYArea().Width() != 1 || area->XYArea().Height() != 1)
				{
					area->SetXYArea(NFmiRect(0,0,1,1));
				}
				itsImageArea = area;
			}
			NFmiParam param(NFmiSettings::Require<int>(itsBaseNameSpace + "::ParameterId")
						   ,NFmiSettings::Require<std::string>(itsBaseNameSpace + "::ParameterName"));
			itsImageDataIdent = NFmiDataIdent(param, NFmiProducer(itsFakeProducerId));
		}
	}
}

void NFmiHelpDataInfo::ImageArea(boost::shared_ptr<NFmiArea> &newValue) 
{ 
  itsImageArea = newValue;
}

static std::string MakeCacheFilePattern(const NFmiHelpDataInfo &theDataInfo, const NFmiHelpDataInfoSystem &theHelpDataSystem)
{
	NFmiFileString fileStr(theDataInfo.FileNameFilter());
	std::string cachePattern(theHelpDataSystem.CacheDirectory());
	cachePattern += static_cast<char*>(fileStr.FileName());
	return cachePattern;
}

void NFmiHelpDataInfo::FileNameFilter(const std::string &newValue, bool forceFileNameFilter) 
{
	itsFileNameFilter = newValue;
	fForceFileFilterName = forceFileNameFilter;
}

// t‰m‰ on viritys, ett‰ olisi funktio, jolla voidaan pyyt‰‰ k‰ytetty fileFilter, riippuen siit‰ onko cache k‰ytˆss‰ vai ei
const std::string NFmiHelpDataInfo::UsedFileNameFilter(const NFmiHelpDataInfoSystem &theHelpDataInfoSystem) const
{
	if(fForceFileFilterName || theHelpDataInfoSystem.UseQueryDataCache() == false || itsDataType == NFmiInfoData::kStationary)
		return FileNameFilter();
	else
	{
		if(itsCombineDataPathAndFileName.empty())
			return ::MakeCacheFilePattern(*this, theHelpDataInfoSystem);
		else
			return PartialDataCacheFileNameFilter();
	}
}

bool NFmiHelpDataInfo::operator==(const NFmiHelpDataInfo &other) const
{
	if(itsName != other.itsName)
        return false;
	if(itsFileNameFilter != other.itsFileNameFilter)
        return false;
	if(itsPartialDataCacheFileNameFilter != other.itsPartialDataCacheFileNameFilter)
        return false;
	if(fForceFileFilterName != other.fForceFileFilterName)
        return false;
	// itsLatestFileName; // HUOM! T‰t‰ muuttujaa ei tarkastella, tarkoitus on varsinaisesti tarkastella onko asetukset muuttuneet
	// itsLatestErroneousFileName; // HUOM! T‰t‰ muuttujaa ei tarkastella, tarkoitus on varsinaisesti tarkastella onko asetukset muuttuneet
	if(itsDataType != other.itsDataType)
        return false;
	// itsLatestFileTimeStamp; // HUOM! T‰t‰ muuttujaa ei tarkastella, tarkoitus on varsinaisesti tarkastella onko asetukset muuttuneet
	if(itsFakeProducerId != other.itsFakeProducerId)
        return false;
	if(itsImageProjectionString != other.itsImageProjectionString)
        return false;
	if(!(itsImageDataIdent == other.itsImageDataIdent))
        return false;
    // itsImageArea; // HUOM! T‰t‰ muuttujaa ei tarkastella, SmartMetilla ei voi muuttaa t‰t‰ (sit‰ t‰ss‰ oikeasti halutaan tarkastella), ja NFmiArea -olioita ei voi verrata oikeni ilman jotain suurempaa virityst‰, koska kyse on polymorphisesta luokka rakenteesta.
	if(fNotifyOnLoad != other.fNotifyOnLoad)
        return false;
	if(itsNotificationLabel != other.itsNotificationLabel)
        return false;
	if(itsCustomMenuFolder != other.itsCustomMenuFolder)
        return false;
	if(itsReportNewDataTimeStepInMinutes != other.itsReportNewDataTimeStepInMinutes)
        return false;
	if(itsReportNewDataLabel != other.itsReportNewDataLabel)
        return false;
	if(itsCombineDataPathAndFileName != other.itsCombineDataPathAndFileName)
        return false;
	if(itsCombineDataMaxTimeSteps != other.itsCombineDataMaxTimeSteps)
        return false;
	if(fMakeSoundingIndexData != other.fMakeSoundingIndexData)
        return false;
	if(itsBaseNameSpace != other.itsBaseNameSpace)
        return false;
	if(itsAdditionalArchiveFileCount != other.itsAdditionalArchiveFileCount)
        return false;
	if(fEnable != other.fEnable)
        return false;
	if(fNonFixedTimeGab != other.fNonFixedTimeGab)
        return false;
	if(itsModelRunTimeGapInHours != other.itsModelRunTimeGapInHours)
        return false;

    return true;
}

bool NFmiHelpDataInfo::operator!=(const NFmiHelpDataInfo &other) const
{
    return !(*this == other);
}

// ***********************************************
// ** T‰st‰ alkaa NFmiHelpDataInfoSystem osio ****
// ***********************************************

NFmiHelpDataInfo& NFmiHelpDataInfoSystem::DynamicHelpDataInfo(int theIndex)
{
	static NFmiHelpDataInfo dummy;
	if(!itsDynamicHelpDataInfos.empty() && theIndex >= 0 && theIndex < static_cast<int>(itsDynamicHelpDataInfos.size()))
		return itsDynamicHelpDataInfos[theIndex];
	return dummy;
}
NFmiHelpDataInfo& NFmiHelpDataInfoSystem::StaticHelpDataInfo(int theIndex)
{
	static NFmiHelpDataInfo dummy;
	if(!itsStaticHelpDataInfos.empty() && theIndex >= 0 && theIndex < static_cast<int>(itsStaticHelpDataInfos.size()))
		return itsStaticHelpDataInfos[theIndex];
	return dummy;
}

// Etsii seuraavan satel-kuvan kanavan. Pit‰‰ olla sama tuottaja.
// Palauttaa uuden dataidentin, miss‰ uusi parametri.
NFmiDataIdent NFmiHelpDataInfoSystem::GetNextSatelChannel(const NFmiDataIdent &theDataIdent, FmiDirection theDir)
{
	NFmiDataIdent returnDataIdent(theDataIdent);
	FmiProducerName prodId = static_cast<FmiProducerName>(theDataIdent.GetProducer()->GetIdent());
	size_t count = itsDynamicHelpDataInfos.size();
	std::vector<NFmiDataIdent> dataIdentVec;
	int counter = 0;
	int currentIndex = -1;
	for(size_t i=0; i<count; i++)
	{
		if(itsDynamicHelpDataInfos[i].DataType() == NFmiInfoData::kSatelData)
		{
			if(prodId == itsDynamicHelpDataInfos[i].ImageDataIdent().GetProducer()->GetIdent())
			{
				dataIdentVec.push_back(itsDynamicHelpDataInfos[i].ImageDataIdent());
				if(theDataIdent.GetParamIdent() == itsDynamicHelpDataInfos[i].ImageDataIdent().GetParamIdent())
					currentIndex = counter; // laitetaan currentti param index talteen
				counter++;
			}
		}
	}
	if(counter > 1)
	{
		if(theDir == kForward)
			currentIndex++;
		else
			currentIndex--;
		if(currentIndex < 0)
			currentIndex = counter-1;
		if(currentIndex >= counter)
			currentIndex = 0;
		returnDataIdent = dataIdentVec[currentIndex];
	}
	return returnDataIdent;
}


// etsii sateldata-tyypit (oikeasti imagedatatype) l‰pi ja etsii haluttua parametria
// jolle lˆytyy ns. piirrett‰v‰ projektio. Palauttaa kyseisen datan projektion
// ja filepatternin.
// Jos fDemandMatchingArea on true, pit‰‰ areoiden sopia niin ett‰ piirto onnistuu blittaamalla.
// jos false, palautetaan area ja filepattern jos datatyyppi ja parametri ovat halutut.
boost::shared_ptr<NFmiArea> NFmiHelpDataInfoSystem::GetDataFilePatternAndArea(NFmiInfoData::Type theDataType, FmiProducerName theProdId, FmiParameterName theParamId, const boost::shared_ptr<NFmiArea> &theZoomedArea, std::string &theFilePattern, bool fDemandMatchingArea)
{
	if(theDataType == NFmiInfoData::kSatelData)
	{
		size_t count = itsDynamicHelpDataInfos.size();
		for(size_t i=0; i<count; i++)
		{
			if(itsDynamicHelpDataInfos[i].DataType() == theDataType)
			{
				if(theProdId == itsDynamicHelpDataInfos[i].ImageDataIdent().GetProducer()->GetIdent() &&
				   static_cast<unsigned long>(theParamId) == itsDynamicHelpDataInfos[i].ImageDataIdent().GetParamIdent())
				{
					if(fDemandMatchingArea && IsSameTypeProjections(theZoomedArea, itsDynamicHelpDataInfos[i].ImageArea()))
					{
						theFilePattern = itsDynamicHelpDataInfos[i].FileNameFilter();
						return itsDynamicHelpDataInfos[i].ImageArea();
					}
					else if(fDemandMatchingArea == false)
					{
						theFilePattern = itsDynamicHelpDataInfos[i].FileNameFilter();
						return itsDynamicHelpDataInfos[i].ImageArea();
					}
			}
			}
		}
	}
	return boost::shared_ptr<NFmiArea>();
}

// Ovatko kaksi annettua projektiota siin‰ mieless‰ samanlaisia, ett‰ 
// voidaan sijoittaa rasterikuva oikein toisen rasterikuvan p‰‰lle.
// Eli palauta false = ei voi piirt‰‰ ja true = voi, 
// jos projektio luokat ovat samoja ja jos kyseess‰ oli stereographic areat, niiden
// orientaatiot pit‰‰ olla samat.
bool NFmiHelpDataInfoSystem::IsSameTypeProjections(const boost::shared_ptr<NFmiArea> &theFirst, const boost::shared_ptr<NFmiArea> &theSecond)
{
	if(theFirst && theSecond)
	{
		if(theFirst->ClassId() == theSecond->ClassId() && theFirst->ClassId() == kNFmiStereographicArea)
		{
            if(static_cast<const NFmiStereographicArea*>(theFirst.get())->Orientation() == static_cast<const NFmiStereographicArea*>(theSecond.get())->Orientation())
				return true;
			else
				return false;
		}
		else if(theFirst->ClassId() == theSecond->ClassId())
			return true;
	}
	return false;
}

void NFmiHelpDataInfoSystem::AddDynamic(const NFmiHelpDataInfo &theInfo)
{
	itsDynamicHelpDataInfos.push_back(theInfo);
}

void NFmiHelpDataInfoSystem::AddStatic(const NFmiHelpDataInfo &theInfo)
{
	itsStaticHelpDataInfos.push_back(theInfo);
}

void NFmiHelpDataInfoSystem::InitDataType(const std::string &theBaseKey, checkedVector<NFmiHelpDataInfo> &theHelpDataInfos, bool fStaticData)
{
	std::vector<std::string> dataKeys = NFmiSettings::ListChildren(theBaseKey);
	std::vector<std::string>::iterator iter = dataKeys.begin();
	for( ; iter != dataKeys.end(); ++iter)
	{
		NFmiHelpDataInfo hdi;
		hdi.InitFromSettings(theBaseKey, *iter, *this);
		if(fStaticData)
			hdi.ForceFileFilterName(true); // varmistetaan ett‰ staattisia datoja ei yritet‰ lukea lokaali cachesta

        // HelpDataInfolla pit‰‰ olla tyyppi, muuten sit‰ ei lis‰t‰ listaan. Kun vuoden 2013 lopussa tehtiin erillinen lista 
        // eri datojen enable-ominaisuudesta yhteen konffitiedostoon (mm. helpdatainfo_enable_data_fmi_heavy.conf), 
        // tuli mahdolliseksi, ett‰ t‰ss‰ tuli ns. haamu dataInfoja, jotka nyt pit‰‰ karsia.
        if(hdi.DataType() != NFmiInfoData::kNoDataType) 
    		theHelpDataInfos.push_back(hdi);
	}
}

void NFmiHelpDataInfoSystem::InitFromSettings(const std::string &theBaseNameSpaceStr, std::string theHelpEditorFileNameFilter, std::string theHelpDataName)
{
	itsBaseNameSpace = theBaseNameSpaceStr;
	itsCacheDirectory = NFmiSettings::Require<std::string>(itsBaseNameSpace + "::CacheDirectory");
	::FixPathEndWithSeparator(itsCacheDirectory);
	itsCacheTmpDirectory = NFmiSettings::Require<std::string>(itsBaseNameSpace + "::CacheTmpDirectory");
	::FixPathEndWithSeparator(itsCacheTmpDirectory);
	itsCachePartialDataDirectory = NFmiSettings::Require<std::string>(itsBaseNameSpace + "::CachePartialDataDirectory");
	::FixPathEndWithSeparator(itsCachePartialDataDirectory);
	itsCacheTmpFileNameFix = NFmiSettings::Require<std::string>(itsBaseNameSpace + "::CacheTmpFileNameFix");
	fUseQueryDataCache = NFmiSettings::Require<bool>(itsBaseNameSpace + "::UseQueryDataCache");

	fDoCleanCache = NFmiSettings::Require<bool>(itsBaseNameSpace + "::DoCleanCache");
	itsCacheFileKeepMaxDays = NFmiSettings::Require<float>(itsBaseNameSpace + "::CacheFileKeepMaxDays");
	itsCacheMaxFilesPerPattern = NFmiSettings::Require<int>(itsBaseNameSpace + "::CacheMaxFilesPerPattern");

	itsCacheMediumFileSizeMB = NFmiSettings::Require<double>(itsBaseNameSpace + "::CacheMediumFileSizeMB");
	itsCacheLargeFileSizeMB = NFmiSettings::Require<double>(itsBaseNameSpace + "::CacheLargeFileSizeMB");
	itsCacheMaximumFileSizeMB = NFmiSettings::Require<double>(itsBaseNameSpace + "::CacheMaximumFileSizeMB");

	// Read static helpdata configurations
	InitDataType(itsBaseNameSpace + "::Static", itsStaticHelpDataInfos, true);

	// Read dynamic helpdata configurations
	InitDataType(itsBaseNameSpace + "::Dynamic", itsDynamicHelpDataInfos, false);

	// Lis‰t‰‰n help editor mode datan luku jos niin on haluttu
	if(theHelpEditorFileNameFilter.empty() == false)
	{
		NFmiHelpDataInfo helpDataInfo;
		helpDataInfo.Name(theHelpDataName);
		helpDataInfo.FileNameFilter(theHelpEditorFileNameFilter);
		helpDataInfo.DataType(NFmiInfoData::kEditingHelpData);
		AddDynamic(helpDataInfo);
	}
}

void NFmiHelpDataInfoSystem::StoreToSettings(void)
{
	if(itsBaseNameSpace.empty() == false)
	{
		// HUOM! t‰ss‰ on toistaiseksi vain cacheen liittyvien muutosten talletukset
		NFmiSettings::Set(std::string(itsBaseNameSpace + "::CacheDirectory"), itsCacheDirectory);
		NFmiSettings::Set(std::string(itsBaseNameSpace + "::CacheTmpDirectory"), itsCacheTmpDirectory);
		NFmiSettings::Set(std::string(itsBaseNameSpace + "::CachePartialDataDirectory"), itsCachePartialDataDirectory);
		NFmiSettings::Set(std::string(itsBaseNameSpace + "::CacheTmpFileNameFix"), itsCacheTmpFileNameFix);
		NFmiSettings::Set(std::string(itsBaseNameSpace + "::UseQueryDataCache"), NFmiStringTools::Convert(fUseQueryDataCache));
		NFmiSettings::Set(std::string(itsBaseNameSpace + "::DoCleanCache"), NFmiStringTools::Convert(fDoCleanCache));
		NFmiSettings::Set(std::string(itsBaseNameSpace + "::CacheFileKeepMaxDays"), NFmiStringTools::Convert(itsCacheFileKeepMaxDays));
		NFmiSettings::Set(std::string(itsBaseNameSpace + "::CacheMaxFilesPerPattern"), NFmiStringTools::Convert(itsCacheMaxFilesPerPattern));
		NFmiSettings::Set(std::string(itsBaseNameSpace + "::CacheMediumFileSizeMB"), NFmiStringTools::Convert(itsCacheMediumFileSizeMB));
		NFmiSettings::Set(std::string(itsBaseNameSpace + "::CacheLargeFileSizeMB"), NFmiStringTools::Convert(itsCacheLargeFileSizeMB));
		NFmiSettings::Set(std::string(itsBaseNameSpace + "::CacheMaximumFileSizeMB"), NFmiStringTools::Convert(itsCacheMaximumFileSizeMB));
	}
	else
		throw std::runtime_error("Error in NFmiHelpDataInfoSystem::StoreToSettings, unable to store setting.");
}

bool NFmiHelpDataInfoSystem::AreStoredSettingsChanged(const NFmiHelpDataInfoSystem &other)
{
	if(itsCacheDirectory != other.itsCacheDirectory)
        return true;
	if(itsCacheTmpDirectory != other.itsCacheTmpDirectory)
        return true;
	if(itsCachePartialDataDirectory != other.itsCachePartialDataDirectory)
        return true;
	if(itsCacheTmpFileNameFix != other.itsCacheTmpFileNameFix)
        return true;
	if(fUseQueryDataCache != other.fUseQueryDataCache)
        return true;
	if(fDoCleanCache != other.fDoCleanCache)
        return true;
	if(itsCacheFileKeepMaxDays != other.itsCacheFileKeepMaxDays)
        return true;
	if(itsCacheMaxFilesPerPattern != other.itsCacheMaxFilesPerPattern)
        return true;
	if(itsCacheMediumFileSizeMB != other.itsCacheMediumFileSizeMB)
        return true;
	if(itsCacheLargeFileSizeMB != other.itsCacheLargeFileSizeMB)
        return true;
	if(itsCacheMaximumFileSizeMB != other.itsCacheMaximumFileSizeMB)
        return true;

    return false;
}

void NFmiHelpDataInfoSystem::InitSettings(const NFmiHelpDataInfoSystem &theOther, bool fDoHelpDataInfo)
{
	this->itsCacheDirectory = theOther.itsCacheDirectory;
	this->itsCacheTmpDirectory = theOther.itsCacheTmpDirectory;
	this->itsCachePartialDataDirectory = theOther.itsCachePartialDataDirectory;
	this->itsCacheTmpFileNameFix = theOther.itsCacheTmpFileNameFix;
	this->fUseQueryDataCache = theOther.fUseQueryDataCache;
	this->fDoCleanCache = theOther.fDoCleanCache;
	this->itsCacheFileKeepMaxDays = theOther.itsCacheFileKeepMaxDays;
	this->itsCacheMaxFilesPerPattern = theOther.itsCacheMaxFilesPerPattern;
	this->itsBaseNameSpace = theOther.itsBaseNameSpace;

	if(fDoHelpDataInfo)
	{
		this->itsDynamicHelpDataInfos = theOther.itsDynamicHelpDataInfos;
		this->itsStaticHelpDataInfos = theOther.itsStaticHelpDataInfos;
	}
}

void NFmiHelpDataInfoSystem::ResetAllDynamicDataTimeStamps()
{
	size_t ssize = itsDynamicHelpDataInfos.size();
	for(size_t i = 0; i<ssize; i++)
		itsDynamicHelpDataInfos[i].LatestFileTimeStamp(-1);
}

static NFmiHelpDataInfo* FindHelpDataInfo(checkedVector<NFmiHelpDataInfo> &theHelpInfos, const std::string &theFileNameFilter, const NFmiHelpDataInfoSystem &theHelpDataInfoSystem)
{
	size_t ssize = theHelpInfos.size();
	for(size_t i = 0; i<ssize; i++)
	{
		// Siis jos joko FileNameFilter tai CombineDataPathAndFileName (yhdistelm‰ datoissa t‰m‰ 
		// on se data joka luetaan sis‰‰n SmartMetiin) on etsitty, palautetaan helpInfo.
		if(theHelpInfos[i].UsedFileNameFilter(theHelpDataInfoSystem) == theFileNameFilter || theHelpInfos[i].CombineDataPathAndFileName() == theFileNameFilter)
			return &theHelpInfos[i];
	}
	return 0;
}

// Etsii annetun fileNameFilterin avulla HelpDataInfon ja palauttaa sen, jos lˆytyi.
// Jos ei lˆytynyt vastaavaa filePatternia, palauttaa 0-pointterin.
// K‰y ensin l‰pi dynaamiset helpDataInfot ja sitten staattiset.
NFmiHelpDataInfo* NFmiHelpDataInfoSystem::FindHelpDataInfo(const std::string &theFileNameFilter)
{
	if(theFileNameFilter.empty())
		return 0;

	NFmiHelpDataInfo *helpInfo = ::FindHelpDataInfo(itsDynamicHelpDataInfos, theFileNameFilter, *this);
	if(helpInfo == 0)
		helpInfo = ::FindHelpDataInfo(itsStaticHelpDataInfos, theFileNameFilter, *this);

	return helpInfo;
}

static void CollectCustomMenuItems(const checkedVector<NFmiHelpDataInfo> &theHelpInfos, std::set<std::string> &theMenuSet)
{
	size_t ssize = theHelpInfos.size();
	for(size_t i = 0; i<ssize; i++)
	{
		if(theHelpInfos[i].CustomMenuFolder().empty() == false)
			theMenuSet.insert(theHelpInfos[i].CustomMenuFolder());
	}
}

// ker‰‰ uniikki lista mahdollisista custom Menu folder asetuksista
std::vector<std::string> NFmiHelpDataInfoSystem::GetUniqueCustomMenuList(void)
{
	std::set<std::string> menuSet;
	::CollectCustomMenuItems(itsDynamicHelpDataInfos, menuSet);
	::CollectCustomMenuItems(itsStaticHelpDataInfos, menuSet);

	std::vector<std::string> menuList(menuSet.begin(), menuSet.end());
	return menuList;
}

static void CollectCustomMenuHelpDatas(const checkedVector<NFmiHelpDataInfo> &theHelpInfos, const std::string &theCustomFolder, std::vector<NFmiHelpDataInfo> &theCustomHelpDatas)
{
	size_t ssize = theHelpInfos.size();
	for(size_t i = 0; i<ssize; i++)
	{
		if(theHelpInfos[i].CustomMenuFolder().empty() == false)
			if(theHelpInfos[i].CustomMenuFolder() == theCustomFolder)
				theCustomHelpDatas.push_back(theHelpInfos[i]);
	}
}

// ker‰t‰‰n list‰ niista helpDataInfoissta, joissa on asetettu kyseinen customFolder
std::vector<NFmiHelpDataInfo> NFmiHelpDataInfoSystem::GetCustomMenuHelpDataList(const std::string &theCustomFolder)
{
	std::vector<NFmiHelpDataInfo> helpDataList;
	if(theCustomFolder.empty() == false)
	{
		::CollectCustomMenuHelpDatas(itsDynamicHelpDataInfos, theCustomFolder, helpDataList);
		::CollectCustomMenuHelpDatas(itsStaticHelpDataInfos, theCustomFolder, helpDataList);
	}
	return helpDataList;
}

NFmiHelpDataInfoSystem::NFmiHelpDataInfoSystem(const NFmiHelpDataInfoSystem &other)
:itsDynamicHelpDataInfos(other.itsDynamicHelpDataInfos)
,itsStaticHelpDataInfos(other.itsStaticHelpDataInfos)
,itsCacheDirectory(other.itsCacheDirectory)
,itsCacheTmpDirectory(other.itsCacheTmpDirectory)
,itsCachePartialDataDirectory(other.itsCachePartialDataDirectory)
,itsCacheTmpFileNameFix(other.itsCacheTmpFileNameFix)
,fUseQueryDataCache(other.fUseQueryDataCache)
,fDoCleanCache(other.fDoCleanCache)
,itsCacheFileKeepMaxDays(other.itsCacheFileKeepMaxDays)
,itsCacheMaxFilesPerPattern(other.itsCacheMaxFilesPerPattern)
,itsCacheMediumFileSizeMB(other.itsCacheMediumFileSizeMB)
,itsCacheLargeFileSizeMB(other.itsCacheLargeFileSizeMB)
,itsCacheMaximumFileSizeMB(other.itsCacheMaximumFileSizeMB)
,itsBaseNameSpace(other.itsBaseNameSpace)
{
}

NFmiHelpDataInfoSystem& NFmiHelpDataInfoSystem::operator=(const NFmiHelpDataInfoSystem &other)
{
	if(this != &other)
	{
	    itsDynamicHelpDataInfos = other.itsDynamicHelpDataInfos;
	    itsStaticHelpDataInfos = other.itsStaticHelpDataInfos;
	    itsCacheDirectory = other.itsCacheDirectory;
	    itsCacheTmpDirectory = other.itsCacheTmpDirectory;
	    itsCachePartialDataDirectory = other.itsCachePartialDataDirectory;
	    itsCacheTmpFileNameFix = other.itsCacheTmpFileNameFix;
	    fUseQueryDataCache = other.fUseQueryDataCache;
	    fDoCleanCache = other.fDoCleanCache;
	    itsCacheFileKeepMaxDays = other.itsCacheFileKeepMaxDays;
	    itsCacheMaxFilesPerPattern = other.itsCacheMaxFilesPerPattern;
	    itsCacheMediumFileSizeMB = other.itsCacheMediumFileSizeMB;
	    itsCacheLargeFileSizeMB = other.itsCacheLargeFileSizeMB;
	    itsCacheMaximumFileSizeMB = other.itsCacheMaximumFileSizeMB;
	    itsBaseNameSpace = other.itsBaseNameSpace;
    }
	return *this;
}

bool NFmiHelpDataInfoSystem::operator==(const NFmiHelpDataInfoSystem &other) const
{
	if(itsDynamicHelpDataInfos != other.itsDynamicHelpDataInfos)
        return false;
	if(itsStaticHelpDataInfos != other.itsStaticHelpDataInfos)
        return false;
	if(itsCacheDirectory != other.itsCacheDirectory)
        return false;
	if(itsCacheTmpDirectory != other.itsCacheTmpDirectory)
        return false;
	if(itsCachePartialDataDirectory != other.itsCachePartialDataDirectory)
        return false;
	if(itsCacheTmpFileNameFix != other.itsCacheTmpFileNameFix)
        return false;
	if(fUseQueryDataCache != other.fUseQueryDataCache)
        return false;
	if(fDoCleanCache != other.fDoCleanCache)
        return false;
	if(itsCacheFileKeepMaxDays != other.itsCacheFileKeepMaxDays)
        return false;
	if(itsCacheMaxFilesPerPattern != other.itsCacheMaxFilesPerPattern)
        return false;
	if(itsCacheMediumFileSizeMB != other.itsCacheMediumFileSizeMB)
        return false;
	if(itsCacheLargeFileSizeMB != other.itsCacheLargeFileSizeMB)
        return false;
	if(itsCacheMaximumFileSizeMB != other.itsCacheMaximumFileSizeMB)
        return false;
	if(itsBaseNameSpace != other.itsBaseNameSpace)
        return false;

    return true;
}

bool NFmiHelpDataInfoSystem::operator!=(const NFmiHelpDataInfoSystem &other) const
{
    return !(*this == other);
}

