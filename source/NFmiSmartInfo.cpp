//**********************************************************
// C++ Class Name : NFmiSmartInfo 
// ---------------------------------------------------------
// Filetype: (SOURCE)
// Filepath: D:/projekti/GDPro/GDTemp/NFmiSmartInfo.cpp 
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
// 
//  Change Log: 
// Changed 1999.08.25/Marko	Lisäsin itsDataType dataosan, jonka avulla smartinfoja voidaan
//							järjestää NFmiInfoOrganizer:in avulla.
// Changed 1999.09.06/Marko Muutin LocationMaskStep()-metodien rajapintaa niin, että voidaan 
//							määrätä resetoidaanko ensin arvot vai ei.
// 
//**********************************************************
#ifdef WIN32
#pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta (liian pitkä nimi >255 merkkiä joka johtuu 'puretuista' STL-template nimistä)
#endif

#include "NFmiSmartInfo.h"
//#include "FmiNMeteditLibraryDefinitions.h"
#include "NFmiUndoableMultiLevelMask.h"
#include "NFmiMultiLevelMask.h"
#include "NFmiQueryData.h"
#include "NFmiGrid.h"
#include "NFmiCombinedParam.h"
#include "NFmiMetEditorTypes.h"

NFmiSmartInfo::NFmiSmartInfo()
:NFmiFastQueryInfo()
,itsAreaMask(0)
,itsAreaFactors(0)
,itsDataReference(0)
,itsDataFileName("")
,itsPriority(0)
,itsEditedParamBag(0)
,itsDataType(NFmiInfoData::kEditable) // 1999.08.24/Marko
{
	fDirty = new bool;
	*fDirty = false;
	fLoadedFromFile = new bool;
	*fLoadedFromFile = false;
	itsMaxUndoLevelPtr = NULL;
	itsMaxRedoLevelPtr = NULL;
	itsCurrentUndoLevelPtr = NULL;
	itsCurrentRedoLevelPtr = NULL;
	itsUndoTable = NULL;
	itsUndoTextTable = NULL;
	itsEditedParamBag = new NFmiParamBag;
}

NFmiSmartInfo::NFmiSmartInfo(const NFmiQueryInfo & theInfo, NFmiQueryData* theData
							 ,NFmiString theDataFileName
							 ,NFmiInfoData::Type theType)
:NFmiFastQueryInfo(theInfo)
,itsDataType(theType) // 1999.08.24/Marko
{
	itsDataReference = theData;
	itsDataFileName = theDataFileName;
	itsAreaMask = new NFmiUndoableMultiLevelMask(HPlaceDescriptor().Size());
	itsAreaFactors = 0;
	fEditable = true; //mikä alkuarvoksi?!! 
	itsPriority = 0;

	fDirty = new bool;
	*fDirty = false;
	fLoadedFromFile = new bool;
	*fLoadedFromFile = false;

	itsMaxUndoLevelPtr = NULL; 
	itsMaxRedoLevelPtr = NULL;
	itsCurrentUndoLevelPtr = NULL;
	itsCurrentRedoLevelPtr = NULL;
	itsUndoTable = NULL;
	itsUndoTextTable = NULL;

	NFmiParamDescriptor& temp = const_cast<NFmiParamDescriptor&>(theInfo.ParamDescriptor());
	itsEditedParamBag = new NFmiParamBag(*(temp.ParamBag()));
	InitEditedParamBag();
}

NFmiSmartInfo::NFmiSmartInfo (const NFmiSmartInfo & theInfo)
:NFmiFastQueryInfo(theInfo)
,itsDataFileName(theInfo.itsDataFileName)
,itsDataType(theInfo.itsDataType)
{
	this->itsPriority = theInfo.itsPriority;
	this->fEditable = theInfo.fEditable;
	
	this->fDirty = theInfo.fDirty;
	this->fLoadedFromFile = theInfo.fLoadedFromFile;

	//seuraavista vain osoite
	this->itsMaxUndoLevelPtr = theInfo.itsMaxUndoLevelPtr;
	this->itsMaxRedoLevelPtr = theInfo.itsMaxRedoLevelPtr;
	this->itsCurrentUndoLevelPtr = theInfo.itsCurrentUndoLevelPtr;
	this->itsCurrentRedoLevelPtr = theInfo.itsCurrentRedoLevelPtr;
	this->itsUndoTable = theInfo.itsUndoTable;
	this->itsUndoTextTable = theInfo.itsUndoTextTable;		
	this->itsAreaMask = theInfo.itsAreaMask; 
	this->itsAreaFactors = theInfo.itsAreaFactors; 
	this->itsDataReference = theInfo.itsDataReference;
	this->itsEditedParamBag = theInfo.itsEditedParamBag;
}

NFmiSmartInfo::~NFmiSmartInfo()
{
}

void NFmiSmartInfo::DestroyData(void)
{
	//kopio-konstruktori kopioi näistä vain osoitteet, 
	//joten nämä saa tuhota vaan yhdestä oliosta, käyttäjän pitää tietää mistä ja milloin 
	if(itsUndoTable!=NULL)
	{
		for (int level = 0; level < (*itsMaxUndoLevelPtr); level++)
		{
			if (itsUndoTable[level])
				delete itsUndoTable[level];
		}
	}
	if (itsUndoTable)
	{
		delete [] itsUndoTable;
		itsUndoTable = NULL;
	}	
	if (itsUndoTextTable)
	{
		delete [] itsUndoTextTable;
		itsUndoTextTable = NULL;
	}

	if (itsMaxUndoLevelPtr)
	{
		delete itsMaxUndoLevelPtr;
		itsMaxUndoLevelPtr = NULL;
	}

	if (itsMaxRedoLevelPtr)
	{
		delete itsMaxRedoLevelPtr;
		itsMaxRedoLevelPtr = NULL;
	}

	if (itsCurrentUndoLevelPtr)
	{
		delete itsCurrentUndoLevelPtr;
		itsCurrentUndoLevelPtr = NULL;
	}
	
	if (itsCurrentRedoLevelPtr)
	{
		delete itsCurrentRedoLevelPtr;
		itsCurrentRedoLevelPtr = NULL;
	}

	delete itsAreaFactors;
	delete itsAreaMask;
	itsAreaMask = 0;

	delete itsDataReference;
	itsDataReference = 0;

	delete fDirty;
	fDirty = 0;
	delete fLoadedFromFile;
	fLoadedFromFile = 0;
	
	delete itsEditedParamBag;
	itsEditedParamBag = 0;
}

//--------------------------------------------------------
// Clone				M.K. 31.3.99 
//--------------------------------------------------------
// Tämä metodi ei kopioi seuraavia: (ei edes pointtereita)
// * Undo/Redo -toimintoihin liittyvät ominaisuudet
// * itsAreaFactors
// * fDirty (asetetaan aina puhtaaksi) 
// * fEditable 
// * itsPriority
NFmiSmartInfo* NFmiSmartInfo::Clone(void)
{
	if(this != 0)
	{
		NFmiQueryData* queryDataCopy = new NFmiQueryData(*(this->itsDataReference));
		NFmiQueryInfo queryInfo(queryDataCopy);
		NFmiSmartInfo* copy = new NFmiSmartInfo(queryInfo, queryDataCopy, this->itsDataFileName
											   ,this->itsDataType); // 1999.08.24/Marko
		copy->itsDataFileName = this->itsDataFileName;
		
		*(copy->itsAreaMask) = *(this->itsAreaMask); // laura muutti 06051999
		
		*(copy->fDirty) = false; //laura 07051999
		*(copy->fLoadedFromFile) = *(this->fLoadedFromFile); //laura 07051999
		copy->itsMaxUndoLevelPtr = 0;
		copy->itsMaxRedoLevelPtr = 0;
		copy->itsCurrentUndoLevelPtr = 0;
		copy->itsCurrentRedoLevelPtr = 0;
		copy->itsUndoTable = 0;
		copy->itsUndoTextTable = 0;

		copy->SetDescriptors(this, false);

		return copy;
	}
	else
		return 0;
}

bool NFmiSmartInfo::InitEditedParamBag()
{
	for(itsEditedParamBag->Reset(); itsEditedParamBag->Next();)
		itsEditedParamBag->SetActive(itsEditedParamBag->CurrentParam(),false);
	return true;
	
}

//--------------------------------------------------------
// AreaMask				M.K. 1.4.99 
//--------------------------------------------------------
//void NFmiSmartInfo::AreaMask(NFmiMultiLevelMask* theAreaMask)
void NFmiSmartInfo::AreaMask(NFmiUndoableMultiLevelMask* theAreaMask)
{
	delete itsAreaMask;
	itsAreaMask = theAreaMask;			// Huom vain pointteri!!!!!
	return;
}

NFmiSmartInfo& NFmiSmartInfo::operator=(const NFmiSmartInfo& theSmartInfo)
{
	NFmiFastQueryInfo::operator=(theSmartInfo);

	this->itsDataFileName = theSmartInfo.itsDataFileName;
	this->itsPriority = theSmartInfo.itsPriority;
	this->fEditable = theSmartInfo.fEditable;
	this->fDirty = theSmartInfo.fDirty; //laura 07051999
	this->fLoadedFromFile = theSmartInfo.fLoadedFromFile; //laura 07051999

	//seuraavista vain osoitteet
	this->itsMaxUndoLevelPtr = theSmartInfo.itsMaxUndoLevelPtr;
	this->itsMaxRedoLevelPtr = theSmartInfo.itsMaxRedoLevelPtr;
	this->itsCurrentUndoLevelPtr = theSmartInfo.itsCurrentUndoLevelPtr;
	this->itsCurrentRedoLevelPtr = theSmartInfo.itsCurrentRedoLevelPtr;
	this->itsUndoTable = theSmartInfo.itsUndoTable;
	this->itsUndoTextTable = theSmartInfo.itsUndoTextTable;	
	this->itsAreaMask = theSmartInfo.itsAreaMask; 
	this->itsAreaFactors = theSmartInfo.itsAreaFactors;
	this->itsDataReference = theSmartInfo.itsDataReference;
	
	this->itsEditedParamBag = theSmartInfo.itsEditedParamBag;
	this->itsDataType = theSmartInfo.itsDataType;
	return *this;
}

bool NFmiSmartInfo::operator==(const NFmiSmartInfo& theSmartInfo) const
{
	if(this->itsPriority == theSmartInfo.itsPriority)
		return true;
	else
		return false;
}

bool NFmiSmartInfo::operator<(const NFmiSmartInfo& theSmartInfo) const
{
	if(this->itsPriority < theSmartInfo.itsPriority)
		return true;
	else
		return false;
}

bool NFmiSmartInfo::MaskByArea(const NFmiArea &theArea, unsigned long theMaskType)
{
	unsigned long oldMask = MaskType();
	MaskType(NFmiMetEditorTypes::kFmiNoMask);
	for(ResetLocation(); NextLocation();)
	{
		NFmiPoint latlon = LatLon();
		if(theArea.IsInside(latlon))
			MaskLocation(true,theMaskType);
		else
			MaskLocation(false,theMaskType);
	}
	MaskType(oldMask);
	return true;
}

//--------------------------------------------------------
// MaskAllLocations 
//--------------------------------------------------------
//   Aktivoi tai deaktivoi kaikki locationit.
//   
void NFmiSmartInfo::MaskAllLocations (const bool& newState, 
									  unsigned long theMaskType)
{
	(*itsAreaMask)->MaskAll(newState,theMaskType);
}

void NFmiSmartInfo::MaskAllLocations (const bool& newState)
{
   (*itsAreaMask)->MaskAll(newState);
}

//--------------------------------------------------------
// Next 
//--------------------------------------------------------
//   Siirtää 'iteraattorin' osoittamaan seuraavaa 
//   maskattua paikkaa.
//   Kutsutaan emon Next()-metodia ja katsotaan, 
//   mikä itsAreaMask-olio 
//   palauttaa. Jos Maski on true, palautetaan 
//   true, muuten kutsutaan
//   taas emon Next:iä ja jatketaan kunnes emon 
//   Next palauttaa falsen.

bool NFmiSmartInfo::NextLocation()
{
	bool returnVal = false;

	while((returnVal==false) && NFmiFastQueryInfo::NextLocation())
	{
		returnVal = (*itsAreaMask)->IsMasked(LocationIndex());
	}
	return returnVal;
}

//--------------------------------------------------------
// IsMaskedLocation 
//--------------------------------------------------------
//
//   Kysy hplacedesc:in indeksia ja kysy sitten 
//   itsMaskArea-oliolta onko
//   maski päällä.
bool NFmiSmartInfo::IsMaskedLocation (unsigned long theMaskType) const
{
	return (*itsAreaMask)->IsMasked(LocationIndex(),theMaskType);
}

//--------------------------------------------------------
// LocationMask 
//--------------------------------------------------------

//   Palauttaa HPlaceDesc:issa käytetyn maskin 
//   referenssin, riippuen 
//   itsMaskStatesta (huom nomask palauttaa dummy 
//   otuksen ja jos 
//   on monta maskStatea kerralla päällä, palautetaan 
//   ensimmäinen).
const NFmiBitMask& NFmiSmartInfo::LocationMask (unsigned long theMaskType) const
{
	return (*itsAreaMask)->Mask(theMaskType);
}
   
//--------------------------------------------------------
// LocationMask 
//--------------------------------------------------------

//   Asettaa ulkopuolella rakennetun maskin HPlaceDesc:in 
//   maskiksi., 
//   riippuen itsMaskStatesta (huom nomask ei 
//   tee mitään ja jos 
//   on monta maskStatea kerralla päällä, asetetaan 
//   ensimmäinen).
void NFmiSmartInfo::LocationMask (const NFmiBitMask& theMask, unsigned long theMaskType)
{
    (*itsAreaMask)->Mask(theMask,theMaskType);
}

//--------------------------------------------------------
// SnapShotData			M.K.
//--------------------------------------------------------
//   Makes 'snapshot' of the data for the undo/redo 
//   purpose. Time and parameter are saved
//   in querydata to make 'undo text'? "Undo temperature 
//   at 1998-12-09-12:00"  Tallettaa vain raakadatan, mutta 
//   ei esim. infon iteraattorien tiloja tai muuta 
//   sellaista. theAction on kuvaileva sana tehdystä 
//   toiminnosta ("aikasarjamuutos", "kursori 
//   ylös", "aluemuutos")

bool NFmiSmartInfo::SnapShotData (const NFmiString& theAction)
{
	if(itsCurrentUndoLevelPtr == 0)
		return false;
	if(!itsUndoTable || !itsUndoTextTable)
		return false;

	if (*itsMaxUndoLevelPtr <= 0)
		return false;

	if (*itsCurrentUndoLevelPtr == *itsMaxUndoLevelPtr - 1)
		RearrangeUndoTable();

	(*itsCurrentUndoLevelPtr)++;
	memcpy(itsUndoTable[*itsCurrentUndoLevelPtr], itsRefDataPool->Data(), 
				itsRefDataPool->Size());
	(*itsCurrentRedoLevelPtr) = (*itsCurrentUndoLevelPtr);
	(*itsMaxRedoLevelPtr) = (*itsCurrentRedoLevelPtr);

	itsUndoTextTable[*itsCurrentUndoLevelPtr] += theAction;

	return true;
}


//--------------------------------------------------------
// RearrangeUndoTable			M.K.
//--------------------------------------------------------
void NFmiSmartInfo::RearrangeUndoTable(void)
{
	if(itsCurrentUndoLevelPtr == 0)
		return ;
	char* undoTmp = itsUndoTable[0];
	NFmiString undoTmpText = itsUndoTextTable[0];

	for (int i = 0; i < (*itsMaxUndoLevelPtr) - 1; i++)
	{
		itsUndoTable[i] = itsUndoTable[i + 1];
		itsUndoTextTable[i] = itsUndoTextTable[i + 1];
	}

	itsUndoTable[*itsMaxUndoLevelPtr - 1] = undoTmp;
	itsUndoTextTable[*itsMaxUndoLevelPtr - 1] = undoTmpText;

	(*itsCurrentUndoLevelPtr)--;
	(*itsCurrentRedoLevelPtr)--;

	return;
}

//--------------------------------------------------------
// SnapShotData			M.K.
//--------------------------------------------------------
//   Makes 'snapshot' of the data for the undo/redo 
//   purpose. Only parameter is saved
//   in querydata to make 'undo text'? "Undo temperature". 
//    theAction on kuvaileva sana 
//   tehdystä toiminnosta ("aikasarjamuutos", 
//   "kursori ylös", "aluemuutos").

bool NFmiSmartInfo::SnapShotData (const NFmiString& theAction
 ,FmiParameterName theParameter)
{
   bool returnVal = false;
   return returnVal;
}

//--------------------------------------------------------
// UndoText			M.K. 
//--------------------------------------------------------
//   Return text that describes the operation 
//   that undo will revert next. E.g.
//   "Undo temperature at 1998-12-09-12:00"

NFmiString NFmiSmartInfo::UndoText (void)
{
	NFmiString undoText("");
	if(itsCurrentUndoLevelPtr == 0)
		return undoText;
	if (*itsCurrentUndoLevelPtr >= 0)
	{
		undoText += NFmiString("Undo ");
		undoText += itsUndoTextTable[*itsCurrentUndoLevelPtr];
	}		
	return undoText;
}
//--------------------------------------------------------
// RedoText			M.K. 
//--------------------------------------------------------
NFmiString NFmiSmartInfo::RedoText (void)
{
	NFmiString undoText("");
	if(itsCurrentUndoLevelPtr == 0)
		return undoText;
	if(!((*itsCurrentRedoLevelPtr) == (*itsCurrentUndoLevelPtr) 
	   || (*itsCurrentRedoLevelPtr) == ((*itsCurrentUndoLevelPtr) + 1)))
	{
		undoText += NFmiString("Redo ");
		undoText += itsUndoTextTable[(*itsCurrentUndoLevelPtr) + 1];
	}
	return undoText;
}
//--------------------------------------------------------
// Undo				M.K. 
//--------------------------------------------------------
// Kertoo onko undo mahdollista suorittaa.
bool NFmiSmartInfo::Undo (void)
{
	if(itsCurrentUndoLevelPtr == 0)
		return false;
	if ((*itsCurrentUndoLevelPtr) < 0)
		return false;
	else
		return true;
}

//--------------------------------------------------------
// Redo				M.K. 
//--------------------------------------------------------
// Kertoo onko redo mahdollista suorittaa.
bool NFmiSmartInfo::Redo (void)
{
	if(itsCurrentUndoLevelPtr == 0)
		return false;
	if ((*itsCurrentRedoLevelPtr) == (*itsCurrentUndoLevelPtr) 
			|| (*itsCurrentRedoLevelPtr) == ((*itsCurrentUndoLevelPtr) + 1))
		return false;
	else
		return true;
}
//--------------------------------------------------------
// CommitData			M.K.
//--------------------------------------------------------

//   Commit:ia kutsutaan jos ei haluta enää käyttää 
//   jo kerääntynyttä undo-listaa. Eli jos on
//   tehnyt sarjan muutoksia dataan ja tallettaa 
//   datan tiedostoon voidaan kutsua committia.
//   
void NFmiSmartInfo::CommitData (void)
{
	if(itsCurrentUndoLevelPtr == 0)
		return ;
	*itsMaxRedoLevelPtr = 0;
	*itsCurrentUndoLevelPtr = -1;
	*itsCurrentRedoLevelPtr = -1;

	return;
}
//--------------------------------------------------------
// UndoData			M.K. 
//--------------------------------------------------------
//
//   Palaa undo-listassa yhden pykälän takaisin 
//   päin, mutta ei tuhoa viimeisiä muutoksia
//   että voidaan myös tehdä Redo(). Jos ei ole 
//   mitään Undo:ta tehtävissä, palautetaan false.
   
bool NFmiSmartInfo::UndoData (void)
{
	if(itsCurrentUndoLevelPtr == 0)
		return false;
	if ((*itsCurrentUndoLevelPtr) < 0)
		return false;
	if ((*itsCurrentUndoLevelPtr) == (*itsCurrentRedoLevelPtr))
	{
		NFmiString action("");
		SnapShotData(action);		// "Ottaa kuvan" undo-toimintoa edeltäneestä tilanteesta,
		(*itsCurrentUndoLevelPtr)--;		// jos siihen halutaankin myöhemmin palata redo:lla.
	}

	memcpy(itsRefDataPool->Data(), itsUndoTable[*itsCurrentUndoLevelPtr], itsRefDataPool->Size());
	(*itsCurrentUndoLevelPtr)--;
	*itsCurrentRedoLevelPtr = (*itsCurrentUndoLevelPtr) + 2;
	*fDirty = true; // 18.1.2002/Marko lisäsin datan likauksen.
	return true;
}

//--------------------------------------------------------
// RedoData			M.K. 
//--------------------------------------------------------
//
//   Redo palauttaa Undo:n jälkeen datan tilan 
//   takaisin Undo:ta edeltäneeseen tilaan. Jos    
//   ei ole tehty Undo:ta edellä, ei Redo:ta voida 
//   tehdä ja palauttaa false.

bool NFmiSmartInfo::RedoData (void)
{
	if(itsCurrentUndoLevelPtr == 0)
		return false;
	if ((*itsCurrentUndoLevelPtr) == (*itsCurrentRedoLevelPtr) 
			|| ((*itsCurrentUndoLevelPtr) + 1) == (*itsCurrentRedoLevelPtr))
		return false;
	else
	{
		memcpy(itsRefDataPool->Data(), itsUndoTable[*itsCurrentRedoLevelPtr], itsRefDataPool->Size());
		(*itsCurrentUndoLevelPtr)++;
		if((*itsCurrentRedoLevelPtr) + 1 <= (*itsMaxRedoLevelPtr))
			(*itsCurrentRedoLevelPtr)++;
	}
	*fDirty = true; // 18.1.2002/Marko lisäsin datan likauksen.
	return true;
}

//--------------------------------------------------------
// UndoLevel			M.K. 
//--------------------------------------------------------
//
//   Asettaa Undo:n syvyyden. Eli kuinka monta 
//   Undo:ta voidaan tehdä (eli tallettaa, vie 
//   muistia ja kuluttaa CPU aikaa). Jos theDepth 
//   0 tai pienempi, ei Undo:ta tehdä ollenkaan.
//	 Huom! Tämän voi toistaiseksi tehdä vain, jos 
//   undotaulukkoja ei ole entuudestaan olemassa.

void NFmiSmartInfo::UndoLevel (const long& theDepth)	// theDepth kuvaa kuinka monta
{														// Undota voidaan tehdä.
	if (!itsUndoTable && !itsUndoTextTable)	// Muuten taulukon ulkopuolelle viittaaminen voisi
	{										// olla mahdollista!
		itsMaxUndoLevelPtr = new long;
		itsMaxRedoLevelPtr = new long;
		itsCurrentUndoLevelPtr = new int;
		itsCurrentRedoLevelPtr = new int;

		if (theDepth <= 0)
			*itsMaxUndoLevelPtr = 0;
		else
		{
			*itsMaxRedoLevelPtr = 0;
			*itsCurrentUndoLevelPtr = -1;
			*itsCurrentRedoLevelPtr = -1;
			*itsMaxUndoLevelPtr = theDepth + 1;		// Redon tekemisen mahdollistamiseksi yksi
			itsUndoTable = new char* [*itsMaxUndoLevelPtr];				// taso lisää.
			itsUndoTextTable = new NFmiString [*itsMaxUndoLevelPtr];

			for (int level = 0; level < (*itsMaxUndoLevelPtr); level++)
			{
				itsUndoTable[level] = new char[itsRefDataPool->Size()];
			}
		}
	}

	return;
}

//--------------------------------------------------------
// MaskLocation 
//--------------------------------------------------------
//   Asettaa currentin paikan maskin riippuen 
//   itsMaskStatesta.

void NFmiSmartInfo::MaskLocation (const bool& newState)
{
	(*itsAreaMask)->Mask(LocationIndex(),newState);
}

void NFmiSmartInfo::MaskLocation (const bool& newState, 
								  unsigned long theMaskType)
{
	(*itsAreaMask)->Mask(LocationIndex(),newState, theMaskType);
}

void NFmiSmartInfo::MaskType(const unsigned long& theMaskType)
{
	(*itsAreaMask)->MaskType(theMaskType);
}

unsigned long NFmiSmartInfo::MaskType(void)
{
	return (*itsAreaMask)->MaskType();
}

//--------------------------------------------------------
// LocationMaskStep 
//--------------------------------------------------------

//   Tämän tarkoituksena olisi asettaa aktiivisiksi maskeiksi esim. joka
//   neljännen aseman tai joka viides hila ja joka neljäs rivi (kun piirretään
//   esim. pieneen tilaan tiheää hilaa ja kaikkia paikkoja ei haluta piirtää).

void NFmiSmartInfo::LocationMaskStep (bool newStatus, bool fResetAllFirst, const long& theXStep, const long& theYStep)
{
	if(IsGrid())
		LocationMaskStepGrid(newStatus, fResetAllFirst, theXStep,theYStep);
	else if(IsLocation())
		LocationMaskStepLocation(newStatus, fResetAllFirst, theXStep);
}

void NFmiSmartInfo::LocationMaskStep (bool newStatus, unsigned long theMaskType, bool fResetAllFirst, const long& theXStep, const long& theYStep)
{
	if(IsGrid())
		LocationMaskStepGrid(newStatus, theMaskType, fResetAllFirst, theXStep, theYStep);
	else if(IsLocation())
		LocationMaskStepLocation(newStatus, theMaskType, fResetAllFirst, theXStep);
}

void NFmiSmartInfo::LocationMaskStepGrid(bool newStatus, bool fResetAllFirst, const long& theXStep, const long& theYStep)
{
	if(!(theYStep && theXStep)) // jos ystep tai xstep 0, ei tehdä mitään
		return;

	unsigned long x = HPlaceDescriptor().Grid()->XNumber();
	unsigned long y = HPlaceDescriptor().Grid()->YNumber();	
	
	if(fResetAllFirst)
		MaskAllLocations(!newStatus);

	unsigned long midX = (unsigned long)(theXStep/2); // sijoittaa näkyvät hilat 'keskemmälle'
	unsigned long midY = (unsigned long)(theYStep/2);

	ResetLocation();
	for(unsigned int j=0; j<y; j++)
	{	
		for(unsigned int i=0; i<x && NFmiFastQueryInfo::NextLocation(); i++)
		{
			if(!(j%theYStep == midY && i%theXStep == midX)) // 1999.09.17/Marko Laitoin aktiivisten hilojen aloituksen keskemmälle (== mid)
				MaskLocation(newStatus);
		}
	}
}

void NFmiSmartInfo::LocationMaskStepGrid(bool newStatus, unsigned long theMaskType, bool fResetAllFirst, const long& theXStep, const long& theYStep)
{
	if(!(theYStep && theXStep)) // jos ystep tai xstep 0, ei tehdä mitään
		return;

	unsigned long x = HPlaceDescriptor().Grid()->XNumber();
	unsigned long y = HPlaceDescriptor().Grid()->YNumber();	
	
	if(fResetAllFirst)
		MaskAllLocations(!newStatus, theMaskType);

	unsigned long midX = (unsigned long)(theXStep/2); // sijoittaa näkyvät hilat 'keskemmälle'
	unsigned long midY = (unsigned long)(theYStep/2);

	ResetLocation();
	for(unsigned int j=0; j<y; j++)
	{	
		for(unsigned int i=0; i<x && NFmiFastQueryInfo::NextLocation(); i++)
		{
			if(!(j%theYStep == midY && i%theXStep == midX)) // 1999.09.17/Marko Laitoin aktiivisten hilojen aloituksen keskemmälle (== mid)
				MaskLocation(newStatus,theMaskType);
		}
	}
}

void NFmiSmartInfo::LocationMaskStepLocation(bool newStatus, bool fResetAllFirst, const long& theXStep)
{
	if(!theXStep) // jos xstep 0, ei tehdä mitään
		return;

	if(fResetAllFirst)
		MaskAllLocations(!newStatus);
	ResetLocation();
	while(NFmiFastQueryInfo::NextLocation())
	{
		if(!(LocationIndex()%theXStep==0)) // 1999.09.07/Marko
			MaskLocation(newStatus);
	}
}

void NFmiSmartInfo::LocationMaskStepLocation(bool newStatus, unsigned long theMaskType, bool fResetAllFirst, const long& theXStep)
{
	if(!theXStep) // jos xstep 0, ei tehdä mitään
		return;

	if(fResetAllFirst)
		MaskAllLocations(!newStatus, theMaskType);
	ResetLocation();
	while(NFmiFastQueryInfo::NextLocation())
	{
		if(!(LocationIndex()%theXStep==0)) // 1999.09.07/Marko
			MaskLocation(newStatus, theMaskType);
	}
}


NFmiQueryData* NFmiSmartInfo::DataReference()
{
	return itsDataReference;
}

bool NFmiSmartInfo::Extrapolate(void)
{
	for(this->ResetLevel(); this->NextLevel();)
	{
		//tähän tarkistetaan jokaiselta parametrilta erikseen ekstrapoloidaanko
		for(this->ResetParam(); this->NextParam();)
		{
			for(this->ResetLocation(); this->NFmiFastQueryInfo::NextLocation();)
			{	
				float edellinen = kFloatMissing;
				for(this->ResetTime(); this->NextTime();)
				{
					float f1 = this->FloatValue();
					if(f1 == kFloatMissing || f1 == kTCombinedWeatherFloatMissing)
					{
						if(!(edellinen == kFloatMissing || edellinen == kTCombinedWeatherFloatMissing))
							this->FloatValue(edellinen);
					}
					else
						edellinen = f1;
				}	
			}
		}
	}
	return true;
}

unsigned long NFmiSmartInfo::LocationMaskedCount(unsigned long theMaskType)
{
	return (*itsAreaMask)->MaskedCount(theMaskType);
}

bool NFmiSmartInfo::LocationSelectionSnapShot(void)
{
	return itsAreaMask->SnapShotData();
}

bool NFmiSmartInfo::LocationSelectionUndo(void)
{
	return itsAreaMask->Undo();
}

bool NFmiSmartInfo::LocationSelectionRedo(void)
{
	return itsAreaMask->Redo();
}

void NFmiSmartInfo::LocationSelectionCommitData(void)
{
	itsAreaMask->CommitData();
}

bool NFmiSmartInfo::LocationSelectionUndoData(void)
{
	return itsAreaMask->UndoData();
}

bool NFmiSmartInfo::LocationSelectionRedoData(void)
{
	return itsAreaMask->RedoData();
}

void NFmiSmartInfo::LocationSelectionUndoLevel(int theNewUndoLevel)
{
	itsAreaMask->UndoLevel(theNewUndoLevel);
}

void NFmiSmartInfo::AreaFactors(NFmiGrid* theAreaFactor)
{
	delete itsAreaFactors; 
	itsAreaFactors = theAreaFactor;
}

int NFmiSmartInfo::MaskedCount(unsigned long theMaskType, unsigned long theIndex, const NFmiRect& theSearchArea)
{
	return (*itsAreaMask)->MaskedCount(theMaskType, theIndex, theSearchArea, itsGridXNumber, itsGridYNumber);
}
