//**********************************************************
// C++ Class Name : NFmiInfoOrganizer 
// ---------------------------------------------------------
// Filetype: (SOURCE)
// Filepath: D:/projekti/GDPro/GDTemp/NFmiInfoOrganizer.cpp 
// 
// 
// GDPro Properties 
// ---------------------------------------------------
//  - GD Symbol Type    : CLD_Class 
//  - GD Method         : UML ( 2.1.4 ) 
//  - GD System Name    : Met-editor Plan 2 
//  - GD View Type      : Class Diagram 
//  - GD View Name      : Markon ehdotus 
// ---------------------------------------------------  
//  Author         : pietarin 
//  Creation Date  : Tues - Feb 9, 1999 
// 
// 
//  Description: 
//   Sisältää mahdollisesti listan infoja, joista 
//   luokka osaa antaa tarvittavan
//   infon pointterin parametrin nimellä. Listalla 
//   ei ole järjestystä.
// 
//  Change Log: 
// 1999.08.26/Marko	Laitoin organizeriin eri tyyppisten datojen talletuksen
//					mahdollisuuden. Editoitavaa dataa voi olla vain yksi, mutta 
//					katsottavaa/maskattavaa dataa voi olla useita erilaisia ja statio-
//					näärisiä (esim. topografia) datoja voi olla myös useita erilaisia. 
// 1999.08.26/Marko	Laitoin level tiedon DrawParam:in luonti-kutsuun varten.
// 1999.09.22/Marko	DrawParamin luonnissa otetaan huomioon datan tyyppi tästälähtien.
// 1999.09.22/Marko	Lisäsin EditedDatan kopion, jota käytetään visualisoimaan dataan tehtyjä muutoksia.
// 
//**********************************************************
#include "NFmiInfoOrganizer.h"
#include "NFmiSmartInfo.h"
#include "NFmiDrawParamFactory.h"
#include "NFmiQueryData.h"
#include "NFmiProducerList.h"
#include "NFmiGrid.h"

#include "stdafx.h" // TRACE-kutsu

//--------------------------------------------------------
// Constructor/Destructor 
//--------------------------------------------------------
// luodaan tyhjä lista
NFmiInfoOrganizer::NFmiInfoOrganizer (bool theIsToolMasterAvailable)
:itsList()
,itsIter(itsList.Start())
,itsDrawParamFactory(0)
,itsWorkingDirectory("")
,itsEditedData(0)
,itsEditedDataCopy(0)
,fToolMasterAvailable(theIsToolMasterAvailable)
{
}

// destruktori tuhoaa infot
NFmiInfoOrganizer::~NFmiInfoOrganizer (void)
{
	Clear();
	delete itsDrawParamFactory;
	if(itsEditedData)
		itsEditedData->DestroySharedData();
	delete itsEditedData;

	if(itsEditedDataCopy)
		itsEditedDataCopy->DestroySharedData();
	delete itsEditedDataCopy;
}


//--------------------------------------------------------
// Init 
//--------------------------------------------------------
FmiBoolean NFmiInfoOrganizer::Init (void)
{
 	itsDrawParamFactory =new NFmiDrawParamFactory;
	itsDrawParamFactory->WorkingDirectory(WorkingDirectory());
 	return itsDrawParamFactory->Init(); 
}

//--------------------------------------------------------
// Info 
//--------------------------------------------------------

//  Palauttaa smartinfon pointterin, jossa on annettu parametri 
//  joko kokonaisena tai jonkin parametrin osaparametrina.
//	Kysytään ensimmäisenä listassa olevalta infolta, onko infolla theParam
//	suoraan. Jos miltään infolta ei suoraan saada theParam:a, tutkitaan
//	löytyykö theParam aliparametrina (esim. pilvisyys voi löytyä parametrin
//	kWeatherAndCloudiness aliparametrina). Periaatteessa theParam voisi löytyä
//	myös aliparametrin aliparametrilta jne - tutkittavia aliparametri-tasoja
//	ei ole rajoitettu. Tässä metodissa tutkitaan vain parameri ja tarvittaessa
//  parametrin aliparametri. 

//  22.02.1999 lisättiin  tunnistin FmiBoolean fSubParameter, josta saadaan tieto
//  siitä, oliko löydetty parametri itsenäinen vaiko jonkin parametrin aliparametri.
NFmiSmartInfo* NFmiInfoOrganizer::Info ( const FmiParameterName& theParam
									   , FmiBoolean& fSubParameter 
									   , const NFmiLevel* theLevel
									   , FmiQueryInfoDataType theType)
{
	if(itsEditedData && itsEditedData->DataType() == theType && itsEditedData->Param(theParam) && (!theLevel || (theLevel && itsEditedData->Level(*theLevel))))
	{
		fSubParameter = itsEditedData->UseSubParam();
		return itsEditedData;
	}

	// 28.09.1999/Marko
	if(itsEditedDataCopy && itsEditedDataCopy->DataType() == theType && itsEditedDataCopy->Param(theParam) && (!theLevel || (theLevel && itsEditedDataCopy->Level(*theLevel))))
	{
		fSubParameter = itsEditedDataCopy->UseSubParam();
		return itsEditedDataCopy;
	}

	NFmiSmartInfo* aInfo = 0;

	NFmiPtrList<NFmiSmartInfo>::Iterator aIter = itsList.Start();
// tutkitaan ensin löytyykö theParam suoraan joltain listassa olevalta NFmiSmartInfo-pointterilta
	while(aIter.Next())
	{
		aInfo = aIter.CurrentPtr();
		if(aInfo->DataType() == theType && aInfo->Param(theParam) && (!theLevel || (theLevel && aInfo->Level(*theLevel))))
		{
			fSubParameter = aInfo->UseSubParam();
			return aInfo; // theParam löytyi suoraan pointterista aInfo
		}
	}
	return 0; // theParam ei löytynyt edes aliparametrina miltään listassa olevalta aInfo-pointterilta
}
//--------------------------------------------------------
// Info 
//--------------------------------------------------------

NFmiSmartInfo* NFmiInfoOrganizer::Info ( const NFmiDataIdent& theDataIdent
									   , FmiBoolean& fSubParameter 
									   , const NFmiLevel* theLevel
									   , FmiQueryInfoDataType theType)
{
	if(itsEditedData && itsEditedData->DataType() == theType && itsEditedData->Param(theDataIdent) && (!theLevel || (theLevel && itsEditedData->Level(*theLevel))))
	{
		fSubParameter = itsEditedData->UseSubParam();
		return itsEditedData;
	}

	// 28.09.1999/Marko
	if(itsEditedDataCopy && itsEditedDataCopy->DataType() == theType && itsEditedDataCopy->Param(theDataIdent) && (!theLevel || (theLevel && itsEditedDataCopy->Level(*theLevel))))
	{
		fSubParameter = itsEditedDataCopy->UseSubParam();
		return itsEditedDataCopy;
	}

	NFmiSmartInfo* aInfo = 0;
// tutkitaan ensin löytyykö theParam suoraan joltain listassa olevalta NFmiSmartInfo-pointterilta
	for(Reset(); Next(); )
	{
		aInfo = Current();
		if(aInfo->DataType() == theType && aInfo->Param(theDataIdent) && (!theLevel || (theLevel && aInfo->Level(*theLevel))))
		{
			fSubParameter = aInfo->UseSubParam();
			return aInfo; // theParam löytyi suoraan pointterista aInfo
		}
	}
	return 0; // theParam ei löytynyt edes aliparametrina miltään listassa olevalta aInfo-pointterilta
}

// itsEditedData infon parambagi
NFmiParamBag NFmiInfoOrganizer::EditedParams(void)
{
	if(itsEditedData)
		return *(itsEditedData->ParamDescriptor().ParamBag());
	return NFmiParamBag();
}

// kaikkien apudatojen parametrit yhdessä bagissa (joita voidaan katsoa/maskata)
NFmiParamBag NFmiInfoOrganizer::ViewableParams(void)
{
	return GetParams(kFmiDataTypeViewable);
}

// vain halutun indeksin parametrit (HUONO VIRITYS KORJAA!!!!)
NFmiParamBag NFmiInfoOrganizer::ViewableParams(int theIndex)
{
	NFmiSmartInfo* info = ViewableInfo(theIndex);
	if(info)
		return info->ParamBag();
	return NFmiParamBag();
}

// vain halutun indeksin parametrit (HUONO VIRITYS KORJAA!!!!)
NFmiParamBag NFmiInfoOrganizer::ObservationParams(int theIndex)
{
	NFmiSmartInfo* info = ObservationInfo(theIndex);
	if(info)
		return info->ParamBag();
	return NFmiParamBag();
}

NFmiParamBag NFmiInfoOrganizer::GetParams(FmiQueryInfoDataType theDataType)
{
	NFmiParamBag paramBag;
	for(Reset();Next();)
	{
		if(Current()->DataType() == theDataType)
			paramBag = paramBag.Combine(Current()->ParamBag());
	}

    return paramBag;
}
/*
NFmiParamBag NFmiInfoOrganizer::GetParams(FmiQueryInfoDataType theDataType)
{
// Luodaan vektori NFmiDataIdent-pointtereille . Vektorin pituus
// saadaan laskemalla kaikkien tietyn tyyppisten infojen parametrien lukumärät yhteen.	
	if(theDataType == kFmiDataTypeEditable)
		return EditedParams();

	long count = 0;
	for(Reset();Next();)
		if(Current()->DataType() == theDataType)
			count += Current()->SizeParams();
	if(count == 0)
		return NFmiParamBag();
	NFmiDataIdent* dataIdents = new NFmiDataIdent[count];

// Käydään läpi kaikki listan itsList SmartInfot
	int ind=0;
	for (Reset();Next();)
	{
//  Kunkin SmartInfon sisällä (attribute itsParamDescriptor) olevat 
//  dataIdentit listätää vektoriin
		if(Current()->DataType() == theDataType)
			for(Current()->ResetParam();Current()->NextParam();ind++)
				dataIdents[ind] = Current()->Param();
	}
// Luodaan palautettava paramBag 
    NFmiParamBag paramBag(dataIdents, count);
    delete[] dataIdents;
    return paramBag;
}
*/
// kaikkien staattisten (ei muutu ajan mukana) datojen parambag (esim. topografia)
NFmiParamBag NFmiInfoOrganizer::StaticParams(void)
{
	return GetParams(kFmiDataTypeStationary);
}

NFmiSmartInfo* NFmiInfoOrganizer::CreateInfo(FmiParameterName theParamName, const NFmiLevel* theLevel, FmiQueryInfoDataType theType)
{
	FmiBoolean aSubParam;	
	NFmiSmartInfo* info = Info(theParamName, aSubParam, theLevel, theType);
	if(info)
	{
		if(info->Param(theParamName))
			return info->Clone();
	}
	return 0;
}

NFmiSmartInfo* NFmiInfoOrganizer::CreateInfo(const NFmiDataIdent& theDataIdent, const NFmiLevel* theLevel, FmiQueryInfoDataType theType)
{
	FmiBoolean aSubParam;	
	NFmiSmartInfo* info = Info(theDataIdent, aSubParam, theLevel, theType);
	if(info)
	{
		if(info->Param(theDataIdent))
			return info->Clone();
	}
	return 0;
}

NFmiSmartInfo* NFmiInfoOrganizer::CreateInfo(NFmiSmartInfo* theUsedInfo, const NFmiDataIdent& theDataIdent, const NFmiLevel* theLevel, FmiQueryInfoDataType theType)
{
	FmiBoolean aSubParam = kFalse;	
	if(theUsedInfo && theUsedInfo->DataType() == theType && theUsedInfo->Param(theDataIdent) && (!theLevel || (theLevel && theUsedInfo->Level(*theLevel))))
	{
		aSubParam = theUsedInfo->UseSubParam();
		NFmiSmartInfo* copyOfInfo = new NFmiSmartInfo(*theUsedInfo);
		return copyOfInfo;
	}
	return 0;
}

//--------------------------------------------------------
// CreateDrawParam(FmiParameterName theParamName)
//--------------------------------------------------------
// Tutkii löytyykö listasta itsList infoa, jossa on theParam.
// Jos tälläinen info löytyy, pyydetään itsDrawParamFactory luomaan
// drawParam kyseiselle parametrille löydetyn infon avulla.
NFmiDrawParam* NFmiInfoOrganizer::CreateDrawParam(FmiParameterName theParamName, const NFmiLevel* theLevel, FmiQueryInfoDataType theType)
{
// Huomaa, että palautettava pointteri drawParam luodaan attribuutin 
// itsDrawParamFactory sisällä new:llä, joten drawParam  
// pitää muistaa tuhota  NFmiInfoOrganizer:n ulkopuolella

	NFmiDrawParam* drawParam = 0;
	FmiBoolean aSubParam;	
	NFmiSmartInfo* info = Info(theParamName, aSubParam, theLevel, theType);
	if(info)
	{
		if(info->Param(theParamName))
		{
			NFmiDataIdent dataIdent(info->Param());
			return CreateDrawParam(dataIdent, theLevel, theType);
		}
		if(aSubParam)
		{
			info->FirstParam();
			NFmiDataIdent dataIdent(info->Param());
			dataIdent.GetParam()->SetIdent(theParamName);
			return CreateDrawParam(dataIdent, theLevel, theType);
		}
	}
	return drawParam;
}

//--------------------------------------------------------
// CreateDrawParam(NFmiDataIdent& theDataIdent)
//--------------------------------------------------------
// Tutkii löytyykö listasta itsList infoa, jossa on theDataIdent - siis
// etsitään info, jonka tuottaja ja parametri saadaan theDataIdent:stä.
// Jos tälläinen info löytyy, pyydetään itsDrawParamFactory luomaan
// drawParam kyseiselle parametrille löydetyn infon avulla.
NFmiDrawParam* NFmiInfoOrganizer::CreateDrawParam(const NFmiDataIdent& theIdent, const NFmiLevel* theLevel, FmiQueryInfoDataType theType)
{
// Huomaa, että itsDrawParamFactory luo pointterin drawParam new:llä, joten   
// drawParam pitää muistaa tuhota  NFmiInfoOrganizer:n ulkopuolella
	int thePriority = 1;// toistaiseksi HARDCODE, thePriority määritys tehdään myöhemmin
	NFmiDrawParam* drawParam = 0;
	FmiBoolean aSubParam;	
	NFmiSmartInfo* info = Info(theIdent, aSubParam, theLevel, theType);
	if(info)
	{
		NFmiSmartInfo* copyOfInfo = new NFmiSmartInfo(*info);
		drawParam = itsDrawParamFactory->CreateDrawParam(copyOfInfo, theIdent, aSubParam, theLevel, fToolMasterAvailable);	
	}
	return drawParam;
}

// Luo halutun drawparam:in, mutta käyttäen annettua smartinfoa. 
// Käytetään kun ratkaistaan parametrin tasaus + maski ongelmaa (maski muuttuu kun 
// tasausta suoritetaan, joten pitää käyttää kopiota)
NFmiDrawParam* NFmiInfoOrganizer::CreateDrawParam(NFmiSmartInfo* theUsedInfo
												 ,const NFmiDataIdent& theDataIdent
												 ,const NFmiLevel* theLevel
//												 ,FmiSmartInfoDataType theType)
												 ,FmiQueryInfoDataType theType)
{
	NFmiDrawParam* drawParam = 0;
	FmiBoolean aSubParam = kFalse;	
	if(theUsedInfo && theUsedInfo->DataType() == theType && theUsedInfo->Param(theDataIdent) && (!theLevel || (theLevel && theUsedInfo->Level(*theLevel))))
	{
		aSubParam = theUsedInfo->UseSubParam();
		NFmiSmartInfo* copyOfInfo = new NFmiSmartInfo(*theUsedInfo);
		drawParam = itsDrawParamFactory->CreateDrawParam(copyOfInfo, theDataIdent, aSubParam, theLevel, fToolMasterAvailable);	
	}
	return drawParam;
}

//--------------------------------------------------------
// AddData 
//--------------------------------------------------------
// HUOM!!!! Tänne ei ole sitten tarkoitus antaa kFmiDataTypeCopyOfEdited-tyyppistä
// dataa, koska se luodaan kun tänne annetaan editoitavaa dataa.
FmiBoolean NFmiInfoOrganizer::AddData(NFmiQueryData* theData
									 ,const NFmiString& theDataFileName
//									 ,FmiSmartInfoDataType theDataType
									 ,FmiQueryInfoDataType theDataType
									 ,int theUndoLevel)
{
	FmiBoolean status = kFalse;
	TRACE("Dataa NFmiInfoOrganizerissa ennen AddData:a %d kpl.\n", CountData());
	if(theData)
	{
		NFmiQueryInfo aQueryInfo(theData);
		NFmiSmartInfo* aSmartInfo = new NFmiSmartInfo(aQueryInfo, theData, theDataFileName, theDataType);// ...otetaan käyttöön myöhemmin
		if(aSmartInfo)
			aSmartInfo->First();

		if(theDataType == kFmiDataTypeEditable)
		{
			if(theUndoLevel)
				aSmartInfo->UndoLevel(theUndoLevel);
			if(itsEditedData)
			{
				itsEditedData->DestroySharedData();
				delete itsEditedData;
			}
			itsEditedData = aSmartInfo;
			UpdateEditedDataCopy();
			status = kTrue;
		}
		else
			status = Add(aSmartInfo); // muun tyyppiset datat kuin editoitavat menevät listaan
	}
	TRACE("Dataa NFmiInfoOrganizerissa jälkeen AddData:a %d kpl.\n", CountData());
	return status;
}

void NFmiInfoOrganizer::ClearData(FmiQueryInfoDataType theDataType)
{
	if(theDataType == kFmiDataTypeEditable)
	{
		if(itsEditedData)
		{
			itsEditedData->DestroySharedData();
			delete itsEditedData;
			itsEditedData = 0;
		}
	}
	else
	{ // käydään lista läpi ja tuhotaan halutun tyyppiset datat
		for(Reset(); Next(); )
		{
			if(Current()->DataType() == theDataType)
			{
				Current()->DestroySharedData();
				Remove();
			}
		}
	}
}

void NFmiInfoOrganizer::ClearThisKindOfData(NFmiQueryInfo* theInfo)
{
	if(theInfo)
	{
		if(itsEditedData)
		{
			if(IsInfosTwoOfTheKind(theInfo, itsEditedData))
			{
				itsEditedData->DestroySharedData();
				delete itsEditedData;
				itsEditedData = 0;
				return;
			}
		}

		for(Reset(); Next(); )
		{
			if(IsInfosTwoOfTheKind(theInfo, Current()))
			{
				Current()->DestroySharedData();
				Remove();
				break; // tuhotaan vain yksi!
			}
		}
	}
}

int NFmiInfoOrganizer::CountData(void)
{
	int count = 0;
	if(itsEditedData)
		count++;

	count += itsList.NumberOfItems();
	return count;
}

// this kind of määritellään tällä hetkellä:
// parametrien, leveleiden ja mahdollisen gridin avulla (ei location bagin avulla)
bool NFmiInfoOrganizer::IsInfosTwoOfTheKind(NFmiQueryInfo* theInfo1, NFmiQueryInfo* theInfo2)
{
	// parametrit ja tuottajat samoja
	bool status1 = (theInfo1->ParamBag() == theInfo2->ParamBag()) == kTrue;

	// level jutut samoja
	bool status2 = (theInfo1->VPlaceDescriptor() == theInfo2->VPlaceDescriptor()) == kTrue;

	// mahdollinen gridi samoja
	bool status3 = kTrue;
	if(theInfo1->Grid() && theInfo2->Grid())
	{
		status3 = (theInfo1->Grid()->AreGridsIdentical(*theInfo2->Grid())) == kTrue;
	}
	return status1 && status2 && status3;
}

//--------------------------------------------------------
// ProducerList 
//--------------------------------------------------------
// EI TOIMI TEHTYJEN MUUTOSTEN JÄLKEEN (1999.08.26/Marko), KORJAA!!!
NFmiProducerList* NFmiInfoOrganizer::ProducerList(void)
{
// duplikaattien poisto tehtävä joskus...
	NFmiProducerList* prodList = new NFmiProducerList;
	for(Reset();Next();)
	{
		NFmiProducer* produ = new NFmiProducer(*(Current()->Param().GetProducer()));
		prodList->Add(produ);
	}
	return prodList;
}

//--------------------------------------------------------
// Add 
//--------------------------------------------------------
//   Laittaa alkuperäisen pointterin listaan - uutta pointteria
//   ei luoda vaan NFmiInfoOrganizer::itsList ottaa pointterin 
//   'omistukseen'. Tämän luokan destruktori tuhoaa infot.
FmiBoolean NFmiInfoOrganizer::Add (NFmiSmartInfo* theInfo)
{
	return itsList.InsertionSort(theInfo);
}
//--------------------------------------------------------
// Clear 
//--------------------------------------------------------
// tuhoaa aina datan
FmiBoolean NFmiInfoOrganizer::Clear (void)
{ 
	for( Reset(); Next(); )
	{
		Current()->DestroySharedData();
		delete Current();
	}
	itsList.Clear(kFalse);
	return kTrue; // jotain pitäsi varmaan tsekatakin? 
}
 
//--------------------------------------------------------
// Reset 
//--------------------------------------------------------
FmiBoolean NFmiInfoOrganizer::Reset (void)
{
   itsIter = itsList.Start();
   return kTrue;
}
//--------------------------------------------------------
// Next 
//--------------------------------------------------------
FmiBoolean NFmiInfoOrganizer::Next (void)
{
   return itsIter.Next();
}
//--------------------------------------------------------
// Current 
//--------------------------------------------------------
NFmiSmartInfo* NFmiInfoOrganizer::Current (void)
{
   return itsIter.CurrentPtr();
}

//--------------------------------------------------------
// Remove 
//--------------------------------------------------------
FmiBoolean NFmiInfoOrganizer::Remove(void)
{
	return itsIter.Remove(kTrue);
}

// tämä toimii vajavaisesti, koska se palauttaa aina 1. kyseisen tyyppisen infon
NFmiSmartInfo* NFmiInfoOrganizer::ViewableInfo(void)
{
	for(Reset(); Next();)
	{
		if(Current()->DataType() == kFmiDataTypeViewable)
			return Current();
	}
	return 0;
}

// Tämä on pikaviritys fuktio (kuten nimestäkin voi päätellä)
// Tarvitaan vielä kun pelataan parametrin valinnassa popupien kanssa (ja niiden kanssa on vaikea pelata).
NFmiLevelBag* NFmiInfoOrganizer::GetAndCreateViewableInfoWithManyLevelsOrZeroPointer(void)
{
	NFmiLevelBag* levels = 0;
	for(Reset(); Next();)
	{
		if(Current()->DataType() == kFmiDataTypeViewable)
			if(Current()->SizeLevels() > 1)
			{
				levels = new NFmiLevelBag(*Current()->VPlaceDescriptor().Levels());
				break;
			}
	}
	return levels;
}

// palauttaa halutun indeksin infon (huono viritys, KORJAA!!!!)
NFmiSmartInfo* NFmiInfoOrganizer::ViewableInfo(int theIndex)
{
	int ind = 0;
	for(Reset(); Next();)
	{
		if(Current()->DataType() == kFmiDataTypeViewable)
		{
			if(ind == theIndex)
				return Current();
			ind++;
		}
	}
	return 0;
}

// palauttaa halutun indeksin infon (huono viritys, KORJAA!!!!)
NFmiSmartInfo* NFmiInfoOrganizer::ObservationInfo(int theIndex)
{
	int ind = 0;
	for(Reset(); Next();)
	{
		if(Current()->DataType() == kFmiDataTypeObservations)
		{
			if(ind == theIndex)
				return Current();
			ind++;
		}
	}
	return 0;
}

// 28.09.1999/Marko Tekee uuden kopion editoitavasta datasta
void NFmiInfoOrganizer::UpdateEditedDataCopy(void)
{
	if(itsEditedData)
	{
		if(itsEditedDataCopy)
			itsEditedDataCopy->DestroySharedData();
		delete itsEditedDataCopy;
		itsEditedDataCopy = itsEditedData->Clone();
		itsEditedDataCopy->DataType(kFmiDataTypeCopyOfEdited);
	}
}

// kaikkien staattisten (ei muutu ajan mukana) datojen parambag (esim. topografia)
NFmiParamBag NFmiInfoOrganizer::ObservationParams(void)
{
	return GetParams(kFmiDataTypeObservations);
}

// tämä toimii vajavaisesti, koska se palauttaa aina 1. kyseisen tyyppisen infon
NFmiSmartInfo* NFmiInfoOrganizer::ObservationInfo(void)
{
	for(Reset(); Next();)
	{
		if(Current()->DataType() == kFmiDataTypeObservations)
			return Current();
	}
	return 0;
}

// tämä toimii vajavaisesti, koska se palauttaa aina 1. kyseisen tyyppisen infon
NFmiSmartInfo* NFmiInfoOrganizer::KepaDataInfo(void)
{
	for(Reset(); Next();)
	{
		if(Current()->DataType() == kFmiDataTypeKepaData)
			return Current();
	}
	return 0;
}

// tämä toimii vajavaisesti, koska se palauttaa aina 1. kyseisen tyyppisen infon
NFmiSmartInfo* NFmiInfoOrganizer::ClimatologyInfo(void)
{
	for(Reset(); Next();)
	{
		if(Current()->DataType() == kFmiDataTypeClimatologyData)
			return Current();
	}
	return 0;
}

NFmiSmartInfo* NFmiInfoOrganizer::AnalyzeDataInfo(void) // tämä toimii vajavaisesti, koska se palauttaa aina 1. kyseisen tyyppisen infon
{
	return FindInfo(kFmiDataTypeAnalyzeData);
}

NFmiSmartInfo* NFmiInfoOrganizer::FindInfo(FmiQueryInfoDataType theDataType) // Hakee 1. tietyn datatyypin infon
{
	return FindInfo(theDataType, 0); // indeksi alkaa 0:sta!!!
}

NFmiSmartInfo* NFmiInfoOrganizer::FindInfo(FmiQueryInfoDataType theDataType, int theIndex) // Hakee indeksin mukaisen tietyn datatyypin infon
{
	int ind = 0;
	for(Reset(); Next();)
	{
		if(Current()->DataType() == theDataType)
		{
			if(ind == theIndex)
				return Current();
			ind++;
		}
	}
	return 0;
}

// tämä toimii vajavaisesti, koska se palauttaa aina 1. kyseisen tyyppisen infon
NFmiSmartInfo* NFmiInfoOrganizer::CompareModelsInfo(void)
{
	for(Reset(); Next();)
	{
		if(Current()->DataType() == kFmiDataTypeCompareModels)
			return Current();
	}
	return 0;
}

// palauttaa vain 1. kyseisen datan parametrit!
NFmiParamBag NFmiInfoOrganizer::CompareModelsParams(void)
{
//	return GetParams(kFmiDataTypeCompareModels);
	NFmiSmartInfo* info = CompareModelsInfo();
	if(info)
		return *(info->ParamDescriptor().ParamBag());
	return NFmiParamBag();
}

NFmiSmartInfo* NFmiInfoOrganizer::CompareModelsInfo(const NFmiProducer& theProducer)
{
	for(Reset(); Next();)
	{
		NFmiSmartInfo* info = Current();
		if(!info->IsParamUsable())
			info->FirstParam();
		NFmiProducer* producer = info->Producer();
		if(info->DataType() == kFmiDataTypeCompareModels && (*producer == theProducer))
			return Current();
	}
	return 0;
}
