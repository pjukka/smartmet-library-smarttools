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
#include "FmiNMeteditLibraryDefinitions.h"
#include "NFmiProducer.h"

#include <string>
#include <vector>
#include <map>
#include <queue>

class NFmiSmartToolCalculationSectionInfo; 
class NFmiAreaMaskSectionInfo;
class NFmiAreaMaskInfo;
class NFmiSmartToolCalculationInfo;

class NFmiSmartToolIntepreter 
{
public:
	class Exception
	{
	public:
		Exception(const std::string &theText):itsText(theText){}
		const std::string& What(void){return itsText;}
	private:
		const std::string itsText;
	};

	void Interpret(const std::string &theMacroText) throw (NFmiSmartToolIntepreter::Exception);

	NFmiSmartToolIntepreter(void);
	~NFmiSmartToolIntepreter(void);

	void Clear(void);
	const std::string& GetMacroText(void) const {return itsMacroText;}
	const std::string& GetStrippedMacroText(void) const {return itsStrippedMacroText;}

	NFmiSmartToolCalculationSectionInfo* FirstCalculationSectionInfo(void){return itsFirstCalculationSectionInfo;}
	NFmiSmartToolCalculationSectionInfo* LastCalculationSectionInfo(void){return itsLastCalculationSectionInfo;}
	NFmiAreaMaskSectionInfo* IfAreaMaskSectionInfo(void){return itsIfAreaMaskSectionInfo;}
	NFmiSmartToolCalculationSectionInfo* IfCalculationSectionInfo(void){return itsIfCalculationSectionInfo;}
//	std::vector<NFmiAreaMaskSectionInfo*>& ElseIfAreaMaskSectionInfoVector(void){return itsElseIfAreaMaskSectionInfoVector;}
//	std::vector<NFmiSmartToolCalculationSectionInfo*>& ElseIfCalculationSectionInfoVector(void){return itsElseIfCalculationSectionInfoVector;}
	NFmiAreaMaskSectionInfo* ElseIfAreaMaskSectionInfo(void){return itsElseIfAreaMaskSectionInfo;}
	NFmiSmartToolCalculationSectionInfo* ElseIfCalculationSectionInfo(void){return itsElseIfCalculationSectionInfo;}
//	NFmiAreaMaskSectionInfo* ElseAreaMaskSectionInfo(void){return itsElseAreaMaskSectionInfo;}
	bool ElseSectionExist(void){return fElseSectionExist;}
	NFmiSmartToolCalculationSectionInfo* ElseCalculationSectionInfo(void){return itsElseCalculationSectionInfo;}

private:
	typedef std::map<std::string, FmiParameterName> ParamMap;

	std::string NFmiSmartToolIntepreter::HandlePossibleUnaryMarkers(const std::string &theCurrentString);
	NFmiLevel GetPossibleLevelInfo(const std::string &theLevelText) throw (NFmiSmartToolIntepreter::Exception);
	NFmiProducer GetPossibleProducerInfo(const std::string &theProducerText) throw (NFmiSmartToolIntepreter::Exception);
	bool IsProducerOrig(std::string &theProducerText);
	bool FindParamAndLevelAndSetMaskInfo(const std::string &theVariableText, const std::string &theLevelText, NFmiAreaMask::CalculationOperationType theOperType, FmiQueryInfoDataType theDataType, NFmiAreaMaskInfo *theMaskInfo);
	bool FindParamAndProducerAndSetMaskInfo(const std::string &theVariableText, const std::string &theProducerText, NFmiAreaMask::CalculationOperationType theOperType, FmiQueryInfoDataType theDataType, NFmiAreaMaskInfo *theMaskInfo);
	bool FindParamAndLevelAndProducerAndSetMaskInfo(const std::string &theVariableText, const std::string &theLevelText,const std::string &theProducerText, NFmiAreaMask::CalculationOperationType theOperType, FmiQueryInfoDataType theDataType, NFmiAreaMaskInfo *theMaskInfo);
	bool ExtractParamAndLevel(const std::string &theVariableText, std::string *theParamNameOnly, std::string *theLevelNameOnly);
	bool IsVariableTMF(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo);
	bool IsVariableRampFunction(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo) throw (NFmiSmartToolIntepreter::Exception);
	bool FindParamAndSetMaskInfo(const std::string &theVariableText, ParamMap& theParamMap, NFmiAreaMask::CalculationOperationType theOperType, FmiQueryInfoDataType theDataType, NFmiAreaMaskInfo *theMaskInfo);
	bool FindParamAndSetMaskInfo(const std::string &theVariableText, ParamMap& theParamMap, NFmiAreaMask::CalculationOperationType theOperType, FmiQueryInfoDataType theDataType, NFmiAreaMaskInfo *theMaskInfo, const NFmiProducer &theProducer);
	void InterpretDelimiter(const std::string &theDelimText, NFmiAreaMaskInfo *theMaskInfo) throw (NFmiSmartToolIntepreter::Exception);
	void InterpretToken(const std::string &theTokenText, NFmiAreaMaskInfo *theMaskInfo) throw (NFmiSmartToolIntepreter::Exception);
	bool InterpretMaskSection(const std::string &theMaskSectorText, NFmiAreaMaskSectionInfo* theAreaMaskSectionInfo) throw (NFmiSmartToolIntepreter::Exception);
	bool InterpretMasks(std::string &theMaskSectionText, NFmiAreaMaskSectionInfo* theAreaMaskSectionInfo) throw (NFmiSmartToolIntepreter::Exception);
	bool InterpretCalculationSection(std::string &theCalculationSectionText, NFmiSmartToolCalculationSectionInfo* theSectionInfo) throw (NFmiSmartToolIntepreter::Exception);
	NFmiAreaMaskInfo* CreateWantedAreaMaskInfo(const std::string &theMaskSectionText, std::queue<NFmiAreaMaskInfo *> &theMaskQueue) throw (NFmiSmartToolIntepreter::Exception);
	NFmiSmartToolCalculationInfo* InterpretCalculationLine(const std::string &theCalculationLineText) throw (NFmiSmartToolIntepreter::Exception);
	bool InterpretNextMask(const std::string &theMaskSectionText);
	std::string::iterator ExtractFirstCalculationSection(const std::string &theMacroText, std::string::iterator theStartPosition);
	void InitSectionInfos(void);
	void InitCheckOut(void);
	void CheckoutPossibleFirstCalculationSection(void);
	bool CheckoutPossibleIfClauseSection(void);
	bool CheckoutPossibleElseIfClauseSection(void);
	bool CheckoutPossibleElseClauseSection(void);
	bool CheckoutPossibleNextCalculationSection(NFmiSmartToolCalculationSectionInfo* theSectionInfo);
	void CheckoutPossibleLastCalculationSection(void);
	bool ExtractPossibleNextCalculationSection(void) throw (NFmiSmartToolIntepreter::Exception);
	bool ExtractPossibleIfClauseSection(void);
	bool ExtractPossibleElseIfClauseSection(void);
	template<typename memfunction>
	bool ExtractPossibleConditionalClauseSection(memfunction conditionalChecker);
	bool IsPossibleCalculationLine(const std::string &theTextLine);
	bool IsPossibleIfConditionLine(const std::string &theTextLine);
	bool IsPossibleElseIfConditionLine(const std::string &theTextLine);
	bool IsPossibleElseConditionLine(const std::string &theTextLine) throw (NFmiSmartToolIntepreter::Exception);
	bool FindAnyFromText(const std::string &theText, const std::vector<std::string>& theSearchedItems);
	bool ConsistOnlyWhiteSpaces(const std::string &theText);
	bool IsVariableBinaryOperator(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo);
	NFmiAreaMask::CalculationOperator InterpretCalculationOperator(const std::string &theOperatorText) throw (NFmiSmartToolIntepreter::Exception);
	void InterpretVariable(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo) throw (NFmiSmartToolIntepreter::Exception);
	void CheckVariableString(const std::string &theVariableText, std::string &theParamText,
							 bool &fLevelExist, std::string &theLevelText,
							 bool &fProducerExist, std::string &theProducerText) throw (NFmiSmartToolIntepreter::Exception);
	template<typename mapType, typename T>
	bool IsInMap(mapType& theMap, const T &theSearchedItem);
	bool IsVariableConstantValue(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo);
	std::string ExtractNextLine(std::string &theText, std::string::iterator theStartPos, std::string::iterator* theEndPos);
	bool IsVariableFunction(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo) throw (NFmiSmartToolIntepreter::Exception);
	bool IsVariableMathFunction(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo) throw (NFmiSmartToolIntepreter::Exception);
	std::string itsCheckOutSectionText; // esim. if-sectionin koko teksti
	std::string::iterator itsCheckOutTextStartPosition; // sen hetkinen tekstiosan alkupiste
	std::string::iterator itsCheckOutTextEndPosition; // sen hetkinen tekstiosan alkupiste
	bool fContinueCurrentSectionCheckOut; // jatketaanko sen hetkisen sectionin tutkimista, vai onko selv‰‰, ettei esim. else-sectionia ole olemassa

	void SetMacroTexts(const std::string &theMacroText) throw (NFmiSmartToolIntepreter::Exception);
	std::string itsMacroText;
	std::string itsStrippedMacroText;

	NFmiSmartToolCalculationSectionInfo* itsFirstCalculationSectionInfo;
	NFmiSmartToolCalculationSectionInfo* itsLastCalculationSectionInfo;
	NFmiAreaMaskSectionInfo* itsIfAreaMaskSectionInfo;
	NFmiSmartToolCalculationSectionInfo* itsIfCalculationSectionInfo;
//	std::vector<NFmiAreaMaskSectionInfo*> itsElseIfAreaMaskSectionInfoVector;
//	std::vector<NFmiSmartToolCalculationSectionInfo*> itsElseIfCalculationSectionInfoVector;
	NFmiAreaMaskSectionInfo* itsElseIfAreaMaskSectionInfo;
	NFmiSmartToolCalculationSectionInfo* itsElseIfCalculationSectionInfo;
	bool fElseSectionExist;
	NFmiSmartToolCalculationSectionInfo* itsElseCalculationSectionInfo;

	int itsMaxCalculationSectionCount;

	static void InitTokens(void);
	static bool fTokensInitialized;
	static ParamMap itsTokenParameterNamesAndIds;
	typedef std::map<std::string, FmiProducerName> ProducerMap;
	static ProducerMap itsTokenProducerNamesAndIds;
	static std::vector<std::string> itsTokenConditionalCommands;
	static std::vector<std::string> itsTokenIfCommands;
	static std::vector<std::string> itsTokenElseIfCommands;
	static std::vector<std::string> itsTokenElseCommands;
	static std::vector<std::string> itsTokenCalculationBlockMarkers;
	static std::vector<std::string> itsTokenMaskBlockMarkers;
//	static std::vector<std::string> itsTokenFunctions;
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

	typedef std::map<std::string, std::pair<unsigned long, unsigned long> > LevelMap; // nimi, ident, levelValue 
	static LevelMap itsTokenLevelNamesIdentsAndValues;

	typedef std::map<std::string, NFmiAreaMask::FunctionType> FunctionMap;
	static FunctionMap itsTokenFunctions;

	typedef std::map<std::string, NFmiAreaMask::MathFunctionType> MathFunctionMap;
	static MathFunctionMap itsMathFunctions;

// GetToken ja IsDelim otettu H. Schilbertin  C++: the Complete Refeference third ed.
// jouduin muuttamaan niit‰ v‰h‰n sopimaan t‰h‰n ymp‰ristˆˆn.
	bool GetToken(void);
	bool IsDelim(char c);
	// Ed. funktiot k‰ytt‰v‰t seuraavia muuttujia:
	enum types { NOTYPE=0, DELIMITER = 1, VARIABLE, NUMBER};
	char *exp_ptr;  // points to the expression
	char token[128]; // holds current token
	types tok_type;  // holds token's type
	
};
#endif

