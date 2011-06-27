#pragma once
//**********************************************************
// C++ Class Name : NFmiSmartToolIntepreter
// ---------------------------------------------------------
//  Author         : pietarin
//  Creation Date  : 8.11. 2010
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

#include "NFmiParameterName.h"
#include "NFmiProducerName.h"
#include "NFmiAreaMask.h"
#include "NFmiProducer.h"
#include "NFmiLevelType.h"
#include "NFmiParamBag.h"
#include "NFmiDataMatrix.h"

#include <string>
#include <map>
#include <set>
#include <queue>
#include "boost/shared_ptr.hpp"

class NFmiSmartToolCalculationSectionInfo;
class NFmiAreaMaskSectionInfo;
class NFmiAreaMaskInfo;
class NFmiSmartToolCalculationInfo;
class NFmiSmartToolCalculationBlockInfo;
class NFmiProducerSystem;

class NFmiSmartToolCalculationBlockInfoVector
{
public:
	typedef checkedVector<boost::shared_ptr<NFmiSmartToolCalculationBlockInfo> >::iterator Iterator;

	NFmiSmartToolCalculationBlockInfoVector(void);
	~NFmiSmartToolCalculationBlockInfoVector(void);
	void Clear(void);
	void Add(boost::shared_ptr<NFmiSmartToolCalculationBlockInfo> &theBlockInfo);
	void AddModifiedParams(std::set<int> &theModifiedParams);
	Iterator Begin(void) {return itsCalculationBlockInfos.begin();};
	Iterator End(void) {return itsCalculationBlockInfos.end();};
	bool Empty(void) const{return itsCalculationBlockInfos.empty();}

private:
	// luokka ei omista vektorissa olevia otuksia, Clear pit‰‰ kutsua erikseen!!!
	checkedVector<boost::shared_ptr<NFmiSmartToolCalculationBlockInfo> > itsCalculationBlockInfos;
};

class NFmiSmartToolCalculationBlockInfo
{
public:
	NFmiSmartToolCalculationBlockInfo(void);
	~NFmiSmartToolCalculationBlockInfo(void);
	void Clear(void);
	void AddModifiedParams(std::set<int> &theModifiedParams);

	// luokka ei omista n‰it‰, Clear pit‰‰ kutsua erikseen!!!
	boost::shared_ptr<NFmiSmartToolCalculationSectionInfo> itsFirstCalculationSectionInfo;
	boost::shared_ptr<NFmiAreaMaskSectionInfo> itsIfAreaMaskSectionInfo;
	boost::shared_ptr<NFmiSmartToolCalculationBlockInfoVector> itsIfCalculationBlockInfos;
	boost::shared_ptr<NFmiAreaMaskSectionInfo> itsElseIfAreaMaskSectionInfo;
	boost::shared_ptr<NFmiSmartToolCalculationBlockInfoVector> itsElseIfCalculationBlockInfos;
	bool fElseSectionExist;
	boost::shared_ptr<NFmiSmartToolCalculationBlockInfoVector> itsElseCalculationBlockInfos;
	boost::shared_ptr<NFmiSmartToolCalculationSectionInfo> itsLastCalculationSectionInfo;
};

class NFmiSmartToolIntepreter
{
public:
	typedef std::map<std::string, FmiProducerName> ProducerMap;
	typedef std::map<std::string, double> ConstantMap; // esim. MISS 32700 tai PI 3.14159
	void Interpret(const std::string &theMacroText, bool fThisIsMacroParamSkript = false);

	NFmiSmartToolIntepreter(NFmiProducerSystem *theProducerSystem);
	~NFmiSmartToolIntepreter(void);

	void Clear(void);
	const std::string& GetMacroText(void) const {return itsMacroText;}
	const std::string& GetStrippedMacroText(void) const {return itsStrippedMacroText;}
	const std::string& IncludeDirectory(void) const {return itsIncludeDirectory;}
	void IncludeDirectory(const std::string& newValue) {itsIncludeDirectory = newValue;}

	checkedVector<NFmiSmartToolCalculationBlockInfo>& SmartToolCalculationBlocks(void){return itsSmartToolCalculationBlocks;}
	NFmiParamBag ModifiedParams(void);
	NFmiParam GetParamFromString(const std::string &theParamText);
	bool IsInterpretedSkriptMacroParam(void); // kun intepreter on tulkinnut smarttool-tekstin, voidaan kysy‰, onko kyseinen makro ns. macroParam-skripti eli sis‰lt‰‰kˆ se RESULT = ??? tapaista teksti‰

private:
	typedef std::map<std::string, FmiParameterName> ParamMap;

	bool CheckoutPossibleNextCalculationBlockVector(boost::shared_ptr<NFmiSmartToolCalculationBlockInfoVector> &theBlockVector);
	bool CheckoutPossibleNextCalculationBlock(NFmiSmartToolCalculationBlockInfo &theBlock, bool fFirstLevelCheckout, int theBlockIndex = -1);
	std::string HandlePossibleUnaryMarkers(const std::string &theCurrentString);
	NFmiLevel GetPossibleLevelInfo(const std::string &theLevelText, NFmiInfoData::Type theDataType);
	NFmiProducer GetPossibleProducerInfo(const std::string &theProducerText);
	bool IsProducerOrig(std::string &theProducerText);
	bool FindParamAndLevelAndSetMaskInfo(const std::string &theVariableText, const std::string &theLevelText, NFmiAreaMask::CalculationOperationType theOperType, NFmiInfoData::Type theDataType, boost::shared_ptr<NFmiAreaMaskInfo> &theMaskInfo);
	bool FindParamAndProducerAndSetMaskInfo(const std::string &theVariableText, const std::string &theProducerText, NFmiAreaMask::CalculationOperationType theOperType, NFmiInfoData::Type theDataType, boost::shared_ptr<NFmiAreaMaskInfo> &theMaskInfo);
	bool FindParamAndLevelAndProducerAndSetMaskInfo(const std::string &theVariableText, const std::string &theLevelText,const std::string &theProducerText, NFmiAreaMask::CalculationOperationType theOperType, NFmiInfoData::Type theDataType, boost::shared_ptr<NFmiAreaMaskInfo> &theMaskInfo);
	bool ExtractParamAndLevel(const std::string &theVariableText, std::string *theParamNameOnly, std::string *theLevelNameOnly);
	bool IsVariableMacroParam(const std::string &theVariableText, boost::shared_ptr<NFmiAreaMaskInfo> &theMaskInfo);
	bool IsVariableDeltaZ(const std::string &theVariableText, boost::shared_ptr<NFmiAreaMaskInfo> &theMaskInfo);
	bool IsVariableRampFunction(const std::string &theVariableText, boost::shared_ptr<NFmiAreaMaskInfo> &theMaskInfo);
	bool FindParamAndSetMaskInfo(const std::string &theVariableText, ParamMap& theParamMap, NFmiAreaMask::CalculationOperationType theOperType, NFmiInfoData::Type theDataType, boost::shared_ptr<NFmiAreaMaskInfo> &theMaskInfo);
	bool FindParamAndSetMaskInfo(const std::string &theVariableText, ParamMap& theParamMap, NFmiAreaMask::CalculationOperationType theOperType, NFmiInfoData::Type theDataType, boost::shared_ptr<NFmiAreaMaskInfo> &theMaskInfo, const NFmiProducer &theProducer);
	void InterpretDelimiter(const std::string &theDelimText, boost::shared_ptr<NFmiAreaMaskInfo> &theMaskInfo);
	void InterpretToken(const std::string &theTokenText, boost::shared_ptr<NFmiAreaMaskInfo> &theMaskInfo);
	bool InterpretMaskSection(const std::string &theMaskSectorText, boost::shared_ptr<NFmiAreaMaskSectionInfo> &theAreaMaskSectionInfo);
	bool InterpretMasks(std::string &theMaskSectionText, boost::shared_ptr<NFmiAreaMaskSectionInfo> &theAreaMaskSectionInfo);
	bool InterpretCalculationSection(std::string &theCalculationSectionText, boost::shared_ptr<NFmiSmartToolCalculationSectionInfo> &theSectionInfo);
	boost::shared_ptr<NFmiAreaMaskInfo> CreateWantedAreaMaskInfo(const std::string &theMaskSectionText, std::queue<boost::shared_ptr<NFmiAreaMaskInfo> > &theMaskQueue);
	boost::shared_ptr<NFmiSmartToolCalculationInfo> InterpretCalculationLine(const std::string &theCalculationLineText);
	bool InterpretNextMask(const std::string &theMaskSectionText);

  // 1310 eli k‰‰nt‰j‰n versio 13.1 eli MSVC++ 7.1  toteuttaa 1. kerran kunnolla standardia

#ifdef OLD_MSC
	std::string::iterator ExtractFirstCalculationSection(const std::string &theMacroText, std::string::iterator theStartPosition);
#else
	std::string::const_iterator ExtractFirstCalculationSection(const std::string &theMacroText, std::string::iterator theStartPosition);
#endif

	void InitCheckOut(void);
	bool IsCaseInsensitiveEqual(const std::string &theStr1, const std::string &theStr2);
	bool IsPossiblyLevelItem(const std::string &theText);
	bool IsPossiblyProducerItem(const std::string &theText, ProducerMap &theMap);
	bool GetProducerFromVariableById(const std::string &theVariableText, NFmiProducer &theProducer);
	bool GetLevelFromVariableById(const std::string &theVariableText, NFmiLevel &theLevel, NFmiInfoData::Type theDataType);
	bool IsWantedStart(const std::string &theText, const std::string &theWantedStart);
	bool GetParamFromVariable(const std::string &theVariableText, ParamMap& theParamMap, NFmiParam &theParam, bool &fUseWildDataType);
	bool GetParamFromVariableById(const std::string &theVariableText, NFmiParam &theParam);
	bool CheckoutPossibleIfClauseSection(boost::shared_ptr<NFmiAreaMaskSectionInfo> &theAreaMaskSectionInfo);
	bool CheckoutPossibleElseIfClauseSection(boost::shared_ptr<NFmiAreaMaskSectionInfo> &theAreaMaskSectionInfo);
	bool CheckoutPossibleElseClauseSection(void);
	bool CheckoutPossibleNextCalculationSection(boost::shared_ptr<NFmiSmartToolCalculationSectionInfo> &theSectionInfo, bool &fWasBlockMarksFound);
	bool ExtractPossibleNextCalculationSection(bool &fWasBlockMarksFound);
	bool ExtractPossibleIfClauseSection(void);
	bool ExtractPossibleElseIfClauseSection(void);
	template<typename memfunction>
	bool ExtractPossibleConditionalClauseSection(memfunction conditionalChecker);
	bool IsPossibleCalculationLine(const std::string &theTextLine);
	bool IsPossibleIfConditionLine(const std::string &theTextLine);
	bool IsPossibleElseIfConditionLine(const std::string &theTextLine);
	bool IsPossibleElseConditionLine(const std::string &theTextLine);
	bool FindAnyFromText(const std::string &theText, const checkedVector<std::string>& theSearchedItems);
	bool ConsistOnlyWhiteSpaces(const std::string &theText);
	bool IsVariableBinaryOperator(const std::string &theVariableText, boost::shared_ptr<NFmiAreaMaskInfo> &theMaskInfo);
	NFmiAreaMask::CalculationOperator InterpretCalculationOperator(const std::string &theOperatorText);
	void InterpretVariable(const std::string &theVariableText, boost::shared_ptr<NFmiAreaMaskInfo> &theMaskInfo, bool fNewScriptVariable = false);
	bool InterpretVariableCheckTokens(const std::string &theVariableText, boost::shared_ptr<NFmiAreaMaskInfo> &theMaskInfo, bool fOrigWanted, bool fLevelExist, bool fProducerExist, const std::string &theParamNameOnly, const std::string &theLevelNameOnly, const std::string &theProducerNameOnly);
	bool InterpretPossibleScriptVariable(const std::string &theVariableText, boost::shared_ptr<NFmiAreaMaskInfo> &theMaskInfo, bool fNewScriptVariable);
	void CheckVariableString(const std::string &theVariableText, std::string &theParamText,
							 bool &fLevelExist, std::string &theLevelText,
							 bool &fProducerExist, std::string &theProducerText);
	template<typename mapType>
	bool IsInMap(mapType& theMap, const std::string &theSearchedItem);
	bool IsVariableConstantValue(const std::string &theVariableText, boost::shared_ptr<NFmiAreaMaskInfo> &theMaskInfo);
	std::string ExtractNextLine(std::string &theText, std::string::iterator theStartPos, std::string::iterator* theEndPos);
	bool IsVariableThreeArgumentFunction(const std::string &theVariableText, boost::shared_ptr<NFmiAreaMaskInfo> &theMaskInfo);
	bool IsVariableFunction(const std::string &theVariableText, boost::shared_ptr<NFmiAreaMaskInfo> &theMaskInfo);
	bool IsVariablePeekFunction(const std::string &theVariableText, boost::shared_ptr<NFmiAreaMaskInfo> &theMaskInfo);
	bool IsVariableMetFunction(const std::string &theVariableText, boost::shared_ptr<NFmiAreaMaskInfo> &theMaskInfo);
	bool IsVariableMathFunction(const std::string &theVariableText, boost::shared_ptr<NFmiAreaMaskInfo> &theMaskInfo);

	NFmiProducerSystem *itsProducerSystem;  // ei omista
	std::string itsCheckOutSectionText; // esim. if-sectionin koko teksti
	std::string::iterator itsCheckOutTextStartPosition; // sen hetkinen tekstiosan alkupiste
	std::string::iterator itsCheckOutTextEndPosition; // sen hetkinen tekstiosan alkupiste
	bool fContinueCurrentSectionCheckOut; // jatketaanko sen hetkisen sectionin tutkimista, vai onko selv‰‰, ettei esim. else-sectionia ole olemassa

	void SetMacroTexts(const std::string &theMacroText);
	std::string itsMacroText;
	std::string itsStrippedMacroText;
	std::string itsIncludeDirectory; // mist‰ ladataan mahd. include filet

	checkedVector<NFmiSmartToolCalculationBlockInfo> itsSmartToolCalculationBlocks;

	int itsMaxCalculationSectionCount;

	static void InitTokens(NFmiProducerSystem *theProducerSystem);
	static bool fTokensInitialized;
	static ParamMap itsTokenParameterNamesAndIds;
	static ProducerMap itsTokenProducerNamesAndIds;
	static ConstantMap itsTokenConstants;
	static checkedVector<std::string> itsTokenConditionalCommands;
	static checkedVector<std::string> itsTokenIfCommands;
	static checkedVector<std::string> itsTokenElseIfCommands;
	static checkedVector<std::string> itsTokenElseCommands;
	static checkedVector<std::string> itsTokenCalculationBlockMarkers;
	static checkedVector<std::string> itsTokenMaskBlockMarkers;
	static checkedVector<std::string> itsTokenRampUpFunctions;
	static checkedVector<std::string> itsTokenRampDownFunctions;
	static checkedVector<std::string> itsTokenDoubleRampFunctions;
	static checkedVector<std::string> itsTokenRampFunctions;
	static checkedVector<std::string> itsTokenMacroParamIdentifiers; // t‰nne listataan result jne. sanat joita k‰ytet‰‰n makrojen visualisoinnissa
	static checkedVector<std::string> itsTokenDeltaZIdentifiers; // t‰nne listataan deltaz 'funktiot'

	typedef std::map<std::string, FmiMaskOperation> MaskOperMap;
	static MaskOperMap itsTokenMaskOperations;
	typedef std::map<std::string, NFmiAreaMask::CalculationOperator> CalcOperMap;
	static CalcOperMap itsCalculationOperations;
	typedef std::map<std::string, NFmiAreaMask::BinaryOperator> BinaOperMap;
	static BinaOperMap itsBinaryOperator;
	static ParamMap itsTokenStaticParameterNamesAndIds;
	static ParamMap itsTokenCalculatedParameterNamesAndIds; // mm. lat, lon ja elevAngle

	typedef std::map<std::string, NFmiAreaMask::FunctionType> FunctionMap;
	static FunctionMap itsTokenFunctions;
	static FunctionMap itsTokenThreeArgumentFunctions;

	typedef std::pair<NFmiAreaMask::FunctionType, int> MetFunctionMapValue; // MetFunktioihin talletetaan 'taika'-sanan lis‰ksi Funktio tyyppi ja funktion argumenttien lukum‰‰r‰.
	typedef std::map<std::string, MetFunctionMapValue> MetFunctionMap; // 'Meteorologiset' funktiot. N‰ill‰ funktioilla k‰sitell‰‰n queryData-olioita eli pyydet‰‰n erilaisia 
																		// arvoja siit‰ (esim. advektiota Adv(T_Hir)).
	static MetFunctionMap itsTokenMetFunctions;

	typedef std::map<std::string, NFmiAreaMask::CalculationOperationType> PeekFunctionMap;
	static PeekFunctionMap itsTokenPeekFunctions;

	typedef std::map<std::string, NFmiAreaMask::MathFunctionType> MathFunctionMap;
	static MathFunctionMap itsMathFunctions;

	typedef std::map<std::string, int> ScriptVariableMap;
	ScriptVariableMap itsTokenScriptVariableNames; // skriptiss‰ varatut muuttujat (var x = ?) talletetaan t‰nne, ett‰ voidaan tarkistaa niiden olemassa olo
	int itsScriptVariableParamIdCounter; // pit‰‰ keksia muutujille id, joten tehd‰‰ juokseva counter
	bool fUseAnyDataTypeBecauseUsingProdID; // jos skripti muuttujan tuottaja annetaan id:n‰ (T_PROD104 eli analyysi), pit‰‰ data tyypiksi laittaa anydata

	// normaali ja macroParam sijoituksia halutaan seurata, ett‰ ei tapahdu vahinkoja eli niit‰ olisi sekaisin, jolloin seuramukset ovat vahingollisia
	bool fNormalAssigmentFound; // loytyykˆ skriptist‰ normaaleja sijoituksia esim. T = ???
	bool fMacroParamFound; // loytyykˆ skriptist‰ ns. macroParameri sijoituksia eli RESULT = ?????
	bool fMacroParamSkriptInProgress; // Tieto siit‰ tulkitaanko macroParam-skripti‰ vai tavallista skripti‰. Poikkeus heitet‰‰n jos macrpParam-skripti p‰‰ll‰, mutta tehd‰‰n tavallinen sijoitus

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


