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
#include "NFmiString.h"
#include "FmiNMeteditLibraryDefinitions.h"

class NFmiUndoableMultiLevelMask;
class NFmiBitMask;
class NFmiRect;

class NFmiSmartInfo : public NFmiFastQueryInfo
{
 public:
	 
	NFmiSmartInfo(void);
	NFmiSmartInfo(const NFmiQueryInfo & theInfo
				 ,NFmiQueryData* theData=0,NFmiString theDataFileName=""
				 ,FmiQueryInfoDataType theType = kFmiDataTypeEditable);
	NFmiSmartInfo (const NFmiSmartInfo & theInfo); 
	virtual ~NFmiSmartInfo();
	
	NFmiSmartInfo* Clone(void);
	void AreaMask(NFmiUndoableMultiLevelMask* theAreaMask);
	int MaskedCount(unsigned long theMaskType, unsigned long theIndex, const NFmiRect& theSearchArea);
	FmiBoolean MaskByArea(const NFmiArea &theArea, unsigned long theMaskType);

	void MaskAllLocations (const FmiBoolean& newState, unsigned long theMaskType);
	void MaskAllLocations (const FmiBoolean& newState);
	
	virtual FmiBoolean NextLocation (void);
   
	unsigned long LocationMaskedCount(unsigned long theMaskType);

	//toimii vaan, jos on yksi info
	FmiBoolean IsDirty(void) const {return *fDirty;};
	void Dirty(FmiBoolean newState){*fDirty = newState;};

	FmiBoolean IsMaskedLocation (unsigned long theMaskType) const;

	void MaskType(const unsigned long& theMaskType);
	unsigned long MaskType(void);

	const NFmiBitMask& LocationMask (unsigned long theMaskType) const; 
	void LocationMask (const NFmiBitMask& theMask, unsigned long theMaskType);
   
	FmiBoolean SnapShotData (const NFmiString& theAction);
	void RearrangeUndoTable(void);
	FmiBoolean SnapShotData (const NFmiString& theAction,FmiParameterName theParameter);
	NFmiString UndoText (void);
	NFmiString RedoText (void);
	FmiBoolean Undo (void);
	FmiBoolean Redo (void);
	void CommitData (void);
	FmiBoolean UndoData (void);
	FmiBoolean RedoData (void);
	void UndoLevel (const long& theDepth);

	void MaskLocation (const FmiBoolean& newState);
	void MaskLocation (const FmiBoolean& newState, unsigned long theMaskType);

	void LocationMaskStep (FmiBoolean newStatus, FmiBoolean fResetAllFirst, const long& theXStep, const long& theYStep=1);
	void LocationMaskStep (FmiBoolean newStatus, unsigned long theMaskType, FmiBoolean fResetAllFirst, const long& theXStep, const long& theYStep=1);

	NFmiGrid* AreaFactors(void){return itsAreaFactors;};
	void AreaFactors(NFmiGrid* theAreaFactor);

	const NFmiString& DataFileName(void){return itsDataFileName;}
	void DataFileName(const NFmiString& theDataFileName){itsDataFileName = theDataFileName;}
	NFmiSmartInfo& operator=(const NFmiSmartInfo& theSmartInfo);
	
	FmiBoolean operator==(const NFmiSmartInfo& theSmartInfo) const;
	FmiBoolean operator< (const NFmiSmartInfo& theSmartInfo) const;
	
	void DestroyData(void);
	void DestroySharedData(void){DestroyData();};
	NFmiQueryData* DataReference(void);

	FmiBoolean IsEditable(void){return fEditable;}
	void Editable(FmiBoolean newState){fEditable = newState;}
	
	int Priority(void){return itsPriority;}
	void Priority(int thePriority){itsPriority = thePriority;}

	FmiBoolean LoadedFromFile(void){return *fLoadedFromFile;}
	void LoadedFromFile(FmiBoolean loadedFromFile){*fLoadedFromFile = loadedFromFile;}

	FmiBoolean Extrapolate(void);

	NFmiParamBag* EditedParamBag(void){return itsEditedParamBag;}
	void EditedParamBag(const NFmiParamBag& theParamBag){*itsEditedParamBag = theParamBag;}
	FmiBoolean InitEditedParamBag(void);

	FmiBoolean LocationSelectionSnapShot(void);	// ota maskit talteen
	FmiBoolean LocationSelectionUndo(void);		// kysyy onko undo mahdollinen
	FmiBoolean LocationSelectionRedo(void);		// kysyy onko redo mahdollinen
	void LocationSelectionCommitData(void);		// hyv‰ksy tehdyt muutokset (eli tuhoaa undo mahdollisuudet)
	FmiBoolean LocationSelectionUndoData(void);	// suorittaa todellisen undon
	FmiBoolean LocationSelectionRedoData(void);	// suorittaa todellisen redon
	void LocationSelectionUndoLevel(int theNewUndoLevel); // undolevel asetetaan t‰ll‰
	FmiQueryInfoDataType DataType(void){return itsDataType;}; // 1999.08.24/Marko
	void DataType(FmiQueryInfoDataType newType){itsDataType = newType;}; // 1999.08.24/Marko

 private:

	void LocationMaskStepGrid(FmiBoolean newStatus, FmiBoolean fResetAllFirst, const long& theXStep, const long& theYStep);
	void LocationMaskStepGrid(FmiBoolean newStatus, unsigned long theMaskType, FmiBoolean fResetAllFirst, const long& theXStep, const long& theYStep);
	
	void LocationMaskStepLocation(FmiBoolean newStatus, FmiBoolean fResetAllFirst, const long& theXStep);
	void LocationMaskStepLocation(FmiBoolean newStatus, unsigned long theMaskType, FmiBoolean fResetAllFirst, const long& theXStep);


//   Vain pointteri, ei tuhota destruktorissa. 
//   smartinfo k‰ytt‰‰ oliota maskaamaan aktivaation, 
//	 valinnan ja parametrimaskin.
   
	NFmiUndoableMultiLevelMask* itsAreaMask;
	NFmiGrid* itsAreaFactors;//t‰m‰n gridin avulla yhdistet‰‰n eri aluetoimistojen datat
	NFmiQueryData* itsDataReference;
	
	NFmiString itsDataFileName;
	int itsPriority;
	FmiBoolean fEditable;
	
	long* itsMaxUndoLevelPtr;
	long* itsMaxRedoLevelPtr;
	int* itsCurrentUndoLevelPtr;
	int* itsCurrentRedoLevelPtr;

	char** itsUndoTable;
	NFmiString* itsUndoTextTable;

	//(kertoo onko data ladattu tiedostosta vai tyˆlistan mukaisesti)
	FmiBoolean* fLoadedFromFile;
   	FmiBoolean*	fDirty; //laura muutti pointteriksi, ett‰ tieto s‰ilyisi kaikissa listoissa

	//parambag, siksi ett‰ tiedett‰isiin mit‰ parametri‰ on muutettu
	NFmiParamBag* itsEditedParamBag;
	FmiQueryInfoDataType itsDataType; // 1999.08.24/Marko
};

#endif
