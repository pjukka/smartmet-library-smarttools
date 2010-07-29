//**********************************************************
// C++ Class Name : NFmiSmartInfo
// ---------------------------------------------------------
// Filetype: (HEADER)
// Filepath: D:/projekti/GDPro/GDTemp/NFmiSmartInfo.h
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
#ifndef  NFMISMARTINFO_H
#define  NFMISMARTINFO_H

#include "NFmiFastQueryInfo.h"
#include "NFmiHarmonizerBookKeepingData.h"

#include <deque>
#include <set>

class NFmiBitMask;
class NFmiRect;
class NFmiModifiableQDatasBookKeeping;

class NFmiSmartInfo : public NFmiFastQueryInfo
{
 public:

	NFmiSmartInfo(const NFmiQueryInfo & theInfo
				 ,NFmiQueryData* theData, std::string theDataFileName, std::string theDataFilePattern
				 ,NFmiInfoData::Type theType = NFmiInfoData::kEditable);
	NFmiSmartInfo (const NFmiSmartInfo & theInfo);
	~NFmiSmartInfo();

	NFmiSmartInfo* Clone(void) const;
	NFmiSmartInfo& operator=(const NFmiSmartInfo& theSmartInfo);
	void DestroyData(bool deleteQData = true);
	NFmiQueryData* DataReference(void);

	int MaskedCount(unsigned long theMaskType, unsigned long theIndex, const NFmiRect& theSearchArea);
	void InverseMask(unsigned long theMaskType);
	void MaskAllLocations (const bool& newState, unsigned long theMaskType);
	unsigned long LocationMaskedCount(unsigned long theMaskType);
	bool Mask(const NFmiBitMask& theMask, unsigned long theMaskType);
	const NFmiBitMask& Mask(unsigned long theMaskType) const;
	void MaskLocation (const bool& newState, unsigned long theMaskType);
	void MaskType(unsigned long theMaskType);
	unsigned long MaskType(void);

	bool NextLocation (void);

	bool SnapShotData(const std::string& theAction, const NFmiHarmonizerBookKeepingData &theCurrentHarmonizerBookKeepingData);
	bool Undo(void);
	bool Redo(void);
	bool UndoData(const NFmiHarmonizerBookKeepingData &theHarmonizerBookKeepingData);
	bool RedoData(void);
	void UndoLevel(long theDepth);

	const std::string& DataFileName(void){return itsDataFileName;}
	void DataFileName(const std::string& theDataFileName){itsDataFileName = theDataFileName;}
	const std::string& DataFilePattern(void) const {return itsDataFilePattern;}
	void DataFilePattern(const std::string &theDataFilePattern) {itsDataFilePattern = theDataFilePattern;}

	bool LoadedFromFile(void);
	void LoadedFromFile(bool loadedFromFile);
	bool IsDirty(void) const;
	void Dirty(bool newState);

	bool LocationSelectionSnapShot(void);	// ota maskit talteen
	bool LocationSelectionUndo(void);		// kysyy onko undo mahdollinen
	bool LocationSelectionRedo(void);		// kysyy onko redo mahdollinen
	bool LocationSelectionUndoData(void);	// suorittaa todellisen undon
	bool LocationSelectionRedoData(void);	// suorittaa todellisen redon
	void LocationSelectionUndoLevel(int theNewUndoLevel); // undolevel asetetaan tällä

	NFmiInfoData::Type DataType(void) const {return itsDataType;}; // 1999.08.24/Marko
	void DataType(NFmiInfoData::Type newType){itsDataType = newType;}; // 1999.08.24/Marko

	const NFmiHarmonizerBookKeepingData* CurrentHarmonizerBookKeepingData(void) const; // palauttaa nyt käytössä olevan harmonisaattori parambagin

 private:
//   Vain pointteri, ei tuhota destruktorissa.
	NFmiModifiableQDatasBookKeeping *itsModifiableQDatasBookKeeping;

	NFmiQueryData* itsDataReference;
	NFmiInfoData::Type itsDataType;
	std::string itsDataFileName;
	std::string itsDataFilePattern; // tätä käytetään tunnistamaan mm. info-organizerissa, että onko data samanlaista, eli pyyhitäänkö vanha tälläinen data pois alta
};

#endif
