// ======================================================================
/*!
 * C++ Class Name : NFmiInfoOrganizer
 * ---------------------------------------------------------
 * Filetype: (SOURCE)
 * Filepath: D:/projekti/GDPro/GDTemp/NFmiInfoOrganizer.cpp
 *
 *
 * GDPro Properties
 * ---------------------------------------------------
 *  - GD Symbol Type    : CLD_Class
 *  - GD Method         : UML ( 2.1.4 )
 *  - GD System Name    : Met-editor Plan 2
 *  - GD View Type      : Class Diagram
 *  - GD View Name      : Markon ehdotus
 * ---------------------------------------------------
 *  Author         : pietarin
 *  Creation Date  : Tues - Feb 9, 1999
 *
 *
 *  Description:
 *   Sisältää mahdollisesti listan infoja, joista
 *   luokka osaa antaa tarvittavan
 *   infon pointterin parametrin nimellä. Listalla
 *   ei ole järjestystä.
 *
 *  Change Log:
 * 1999.08.26/Marko	Laitoin organizeriin eri tyyppisten datojen talletuksen
 *					mahdollisuuden. Editoitavaa dataa voi olla vain yksi, mutta
 *					katsottavaa/maskattavaa dataa voi olla useita erilaisia ja statio-
 *					näärisiä (esim. topografia) datoja voi olla myös useita erilaisia.
 * 1999.08.26/Marko	Laitoin level tiedon DrawParam:in luonti-kutsuun varten.
 * 1999.09.22/Marko	DrawParamin luonnissa otetaan huomioon datan tyyppi tästälähtien.
 * 1999.09.22/Marko	Lisäsin EditedDatan kopion, jota käytetään visualisoimaan dataan tehtyjä muutoksia.
 *
 */
// ======================================================================

#ifdef _MSC_VER
#pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta (liian pitkä nimi >255 merkkiä joka johtuu 'puretuista' STL-template nimistä)
#endif

#include "NFmiInfoOrganizer.h"
#include "NFmiSmartInfo.h"
#include "NFmiDrawParamFactory.h"
#include "NFmiDrawParam.h"
#include "NFmiGrid.h"
#include "NFmiQueryDataUtil.h"
#include "NFmiQueryData.h"
#include "NFmiLatLonArea.h"
#include "NFmiProducerName.h"

//--------------------------------------------------------
// Constructor/Destructor
//--------------------------------------------------------
// luodaan tyhjä lista
NFmiInfoOrganizer::NFmiInfoOrganizer(void)
:itsList()
,itsDrawParamFactory(0)
,itsWorkingDirectory("")
,itsEditedData(0)
,itsEditedDataCopy(0)
,itsMacroParamGridSize(50, 50)
,itsMacroParamMinGridSize(10, 10)
,itsMacroParamMaxGridSize(200, 200)
,itsMacroParamData(0)
,itsMacroParamMissingValueMatrix()
,itsCrossSectionMacroParamData(0)
,itsCrossSectionMacroParamMissingValueMatrix()
,fCreateEditedDataCopy(true)
{
	NFmiLatLonArea defaultArea(NFmiPoint(0,0), NFmiPoint(50,50)); // tehdään vain dummy area, kun dataa piirretään ja lasketaan, halutun karttanäytön alue asetetaan NFmiEditMapGeneralDataDoc::MakeDrawedInfoVectorForMapView -metodissa 
	UpdateSpecialDataArea(&defaultArea, itsMacroParamGridSize, NFmiInfoData::kMacroParam, &itsMacroParamData, itsMacroParamMissingValueMatrix);
}

// destruktori tuhoaa infot
NFmiInfoOrganizer::~NFmiInfoOrganizer (void)
{
	Clear();
	delete itsDrawParamFactory;
	if(itsEditedData)
		itsEditedData->DestroyData();
	delete itsEditedData;

	if(itsEditedDataCopy)
		itsEditedDataCopy->DestroyData();
	delete itsEditedDataCopy;

	if(itsMacroParamData)
		itsMacroParamData->DestroyData();
	delete itsMacroParamData;

	if(itsCrossSectionMacroParamData)
		itsCrossSectionMacroParamData->DestroyData();
	delete itsCrossSectionMacroParamData;
}


//--------------------------------------------------------
// Init
//--------------------------------------------------------
bool NFmiInfoOrganizer::Init(const std::string &theDrawParamPath, bool createDrawParamFileIfNotExist, bool createEditedDataCopy, bool fUseOnePressureLevelDrawParam)
{
	fCreateEditedDataCopy = createEditedDataCopy;
 	itsDrawParamFactory =new NFmiDrawParamFactory(createDrawParamFileIfNotExist, fUseOnePressureLevelDrawParam);
	itsDrawParamFactory->LoadDirectory(theDrawParamPath);
 	return itsDrawParamFactory->Init();
}

NFmiSmartInfo* NFmiInfoOrganizer::GetSynopPlotParamInfo(NFmiInfoData::Type theType)
{
	if(theType == NFmiInfoData::kEditable)
		return itsEditedData;
	else
	{
		NFmiSmartInfo * aInfo = 0;
		NFmiPtrList<NFmiSmartInfo>::Iterator aIter = itsList.Start();
		// tutkitaan ensin löytyykö theParam suoraan joltain listassa olevalta NFmiSmartInfo-pointterilta
		while(aIter.Next())
		{
			aInfo = aIter.CurrentPtr();
			if(aInfo->DataType() == theType && aInfo->Producer()->GetIdent() == kFmiSYNOP)
				return aInfo;
		}
	}
	return 0;
}

NFmiSmartInfo* NFmiInfoOrganizer::GetSoundingPlotParamInfo(NFmiInfoData::Type theType)
{
	if(theType == NFmiInfoData::kEditable)
		return itsEditedData;
	else
	{
		NFmiSmartInfo * aInfo = 0;
		NFmiPtrList<NFmiSmartInfo>::Iterator aIter = itsList.Start();
		// tutkitaan ensin löytyykö theParam suoraan joltain listassa olevalta NFmiSmartInfo-pointterilta
		while(aIter.Next())
		{
			aInfo = aIter.CurrentPtr();
			if(aInfo->DataType() == theType && aInfo->Producer()->GetIdent() == kFmiTEMP)
				return aInfo;
		}
	}
	return 0;
}

static bool CheckDataType(NFmiSmartInfo *theInfo, NFmiInfoData::Type theType)
{
	bool anyDataOk = (theType == NFmiInfoData::kAnyData);
	if(theInfo && (theInfo->DataType() == theType || anyDataOk))
		return true;
	return false;
}

static bool CheckDataIdent(NFmiSmartInfo *theInfo, const NFmiDataIdent& theDataIdent, bool fUseParIdOnly)
{
	if(theInfo && (fUseParIdOnly ? theInfo->Param(static_cast<FmiParameterName>(theDataIdent.GetParamIdent())): theInfo->Param(theDataIdent)))
		return true;
	return false;
}

static bool CheckLevel(NFmiSmartInfo *theInfo, const NFmiLevel *theLevel)
{
	if(theInfo && (!theLevel || (theLevel && theInfo->Level(*theLevel))))
		return true;
	return false;
}

//--------------------------------------------------------
// GetInfo
// Yritin aluksi tehdä metodin käyttämällä parametria 
// bool fIgnoreProducerName = false
// siksi että voisi olla samalta tuottajalta useita samaantyyppisiä
// datoja käytössä yhtäaikaa, mutta tämä osoittautui liian
// haavoittuvaiseksi koska eri tilanteissa datoilla voi olla erilaisia nimiä
// ja tuottajien nimet pitää pystyä vaihtamaan ilman ongelmia
// querydatojen tuotanto ketjuissa. 
// Kun törmäsin nyt kahteen eri ongelmaan:
// 1. querydatan tuottaja nimi vaihdetaan, mutta se on laitettu view-makroon.
// 2. EC:n 3 vrk pinta datassa on 12 utc ajossa eri nimi kuin 00 ajossa.
// SIKSI nyt metodi toimii siten että se yrittää etsiä dataa oikealla tuottaja nimellä.
// Mutta jos oikealla nimellä ei löytynyt, otetaan talteen 1. muilta kriteereiltä
// oikea data ja palautetaan se.
//
//--------------------------------------------------------
NFmiSmartInfo* NFmiInfoOrganizer::GetInfo(const NFmiDataIdent& theDataIdent
									   , const NFmiLevel* theLevel
									   , NFmiInfoData::Type theType
									   , bool fUseParIdOnly)
{
	NFmiSmartInfo *backupData = 0; // etsitää tähän 1. data joka muuten sopii kriteereihin, mutta 
									// jonka tuottaja nimi on eri kuin haluttu. Jos oikealla nimellä ei löydy dataa, käytetään tätä.
	if(theDataIdent.GetParamIdent() == NFmiInfoData::kFmiSpSynoPlot || theDataIdent.GetParamIdent() == NFmiInfoData::kFmiSpMinMaxPlot) // synop plot paramille pitää tehdä kikka (ja min/max plot 9996)
		return GetSynopPlotParamInfo(theType);
	if(theLevel && theLevel->LevelType() == kFmiSoundingLevel) // sounding plot paramille pitää tehdä kikka
		return GetSoundingPlotParamInfo(theType);
	if(theType == NFmiInfoData::kMacroParam || theType == NFmiInfoData::kQ3MacroParam) // macro- parametrit lasketaan tällä
		return itsMacroParamData; // tässä ei parametreja ja leveleitä ihmetellä, koska ne muutetaan aina lennossa tarpeen vaatiessa
	if(theType == NFmiInfoData::kCrossSectionMacroParam)
		return itsCrossSectionMacroParamData; // tässä ei parametreja ja leveleitä ihmetellä, koska ne muutetaan aina lennossa tarpeen vaatiessa

	NFmiSmartInfo* foundData = 0;
	if(::CheckDataType(itsEditedData, theType) && ::CheckDataIdent(itsEditedData, theDataIdent, fUseParIdOnly) && ::CheckLevel(itsEditedData, theLevel))
		foundData = itsEditedData;
	else if(::CheckDataType(itsEditedDataCopy, theType) && ::CheckDataIdent(itsEditedDataCopy, theDataIdent, fUseParIdOnly) && ::CheckLevel(itsEditedDataCopy, theLevel))
		foundData = itsEditedDataCopy;
	else
	{
		// tutkitaan ensin löytyykö theParam suoraan joltain listassa olevalta NFmiSmartInfo-pointterilta
		for(NFmiPtrList<NFmiSmartInfo>::Iterator iter = itsList.Start(); iter.Next(); )
		{
			NFmiSmartInfo *aInfo = &(iter.Current());
			if(::CheckDataType(aInfo, theType) && ::CheckDataIdent(aInfo, theDataIdent, fUseParIdOnly) && ::CheckLevel(aInfo,theLevel))
			{
				if(!(theLevel == 0 && aInfo->SizeLevels() > 1))
				{
					if(theDataIdent.GetProducer()->GetName() == aInfo->Param().GetProducer()->GetName())
					{
						foundData = aInfo;
						break;
					}
					else if(backupData == 0)
						backupData = aInfo;
				}
			}
		}
	}
	if(foundData == 0 && backupData != 0)
		foundData = backupData;

	if(foundData)
	{
		if(foundData->SizeLevels() == 1)
			foundData->FirstLevel();
	}
	return foundData;
}

// Etsi haluttu crossSection-data. Eli pitää olla yli 1 leveliä
// eikä etsitä tiettyä leveliä.
// HUOM! Tein tähän CrossSectionInfo-metodiin saman tuottaja nimi ohitus virityksen kuin
// Info-metodiin. Ks. kommenttia sieltä.
NFmiSmartInfo* NFmiInfoOrganizer::CrossSectionInfo(const NFmiDataIdent& theDataIdent
													, NFmiInfoData::Type theType)
{
	if(theType == NFmiInfoData::kCrossSectionMacroParam)
		return itsCrossSectionMacroParamData;
	NFmiSmartInfo *backupData = 0; // etsitää tähän 1. data joka muuten sopii kriteereihin, mutta 
									// jonka tuottaja nimi on eri kuin haluttu. Jos oikealla nimellä ei löydy dataa, käytetään tätä.
	NFmiSmartInfo *foundData = 0;
	if(::CheckDataType(itsEditedData, theType) && itsEditedData->SizeLevels() > 1 && ::CheckDataIdent(itsEditedData, theDataIdent, true))
		foundData = itsEditedData;
	else
	{
		// tutkitaan ensin löytyykö theParam suoraan joltain listassa olevalta NFmiSmartInfo-pointterilta
		for(NFmiPtrList<NFmiSmartInfo>::Iterator iter = itsList.Start(); iter.Next(); )
		{
			NFmiSmartInfo *aInfo = &(iter.Current());
			if(::CheckDataType(aInfo, theType) && aInfo->SizeLevels() > 1 && ::CheckDataIdent(aInfo, theDataIdent, false))
			{
				if(theDataIdent.GetProducer()->GetName() == aInfo->Param().GetProducer()->GetName())
				{
					foundData = aInfo;
					break;
				}
				else if(backupData == 0)
					backupData = aInfo;
			}
		}
	}

	if(foundData == 0 && backupData != 0)
		foundData = backupData;

	return foundData;
}

//--------------------------------------------------------
// CreateDrawParam(NFmiDataIdent& theDataIdent)
//--------------------------------------------------------
// Tutkii löytyykö listasta itsList infoa, jossa on theDataIdent - siis
// etsitään info, jonka tuottaja ja parametri saadaan theDataIdent:stä.
// Jos tälläinen info löytyy, pyydetään itsDrawParamFactory luomaan
// drawParam kyseiselle parametrille löydetyn infon avulla.
NFmiDrawParam* NFmiInfoOrganizer::CreateDrawParam(const NFmiDataIdent& theIdent, const NFmiLevel* theLevel, NFmiInfoData::Type theType)
{
// Huomaa, että itsDrawParamFactory luo pointterin drawParam new:llä, joten
// drawParam pitää muistaa tuhota  NFmiInfoOrganizer:n ulkopuolella
	NFmiDrawParam* drawParam = 0;
	if(theType == NFmiInfoData::kSatelData || theType == NFmiInfoData::kConceptualModelData) // spesiaali keissi satelliitti kuville, niillä ei ole infoa
	{
		drawParam = new NFmiDrawParam(theIdent, NFmiLevel(), 1, theType);
		drawParam->ParameterAbbreviation(static_cast<char*>(theIdent.GetParamName()));
		return drawParam;
	}
	if(theIdent.GetParamIdent() == NFmiInfoData::kFmiSpSynoPlot) // synop plottia varten taas kikkailua
	{
		return CreateSynopPlotDrawParam(theIdent, theLevel, theType);
	}
	drawParam = itsDrawParamFactory->CreateDrawParam(theIdent, theLevel);
	if(drawParam)
		drawParam->DataType(theType); // data tyyppi pitää myös asettaa!!
	return drawParam;
}

// hakee poikkileikkausta varten haluttua dataa ja luo siihen sopivan drawparamin
NFmiDrawParam* NFmiInfoOrganizer::CreateCrossSectionDrawParam(const NFmiDataIdent& theDataIdent, NFmiInfoData::Type theType)
{
	NFmiDrawParam* drawParam = 0;
	drawParam = itsDrawParamFactory->CreateCrossSectionDrawParam(theDataIdent);
	if(drawParam)
		drawParam->DataType(theType); // data tyyppi pitää myös asettaa!!
	return drawParam;
}

NFmiDrawParam* NFmiInfoOrganizer::CreateSynopPlotDrawParam(const NFmiDataIdent& theDataIdent
														  ,const NFmiLevel* theLevel
														  ,NFmiInfoData::Type theType)
{
	NFmiDrawParam *drawParam = itsDrawParamFactory->CreateDrawParam(theDataIdent, theLevel); // false merkitsee, että parametria ei taas aseteta tuolla metodissa
	if(drawParam)
		drawParam->DataType(theType);
	return drawParam;
}

//--------------------------------------------------------
// AddData
//--------------------------------------------------------
// HUOM!!!! Tänne ei ole sitten tarkoitus antaa kFmiDataTypeCopyOfEdited-tyyppistä
// dataa, koska se luodaan kun tänne annetaan editoitavaa dataa.
bool NFmiInfoOrganizer::AddData(NFmiQueryData* theData
									 ,const std::string& theDataFileName
									 ,const std::string& theDataFilePattern
									 ,NFmiInfoData::Type theDataType
									 ,int theUndoLevel)
{
	bool status = false;
	if(theData)
	{
		NFmiQueryInfo aQueryInfo(theData);
		NFmiSmartInfo* aSmartInfo = new NFmiSmartInfo(aQueryInfo, theData, theDataFileName, theDataFilePattern, theDataType);// ...otetaan käyttöön myöhemmin
		if(aSmartInfo)
			aSmartInfo->First();

		if(theDataType == NFmiInfoData::kEditable)
		{
			try
			{
				if(theUndoLevel)
					aSmartInfo->UndoLevel(theUndoLevel);
			}
			catch(...)
			{
				// jos muisti loppuu tai muu poikkeus, asetetaan undo level 0:ksi ja jatketaan
				theUndoLevel = 0;
				aSmartInfo->DestroyData(false); // false tässä tarkoittaa että ei tuhota queryDataa

				throw ; // heitetään poikkeus eteenpäin
			}

			if(itsEditedData)
			{
				itsEditedData->DestroyData();
				delete itsEditedData;
			}
			itsEditedData = aSmartInfo;
			fCreateEditedDataCopy = theUndoLevel ? true : false; // pitää päivittää kopion luomiseen vaikuttavaa muuttujaa undo-levelin mukaan
			UpdateEditedDataCopy();

			status = true;
		}
		else
			status = Add(aSmartInfo); // muun tyyppiset datat kuin editoitavat menevät listaan

	}
	return status;
}

void NFmiInfoOrganizer::ClearData(NFmiInfoData::Type theDataType)
{
	if(theDataType == NFmiInfoData::kEditable)
	{
		if(itsEditedData)
		{
			itsEditedData->DestroyData();
			delete itsEditedData;
			itsEditedData = 0;
		}
	}
	else
	{ // käydään lista läpi ja tuhotaan halutun tyyppiset datat
		for(NFmiPtrList<NFmiSmartInfo>::Iterator iter = itsList.Start(); iter.Next(); )
		{
			if(iter.Current().DataType() == theDataType)
			{
				iter.Current().DestroyData();
				iter.Remove(true);
			}
		}
	}
}

void NFmiInfoOrganizer::ClearDynamicHelpData()
{ 
	std::vector<NFmiInfoData::Type> ignoreTypesVector;
	ignoreTypesVector.push_back(NFmiInfoData::kStationary); // stationaariset eli esim. topografia data ei kuulu poistettaviin datoihin
	ignoreTypesVector.push_back(NFmiInfoData::kClimatologyData); // klimatologiset datat luetaan vain kerran ohjelman käynnistyessä

	// käydään lista läpi ja tuhotaan dynaamiset help datat
	for(NFmiPtrList<NFmiSmartInfo>::Iterator iter = itsList.Start(); iter.Next(); )
	{
		if(std::find(ignoreTypesVector.begin(), ignoreTypesVector.end(), iter.Current().DataType()) == ignoreTypesVector.end())
		{
			iter.Current().DestroyData();
			iter.Remove(true);
		}
	}
}


// theRemovedDatasTimesOut -parametri käytetään optimoidaan esim. havaittua hila datan kanssa tehtäviä ruudun päivityksiä,
// editoitavasta datasta ei oteta mitään aikoja talteen.
void NFmiInfoOrganizer::ClearThisKindOfData(NFmiQueryInfo* theInfo, NFmiInfoData::Type theDataType, const std::string &theFileNamePattern, NFmiTimeDescriptor &theRemovedDatasTimesOut)
{
	if(theInfo)
	{
		if(itsEditedData)
		{
			if(IsInfosTwoOfTheKind(theInfo, theDataType, theFileNamePattern, itsEditedData))
			{
				itsEditedData->DestroyData();
				delete itsEditedData;
				itsEditedData = 0;
				return;
			}
		}

		for(NFmiPtrList<NFmiSmartInfo>::Iterator iter = itsList.Start(); iter.Next(); )
		{
			if(IsInfosTwoOfTheKind(theInfo, theDataType, theFileNamePattern, &(iter.Current())))
			{
				theRemovedDatasTimesOut = iter.Current().TimeDescriptor();
				iter.Current().DestroyData();
				iter.Remove(true);
				break; // tuhotaan vain yksi!
			}
		}
	}
}

int NFmiInfoOrganizer::CountData(void)
{
	int count = 0;
	if(this->itsEditedData)
		count++;
	if(this->itsEditedDataCopy)
		count++;

	count += itsList.NumberOfItems();
	return count;
}

double NFmiInfoOrganizer::CountDataSize(void)
{
	double dataSize = 0;
	if(itsEditedData)
		dataSize += itsEditedData->Size() * sizeof(float);
	if(itsEditedDataCopy)
		dataSize += itsEditedDataCopy->Size() * sizeof(float);

	for(NFmiPtrList<NFmiSmartInfo>::Iterator iter = itsList.Start(); iter.Next(); )
	{
		dataSize += iter.Current().Size() * sizeof(float);
	}
	return dataSize;
}

// this kind of määritellään tällä hetkellä:
// parametrien, leveleiden ja mahdollisen gridin avulla (ei location bagin avulla)
bool NFmiInfoOrganizer::IsInfosTwoOfTheKind(NFmiQueryInfo* theInfo1, NFmiInfoData::Type theType1, const std::string &theFileNamePattern, NFmiSmartInfo* theSmartInfo2)
{
	// parametrit ja tuottajat samoja
	if(theInfo1 && theSmartInfo2)
	{
		if(theType1 == theSmartInfo2->DataType())
		{
			if(theFileNamePattern.empty() == false && theSmartInfo2->DataFilePattern().empty() == false && theFileNamePattern == theSmartInfo2->DataFilePattern())
				return true; // jos filepatternit eivät olleet tyhjiä ja ne ovat samanlaisia, pidetään datoja samanlaisina (tämä auttaa, jos datat muuttuvat (paramereja lisää, asemia lisää jne.))
			if(theInfo1->ParamBag() == theSmartInfo2->ParamBag())
			{
				// Level tyypit pitääolla samoja ja niiden lukumäärät niin että joko leveleitä on molemmissa
				// tasan yksi tai molemmissa on yli yksi.
				// Ongelmia muuten data päivityksen kanssa jos esim. luotaus datoissa on eri määrä leveleitä
				// tai päivitetään hybridi dataa ja level määrä muuttuu tms.
				FmiLevelType levelType1 = theInfo1->VPlaceDescriptor().Level(0)->LevelType();
				FmiLevelType levelType2 = theSmartInfo2->VPlaceDescriptor().Level(0)->LevelType();
				unsigned long size1 = theInfo1->VPlaceDescriptor().Size();
				unsigned long size2 = theSmartInfo2->VPlaceDescriptor().Size();
				if(levelType1 == levelType2 && (size1 == size2 || (size1 > 1 && size2 > 1)))
				{
					// mahdollinen gridi samoja
					bool status3 = true;
					if(theInfo1->Grid() && theSmartInfo2->Grid())
					{
						status3 = (theInfo1->Grid()->AreGridsIdentical(*theSmartInfo2->Grid())) == true;
					}
					if(status3)
					{
						theInfo1->FirstParam(); // varmistaa, että producer löytyy
						theSmartInfo2->FirstParam();
						if(*theInfo1->Producer() == *theSmartInfo2->Producer())
							return true;
					}
				}
			}
		}
	}
	return false;
}

//--------------------------------------------------------
// Add
//--------------------------------------------------------
//   Laittaa alkuperäisen pointterin listaan - uutta pointteria
//   ei luoda vaan NFmiInfoOrganizer::itsList ottaa pointterin
//   'omistukseen'. Tämän luokan destruktori tuhoaa infot.
bool NFmiInfoOrganizer::Add (NFmiSmartInfo* theInfo)
{
	if(theInfo)
		return itsList.AddEnd(theInfo);
	return false;
}
//--------------------------------------------------------
// Clear
//--------------------------------------------------------
// tuhoaa aina datan
bool NFmiInfoOrganizer::Clear (void)
{
	for(NFmiPtrList<NFmiSmartInfo>::Iterator iter = itsList.Start(); iter.Next(); )
	{
		iter.Current().DestroyData();
		iter.Remove(true);
	}
	itsList.Clear(false);
	return true; // jotain pitäsi varmaan tsekatakin?
}

// 28.09.1999/Marko Tekee uuden kopion editoitavasta datasta
void NFmiInfoOrganizer::UpdateEditedDataCopy(void)
{
	if(fCreateEditedDataCopy)
	{
		if(itsEditedData)
		{
			if(itsEditedDataCopy)
				itsEditedDataCopy->DestroyData();
			delete itsEditedDataCopy;
			itsEditedDataCopy = itsEditedData->Clone();
			itsEditedDataCopy->DataType(NFmiInfoData::kCopyOfEdited);
		}
	}
}

static NFmiQueryData* CreateDefaultMacroParamQueryData(const NFmiArea *theArea, int gridSizeX, int gridSizeY)
{
	NFmiLevelBag levelBag;
	levelBag.AddLevel(NFmiLevel(kFmiGroundSurface, 0)); // ihan mitä puppua vain, ei väliä
	NFmiVPlaceDescriptor vPlace(levelBag);

	NFmiParamBag parBag;
	parBag.Add(NFmiDataIdent(NFmiParam(998, "macroParam", kFloatMissing, kFloatMissing, 1, 0, NFmiString("%.1f"), kLinearly)));
	NFmiParamDescriptor parDesc(parBag);

	NFmiMetTime originTime;
	NFmiTimeBag validTimes(originTime, originTime, 60); // yhden kokoinen feikki timebagi
	NFmiTimeDescriptor timeDesc(originTime, validTimes);

	NFmiGrid grid(theArea, gridSizeX, gridSizeY);
	NFmiHPlaceDescriptor hPlace(grid);

	NFmiQueryInfo info(parDesc, timeDesc, hPlace, vPlace);
	return NFmiQueryDataUtil::CreateEmptyData(info);
}

void NFmiInfoOrganizer::SetMacroParamDataGridSize(int x, int y)
{
	x = FmiMin(x, static_cast<int>(itsMacroParamMaxGridSize.X()));
	y = FmiMin(y, static_cast<int>(itsMacroParamMaxGridSize.Y()));
	x = FmiMax(x, static_cast<int>(itsMacroParamMinGridSize.X()));
	y = FmiMax(y, static_cast<int>(itsMacroParamMinGridSize.Y()));
	itsMacroParamGridSize = NFmiPoint(x, y);
	UpdateMacroParamData();
}
void NFmiInfoOrganizer::SetMacroParamDataMinGridSize(int x, int y)
{
	itsMacroParamMinGridSize = NFmiPoint(x, y);
}
void NFmiInfoOrganizer::SetMacroParamDataMaxGridSize(int x, int y)
{
	itsMacroParamMaxGridSize = NFmiPoint(x, y);
}

void NFmiInfoOrganizer::UpdateMacroParamData(void)
{
	if(itsMacroParamData)
	{
		std::auto_ptr<NFmiArea> arePtr(itsMacroParamData->Area()->Clone());
		UpdateSpecialDataArea(arePtr.get(), itsMacroParamGridSize, NFmiInfoData::kMacroParam, &itsMacroParamData, itsMacroParamMissingValueMatrix);
	}
}

void NFmiInfoOrganizer::UpdateSpecialDataArea(const NFmiArea *theArea, const NFmiPoint &theGridSize, NFmiInfoData::Type theType, NFmiSmartInfo ** theData, NFmiDataMatrix<float> &theMissingValueMatrix)
{
	// tuhoa ensin vanha pois alta
	if(*theData)
		(*theData)->DestroyData();
	delete *theData;
	*theData = 0;

	// Luo sitten uusi data jossa on yksi aika,param ja level ja luo hplaceDesc annetusta areasta ja hila koosta
	NFmiQueryData* data = CreateDefaultMacroParamQueryData(theArea, static_cast<int>(theGridSize.X()), static_cast<int>(theGridSize.Y()));
	if(data)
	{
		NFmiQueryInfo infoIter(data);
		*theData = new NFmiSmartInfo(infoIter, data, "", "", theType);
		theMissingValueMatrix.Resize((*theData)->Grid()->XNumber(), (*theData)->Grid()->YNumber(), kFloatMissing);
	}
}

void NFmiInfoOrganizer::UpdateCrossSectionMacroParamDataSize(int x, int y)
{
	static std::auto_ptr<NFmiArea> dummyArea(new NFmiLatLonArea(NFmiPoint(19,57), NFmiPoint(32,71)));
	// tuhoa ensin vanha pois alta
	if(itsCrossSectionMacroParamData)
		itsCrossSectionMacroParamData->DestroyData();
	delete itsCrossSectionMacroParamData;
	itsCrossSectionMacroParamData = 0;

	// Luo sitten uusi data jossa on yksi aika,param ja level ja luo hplaceDesc annetusta areasta ja hila koosta
	NFmiQueryData* data = CreateDefaultMacroParamQueryData(dummyArea.get(), x, y);
	if(data)
	{
		NFmiQueryInfo infoIter(data);
		itsCrossSectionMacroParamData = new NFmiSmartInfo(infoIter, data, "", "", NFmiInfoData::kCrossSectionMacroParam);
		itsCrossSectionMacroParamMissingValueMatrix.Resize(itsCrossSectionMacroParamData->Grid()->XNumber(), itsCrossSectionMacroParamData->Grid()->YNumber(), kFloatMissing);
	}
}

NFmiSmartInfo* NFmiInfoOrganizer::FindInfo(NFmiInfoData::Type theDataType, int theIndex) // Hakee indeksin mukaisen tietyn datatyypin infon
{
	if(theDataType == NFmiInfoData::kEditable)
		return itsEditedData;
	else if(theDataType == NFmiInfoData::kCopyOfEdited)
		return itsEditedDataCopy;
	else
	{
		int ind = 0;
		for(NFmiPtrList<NFmiSmartInfo>::Iterator iter = itsList.Start(); iter.Next(); )
		{
			if(iter.Current().DataType() == theDataType)
			{
				if(ind == theIndex)
					return &(iter.Current());
				ind++;
			}
		}
	}
	return 0;
}

// Haetaan infoOrganizerista kaikki ne SmartInfot, joihin annettu fileNameFilter sopii.
// Mielestäni vastauksia pitäisi tulla korkeintaan yksi, mutta ehkä tulevaisuudessa voisi tulla lista.
// HUOM! Palauttaa vectorin halutunlaisia infoja, vectori ei omista pointtereita, joten infoja ei saa tuhota delete:llä.
// Ei käy läpi kEditable, eikä kCopyOfEdited erikois datoja!
checkedVector<NFmiSmartInfo*> NFmiInfoOrganizer::GetInfos(const std::string &theFileNameFilter)
{
	checkedVector<NFmiSmartInfo*> infoVector;

	if(theFileNameFilter.empty() == false)
	{
		for(NFmiPtrList<NFmiSmartInfo>::Iterator iter = itsList.Start(); iter.Next(); )
		{
			if(iter.Current().DataFilePattern() == theFileNameFilter)
				infoVector.push_back(&(iter.Current()));
		}
	}
	return infoVector;
}

static bool IsProducerWanted(int theCurrentProdId, int theProducerId1, int theProducerId2, int theProducerId3 = -1, int theProducerId4 = -1)
{
	if(theCurrentProdId == theProducerId1)
		return true;
	else if(theProducerId2 != -1 && theCurrentProdId == theProducerId2)
		return true;
	else if(theProducerId3 != -1 && theCurrentProdId == theProducerId3)
		return true;
	else if(theProducerId4 != -1 && theCurrentProdId == theProducerId4)
		return true;
	return false;
}

// Palauttaa vectorin viewable infoja, vectori ei omista pointtereita,
// joten infoja ei saa tuhota.
checkedVector<NFmiSmartInfo*> NFmiInfoOrganizer::GetInfos(NFmiInfoData::Type theDataType)
{
	checkedVector<NFmiSmartInfo*> infoVector;

	if(theDataType == NFmiInfoData::kEditable)
	{
		if(itsEditedData)
			infoVector.push_back(itsEditedData);
	}
	else if(theDataType == NFmiInfoData::kCopyOfEdited)
	{
		if(itsEditedDataCopy)
			infoVector.push_back(itsEditedDataCopy);
	}
	else
	{
		for(NFmiPtrList<NFmiSmartInfo>::Iterator iter = itsList.Start(); iter.Next(); )
		{
			if(iter.Current().DataType() == theDataType)
				infoVector.push_back(&(iter.Current()));
		}
	}
	return infoVector;
}

// Palauttaa vectorin halutun tuottajan infoja, vectori ei omista pointtereita, joten infoja ei saa tuhota.
// Ei katso tuottaja datoja editable infosta eikä sen kopioista!
// voi antaa kaksi eri tuottaja id:tä jos haluaa, jos esim. hirlamia voi olla kahden eri tuottaja id:n alla
checkedVector<NFmiSmartInfo*> NFmiInfoOrganizer::GetInfos(int theProducerId, int theProducerId2, int theProducerId3, int theProducerId4)
{
	checkedVector<NFmiSmartInfo*> infoVector;

	int currentProdId = 0;
	if(itsEditedData && itsEditedData->IsGrid() == false) // laitetaan myös mahdollisesti editoitava data, jos kyseessä on asema dataa eli havainto
	{
		currentProdId = itsEditedData->Producer()->GetIdent();
		if(::IsProducerWanted(currentProdId, theProducerId, theProducerId2, theProducerId3, theProducerId4))
			infoVector.push_back(itsEditedData);
	}

	for(NFmiPtrList<NFmiSmartInfo>::Iterator iter = itsList.Start(); iter.Next(); ) // HUOM! tässä ei kiinnosta editoitu data tai sen kopio!!!!
	{
	    currentProdId = static_cast<int>(iter.Current().Producer()->GetIdent());
		if(::IsProducerWanted(currentProdId, theProducerId, theProducerId2, theProducerId3, theProducerId4))
			infoVector.push_back(&(iter.Current()));
	}
	return infoVector;
}

// HUOM! Tästä pitää tehdä multithreaddauksen kestävää koodia, eli
// iteraattorista pitää tehdä lokaali kopio.
checkedVector<NFmiSmartInfo*> NFmiInfoOrganizer::GetInfos(NFmiInfoData::Type theType, bool fGroundData, int theProducerId, int theProducerId2)
{
	checkedVector<NFmiSmartInfo*> infoVector;
	NFmiPtrList<NFmiSmartInfo>::Iterator iter = itsList.Start();
	for( ; iter.Next(); ) // HUOM! tässä ei kiinnosta editoitu data tai sen kopio!!!!
	{
		NFmiSmartInfo *info = iter.CurrentPtr();
		if(info && info->DataType() == theType)
		{
			if((fGroundData == true && info->SizeLevels() == 1) || (fGroundData == false && info->SizeLevels() > 1))
			{
				// HUOM! info->Producer() on potentiaalisti vaarallinen kutsu multi-threaddaavassa tilanteessa.
				int currentProdId = static_cast<int>(info->Producer()->GetIdent());
				if(::IsProducerWanted(currentProdId, theProducerId, theProducerId2))
					infoVector.push_back(info);
			}
		}
	}
	return infoVector;
}

// Haetaan halutun datatyypin, tuottajan joko pinta tai level dataa (mahd indeksi kertoo sitten konfliktin
// yhteydessä, monesko otetaan)
NFmiSmartInfo* NFmiInfoOrganizer::FindInfo(NFmiInfoData::Type theDataType, const NFmiProducer &theProducer, bool fGroundData, int theIndex)
{
	if(theDataType == NFmiInfoData::kEditable)
		return itsEditedData;
	else if(theDataType == NFmiInfoData::kCopyOfEdited)
		return itsEditedDataCopy;
	else
	{
		int ind = 0;
		for(NFmiPtrList<NFmiSmartInfo>::Iterator iter = itsList.Start(); iter.Next(); )
		{
			NFmiSmartInfo* info = &(iter.Current());
			if(info && info->DataType() == theDataType)
			{
				info->FirstParam(); // pitää varmistaa, että producer löytyy
				if(*(info->Producer()) == theProducer)
				{
					int levSize = info->SizeLevels();
					if((levSize == 1 && fGroundData) || (levSize > 1 && (!fGroundData)))
					{
						if(ind == theIndex)
							return info;
						ind++;
					}
				}
			}
		}
	}
	return 0;
}

// vastaus 0 = ei ole
// 1 = on sounding dataa, mutta ei välttämättä paras mahd.
// 2 = on hyvää dataa
// Tämä on malli datojen kanssa  niin että painepinta data on 1 ja hybridi on 2
static int IsGoodSoundingData(NFmiSmartInfo* info, const NFmiProducer &theProducer, bool ignoreProducer)
{
	if(info)
	{
		if(ignoreProducer || (*(info->Producer()) == theProducer))
		{
			if(info->SizeLevels() > 3) // pitää olla väh 4 leveliä ennen kuin kelpuutetaan sounding dataksi
			{
				if(info->DataType() == NFmiInfoData::kHybridData)
					return 2;
				else
					return 1;
			}
		}
	}
	return 0;
}

// Hakee parhaan luotaus infon tuottajalle. Eli jos kyseessä esim hirlam tuottaja, katsotaan löytyykö
// hybridi dataa ja sitten tyydytään viewable-dataa (= painepinta)
NFmiSmartInfo* NFmiInfoOrganizer::FindSoundingInfo(const NFmiProducer &theProducer)
{
	NFmiSmartInfo* exceptableInfo = 0;
	for(NFmiPtrList<NFmiSmartInfo>::Iterator iter = itsList.Start(); iter.Next(); )
	{
		NFmiSmartInfo* info = &(iter.Current());
		int result = ::IsGoodSoundingData(info, theProducer, false);
		if(result == 2)
			return info;
		else if(result == 1)
			exceptableInfo = info;
	}

	NFmiSmartInfo* info = FindInfo(NFmiInfoData::kEditable);
	if(info)
	{
		if(theProducer.GetIdent() == kFmiMETEOR || (*info->Producer() == theProducer)) // tässä hanskataan 'editoitu' data, jolloin ignoorataan tuottaja
		{
			info = FindInfo(NFmiInfoData::kEditable);
			int result = ::IsGoodSoundingData(info, theProducer, true);
			if(result != 0)
				exceptableInfo = info;
		}
	}

	return exceptableInfo;
}

void NFmiInfoOrganizer::SetDrawParamPath(const std::string &theDrawParamPath)
{
	if(itsDrawParamFactory)
		itsDrawParamFactory->LoadDirectory(theDrawParamPath);
}

const std::string NFmiInfoOrganizer::GetDrawParamPath(void)
{
	std::string retValue;
	if(itsDrawParamFactory)
		retValue = itsDrawParamFactory->LoadDirectory();
	return retValue;
}

static bool UseParIdOnly(NFmiInfoData::Type theDataType)
{
	if(theDataType == NFmiInfoData::kEditable || theDataType == NFmiInfoData::kCopyOfEdited || theDataType == NFmiInfoData::kAnyData) // jos editoitava data, ei tuottajalla väliä
		return true;
	return false;
}

NFmiSmartInfo* NFmiInfoOrganizer::Info(NFmiDrawParam &theDrawParam, bool fCrossSectionInfoWanted)
{
	NFmiInfoData::Type dataType = theDrawParam.DataType();
	if(fCrossSectionInfoWanted)
		return CrossSectionInfo(theDrawParam.Param(), dataType);
	else
	{
		NFmiLevel* level = &theDrawParam.Level();
		if(level && level->GetIdent() == 0) // jos tämä on ns. default-level otus (GetIdent() == 0), annetaan 0-pointteri Info-metodiin
			level = 0;
		return GetInfo(theDrawParam.Param(), level, dataType, ::UseParIdOnly(dataType));
	}
}

NFmiSmartInfo* NFmiInfoOrganizer::Info(const NFmiDataIdent& theIdent, const NFmiLevel* theLevel, NFmiInfoData::Type theType, bool fUseParIdOnly, bool fLevelData)
{
	if(fLevelData)
		return CrossSectionInfo(theIdent, theType);
	else 
		return GetInfo(theIdent, theLevel, theType, (fUseParIdOnly || ::UseParIdOnly(theType)));
}

NFmiParamBag NFmiInfoOrganizer::GetParams(int theProducerId1)
{
	NFmiParamBag paramBag;
	checkedVector<NFmiSmartInfo*> infos(GetInfos(theProducerId1));
	size_t size = infos.size();
	if(size > 0)
	{
		for(size_t i=0; i<size; i++)
		{
			paramBag = paramBag.Combine(infos[i]->ParamBag());
		}
	}

    return paramBag;
}
