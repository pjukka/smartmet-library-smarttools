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
#ifdef _MSC_VER
#pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta (liian pitkä nimi >255 merkkiä joka johtuu 'puretuista' STL-template nimistä)
#endif

#include "NFmiSmartInfo.h"
#include "NFmiUndoableMultiLevelMask.h"
#include "NFmiMultiLevelMask.h"
#include "NFmiQueryData.h"
#include "NFmiGrid.h"
#include "NFmiCombinedParam.h"
#include "NFmiMetEditorTypes.h"
#include "NFmiModifiableQDatasBookKeeping.h"
#include <stdexcept>

// **********************************************************************

NFmiSmartInfo::NFmiSmartInfo(const NFmiQueryInfo & theInfo, NFmiQueryData* theData
							 ,std::string theDataFileName, std::string theDataFilePattern
							 ,NFmiInfoData::Type theType)
:NFmiFastQueryInfo(theInfo)
,itsDataType(theType)
,itsModifiableQDatasBookKeeping(0)
{
	itsDataReference = theData;
	itsDataFileName = theDataFileName;
	itsDataFilePattern = theDataFilePattern;
	itsModifiableQDatasBookKeeping = new NFmiModifiableQDatasBookKeeping(HPlaceDescriptor().Size());
}

NFmiSmartInfo::NFmiSmartInfo (const NFmiSmartInfo & theInfo)
:NFmiFastQueryInfo(theInfo)
,itsDataFileName(theInfo.itsDataFileName)
,itsDataFilePattern(theInfo.itsDataFilePattern)
,itsDataType(theInfo.itsDataType)
,itsModifiableQDatasBookKeeping(theInfo.itsModifiableQDatasBookKeeping)
,itsDataReference(theInfo.itsDataReference)
{
}

NFmiSmartInfo::~NFmiSmartInfo()
{
}

void NFmiSmartInfo::DestroyData(bool deleteQData)
{
	//kopio-konstruktori kopioi näistä vain osoitteet,
	//joten nämä saa tuhota vaan yhdestä oliosta, käyttäjän pitää tietää mistä ja milloin
	if(itsModifiableQDatasBookKeeping)
	{
		delete itsModifiableQDatasBookKeeping;
		itsModifiableQDatasBookKeeping = 0;
	}

	if(deleteQData)
	{
		delete itsDataReference;
		itsDataReference = 0;
	}
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
NFmiSmartInfo* NFmiSmartInfo::Clone(void) const
{
	if(this != 0)
	{
		NFmiQueryData* queryDataCopy = new NFmiQueryData(*(this->itsDataReference));
		NFmiQueryInfo queryInfo(queryDataCopy);
		NFmiSmartInfo* copy = new NFmiSmartInfo(queryInfo, queryDataCopy, this->itsDataFileName, this->itsDataFilePattern
											   ,this->itsDataType); // 1999.08.24/Marko
		copy->itsDataFileName = this->itsDataFileName;
		copy->itsModifiableQDatasBookKeeping->CopyClonedDatas(*(this->itsModifiableQDatasBookKeeping));

		copy->SetDescriptors(const_cast<NFmiSmartInfo *>(this), false);
		return copy;
	}
	else
		return 0;
}

NFmiSmartInfo& NFmiSmartInfo::operator=(const NFmiSmartInfo &theSmartInfo)
{
	if(this != &theSmartInfo)
	{
		NFmiFastQueryInfo::operator=(theSmartInfo);

		this->itsDataFileName = theSmartInfo.itsDataFileName;
		this->itsModifiableQDatasBookKeeping = theSmartInfo.itsModifiableQDatasBookKeeping;
		this->itsDataReference = theSmartInfo.itsDataReference;
		this->itsDataType = theSmartInfo.itsDataType;
	}
	return *this;
}

void NFmiSmartInfo::InverseMask(unsigned long theMaskType)
{
	itsModifiableQDatasBookKeeping->InverseMask(theMaskType);
}

//--------------------------------------------------------
// MaskAllLocations
//--------------------------------------------------------
//   Aktivoi tai deaktivoi kaikki locationit.
//
void NFmiSmartInfo::MaskAllLocations(const bool& newState, unsigned long theMaskType)
{
	itsModifiableQDatasBookKeeping->MaskAllLocations(newState, theMaskType);
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
		returnVal = itsModifiableQDatasBookKeeping->IsMasked(LocationIndex());
	}
	return returnVal;
}

bool NFmiSmartInfo::Mask(const NFmiBitMask& theMask, unsigned long theMaskType)
{
	return itsModifiableQDatasBookKeeping->Mask(theMask, theMaskType);
}

const NFmiBitMask& NFmiSmartInfo::Mask(unsigned long theMaskType) const
{
	return itsModifiableQDatasBookKeeping->Mask(theMaskType);
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

bool NFmiSmartInfo::SnapShotData(const std::string& theAction, const NFmiHarmonizerBookKeepingData &theHarmonizerBookKeepingData)
{
	return itsModifiableQDatasBookKeeping->SnapShotData(theAction, theHarmonizerBookKeepingData, *itsRefRawData);
}

//--------------------------------------------------------
// Undo				M.K.
//--------------------------------------------------------
// Kertoo onko undo mahdollista suorittaa.
bool NFmiSmartInfo::Undo(void)
{
	return itsModifiableQDatasBookKeeping->Undo();
}

//--------------------------------------------------------
// Redo				M.K.
//--------------------------------------------------------
// Kertoo onko redo mahdollista suorittaa.
bool NFmiSmartInfo::Redo(void)
{
	return itsModifiableQDatasBookKeeping->Redo();
}
//--------------------------------------------------------
// UndoData			M.K.
//--------------------------------------------------------
//
//   Palaa undo-listassa yhden pykälän takaisin
//   päin, mutta ei tuhoa viimeisiä muutoksia
//   että voidaan myös tehdä Redo(). Jos ei ole
//   mitään Undo:ta tehtävissä, palautetaan false.

bool NFmiSmartInfo::UndoData(const NFmiHarmonizerBookKeepingData &theHarmonizerBookKeepingData)
{
	return itsModifiableQDatasBookKeeping->UndoData(theHarmonizerBookKeepingData, *itsRefRawData);
}

//--------------------------------------------------------
// RedoData			M.K.
//--------------------------------------------------------
//
//   Redo palauttaa Undo:n jälkeen datan tilan
//   takaisin Undo:ta edeltäneeseen tilaan. Jos
//   ei ole tehty Undo:ta edellä, ei Redo:ta voida
//   tehdä ja palauttaa false.

bool NFmiSmartInfo::RedoData(void)
{
	return itsModifiableQDatasBookKeeping->RedoData(*itsRefRawData);
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

void NFmiSmartInfo::UndoLevel(long theDepth)	// theDepth kuvaa kuinka monta Undota voidaan tehdä.
{
	itsModifiableQDatasBookKeeping->UndoLevel(theDepth, *itsRefRawData);
}

void NFmiSmartInfo::MaskLocation(const bool& newState, unsigned long theMaskType)
{
	itsModifiableQDatasBookKeeping->MaskLocation(newState, theMaskType, LocationIndex());
}

void NFmiSmartInfo::MaskType(unsigned long theMaskType)
{
	itsModifiableQDatasBookKeeping->MaskType(theMaskType);
}

unsigned long NFmiSmartInfo::MaskType(void)
{
	return itsModifiableQDatasBookKeeping->MaskType();
}

NFmiQueryData* NFmiSmartInfo::DataReference()
{
	return itsDataReference;
}

unsigned long NFmiSmartInfo::LocationMaskedCount(unsigned long theMaskType)
{
	return itsModifiableQDatasBookKeeping->LocationMaskedCount(theMaskType);
}

bool NFmiSmartInfo::LocationSelectionSnapShot(void)
{
	return itsModifiableQDatasBookKeeping->LocationSelectionSnapShot();
}

bool NFmiSmartInfo::LocationSelectionUndo(void)
{
	return itsModifiableQDatasBookKeeping->LocationSelectionUndo();
}

bool NFmiSmartInfo::LocationSelectionRedo(void)
{
	return itsModifiableQDatasBookKeeping->LocationSelectionRedo();
}

bool NFmiSmartInfo::LocationSelectionUndoData(void)
{
	return itsModifiableQDatasBookKeeping->LocationSelectionUndoData();
}

bool NFmiSmartInfo::LocationSelectionRedoData(void)
{
	return itsModifiableQDatasBookKeeping->LocationSelectionRedoData();
}

void NFmiSmartInfo::LocationSelectionUndoLevel(int theNewUndoLevel)
{
	itsModifiableQDatasBookKeeping->LocationSelectionUndoLevel(theNewUndoLevel);
}

int NFmiSmartInfo::MaskedCount(unsigned long theMaskType, unsigned long theIndex, const NFmiRect& theSearchArea)
{
	return itsModifiableQDatasBookKeeping->MaskedCount(theMaskType, theIndex, theSearchArea, itsGridXNumber, itsGridYNumber);
}

const NFmiHarmonizerBookKeepingData* NFmiSmartInfo::CurrentHarmonizerBookKeepingData(void) const
{
	return itsModifiableQDatasBookKeeping->CurrentHarmonizerBookKeepingData();
}

bool NFmiSmartInfo::IsDirty(void) const 
{
	return itsModifiableQDatasBookKeeping->IsDirty();
}

void NFmiSmartInfo::Dirty(bool newState)
{
	itsModifiableQDatasBookKeeping->Dirty(newState);
}

bool NFmiSmartInfo::LoadedFromFile(void)
{
	return itsModifiableQDatasBookKeeping->LoadedFromFile();
}

void NFmiSmartInfo::LoadedFromFile(bool loadedFromFile)
{
	itsModifiableQDatasBookKeeping->LoadedFromFile(loadedFromFile);
}
