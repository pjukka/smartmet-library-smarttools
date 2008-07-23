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
#include "NFmiQDLog.h"

#include <string>
#include <stdexcept>
#include "NFmiDataMatrix.h"

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
class NFmiInfoOrganizer;

    // Separating the required interface from 'NFmiInfoOrganizer', for
    // Q2 brainstorm module's needs (so it does not need to create whole
    // 'NFmiInfoOrganizer').
    //
#include "NFmiInfoData.h"
class NFmiDrawParam;
class NFmiFastQueryInfo;

class NFmiInfoOrganizer_fake_ {
  public:
    virtual NFmiSmartInfo* CrossSectionMacroParamData() = 0;
    virtual NFmiSmartInfo* MacroParamData() = 0;
    virtual NFmiSmartInfo* EditedInfo() = 0;
    virtual std::vector<NFmiSmartInfo*> GetInfos( int prod_id, const NFmiMetTime* origin_time ) = 0;
	virtual NFmiSmartInfo* AnalyzeDataInfo( const NFmiMetTime* origin_time ) = 0;
	virtual NFmiSmartInfo* CreateShallowCopyInfo( const NFmiDataIdent&, const NFmiLevel*, NFmiInfoData::Type, bool fUseParIdOnly, bool fLevelData, const NFmiMetTime *origin_time ) = 0;
	virtual NFmiDrawParam* CreateDrawParam(const NFmiDataIdent&, const NFmiLevel*, NFmiInfoData::Type) = 0;
	virtual ~NFmiInfoOrganizer_fake_() { };     // = 0;
};

class NFmiSmartToolCalculationBlockVector
{
public:
	typedef checkedVector<NFmiSmartToolCalculationBlock*>::iterator Iterator;

	NFmiSmartToolCalculationBlockVector();
	~NFmiSmartToolCalculationBlockVector();
	void Clear();
	NFmiSmartInfo* FirstVariableInfo();
	void SetTime(const NFmiMetTime &theTime);
	void Calculate(const NFmiPoint &theLatlon, unsigned long theLocationIndex, const NFmiMetTime &theTime, int theTimeIndex, NFmiMacroParamValue &theMacroParamValue);
	void Add(NFmiSmartToolCalculationBlock* theBlock);
	Iterator Begin() {return itsCalculationBlocks.begin();}
	Iterator End() {return itsCalculationBlocks.end();}

private:
	// luokka ei omista vektorissa olevia otuksia, Clear pit‰‰ kutsua erikseen!!!
	checkedVector<NFmiSmartToolCalculationBlock*> itsCalculationBlocks;
};

class NFmiSmartToolCalculationBlock
{
public:
	NFmiSmartToolCalculationBlock();
	~NFmiSmartToolCalculationBlock();
	void Clear();
	NFmiSmartInfo* FirstVariableInfo();
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
	NFmiSmartToolModifier(NFmiInfoOrganizer_fake_*);

    /* It's hard to take this out, either, and get sources to compile. Likely
     * used only when editing points, which Q2 does not anyways support.
     */
	NFmiSmartToolModifier(NFmiInfoOrganizer*) { 
	   LOG_BUG0( "Should not have come here! NOT IMPLEMENTED" );
	   throw std::runtime_error( "'NFmiSmartToolModifier(NFmiInfoOrganizer*)' not available!" ); 
    }
	~NFmiSmartToolModifier();

	bool IsMacroRunnable() const {return fMacroRunnable;}
	const std::string& GetErrorText() const {return itsErrorText;}
	const std::string& IncludeDirectory() const {return itsIncludeDirectory;}
	void IncludeDirectory(const std::string& newValue) {itsIncludeDirectory = newValue;}
	NFmiParamBag ModifiedParams();
	const std::string& GetStrippedMacroText() const;
	bool IsInterpretedSkriptMacroParam(); // kun intepreter on tulkinnut smarttool-tekstin, voidaan kysy‰, onko kyseinen makro ns. macroParam-skripti eli sis‰lt‰‰kˆ se RESULT = ??? tapaista teksti‰
	NFmiSmartInfo* MacroParamData() {return itsMacroParamData;}
	void MacroParamData(NFmiSmartInfo *theInfo) {itsMacroParamData = theInfo;}
private:
	NFmiSmartInfo* UsedMacroParamData();
	void ModifyConditionalData(NFmiSmartToolCalculationBlock *theCalculationBlock, NFmiMacroParamValue &theMacroParamValue);
	void ModifyBlockData(NFmiSmartToolCalculationBlock *theCalculationBlock, NFmiMacroParamValue &theMacroParamValue);
	NFmiSmartToolCalculationBlockVector* CreateCalculationBlockVector(NFmiSmartToolCalculationBlockInfoVector* theBlockInfoVector);
	NFmiSmartToolCalculationBlock* CreateCalculationBlock(NFmiSmartToolCalculationBlockInfo* theBlockInfo);
	NFmiSmartInfo* CreateRealScriptVariableInfo(const NFmiDataIdent &theDataIdent);
	NFmiSmartInfo* GetScriptVariableInfo(const NFmiDataIdent &theDataIdent);
	void ClearScriptVariableInfos();
	NFmiSmartInfo* CreateScriptVariableInfo(const NFmiDataIdent &theDataIdent);
	NFmiAreaMask* CreateCalculatedAreaMask(const NFmiAreaMaskInfo &theAreaMaskInfo);
	void GetParamValueLimits(const NFmiAreaMaskInfo &theAreaMaskInfo, float *theLowerLimit, float *theUpperLimit, bool *fCheckLimits);
	NFmiDataModifier* CreateIntegrationFuction(const NFmiAreaMaskInfo &theAreaMaskInfo);
	NFmiDataIterator* CreateIterator(const NFmiAreaMaskInfo &theAreaMaskInfo, NFmiSmartInfo* theInfo);
	void ModifyData2(NFmiSmartToolCalculationSection* theCalculationSection, NFmiMacroParamValue &theMacroParamValue);
	NFmiAreaMask* CreateAreaMask(const NFmiAreaMaskInfo &theInfo);
	NFmiAreaMask* CreateEndingAreaMask();
	NFmiSmartInfo* CreateInfo(const NFmiAreaMaskInfo &theAreaMaskInfo, bool &mustUsePressureInterpolation );
	NFmiSmartInfo* GetPossibleLevelInterpolatedInfo(const NFmiAreaMaskInfo &theAreaMaskInfo, bool &mustUsePressureInterpolation );
	void CreateCalculationModifiers();
	void CreateFirstCalculationSection();
	NFmiSmartToolCalculationSection* CreateCalculationSection(NFmiSmartToolCalculationSectionInfo *theCalcSectionInfo);
	NFmiSmartToolCalculation* CreateCalculation(NFmiSmartToolCalculationInfo *theCalcInfo);
	NFmiSmartToolCalculation* CreateConditionalSection(NFmiAreaMaskSectionInfo* theAreaMaskSectionInfo);
	NFmiAreaMask* CreateSoundingIndexFunctionAreaMask(const NFmiAreaMaskInfo &theAreaMaskInfo);
	NFmiSmartInfo* CreateSoundingParamInfo(const NFmiDataIdent &theDataIdent, bool useEditedData, const NFmiMetTime *origin_time);
	NFmiSmartInfo* CreateCopyOfAnalyzeInfo(const NFmiDataIdent& theDataIdent, const NFmiLevel* theLevel, const NFmiMetTime* origin_time);

	NFmiInfoOrganizer_fake_ *itsInfoOrganizer_; // eli database, ei omista ei tuhoa
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
