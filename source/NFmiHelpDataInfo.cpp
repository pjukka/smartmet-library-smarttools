// NFmiHelpDataInfo.cpp

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
static bool EatWhiteSpaces(istream & theInput)
{
  char ch = '\0';
  do
	{
	  ch = theInput.get();
	}
  while(isspace(ch));
  if(theInput.fail()) 
	return false; // jos stremin lopussa, ep‰onnistuu
  else
	theInput.unget();
  return true;
}

NFmiHelpDataInfo::NFmiHelpDataInfo(void)
:itsFileNameFilter()
,itsLatestFileName()
,itsDataType(NFmiInfoData::kNoDataType)
,itsLatestFileTimeStamp(0)
,itsFakeProducerId(0)
,itsImageProjectionString()
,itsImageDataIdent()
,itsImageArea(0)
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
}

std::istream & NFmiHelpDataInfo::Read(std::istream & file)
{
	Clear();
	ReadNextLine(file, itsFileNameFilter);
	int tmp = 0;
	file >> tmp;
	itsDataType = static_cast<NFmiInfoData::Type>(tmp);
	file >> itsFakeProducerId;
	if(itsDataType == NFmiInfoData::kSatelData)
	{
		file >> itsImageProjectionString;
		itsImageArea = NFmiAreaFactory::Create(itsImageProjectionString).release(); // HUOM! voi heitt‰‰ poikkeuksen
		int tmpParId;
		file >> tmpParId;
		string tmpParNameStr;
		ReadNextLine(file, tmpParNameStr);
		itsImageDataIdent = NFmiDataIdent(NFmiParam(tmpParId, tmpParNameStr), NFmiProducer(itsFakeProducerId)); // ik‰v‰ koukku, mutta fake prod id onkin satelliitin prod id
	}
	return file;
}

bool NFmiHelpDataInfo::ReadNextLine(std::istream & file, std::string &theLine)
{
	const int maxBufferSize = 512; // kuinka pitk‰ tiedoston nimi voi olla polkuineen maksimissaan
	string buffer;
	buffer.resize(maxBufferSize);
	::EatWhiteSpaces(file);
	file.getline(&buffer[0], maxBufferSize);
	int realSize = strlen(buffer.c_str());
	buffer.resize(realSize);
	theLine = buffer;
	return file.good();
}

std::istream & NFmiHelpDataInfoSystem::Read(std::istream & file)
{
	Clear();
	int dataCount = 0;
	file >> dataCount; // dynaamisten lukum‰‰r‰
	int i=0;
	NFmiHelpDataInfo hdInfo;
	for(i=0; i<dataCount; i++)
	{
		file >> hdInfo;
		AddDynamic(hdInfo);
	}
	file >> dataCount; // staattistem lukum‰‰r‰
	for(i=0; i<dataCount; i++)
	{
		file >> hdInfo;
		AddStatic(hdInfo);
	}
	return file;
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
	int count = itsDynamicHelpDataInfos.size();
	std::vector<NFmiDataIdent> dataIdentVec;
	int counter = 0;
	int currentIndex = -1;
	for(int i=0; i<count; i++)
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
		int count = itsDynamicHelpDataInfos.size();
		for(int i=0; i<count; i++)
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

bool NFmiHelpDataInfoSystem::Init(const std::string &theBaseNameSpaceStr, std::string theHelpEditorFileNameFilter)
{
	string rootKey = theBaseNameSpaceStr + "::RootDir";
	string rootDir = NFmiSettings::Optional(rootKey.c_str(), string(""));
	RootDirectory(rootDir);
	bool useRootDir = rootDir.empty() == false;

	// Read static helpdata configurations
	string staticBaseKey = theBaseNameSpaceStr + "::Static";
	string staticFileFilterKey = staticBaseKey + "::%s::FilenameFilter";
	string staticDataTypeKey = staticBaseKey + "::%s::DataType";
	string staticProducerKey = staticBaseKey + "::%s::ProducerId";
	
	char key1[128], key2[128];
	std::vector<std::string> stats = NFmiSettings::ListChildren(staticBaseKey.c_str());
	std::vector<std::string>::iterator statiter = stats.begin();
	while (statiter != stats.end())
	{
		std::string stat(*statiter);
		sprintf(key1, staticFileFilterKey.c_str(), stat.c_str());
		if (NFmiSettings::IsSet(key1))
		{
			// Read configuration
			NFmiHelpDataInfo hdi;
			hdi.FileNameFilter(NFmiSettings::Require<std::string>(key1));
			sprintf(key1, staticDataTypeKey.c_str(), stat.c_str()); // Datatype
			NFmiInfoData::Type datatype = static_cast<NFmiInfoData::Type> (NFmiSettings::Optional<int>(key1, 0));
			hdi.DataType(datatype);
			sprintf(key1, staticProducerKey.c_str(), stat.c_str()); // (Fake) Producer ID
			hdi.FakeProducerId(NFmiSettings::Optional<int>(key1, 0));

			AddStatic(hdi);
		}

		statiter++;
	}

	// Read dynamic helpdata configurations
	string dynamicBaseKey = theBaseNameSpaceStr + "::Dynamic";
	string dynamicFileFilterKey = dynamicBaseKey + "::%s::FilenameFilter";
	string dynamicDataTypeKey = dynamicBaseKey + "::%s::DataType";
	string dynamicProducerKey = dynamicBaseKey + "::%s::ProducerId";
	string dynamicImageProjectionKey = dynamicBaseKey + "::%s::ImageProjection";
	string dynamicParameterNameKey = dynamicBaseKey + "::%s::ParameterName";
	string dynamicParameterIdKey = dynamicBaseKey + "::%s::ParameterId";

	std::vector<std::string> dyns = NFmiSettings::ListChildren(dynamicBaseKey.c_str());
	std::vector<std::string>::iterator dyniter = dyns.begin();
	while (dyniter != dyns.end())
	{
		std::string dyn(*dyniter);
		sprintf(key1, dynamicFileFilterKey.c_str(), dyn.c_str());
		if (NFmiSettings::IsSet(key1))
		{
			// Read configuration
			NFmiHelpDataInfo hdi;
			string tmpFileNameFilter = NFmiSettings::Require<std::string>(key1);
			if(useRootDir)
			{
				NFmiFileString fileStr(tmpFileNameFilter);
				fileStr.NormalizeDelimiter();
				unsigned long bs1 = 1; // V***n MSVC k‰‰nt‰j‰ ei p‰‰st‰ muuten l‰pi
				unsigned long bs2 = 2;
				// T‰m‰ kikkailu on sit‰ varten ett‰ helpdata-tiedostossa on k‰ytetty
				// ./alkuisia suhteellisia polkuja, joita halutaan k‰ytt‰‰ vaikka rootti-dir onkin m‰‰ritelty!!!
				if(fileStr.IsAbsolutePath() == false && (fileStr.GetLen() > 2 && fileStr[bs1] != '.' && fileStr[bs2] != kFmiDirectorySeparator))
				{
					NFmiFileString finalFileStr(rootDir);
					finalFileStr.NormalizeDelimiter();
					if(finalFileStr[finalFileStr.GetLen()] != kFmiDirectorySeparator)
						finalFileStr += kFmiDirectorySeparator;
					finalFileStr += tmpFileNameFilter;

					tmpFileNameFilter = static_cast<char *>(finalFileStr);
				}
			}
	//		hdi.FileNameFilter(NFmiSettings::Require<std::string>(key1));
			hdi.FileNameFilter(tmpFileNameFilter);
			sprintf(key1, dynamicDataTypeKey.c_str(), dyn.c_str()); // Datatype
			NFmiInfoData::Type datatype = static_cast<NFmiInfoData::Type> (NFmiSettings::Optional<int>(key1, 0));
			hdi.DataType(datatype);
			sprintf(key1, dynamicProducerKey.c_str(), dyn.c_str()); // (Fake) Producer ID
			hdi.FakeProducerId(NFmiSettings::Optional<int>(key1, 0));
			sprintf(key1, dynamicImageProjectionKey.c_str(), dyn.c_str()); // Image projection
			if (NFmiSettings::IsSet(key1))
			{
				NFmiArea *area = NFmiAreaFactory::Create(NFmiSettings::Require<std::string>(key1)).release();
				if(area)
				{
					if(area->XYArea().Width() != 1 || area->XYArea().Height() != 1)
					{
						area->SetXYArea(NFmiRect(0,0,1,1));
					}
					hdi.ImageArea(area);
				}
			}
			sprintf(key1, dynamicParameterIdKey.c_str(), dyn.c_str()); // Parameter ID
			sprintf(key2, dynamicParameterNameKey.c_str(), dyn.c_str()); // Parameter name
			if (NFmiSettings::IsSet(key1) && NFmiSettings::IsSet(key2))
			{
			  NFmiParam p(NFmiSettings::Require<int>(key1),
						  NFmiSettings::Require<std::string>(key2));
			  NFmiDataIdent ident(p,hdi.FakeProducerId());
			  hdi.ImageDataIdent(ident);
			}

			AddDynamic(hdi);
		}

		dyniter++;
	}

	// Lis‰t‰‰n help editor mode datan luku jos niin on haluttu
	if(theHelpEditorFileNameFilter.empty() == false)
	{
		NFmiHelpDataInfo helpDataInfo;
		helpDataInfo.FileNameFilter(theHelpEditorFileNameFilter);
		helpDataInfo.DataType(NFmiInfoData::kEditingHelpData);
		AddDynamic(helpDataInfo);
	}

	return true;
}

