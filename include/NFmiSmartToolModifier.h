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

class NFmiSmartToolCalculationBlockVector
{
public:
	typedef checkedVector<NFmiSmartToolCalculationBlock*>::iterator Iterator;

	NFmiSmartToolCalculationBlockVector(void);
	~NFmiSmartToolCalculationBlockVector(void);
	void Clear(void);
	NFmiSmartInfo* FirstVariableInfo(void);
	void SetTime(const NFmiMetTime &theTime);
	void Calculate(const NFmiPoint &theLatlon, unsigned long theLocationIndex, const NFmiMetTime &theTime, int theTimeIndex);
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
	void Calculate(const NFmiPoint &theLatlon, unsigned long theLocationIndex, const NFmiMetTime &theTime, int theTimeIndex);

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
	void ModifyData(NFmiTimeDescriptor* theModifiedTimes, bool fSelectedLocationsOnly);
	NFmiSmartToolModifier(NFmiInfoOrganizer* theInfoOrganizer);
	~NFmiSmartToolModifier(void);

	bool IsMacroRunnable(void) const {return fMacroRunnable;}
	const std::string& GetErrorText(void) const {return itsErrorText;}
	const std::string& IncludeDirectory(void) const {return itsIncludeDirectory;}
	void IncludeDirectory(const std::string& newValue) {itsIncludeDirectory = newValue;}
	NFmiParamBag ModifiedParams(void);
	const std::string& GetStrippedMacroText(void) const;
	bool IsInterpretedSkriptMacroParam(void); // kun intepreter on tulkinnut smarttool-tekstin, voidaan kysy‰, onko kyseinen makro ns. macroParam-skripti eli sis‰lt‰‰kˆ se RESULT = ??? tapaista teksti‰

private:
	void ModifyConditionalData(NFmiSmartToolCalculationBlock *theCalculationBlock);
	void ModifyBlockData(NFmiSmartToolCalculationBlock *theCalculationBlock);
	NFmiSmartToolCalculationBlockVector* CreateCalculationBlockVector(NFmiSmartToolCalculationBlockInfoVector* theBlockInfoVector);
	NFmiSmartToolCalculationBlock* CreateCalculationBlock(NFmiSmartToolCalculationBlockInfo* theBlockInfo);
	NFmiSmartInfo* CreateRealScriptVariableInfo(const NFmiDataIdent &theDataIdent);
	NFmiSmartInfo* GetScriptVariableInfo(const NFmiDataIdent &theDataIdent);
	void ClearScriptVariableInfos(void);
	NFmiSmartInfo* CreateScriptVariableInfo(const NFmiDataIdent &theDataIdent);
	NFmiAreaMask* CreateCalculatedAreaMask(const NFmiAreaMaskInfo &theAreaMaskInfo);
	void GetParamValueLimits(const NFmiAreaMaskInfo &theAreaMaskInfo, float *theLowerLimit, float *theUpperLimit);
	NFmiDataModifier* CreateIntegrationFuction(const NFmiAreaMaskInfo &theAreaMaskInfo);
	NFmiDataIterator* CreateIterator(const NFmiAreaMaskInfo &theAreaMaskInfo, NFmiSmartInfo* theInfo);
	void ModifyData2(NFmiSmartToolCalculationSection* theCalculationSection);
	NFmiAreaMask* CreateAreaMask(const NFmiAreaMaskInfo &theInfo);
	NFmiAreaMask* CreateEndingAreaMask(void);
	NFmiSmartInfo* CreateInfo(const NFmiAreaMaskInfo &theAreaMaskInfo);
	void CreateCalculationModifiers(void);
	void CreateFirstCalculationSection(void);
	NFmiSmartToolCalculationSection* CreateCalculationSection(NFmiSmartToolCalculationSectionInfo *theCalcSectionInfo);
	NFmiSmartToolCalculation* CreateCalculation(NFmiSmartToolCalculationInfo *theCalcInfo);
	NFmiSmartToolCalculation* CreateConditionalSection(NFmiAreaMaskSectionInfo* theAreaMaskSectionInfo);

	NFmiInfoOrganizer *itsInfoOrganizer; // eli database, ei omista ei tuhoa
	NFmiSmartToolIntepreter *itsSmartToolIntepreter; // omistaa, tuhoaa
	bool fMacroRunnable;
	std::string itsErrorText;

	bool fModifySelectedLocationsOnly;
	checkedVector<NFmiSmartInfo*> itsScriptVariableInfos; // mahdolliset skripti-muuttujat talletetaan t‰nne
	std::string itsIncludeDirectory; // mist‰ ladataan mahd. include filet

	NFmiTimeDescriptor *itsModifiedTimes; // ei omista/tuhoa

	// N‰m‰ muuttujat ovat sit‰ varten ett‰ SumZ ja MinH tyyppisiss‰ funktoissa
	// k‰ytet‰‰n parasta mahdollista level-dataa. Eli ensin hybridi ja sitten painepinta dataa.
	bool fHeightFunctionFlag; // ollaanko tekem‰ss‰ SumZ tai MinH tyyppisen funktion calculaatio-otusta
	bool fUseLevelData; // kun t‰m‰ flagi on p‰‰ll‰, k‰ytet‰‰n CreateInfo-metodissa hybridi-datoja jos mahd. ja sitten painepinta datoja.
	int itsCommaCounter; // tarvitaan laskemaan pilkkuja, kun lasketaan milloin level-dataa pit‰‰ k‰ytt‰‰.
	int itsParethesisCounter; // kun k‰ytet‰‰n esim. Sumz-funktion 2. pilkun j‰lkeen level-dataa, 
							  // pit‰‰ laskea sulkujen avulla, milloin funktio loppuu.
							  // HUOM! sulkujen lis‰ksi pit‰‰ laskea myˆs erilaisten funktioiden alut.
};

#endif
