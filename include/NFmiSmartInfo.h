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
#include <deque>
#include <set>
//#include "FmiNMeteditLibraryDefinitions.h"

class NFmiUndoableMultiLevelMask;
class NFmiBitMask;
class NFmiRect;

// smartinfon pit‰‰ pit‰‰ kirjaa harmonisaattoriin liittyvist‰ asioista
// mitk‰ parametrit ja ajat ovat olleet mill‰kin hetkell‰ muokatttuina
struct NFmiHarmonizerBookKeepingData
{
	NFmiHarmonizerBookKeepingData(void)
	:itsHarmonizerTimesSet()
	,fHarmonizeAllTimes(false)
	,itsHarmonizerParams()
	{}

	NFmiHarmonizerBookKeepingData(const std::set<NFmiMetTime> &theHarmonizerTimesSet,
								  bool /* foobar */ ,
								  const NFmiParamBag &theHarmonizerParams)
	:itsHarmonizerTimesSet(theHarmonizerTimesSet)
	,fHarmonizeAllTimes(false)
	,itsHarmonizerParams(theHarmonizerParams)
	{}

	std::set<NFmiMetTime> itsHarmonizerTimesSet; // t‰nne laitetaan kaikki muokatut ajat jotta voidaan
												// harmonisoinnin yhteydess‰ tehd‰ timedesc, jonka avulla ajetaan
												// harmonisaattori skripti
	bool fHarmonizeAllTimes; // apuna edellisen set:in lis‰ksi, ett‰ jos k‰yd‰‰n l‰pi koko data
	NFmiParamBag itsHarmonizerParams; // t‰h‰n merkit‰‰n parametrit, joita on muokattu eri tyˆkaluilla.
};

class NFmiSmartInfo : public NFmiFastQueryInfo
{
 public:

	NFmiSmartInfo(void);
	NFmiSmartInfo(const NFmiQueryInfo & theInfo
				 ,NFmiQueryData* theData, std::string theDataFileName, std::string theDataFilePattern
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

	bool SnapShotData (const std::string& theAction, const NFmiHarmonizerBookKeepingData &theCurrentHarmonizerBookKeepingData);
	void RearrangeUndoTable(void);
	bool SnapShotData (const std::string& theAction,FmiParameterName theParameter);
	std::string UndoText (void);
	std::string RedoText (void);
	bool Undo (void);
	bool Redo (void);
	void CommitData (void);
	bool UndoData (const NFmiHarmonizerBookKeepingData &theHarmonizerBookKeepingData);
	bool RedoData (void);
	void UndoLevel (const long& theDepth);

	void MaskLocation (const bool& newState);
	void MaskLocation (const bool& newState, unsigned long theMaskType);

	void LocationMaskStep (bool newStatus, bool fResetAllFirst, const long& theXStep, const long& theYStep=1);
	void LocationMaskStep (bool newStatus, unsigned long theMaskType, bool fResetAllFirst, const long& theXStep, const long& theYStep=1);

	NFmiGrid* AreaFactors(void){return itsAreaFactors;};
	void AreaFactors(NFmiGrid* theAreaFactor);

	const std::string& DataFileName(void){return itsDataFileName;}
	void DataFileName(const std::string& theDataFileName){itsDataFileName = theDataFileName;}
	const std::string& DataFilePattern(void) const {return itsDataFilePattern;}
	void DataFilePattern(const std::string &theDataFilePattern) {itsDataFilePattern = theDataFilePattern;}
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
	const NFmiHarmonizerBookKeepingData* CurrentHarmonizerBookKeepingData(void) const; // palauttaa nyt k‰ytˆss‰ olevan harmonisaattori parambagin

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

	std::string itsDataFileName;
	std::string itsDataFilePattern; // t‰t‰ k‰ytet‰‰n tunnistamaan mm. info-organizerissa, ett‰ onko data samanlaista, eli pyyhit‰‰nkˆ vanha t‰ll‰inen data pois alta
	int itsPriority;
	bool fEditable;

	long* itsMaxUndoLevelPtr;
	long* itsMaxRedoLevelPtr;
	int* itsCurrentUndoLevelPtr;
	int* itsCurrentRedoLevelPtr;

	char** itsUndoTable;
	std::string* itsUndoTextTable;

	//(kertoo onko data ladattu tiedostosta vai tyˆlistan mukaisesti)
	bool* fLoadedFromFile;
   	bool*	fDirty; //laura muutti pointteriksi, ett‰ tieto s‰ilyisi kaikissa listoissa

	//parambag, siksi ett‰ tiedett‰isiin mit‰ parametri‰ on muutettu
	NFmiParamBag* itsEditedParamBag;
	NFmiInfoData::Type itsDataType; // 1999.08.24/Marko
	std::deque<NFmiHarmonizerBookKeepingData> *itsUndoRedoHarmonizerBookKeepingData; // t‰m‰n elin kaari seuraa tiiviisti itsUndoTable:a
									// t‰h‰n talletetaan harmonisaatiossa 'likaantuvat' parametrit ja ajat ja koska
									// editorissa on undo/redo toiminto, pit‰‰ myˆs t‰m‰n olla synkassa datan kanssa
};

#endif
