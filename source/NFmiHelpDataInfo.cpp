// NFmiHelpDataInfo.cpp

#ifdef _MSC_VER
#pragma warning(disable : 4996) // 4996 poistaa ep‰turvallisten string manipulaatio funktioiden k‰ytˆst‰ tulevat varoitukset. En aio k‰ytt‰‰ ehdotettuja turvallisia _s -funktioita (esim. sprintf_s), koska ne eiv‰t ole linux yhteensopivia.
#endif

#include "NFmiHelpDataInfo.h"
#include "NFmiArea.h"
#include "NFmiAreaFactory.h"
#include "NFmiStereographicArea.h"
#include "NFmiSettings.h"
#include "NFmiFileString.h"

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
:itsFileNameFilter()
,itsLatestFileName()
,itsDataType(NFmiInfoData::kNoDataType)
,itsLatestFileTimeStamp(0)
,itsFakeProducerId(0)
,itsImageProjectionString()
,itsImageDataIdent()
,itsImageArea(0)
,fNotifyOnLoad(false)
,itsNotificationLabel()
,itsCustomMenuFolder()
,itsReportNewDataTimeStepInMinutes(0)
,itsReportNewDataLabel()
,itsCombineDataPathAndFileName()
,itsCombineDataMaxTimeSteps(0)

,itsBaseNameSpace()
{}

NFmiHelpDataInfo::NFmiHelpDataInfo(const NFmiHelpDataInfo &theOther)
:itsFileNameFilter(theOther.itsFileNameFilter)
,itsLatestFileName(theOther.itsLatestFileName)
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

,itsBaseNameSpace(theOther.itsBaseNameSpace)
{}

NFmiHelpDataInfo& NFmiHelpDataInfo::operator=(const NFmiHelpDataInfo &theOther)
{
	if(this != &theOther)
	{
		Clear(); // l‰hinn‰ area-otuksen tuhoamista varten kutsutaan
		itsFileNameFilter = theOther.itsFileNameFilter;
		itsLatestFileName = theOther.itsLatestFileName;
		itsDataType = theOther.itsDataType;
		itsLatestFileTimeStamp = theOther.itsLatestFileTimeStamp;
		itsFakeProducerId = theOther.itsFakeProducerId;
		itsImageProjectionString = theOther.itsImageProjectionString;
		itsImageDataIdent = theOther.itsImageDataIdent;
		itsImageArea = theOther.itsImageArea ? theOther.itsImageArea->Clone() : 0;
		fNotifyOnLoad = theOther.fNotifyOnLoad;
		itsNotificationLabel = theOther.itsNotificationLabel;
		itsCustomMenuFolder = theOther.itsCustomMenuFolder;
		itsReportNewDataTimeStepInMinutes = theOther.itsReportNewDataTimeStepInMinutes;
		itsReportNewDataLabel = theOther.itsReportNewDataLabel;
		itsCombineDataPathAndFileName = theOther.itsCombineDataPathAndFileName;
		itsCombineDataMaxTimeSteps = theOther.itsCombineDataMaxTimeSteps;

		itsBaseNameSpace = theOther.itsBaseNameSpace;
	}
	return *this;
}

void NFmiHelpDataInfo::Clear(void)
{
	itsFileNameFilter = "";
	itsLatestFileName = "";
	itsDataType = NFmiInfoData::kNoDataType;
	itsLatestFileTimeStamp = 0;
	itsFakeProducerId = 0;
	itsImageProjectionString = "";
	itsImageDataIdent = NFmiDataIdent();
	delete itsImageArea;
	itsImageArea = 0;
	fNotifyOnLoad = false;
	itsNotificationLabel = "";
	itsCustomMenuFolder = "";
	itsBaseNameSpace = "";
	itsReportNewDataTimeStepInMinutes = 0;
	itsReportNewDataLabel = "";
	itsCombineDataPathAndFileName = "";
	itsCombineDataMaxTimeSteps = 0;
}

static std::string MakeFileNameFilter(const std::string &theFileFilterBase, const std::string &theRootDir)
{
	std::string returnValue = theFileFilterBase;
	bool useRootDir = theRootDir.empty() == false;
	if(useRootDir)
	{
		NFmiFileString fileStr(returnValue);
		fileStr.NormalizeDelimiter();
		// T‰m‰ kikkailu on sit‰ varten ett‰ helpdata-tiedostossa on k‰ytetty
		// ./alkuisia suhteellisia polkuja, joita halutaan k‰ytt‰‰ vaikka rootti-dir onkin m‰‰ritelty!!!
		if(fileStr.IsAbsolutePath() == false && (fileStr.GetLen() > 2 && fileStr[1ul] != '.' && fileStr[2ul] != kFmiDirectorySeparator)) // 1ul on unsigned long casti, jonka msvc++ 2008 k‰‰nt‰j‰ vaatii
		{
			NFmiFileString finalFileStr(theRootDir);
			finalFileStr.NormalizeDelimiter();
			if(finalFileStr[finalFileStr.GetLen()] != kFmiDirectorySeparator)
				finalFileStr += kFmiDirectorySeparator;
			finalFileStr += returnValue;

			returnValue = static_cast<char *>(finalFileStr);
		}
	}
	return returnValue;
}

void NFmiHelpDataInfo::InitFromSettings(const std::string &theInitNameSpace, const std::string &theRootDir)
{
	itsBaseNameSpace = theInitNameSpace;

	std::string fileNameFilterKey = itsBaseNameSpace + "::FilenameFilter";
	if(NFmiSettings::IsSet(fileNameFilterKey))
	{
		// Read configuration
		string tmpFileNameFilter = NFmiSettings::Require<std::string>(fileNameFilterKey);
		itsFileNameFilter = ::MakeFileNameFilter(tmpFileNameFilter, theRootDir);
		itsDataType = static_cast<NFmiInfoData::Type> (NFmiSettings::Require<int>(itsBaseNameSpace + "::DataType"));
		itsFakeProducerId = NFmiSettings::Optional<int>(itsBaseNameSpace + "::ProducerId", 0);
		fNotifyOnLoad = NFmiSettings::Optional<bool>(itsBaseNameSpace + "::NotifyOnLoad", false);
		itsNotificationLabel = NFmiSettings::Optional<string>(itsBaseNameSpace + "::NotificationLabel", "");
		itsCustomMenuFolder = NFmiSettings::Optional<string>(itsBaseNameSpace + "::CustomMenuFolder", "");
		itsReportNewDataTimeStepInMinutes = NFmiSettings::Optional<int>(itsBaseNameSpace + "::ReportNewDataTimeStepInMinutes", 0);
		itsReportNewDataLabel = NFmiSettings::Optional<string>(itsBaseNameSpace + "::ReportNewDataLabel", "");
		itsCombineDataPathAndFileName = NFmiSettings::Optional<string>(itsBaseNameSpace + "::CombineDataPathAndFileName", "");
		itsCombineDataMaxTimeSteps = NFmiSettings::Optional<int>(itsBaseNameSpace + "::CombineDataMaxTimeSteps", 0);

		std::string imageProjectionKey(itsBaseNameSpace + "::ImageProjection");
		if (NFmiSettings::IsSet(imageProjectionKey))
		{
			NFmiArea *area = NFmiAreaFactory::Create(NFmiSettings::Require<std::string>(imageProjectionKey)).release();
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
			itsImageDataIdent = NFmiDataIdent(param, itsFakeProducerId);
		}
	}
}

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
			if(static_cast<unsigned long>(prodId) == itsDynamicHelpDataInfos[i].ImageDataIdent().GetProducer()->GetIdent())
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
const NFmiArea* NFmiHelpDataInfoSystem::GetDataFilePatternAndArea(NFmiInfoData::Type theDataType, FmiProducerName theProdId, FmiParameterName theParamId, const NFmiArea *theZoomedArea, std::string &theFilePattern, bool fDemandMatchingArea)
{
	if(theDataType == NFmiInfoData::kSatelData)
	{
		size_t count = itsDynamicHelpDataInfos.size();
		for(size_t i=0; i<count; i++)
		{
			if(itsDynamicHelpDataInfos[i].DataType() == theDataType)
			{
				if(static_cast<unsigned long>(theProdId) == itsDynamicHelpDataInfos[i].ImageDataIdent().GetProducer()->GetIdent() &&
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
	return 0;
}

// Ovatko kaksi annettua projektiota siin‰ mieless‰ samanlaisia, ett‰ 
// voidaan sijoittaa rasterikuva oikein toisen rasterikuvan p‰‰lle.
// Eli palauta false = ei voi piirt‰‰ ja true = voi, 
// jos projektio luokat ovat samoja ja jos kyseess‰ oli stereographic areat, niiden
// orientaatiot pit‰‰ olla samat.
bool NFmiHelpDataInfoSystem::IsSameTypeProjections(const NFmiArea *theFirst, const NFmiArea *theSecond)
{
	if(theFirst && theSecond)
	{
		if(theFirst->ClassId() == theSecond->ClassId() && theFirst->ClassId() == kNFmiStereographicArea)
		{
			if(static_cast<const NFmiStereographicArea*>(theFirst)->Orientation() == static_cast<const NFmiStereographicArea*>(theSecond)->Orientation())
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

void NFmiHelpDataInfoSystem::InitDataType(const std::string &theBaseKey, const std::string &theRootDir, checkedVector<NFmiHelpDataInfo> &theHelpDataInfos)
{
	std::vector<std::string> dataKeys = NFmiSettings::ListChildren(theBaseKey);
	std::vector<std::string>::iterator iter = dataKeys.begin();
	for( ; iter != dataKeys.end(); ++iter)
	{
		NFmiHelpDataInfo hdi;
		hdi.InitFromSettings(theBaseKey + "::" + (*iter), theRootDir);
		theHelpDataInfos.push_back(hdi);
	}
}

void NFmiHelpDataInfoSystem::InitFromSettings(const std::string &theBaseNameSpaceStr, std::string theHelpEditorFileNameFilter)
{
	string rootKey = theBaseNameSpaceStr + "::RootDir";
	string rootDir = NFmiSettings::Optional(rootKey.c_str(), string(""));
	RootDirectory(rootDir);
	// Read static helpdata configurations
	InitDataType(theBaseNameSpaceStr + "::Static", rootDir, itsStaticHelpDataInfos);

	// Read dynamic helpdata configurations
	InitDataType(theBaseNameSpaceStr + "::Dynamic", rootDir, itsDynamicHelpDataInfos);

	// Lis‰t‰‰n help editor mode datan luku jos niin on haluttu
	if(theHelpEditorFileNameFilter.empty() == false)
	{
		NFmiHelpDataInfo helpDataInfo;
		helpDataInfo.FileNameFilter(theHelpEditorFileNameFilter);
		helpDataInfo.DataType(NFmiInfoData::kEditingHelpData);
		AddDynamic(helpDataInfo);
	}
}

void NFmiHelpDataInfoSystem::MarkAllDynamicDatasAsNotReaded()
{
	size_t ssize = itsDynamicHelpDataInfos.size();
	for(size_t i = 0; i<ssize; i++)
		itsDynamicHelpDataInfos[i].LatestFileTimeStamp(-1);
}

static NFmiHelpDataInfo* FindHelpDataInfo(checkedVector<NFmiHelpDataInfo> &theHelpInfos, const std::string &theFileNameFilter)
{
	size_t ssize = theHelpInfos.size();
	for(size_t i = 0; i<ssize; i++)
	{
		// Siis jos joko FileNameFilter tai CombineDataPathAndFileName (yhdistelm‰ datoissa t‰m‰ 
		// on se data joka luetaan sis‰‰n SmartMetiin) on etsitty, palautetaan helpInfo.
		if(theHelpInfos[i].FileNameFilter() == theFileNameFilter || theHelpInfos[i].CombineDataPathAndFileName() == theFileNameFilter)
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

	NFmiHelpDataInfo *helpInfo = ::FindHelpDataInfo(itsDynamicHelpDataInfos, theFileNameFilter);
	if(helpInfo == 0)
		helpInfo = ::FindHelpDataInfo(itsStaticHelpDataInfos, theFileNameFilter);

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
