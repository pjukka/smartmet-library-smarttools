//**********************************************************
// C++ Class Name : NFmiSmartToolIntepreter 
// ---------------------------------------------------------
// Filetype: (HEADER)
// Filepath: G:/siirto/marko/oc/NFmiSmartToolIntepreter.h 
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
// Aluksi simppeli smarttool-tulkki. Macro voi olla muotoa:
// 
// calculationSection1
// IF(mask1)
//   calculationSection2
// ELSEIF(mask2)
//   calculationSection3
// ELSE(mask3)
// {
//   calculationSection4
// }
// calculationSection5
// 
// Eli Voi olla laskuja, jotka suoritetaan aina ensin (calculationSection1), sitten tulee
// haluttu m‰‰r‰ IF - ELSEIF - ELSE osioita, joihin jokaiseen liittyy jokin calculationSection.
// ELSEIF:j‰ voi olla useita, mutta IF ja ELSE osioita voi olla vain yksi kumpaakin
// ja niiden on oltava p‰iss‰ (IF alussa ja ELSE lopussa).
// Lopuksi viel‰ on calculationSection, joka suoritetaan aina.
// Kaikki osiot sik‰ vapaa ehtoisia, ettei mit‰‰n tarvitse olla, mutta esim IF, ELSEIF ja ELSE:n j‰lkeen pit‰‰ tulla calculationSection.
//**********************************************************
#ifndef  NFMISMARTTOOLINTEPRETER_H
#define  NFMISMARTTOOLINTEPRETER_H

#include "NFmiParameterName.h"
#include "NFmiProducerName.h"
#include "NFmiGlobals.h"
#include "NFmiAreaMask.h"
//#include "FmiNMeteditLibraryDefinitions.h"
#include "NFmiProducer.h"
#include "NFmiLeveltype.h"

#include <string>
#include <vector>
#include <map>
#include <set>
#include <queue>

class NFmiSmartToolCalculationSectionInfo; 
class NFmiAreaMaskSectionInfo;
class NFmiAreaMaskInfo;
class NFmiSmartToolCalculationInfo;
class NFmiInfoOrganizer;

class NFmiSmartToolCalculationBlock
{
public:
	NFmiSmartToolCalculationBlock(void);
	~NFmiSmartToolCalculationBlock(void);
	void Clear(void);
	
	NFmiSmartToolCalculationSectionInfo* itsFirstCalculationSectionInfo;
	NFmiSmartToolCalculationSectionInfo* itsLastCalculationSectionInfo;
	NFmiAreaMaskSectionInfo* itsIfAreaMaskSectionInfo;
	NFmiSmartToolCalculationSectionInfo* itsIfCalculationSectionInfo;
	NFmiAreaMaskSectionInfo* itsElseIfAreaMaskSectionInfo;
	NFmiSmartToolCalculationSectionInfo* itsElseIfCalculationSectionInfo;
	bool fElseSectionExist;
	NFmiSmartToolCalculationSectionInfo* itsElseCalculationSectionInfo;
};

class NFmiSmartToolIntepreter 
{
public:
	typedef std::map<std::string, FmiProducerName> ProducerMap;
	typedef std::map<std::string, std::pair<unsigned long, unsigned long> > LevelMap; // nimi, ident, levelValue 
	typedef std::map<std::string, double> ConstantMap; // esim. MISS 32700 tai PI 3.14159
	class Exception
	{
	public:
		Exception(const std::string &theText):itsText(theText){}
		const std::string& What(void){return itsText;}
	private:
		const std::string itsText;
	};

	void Interpret(const std::string &theMacroText);

	NFmiSmartToolIntepreter(NFmiInfoOrganizer* theInfoOrganizer);
	~NFmiSmartToolIntepreter(void);

	void Clear(void);
	const std::string& GetMacroText(void) const {return itsMacroText;}
	const std::string& GetStrippedMacroText(void) const {return itsStrippedMacroText;}

	std::vector<NFmiSmartToolCalculationBlock>& SmartToolCalculationBlocks(void){return itsSmartToolCalculationBlocks;}

private:
	typedef std::map<std::string, FmiParameterName> ParamMap;

	bool CheckoutPossibleNextCalculationBlock(NFmiSmartToolCalculationBlock* theBlock);
	std::string HandlePossibleUnaryMarkers(const std::string &theCurrentString);
	NFmiLevel GetPossibleLevelInfo(const std::string &theLevelText, NFmiInfoData::Type theDataType);
	NFmiProducer GetPossibleProducerInfo(const std::string &theProducerText);
	bool IsProducerOrig(std::string &theProducerText);
	bool FindParamAndLevelAndSetMaskInfo(const std::string &theVariableText, const std::string &theLevelText, NFmiAreaMask::CalculationOperationType theOperType, NFmiInfoData::Type theDataType, NFmiAreaMaskInfo *theMaskInfo);
	bool FindParamAndProducerAndSetMaskInfo(const std::string &theVariableText, const std::string &theProducerText, NFmiAreaMask::CalculationOperationType theOperType, NFmiInfoData::Type theDataType, NFmiAreaMaskInfo *theMaskInfo);
	bool FindParamAndLevelAndProducerAndSetMaskInfo(const std::string &theVariableText, const std::string &theLevelText,const std::string &theProducerText, NFmiAreaMask::CalculationOperationType theOperType, NFmiInfoData::Type theDataType, NFmiAreaMaskInfo *theMaskInfo);
	bool ExtractParamAndLevel(const std::string &theVariableText, std::string *theParamNameOnly, std::string *theLevelNameOnly);
	bool IsVariableTMF(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo);
	bool IsVariableRampFunction(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo);
	bool FindParamAndSetMaskInfo(const std::string &theVariableText, ParamMap& theParamMap, NFmiAreaMask::CalculationOperationType theOperType, NFmiInfoData::Type theDataType, NFmiAreaMaskInfo *theMaskInfo);
	bool FindParamAndSetMaskInfo(const std::string &theVariableText, ParamMap& theParamMap, NFmiAreaMask::CalculationOperationType theOperType, NFmiInfoData::Type theDataType, NFmiAreaMaskInfo *theMaskInfo, const NFmiProducer &theProducer);
	void InterpretDelimiter(const std::string &theDelimText, NFmiAreaMaskInfo *theMaskInfo);
	void InterpretToken(const std::string &theTokenText, NFmiAreaMaskInfo *theMaskInfo);
	bool InterpretMaskSection(const std::string &theMaskSectorText, NFmiAreaMaskSectionInfo* theAreaMaskSectionInfo);
	bool InterpretMasks(std::string &theMaskSectionText, NFmiAreaMaskSectionInfo* theAreaMaskSectionInfo);
	bool InterpretCalculationSection(std::string &theCalculationSectionText, NFmiSmartToolCalculationSectionInfo* theSectionInfo);
	NFmiAreaMaskInfo* CreateWantedAreaMaskInfo(const std::string &theMaskSectionText, std::queue<NFmiAreaMaskInfo *> &theMaskQueue);
	NFmiSmartToolCalculationInfo* InterpretCalculationLine(const std::string &theCalculationLineText);
	bool InterpretNextMask(const std::string &theMaskSectionText);
#if (defined _MSC_VER) && (_MSC_VER < 1310) // 1310 eli k‰‰nt‰j‰n versio 13.1 eli MSVC++ 7.1  toteuttaa 1. kerran kunnolla standardia
	std::string::iterator ExtractFirstCalculationSection(const std::string &theMacroText, std::string::iterator theStartPosition);
#else
	std::string::const_iterator ExtractFirstCalculationSection(const std::string &theMacroText, std::string::iterator theStartPosition);
#endif
	void InitCheckOut(void);
	bool IsCaseInsensitiveEqual(const std::string &theStr1, const std::string &theStr2);
	bool IsPossiblyLevelItem(const std::string &theText, LevelMap &theMap);
	bool IsPossiblyProducerItem(const std::string &theText, ProducerMap &theMap);
	bool GetProducerFromVariableById(const std::string &theVariableText, NFmiProducer &theProducer);
	bool GetLevelFromVariableById(const std::string &theVariableText, NFmiLevel &theLevel, NFmiInfoData::Type theDataType);
	bool IsWantedStart(const std::string &theText, const std::string &theWantedStart);
	bool GetParamFromVariable(const std::string &theVariableText, ParamMap& theParamMap, NFmiParam &theParam, bool &fUseWildDataType);
	bool GetParamFromVariableById(const std::string &theVariableText, NFmiParam &theParam);
	bool CheckoutPossibleIfClauseSection(NFmiAreaMaskSectionInfo* theAreaMaskSectionInfo);
	bool CheckoutPossibleElseIfClauseSection(NFmiAreaMaskSectionInfo* theAreaMaskSectionInfo);
	bool CheckoutPossibleElseClauseSection(void);
	bool CheckoutPossibleNextCalculationSection(NFmiSmartToolCalculationSectionInfo* theSectionInfo);
	bool ExtractPossibleNextCalculationSection(void);
	bool ExtractPossibleIfClauseSection(void);
	bool ExtractPossibleElseIfClauseSection(void);
	template<typename memfunction>
	bool ExtractPossibleConditionalClauseSection(memfunction conditionalChecker);
	bool IsPossibleCalculationLine(const std::string &theTextLine);
	bool IsPossibleIfConditionLine(const std::string &theTextLine);
	bool IsPossibleElseIfConditionLine(const std::string &theTextLine);
	bool IsPossibleElseConditionLine(const std::string &theTextLine);
	bool FindAnyFromText(const std::string &theText, const std::vector<std::string>& theSearchedItems);
	bool ConsistOnlyWhiteSpaces(const std::string &theText);
	bool IsVariableBinaryOperator(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo);
	NFmiAreaMask::CalculationOperator InterpretCalculationOperator(const std::string &theOperatorText);
	void InterpretVariable(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo, bool fNewScriptVariable = false);
	bool InterpretVariableCheckTokens(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo, bool fOrigWanted, bool fLevelExist, bool fProducerExist, const std::string &theParamNameOnly, const std::string &theLevelNameOnly, const std::string &theProducerNameOnly);
	bool InterpretPossibleScriptVariable(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo, bool fNewScriptVariable);
	void CheckVariableString(const std::string &theVariableText, std::string &theParamText,
							 bool &fLevelExist, std::string &theLevelText,
							 bool &fProducerExist, std::string &theProducerText);
	template<typename mapType>
	bool IsInMap(mapType& theMap, const std::string &theSearchedItem);
	bool IsVariableConstantValue(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo);
	std::string ExtractNextLine(std::string &theText, std::string::iterator theStartPos, std::string::iterator* theEndPos);
	bool IsVariableFunction(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo);
	bool IsVariableMathFunction(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo);
	FmiLevelType GetLevelType(NFmiInfoData::Type theDataType, long levelValue);

	NFmiInfoOrganizer* itsInfoOrganizer; // ei omista
	std::string itsCheckOutSectionText; // esim. if-sectionin koko teksti
	std::string::iterator itsCheckOutTextStartPosition; // sen hetkinen tekstiosan alkupiste
	std::string::iterator itsCheckOutTextEndPosition; // sen hetkinen tekstiosan alkupiste
	bool fContinueCurrentSectionCheckOut; // jatketaanko sen hetkisen sectionin tutkimista, vai onko selv‰‰, ettei esim. else-sectionia ole olemassa

	void SetMacroTexts(const std::string &theMacroText);
	std::string itsMacroText;
	std::string itsStrippedMacroText;

	std::vector<NFmiSmartToolCalculationBlock> itsSmartToolCalculationBlocks;

	int itsMaxCalculationSectionCount;

	static void InitTokens(void);
	static bool fTokensInitialized;
	static ParamMap itsTokenParameterNamesAndIds;
	static ProducerMap itsTokenProducerNamesAndIds;
	static ConstantMap itsTokenConstants; 
	static std::vector<std::string> itsTokenConditionalCommands;
	static std::vector<std::string> itsTokenIfCommands;
	static std::vector<std::string> itsTokenElseIfCommands;
	static std::vector<std::string> itsTokenElseCommands;
	static std::vector<std::string> itsTokenCalculationBlockMarkers;
	static std::vector<std::string> itsTokenMaskBlockMarkers;
	static std::vector<std::string> itsTokenRampUpFunctions;
	static std::vector<std::string> itsTokenRampDownFunctions;
	static std::vector<std::string> itsTokenDoubleRampFunctions;
	static std::vector<std::string> itsTokenRampFunctions;
	static std::vector<std::string> itsTokenTMFs; // aikasarjamuokkauskerroin

	typedef std::map<std::string, FmiMaskOperation> MaskOperMap;
	static MaskOperMap itsTokenMaskOperations;
	typedef std::map<std::string, NFmiAreaMask::CalculationOperator> CalcOperMap;
	static CalcOperMap itsCalculationOperations;
	typedef std::map<std::string, NFmiAreaMask::BinaryOperator> BinaOperMap;
	static BinaOperMap itsBinaryOperator;
	static ParamMap itsTokenStaticParameterNamesAndIds;
	static ParamMap itsTokenCalculatedParameterNamesAndIds; // mm. lat, lon ja elevAngle

	static LevelMap itsTokenLevelNamesIdentsAndValues;

	typedef std::map<std::string, NFmiAreaMask::FunctionType> FunctionMap;
	static FunctionMap itsTokenFunctions;

	typedef std::map<std::string, NFmiAreaMask::MathFunctionType> MathFunctionMap;
	static MathFunctionMap itsMathFunctions;

	typedef std::map<std::string, int> ScriptVariableMap;
	ScriptVariableMap itsTokenScriptVariableNames; // skriptiss‰ varatut muuttujat (var x = ?) talletetaan t‰nne, ett‰ voidaan tarkistaa niiden olemassa olo
	int itsScriptVariableParamIdCounter; // pit‰‰ keksia muutujille id, joten tehd‰‰ juokseva counter

// GetToken ja IsDelim otettu H. Schilbertin  C++: the Complete Refeference third ed.
// jouduin muuttamaan niit‰ v‰h‰n sopimaan t‰h‰n ymp‰ristˆˆn.
	bool GetToken(void);
	bool IsDelim(char c);
	// Ed. funktiot k‰ytt‰v‰t seuraavia muuttujia:
	enum types { NOTYPE=0, DELIMITER = 1, VARIABLE, NUMBER};
    std::string::iterator exp_ptr;  // points to the expression
    std::string::iterator exp_end;
	char token[128]; // holds current token
	types tok_type;  // holds token's type
	
};
#endif

