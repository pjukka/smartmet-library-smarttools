
#include "NFmiSmartInfo2.h"
#include "NFmiQueryData.h"
#include "NFmiModifiableQDatasBookKeeping.h"


NFmiSmartInfo2::NFmiSmartInfo2(NFmiQueryData *theOwnedData, NFmiInfoData::Type theDataType, const std::string &theDataFileName, const std::string &theDataFilePattern)
:NFmiOwnerInfo(theOwnedData, theDataType, theDataFileName, theDataFilePattern)
,itsQDataBookKeepingPtr()
{
	itsQDataBookKeepingPtr = boost::shared_ptr<NFmiModifiableQDatasBookKeeping>(new NFmiModifiableQDatasBookKeeping(SizeLocations()));
}

NFmiSmartInfo2::NFmiSmartInfo2(const NFmiOwnerInfo &theInfo)
:NFmiOwnerInfo(theInfo)
,itsQDataBookKeepingPtr()
{
}

NFmiSmartInfo2::NFmiSmartInfo2(const NFmiSmartInfo2 &theInfo)
:NFmiOwnerInfo(theInfo)
,itsQDataBookKeepingPtr(theInfo.itsQDataBookKeepingPtr)
{
}

NFmiSmartInfo2::~NFmiSmartInfo2(void)
{
}

NFmiSmartInfo2& NFmiSmartInfo2::operator=(const NFmiSmartInfo2 &theInfo)
{
	if(this != &theInfo)
	{
		NFmiOwnerInfo::operator=(theInfo);
		itsQDataBookKeepingPtr = theInfo.itsQDataBookKeepingPtr;
	}
	return *this;
}

NFmiSmartInfo2* NFmiSmartInfo2::Clone(void) const
{
	NFmiQueryData *cloneData = itsDataPtr.get()->Clone(); // datasta tehtävä tässä kopio!
	NFmiSmartInfo2 *copy = new NFmiSmartInfo2(cloneData, itsDataType, itsDataFileName, itsDataFilePattern); // tämä ei osaa tehdä kaikesta tarvittavasta datasta kopiota
	copy->CopyClonedDatas(*this); // tässä laitetaan kaikki loput tarvittavat NFmiSmartInfo2-data osat kopioitavaksi clooniin.
	return copy;
}

void NFmiSmartInfo2::CopyClonedDatas(const NFmiSmartInfo2 &theOther)
{
	if(this != &theOther)
	{
		itsQDataBookKeepingPtr->CopyClonedDatas(*(theOther.itsQDataBookKeepingPtr.get()));
	}
}

void NFmiSmartInfo2::UndoLevel(long theDepth)	// theDepth kuvaa kuinka monta Undota voidaan tehdä.
{
	itsQDataBookKeepingPtr->UndoLevel(theDepth, *itsRefRawData);
}

bool NFmiSmartInfo2::LocationSelectionSnapShot(void)
{
	return itsQDataBookKeepingPtr->LocationSelectionSnapShot();
}

bool NFmiSmartInfo2::LocationSelectionUndo(void)
{
	return itsQDataBookKeepingPtr->LocationSelectionUndo();
}

bool NFmiSmartInfo2::LocationSelectionRedo(void)
{
	return itsQDataBookKeepingPtr->LocationSelectionRedo();
}

bool NFmiSmartInfo2::LocationSelectionUndoData(void)
{
	return itsQDataBookKeepingPtr->LocationSelectionUndoData();
}

bool NFmiSmartInfo2::LocationSelectionRedoData(void)
{
	return itsQDataBookKeepingPtr->LocationSelectionRedoData();
}

void NFmiSmartInfo2::LocationSelectionUndoLevel(int theNewUndoLevel)
{
	itsQDataBookKeepingPtr->LocationSelectionUndoLevel(theNewUndoLevel);
}

bool NFmiSmartInfo2::LoadedFromFile(void)
{
	return itsQDataBookKeepingPtr->LoadedFromFile();
}

void NFmiSmartInfo2::LoadedFromFile(bool loadedFromFile)
{
	itsQDataBookKeepingPtr->LoadedFromFile(loadedFromFile);
}

//   Siirtää 'iteraattorin' osoittamaan seuraavaa
//   maskattua paikkaa.
//   Kutsutaan emon Next()-metodia ja katsotaan,
//   mikä itsAreaMask-olio
//   palauttaa. Jos Maski on true, palautetaan
//   true, muuten kutsutaan
//   taas emon Next:iä ja jatketaan kunnes emon
//   Next palauttaa falsen.
bool NFmiSmartInfo2::NextLocation()
{
	bool returnVal = false;

	while((returnVal==false) && NFmiFastQueryInfo::NextLocation())
	{
		returnVal = itsQDataBookKeepingPtr->IsMasked(LocationIndex());
	}
	return returnVal;
}

bool NFmiSmartInfo2::SnapShotData(const std::string& theAction, const NFmiHarmonizerBookKeepingData &theHarmonizerBookKeepingData)
{
	return itsQDataBookKeepingPtr->SnapShotData(theAction, theHarmonizerBookKeepingData, *itsRefRawData);
}

bool NFmiSmartInfo2::Undo(void)
{
	return itsQDataBookKeepingPtr->Undo();
}

bool NFmiSmartInfo2::Redo(void)
{
	return itsQDataBookKeepingPtr->Redo();
}

bool NFmiSmartInfo2::UndoData(const NFmiHarmonizerBookKeepingData &theHarmonizerBookKeepingData)
{
	return itsQDataBookKeepingPtr->UndoData(theHarmonizerBookKeepingData, *itsRefRawData);
}

bool NFmiSmartInfo2::RedoData(void)
{
	return itsQDataBookKeepingPtr->RedoData(*itsRefRawData);
}

const NFmiHarmonizerBookKeepingData* NFmiSmartInfo2::CurrentHarmonizerBookKeepingData(void) const
{
	return itsQDataBookKeepingPtr->CurrentHarmonizerBookKeepingData();
}

bool NFmiSmartInfo2::IsDirty(void) const 
{
	return itsQDataBookKeepingPtr->IsDirty();
}

void NFmiSmartInfo2::Dirty(bool newState)
{
	itsQDataBookKeepingPtr->Dirty(newState);
}

int NFmiSmartInfo2::MaskedCount(unsigned long theMaskType, unsigned long theIndex, const NFmiRect& theSearchArea)
{
	return itsQDataBookKeepingPtr->MaskedCount(theMaskType, theIndex, theSearchArea, itsGridXNumber, itsGridYNumber);
}

void NFmiSmartInfo2::InverseMask(unsigned long theMaskType)
{
	itsQDataBookKeepingPtr->InverseMask(theMaskType);
}

void NFmiSmartInfo2::MaskAllLocations(const bool& newState, unsigned long theMaskType)
{
	itsQDataBookKeepingPtr->MaskAllLocations(newState, theMaskType);
}

unsigned long NFmiSmartInfo2::LocationMaskedCount(unsigned long theMaskType)
{
	return itsQDataBookKeepingPtr->LocationMaskedCount(theMaskType);
}

bool NFmiSmartInfo2::Mask(const NFmiBitMask& theMask, unsigned long theMaskType)
{
	return itsQDataBookKeepingPtr->Mask(theMask, theMaskType);
}

const NFmiBitMask& NFmiSmartInfo2::Mask(unsigned long theMaskType) const
{
	return itsQDataBookKeepingPtr->Mask(theMaskType);
}

void NFmiSmartInfo2::MaskLocation(const bool& newState, unsigned long theMaskType)
{
	itsQDataBookKeepingPtr->MaskLocation(newState, theMaskType, LocationIndex());
}

void NFmiSmartInfo2::MaskType(unsigned long theMaskType)
{
	itsQDataBookKeepingPtr->MaskType(theMaskType);
}

unsigned long NFmiSmartInfo2::MaskType(void)
{
	return itsQDataBookKeepingPtr->MaskType();
}

