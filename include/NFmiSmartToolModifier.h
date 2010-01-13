//**********************************************************
// C++ Class Name : NFmiSmartToolModifier
// ---------------------------------------------------------
// Filetype: (HEADER)
// Filepath: G:/siirto/marko/oc/NFmiSmartToolModifier.h
//
//
// GDPro Properties
// ---------------------------------------------------
//  - GD Symbol Type    : CLD_Class
//  - GD Method         : UML ( 4.0 )
//  - GD System Name    : aSmartTools
//  - GD View Type      : Class Diagram
//  - GD View Name      : smarttools 1
// ---------------------------------------------------
//  Author         : pietarin
//  Creation Date  : Thur - Jun 20, 2002
//
//  Change Log     :
//
// T‰m‰ luokka hoitaa koko smarttool-toiminnan. Sill‰ on tietokanta,
// tulkki, ja erilaisia maski/operaatio generaattoreita, joiden avulla
// laskut suoritetaan.
//**********************************************************
#ifndef  NFMISMARTTOOLMODIFIER_H
#define  NFMISMARTTOOLMODIFIER_H

#include "NFmiParamBag.h"

#include <string>
#include "NFmiDataMatrix.h"

class NFmiInfoOrganizer;
class NFmiSmartToolIntepreter;
class NFmiTimeDescriptor;
class NFmiAreaMaskInfo;
class NFmiSmartToolCalculationSection;
class NFmiSmartToolCalculationSectionInfo;
class NFmiSmartToolCalculation;
class NFmiSmartToolCalculationInfo;
class NFmiSmartInfo;
class NFmiAreaMaskSectionInfo;
class NFmiAreaMask;
class NFmiDataModifier;
class NFmiDataIterator;
class NFmiSmartToolCalculationBlockInfo;
class NFmiSmartToolCalculationBlockInfoVector;
class NFmiDataIdent;
class NFmiMetTime;
class NFmiPoint;
class NFmiSmartToolCalculationBlock;
class NFmiMacroParamValue;
class NFmiLevel;

class NFmiSmartToolCalculationBlockVector
{
public:
	typedef checkedVector<NFmiSmartToolCalculationBlock*>::iterator Iterator;

	NFmiSmartToolCalculationBlockVector(void);
	~NFmiSmartToolCalculationBlockVector(void);
	void Clear(void);
	NFmiSmartInfo* FirstVariableInfo(void);
	void SetTime(const NFmiMetTime &theTime);
	void Calculate(const NFmiPoint &theLatlon, unsigned long theLocationIndex, const NFmiMetTime &theTime, int theTimeIndex, NFmiMacroParamValue &theMacroParamValue);
	void Add(NFmiSmartToolCalculationBlock* theBlock);
	Iterator Begin(void) {return itsCalculationBlocks.begin();}
	Iterator End(void) {return itsCalculationBlocks.end();}

private:
	// luokka ei omista vektorissa olevia otuksia, Clear pit‰‰ kutsua erikseen!!!
	checkedVector<NFmiSmartToolCalculationBlock*> itsCalculationBlocks;
};

class NFmiSmartToolCalculationBlock
{
public:
	NFmiSmartToolCalculationBlock(void);
	~NFmiSmartToolCalculationBlock(void);
	void Clear(void);
	NFmiSmartInfo* FirstVariableInfo(void);
	void SetTime(const NFmiMetTime &theTime);
	void Calculate(const NFmiPoint &theLatlon, unsigned long theLocationIndex, const NFmiMetTime &theTime, int theTimeIndex, NFmiMacroParamValue &theMacroParamValue);

	NFmiSmartToolCalculationSection* itsFirstCalculationSection;
	NFmiSmartToolCalculation* itsIfAreaMaskSection;
	NFmiSmartToolCalculationBlockVector* itsIfCalculationBlocks;
	NFmiSmartToolCalculation* itsElseIfAreaMaskSection;
	NFmiSmartToolCalculationBlockVector* itsElseIfCalculationBlocks;
	NFmiSmartToolCalculationBlockVector* itsElseCalculationBlocks;
	NFmiSmartToolCalculationSection* itsLastCalculationSection;
};

class NFmiSmartToolModifier
{
public:
	void InitSmartTool(const std::string &theSmartToolText, bool fThisIsMacroParamSkript = false);
	void ModifyData(NFmiTimeDescriptor* theModifiedTimes, bool fSelectedLocationsOnly, bool isMacroParamCalculation);
	void ModifyData(NFmiTimeDescriptor* theModifiedTimes, bool fSelectedLocationsOnly, bool isMacroParamCalculation, NFmiMacroParamValue &theMacroParamValue);
	float CalcSmartToolValue(NFmiMacroParamValue &theMacroParamValue);
	float CalcSmartToolValue(const NFmiMetTime &theTime, const NFmiPoint &theLatlon);
	void CalcCrossSectionSmartToolValues(NFmiDataMatrix<float> &theValues, checkedVector<float> &thePressures, checkedVector<NFmiPoint> &theLatlonPoints, const checkedVector<NFmiMetTime> &thePointTimes);
	NFmiSmartToolModifier(NFmiInfoOrganizer* theInfoOrganizer);
	~NFmiSmartToolModifier(void);

	bool IsMacroRunnable(void) const {return fMacroRunnable;}
	const std::string& GetErrorText(void) const {return itsErrorText;}
	const std::string& IncludeDirectory(void) const {return itsIncludeDirectory;}
	void IncludeDirectory(const std::string& newValue) {itsIncludeDirectory = newValue;}
	NFmiParamBag ModifiedParams(void);
	const std::string& GetStrippedMacroText(void) const;
	bool IsInterpretedSkriptMacroParam(void); // kun intepreter on tulkinnut smarttool-tekstin, voidaan kysy‰, onko kyseinen makro ns. macroParam-skripti eli sis‰lt‰‰kˆ se RESULT = ??? tapaista teksti‰
	NFmiSmartInfo* MacroParamData(void) {return itsMacroParamData;}
	void MacroParamData(NFmiSmartInfo *theInfo) {itsMacroParamData = theInfo;}
private:
	void SetInfosMaskType(NFmiSmartInfo *theInfo);
	NFmiSmartInfo* UsedMacroParamData(void);
	void ModifyConditionalData(NFmiSmartToolCalculationBlock *theCalculationBlock, NFmiMacroParamValue &theMacroParamValue);
	void ModifyBlockData(NFmiSmartToolCalculationBlock *theCalculationBlock, NFmiMacroParamValue &theMacroParamValue);
	NFmiSmartToolCalculationBlockVector* CreateCalculationBlockVector(NFmiSmartToolCalculationBlockInfoVector* theBlockInfoVector);
	NFmiSmartToolCalculationBlock* CreateCalculationBlock(NFmiSmartToolCalculationBlockInfo* theBlockInfo);
	NFmiSmartInfo* CreateRealScriptVariableInfo(const NFmiDataIdent &theDataIdent);
	NFmiSmartInfo* GetScriptVariableInfo(const NFmiDataIdent &theDataIdent);
	void ClearScriptVariableInfos(void);
	NFmiSmartInfo* CreateScriptVariableInfo(const NFmiDataIdent &theDataIdent);
	NFmiAreaMask* CreateCalculatedAreaMask(const NFmiAreaMaskInfo &theAreaMaskInfo);
	void GetParamValueLimits(const NFmiAreaMaskInfo &theAreaMaskInfo, float *theLowerLimit, float *theUpperLimit, bool *fCheckLimits);
	NFmiDataModifier* CreateIntegrationFuction(const NFmiAreaMaskInfo &theAreaMaskInfo);
	NFmiDataIterator* CreateIterator(const NFmiAreaMaskInfo &theAreaMaskInfo, NFmiSmartInfo* theInfo);
	void ModifyData2(NFmiSmartToolCalculationSection* theCalculationSection, NFmiMacroParamValue &theMacroParamValue);
	NFmiAreaMask* CreateAreaMask(const NFmiAreaMaskInfo &theInfo);
	NFmiAreaMask* CreateEndingAreaMask(void);
	NFmiSmartInfo* CreateInfo(const NFmiAreaMaskInfo &theAreaMaskInfo, bool &mustUsePressureInterpolation);
	NFmiSmartInfo* GetPossibleLevelInterpolatedInfo(const NFmiAreaMaskInfo &theAreaMaskInfo, bool &mustUsePressureInterpolation);
	void CreateCalculationModifiers(void);
	void CreateFirstCalculationSection(void);
	NFmiSmartToolCalculationSection* CreateCalculationSection(NFmiSmartToolCalculationSectionInfo *theCalcSectionInfo);
	NFmiSmartToolCalculation* CreateCalculation(NFmiSmartToolCalculationInfo *theCalcInfo);
	NFmiSmartToolCalculation* CreateConditionalSection(NFmiAreaMaskSectionInfo* theAreaMaskSectionInfo);
	NFmiAreaMask* CreateSoundingIndexFunctionAreaMask(const NFmiAreaMaskInfo &theAreaMaskInfo);
//	NFmiSmartInfo* CreateSoundingParamInfo(const NFmiDataIdent &theDataIdent, bool useEditedData);
	NFmiSmartInfo* CreateCopyOfAnalyzeInfo(const NFmiDataIdent& theDataIdent, const NFmiLevel* theLevel);

	NFmiInfoOrganizer *itsInfoOrganizer; // eli database, ei omista ei tuhoa
	NFmiSmartToolIntepreter *itsSmartToolIntepreter; // omistaa, tuhoaa
	bool fMacroRunnable;
	std::string itsErrorText;

	bool fModifySelectedLocationsOnly;
	checkedVector<NFmiSmartInfo*> itsScriptVariableInfos; // mahdolliset skripti-muuttujat talletetaan t‰nne
	std::string itsIncludeDirectory; // mist‰ ladataan mahd. include filet

	NFmiTimeDescriptor *itsModifiedTimes; // ei omista/tuhoa
	bool fMacroParamCalculation; // t‰m‰ tieto tarvitaan scriptVariablejen kanssa, jos true, k‰ytet‰‰n pohjana macroParam-infoa, muuten editoitua dataa

	// N‰m‰ muuttujat ovat sit‰ varten ett‰ SumZ ja MinH tyyppisiss‰ funktoissa
	// k‰ytet‰‰n parasta mahdollista level-dataa. Eli ensin hybridi ja sitten painepinta dataa.
	bool fHeightFunctionFlag; // ollaanko tekem‰ss‰ SumZ tai MinH tyyppisen funktion calculaatio-otusta
	bool fUseLevelData; // kun t‰m‰ flagi on p‰‰ll‰, k‰ytet‰‰n CreateInfo-metodissa hybridi-datoja jos mahd. ja sitten painepinta datoja.
	bool fDoCrossSectionCalculation; // kun t‰m‰ flagi on p‰‰ll‰, ollaan laskemassa poikkileikkauksia ja silloin level-infot yritet‰‰n tehd‰ ensin hybrididatasta ja sitten painepintadatasta
	int itsCommaCounter; // tarvitaan laskemaan pilkkuja, kun lasketaan milloin level-dataa pit‰‰ k‰ytt‰‰.
	int itsParethesisCounter; // kun k‰ytet‰‰n esim. Sumz-funktion 2. pilkun j‰lkeen level-dataa,
							  // pit‰‰ laskea sulkujen avulla, milloin funktio loppuu.
							  // HUOM! sulkujen lis‰ksi pit‰‰ laskea myˆs erilaisten funktioiden alut.
	NFmiSmartInfo* itsMacroParamData; // t‰ss‰ on vara macroParamData, jota k‰ytet‰‰n mm. multithreaddaavassa ymp‰ristˆss‰ (ei omista, ei tuhoa)
};

#endif
