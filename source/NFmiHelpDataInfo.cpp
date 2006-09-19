// NFmiHelpDataInfo.cpp

#include "NFmiHelpDataInfo.h"
#include "NFmiArea.h"
#include "NFmiAreaFactory.h"
#include "NFmiStereographicArea.h"

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
	itsDataType = (NFmiInfoData::Type)tmp;
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
		itsDynamicHelpDataInfos.push_back(hdInfo);
	}
	file >> dataCount; // staattistem lukum‰‰r‰
	for(i=0; i<dataCount; i++)
	{
		file >> hdInfo;
		itsStaticHelpDataInfos.push_back(hdInfo);
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
const NFmiArea* NFmiHelpDataInfoSystem::GetDataFilePatternAndArea(NFmiInfoData::Type theDataType, FmiProducerName theProdId, FmiParameterName theParamId, const NFmiArea *theZoomedArea, std::string &theFilePattern, bool fDemandMatchingArea)
{
	if(theDataType == NFmiInfoData::kSatelData)
	{
		int count = itsDynamicHelpDataInfos.size();
		for(int i=0; i<count; i++)
		{
			if(itsDynamicHelpDataInfos[i].DataType() == theDataType)
			{
				if(theProdId == itsDynamicHelpDataInfos[i].ImageDataIdent().GetProducer()->GetIdent() && theParamId == itsDynamicHelpDataInfos[i].ImageDataIdent().GetParamIdent())
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
