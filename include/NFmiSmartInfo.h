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
// Changed 1999.08.25/Marko	Lis‰sin itsDataType dataosan, jonka avulla smartinfoja voidaan
//							j‰rjest‰‰ NFmiInfoOrganizer:in avulla.
// Changed 1999.09.06/Marko Muutin LocationMaskStep()-metodien rajapintaa niin, ett‰ voidaan 
//							m‰‰r‰t‰ resetoidaanko ensin arvot vai ei.
// 
//**********************************************************
#ifndef  NFMISMARTINFO_H
#define  NFMISMARTINFO_H

#include "NFmiFastQueryInfo.h"
//#include "FmiNMeteditLibraryDefinitions.h"

class NFmiUndoableMultiLevelMask;
class NFmiBitMask;
class NFmiRect;

class NFmiSmartInfo : public NFmiFastQueryInfo
{
 public:
	 
	NFmiSmartInfo(void);
	NFmiSmartInfo(const NFmiQueryInfo & theInfo
				 ,NFmiQueryData* theData=0,NFmiString theDataFileName=""
				 ,NFmiInfoData::Type theType = NFmiInfoData::kEditable);
	NFmiSmartInfo (const NFmiSmartInfo & theInfo); 
	~NFmiSmartInfo();
	
	NFmiSmartInfo* Clone(void) const;
	void AreaMask(NFmiUndoableMultiLevelMask* theAreaMask);
	int MaskedCount(unsigned long theMaskType, unsigned long theIndex, const NFmiRect& theSearchArea);
	bool MaskByArea(const NFmiArea &theArea, unsigned long theMaskType);

	void MaskAllLocations (const bool& newState, unsigned long theMaskType);
	void MaskAllLocations (const bool& newState);
	
	bool NextLocation (void);
   
	unsigned long LocationMaskedCount(unsigned long theMaskType);

	//toimii vaan, jos on yksi info
	bool IsDirty(void) const {return *fDirty;};
	void Dirty(bool newState){*fDirty = newState;};

	bool IsMaskedLocation (unsigned long theMaskType) const;

	void MaskType(unsigned long theMaskType);
	unsigned long MaskType(void);

	const NFmiBitMask& LocationMask (unsigned long theMaskType) const; 
	void LocationMask (const NFmiBitMask& theMask, unsigned long theMaskType);
   
	bool SnapShotData (const NFmiString& theAction);
	void RearrangeUndoTable(void);
	bool SnapShotData (const NFmiString& theAction,FmiParameterName theParameter);
	NFmiString UndoText (void);
	NFmiString RedoText (void);
	bool Undo (void);
	bool Redo (void);
	void CommitData (void);
	bool UndoData (void);
	bool RedoData (void);
	void UndoLevel (const long& theDepth);

	void MaskLocation (const bool& newState);
	void MaskLocation (const bool& newState, unsigned long theMaskType);

	void LocationMaskStep (bool newStatus, bool fResetAllFirst, const long& theXStep, const long& theYStep=1);
	void LocationMaskStep (bool newStatus, unsigned long theMaskType, bool fResetAllFirst, const long& theXStep, const long& theYStep=1);

	NFmiGrid* AreaFactors(void){return itsAreaFactors;};
	void AreaFactors(NFmiGrid* theAreaFactor);

	const NFmiString& DataFileName(void){return itsDataFileName;}
	void DataFileName(const NFmiString& theDataFileName){itsDataFileName = theDataFileName;}
	NFmiSmartInfo& operator=(const NFmiSmartInfo& theSmartInfo);
	
	bool operator==(const NFmiSmartInfo& theSmartInfo) const;
	bool operator< (const NFmiSmartInfo& theSmartInfo) const;
	
	void DestroyData(void);
	void DestroySharedData(void){DestroyData();};
	NFmiQueryData* DataReference(void);

	bool IsEditable(void){return fEditable;}
	void Editable(bool newState){fEditable = newState;}
	
	int Priority(void){return itsPriority;}
	void Priority(int thePriority){itsPriority = thePriority;}

	bool LoadedFromFile(void){return *fLoadedFromFile;}
	void LoadedFromFile(bool loadedFromFile){*fLoadedFromFile = loadedFromFile;}

	bool Extrapolate(void);

	NFmiParamBag* EditedParamBag(void){return itsEditedParamBag;}
	void EditedParamBag(const NFmiParamBag& theParamBag){*itsEditedParamBag = theParamBag;}
	bool InitEditedParamBag(void);

	bool LocationSelectionSnapShot(void);	// ota maskit talteen
	bool LocationSelectionUndo(void);		// kysyy onko undo mahdollinen
	bool LocationSelectionRedo(void);		// kysyy onko redo mahdollinen
	void LocationSelectionCommitData(void);		// hyv‰ksy tehdyt muutokset (eli tuhoaa undo mahdollisuudet)
	bool LocationSelectionUndoData(void);	// suorittaa todellisen undon
	bool LocationSelectionRedoData(void);	// suorittaa todellisen redon
	void LocationSelectionUndoLevel(int theNewUndoLevel); // undolevel asetetaan t‰ll‰
	NFmiInfoData::Type DataType(void) const {return itsDataType;}; // 1999.08.24/Marko
	void DataType(NFmiInfoData::Type newType){itsDataType = newType;}; // 1999.08.24/Marko

 private:

	void LocationMaskStepGrid(bool newStatus, bool fResetAllFirst, const long& theXStep, const long& theYStep);
	void LocationMaskStepGrid(bool newStatus, unsigned long theMaskType, bool fResetAllFirst, const long& theXStep, const long& theYStep);
	
	void LocationMaskStepLocation(bool newStatus, bool fResetAllFirst, const long& theXStep);
	void LocationMaskStepLocation(bool newStatus, unsigned long theMaskType, bool fResetAllFirst, const long& theXStep);


//   Vain pointteri, ei tuhota destruktorissa. 
//   smartinfo k‰ytt‰‰ oliota maskaamaan aktivaation, 
//	 valinnan ja parametrimaskin.
   
	NFmiUndoableMultiLevelMask* itsAreaMask;
	NFmiGrid* itsAreaFactors;//t‰m‰n gridin avulla yhdistet‰‰n eri aluetoimistojen datat
	NFmiQueryData* itsDataReference;
	
	NFmiString itsDataFileName;
	int itsPriority;
	bool fEditable;
	
	long* itsMaxUndoLevelPtr;
	long* itsMaxRedoLevelPtr;
	int* itsCurrentUndoLevelPtr;
	int* itsCurrentRedoLevelPtr;

	char** itsUndoTable;
	NFmiString* itsUndoTextTable;

	//(kertoo onko data ladattu tiedostosta vai tyˆlistan mukaisesti)
	bool* fLoadedFromFile;
   	bool*	fDirty; //laura muutti pointteriksi, ett‰ tieto s‰ilyisi kaikissa listoissa

	//parambag, siksi ett‰ tiedett‰isiin mit‰ parametri‰ on muutettu
	NFmiParamBag* itsEditedParamBag;
	NFmiInfoData::Type itsDataType; // 1999.08.24/Marko
};

#endif
