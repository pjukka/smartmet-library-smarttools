//**********************************************************
// C++ Class Name : NFmiSmartToolIntepreter
// ---------------------------------------------------------
// Filetype: (SOURCE)
// Filepath: G:/siirto/marko/oc/NFmiSmartToolIntepreter.cpp
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
//**********************************************************
#ifdef WIN32
#pragma warning(disable : 4786) // poistaa n kpl VC++ k��nt�j�n varoitusta
#endif

#include "NFmiSmartToolIntepreter.h"
#include "NFmiAreaMaskInfo.h"
#include "NFmiSmartToolCalculationSectionInfo.h"
#include "NFmiAreaMaskSectionInfo.h"
#include "NFmiPreProcessor.h"
#include "NFmiSmartToolCalculationInfo.h"
#include "NFmiValueString.h"
#include "NFmiLevelType.h"
#include "NFmiLevel.h"
#include "NFmiInfoOrganizer.h"
#include "NFmiSmartInfo.h"
#include "NFmiEnumConverter.h"
#include "NFmiDictionaryFunction.h"
#include "NFmiProducerSystem.h"

#include <algorithm>
#include <utility>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <functional>
#include <cctype>

static const int gMesanProdId = 104;

using namespace std;
/*
#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/
// globaali tarkistus luokka etsim��n rivin lopetusta
struct EndOfLineSearcher
{
	template<typename T>
	bool operator()(T theChar)
	{ // tarkistetaan my�s blokin loppu merkki '}' koska blokki-koodi voi olla seuraavanlaista "{T = T + 1}" eli blokin loppu merkki samalla rivill�
	  return(theChar == '\r' || theChar == '\n' || theChar == '}');
	}

};

//HUOM!!! vaarallinen luokka, pit�� muistaa kutsua Clearia, muuten vuotaa!!!
NFmiSmartToolCalculationBlockInfoVector::NFmiSmartToolCalculationBlockInfoVector(void)
:itsCalculationBlockInfos()
{
}

NFmiSmartToolCalculationBlockInfoVector::~NFmiSmartToolCalculationBlockInfoVector(void)
{ // Huom! ei kutsu Clear:ia!!!
}

void NFmiSmartToolCalculationBlockInfoVector::Clear(void)
{
	Iterator it = Begin();
	Iterator endIt = End();
	for( ; it != endIt; ++it)
	{
		(*it)->Clear();
	}
	std::for_each(itsCalculationBlockInfos.begin(), itsCalculationBlockInfos.end(), PointerDestroyer());
	itsCalculationBlockInfos.clear();
}

// Ottaa pointterin 'omistukseensa' eli pit�� luoda ulkona new:ll� ja antaa t�nne
void NFmiSmartToolCalculationBlockInfoVector::Add(NFmiSmartToolCalculationBlockInfo* theBlockInfo)
{
	itsCalculationBlockInfos.push_back(theBlockInfo);
}

void NFmiSmartToolCalculationBlockInfoVector::AddModifiedParams(std::set<int> &theModifiedParams)
{
	Iterator it = Begin();
	Iterator endIt = End();
	for( ; it != endIt; ++it)
	{
		(*it)->AddModifiedParams(theModifiedParams);
	}
}

//HUOM!!! vaarallinen luokka, pit�� muistaa kutsua Clearia, muuten vuotaa!!!
NFmiSmartToolCalculationBlockInfo::NFmiSmartToolCalculationBlockInfo(void)
:itsFirstCalculationSectionInfo(new NFmiSmartToolCalculationSectionInfo)
,itsIfAreaMaskSectionInfo(new NFmiAreaMaskSectionInfo)
,itsIfCalculationBlockInfos(0)
,itsElseIfAreaMaskSectionInfo(new NFmiAreaMaskSectionInfo)
,itsElseIfCalculationBlockInfos(0)
,itsElseCalculationBlockInfos(0)
,itsLastCalculationSectionInfo(new NFmiSmartToolCalculationSectionInfo)
{
}

NFmiSmartToolCalculationBlockInfo::~NFmiSmartToolCalculationBlockInfo(void)
{
}

// �l� kutsu Clear:ia destruktorissa!!!!
void NFmiSmartToolCalculationBlockInfo::Clear(void)
{
	delete itsFirstCalculationSectionInfo;
	itsFirstCalculationSectionInfo = 0;
	delete itsLastCalculationSectionInfo;
	itsLastCalculationSectionInfo = 0;
	delete itsIfAreaMaskSectionInfo;
	itsIfAreaMaskSectionInfo = 0;
	if(itsIfCalculationBlockInfos)
	{
		itsIfCalculationBlockInfos->Clear();
		delete itsIfCalculationBlockInfos;
		itsIfCalculationBlockInfos = 0;
	}
	delete itsElseIfAreaMaskSectionInfo;
	itsElseIfAreaMaskSectionInfo = 0;
	if(itsElseIfCalculationBlockInfos)
	{
		itsElseIfCalculationBlockInfos->Clear();
		delete itsElseIfCalculationBlockInfos;
		itsElseIfCalculationBlockInfos = 0;
	}
	if(itsElseCalculationBlockInfos)
	{
		itsElseCalculationBlockInfos->Clear();
		delete itsElseCalculationBlockInfos;
		itsElseCalculationBlockInfos = 0;
	}
	fElseSectionExist = false;
}

// Lis�t��n set:iin kaikki parametrit, joita t�ss� calculationblokissa
// voidaan muokata. Talteen otetaan vain identti, koska muu ei
// kiinnosta (ainakaan nyt).
void NFmiSmartToolCalculationBlockInfo::AddModifiedParams(std::set<int> &theModifiedParams)
{
	if(itsFirstCalculationSectionInfo) // eka section
		itsFirstCalculationSectionInfo->AddModifiedParams(theModifiedParams);
	if(itsIfCalculationBlockInfos) // if section
		itsIfCalculationBlockInfos->AddModifiedParams(theModifiedParams);
	if(itsElseIfCalculationBlockInfos) // elseif section
		itsElseIfCalculationBlockInfos->AddModifiedParams(theModifiedParams);
	if(itsElseCalculationBlockInfos) // else section
		itsElseCalculationBlockInfos->AddModifiedParams(theModifiedParams);
	if(itsLastCalculationSectionInfo) // vika section
		itsLastCalculationSectionInfo->AddModifiedParams(theModifiedParams);
}

// Luokan staattisten dataosien pakollinen alustus.
bool NFmiSmartToolIntepreter::fTokensInitialized = false;
NFmiSmartToolIntepreter::ParamMap NFmiSmartToolIntepreter::itsTokenParameterNamesAndIds;
NFmiSmartToolIntepreter::ProducerMap NFmiSmartToolIntepreter::itsTokenProducerNamesAndIds;
NFmiSmartToolIntepreter::ConstantMap NFmiSmartToolIntepreter::itsTokenConstants;
checkedVector<std::string> NFmiSmartToolIntepreter::itsTokenConditionalCommands;
checkedVector<std::string> NFmiSmartToolIntepreter::itsTokenIfCommands;
checkedVector<std::string> NFmiSmartToolIntepreter::itsTokenElseIfCommands;
checkedVector<std::string> NFmiSmartToolIntepreter::itsTokenElseCommands;
checkedVector<std::string> NFmiSmartToolIntepreter::itsTokenCalculationBlockMarkers;
checkedVector<std::string> NFmiSmartToolIntepreter::itsTokenMaskBlockMarkers;
//checkedVector<std::string> NFmiSmartToolIntepreter::itsTokenFunctions;
checkedVector<std::string> NFmiSmartToolIntepreter::itsTokenRampUpFunctions;
checkedVector<std::string> NFmiSmartToolIntepreter::itsTokenRampDownFunctions;
checkedVector<std::string> NFmiSmartToolIntepreter::itsTokenDoubleRampFunctions;
checkedVector<std::string> NFmiSmartToolIntepreter::itsTokenRampFunctions;
checkedVector<std::string> NFmiSmartToolIntepreter::itsTokenMacroParamIdentifiers;
checkedVector<std::string> NFmiSmartToolIntepreter::itsTokenDeltaZIdentifiers;

NFmiSmartToolIntepreter::MaskOperMap NFmiSmartToolIntepreter::itsTokenMaskOperations;
NFmiSmartToolIntepreter::CalcOperMap NFmiSmartToolIntepreter::itsCalculationOperations;
NFmiSmartToolIntepreter::BinaOperMap NFmiSmartToolIntepreter::itsBinaryOperator;
NFmiSmartToolIntepreter::ParamMap NFmiSmartToolIntepreter::itsTokenStaticParameterNamesAndIds;
NFmiSmartToolIntepreter::ParamMap NFmiSmartToolIntepreter::itsTokenCalculatedParameterNamesAndIds;
NFmiSmartToolIntepreter::LevelMap NFmiSmartToolIntepreter::itsTokenLevelNamesIdentsAndValues;
NFmiSmartToolIntepreter::SoundingIndexMap NFmiSmartToolIntepreter::itsTokenSoundingIndexFunctions;
NFmiSmartToolIntepreter::FunctionMap NFmiSmartToolIntepreter::itsTokenFunctions;
NFmiSmartToolIntepreter::FunctionMap NFmiSmartToolIntepreter::itsTokenThreeArgumentFunctions;
NFmiSmartToolIntepreter::PeekFunctionMap NFmiSmartToolIntepreter::itsTokenPeekFunctions;
NFmiSmartToolIntepreter::MathFunctionMap NFmiSmartToolIntepreter::itsMathFunctions;

//--------------------------------------------------------
// Constructor/Destructor
//--------------------------------------------------------
NFmiSmartToolIntepreter::NFmiSmartToolIntepreter(NFmiInfoOrganizer* theInfoOrganizer, NFmiProducerSystem *theProducerSystem)
:itsInfoOrganizer(theInfoOrganizer)
,itsProducerSystem(theProducerSystem)
,itsSmartToolCalculationBlocks()
,fNormalAssigmentFound(false)
,fMacroParamFound(false)
,fMacroParamSkriptInProgress(false)
{
	NFmiSmartToolIntepreter::InitTokens(itsProducerSystem);
}
NFmiSmartToolIntepreter::~NFmiSmartToolIntepreter(void)
{
	Clear();
}
//--------------------------------------------------------
// Interpret
//--------------------------------------------------------
// Tulkitsee annetun macro-tekstin. Erottelee eri calculationSectionit, mahdolliset ehto rakenteet ja niiden maskit ja rakentaa sen
// mukaiset maski ja calculation infot, ett� SmartToolModifier osaa rakentaa oikeat systeemit (areamaskit ja lasku-oliot).
// Jos macrossa virhe, heitt�� poikkeuksen.
// fThisIsMacroParamSkript -parametrin avulla voidaan tarkistaa, ettei ajeta tavallista sijoitusta macroParamin
// RESULT = ... sijoitusten sijaan. Tulkin pit�� olla tietoinen 'moodista' ett� voi heitt�� poikkeuksen
// huomatessaan t�ll�isen tilanteen.
void NFmiSmartToolIntepreter::Interpret(const std::string &theMacroText, bool fThisIsMacroParamSkript)
{
	fMacroParamSkriptInProgress = fThisIsMacroParamSkript;
	Clear();
	itsTokenScriptVariableNames.clear(); // tyhjennetaan aluksi kaikki skripti muuttujat
	itsScriptVariableParamIdCounter = 1000000; // alustetaan isoksi, ettei mene p��llekk�in todellisten param id::::ien kanssa
//	InitSectionInfos();
	SetMacroTexts(theMacroText);
	InitCheckOut();

	// nollataan tulkinnan aluksi, mink� tyyppisi� parametri sijoituksia sijoituksia l�ytyy
	fNormalAssigmentFound = false;
	fMacroParamFound = false;

	bool fGoOn = true;
	int index = 0;
	while(fGoOn)
	{
		index++;
		NFmiSmartToolCalculationBlockInfo block;
		try
		{
			if(index > 500)
				throw runtime_error(::GetDictionaryString("SmartToolErrorTooManyBlocks"));
			fGoOn = CheckoutPossibleNextCalculationBlock(&block, true);
			itsSmartToolCalculationBlocks.push_back(block);
			if(*itsCheckOutTextStartPosition == '}') // jos ollaan blokin loppu merkiss�, siirryt��n sen yli ja jatketaan seuraavalle kierrokselle
				++itsCheckOutTextStartPosition;
		}
		catch(exception & /* e */ )
		{
			block.Clear();
			for(unsigned int i=0; i<itsSmartToolCalculationBlocks.size(); i++)
				itsSmartToolCalculationBlocks[i].Clear();
			itsSmartToolCalculationBlocks.clear();
			throw ;
		}
	}
}

// kun joku skripti on tulkittu Interpret-metodissa, on tuotettu
// laskenta-lausekkeet itsSmartToolCalculationBlocks, joista t�m�
// metodi k�y katsomassa, mit� parametreja ollaan mahd. muokkaamassa
// eli T = ? jne. tarkoittaa ett� l�mp�tilaa ollaan ainakin
// mahdollisesti muokkaamassa.
NFmiParamBag NFmiSmartToolIntepreter::ModifiedParams(void)
{
	std::set<int> modifiedParams;
	checkedVector<NFmiSmartToolCalculationBlockInfo>::size_type ssize = itsSmartToolCalculationBlocks.size();
	checkedVector<NFmiSmartToolCalculationBlockInfo>::size_type i = 0;
	for( ;i<ssize; i++)
	{
		itsSmartToolCalculationBlocks[i].AddModifiedParams(modifiedParams);
	}
	NFmiEnumConverter converter;
	NFmiParamBag params;
	std::set<int>::iterator it = modifiedParams.begin();
	std::set<int>::iterator endIt = modifiedParams.end();
	for( ; it != endIt; ++it)
	{
		params.Add(NFmiDataIdent(NFmiParam(*it, converter.ToString(*it))));
	}
	params.SetActivities(true);
	return params;
}

bool NFmiSmartToolIntepreter::CheckoutPossibleNextCalculationBlockVector(NFmiSmartToolCalculationBlockInfoVector* theBlockVector)
{
	bool fBlockFound = false;
	int safetyIndex = 0;
	NFmiSmartToolCalculationBlockInfo *block = new NFmiSmartToolCalculationBlockInfo;
//	auto_ptr<NFmiSmartToolCalculationBlockInfo> blockPtr(block); // tuhoaa poikkeusten tapauksessa dynaamisen datan
	try
	{
		for( ; fBlockFound = CheckoutPossibleNextCalculationBlock(block, false, safetyIndex); safetyIndex++)
		{
			theBlockVector->Add(block);
			block = 0;
			if(safetyIndex > 500)
				throw runtime_error(::GetDictionaryString("SmartToolErrorTooManyBlocks"));

			if(*itsCheckOutTextStartPosition == '}') // jos ollaan loppu merkiss�, siirryt��n sen yli ja jatketaan seuraavalle kierrokselle
			{
				++itsCheckOutTextStartPosition;
				break; // lopetetaan blokki vektorin luku t�h�n kun loppu merkki tuli vastaan
			}
			block = new NFmiSmartToolCalculationBlockInfo;
		}

		if(!fBlockFound)
			if(block)
			{
				block->Clear();
				delete block;
			}
	}
	catch(...)
	{
		if(block)
		{
			block->Clear();
			delete block;
		}
		throw ;
	}
	return !theBlockVector->Empty();
}

// paluu arvo tarkoittaa, jatketaanko tekstin l�pik�ymist� viel�, vai ollaanko tultu jo loppuun.
bool NFmiSmartToolIntepreter::CheckoutPossibleNextCalculationBlock(NFmiSmartToolCalculationBlockInfo* theBlock, bool fFirstLevelCheckout, int theBlockIndex)
{
	bool fWasBlockMarksFound = false;
	CheckoutPossibleNextCalculationSection(theBlock->itsFirstCalculationSectionInfo, fWasBlockMarksFound);
	if(fFirstLevelCheckout || (fWasBlockMarksFound && theBlockIndex == 0) || theBlockIndex > 0) // vain 1. tason kyselyss� jatketaan tai jos blokki merkit l�ytyiv�t {}
		// eli IF()-lauseen j�lkeen pit�� olla {}-blokki muuten ei oteta kuin 1. calc-sektio kun ollaan extraktoimassa if, else if tai else -blokkia
		// tai jos useita blokkeja if-lauseen sis�ll�, jatketaan my�s
	{
		if(CheckoutPossibleIfClauseSection(theBlock->itsIfAreaMaskSectionInfo))
		{
			// blokit voidaan luoda  vasta t��ll� eik� konstruktorissa, koska muuten konstruktori joutuisi iki-looppiin
			theBlock->itsIfCalculationBlockInfos = new NFmiSmartToolCalculationBlockInfoVector;
			CheckoutPossibleNextCalculationBlockVector(theBlock->itsIfCalculationBlockInfos);
			if(CheckoutPossibleElseIfClauseSection(theBlock->itsElseIfAreaMaskSectionInfo))
			{
				theBlock->itsElseIfCalculationBlockInfos = new NFmiSmartToolCalculationBlockInfoVector;
				CheckoutPossibleNextCalculationBlockVector(theBlock->itsElseIfCalculationBlockInfos);
			}
			if((theBlock->fElseSectionExist = CheckoutPossibleElseClauseSection()) == true)
			{
				theBlock->itsElseCalculationBlockInfos = new NFmiSmartToolCalculationBlockInfoVector;
				CheckoutPossibleNextCalculationBlockVector(theBlock->itsElseCalculationBlockInfos);
			}
		}
		if(!fWasBlockMarksFound) // jos 1. checkoutiss ei t�rm�tty blokin alkumerkkiin '{' voidaan kokeilla l�ytyyk� t�st� lasku-sektiota
		CheckoutPossibleNextCalculationSection(theBlock->itsLastCalculationSectionInfo, fWasBlockMarksFound);
	}
	if(itsCheckOutTextStartPosition == itsStrippedMacroText.end())
		return false;
	return true;
}

void NFmiSmartToolIntepreter::InitCheckOut(void)
{
	itsCheckOutSectionText = "";
	itsCheckOutTextStartPosition = itsStrippedMacroText.begin();
	itsCheckOutTextEndPosition = itsStrippedMacroText.begin(); // t�ll� ei viel� v�li�
	fContinueCurrentSectionCheckOut = true;
}

#ifdef UNIX
static std::string::iterator EatWhiteSpaces(std::string::iterator it)
{
  for(; isspace(*it) ; ++it)  ;
	return it;
}
#else
static std::string::iterator EatWhiteSpaces(std::string::iterator it)
{
  for(; ::isspace(*it) ; ++it)  ;
	return it;
}
#endif

// Irroitetaan mahdollisia laskuoperaatio rivej� tulkkausta varten.
// laskun pit�� olla yhdell� rivill� ja ne ovat muotoa:
// T = T + 1
// Eli pit�� olla joku sijoitus johonkin parametriin.
// Palauttaa true jos l�ytyi jotai lasku-operaatioita.
// P�ivitt�� my�s luokan sis�isi� iteraattoreita macro-tekstiin.
bool NFmiSmartToolIntepreter::ExtractPossibleNextCalculationSection(bool &fWasBlockMarksFound)
{
	fWasBlockMarksFound = false;
	itsCheckOutSectionText = "";
	string nextLine;
//	std::string::iterator eolPos = itsStrippedMacroText.begin();
	std::string::iterator eolPos = itsCheckOutTextStartPosition;
	eolPos = EatWhiteSpaces(eolPos);
	itsCheckOutTextStartPosition = itsCheckOutTextEndPosition = eolPos;
	if(*eolPos == '{') // jos blokin alkumerkki l�ytyi, haetaan sen loppua
	{
		eolPos = std::find(itsCheckOutTextStartPosition, itsStrippedMacroText.end(), '}');
		if(eolPos == itsStrippedMacroText.end())
			throw runtime_error(::GetDictionaryString("SmartToolErrorEndOfBlockNotFound"));
		else
		{
			fWasBlockMarksFound = true;
			++itsCheckOutTextStartPosition; // hyp�t��n alkumerkin ohi
			eolPos = itsCheckOutTextStartPosition;
//			itsCheckOutSectionText = string(itsCheckOutTextStartPosition, eolPos);
//			++eolPos; // hyp�t��n blokin loppumerkin ohi
//			itsCheckOutTextStartPosition = itsCheckOutTextEndPosition = eolPos;
		}
	}
//	else // muuten etsit��n laskurivej� rivi kerrallaan
	{
		do
		{
			eolPos = EatWhiteSpaces(eolPos);
			// en ole varma viel� n�ist� iteraattoreista, mitk� ovat tarpeellisisa ja mitk� turhia.
			itsCheckOutTextStartPosition = itsCheckOutTextEndPosition = eolPos;
			itsCheckOutSectionText += nextLine;
			if(*eolPos == '{' || *eolPos == '}') // jos seuraavan blokin alkumerkki tai loppumerkki l�ytyi, lopetetaan t�m� blokki t�h�n
			{
				if(*eolPos == '{') // jos l�ytyy alkumerkki, ilmoitetaan siit� ulos t��lt�
					fWasBlockMarksFound = true;
//				if(*eolPos == '}') // jos ollaan loppu merkiss�, siirryt��n sen yli ja jatketaan seuraavalle kierrokselle
//					++itsCheckOutTextStartPosition;
				if(itsCheckOutSectionText.empty())
					return false;
				else
					return true;
			}

//			eolPos = std::find(itsCheckOutTextStartPosition, itsStrippedMacroText.end(), '\n');
			eolPos = std::find_if(itsCheckOutTextStartPosition, itsStrippedMacroText.end(), EndOfLineSearcher());

			nextLine = string(itsCheckOutTextStartPosition, eolPos);
			nextLine += '\n';
			if(*eolPos == '\n' || *eolPos == '\r')
				++eolPos;
		}while(IsPossibleCalculationLine(nextLine));
	}
	if(itsCheckOutSectionText.empty())
		return false;
	return true;
}

// aluksi yksinkertainen laskurivi tarkistus:
// 1. Ei saa olla ehto-lause sanoja (IF,ELSEIF jne.).
// 2. Pit�� olla sijoitus-operaatio eli '='
bool NFmiSmartToolIntepreter::IsPossibleCalculationLine(const std::string &theTextLine)
{
	if(FindAnyFromText(theTextLine, itsTokenConditionalCommands))
			return false;
	if(theTextLine.find(string("=")) != string::npos)
		return true;

	if(std::find_if(theTextLine.begin(), theTextLine.end(), std::not1(std::ptr_fun(::isspace))) != theTextLine.end())
		throw runtime_error(::GetDictionaryString("SmartToolErrorIllegalTextFound") + ": \n" + theTextLine);
	return false;
}

// Yksinkertainen IF(mask) -rivin tarkistus:
// 1. aluksi pit�� olla IF
// 2. ei saa olla ELSE/ELSEIF sanoja
// 3. Pit�� olla ensin '('- ja sitten ')' -merkit
bool NFmiSmartToolIntepreter::IsPossibleIfConditionLine(const std::string &theTextLine)
{
	if(!FindAnyFromText(theTextLine, itsTokenIfCommands))
			return false;
	if(FindAnyFromText(theTextLine, itsTokenElseIfCommands))
			return false;
	if(FindAnyFromText(theTextLine, itsTokenElseCommands))
			return false;
	if((theTextLine.find(string("(")) != string::npos) && (theTextLine.find(string(")")) != string::npos))
		return true;
	return false;
}

// Yksinkertainen ELSEIF(mask) -rivin tarkistus:
// 1. aluksi pit�� olla ELSEIF
// 2. ei saa olla IF/ELSE sanoja
// 3. Pit�� olla ensin '('- ja sitten ')' -merkit
bool NFmiSmartToolIntepreter::IsPossibleElseIfConditionLine(const std::string &theTextLine)
{
	if(!FindAnyFromText(theTextLine, itsTokenElseIfCommands))
			return false;
	if((theTextLine.find(string("(")) != string::npos) && (theTextLine.find(string(")")) != string::npos))
		return true;
	return false;
}

// Yksinkertainen ELSE -rivin tarkistus:
// 1. saa olla vain else-sana
bool NFmiSmartToolIntepreter::IsPossibleElseConditionLine(const std::string &theTextLine)
{
	stringstream sstream(theTextLine);
	string tmp;
	sstream >> tmp;
	if(!FindAnyFromText(tmp, itsTokenElseCommands))
			return false;
	tmp = ""; // nollataan t�m�, koska MSVC++7.1 ei sijoita jostain syyst� mit��n kun ollaan tultu loppuun (muilla k��nt�jill� on sijoitettu tyhj� tmp-stringiin)
	sstream >> tmp;
	if(tmp.empty())
		return true;
	else
		throw runtime_error(::GetDictionaryString("SmartToolErrorIllegalElseLine") + ": \n" + theTextLine);
	return false;
}

static bool IsWordContinuing(char ch)
{
	if(isalnum(ch) || ch == '_')
		return true;
	return false;
}

// Pit�� olla kokonainen sana eli juuri ennen sanaa ei saa olla kirjaimia,numeroita tai _-merkki�, eik� heti sen j�lkeenk��n.
bool NFmiSmartToolIntepreter::FindAnyFromText(const std::string &theText, const checkedVector<std::string>& theSearchedItems)
{
	int size = theSearchedItems.size();
	for(int i = 0; i < size; i++)
	{
		string::size_type pos = string::npos;
		if((pos = theText.find(theSearchedItems[i])) != string::npos)
		{
			if(pos > 0)
			{
				char ch1 = theText[pos-1];
				if(IsWordContinuing(ch1))
					continue;
			}
			if(pos + theSearchedItems[i].size() < theText.size())
			{
				char ch2 = theText[pos + theSearchedItems[i].size()];
				if(IsWordContinuing(ch2))
					continue;
			}
			return true;
		}
	}
	return false;
}

// palauttaa true, jos if-lause l�ytyi
bool NFmiSmartToolIntepreter::CheckoutPossibleIfClauseSection(NFmiAreaMaskSectionInfo* theAreaMaskSectionInfo)
{
	if(ExtractPossibleIfClauseSection())
		return InterpretMaskSection(itsCheckOutSectionText, theAreaMaskSectionInfo);
	return false;
}


// IF-lause koostuu sanasta IF ja suluista, jotka sulkevat v�hint��n yhden mahd. useita ehtoja
// joita yhdistet��n erilaisilla bin��ri operaatioilla. Koko jutun pit�� olla yhdell� rivill�.
// Esim:
// IF(T<1)
// IF(T<1 && P>1012)
// IF(T<1 && P>1012 || RH>=95)
bool NFmiSmartToolIntepreter::ExtractPossibleIfClauseSection(void)
{
	itsCheckOutSectionText = "";
	std::string::iterator eolPos = itsCheckOutTextStartPosition;
	eolPos = EatWhiteSpaces(eolPos);
	// en ole varma viel� n�ist� iteraattoreista, mitk� ovat tarpeellisisa ja mitk� turhia.
	itsCheckOutTextStartPosition = itsCheckOutTextEndPosition = eolPos;

	// HUOM!!! NT4 ja W2000 ero:
	// Dialogista tulevasta stringiss� rivin vaihto on W2000:ssa 13 (=CR) ja NT4:ssa 10 (=LF)
	// Pit�� Testata molemmat tapaukset!!!!!!
	eolPos = std::find_if(itsCheckOutTextStartPosition, itsStrippedMacroText.end(), EndOfLineSearcher()); // W2000:ssa etsit��n CR:��
	string nextLine = string(itsCheckOutTextStartPosition, eolPos);
	if(IsPossibleIfConditionLine(nextLine))
	{
		itsCheckOutSectionText += nextLine;
		itsCheckOutTextStartPosition = eolPos; // asetetaan positio seuraavia tarkasteluja varten
		return true;
	}
	return false;
}

// En ymm�rr� miten t�m�n saa toimimaan, miten saa k�ytetty� mem_fun-adapteria.
// VC:ssa n�ytt�� olevan taas oma systeemi ja nyt mem_fun ei toimi
// standardin mukaisesti. En saa kutsua toimiaan.
template<typename memfunction>
bool NFmiSmartToolIntepreter::ExtractPossibleConditionalClauseSection(memfunction conditionalChecker)
{
	itsCheckOutSectionText = "";
	std::string::iterator eolPos = itsCheckOutTextStartPosition;
	eolPos = EatWhiteSpaces(eolPos);
	// en ole varma viel� n�ist� iteraattoreista, mitk� ovat tarpeellisisa ja mitk� turhia.
	itsCheckOutTextStartPosition = itsCheckOutTextEndPosition = eolPos;

	// HUOM!!! NT4 ja W2000 ero:
	// Dialogista tulevasta stringiss� rivin vaihto on W2000:ssa 13 (=CR) ja NT4:ssa 10 (=LF)
	// Pit�� Testata molemmat tapaukset!!!!!!
	eolPos = std::find_if(itsCheckOutTextStartPosition, itsStrippedMacroText.end(), EndOfLineSearcher()); // W2000:ssa etsit��n CR:��
	string nextLine = string(itsCheckOutTextStartPosition, eolPos);
	if(conditionalChecker(nextLine))
	{
		itsCheckOutSectionText += nextLine;
		itsCheckOutTextStartPosition = eolPos; // asetetaan positio seuraavia tarkasteluja varten
		return true;
	}
	return false;
}

// En saanut toimimaan mem_fun adapteri-systeemi�, joten joudun kopioimaan koodia
// ExtractPossibleIfClauseSection-metodista. Olisin halunnut k�ytt�� yleist�
// funktiota, jolle annetaan parametrina yksi erottava metodi.
// palauttaa true, jos ifelse-lause l�ytyi
bool NFmiSmartToolIntepreter::CheckoutPossibleElseIfClauseSection(NFmiAreaMaskSectionInfo* theAreaMaskSectionInfo)
{
	if(ExtractPossibleElseIfClauseSection())
		return InterpretMaskSection(itsCheckOutSectionText, theAreaMaskSectionInfo);
	return false;
}

bool NFmiSmartToolIntepreter::ExtractPossibleElseIfClauseSection(void)
{
	itsCheckOutSectionText = "";
	std::string::iterator eolPos = itsCheckOutTextStartPosition;
	eolPos = EatWhiteSpaces(eolPos);
	// en ole varma viel� n�ist� iteraattoreista, mitk� ovat tarpeellisisa ja mitk� turhia.
	itsCheckOutTextStartPosition = itsCheckOutTextEndPosition = eolPos;

	// HUOM!!! NT4 ja W2000 ero:
	// Dialogista tulevasta stringiss� rivin vaihto on W2000:ssa 13 (=CR) ja NT4:ssa 10 (=LF)
	// Pit�� Testata molemmat tapaukset!!!!!!
	eolPos = std::find_if(itsCheckOutTextStartPosition, itsStrippedMacroText.end(), EndOfLineSearcher()); // W2000:ssa etsit��n CR:��
	string nextLine = string(itsCheckOutTextStartPosition, eolPos);
	if(IsPossibleElseIfConditionLine(nextLine))
	{
		itsCheckOutSectionText += nextLine;
		itsCheckOutTextStartPosition = eolPos; // asetetaan positio seuraavia tarkasteluja varten
		return true;
	}
	return false;
	// Allaolevaa kutsua en saanut toimimaan, VC:n mem_fun declaraatio outo??
//	return ExtractPossibleConditionalClauseSection(std::mem_fun(&NFmiSmartToolIntepreter::IsPossibleElseIfConditionLine));
}

bool NFmiSmartToolIntepreter::CheckoutPossibleElseClauseSection(void)
{
	itsCheckOutSectionText = "";
	std::string::iterator eolPos = itsCheckOutTextStartPosition;
	eolPos = EatWhiteSpaces(eolPos);
	// en ole varma viel� n�ist� iteraattoreista, mitk� ovat tarpeellisisa ja mitk� turhia.
	itsCheckOutTextStartPosition = itsCheckOutTextEndPosition = eolPos;

	// HUOM!!! NT4 ja W2000 ero:
	// Dialogista tulevasta stringiss� rivin vaihto on W2000:ssa 13 (=CR) ja NT4:ssa 10 (=LF)
	// Pit�� Testata molemmat tapaukset!!!!!!
	eolPos = std::find_if(itsCheckOutTextStartPosition, itsStrippedMacroText.end(), EndOfLineSearcher()); // W2000:ssa etsit��n CR:��
	string nextLine = string(itsCheckOutTextStartPosition, eolPos);
	if(IsPossibleElseConditionLine(nextLine))
	{
		itsCheckOutSectionText += nextLine;
		itsCheckOutTextStartPosition = eolPos; // asetetaan positio seuraavia tarkasteluja varten
		return true;
	}
	return false;
}

bool NFmiSmartToolIntepreter::CheckoutPossibleNextCalculationSection(NFmiSmartToolCalculationSectionInfo* theSectionInfo, bool &fWasBlockMarksFound)
{
	if(ExtractPossibleNextCalculationSection(fWasBlockMarksFound))
		return InterpretCalculationSection(itsCheckOutSectionText, theSectionInfo);
	return false;
}

void NFmiSmartToolIntepreter::SetMacroTexts(const std::string &theMacroText)
{
	itsMacroText = theMacroText;
	NFmiPreProcessor commentStripper;
	commentStripper.SetIncluding("#include", itsIncludeDirectory);
	commentStripper.SetString(theMacroText);
	if(commentStripper.Strip())
	{
		itsStrippedMacroText = commentStripper.GetString();
	}
	else
	{
		throw runtime_error(::GetDictionaryString("SmartToolErrorCommentRemovalFailed") + ":\n" + commentStripper.GetMessage());
	}
}
//--------------------------------------------------------
// InterpretMaskSection
//--------------------------------------------------------
// Koko Section on yhdell� rivill�!!!
// Esim:
// IF(T<1)
// IF(T<1 && P>1012)
// IF(T<1 && P>1012 || RH>=95)
bool NFmiSmartToolIntepreter::InterpretMaskSection(const std::string &theMaskSectorText, NFmiAreaMaskSectionInfo* theAreaMaskSectionInfo)
{
	theAreaMaskSectionInfo->SetCalculationText(theMaskSectorText);
	string maskText(theMaskSectorText);
	exp_ptr = maskText.begin();
	exp_end = maskText.end();

	string tmp;
	if(GetToken()) // luetaan komento t�h�n esim. if, else jne
	{
		tmp = token;
		if(FindAnyFromText(tmp, itsTokenConditionalCommands))
		{
			if(GetToken()) // luetaan komento t�h�n esim. if, else jne
			{
				tmp = token;
				if(tmp == "(") // pit�� l�yty� alku sulku
				{
					string::size_type startPos = exp_ptr - maskText.begin();
					string::size_type endPos = maskText.rfind(string(")"));
					string::iterator it1 = maskText.begin() + startPos;
					string::iterator it2 = maskText.begin() + endPos;
					if(endPos != string::npos) // pit�� l�yty� lopetus sulku
					{
						string finalText(it1, it2);
						return InterpretMasks(finalText, theAreaMaskSectionInfo);
					}
				}
			}
		}
	}
	throw runtime_error(::GetDictionaryString("SmartToolErrorIllegalConditional") + ":\n" + maskText);
}

// t�ss� on en�� ehtolauseen sulkujen sis�lt�v� oleva teksti esim.
// T<1
// T<1 && P>1012
// T<1 && P>1012 || RH>=95
// HUOM!!!! T�m� vuotaa exceptionin yhteydess�, pino ei tuhoa AreaMaskInfoja!!!!! (korjaa)
bool NFmiSmartToolIntepreter::InterpretMasks(std::string &theMaskSectionText, NFmiAreaMaskSectionInfo* theAreaMaskSectionInfo)
{
	string maskText(theMaskSectionText);
	exp_ptr = maskText.begin();
	exp_end = maskText.end();

	string tmp;
	for(; GetToken(); )
	{
		tmp = token; // luetaan muuttuja/vakio/funktio tai mik� lie
		NFmiAreaMaskInfo *maskInfo = new NFmiAreaMaskInfo;
		auto_ptr<NFmiAreaMaskInfo> maskInfoPtr(maskInfo);
		InterpretToken(tmp, maskInfo); // ei saa antaa auto_ptr-otusta t�ss�, muuten se menett�� omistuksen!
		theAreaMaskSectionInfo->Add(maskInfoPtr.release()); // auto_ptr menett�� omistuksen t�ss�
	}

	// minimiss��n erilaisia lasku elementtej� pit�� olla vahint�in 3 (esim. T > 15)
	if(theAreaMaskSectionInfo->GetAreaMaskInfoVector()->size() >= 3)
		return true;
	throw runtime_error(::GetDictionaryString("SmartToolErrorConditionalWasNotComplete") + ":\n" + theMaskSectionText);
}

NFmiAreaMaskInfo* NFmiSmartToolIntepreter::CreateWantedAreaMaskInfo(const std::string &theMaskSectionText, queue<NFmiAreaMaskInfo *> &theMaskQueue)
{
	NFmiAreaMaskInfo *maskInfo1 = theMaskQueue.front(); // t�ss� pit�� olla muuttuja esim T, p, jne.
	auto_ptr<NFmiAreaMaskInfo> maskInfoPtr1(maskInfo1); // tuhoaa automaattisesti maskInfon
	theMaskQueue.pop();
	NFmiAreaMaskInfo *maskInfo2 = theMaskQueue.front(); // t�ss� pit�� olla vertailu operator esim. >, < jne.
	auto_ptr<NFmiAreaMaskInfo> maskInfoPtr2(maskInfo2); // tuhoaa automaattisesti maskInfon
	theMaskQueue.pop();
	NFmiAreaMaskInfo *maskInfo3 = theMaskQueue.front(); // t�ss� pit�� olla vakio esim. 2.5
	auto_ptr<NFmiAreaMaskInfo> maskInfoPtr3(maskInfo3); // tuhoaa automaattisesti maskInfon
	// ************************************************************************
	// HUOM!! t�m� pit�� muuttaa niin, ett� voisi olla vaikka seuraavia maskeja
	// (T < DP), (T > T_850 - 15), (T/2.2 > T_850) jne.
	// ************************************************************************
	theMaskQueue.pop();
	if(maskInfo1->GetOperationType() == NFmiAreaMask::InfoMask || maskInfo1->GetOperationType() == NFmiAreaMask::CalculationMask)
	{
		if(maskInfo2->GetOperationType() == NFmiAreaMask::Comparison)
		{
			if(maskInfo3->GetOperationType() == NFmiAreaMask::Constant)
			{
				NFmiAreaMaskInfo *finalMaskInfo = new NFmiAreaMaskInfo;
				if(maskInfo1->GetOperationType() == NFmiAreaMask::InfoMask)
					finalMaskInfo->SetOperationType(NFmiAreaMask::InfoMask);
				else
					finalMaskInfo->SetOperationType(NFmiAreaMask::CalculationMask);
				finalMaskInfo->SetDataIdent(maskInfo1->GetDataIdent());
				finalMaskInfo->SetUseDefaultProducer(maskInfo1->GetUseDefaultProducer());
				NFmiCalculationCondition maskCondition(maskInfo2->GetMaskCondition().Condition(), maskInfo3->GetMaskCondition().LowerLimit());
				finalMaskInfo->SetMaskCondition(maskCondition);
				return finalMaskInfo;
			}
		}
	}
	throw runtime_error(::GetDictionaryString("SmartToolErrorConditionalWasIllegal") + ":\n" + theMaskSectionText);
}

//--------------------------------------------------------
// InterpretCalculationSection
//--------------------------------------------------------
// Jokainen rivi tekstiss� on mahdollinen laskuoperaatio esim.
// T = T + 1
// Jokaiselta rivilt� pit�� siis l�ty� muuttuja johon sijoitetaan ja jotain laskuja
// palauttaa true, jos l�ytyi laskuja ja false jos ei.
bool NFmiSmartToolIntepreter::InterpretCalculationSection(std::string &theCalculationSectiontext, NFmiSmartToolCalculationSectionInfo* theSectionInfo)
{
	std::string::iterator pos = theCalculationSectiontext.begin();
	std::string::iterator end = theCalculationSectiontext.end();
	NFmiSmartToolCalculationInfo *calculationInfo = 0;
	do
	{
		calculationInfo = 0; // pit�� nollata joka kierroksella!
		string nextLine = ExtractNextLine(theCalculationSectiontext, pos, &end);
		if(!nextLine.empty() && !ConsistOnlyWhiteSpaces(nextLine))
			calculationInfo = InterpretCalculationLine(nextLine);
		if(calculationInfo)
			theSectionInfo->AddCalculationInfo(calculationInfo);
		if(end != theCalculationSectiontext.end()) // jos ei tarkistusta, menee yli lopusta
			pos = ++end;
	}while(end != theCalculationSectiontext.end());

	return (!theSectionInfo->GetCalculationInfos()->empty());
}

bool NFmiSmartToolIntepreter::ConsistOnlyWhiteSpaces(const std::string &theText)
{
	static const string someSpaces(" \t\r\n");
	if(theText.find_first_not_of(someSpaces) == string::npos)
		return true;
	return false;
}

std::string NFmiSmartToolIntepreter::ExtractNextLine(std::string &theText, std::string::iterator theStartPos, std::string::iterator* theEndPos)
{
	*theEndPos = std::find_if(theStartPos, theText.end(), EndOfLineSearcher());

	string str(theStartPos, *theEndPos);
	return str;
}

// Riviss� on mahdollinen laskuoperaatio esim.
// T = T + 1
// Rivilt� pit�� siis l�ty� muuttuja johon sijoitetaan ja jotain laskuja
// HUOM! Oletus aluksi, pit�� olla scape aina v�liss�, yksinkertaistaa ohjelmointia.
// HUOM!! T�m� vuotaa, jos tapahtuu exception!!!!!!

/*
NFmiSmartToolCalculationInfo* NFmiSmartToolIntepreter::InterpretCalculationLine(const std::string &theCalculationLineText) throw (NFmiSmartToolIntepreter::Exception)
{
	NFmiSmartToolCalculationInfo *calculationInfo = new NFmiSmartToolCalculationInfo;
	stringstream sStream(theCalculationLineText);
	string tmp;
	string tmp2;
	sStream >> tmp; // luetaan muuttuja esim. T tai jos luodaan uutta skripti muuttujaa on sana var
	bool fNewScriptVariable = false;
	if(IsCaseInsensitiveEqual(tmp, "var"))
	{
		sStream >> tmp; // ollaan alustamassa uutta skripti muuttujaa, luetaan nimi talteen
		fNewScriptVariable = true;
	}
	NFmiAreaMaskInfo *assignedVariable = new NFmiAreaMaskInfo;
	InterpretVariable(tmp, assignedVariable);
	calculationInfo->SetResultDataInfo(assignedVariable);
	if(fNewScriptVariable)
		calculationInfo->GetResultDataInfo()->SetDataType(kVariable);
	sStream >> tmp; // luetaan sijoitus operaattori =
	if(tmp != "=")
		throw NFmiSmartToolIntepreter::Exception(string("Sijoitus operaattori '=' v��r�ss� paikassa"));
	NFmiAreaMaskInfo *variableInfo = 0;
	do
	{
		tmp = tmp2 = ""; // tyhjennet��n varmuuden vuoksi
		sStream >> tmp; // luetaan muuttuja/vakio/funktio tai mik� lie
		if(tmp == "")
			break;
		variableInfo = new NFmiAreaMaskInfo;
		InterpretVariable(tmp, variableInfo);

		sStream >> tmp2; // luetaan operaattori (+ - / *)
		calculationInfo->AddCalculationInfo(variableInfo, InterpretCalculationOperator(tmp2));
	}while(!sStream.fail());
	if(calculationInfo->GetCalculationOperandInfoVector()->empty())
	{
		delete calculationInfo;
		calculationInfo = 0;
	}
	return calculationInfo;
}
*/

NFmiSmartToolCalculationInfo* NFmiSmartToolIntepreter::InterpretCalculationLine(const std::string &theCalculationLineText)
{
	string calculationLineText(theCalculationLineText);
	NFmiSmartToolCalculationInfo *calculationInfo = new NFmiSmartToolCalculationInfo;
	calculationInfo->SetCalculationText(theCalculationLineText);
	auto_ptr<NFmiSmartToolCalculationInfo> calculationInfoPtr(calculationInfo); // tuhoaa automaattisesti esim. exceptionin yhteydess�

	exp_ptr = calculationLineText.begin();
	exp_end = calculationLineText.end();

	string tmp;
	if(GetToken()) // luetaan muuttuja johon sijoitetaan esim. T
	{
		tmp = token;
		bool fNewScriptVariable = false;
		if(IsCaseInsensitiveEqual(tmp, "var"))
		{
			GetToken(); // ollaan alustamassa uutta skripti muuttujaa, luetaan nimi talteen
			tmp = token;
			fNewScriptVariable = true;
		}
		NFmiAreaMaskInfo *assignedVariable = new NFmiAreaMaskInfo;
		auto_ptr<NFmiAreaMaskInfo> assignedVariablePtr(assignedVariable); // tuhoaa automaattisesti esim. exceptionin yhteydess�
		InterpretVariable(tmp, assignedVariable, fNewScriptVariable);  // ei saa antaa auto_ptr-otust� t�ss�, muuten se menett�� omistuksen!
		NFmiInfoData::Type dType = assignedVariable->GetDataType();
		if(!(dType == NFmiInfoData::kEditable || dType == NFmiInfoData::kScriptVariableData || dType == NFmiInfoData::kAnyData || dType == NFmiInfoData::kMacroParam))
			throw runtime_error(::GetDictionaryString("SmartToolErrorAssignmentError") + ":\n" + calculationLineText);
		calculationInfo->SetResultDataInfo(assignedVariablePtr.release());// auto_ptr menett�� omistuksen t�ss�

		GetToken(); // luetaan sijoitus operaattori =
		if(string(token) != string("="))
			throw runtime_error(::GetDictionaryString("SmartToolErrorNoAssignmentOperator") + ":\n" + theCalculationLineText);
		NFmiAreaMaskInfo *variableInfo = 0;
		for(; GetToken(); )
		{
			tmp = token; // luetaan muuttuja/vakio/funktio tai mik� lie
			variableInfo = new NFmiAreaMaskInfo;
			auto_ptr<NFmiAreaMaskInfo> variableInfoPtr(variableInfo);
			InterpretToken(tmp, variableInfo); // ei saa antaa auto_ptr-otust� t�ss�, muuten se menett�� omistuksen!
			calculationInfo->AddCalculationInfo(variableInfoPtr.release()); // auto_ptr menett�� omistuksen t�ss�
		}

		if(calculationInfo->GetCalculationOperandInfoVector()->empty())
			return 0;
	}
	calculationInfo->CheckIfAllowMissingValueAssignment();
	if(calculationInfo->GetResultDataInfo()->GetDataType() == NFmiInfoData::kMacroParam)
		fMacroParamFound = true;
	else if(calculationInfo->GetResultDataInfo()->GetDataType() != NFmiInfoData::kScriptVariableData) // skripti muuttuja voi olla kummassakin tapauksessa mukana
	{
		fNormalAssigmentFound = true;
		if(fMacroParamSkriptInProgress)
			throw runtime_error(::GetDictionaryString("SmartToolErrorMacroParamAssignmentError1") + "\n" + ::GetDictionaryString("SmartToolErrorMacroParamAssignmentError2") + "\n" + ::GetDictionaryString("SmartToolErrorThatWontWorkEnding"));
	}
	// tarkistetaan saman tien, onko sijoituksia tehty molempiin tyyppeihin ja heitet��n poikkeus jos on
	if(fMacroParamFound && fNormalAssigmentFound)
		throw runtime_error(::GetDictionaryString("SmartToolErrorMacroParamAssignmentError3") + "\n" + ::GetDictionaryString("SmartToolErrorMacroParamAssignmentError4") + "\n" + ::GetDictionaryString("SmartToolErrorThatWontWorkEnding"));

	calculationInfoPtr.release();
	return calculationInfo;
}

// GetToken ja IsDelim otettu H. Schilbertin  C++: the Complete Refeference third ed.
// jouduin muuttamaan niit� t�h�n ymp�rist��n.
// Obtain the next token. Palauttaa true, jos sellainen saatiin, jos ollaan lopussa, palauttaa false.
bool NFmiSmartToolIntepreter::GetToken(void)
{
	register char *temp;

	tok_type = NOTYPE;
	temp = token;
	*temp = '\0';

	if(exp_ptr>=exp_end)
		return false; // at end of expression

	while(exp_ptr<exp_end && isspace(*exp_ptr))
		++exp_ptr; // skip over white space
	if(exp_ptr>=exp_end)
		return false; // at end of expression

	// HUOM! t�ss� delimiter rimpsussa ei ole spacea, joten ei voi tehd� yhteist� stringi�, muista p�ivitt�� my�s IsDelim-metodi
	if(strchr("+-*/%^=(){}<>&|!,", *exp_ptr))
	{
/*
		if(strchr("+-", *exp_ptr) && isdigit(exp_ptr[1])) // etumerkilliset vakiot otetaan t�ss�
		{
			*temp++ = *exp_ptr++;
			while(!IsDelim(*exp_ptr))
				*temp++ = *exp_ptr++;
			tok_type = NUMBER;
		}
		else */
//		{
		tok_type = DELIMITER;
		// advance to next char
		*temp++ = *exp_ptr++;
		if(*exp_ptr == '>' || *exp_ptr == '=') // t�ss� halutaan ottaa huomioon >=, <=, !=, <> ja == vertailu operaattorit
		{
			*temp++ = *exp_ptr++;
		}
		if(*exp_ptr == '&') // t�ss� halutaan ottaa huomioon && vertailu operaattori
		{
			*temp++ = *exp_ptr++;
		}
		if(*exp_ptr == '|') // t�ss� halutaan ottaa huomioon || vertailu operaattori
		{
			*temp++ = *exp_ptr++;
		}
//		}
	}
	else if(isalpha(*exp_ptr))
	{
		while(!IsDelim(*exp_ptr))
			*temp++ = *exp_ptr++;
		tok_type = VARIABLE;
	}
	else if(isdigit(*exp_ptr))
	{
		while(!IsDelim(*exp_ptr))
			*temp++ = *exp_ptr++;
		tok_type = NUMBER;
	}

	*temp = '\0';
	return true;
}

// Return true if c is a delimiter.
bool NFmiSmartToolIntepreter::IsDelim(char c)
{ // HUOM! t�ss� delimiter rimpsussa on space muiden lis�ksi, joten ei voi tehd� yhteist� stringi�, muista p�ivitt�� my�s GetToken-metodi
	if(strchr(" +-*/%^=(){}<>&|!,", c) || c==9 || c=='\r' || c=='\t' || c==0) // lis�sin muutaman delimiter merkin
		return true;
	return false;
}

NFmiAreaMask::CalculationOperator NFmiSmartToolIntepreter::InterpretCalculationOperator(const std::string &theOperatorText)
{
	if(theOperatorText == string(""))
		return NFmiAreaMask::NotOperation;
	else if(theOperatorText == "+")
		return NFmiAreaMask::Add;
	else if(theOperatorText == "-")
		return NFmiAreaMask::Sub;
	else if(theOperatorText == "/")
		return NFmiAreaMask::Div;
	else if(theOperatorText == "*")
		return NFmiAreaMask::Mul;
	else if(theOperatorText == "^")
		return NFmiAreaMask::Pow;
	else if(theOperatorText == "%")
		return NFmiAreaMask::Mod;
	else
		throw runtime_error(::GetDictionaryString("SmartToolErrorCalculationOperatorError") + ": " + theOperatorText);
}

void NFmiSmartToolIntepreter::InterpretToken(const std::string &theTokenText, NFmiAreaMaskInfo *theMaskInfo)
{
	switch(tok_type)
	{
	case DELIMITER:
		InterpretDelimiter(theTokenText, theMaskInfo);
		break;
	case VARIABLE:
	case NUMBER:
		InterpretVariable(theTokenText, theMaskInfo);
		break;
	default:
		throw runtime_error(::GetDictionaryString("SmartToolErrorStrangeWord") + ": " + theTokenText);
	}
}

// HUOM!!!! Muuta k�ytt�m��n itsCalculationOperations-mappia!!!!!!!!!!
void NFmiSmartToolIntepreter::InterpretDelimiter(const std::string &theDelimText, NFmiAreaMaskInfo *theMaskInfo)
{
	theMaskInfo->SetMaskText(theDelimText);
	if(theDelimText == string(""))
	{
		theMaskInfo->SetOperationType(NFmiAreaMask::NoType);
		return;
	}

	CalcOperMap::iterator it = itsCalculationOperations.find(theDelimText);
	if(it != itsCalculationOperations.end())
	{
		theMaskInfo->SetOperationType(NFmiAreaMask::Operator);
		theMaskInfo->SetCalculationOperator((*it).second);
		return ;
	}

	MaskOperMap::iterator it2 = itsTokenMaskOperations.find(theDelimText);
	if(it2 != itsTokenMaskOperations.end())
	{
		theMaskInfo->SetOperationType(NFmiAreaMask::Comparison);
		NFmiCalculationCondition maskCondition((*it2).second, 0);
		theMaskInfo->SetMaskCondition(maskCondition);
		return ;
	}

	BinaOperMap::iterator it3 = itsBinaryOperator.find(theDelimText);
	if(it3 != itsBinaryOperator.end())
	{
		theMaskInfo->SetOperationType(NFmiAreaMask::BinaryOperatorType);
		theMaskInfo->SetBinaryOperator((*it3).second);
		return ;
	}

	if(theDelimText == "(")
	{
		theMaskInfo->SetOperationType(NFmiAreaMask::StartParenthesis);
	}
	else if(theDelimText == ")")
	{
		theMaskInfo->SetOperationType(NFmiAreaMask::EndParenthesis);
	}
	else if(theDelimText == ",")
	{
		theMaskInfo->SetOperationType(NFmiAreaMask::CommaOperator);
	}
	else
		throw runtime_error(::GetDictionaryString("SmartToolErrorCalculationOperatorError") + ": " + theDelimText);

}

// Tulkitsee annetun muuttujan. Hoitelee my�s tuottajan, jos se on annettu, esim.
// T T_HIR jne.
// Voi olla my�s vakio tai funktio systeemi.
// HUOM!!! Ei viel� tuottajan handlausta.
// HUOM!!! Ei hoida viel� vakioita tai funktio systeemej�.
void NFmiSmartToolIntepreter::InterpretVariable(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo, bool fNewScriptVariable)
{
	theMaskInfo->SetMaskText(theVariableText);
	string paramNameOnly;
	string levelNameOnly;
	string producerNameOnly;
	bool levelExist = false;
	bool producerExist = false;
	fUseAnyDataTypeBecauseUsingProdID = false; // alustetaan falseksi, asetus true:ksi tapahtuu siell�, miss� katsotaan onko tuottaja annettu PROD104 tyyliin

	// tutkitaan ensin onko mahdollisesti variable-muuttuja, jolloin voimme sallia _-merkin k�yt�n muuttujissa
	if(InterpretPossibleScriptVariable(theVariableText, theMaskInfo, fNewScriptVariable))
		return ;

	CheckVariableString(theVariableText, paramNameOnly,
						  levelExist, levelNameOnly,
						  producerExist, producerNameOnly);

	bool origWanted = producerExist ? IsProducerOrig(producerNameOnly) : false; // lis�sin ehdon, koska boundchecker valitti tyhj�n stringin vertailua IsProducerOrig-metodissa.

//	bool levelInVariableName = ExtractParamAndLevel(theVariableText, &paramNameOnly, &levelNameOnly);
	if(InterpretVariableCheckTokens(theVariableText, theMaskInfo, origWanted, levelExist, producerExist, paramNameOnly, levelNameOnly, producerNameOnly))
	{
		if(fNewScriptVariable)
			throw runtime_error(::GetDictionaryString("SmartToolErrorTokenWordUsedAsVariable") + ": " + theVariableText);
		else
		{
			if(fUseAnyDataTypeBecauseUsingProdID)
				theMaskInfo->SetDataType(NFmiInfoData::kAnyData);
			return ;
		}
	}

	throw runtime_error(::GetDictionaryString("SmartToolErrorStrangeVariable") + ": " + theVariableText);
}

bool NFmiSmartToolIntepreter::InterpretPossibleScriptVariable(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo, bool fNewScriptVariable)
{
	ScriptVariableMap::iterator it = itsTokenScriptVariableNames.find(theVariableText);
	if(it != itsTokenScriptVariableNames.end() && fNewScriptVariable) // var x k�ytetty uudestaan, esitell��n muuttujat vain kerran
		throw runtime_error(::GetDictionaryString("SmartToolErrorScriptVariableSecondTime") + ": " + theVariableText);
	else if(it != itsTokenScriptVariableNames.end()) // muuttujaa x k�ytetty uudestaan
	{
		NFmiParam param((*it).second, (*it).first);
		NFmiProducer producer; // t�ll� ei ole v�li�
		NFmiDataIdent dataIdent(param, producer);
		theMaskInfo->SetOperationType(NFmiAreaMask::InfoVariable);
		theMaskInfo->SetDataIdent(dataIdent);
		theMaskInfo->SetDataType(NFmiInfoData::kScriptVariableData);
		theMaskInfo->SetUseDefaultProducer(false); // t�m� ei todellakaan ole default tuottajan dataa (t�m� vaikuttaa siihen mm. ett� tehd��nk� datasta kopioita tietyiss� tilanteissa)
		return true;
	}
	else if(fNewScriptVariable) // var x, eli 1. alustus
	{
		NFmiParam param(itsScriptVariableParamIdCounter, theVariableText);
		itsTokenScriptVariableNames.insert(ScriptVariableMap::value_type(theVariableText, itsScriptVariableParamIdCounter));
		itsScriptVariableParamIdCounter++; // kasvatetaan seuraavaa uutta muutujaa varten
		NFmiProducer producer; // t�ll� ei ole v�li�
		NFmiDataIdent dataIdent(param, producer);
		theMaskInfo->SetOperationType(NFmiAreaMask::InfoVariable);
		theMaskInfo->SetDataIdent(dataIdent);
		theMaskInfo->SetDataType(NFmiInfoData::kScriptVariableData);
		theMaskInfo->SetUseDefaultProducer(false); // t�m� ei todellakaan ole default tuottajan dataa (t�m� vaikuttaa siihen mm. ett� tehd��nk� datasta kopioita tietyiss� tilanteissa)
		return true;
	}
	return false;
}

bool NFmiSmartToolIntepreter::InterpretVariableCheckTokens(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo, bool fOrigWanted, bool fLevelExist, bool fProducerExist, const std::string &theParamNameOnly, const std::string &theLevelNameOnly, const std::string &theProducerNameOnly)
{
	if(fLevelExist && fProducerExist) // kokeillaan ensin, l�ytyyk� param+level+producer
	{
		if(FindParamAndLevelAndProducerAndSetMaskInfo(theParamNameOnly, theLevelNameOnly, theProducerNameOnly, NFmiAreaMask::InfoVariable, fOrigWanted ? NFmiInfoData::kCopyOfEdited : NFmiInfoData::kViewable, theMaskInfo))
			return true;
	}
	else if(fLevelExist) // kokeillaan ensin, l�ytyyk� param+level+producer
	{
//		if(FindParamAndLevelAndSetMaskInfo(theParamNameOnly, theLevelNameOnly, NFmiAreaMask::InfoVariable, NFmiInfoData::kViewable, theMaskInfo))
		// Jos tuottajaa ei ole mainittu, oletetaan, ett� kyseess� on editoitava parametri.
		if(FindParamAndLevelAndSetMaskInfo(theParamNameOnly, theLevelNameOnly, NFmiAreaMask::InfoVariable, NFmiInfoData::kEditable, theMaskInfo))
			return true;
	}
	else if(fProducerExist) // kokeillaan ensin, l�ytyyk� param+level+producer
	{
		if(FindParamAndProducerAndSetMaskInfo(theParamNameOnly, theProducerNameOnly, NFmiAreaMask::InfoVariable, fOrigWanted ? NFmiInfoData::kCopyOfEdited : NFmiInfoData::kViewable, theMaskInfo))
			return true;
	}

	if(FindParamAndSetMaskInfo(theVariableText, itsTokenParameterNamesAndIds, NFmiAreaMask::InfoVariable, NFmiInfoData::kEditable, theMaskInfo))
		return true;

	if(FindParamAndSetMaskInfo(theVariableText, itsTokenStaticParameterNamesAndIds, NFmiAreaMask::InfoVariable, NFmiInfoData::kStationary, theMaskInfo))
		return true;

	if(FindParamAndSetMaskInfo(theVariableText, itsTokenCalculatedParameterNamesAndIds, NFmiAreaMask::CalculatedVariable, NFmiInfoData::kCalculatedValue, theMaskInfo))
		return true;

	if(IsVariableConstantValue(theVariableText, theMaskInfo))
		return true;

	if(IsVariableThreeArgumentFunction(theVariableText, theMaskInfo))
		return true;

	if(IsVariableFunction(theVariableText, theMaskInfo))
		return true;

	if(IsVariableMathFunction(theVariableText, theMaskInfo))
		return true;

	if(IsVariableRampFunction(theVariableText, theMaskInfo))
		return true;

	if(IsVariableMacroParam(theVariableText, theMaskInfo))
		return true;

	if(IsVariableDeltaZ(theVariableText, theMaskInfo))
		return true;

	if(IsVariableBinaryOperator(theVariableText, theMaskInfo)) // t�m� on and ja or tapausten k�sittelyyn
		return true;

	if(IsVariableSoundingParameter(theVariableText, theMaskInfo, fProducerExist, theParamNameOnly, theProducerNameOnly))
		return true;

	return false;
}
bool NFmiSmartToolIntepreter::IsProducerOrig(std::string &theProducerText)
{
	// Normalize the type name
	string name(theProducerText);
	transform(name.begin(), name.end(), name.begin(), ::tolower);
	if(name == "orig")
		return true;
	return false;
}

// Saa parametrina kokonaisen parametri stringin, joka voi sis�lt��
// parametrin lis�ksi my�s levelin ja producerin.
// Teksti voi siis olla vaikka: T, T_850, T_Ec, T_850_Ec
// Eri osat asetetaan parametreina annetuille stringeille ja exist
// parametrit asetetaan my�s vastaavasti, paitsi itse paramStringille
// ei ole muutujaa, koska se on pakollinen.
void NFmiSmartToolIntepreter::CheckVariableString(const std::string &theVariableText, std::string &theParamText,
												  bool &fLevelExist, std::string &theLevelText,
												  bool &fProducerExist, std::string &theProducerText)
{
	theParamText = theLevelText = theProducerText = "";
	fLevelExist = fProducerExist = false;

	string::size_type pos1 = theVariableText.find(string("_"));
	string::size_type pos2 = theVariableText.find(string("_"), pos1+1);
	if(pos1 != string::npos)
	{
		theParamText = string(theVariableText.begin(), theVariableText.begin() + pos1);
		pos1++;
		string tmp(theVariableText.begin() + pos1, pos2 != string::npos ? theVariableText.begin() + pos2 : theVariableText.end());
		if(IsPossiblyLevelItem(tmp, itsTokenLevelNamesIdentsAndValues))
		{
			fLevelExist = true;
			theLevelText = tmp;
		}
		else if(IsPossiblyProducerItem(tmp, itsTokenProducerNamesAndIds))
		{
			fProducerExist = true;
			theProducerText = tmp;
		}
		else
			throw runtime_error(::GetDictionaryString("SmartToolErrorVariableWithUndescore") + ":\n" + theVariableText);

		if(pos2 != string::npos)
		{
			pos2++;
			string tmp(theVariableText.begin() + pos2, theVariableText.end());
			if(IsPossiblyLevelItem(tmp, itsTokenLevelNamesIdentsAndValues))
			{
				if(fLevelExist == false)
				{
					fLevelExist = true;
					theLevelText = tmp;
				}
				else
					throw runtime_error(::GetDictionaryString("SmartToolErrorVariableWithTwoLevels") + ":\n" + theVariableText);
			}
			else if(IsPossiblyProducerItem(tmp, itsTokenProducerNamesAndIds))
			{
				if(fProducerExist == false)
				{
					fProducerExist = true;
					theProducerText = tmp;
				}
				else
					throw runtime_error(::GetDictionaryString("SmartToolErrorVariableWithTwoProducers") + ":\n" + theVariableText);
			}
			else
				throw runtime_error(::GetDictionaryString("SmartToolErrorVariableNoLevelOrProducer") + ":\n" + theVariableText);
		}
	}
	else
		theParamText = theVariableText;
}

bool NFmiSmartToolIntepreter::IsPossiblyProducerItem(const std::string &theText, ProducerMap &theMap)
{
	if(IsInMap(theMap, theText))
		return true;
	else if(IsWantedStart(theText, "prod"))
		return true;
	return false;
}
bool NFmiSmartToolIntepreter::IsPossiblyLevelItem(const std::string &theText, LevelMap &theMap)
{
	if(IsInMap(theMap, theText))
		return true;
	else if(IsWantedStart(theText, "lev"))
		return true;
	return false;
}

// HUOM! tekee lower case tarkasteluja, joten theMap pit�� my�s alustaa lower case stringeill�
template<typename mapType>
bool NFmiSmartToolIntepreter::IsInMap(mapType& theMap, const std::string &theSearchedItem)
{
	std::string lowerCaseItem(theSearchedItem);
	NFmiStringTools::LowerCase(lowerCaseItem);
    typename mapType::iterator it = theMap.find(lowerCaseItem);
	if(it != theMap.end())
		return true;
	return false;
}

bool NFmiSmartToolIntepreter::ExtractParamAndLevel(const std::string &theVariableText, std::string *theParamNameOnly, std::string *theLevelNameOnly)
{
	string::size_type pos = theVariableText.find(string("_"));
	if(pos != string::npos)
	{
		*theParamNameOnly = string(theVariableText.begin(), theVariableText.begin() + pos);
		pos++;
		*theLevelNameOnly = string(theVariableText.begin() + pos, theVariableText.end());
		return true;
	}
	return false;
}

bool NFmiSmartToolIntepreter::FindParamAndLevelAndSetMaskInfo(const std::string &theVariableText, const std::string &theLevelText, NFmiAreaMask::CalculationOperationType theOperType, NFmiInfoData::Type theDataType, NFmiAreaMaskInfo *theMaskInfo)
{
	if(FindParamAndSetMaskInfo(theVariableText, itsTokenParameterNamesAndIds, theOperType, theDataType, theMaskInfo))
	{
		NFmiLevel level(GetPossibleLevelInfo(theLevelText, theDataType));
		theMaskInfo->SetLevel(&level);
		return true;
	}
	return false;
}

bool NFmiSmartToolIntepreter::FindParamAndProducerAndSetMaskInfo(const std::string &theVariableText, const std::string &theProducerText, NFmiAreaMask::CalculationOperationType theOperType, NFmiInfoData::Type theDataType, NFmiAreaMaskInfo *theMaskInfo)
{
	NFmiProducer producer(GetPossibleProducerInfo(theProducerText));
	if(producer.GetIdent() == gMesanProdId)
		theDataType = NFmiInfoData::kAnalyzeData; // ik�v�� koodia, mutta analyysi data tyyppi pit�� asettaa jos ANAL-tuottajaa k�ytetty
	if(FindParamAndSetMaskInfo(theVariableText, itsTokenParameterNamesAndIds, theOperType, theDataType, theMaskInfo, producer))
		return true;
	return false;
}

bool NFmiSmartToolIntepreter::FindParamAndLevelAndProducerAndSetMaskInfo(const std::string &theVariableText, const std::string &theLevelText,const std::string &theProducerText, NFmiAreaMask::CalculationOperationType theOperType, NFmiInfoData::Type theDataType, NFmiAreaMaskInfo *theMaskInfo)
{
	NFmiProducer producer(GetPossibleProducerInfo(theProducerText));
	if(producer.GetIdent() == gMesanProdId)
		theDataType = NFmiInfoData::kAnalyzeData; // ik�v�� koodia, mutta analyysi data tyyppi pit�� asettaa jos ANAL-tuottajaa k�ytetty
	if(FindParamAndSetMaskInfo(theVariableText, itsTokenParameterNamesAndIds, theOperType, theDataType, theMaskInfo, producer))
	{
		NFmiLevel level(GetPossibleLevelInfo(theLevelText, theDataType));
		theMaskInfo->SetLevel(&level);
		return true;
	}
	return false;
}

// Level tekstin (850, 500 jne.) luodaan NFmiLevel-otus.
NFmiLevel NFmiSmartToolIntepreter::GetPossibleLevelInfo(const std::string &theLevelText, NFmiInfoData::Type theDataType)
{
	NFmiLevel level;
	LevelMap::iterator it = itsTokenLevelNamesIdentsAndValues.find(theLevelText);
	if(it != itsTokenLevelNamesIdentsAndValues.end())
		level = NFmiLevel((*it).second.first, (*it).first, (*it).second.second);
	else if(!GetLevelFromVariableById(theLevelText, level, theDataType))
		throw runtime_error(::GetDictionaryString("SmartToolErrorLevelInfoFailed") + ":\n" + theLevelText);
	return level;
}

// Tuottaja tekstin (Ec, Hir jne.) luodaan NFmiProducer-otus.
NFmiProducer NFmiSmartToolIntepreter::GetPossibleProducerInfo(const std::string &theProducerText)
{
	NFmiProducer producer;
	std::string lowerCaseProdName(theProducerText);
	NFmiStringTools::LowerCase(lowerCaseProdName);
	ProducerMap::iterator it = itsTokenProducerNamesAndIds.find(lowerCaseProdName);
	if(it != itsTokenProducerNamesAndIds.end())
	{
		producer = NFmiProducer((*it).second, (*it).first);
		return producer;
	}
	else if(!GetProducerFromVariableById(theProducerText, producer))
		throw runtime_error(::GetDictionaryString("SmartToolErrorProducerInfoFailed") + ":\n" + theProducerText);
	return producer;
}

bool NFmiSmartToolIntepreter::IsInterpretedSkriptMacroParam(void)
{
	return (fNormalAssigmentFound == false) && (fMacroParamFound == true);
}

bool NFmiSmartToolIntepreter::GetParamFromVariable(const std::string &theVariableText, ParamMap& theParamMap, NFmiParam &theParam, bool &fUseWildDataType)
{
	std::string tmp(theVariableText);
	ParamMap::iterator it = theParamMap.find(NFmiStringTools::LowerCase(tmp));
	if(it == theParamMap.end())
	{
		if(GetParamFromVariableById(theVariableText, theParam))
			fUseWildDataType = true; // paridta k�ytett�ess� pit�� asettaa data tyyppi 'villiksi' toistaiseksi
		else
			return false;
	}
	else
		theParam = NFmiParam((*it).second, (*it).first);
	return true;
}

// tutkii alkaako annettu sana "par"-osiolla ja sit� seuraavalla numerolla
// esim. par4 tai PAR114 jne.
bool NFmiSmartToolIntepreter::GetParamFromVariableById(const std::string &theVariableText, NFmiParam &theParam)
{
	if(IsWantedStart(theVariableText, "par"))
	{
		NFmiValueString numericPart(theVariableText.substr(3));
		if(numericPart.IsNumeric())
		{
			theParam = NFmiParam((long)numericPart, theVariableText);
			return true;
		}
	}
	return false;
}

// tutkii alkaako annettu sana "prod"-osiolla ja sit� seuraavalla numerolla
// esim. prod230 tai PROD1 jne.
bool NFmiSmartToolIntepreter::GetProducerFromVariableById(const std::string &theVariableText, NFmiProducer &theProducer)
{
	if(IsWantedStart(theVariableText, "prod"))
	{
		NFmiValueString numericPart(theVariableText.substr(4));
		if(numericPart.IsNumeric())
		{
			theProducer = NFmiProducer((long)numericPart, theVariableText);
			fUseAnyDataTypeBecauseUsingProdID = true; // laitetaan trueksi, koska tuottaja annettu PROD104 tyyliin
			return true;
		}
	}
	return false;
}

// tutkii alkaako annettu sana "lev"-osiolla ja sit� seuraavalla numerolla
// esim. par100 tai LEV850 jne.
bool NFmiSmartToolIntepreter::GetLevelFromVariableById(const std::string &theVariableText, NFmiLevel &theLevel, NFmiInfoData::Type theDataType)
{
	if(IsWantedStart(theVariableText, "lev"))
	{
		NFmiValueString numericPart(theVariableText.substr(3));
		if(numericPart.IsNumeric())
		{
			long levelValue = (long)numericPart;
			// pitaisi tunnistaa level tyyppi arvosta kait, nyt oletus ett� painepinta
			FmiLevelType levelType = GetLevelType(theDataType, levelValue);
			theLevel = NFmiLevel(levelType, theVariableText, (long)numericPart);
			return true;
		}
	}
	return false;
}

FmiLevelType NFmiSmartToolIntepreter::GetLevelType(NFmiInfoData::Type theDataType, long levelValue)
{
	FmiLevelType levelType = kFmiPressureLevel; // default
	if(theDataType == NFmiInfoData::kEditable)
	{
		NFmiSmartInfo *editedInfo = itsInfoOrganizer ? itsInfoOrganizer->EditedInfo() : 0;
		if(editedInfo)
		{
			for(editedInfo->ResetLevel(); editedInfo->NextLevel(); )
			{
				if(editedInfo->Level()->LevelValue() == levelValue)
				{
					levelType = editedInfo->Level()->LevelType();
					break;
				}
			}
		}
	}
	return levelType;
}

bool NFmiSmartToolIntepreter::IsWantedStart(const std::string &theText, const std::string &theWantedStart)
{
	string name(theText.substr(0, theWantedStart.size()));
	transform(name.begin(), name.end(), name.begin(), ::tolower);
	if(name == theWantedStart)
		return true;
	return false;
}

bool NFmiSmartToolIntepreter::IsCaseInsensitiveEqual(const std::string &theStr1, const std::string &theStr2)
{
	string tmp1(theStr1);
	transform(tmp1.begin(), tmp1.end(), tmp1.begin(), ::tolower);
	string tmp2(theStr2);
	transform(tmp2.begin(), tmp2.end(), tmp2.begin(), ::tolower);
	if(tmp1 == tmp2)
		return true;
	return false;
}

bool NFmiSmartToolIntepreter::FindParamAndSetMaskInfo(const std::string &theVariableText, ParamMap& theParamMap, NFmiAreaMask::CalculationOperationType theOperType, NFmiInfoData::Type theDataType, NFmiAreaMaskInfo *theMaskInfo)
{
	NFmiParam param;
	bool fUseWildDataType = false;
	if(GetParamFromVariable(theVariableText, theParamMap, param, fUseWildDataType))
	{
		NFmiProducer producer;
		NFmiDataIdent dataIdent(param, producer);
		theMaskInfo->SetOperationType(theOperType);
		theMaskInfo->SetDataIdent(dataIdent);
		theMaskInfo->SetUseDefaultProducer(true);
		if(fUseWildDataType)
			theMaskInfo->SetDataType(NFmiInfoData::kAnyData);
		else
			theMaskInfo->SetDataType(theDataType);
		return true;
	}
	return false;
}

bool NFmiSmartToolIntepreter::FindParamAndSetMaskInfo(const std::string &theVariableText, ParamMap& theParamMap, NFmiAreaMask::CalculationOperationType theOperType, NFmiInfoData::Type theDataType, NFmiAreaMaskInfo *theMaskInfo, const NFmiProducer &theProducer)
{
	NFmiParam param;
	bool fUseWildDataType = false;
	if(GetParamFromVariable(theVariableText, theParamMap, param, fUseWildDataType))
	{
		NFmiDataIdent dataIdent(param, theProducer);
		theMaskInfo->SetOperationType(theOperType);
		theMaskInfo->SetDataIdent(dataIdent);
		theMaskInfo->SetUseDefaultProducer(false);
		if(fUseWildDataType)
			theMaskInfo->SetDataType(NFmiInfoData::kAnyData);
		else if(theProducer.GetIdent() == 999) // t�m� 999 on viritys, mutta se on m��r�tty helpdatainfo.dat tiedostossa kepa-datan feikki id numeroksi. Oikeaa id:t� ei voi k�ytt��, koska se on sama kuin editoitavalla datalla.
			theMaskInfo->SetDataType(NFmiInfoData::kKepaData);
		else
			theMaskInfo->SetDataType(theDataType);
		return true;
	}
	return false;
}

bool NFmiSmartToolIntepreter::IsVariableConstantValue(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo)
{
	NFmiValueString valueString(theVariableText);
	if(valueString.IsNumeric())
	{
		double value = (double)valueString;
		theMaskInfo->SetOperationType(NFmiAreaMask::Constant);
		NFmiCalculationCondition calcCond(kFmiMaskEqual, value);
		theMaskInfo->SetMaskCondition(calcCond);
		return true;
	}

	// sitten katsotaan onko se esim. MISS tai PI tms esi m��ritelty vakio
	std::string tmp(theVariableText);
	ConstantMap::iterator it = itsTokenConstants.find(NFmiStringTools::LowerCase(tmp));
	if(it != itsTokenConstants.end())
	{
		theMaskInfo->SetOperationType(NFmiAreaMask::Constant);
		NFmiCalculationCondition calcCond(kFmiMaskEqual, (*it).second);
		theMaskInfo->SetMaskCondition(calcCond);
		return true;
	}
	return false;
}

bool NFmiSmartToolIntepreter::IsVariableMacroParam(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo)
{
	if(FindAnyFromText(theVariableText, itsTokenMacroParamIdentifiers))
	{
		theMaskInfo->SetOperationType(NFmiAreaMask::InfoVariable);
		theMaskInfo->SetDataType(NFmiInfoData::kMacroParam);
		return true;
	}
	return false;
}

bool NFmiSmartToolIntepreter::IsVariableDeltaZ(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo)
{
	if(FindAnyFromText(theVariableText, itsTokenDeltaZIdentifiers))
	{
		theMaskInfo->SetOperationType(NFmiAreaMask::DeltaZFunction);
		return true;
	}
	return false;
}

bool NFmiSmartToolIntepreter::IsVariableMathFunction(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo)
{
	std::string tmp(theVariableText);
	MathFunctionMap::iterator it = itsMathFunctions.find(NFmiStringTools::LowerCase(tmp));
	if(it != itsMathFunctions.end())
	{
		string tmp;
		if(GetToken())
		{
			tmp = token; // luetaan muuttuja/vakio/funktio tai mik� lie
			if(tmp == string("(")) // etsit��n fuktion aloitus sulkua (lopetus sulku tulee sitten aikanaan, v�liss� voi olla mit� vain!)
			{
				theMaskInfo->SetOperationType(NFmiAreaMask::MathFunctionStart);
				theMaskInfo->SetMathFunctionType((*it).second);
				return true;
			}
		}
		throw runtime_error(::GetDictionaryString("SmartToolErrorMathFunctionParams") + ": " + theVariableText);
	}
	return false;
}

bool NFmiSmartToolIntepreter::IsVariableSoundingParameter(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo, bool fProducerExist, const std::string &theParamNameOnly, const std::string &theProducerNameOnly)
{
	std::string tmp(fProducerExist ? theParamNameOnly : theVariableText);
	SoundingIndexMap::iterator it = itsTokenSoundingIndexFunctions.find(NFmiStringTools::LowerCase(tmp)); // t�ss� tarkastellaan case insensitiivisesti
	if(it != itsTokenSoundingIndexFunctions.end())
	{
		theMaskInfo->SetOperationType(NFmiAreaMask::SoundingIndexFunction);
		theMaskInfo->SoundingParameter((*it).second);
		NFmiParam param(4, "T"); // l�mp�tila on l�ydytt�v� datasta, muuten ei voi laskea mit��n
		NFmiProducer producer;
		if(fProducerExist)
		{
			producer = GetPossibleProducerInfo(theProducerNameOnly);
			theMaskInfo->SetUseDefaultProducer(false);
		}
		else
		{
			theMaskInfo->SetUseDefaultProducer(true);
		}
		theMaskInfo->SetDataType(NFmiInfoData::kSoundingParameterData);
		theMaskInfo->SetDataIdent(NFmiDataIdent(param, producer));
		return true;
	}
	return false;
}

bool NFmiSmartToolIntepreter::IsVariableThreeArgumentFunction(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo)
{
	// sitten katsotaan onko jokin integraatio funktioista
	std::string tmp(theVariableText);
	FunctionMap::iterator it = itsTokenThreeArgumentFunctions.find(NFmiStringTools::LowerCase(tmp));
	if(it != itsTokenThreeArgumentFunctions.end())
	{
		int functionUsed = 1; // 1 = T-funktio, 2 = Z-funktio ja 3 = H-funktio
		if(theVariableText.find('z') != string::npos || theVariableText.find('Z') != string::npos)
			functionUsed = 2;
		else if(theVariableText.find('h') != string::npos || theVariableText.find('H') != string::npos)
			functionUsed = 3;
		theMaskInfo->SetFunctionType((*it).second); // min, max jne. asetus
		theMaskInfo->IntegrationFunctionType(functionUsed);
		string tmp;
		if(GetToken())
		{
			tmp = token; // luetaan muuttuja/vakio/funktio tai mik� lie
			if(tmp == string("(")) // etsit��n fuktion aloitus sulkua (lopetus sulku tulee sitten aikanaan, v�liss� voi olla mit� vain!)
			{
				theMaskInfo->SetOperationType(NFmiAreaMask::ThreeArgumentFunctionStart);
				return true;
			}
		}
		throw runtime_error(::GetDictionaryString("SmartToolErrorTimeFunctionParams") + ":\n" + theVariableText);
	}
	return false;
}

// N�m� ovat muotoa aika-'integroinnin' yhteydess�:
// MIN(T -1 1)
// eli 1. funktion nimi, sulku auki, parametri, aloitus aikaoffset, lopetus aikaoffset ja lopuksi suliku kiinni.
// Tai alue-'integroinnin' yhteydess�:
// MIN(T -1 -1 1 1)
// eli 1. funktion nimi, sulku auki, parametri, aloitus x ja y paikkaoffset, lopetus x ja y paikkaoffset ja lopuksi suliku kiinni.
bool NFmiSmartToolIntepreter::IsVariableFunction(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo)
{
	// katsotaan onko jokin peek-funktioista
	if(IsVariablePeekFunction(theVariableText, theMaskInfo))
		return true;
	// sitten katsotaan onko jokin integraatio funktioista
	std::string tmp(theVariableText);
	FunctionMap::iterator it = itsTokenFunctions.find(NFmiStringTools::LowerCase(tmp));
	if(it != itsTokenFunctions.end())
	{
		theMaskInfo->SetFunctionType((*it).second);
		string tmp;
		checkedVector<pair<string, types> > tokens;
		int i;
		for(i=0; i<7 && GetToken(); i++) // maksimissaan 7 kertaa
		{
			tmp = token; // luetaan muuttuja/vakio/funktio tai mik� lie
			tmp = HandlePossibleUnaryMarkers(tmp);
			tokens.push_back(std::make_pair(tmp, tok_type));
			if(tmp == string(")")) // etsit��n lopetus sulkua
				break;
		}
		// ensin tutkitaan, onko kyseess� aikaintegrointi eli 5 'tokenia'
		if(i==4 && (tokens[0].first == string("(")) && (tokens[4].first == string(")")))
		{
			if(tokens[1].second == VARIABLE && tokens[2].second == NUMBER && tokens[3].second == NUMBER)
			{
				InterpretVariable(tokens[1].first, theMaskInfo);
				if(theMaskInfo->GetOperationType() == NFmiAreaMask::InfoVariable)
				{
					theMaskInfo->SetOperationType(NFmiAreaMask::FunctionTimeIntergration);
					NFmiValueString valueString1(tokens[2].first);
					double value1 = (double)valueString1;
					NFmiValueString valueString2(tokens[3].first);
					double value2 = (double)valueString2;
					theMaskInfo->SetOffsetPoint1(NFmiPoint(value1, value2));
					return true;
				}
			}
		}
		// sitten tutkitaan, onko kyseess� aikaintegrointi eli 7 'tokenia'
		else if(i==6 && (tokens[0].first == string("(")) && (tokens[6].first == string(")")))
		{
			if(tokens[1].second == VARIABLE && tokens[2].second == NUMBER && tokens[3].second == NUMBER && tokens[4].second == NUMBER && tokens[5].second == NUMBER)
			{
				InterpretVariable(tokens[1].first, theMaskInfo);
				if(theMaskInfo->GetOperationType() == NFmiAreaMask::InfoVariable)
				{
					theMaskInfo->SetOperationType(NFmiAreaMask::FunctionAreaIntergration);
					NFmiValueString valueString1(tokens[2].first);
					double value1 = (double)valueString1;
					NFmiValueString valueString2(tokens[3].first);
					double value2 = (double)valueString2;
					theMaskInfo->SetOffsetPoint1(NFmiPoint(value1, value2));
					NFmiValueString valueString3(tokens[4].first);
					double value3 = (double)valueString3;
					NFmiValueString valueString4(tokens[5].first);
					double value4 = (double)valueString4;
					theMaskInfo->SetOffsetPoint2(NFmiPoint(value3, value4));
					return true;
				}
			}
		}
		throw runtime_error(::GetDictionaryString("SmartToolErrorIntegrationFunctionParams") + ": " + theVariableText);
	}
	return false;
}

bool NFmiSmartToolIntepreter::IsVariablePeekFunction(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo)
{
	std::string aVariableText(theVariableText);
	PeekFunctionMap::iterator it = itsTokenPeekFunctions.find(NFmiStringTools::LowerCase(aVariableText)); // t�ss� tarkastellaan case insensitiivisesti
	if(it != itsTokenPeekFunctions.end())
//	if(FindAnyFromText(theVariableText, itsTokenPeekXYFunctions))
	{
		string tmp;
		checkedVector<pair<string, types> > tokens;
		int i;
		for(i=0; i<7 && GetToken(); i++) // maksimissaan 7 kertaa
		{
			tmp = token; // luetaan muuttuja/vakio/funktio tai mik� lie
			tmp = HandlePossibleUnaryMarkers(tmp);
			tokens.push_back(std::make_pair(tmp, tok_type));
			if(tmp == string(")")) // etsit��n lopetus sulkua
				break;
		}
		// ensin tutkitaan, onko kyseess� peekxy eli yhteens� 5 'tokenia' peekxy(T 1 1)
		if(i==4 && (tokens[0].first == string("(")) && (tokens[4].first == string(")")))
		{
			if(tokens[1].second == VARIABLE && tokens[2].second == NUMBER && tokens[3].second == NUMBER)
			{
				InterpretVariable(tokens[1].first, theMaskInfo);
				if(theMaskInfo->GetOperationType() == NFmiAreaMask::InfoVariable)
				{
					theMaskInfo->SetOperationType((*it).second);
					NFmiValueString valueString1(tokens[2].first);
					double value1 = (double)valueString1;
					NFmiValueString valueString2(tokens[3].first);
					double value2 = (double)valueString2;
					theMaskInfo->SetOffsetPoint1(NFmiPoint(value1, value2));
					return true;
				}
			}
		}
	}
	return false;
}

std::string NFmiSmartToolIntepreter::HandlePossibleUnaryMarkers(const std::string &theCurrentString)
{
	string returnStr(theCurrentString);
	if(returnStr == string("-"))
	{
		GetToken();
		returnStr += token; // lis�t��n '-'-etumerkki ja seuraava token ja katsotaan mit� syntyy
	}
	if(returnStr == string("+"))
		GetToken(); // +-merkki ohitetaan merkityksett�m�n�
	return returnStr;
}

// N�m� ovat muotoa:
// RU(T 1 3)
// eli 1. maskin/funktion nimi, sulku auki, parametri, alkuarvo, loppuarvo ja lopuksi suliku kiinni.
bool NFmiSmartToolIntepreter::IsVariableRampFunction(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo)
{
	if(FindAnyFromText(theVariableText, itsTokenRampFunctions))
	{
		string tmp;
		checkedVector<pair<string, types> > tokens;
		int i;
		for(i=0; i<5 && GetToken(); i++)
		{
			tmp = token; // luetaan muuttuja/vakio/funktio tai mik� lie
			tmp = HandlePossibleUnaryMarkers(tmp);
			tokens.push_back(std::make_pair(tmp, tok_type));
			if(tmp == string(")")) // etsit��n lopetus sulkua
				break;
		}
		if(i==4 && (tokens[0].first == string("(")) && (tokens[4].first == string(")")))
		{
			if(tokens[1].second == VARIABLE && tokens[2].second == NUMBER && tokens[3].second == NUMBER)
			{
				InterpretVariable(tokens[1].first, theMaskInfo);
				NFmiAreaMask::CalculationOperationType type = theMaskInfo->GetOperationType();
				if(type == NFmiAreaMask::InfoVariable || type == NFmiAreaMask::CalculatedVariable)
				{
					theMaskInfo->SetOperationType(NFmiAreaMask::RampFunction);
					NFmiValueString valueString1(tokens[2].first);
					double value1 = (double)valueString1;
					NFmiValueString valueString2(tokens[3].first);
					double value2 = (double)valueString2;
					FmiMaskOperation maskOper = kFmiNoMaskOperation;
					if(FindAnyFromText(theVariableText, itsTokenRampUpFunctions))
						maskOper = kFmiMaskRisingRamp;
					else if(FindAnyFromText(theVariableText, itsTokenRampDownFunctions))
						maskOper = kFmiMaskLoweringRamp;
					else if(FindAnyFromText(theVariableText, itsTokenDoubleRampFunctions))
						maskOper = kFmiMaskDoubleRamp;
					NFmiCalculationCondition condition(maskOper, value1, value2);
					theMaskInfo->SetMaskCondition(condition);

					return true;
				}
			}
		}
		throw runtime_error(::GetDictionaryString("SmartToolErrorRampFunctionParams") + ": " + theVariableText);
	}
	return false;
}

bool NFmiSmartToolIntepreter::IsVariableBinaryOperator(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo)
{
	BinaOperMap::iterator it = itsBinaryOperator.find(theVariableText);
	if(it != itsBinaryOperator.end())
	{
		theMaskInfo->SetOperationType(NFmiAreaMask::BinaryOperatorType);
		theMaskInfo->SetBinaryOperator((*it).second);
		return true;
	}
	return false;
}

//--------------------------------------------------------
// InterpretNextMask
//--------------------------------------------------------
bool NFmiSmartToolIntepreter::InterpretNextMask(const std::string &theMaskSectionText)
{
	return false;
}

NFmiParam NFmiSmartToolIntepreter::GetParamFromString(const std::string &theParamText)
{
	NFmiParam param;
	ParamMap::iterator it = itsTokenParameterNamesAndIds.find(theParamText);
	if(it == itsTokenParameterNamesAndIds.end())
	{
		if(!GetParamFromVariableById(theParamText, param))
			throw runtime_error(::GetDictionaryString("SmartToolErrorWantedParam1") + " '" + theParamText + "' " + ::GetDictionaryString("SmartToolErrorWantedParam2"));
	}
	else
		param = NFmiParam((*it).second, (*it).first);
	return param;
}

//--------------------------------------------------------
// ExtractFirstCalculationSection
//--------------------------------------------------------
// Metodi saa parametrina stripatun macro-tekstin ja etsii siit� 1. calculation sectionin.
// Jos l�ytyy, palauta sen positio, jos ei l�ydy, palauta string.end().
// Parametrina tulee aloitus positio (merkitsee enemm�n muissa vastaavissa metodeissa).
// Tee my�s seuraavat metodit:
// ExtractNextCalculationSection
// ExtractIfMaskSection
// ExtractNextElseIfMaskSection
// ExtractElseCalculationSection

 // 1310 eli k��nt�j�n versio 13.1 eli MSVC++ 7.1  toteuttaa 1. kerran kunnolla standardia
#ifdef OLD_MSC
std::string::iterator NFmiSmartToolIntepreter::ExtractFirstCalculationSection(const std::string &theMacroText, std::string::iterator theStartPosition)
#else
std::string::const_iterator NFmiSmartToolIntepreter::ExtractFirstCalculationSection(const std::string &theMacroText, std::string::iterator theStartPosition)
#endif
{
#ifdef OLD_MSC
  return const_cast<std::string::iterator>(theMacroText.end());
#else
  return theMacroText.end();
#endif
}

void NFmiSmartToolIntepreter::Clear(void)
{
	int size = itsSmartToolCalculationBlocks.size();
	for(int i=0; i<size; i++)
		itsSmartToolCalculationBlocks[i].Clear();
}

void NFmiSmartToolIntepreter::InitTokens(NFmiProducerSystem *theProducerSystem)
{
	if(!NFmiSmartToolIntepreter::fTokensInitialized)
	{
		NFmiSmartToolIntepreter::fTokensInitialized = true;

		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("t"), kFmiTemperature));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("p"), kFmiPressure));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("rh"), kFmiHumidity));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("thetaw"), kFmiPseudoAdiabaticPotentialTemperature));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("kind"), kFmiKIndex));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("dp"), kFmiDewPoint));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("lrad"), kFmiRadiationLW));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("srad"), kFmiRadiationGlobal));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("ws"), kFmiWindSpeedMS));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("wd"), kFmiWindDirection));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("u"), kFmiWindUMS));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("v"), kFmiWindVMS));

		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("n"), kFmiTotalCloudCover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("cl"), kFmiLowCloudCover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("cm"), kFmiMediumCloudCover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("ch"), kFmiHighCloudCover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("rr"), kFmiPrecipitation1h));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("pref"), kFmiPrecipitationForm));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("pret"), kFmiPrecipitationType));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("thund"), kFmiProbabilityThunderstorm));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fog"), kFmiFogIntensity));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("hsade"), kFmiWeatherSymbol1));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("hessaa"), kFmiWeatherSymbol3));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("w"), kFmiVerticalVelocityMMS));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("z"), kFmiGeopHeight));

		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("rrcon"), kFmiPrecipitationConv));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("rrlar"), kFmiPrecipitationLarge));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("cape"), kFmiCAPE));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("tke"), kFmiTurbulentKineticEnergy));

		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl1base"), kFmi_FL_1_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl1top"), kFmi_FL_1_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl1cover"), kFmi_FL_1_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl1cloudtype"), kFmi_FL_1_CloudType));

		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl2base"), kFmi_FL_2_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl2top"), kFmi_FL_2_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl2cover"), kFmi_FL_2_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl2cloudtype"), kFmi_FL_2_CloudType));

		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl3base"), kFmi_FL_3_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl3top"), kFmi_FL_3_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl3cover"), kFmi_FL_3_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl3cloudtype"), kFmi_FL_3_CloudType));

		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl4base"), kFmi_FL_4_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl4top"), kFmi_FL_4_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl4cover"), kFmi_FL_4_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl4cloudtype"), kFmi_FL_4_CloudType));

		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl5base"), kFmi_FL_5_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl5top"), kFmi_FL_5_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl5cover"), kFmi_FL_5_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl5cloudtype"), kFmi_FL_5_CloudType));

		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl6base"), kFmi_FL_6_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl6top"), kFmi_FL_6_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl6cover"), kFmi_FL_6_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl6cloudtype"), kFmi_FL_6_CloudType));

		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl7base"), kFmi_FL_7_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl7top"), kFmi_FL_7_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl7cover"), kFmi_FL_7_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl7cloudtype"), kFmi_FL_7_CloudType));

		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl8base"), kFmi_FL_8_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl8top"), kFmi_FL_8_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl8cover"), kFmi_FL_8_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl8cloudtype"), kFmi_FL_8_CloudType));

		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("flcbbase"), kFmi_FL_Cb_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("flcbcover"), kFmi_FL_Cb_Cover));

		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("flminbase"), kFmi_FL_Min_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("flmaxbase"), kFmi_FL_Max_Base));

		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("avivis"), kFmiAviationVisibility));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("vervis"), kFmiVerticalVisibility));

		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("pop"), kFmiPoP));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("mist"), kFmiMist));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("pseudosatel"), kFmiRadiationNetTopAtmLW));

/*

  ,
  ,

  ,
  ,
	kFmiPacked_FL_1 = 625,
	kFmiPacked_FL_2,
	kFmiPacked_FL_3,
	kFmiPacked_FL_4,
	kFmiPacked_FL_5,
	kFmiPacked_FL_6,
	kFmiPacked_FL_7,
	kFmiPacked_FL_8,
	kFmiPacked_FL_9,
	kFmiPacked_FL_10,
	kFmiPacked_FL_11,
	kFmiPacked_FL_12,
*/

		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("topo"), kFmiTopoGraf));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("slope"), kFmiTopoSlope));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("slopedir"), kFmiTopoAzimuth));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("distsea"), kFmiTopoDistanceToSea));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("dirsea"), kFmiTopoDirectionToSea));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("distland"), kFmiTopoDistanceToLand));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("dirland"), kFmiTopoDirectionToLand));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("landseemask"), kFmiLandSeaMask));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("reltopo"), kFmiTopoRelativeHeight));

		itsTokenCalculatedParameterNamesAndIds.insert(ParamMap::value_type(string("lat"), kFmiLatitude));
		itsTokenCalculatedParameterNamesAndIds.insert(ParamMap::value_type(string("lon"), kFmiLongitude));
		itsTokenCalculatedParameterNamesAndIds.insert(ParamMap::value_type(string("eangle"), kFmiElevationAngle));
		itsTokenCalculatedParameterNamesAndIds.insert(ParamMap::value_type(string("jday"), kFmiDay)); // julian day oikeasti
		itsTokenCalculatedParameterNamesAndIds.insert(ParamMap::value_type(string("lhour"), kFmiHour)); // local hour oikeasti
		itsTokenCalculatedParameterNamesAndIds.insert(ParamMap::value_type(string("fhour"), kFmiForecastPeriod)); // forecast hour pikaviritys forperiodia k�ytetty, koska ei ollut valmista parametria
		itsTokenCalculatedParameterNamesAndIds.insert(ParamMap::value_type(string("timestep"), kFmiDeltaTime)); // TIMESTEP eli timestep palauttaa datan currentin ajan aika stepin tunneissa

		itsTokenCalculatedParameterNamesAndIds.insert(ParamMap::value_type(string("gridsizex"), kFmiLastParameter)); // hilan x suuntainen koko metreiss� (muokattavan datan tai macroParam hilan koko)
		itsTokenCalculatedParameterNamesAndIds.insert(ParamMap::value_type(string("gridsizey"), static_cast<FmiParameterName>(kFmiLastParameter+1))); // hilan y suuntainen koko metreiss� (muokattavan datan tai macroParam hilan koko)

		// Alustetaan ensin tuottaja listaan muut tarvittavat tuottajat, Huom! nimi pienell�, koska
		// tehd��n case insensitiivej� tarkasteluja!!
//		itsTokenProducerNamesAndIds.insert(ProducerMap::value_type(string("synop"), kFmiSYNOP));
		itsTokenProducerNamesAndIds.insert(ProducerMap::value_type(string("met"), static_cast<FmiProducerName>(999)));
		itsTokenProducerNamesAndIds.insert(ProducerMap::value_type(string("orig"), kFmiMETEOR));
		itsTokenProducerNamesAndIds.insert(ProducerMap::value_type(string("anal"), static_cast<FmiProducerName>(gMesanProdId)));  // analyysi mesan tuottaja
		itsTokenProducerNamesAndIds.insert(ProducerMap::value_type(string("ana"), static_cast<FmiProducerName>(gMesanProdId)));  // analyysi mesan tuottaja

		if(theProducerSystem)
		{
			// lopuksi tuottaja listaa t�ydennet��n ProducerSystemin tuottajilla
			int modelCount = theProducerSystem->Producers().size();
			int i=0;
			for(i=0; i<modelCount; i++)
			{
				NFmiProducerInfo &prodInfo = theProducerSystem->Producer(i+1);
				std::vector<int> prodIds = prodInfo.ProducerIds();
				FmiProducerName prodId1 = static_cast<FmiProducerName>(0);
				if(prodIds.size() > 0)
					prodId1 = static_cast<FmiProducerName>(prodIds[0]);
				std::string prodName(prodInfo.ShortName());
				NFmiStringTools::LowerCase(prodName); // pit�� muuttaa lower case:en!!!
				itsTokenProducerNamesAndIds.insert(ProducerMap::value_type(prodName, prodId1));
			}
		}
		else
		{
			// t�st� pit�isi varoittaa ja heitt�� poikkeus, mutta aina ei voi alustaa smarttool-systeemi� producersystemill� (esim. SmarttoolFilter ei tied� moisesta mit��n!!)
//			throw std::runtime_error("NFmiSmartToolIntepreter::InitTokens - Was not initialized correctly, ProducerSystem missing, error in program, report it!");

			// joten pakko alustaa t�m� t�ss� tapauksessa sitten jollain hardcode tuottajilla
			itsTokenProducerNamesAndIds.insert(ProducerMap::value_type(string("hir"), kFmiMTAHIRLAM));
			itsTokenProducerNamesAndIds.insert(ProducerMap::value_type(string("ec"), kFmiMTAECMWF));
			itsTokenProducerNamesAndIds.insert(ProducerMap::value_type(string("mbe"), kFmiMTAHIRMESO));
			itsTokenProducerNamesAndIds.insert(ProducerMap::value_type(string("ala"), static_cast<FmiProducerName>(555))); // aladdinille laitetaan pikaviritys 555
		}


		itsTokenConstants.insert(ConstantMap::value_type(string("miss"), kFloatMissing));
		itsTokenConstants.insert(ConstantMap::value_type(string("pi"), 3.14159265358979));

		itsTokenIfCommands.push_back(string("IF"));
		itsTokenIfCommands.push_back(string("if"));
		itsTokenIfCommands.push_back(string("If"));

		itsTokenElseIfCommands.push_back(string("ELSEIF"));
		itsTokenElseIfCommands.push_back(string("elseif"));
		itsTokenElseIfCommands.push_back(string("ElseIf"));

		itsTokenElseCommands.push_back(string("ELSE"));
		itsTokenElseCommands.push_back(string("else"));
		itsTokenElseCommands.push_back(string("Else"));

		// tehdaan yhtenainen ehto komento joukko erilaisiin tarkastuksiin
		itsTokenConditionalCommands.insert(itsTokenConditionalCommands.end(), itsTokenIfCommands.begin(), itsTokenIfCommands.end());
		itsTokenConditionalCommands.insert(itsTokenConditionalCommands.end(), itsTokenElseIfCommands.begin(), itsTokenElseIfCommands.end());
		itsTokenConditionalCommands.insert(itsTokenConditionalCommands.end(), itsTokenElseCommands.begin(), itsTokenElseCommands.end());

		itsTokenMaskOperations.insert(MaskOperMap::value_type(string("="), kFmiMaskEqual));
		itsTokenMaskOperations.insert(MaskOperMap::value_type(string("=="), kFmiMaskEqual));
		itsTokenMaskOperations.insert(MaskOperMap::value_type(string("!="), kFmiMaskNotEqual));
		itsTokenMaskOperations.insert(MaskOperMap::value_type(string("<>"), kFmiMaskNotEqual));
		itsTokenMaskOperations.insert(MaskOperMap::value_type(string(">"), kFmiMaskGreaterThan));
		itsTokenMaskOperations.insert(MaskOperMap::value_type(string("<"), kFmiMaskLessThan));
		itsTokenMaskOperations.insert(MaskOperMap::value_type(string(">="), kFmiMaskGreaterOrEqualThan));
		itsTokenMaskOperations.insert(MaskOperMap::value_type(string("<="), kFmiMaskLessOrEqualThan));

		itsBinaryOperator.insert(BinaOperMap::value_type(string("&&"), NFmiAreaMask::kAnd));
		itsBinaryOperator.insert(BinaOperMap::value_type(string("And"), NFmiAreaMask::kAnd));
		itsBinaryOperator.insert(BinaOperMap::value_type(string("and"), NFmiAreaMask::kAnd));
		itsBinaryOperator.insert(BinaOperMap::value_type(string("AND"), NFmiAreaMask::kAnd));
		itsBinaryOperator.insert(BinaOperMap::value_type(string("||"), NFmiAreaMask::kOr));
		itsBinaryOperator.insert(BinaOperMap::value_type(string("Or"), NFmiAreaMask::kOr));
		itsBinaryOperator.insert(BinaOperMap::value_type(string("or"), NFmiAreaMask::kOr));
		itsBinaryOperator.insert(BinaOperMap::value_type(string("OR"), NFmiAreaMask::kOr));

		itsCalculationOperations.insert(CalcOperMap::value_type(string("+"), NFmiAreaMask::Add));
		itsCalculationOperations.insert(CalcOperMap::value_type(string("-"), NFmiAreaMask::Sub));
		itsCalculationOperations.insert(CalcOperMap::value_type(string("/"), NFmiAreaMask::Div));
		itsCalculationOperations.insert(CalcOperMap::value_type(string("*"), NFmiAreaMask::Mul));
		itsCalculationOperations.insert(CalcOperMap::value_type(string("^"), NFmiAreaMask::Pow));
		itsCalculationOperations.insert(CalcOperMap::value_type(string("%"), NFmiAreaMask::Mod));


		itsTokenLevelNamesIdentsAndValues.insert(LevelMap::value_type(string("1000"), make_pair(kFmiPressureLevel, 1000)));
		itsTokenLevelNamesIdentsAndValues.insert(LevelMap::value_type(string("925"), make_pair(kFmiPressureLevel, 925)));
		itsTokenLevelNamesIdentsAndValues.insert(LevelMap::value_type(string("850"), make_pair(kFmiPressureLevel, 850)));
		itsTokenLevelNamesIdentsAndValues.insert(LevelMap::value_type(string("700"), make_pair(kFmiPressureLevel, 700)));
		itsTokenLevelNamesIdentsAndValues.insert(LevelMap::value_type(string("500"), make_pair(kFmiPressureLevel, 500)));
		itsTokenLevelNamesIdentsAndValues.insert(LevelMap::value_type(string("300"), make_pair(kFmiPressureLevel, 300)));

		// ****** sounding index funktiot  HUOM! ne k�sitell��n case insensitivein�!! *************************
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("lclsur"), kSoundingParLCLSur));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("lfcsur"), kSoundingParLFCSur));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("elsur"), kSoundingParELSur));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("lclheightsur"), kSoundingParLCLHeightSur));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("lfcheightsur"), kSoundingParLFCHeightSur));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("elheightsur"), kSoundingParELHeightSur));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("capesur"), kSoundingParCAPESur));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("cape03kmsur"), kSoundingParCAPE0_3kmSur));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("cinsur"), kSoundingParCINSur));

		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("lcl500m"), kSoundingParLCL500m));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("lfc500m"), kSoundingParLFC500m));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("el500m"), kSoundingParEL500m));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("lclheight500m"), kSoundingParLCLHeight500m));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("lfcheight500m"), kSoundingParLFCHeight500m));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("elheight500m"), kSoundingParELHeight500m));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("cape500m"), kSoundingParCAPE500m));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("cape03km500m"), kSoundingParCAPE0_3km500m));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("cin500m"), kSoundingParCIN500m));

		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("lclmostun"), kSoundingParLCLMostUn));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("lfcmostun"), kSoundingParLFCMostUn));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("elmostun"), kSoundingParELMostUn));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("lclheightmostun"), kSoundingParLCLHeightMostUn));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("lfcheightmostun"), kSoundingParLFCHeightMostUn));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("elheightmostun"), kSoundingParELHeightMostUn));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("capemostun"), kSoundingParCAPEMostUn));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("cape03kmmostun"), kSoundingParCAPE0_3kmMostUn));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("cinmostun"), kSoundingParCINMostUn));

		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("show"), kSoundingParSHOW));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("lift"), kSoundingParLIFT));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("kinx"), kSoundingParKINX));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("ctot"), kSoundingParCTOT));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("vtot"), kSoundingParVTOT));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("totl"), kSoundingParTOTL));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("bulkshear06km"), kSoundingParBS0_6km));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("bulkshear01km"), kSoundingParBS0_1km));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("srh03km"), kSoundingParSRH0_3km));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("srh01km"), kSoundingParSRH0_1km));
		itsTokenSoundingIndexFunctions.insert(SoundingIndexMap::value_type(string("thetae03km"), kSoundingParThetaE0_3km));
		// ****** sounding index funktiot *************************

		itsTokenCalculationBlockMarkers.push_back(string("{"));
		itsTokenCalculationBlockMarkers.push_back(string("}"));

		itsTokenMaskBlockMarkers.push_back(string("("));
		itsTokenMaskBlockMarkers.push_back(string(")"));

		itsTokenFunctions.insert(FunctionMap::value_type(string("avg"), NFmiAreaMask::Avg));
		itsTokenFunctions.insert(FunctionMap::value_type(string("min"), NFmiAreaMask::Min));
		itsTokenFunctions.insert(FunctionMap::value_type(string("max"), NFmiAreaMask::Max));
		itsTokenFunctions.insert(FunctionMap::value_type(string("sum"), NFmiAreaMask::Sum));
//		itsTokenFunctions.insert(FunctionMap::value_type(string("wavg"), NFmiAreaMask::WAvg));

		itsTokenThreeArgumentFunctions.insert(FunctionMap::value_type(string("sumt"), NFmiAreaMask::Sum));
		itsTokenThreeArgumentFunctions.insert(FunctionMap::value_type(string("maxt"), NFmiAreaMask::Max));
		itsTokenThreeArgumentFunctions.insert(FunctionMap::value_type(string("mint"), NFmiAreaMask::Min));
		itsTokenThreeArgumentFunctions.insert(FunctionMap::value_type(string("avgt"), NFmiAreaMask::Avg));

		itsTokenThreeArgumentFunctions.insert(FunctionMap::value_type(string("sumz"), NFmiAreaMask::Sum));
		itsTokenThreeArgumentFunctions.insert(FunctionMap::value_type(string("maxz"), NFmiAreaMask::Max));
		itsTokenThreeArgumentFunctions.insert(FunctionMap::value_type(string("minz"), NFmiAreaMask::Min));
		itsTokenThreeArgumentFunctions.insert(FunctionMap::value_type(string("avgz"), NFmiAreaMask::Avg));

		itsTokenThreeArgumentFunctions.insert(FunctionMap::value_type(string("maxh"), NFmiAreaMask::Max));
		itsTokenThreeArgumentFunctions.insert(FunctionMap::value_type(string("minh"), NFmiAreaMask::Min));

		itsTokenPeekFunctions.insert(std::make_pair(string("peekxy"), NFmiAreaMask::FunctionPeekXY));
		itsTokenPeekFunctions.insert(std::make_pair(string("peekxy2"), NFmiAreaMask::FunctionPeekXY2));

		itsTokenRampUpFunctions.push_back(string("RU"));
		itsTokenRampUpFunctions.push_back(string("Ru"));
		itsTokenRampUpFunctions.push_back(string("ru"));
		itsTokenRampDownFunctions.push_back(string("RD"));
		itsTokenRampDownFunctions.push_back(string("Rd"));
		itsTokenRampDownFunctions.push_back(string("rd"));
		itsTokenDoubleRampFunctions.push_back(string("DD"));
		itsTokenDoubleRampFunctions.push_back(string("Dd"));
		itsTokenDoubleRampFunctions.push_back(string("dd"));

		itsTokenRampFunctions.insert(itsTokenRampFunctions.end(), itsTokenRampUpFunctions.begin(), itsTokenRampUpFunctions.end());
		itsTokenRampFunctions.insert(itsTokenRampFunctions.end(), itsTokenRampDownFunctions.begin(), itsTokenRampDownFunctions.end());
		itsTokenRampFunctions.insert(itsTokenRampFunctions.end(), itsTokenDoubleRampFunctions.begin(), itsTokenDoubleRampFunctions.end());

		itsTokenMacroParamIdentifiers.push_back(string("result"));
		itsTokenMacroParamIdentifiers.push_back(string("Result"));
		itsTokenMacroParamIdentifiers.push_back(string("RESULT"));

		itsTokenDeltaZIdentifiers.push_back(string("deltaz"));
		itsTokenDeltaZIdentifiers.push_back(string("DeltaZ"));
		itsTokenDeltaZIdentifiers.push_back(string("DELTAZ"));

		itsMathFunctions.insert(MathFunctionMap::value_type(string("exp"), NFmiAreaMask::Exp));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("sqrt"), NFmiAreaMask::Sqrt));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("ln"), NFmiAreaMask::Log));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("lg"), NFmiAreaMask::Log10));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("sin"), NFmiAreaMask::Sin));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("cos"), NFmiAreaMask::Cos));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("tan"), NFmiAreaMask::Tan));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("sinh"), NFmiAreaMask::Sinh));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("cosh"), NFmiAreaMask::Cosh));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("tanh"), NFmiAreaMask::Tanh));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("asin"), NFmiAreaMask::Asin));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("acos"), NFmiAreaMask::Acos));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("atan"), NFmiAreaMask::Atan));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("ceil"), NFmiAreaMask::Ceil));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("floor"), NFmiAreaMask::Floor));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("round"), NFmiAreaMask::Round));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("abs"), NFmiAreaMask::Abs));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("rand"), NFmiAreaMask::Rand));

	}
}


