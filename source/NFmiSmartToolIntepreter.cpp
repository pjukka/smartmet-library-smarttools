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
#pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta
#endif

#include "NFmiSmartToolIntepreter.h"
#include "NFmiAreaMaskInfo.h"
#include "NFmiSmartToolCalculationSectionInfo.h"
#include "NFmiAreaMaskSectionInfo.h"
#include "NFmiCommentStripper.h"
#include "NFmiSmartToolCalculationInfo.h"
#include "NFmiValueString.h"
#include "NFmiLevelType.h"
#include "NFmiLevel.h"

#include <algorithm>
#include <utility>
#include <memory>
#include <sstream>
#include <functional>
#include <cctype>

using namespace std;

// globaali tarkistus luokka etsimään rivin lopetusta
struct EndOfLineSearcher
{
	template<typename T>
	bool operator()(T theChar)
	{
	  return(theChar == '\r' || theChar == '\n');
	}

};

//HUOM!!! vaarallinen luokka, pitää muistaa kutsua Clearia, muuten vuotaa!!!
NFmiSmartToolCalculationBlock::NFmiSmartToolCalculationBlock(void)
:itsFirstCalculationSectionInfo(new NFmiSmartToolCalculationSectionInfo)
,itsLastCalculationSectionInfo(new NFmiSmartToolCalculationSectionInfo)
,itsIfAreaMaskSectionInfo(new NFmiAreaMaskSectionInfo)
,itsIfCalculationSectionInfo(new NFmiSmartToolCalculationSectionInfo)
,itsElseIfAreaMaskSectionInfo(new NFmiAreaMaskSectionInfo)
,itsElseIfCalculationSectionInfo(new NFmiSmartToolCalculationSectionInfo)
,itsElseCalculationSectionInfo(new NFmiSmartToolCalculationSectionInfo)
{
}

NFmiSmartToolCalculationBlock::~NFmiSmartToolCalculationBlock(void)
{
}

// älä kutsu Clear:ia destruktorissa!!!!
void NFmiSmartToolCalculationBlock::Clear(void)
{
	delete itsFirstCalculationSectionInfo;
	itsFirstCalculationSectionInfo = 0;
	delete itsLastCalculationSectionInfo;
	itsLastCalculationSectionInfo = 0;
	delete itsIfAreaMaskSectionInfo;
	itsIfAreaMaskSectionInfo = 0;
	delete itsIfCalculationSectionInfo;
	itsIfCalculationSectionInfo = 0;
	delete itsElseIfAreaMaskSectionInfo;
	itsElseIfAreaMaskSectionInfo = 0;
	delete itsElseIfCalculationSectionInfo;
	itsElseIfCalculationSectionInfo = 0;
	delete itsElseCalculationSectionInfo;
	itsElseCalculationSectionInfo = 0;
	fElseSectionExist = false;
}

// Luokan staattisten dataosien pakollinen alustus.
bool NFmiSmartToolIntepreter::fTokensInitialized = false;
NFmiSmartToolIntepreter::ParamMap NFmiSmartToolIntepreter::itsTokenParameterNamesAndIds;
NFmiSmartToolIntepreter::ProducerMap NFmiSmartToolIntepreter::itsTokenProducerNamesAndIds;
std::vector<std::string> NFmiSmartToolIntepreter::itsTokenConditionalCommands;
std::vector<std::string> NFmiSmartToolIntepreter::itsTokenIfCommands;
std::vector<std::string> NFmiSmartToolIntepreter::itsTokenElseIfCommands;
std::vector<std::string> NFmiSmartToolIntepreter::itsTokenElseCommands;
std::vector<std::string> NFmiSmartToolIntepreter::itsTokenCalculationBlockMarkers;
std::vector<std::string> NFmiSmartToolIntepreter::itsTokenMaskBlockMarkers;
//std::vector<std::string> NFmiSmartToolIntepreter::itsTokenFunctions;
std::vector<std::string> NFmiSmartToolIntepreter::itsTokenRampUpFunctions;
std::vector<std::string> NFmiSmartToolIntepreter::itsTokenRampDownFunctions;
std::vector<std::string> NFmiSmartToolIntepreter::itsTokenDoubleRampFunctions;
std::vector<std::string> NFmiSmartToolIntepreter::itsTokenRampFunctions;
std::vector<std::string> NFmiSmartToolIntepreter::itsTokenTMFs;

NFmiSmartToolIntepreter::MaskOperMap NFmiSmartToolIntepreter::itsTokenMaskOperations;
NFmiSmartToolIntepreter::CalcOperMap NFmiSmartToolIntepreter::itsCalculationOperations;
NFmiSmartToolIntepreter::BinaOperMap NFmiSmartToolIntepreter::itsBinaryOperator;
NFmiSmartToolIntepreter::ParamMap NFmiSmartToolIntepreter::itsTokenStaticParameterNamesAndIds;
NFmiSmartToolIntepreter::ParamMap NFmiSmartToolIntepreter::itsTokenCalculatedParameterNamesAndIds;
NFmiSmartToolIntepreter::LevelMap NFmiSmartToolIntepreter::itsTokenLevelNamesIdentsAndValues;
NFmiSmartToolIntepreter::FunctionMap NFmiSmartToolIntepreter::itsTokenFunctions;
NFmiSmartToolIntepreter::MathFunctionMap NFmiSmartToolIntepreter::itsMathFunctions;

//--------------------------------------------------------
// Constructor/Destructor 
//--------------------------------------------------------
NFmiSmartToolIntepreter::NFmiSmartToolIntepreter(void)
:itsSmartToolCalculationBlocks()
{
	NFmiSmartToolIntepreter::InitTokens();
}
NFmiSmartToolIntepreter::~NFmiSmartToolIntepreter(void)
{
	Clear();
}
//--------------------------------------------------------
// Interpret 
//--------------------------------------------------------
// Tulkitsee annetun macro-tekstin. Erottelee eri calculationSectionit, mahdolliset ehto rakenteet ja niiden maskit ja rakentaa sen 
// mukaiset maski ja calculation infot, että SmartToolModifier osaa rakentaa oikeat systeemit (areamaskit ja lasku-oliot).
// Jos macrossa virhe, heittää poikkeuksen.
void NFmiSmartToolIntepreter::Interpret(const std::string &theMacroText)  throw (NFmiSmartToolIntepreter::Exception)
{
	Clear();
	itsTokenScriptVariableNames.clear(); // tyhjennetaan aluksi kaikki skripti muuttujat
	itsScriptVariableParamIdCounter = 1000000; // alustetaan isoksi, ettei mene päällekkäin todellisten param id::::ien kanssa
//	InitSectionInfos();
	SetMacroTexts(theMacroText);
	InitCheckOut();

	bool fGoOn = true;
	int index = 0;
	while(fGoOn)
	{
		index++;
		NFmiSmartToolCalculationBlock block;
		try
		{
			fGoOn = CheckoutPossibleNextCalculationBlock(&block);
			itsSmartToolCalculationBlocks.push_back(block);
			if(index > 10000)
				break; // huvin vuoksi laitoin iki loopin eston, tai jos yli 10000 lasku blokkia niin sorry!
		}
		catch(NFmiSmartToolIntepreter::Exception e)
		{
			block.Clear();
			for(unsigned int i=0; i<itsSmartToolCalculationBlocks.size(); i++)
				itsSmartToolCalculationBlocks[i].Clear();
			itsSmartToolCalculationBlocks.clear();
			throw e;
		}
	}
}

bool NFmiSmartToolIntepreter::CheckoutPossibleNextCalculationBlock(NFmiSmartToolCalculationBlock* theBlock)
{
//	CheckoutPossibleFirstCalculationSection();
	CheckoutPossibleNextCalculationSection(theBlock->itsFirstCalculationSectionInfo);
	if(CheckoutPossibleIfClauseSection(theBlock->itsIfAreaMaskSectionInfo))
	{
		if(!CheckoutPossibleNextCalculationSection(theBlock->itsIfCalculationSectionInfo))
			throw NFmiSmartToolIntepreter::Exception(string("IF-haaran jälkeen ei löytynyt laskuja."));
//		while(CheckoutPossibleElseIfClauseSection())
		if(CheckoutPossibleElseIfClauseSection(theBlock->itsElseIfAreaMaskSectionInfo))
		{
			if(!CheckoutPossibleNextCalculationSection(theBlock->itsElseIfCalculationSectionInfo))
				throw NFmiSmartToolIntepreter::Exception(string("ELSEIF-haaran jälkeen ei löytynyt laskuja."));
		}
		if((theBlock->fElseSectionExist = CheckoutPossibleElseClauseSection()) == true)
			if(!CheckoutPossibleNextCalculationSection(theBlock->itsElseCalculationSectionInfo))
				throw NFmiSmartToolIntepreter::Exception(string("ELSE-haaran jälkeen ei löytynyt laskuja."));
	}
//	CheckoutPossibleLastCalculationSection();
	CheckoutPossibleNextCalculationSection(theBlock->itsLastCalculationSectionInfo);
	if(itsCheckOutTextStartPosition == itsStrippedMacroText.end())
		return false;
	return true;
}

void NFmiSmartToolIntepreter::InitCheckOut(void)
{
	itsCheckOutSectionText = "";
	itsCheckOutTextStartPosition = itsStrippedMacroText.begin(); 
	itsCheckOutTextEndPosition = itsStrippedMacroText.begin(); // tällä ei vielä väliä
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

// Irroitetaan mahdollisia laskuoperaatio rivejä tulkkausta varten.
// laskun pitää olla yhdellä rivillä ja ne ovat muotoa:
// T = T + 1
// Eli pitää olla joku sijoitus johonkin parametriin.
// Palauttaa true jos löytyi jotai lasku-operaatioita.
// Päivittää myös luokan sisäisiä iteraattoreita macro-tekstiin.
bool NFmiSmartToolIntepreter::ExtractPossibleNextCalculationSection(void) throw (NFmiSmartToolIntepreter::Exception)
{
	itsCheckOutSectionText = "";
	string nextLine;
//	std::string::iterator eolPos = itsStrippedMacroText.begin();
	std::string::iterator eolPos = itsCheckOutTextStartPosition;
	eolPos = EatWhiteSpaces(eolPos);
	itsCheckOutTextStartPosition = itsCheckOutTextEndPosition = eolPos;
	if(*eolPos == '{') // jos blokin alkumerkki löytyi, haetaan sen loppua
	{
		eolPos = std::find(itsCheckOutTextStartPosition, itsStrippedMacroText.end(), '}');
		if(eolPos == itsStrippedMacroText.end())
			throw NFmiSmartToolIntepreter::Exception(string("Lasku blokin loppumerkkiä '}' ei löytynyt"));
		else
		{
			++itsCheckOutTextStartPosition; // hypätään alkumerkin ohi
			itsCheckOutSectionText = string(itsCheckOutTextStartPosition, eolPos);
			++eolPos; // hypätään blokin loppumerkin ohi
			itsCheckOutTextStartPosition = itsCheckOutTextEndPosition = eolPos;
		}
	}
	else // muuten etsitään laskurivejä rivi kerrallaan
	{
		do
		{
			eolPos = EatWhiteSpaces(eolPos);
			// en ole varma vielä näistä iteraattoreista, mitkä ovat tarpeellisisa ja mitkä turhia.
			itsCheckOutTextStartPosition = itsCheckOutTextEndPosition = eolPos;
			itsCheckOutSectionText += nextLine;
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
// 2. Pitää olla sijoitus-operaatio eli '='
bool NFmiSmartToolIntepreter::IsPossibleCalculationLine(const std::string &theTextLine)
{
	if(FindAnyFromText(theTextLine, itsTokenConditionalCommands))
			return false;
	if(theTextLine.find(string("=")) != string::npos)
		return true;

	if(std::find_if(theTextLine.begin(), theTextLine.end(), std::not1(std::ptr_fun(::isspace))) != theTextLine.end())
		throw NFmiSmartToolIntepreter::Exception(string("Riviltä löytyi jotain outoa tekstiä: \n") + theTextLine);
	return false;
}

// Yksinkertainen IF(mask) -rivin tarkistus:
// 1. aluksi pitää olla IF
// 2. ei saa olla ELSE/ELSEIF sanoja
// 3. Pitää olla ensin '('- ja sitten ')' -merkit
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
// 1. aluksi pitää olla ELSEIF
// 2. ei saa olla IF/ELSE sanoja
// 3. Pitää olla ensin '('- ja sitten ')' -merkit
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
bool NFmiSmartToolIntepreter::IsPossibleElseConditionLine(const std::string &theTextLine) throw (NFmiSmartToolIntepreter::Exception)
{
	stringstream sstream(theTextLine);
	string tmp;
	sstream >> tmp;
	if(!FindAnyFromText(tmp, itsTokenElseCommands))
			return false;
	sstream >> tmp;
	if(tmp.empty())
		return true;
	else
		throw NFmiSmartToolIntepreter::Exception(string("Else riviltä löytyi jotain outoa: \n") + theTextLine);
	return false;
}

static bool IsWordContinuing(char ch)
{
	if(::isalnum(ch) || ch == '_')
		return true;
	return false;
}

// Pitää olla kokonainen sana eli juuri ennen sanaa ei saa olla kirjaimia,numeroita tai _-merkkiä, eikä heti sen jälkeenkään.
bool NFmiSmartToolIntepreter::FindAnyFromText(const std::string &theText, const std::vector<std::string>& theSearchedItems)
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

// palauttaa true, jos if-lause löytyi
bool NFmiSmartToolIntepreter::CheckoutPossibleIfClauseSection(NFmiAreaMaskSectionInfo* theAreaMaskSectionInfo)
{
	if(ExtractPossibleIfClauseSection())
		return InterpretMaskSection(itsCheckOutSectionText, theAreaMaskSectionInfo);
	return false;
}


// IF-lause koostuu sanasta IF ja suluista, jotka sulkevat vähintään yhden mahd. useita ehtoja
// joita yhdistetään erilaisilla binääri operaatioilla. Koko jutun pitää olla yhdellä rivillä.
// Esim:
// IF(T<1)
// IF(T<1 && P>1012)
// IF(T<1 && P>1012 || RH>=95)
bool NFmiSmartToolIntepreter::ExtractPossibleIfClauseSection(void)
{
	itsCheckOutSectionText = "";
	std::string::iterator eolPos = itsCheckOutTextStartPosition;
	eolPos = EatWhiteSpaces(eolPos);
	// en ole varma vielä näistä iteraattoreista, mitkä ovat tarpeellisisa ja mitkä turhia.
	itsCheckOutTextStartPosition = itsCheckOutTextEndPosition = eolPos;

	// HUOM!!! NT4 ja W2000 ero:
	// Dialogista tulevasta stringissä rivin vaihto on W2000:ssa 13 (=CR) ja NT4:ssa 10 (=LF)
	// Pitää Testata molemmat tapaukset!!!!!!
	eolPos = std::find_if(itsCheckOutTextStartPosition, itsStrippedMacroText.end(), EndOfLineSearcher()); // W2000:ssa etsitään CR:ää
	string nextLine = string(itsCheckOutTextStartPosition, eolPos);
	if(IsPossibleIfConditionLine(nextLine))
	{
		itsCheckOutSectionText += nextLine;
		itsCheckOutTextStartPosition = eolPos; // asetetaan positio seuraavia tarkasteluja varten
		return true;
	}
	return false;
}

// En ymmärrä miten tämän saa toimimaan, miten saa käytettyä mem_fun-adapteria.
// VC:ssa näyttää olevan taas oma systeemi ja nyt mem_fun ei toimi
// standardin mukaisesti. En saa kutsua toimiaan.
template<typename memfunction>
bool NFmiSmartToolIntepreter::ExtractPossibleConditionalClauseSection(memfunction conditionalChecker)
{
	itsCheckOutSectionText = "";
	std::string::iterator eolPos = itsCheckOutTextStartPosition;
	eolPos = EatWhiteSpaces(eolPos);
	// en ole varma vielä näistä iteraattoreista, mitkä ovat tarpeellisisa ja mitkä turhia.
	itsCheckOutTextStartPosition = itsCheckOutTextEndPosition = eolPos;

	// HUOM!!! NT4 ja W2000 ero:
	// Dialogista tulevasta stringissä rivin vaihto on W2000:ssa 13 (=CR) ja NT4:ssa 10 (=LF)
	// Pitää Testata molemmat tapaukset!!!!!!
	eolPos = std::find_if(itsCheckOutTextStartPosition, itsStrippedMacroText.end(), EndOfLineSearcher()); // W2000:ssa etsitään CR:ää
	string nextLine = string(itsCheckOutTextStartPosition, eolPos);
	if(conditionalChecker(nextLine))
	{
		itsCheckOutSectionText += nextLine;
		itsCheckOutTextStartPosition = eolPos; // asetetaan positio seuraavia tarkasteluja varten
		return true;
	}
	return false;
}

// En saanut toimimaan mem_fun adapteri-systeemiä, joten joudun kopioimaan koodia
// ExtractPossibleIfClauseSection-metodista. Olisin halunnut käyttää yleistä
// funktiota, jolle annetaan parametrina yksi erottava metodi.
// palauttaa true, jos ifelse-lause löytyi
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
	// en ole varma vielä näistä iteraattoreista, mitkä ovat tarpeellisisa ja mitkä turhia.
	itsCheckOutTextStartPosition = itsCheckOutTextEndPosition = eolPos;

	// HUOM!!! NT4 ja W2000 ero:
	// Dialogista tulevasta stringissä rivin vaihto on W2000:ssa 13 (=CR) ja NT4:ssa 10 (=LF)
	// Pitää Testata molemmat tapaukset!!!!!!
	eolPos = std::find_if(itsCheckOutTextStartPosition, itsStrippedMacroText.end(), EndOfLineSearcher()); // W2000:ssa etsitään CR:ää
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
	// en ole varma vielä näistä iteraattoreista, mitkä ovat tarpeellisisa ja mitkä turhia.
	itsCheckOutTextStartPosition = itsCheckOutTextEndPosition = eolPos;

	// HUOM!!! NT4 ja W2000 ero:
	// Dialogista tulevasta stringissä rivin vaihto on W2000:ssa 13 (=CR) ja NT4:ssa 10 (=LF)
	// Pitää Testata molemmat tapaukset!!!!!!
	eolPos = std::find_if(itsCheckOutTextStartPosition, itsStrippedMacroText.end(), EndOfLineSearcher()); // W2000:ssa etsitään CR:ää
	string nextLine = string(itsCheckOutTextStartPosition, eolPos);
	if(IsPossibleElseConditionLine(nextLine))
	{
		itsCheckOutSectionText += nextLine;
		itsCheckOutTextStartPosition = eolPos; // asetetaan positio seuraavia tarkasteluja varten
		return true;
	}
	return false;
}

bool NFmiSmartToolIntepreter::CheckoutPossibleNextCalculationSection(NFmiSmartToolCalculationSectionInfo* theSectionInfo)
{
	if(ExtractPossibleNextCalculationSection())
		return InterpretCalculationSection(itsCheckOutSectionText, theSectionInfo);
	return false;
}

void NFmiSmartToolIntepreter::SetMacroTexts(const std::string &theMacroText) throw (NFmiSmartToolIntepreter::Exception)
{
	itsMacroText = theMacroText;
	NFmiCommentStripper commentStripper;
	commentStripper.SetString(theMacroText);
	if(commentStripper.Strip())
	{
		itsStrippedMacroText = commentStripper.GetString();
	}
	else
	{
		throw NFmiSmartToolIntepreter::Exception(string("Kommenttien poisto makrosta ei onnistunut: ") + commentStripper.GetMessage());
	}
}
//--------------------------------------------------------
// InterpretMaskSection 
//--------------------------------------------------------
// Koko Section on yhdellä rivillä!!!
// Esim:
// IF(T<1)
// IF(T<1 && P>1012)
// IF(T<1 && P>1012 || RH>=95)
bool NFmiSmartToolIntepreter::InterpretMaskSection(const std::string &theMaskSectorText, NFmiAreaMaskSectionInfo* theAreaMaskSectionInfo) throw (NFmiSmartToolIntepreter::Exception)
{
	theAreaMaskSectionInfo->SetCalculationText(theMaskSectorText);
	string maskText(theMaskSectorText);
	exp_ptr = maskText.begin();
	exp_end = maskText.end();

	string tmp;
	if(GetToken()) // luetaan komento tähän esim. if, else jne
	{
		tmp = token; 
		if(FindAnyFromText(tmp, itsTokenConditionalCommands))
		{
			if(GetToken()) // luetaan komento tähän esim. if, else jne
			{
				tmp = token; 
				if(tmp == "(") // pitää löytyä alku sulku
				{
					string::size_type startPos = exp_ptr - maskText.begin();
					string::size_type endPos = maskText.rfind(string(")"));
					string::iterator it1 = maskText.begin() + startPos;
					string::iterator it2 = maskText.begin() + endPos;
					if(endPos != string::npos) // pitää löytyä lopetus sulku
					{
						string finalText(it1, it2);
						return InterpretMasks(finalText, theAreaMaskSectionInfo);
					}
				}
			}
		}
	}
	throw NFmiSmartToolIntepreter::Exception(string("Maski-ehto lauseessa oli vikaa: \n" + maskText));
}	

// tässä on enää ehtolauseen sulkujen sisältävä oleva teksti esim.
// T<1
// T<1 && P>1012
// T<1 && P>1012 || RH>=95
// HUOM!!!! Tämä vuotaa exceptionin yhteydessä, pino ei tuhoa AreaMaskInfoja!!!!! (korjaa)
bool NFmiSmartToolIntepreter::InterpretMasks(std::string &theMaskSectionText, NFmiAreaMaskSectionInfo* theAreaMaskSectionInfo) throw (NFmiSmartToolIntepreter::Exception)
{
	string maskText(theMaskSectionText);
	exp_ptr = maskText.begin();
	exp_end = maskText.end();

	string tmp;
	for(; GetToken(); )
	{
		tmp = token; // luetaan muuttuja/vakio/funktio tai mikä lie
		NFmiAreaMaskInfo *maskInfo = new NFmiAreaMaskInfo;
		auto_ptr<NFmiAreaMaskInfo> maskInfoPtr(maskInfo);
		InterpretToken(tmp, maskInfo); // ei saa antaa auto_ptr-otusta tässä, muuten se menettää omistuksen!
		theAreaMaskSectionInfo->Add(maskInfoPtr.release()); // auto_ptr menettää omistuksen tässä
	}

	// minimissään erilaisia lasku elementtejä pitää olla vahintäin 3 (esim. T > 15)
	if(theAreaMaskSectionInfo->GetAreaMaskInfoVector()->size() >= 3)
		return true;
	throw NFmiSmartToolIntepreter::Exception(string("Maski-ehto lause oli puutteellinen: " + theMaskSectionText));
}

NFmiAreaMaskInfo* NFmiSmartToolIntepreter::CreateWantedAreaMaskInfo(const std::string &theMaskSectionText, queue<NFmiAreaMaskInfo *> &theMaskQueue) throw (NFmiSmartToolIntepreter::Exception)
{
	NFmiAreaMaskInfo *maskInfo1 = theMaskQueue.front(); // tässä pitää olla muuttuja esim T, p, jne.
	auto_ptr<NFmiAreaMaskInfo> maskInfoPtr1(maskInfo1); // tuhoaa automaattisesti maskInfon
	theMaskQueue.pop();
	NFmiAreaMaskInfo *maskInfo2 = theMaskQueue.front(); // tässä pitää olla vertailu operator esim. >, < jne.
	auto_ptr<NFmiAreaMaskInfo> maskInfoPtr2(maskInfo2); // tuhoaa automaattisesti maskInfon
	theMaskQueue.pop();
	NFmiAreaMaskInfo *maskInfo3 = theMaskQueue.front(); // tässä pitää olla vakio esim. 2.5
	auto_ptr<NFmiAreaMaskInfo> maskInfoPtr3(maskInfo3); // tuhoaa automaattisesti maskInfon
	// ************************************************************************
	// HUOM!! tämä pitää muuttaa niin, että voisi olla vaikka seuraavia maskeja
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
	throw NFmiSmartToolIntepreter::Exception(string("Ei ollut laillinen maski: ") + theMaskSectionText);
}

//--------------------------------------------------------
// InterpretCalculationSection 
//--------------------------------------------------------
// Jokainen rivi tekstissä on mahdollinen laskuoperaatio esim.
// T = T + 1
// Jokaiselta riviltä pitää siis lötyä muuttuja johon sijoitetaan ja jotain laskuja
// palauttaa true, jos löytyi laskuja ja false jos ei.
bool NFmiSmartToolIntepreter::InterpretCalculationSection(std::string &theCalculationSectiontext, NFmiSmartToolCalculationSectionInfo* theSectionInfo)  throw (NFmiSmartToolIntepreter::Exception)
{
	std::string::iterator pos = theCalculationSectiontext.begin();
	std::string::iterator end = theCalculationSectiontext.end();
	NFmiSmartToolCalculationInfo *calculationInfo = 0;
	do
	{
		calculationInfo = 0; // pitää nollata joka kierroksella!
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

// Rivissä on mahdollinen laskuoperaatio esim.
// T = T + 1
// Riviltä pitää siis lötyä muuttuja johon sijoitetaan ja jotain laskuja
// HUOM! Oletus aluksi, pitää olla scape aina välissä, yksinkertaistaa ohjelmointia.
// HUOM!! Tämä vuotaa, jos tapahtuu exception!!!!!!

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
		throw NFmiSmartToolIntepreter::Exception(string("Sijoitus operaattori '=' väärässä paikassa"));
	NFmiAreaMaskInfo *variableInfo = 0;
	do
	{
		tmp = tmp2 = ""; // tyhjennetään varmuuden vuoksi
		sStream >> tmp; // luetaan muuttuja/vakio/funktio tai mikä lie
		if(tmp == "")
			break;
		variableInfo = new NFmiAreaMaskInfo;
		InterpretVariable(tmp, variableInfo);

		sStream >> tmp2; // luetaan operaattori (+-/*)
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

NFmiSmartToolCalculationInfo* NFmiSmartToolIntepreter::InterpretCalculationLine(const std::string &theCalculationLineText) throw (NFmiSmartToolIntepreter::Exception)
{
	string calculationLineText(theCalculationLineText);
	NFmiSmartToolCalculationInfo *calculationInfo = new NFmiSmartToolCalculationInfo;
	calculationInfo->SetCalculationText(theCalculationLineText);
	auto_ptr<NFmiSmartToolCalculationInfo> calculationInfoPtr(calculationInfo); // tuhoaa automaattisesti esim. exceptionin yhteydessä 

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
		auto_ptr<NFmiAreaMaskInfo> assignedVariablePtr(assignedVariable); // tuhoaa automaattisesti esim. exceptionin yhteydessä 
		InterpretVariable(tmp, assignedVariable, fNewScriptVariable);  // ei saa antaa auto_ptr-otustä tässä, muuten se menettää omistuksen!
		if(!(assignedVariable->GetDataType() == NFmiInfoData::kEditable || assignedVariable->GetDataType() == NFmiInfoData::kScriptVariableData))
			throw NFmiSmartToolIntepreter::Exception(string("Sijoitusta ei voi tehdä kuin editoitavaan dataan tai skripti-muuttujaan: \n") + calculationLineText);
		calculationInfo->SetResultDataInfo(assignedVariablePtr.release());// auto_ptr menettää omistuksen tässä

		GetToken(); // luetaan sijoitus operaattori =
		if(string(token) != string("="))
			throw NFmiSmartToolIntepreter::Exception(string("Laskurivillä ei ollut sijoitus operaattoria '=': " + theCalculationLineText));
		NFmiAreaMaskInfo *variableInfo = 0;
		for(; GetToken(); )
		{
			tmp = token; // luetaan muuttuja/vakio/funktio tai mikä lie
			variableInfo = new NFmiAreaMaskInfo;
			auto_ptr<NFmiAreaMaskInfo> variableInfoPtr(variableInfo);
			InterpretToken(tmp, variableInfo); // ei saa antaa auto_ptr-otustä tässä, muuten se menettää omistuksen!
			calculationInfo->AddCalculationInfo(variableInfoPtr.release()); // auto_ptr menettää omistuksen tässä
		}

		if(calculationInfo->GetCalculationOperandInfoVector()->empty())
			return 0;
	}
	calculationInfoPtr.release();
	return calculationInfo;
}

// GetToken ja IsDelim otettu H. Schilbertin  C++: the Complete Refeference third ed.
// jouduin muuttamaan niitä tähän ympäristöön.
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

	if(strchr("+-*/%^=(){}<>&|!", *exp_ptr))
	{
/*
		if(strchr("+-", *exp_ptr) && isdigit(exp_ptr[1])) // etumerkilliset vakiot otetaan tässä
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
		if(*exp_ptr == '>' || *exp_ptr == '=') // tässä halutaan ottaa huomioon >=, <=, !=, <> ja == vertailu operaattorit
		{
			*temp++ = *exp_ptr++;
		}
		if(*exp_ptr == '&') // tässä halutaan ottaa huomioon && vertailu operaattori
		{
			*temp++ = *exp_ptr++;
		}
		if(*exp_ptr == '|') // tässä halutaan ottaa huomioon || vertailu operaattori
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
{
	if(strchr(" +-/*%^=(){}<>!", c) || c==9 || c=='\r' || c=='\t' || c==0) // lisäsin muutaman delimiter merkin
		return true;
	return false;
}

NFmiAreaMask::CalculationOperator NFmiSmartToolIntepreter::InterpretCalculationOperator(const std::string &theOperatorText) throw (NFmiSmartToolIntepreter::Exception)
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
		throw NFmiSmartToolIntepreter::Exception(string("Lasku operaattori oli outo: ") + theOperatorText);
}

void NFmiSmartToolIntepreter::InterpretToken(const std::string &theTokenText, NFmiAreaMaskInfo *theMaskInfo) throw (NFmiSmartToolIntepreter::Exception)
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
		throw NFmiSmartToolIntepreter::Exception(string("Sana oli outo tai se oli oudossa paikassa: ") + theTokenText);
	}
}

// HUOM!!!! Muuta käyttämään itsCalculationOperations-mappia!!!!!!!!!!
void NFmiSmartToolIntepreter::InterpretDelimiter(const std::string &theDelimText, NFmiAreaMaskInfo *theMaskInfo) throw (NFmiSmartToolIntepreter::Exception)
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
	else
		throw NFmiSmartToolIntepreter::Exception(string("Lasku operaattori oli outo: ") + theDelimText);

}

// Tulkitsee annetun muuttujan. Hoitelee myös tuottajan, jos se on annettu, esim.
// T T_HIR jne.
// Voi olla myös vakio tai funktio systeemi.
// HUOM!!! Ei vielä tuottajan handlausta.
// HUOM!!! Ei hoida vielä vakioita tai funktio systeemejä.
void NFmiSmartToolIntepreter::InterpretVariable(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo, bool fNewScriptVariable) throw (NFmiSmartToolIntepreter::Exception)
{
	theMaskInfo->SetMaskText(theVariableText);
	string paramNameOnly;
	string levelNameOnly;
	string producerNameOnly;
	bool levelExist = false;
	bool producerExist = false;

	// tutkitaan ensin onko mahdollisesti variable-muuttuja, jolloin voimme sallia _-merkin käytön muuttujissa
	if(InterpretPossibleScriptVariable(theVariableText, theMaskInfo, fNewScriptVariable))
		return ;

	CheckVariableString(theVariableText, paramNameOnly,
						  levelExist, levelNameOnly,
						  producerExist, producerNameOnly);

	bool origWanted = producerExist ? IsProducerOrig(producerNameOnly) : false; // lisäsin ehdon, koska boundchecker valitti tyhjän stringin vertailua IsProducerOrig-metodissa.

//	bool levelInVariableName = ExtractParamAndLevel(theVariableText, &paramNameOnly, &levelNameOnly);
	if(InterpretVariableCheckTokens(theVariableText, theMaskInfo, origWanted, levelExist, producerExist, paramNameOnly, levelNameOnly, producerNameOnly))
	{
		if(fNewScriptVariable)
			throw NFmiSmartToolIntepreter::Exception(string("Varattua sanaa yritettiin käyttää \"skripti muuttujana\": " + theVariableText));
		else
			return ;
	}

	throw NFmiSmartToolIntepreter::Exception(string("Outo muuttuja laskussa: " + theVariableText));
}

bool NFmiSmartToolIntepreter::InterpretPossibleScriptVariable(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo, bool fNewScriptVariable) throw (NFmiSmartToolIntepreter::Exception)
{
	ScriptVariableMap::iterator it = itsTokenScriptVariableNames.find(theVariableText);
	if(it != itsTokenScriptVariableNames.end() && fNewScriptVariable) // var x käytetty uudestaan, esitellään muuttujat vain kerran
		throw NFmiSmartToolIntepreter::Exception(string("\"skripti muuttujaa\" yritettiin alustaa uudestaan, käytä var-sanaa vain kerran yhtä muuttujaa kohden: " + theVariableText));
	else if(it != itsTokenScriptVariableNames.end()) // muuttujaa x käytetty uudestaan
	{
		NFmiParam param((*it).second, (*it).first);
		NFmiProducer producer; // tällä ei ole väliä
		NFmiDataIdent dataIdent(param, producer);
		theMaskInfo->SetOperationType(NFmiAreaMask::InfoVariable);
		theMaskInfo->SetDataIdent(dataIdent);
		theMaskInfo->SetDataType(NFmiInfoData::kScriptVariableData);
		return true;
	}
	else if(fNewScriptVariable) // var x, eli 1. alustus
	{
		NFmiParam param(itsScriptVariableParamIdCounter, theVariableText);
		itsTokenScriptVariableNames.insert(ScriptVariableMap::value_type(theVariableText, itsScriptVariableParamIdCounter));
		itsScriptVariableParamIdCounter++; // kasvatetaan seuraavaa uutta muutujaa varten
		NFmiProducer producer; // tällä ei ole väliä
		NFmiDataIdent dataIdent(param, producer);
		theMaskInfo->SetOperationType(NFmiAreaMask::InfoVariable);
		theMaskInfo->SetDataIdent(dataIdent);
		theMaskInfo->SetDataType(NFmiInfoData::kScriptVariableData);
		return true;
	}
	return false;
}

bool NFmiSmartToolIntepreter::InterpretVariableCheckTokens(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo, bool fOrigWanted, bool fLevelExist, bool fProducerExist, const std::string &theParamNameOnly, const std::string &theLevelNameOnly, const std::string &theProducerNameOnly)
{
	if(fLevelExist && fProducerExist) // kokeillaan ensin, löytyykö param+level+producer
	{
		if(FindParamAndLevelAndProducerAndSetMaskInfo(theParamNameOnly, theLevelNameOnly, theProducerNameOnly, NFmiAreaMask::InfoVariable, fOrigWanted ? NFmiInfoData::kCopyOfEdited : NFmiInfoData::kViewable, theMaskInfo))
			return true;
	}
	else if(fLevelExist) // kokeillaan ensin, löytyykö param+level+producer
	{
		if(FindParamAndLevelAndSetMaskInfo(theParamNameOnly, theLevelNameOnly, NFmiAreaMask::InfoVariable, NFmiInfoData::kViewable, theMaskInfo))
			return true;
	}
	else if(fProducerExist) // kokeillaan ensin, löytyykö param+level+producer
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

	if(IsVariableFunction(theVariableText, theMaskInfo))
		return true;

	if(IsVariableMathFunction(theVariableText, theMaskInfo))
		return true;

	if(IsVariableRampFunction(theVariableText, theMaskInfo))
		return true;

	if(IsVariableTMF(theVariableText, theMaskInfo))
		return true;

	if(IsVariableBinaryOperator(theVariableText, theMaskInfo)) // tämä on and ja or tapausten käsittelyyn
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

// Saa parametrina kokonaisen parametri stringin, joka voi sisältää
// parametrin lisäksi myös levelin ja producerin.
// Teksti voi siis olla vaikka: T, T_850, T_Ec, T_850_Ec
// Eri osat asetetaan parametreina annetuille stringeille ja exist
// parametrit asetetaan myös vastaavasti, paitsi itse paramStringille
// ei ole muutujaa, koska se on pakollinen.
void NFmiSmartToolIntepreter::CheckVariableString(const std::string &theVariableText, std::string &theParamText,
												  bool &fLevelExist, std::string &theLevelText,
												  bool &fProducerExist, std::string &theProducerText) throw (NFmiSmartToolIntepreter::Exception)
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
			throw NFmiSmartToolIntepreter::Exception(string("Muuttujassa alaviivan jälkeen ei level eikä tuottaja  tietoa: \n") + theVariableText);

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
					throw NFmiSmartToolIntepreter::Exception(string("Muuttuja tekstissä level kahdesti: \n") + theVariableText);
			}
			else if(IsPossiblyProducerItem(tmp, itsTokenProducerNamesAndIds))
			{
				if(fProducerExist == false)
				{
					fProducerExist = true;
					theProducerText = tmp;
				}
				else
					throw NFmiSmartToolIntepreter::Exception(string("Muuttuja tekstissä tuottaja kahdesti: \n") + theVariableText);
			}
			else
				throw NFmiSmartToolIntepreter::Exception(string("Muuttujassa alaviivan jälkeen ei level eikä tuottaja  tietoa: \n") + theVariableText);
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

template<typename mapType>
bool NFmiSmartToolIntepreter::IsInMap(mapType& theMap, const std::string &theSearchedItem)
{
    typename mapType::iterator it = theMap.find(theSearchedItem);
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
		NFmiLevel level(GetPossibleLevelInfo(theLevelText));
		theMaskInfo->SetLevel(&level);
		return true;
	}
	return false;
}

bool NFmiSmartToolIntepreter::FindParamAndProducerAndSetMaskInfo(const std::string &theVariableText, const std::string &theProducerText, NFmiAreaMask::CalculationOperationType theOperType, NFmiInfoData::Type theDataType, NFmiAreaMaskInfo *theMaskInfo)
{
	NFmiProducer producer(GetPossibleProducerInfo(theProducerText));
	if(FindParamAndSetMaskInfo(theVariableText, itsTokenParameterNamesAndIds, theOperType, theDataType, theMaskInfo, producer))
		return true;
	return false;
}

bool NFmiSmartToolIntepreter::FindParamAndLevelAndProducerAndSetMaskInfo(const std::string &theVariableText, const std::string &theLevelText,const std::string &theProducerText, NFmiAreaMask::CalculationOperationType theOperType, NFmiInfoData::Type theDataType, NFmiAreaMaskInfo *theMaskInfo)
{
	NFmiProducer producer(GetPossibleProducerInfo(theProducerText));
	if(FindParamAndSetMaskInfo(theVariableText, itsTokenParameterNamesAndIds, theOperType, theDataType, theMaskInfo, producer))
	{
		NFmiLevel level(GetPossibleLevelInfo(theLevelText));
		theMaskInfo->SetLevel(&level);
		return true;
	}
	return false;
}

// Level tekstin (850, 500 jne.) luodaan NFmiLevel-otus.
NFmiLevel NFmiSmartToolIntepreter::GetPossibleLevelInfo(const std::string &theLevelText) throw (NFmiSmartToolIntepreter::Exception)
{
	NFmiLevel level;
	LevelMap::iterator it = itsTokenLevelNamesIdentsAndValues.find(theLevelText);
	if(it != itsTokenLevelNamesIdentsAndValues.end())
		level = NFmiLevel((*it).second.first, (*it).first, (*it).second.second);
	else if(!GetLevelFromVariableById(theLevelText, level))
		throw NFmiSmartToolIntepreter::Exception(string("Level tietoa ei saatu rakennettua (vika ohjelmassa): \n") + theLevelText);
	return level;
}

// Tuottaja tekstin (Ec, Hir jne.) luodaan NFmiProducer-otus.
NFmiProducer NFmiSmartToolIntepreter::GetPossibleProducerInfo(const std::string &theProducerText) throw (NFmiSmartToolIntepreter::Exception)
{
	NFmiProducer producer;
	ProducerMap::iterator it = itsTokenProducerNamesAndIds.find(theProducerText);
	if(it != itsTokenProducerNamesAndIds.end())
	{
		producer = NFmiProducer((*it).second, (*it).first);
		return producer;
	}
	else if(!GetProducerFromVariableById(theProducerText, producer))
		throw NFmiSmartToolIntepreter::Exception(string("Producer tietoa ei saatu rakennettua (vika ohjelmassa): \n") + theProducerText);
	return producer;
}

bool NFmiSmartToolIntepreter::GetParamFromVariable(const std::string &theVariableText, ParamMap& theParamMap, NFmiParam &theParam, bool &fUseWildDataType)
{
	ParamMap::iterator it = theParamMap.find(theVariableText);
	if(it == theParamMap.end())
	{
		if(GetParamFromVariableById(theVariableText, theParam))
			fUseWildDataType = true; // paridta käytettäessä pitää asettaa data tyyppi 'villiksi' toistaiseksi
		else
			return false;
	}
	else
		theParam = NFmiParam((*it).second, (*it).first);
	return true;
}

// tutkii alkaako annettu sana "par"-osiolla ja sitä seuraavalla numerolla
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

// tutkii alkaako annettu sana "prod"-osiolla ja sitä seuraavalla numerolla
// esim. prod230 tai PROD1 jne.
bool NFmiSmartToolIntepreter::GetProducerFromVariableById(const std::string &theVariableText, NFmiProducer &theProducer)
{
	if(IsWantedStart(theVariableText, "prod"))
	{
		NFmiValueString numericPart(theVariableText.substr(4));
		if(numericPart.IsNumeric())
		{
			theProducer = NFmiProducer((long)numericPart, theVariableText);
			return true;
		}
	}
	return false;
}

// tutkii alkaako annettu sana "lev"-osiolla ja sitä seuraavalla numerolla
// esim. par100 tai LEV850 jne.
bool NFmiSmartToolIntepreter::GetLevelFromVariableById(const std::string &theVariableText, NFmiLevel &theLevel)
{
	if(IsWantedStart(theVariableText, "lev"))
	{
		NFmiValueString numericPart(theVariableText.substr(3));
		if(numericPart.IsNumeric())
		{
			// pitaisi tunnistaa level tyyppi arvosta kait, nyt oletus että painepinta
			theLevel = NFmiLevel(kFmiPressureLevel, theVariableText, (long)numericPart);
			return true;
		}
	}
	return false;
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
		else if(theProducer.GetIdent() == 999) // tämä 999 on viritys, mutta se on määrätty helpdatainfo.dat tiedostossa kepa-datan feikki id numeroksi. Oikeaa id:tä ei voi käyttää, koska se on sama kuin editoitavalla datalla.
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
	return false;
}

bool NFmiSmartToolIntepreter::IsVariableTMF(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo)
{
	if(FindAnyFromText(theVariableText, itsTokenTMFs))
	{
		theMaskInfo->SetOperationType(NFmiAreaMask::ModifyFactor);
		return true;
	}
	return false;
}

bool NFmiSmartToolIntepreter::IsVariableMathFunction(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo) throw (NFmiSmartToolIntepreter::Exception)
{
	MathFunctionMap::iterator it = itsMathFunctions.find(theVariableText);
	if(it != itsMathFunctions.end())
	{
		string tmp;
		if(GetToken())
		{
			tmp = token; // luetaan muuttuja/vakio/funktio tai mikä lie
			if(tmp == string("(")) // etsitään fuktion aloitus sulkua (lopetus sulku tulee sitten aikanaan, välissä voi olla mitä vain!)
			{
				theMaskInfo->SetOperationType(NFmiAreaMask::MathFunctionStart);
				theMaskInfo->SetMathFunctionType((*it).second);
				return true;
			}
		}
		throw NFmiSmartToolIntepreter::Exception(string("Matemaattisen funktion parametrit väärin: ") + theVariableText);
	}
	return false;
}

// Nämä ovat muotoa aika-'integroinnin' yhteydessä:
// MIN(T -1 1)
// eli 1. funktion nimi, sulku auki, parametri, aloitus aikaoffset, lopetus aikaoffset ja lopuksi suliku kiinni.
// Tai alue-'integroinnin' yhteydessä:
// MIN(T -1 -1 1 1)
// eli 1. funktion nimi, sulku auki, parametri, aloitus x ja y paikkaoffset, lopetus x ja y paikkaoffset ja lopuksi suliku kiinni.
bool NFmiSmartToolIntepreter::IsVariableFunction(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo) throw (NFmiSmartToolIntepreter::Exception)
{
	FunctionMap::iterator it = itsTokenFunctions.find(theVariableText);
	if(it != itsTokenFunctions.end())
	{
		theMaskInfo->SetFunctionType((*it).second);
		string tmp;
		vector<pair<string, types> > tokens;
		int i;
		for(i=0; i<7 && GetToken(); i++) // maksimissaan 7 kertaa
		{
			tmp = token; // luetaan muuttuja/vakio/funktio tai mikä lie
			tmp = HandlePossibleUnaryMarkers(tmp);
			tokens.push_back(std::make_pair(tmp, tok_type));
			if(tmp == string(")")) // etsitään lopetus sulkua
				break;
		}
		// ensin tutkitaan, onko kyseessä aikaintegrointi eli 5 'tokenia'
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
		// sitten tutkitaan, onko kyseessä aikaintegrointi eli 7 'tokenia'
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
		throw NFmiSmartToolIntepreter::Exception(string("Integrointi funktion parametrit väärin: ") + theVariableText);
	}
	return false;
}

std::string NFmiSmartToolIntepreter::HandlePossibleUnaryMarkers(const std::string &theCurrentString)
{
	string returnStr(theCurrentString);
	if(returnStr == string("-"))
	{
		GetToken();
		returnStr += token; // lisätään '-'-etumerkki ja seuraava token ja katsotaan mitä syntyy
	}
	if(returnStr == string("+"))
		GetToken(); // +-merkki ohitetaan merkityksettömänä
	return returnStr;
}

// Nämä ovat muotoa:
// RU(T 1 3)
// eli 1. maskin/funktion nimi, sulku auki, parametri, alkuarvo, loppuarvo ja lopuksi suliku kiinni.
bool NFmiSmartToolIntepreter::IsVariableRampFunction(const std::string &theVariableText, NFmiAreaMaskInfo *theMaskInfo) throw (NFmiSmartToolIntepreter::Exception)
{
	if(FindAnyFromText(theVariableText, itsTokenRampFunctions))
	{
		string tmp;
		vector<pair<string, types> > tokens;
		int i;
		for(i=0; i<5 && GetToken(); i++)
		{
			tmp = token; // luetaan muuttuja/vakio/funktio tai mikä lie
			tmp = HandlePossibleUnaryMarkers(tmp);
			tokens.push_back(std::make_pair(tmp, tok_type));
			if(tmp == string(")")) // etsitään lopetus sulkua
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
		throw NFmiSmartToolIntepreter::Exception(string("Ramppimaskin parametrit väärin: ") + theVariableText);
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
//--------------------------------------------------------
// ExtractFirstCalculationSection 
//--------------------------------------------------------
// Metodi saa parametrina stripatun macro-tekstin ja etsii siitä 1. calculation sectionin.
// Jos löytyy, palauta sen positio, jos ei löydy, palauta string.end().
// Parametrina tulee aloitus positio (merkitsee enemmän muissa vastaavissa metodeissa).
// Tee myös seuraavat metodit:
// ExtractNextCalculationSection
// ExtractIfMaskSection
// ExtractNextElseIfMaskSection
// ExtractElseCalculationSection

#ifdef UNIX
std::string::const_iterator NFmiSmartToolIntepreter::ExtractFirstCalculationSection(const std::string &theMacroText, std::string::iterator theStartPosition)
#else
std::string::iterator NFmiSmartToolIntepreter::ExtractFirstCalculationSection(const std::string &theMacroText, std::string::iterator theStartPosition)
#endif
{
#ifdef UNIX
  return theMacroText.end();
#else
  return const_cast<std::string::iterator>(theMacroText.end());
#endif
}

void NFmiSmartToolIntepreter::Clear(void)
{
	int size = itsSmartToolCalculationBlocks.size();
	for(int i=0; i<size; i++)
		itsSmartToolCalculationBlocks[i].Clear();
}

void NFmiSmartToolIntepreter::InitTokens(void)
{
	if(!NFmiSmartToolIntepreter::fTokensInitialized)
	{
		NFmiSmartToolIntepreter::fTokensInitialized = true;

		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("T"), kFmiTemperature));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("t"), kFmiTemperature));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("P"), kFmiPressure));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("p"), kFmiPressure));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("RH"), kFmiHumidity));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Rh"), kFmiHumidity));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("rh"), kFmiHumidity));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("THETAW"), kFmiPseudoAdiabaticPotentialTemperature));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("ThetaW"), kFmiPseudoAdiabaticPotentialTemperature));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("thetaW"), kFmiPseudoAdiabaticPotentialTemperature));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Thetaw"), kFmiPseudoAdiabaticPotentialTemperature));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("thetaw"), kFmiPseudoAdiabaticPotentialTemperature));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("KIND"), kFmiKIndex));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("KInd"), kFmiKIndex));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Kind"), kFmiKIndex));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("kind"), kFmiKIndex));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("DP"), kFmiDewPoint));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Dp"), kFmiDewPoint));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("dp"), kFmiDewPoint));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("LRAD"), kFmiRadiationLW));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("LRad"), kFmiRadiationLW));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Lrad"), kFmiRadiationLW));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("lrad"), kFmiRadiationLW));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("SRAD"), kFmiRadiationGlobal));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("SRad"), kFmiRadiationGlobal));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Srad"), kFmiRadiationGlobal));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("srad"), kFmiRadiationGlobal));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("WS"), kFmiWindSpeedMS));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Ws"), kFmiWindSpeedMS));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("ws"), kFmiWindSpeedMS));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("WD"), kFmiWindDirection));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Wd"), kFmiWindDirection));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("wd"), kFmiWindDirection));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("N"), kFmiTotalCloudCover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("n"), kFmiTotalCloudCover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("CL"), kFmiLowCloudCover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Cl"), kFmiLowCloudCover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("cl"), kFmiLowCloudCover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("CM"), kFmiMediumCloudCover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Cm"), kFmiMediumCloudCover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("cm"), kFmiMediumCloudCover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("CH"), kFmiHighCloudCover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Ch"), kFmiHighCloudCover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("ch"), kFmiHighCloudCover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("RR"), kFmiPrecipitation1h));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Rr"), kFmiPrecipitation1h));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("rr"), kFmiPrecipitation1h));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("pref"), kFmiPrecipitationForm));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Pref"), kFmiPrecipitationForm));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("PreF"), kFmiPrecipitationForm));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("PREF"), kFmiPrecipitationForm));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("pret"), kFmiPrecipitationType));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Pret"), kFmiPrecipitationType));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("PreT"), kFmiPrecipitationType));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("PRET"), kFmiPrecipitationType));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("THUND"), kFmiProbabilityThunderstorm));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Thund"), kFmiProbabilityThunderstorm));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("thund"), kFmiProbabilityThunderstorm));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FOG"), kFmiFogIntensity));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fog"), kFmiFogIntensity));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fog"), kFmiFogIntensity));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("HSADE"), kFmiWeatherSymbol1));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("HSade"), kFmiWeatherSymbol1));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Hsade"), kFmiWeatherSymbol1));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("hsade"), kFmiWeatherSymbol1));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("HESSAA"), kFmiWeatherSymbol3));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Hessaa"), kFmiWeatherSymbol3));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("hessaa"), kFmiWeatherSymbol3));

		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL1BASE"), kFmi_FL_1_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL1Base"), kFmi_FL_1_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fl1Base"), kFmi_FL_1_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl1base"), kFmi_FL_1_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL1TOP"), kFmi_FL_1_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL1Top"), kFmi_FL_1_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fl1Top"), kFmi_FL_1_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl1top"), kFmi_FL_1_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL1COVER"), kFmi_FL_1_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL1Cover"), kFmi_FL_1_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fl1Cover"), kFmi_FL_1_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl1cover"), kFmi_FL_1_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL1CLOUDTYPE"), kFmi_FL_1_CloudType));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL1CloudType"), kFmi_FL_1_CloudType));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fl1CloudType"), kFmi_FL_1_CloudType));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl1cloudtype"), kFmi_FL_1_CloudType));

		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL2BASE"), kFmi_FL_2_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL2Base"), kFmi_FL_2_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fl2Base"), kFmi_FL_2_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl2base"), kFmi_FL_2_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL2TOP"), kFmi_FL_2_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL2Top"), kFmi_FL_2_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fl2Top"), kFmi_FL_2_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl2top"), kFmi_FL_2_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL2COVER"), kFmi_FL_2_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL2Cover"), kFmi_FL_2_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fl2Cover"), kFmi_FL_2_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl2cover"), kFmi_FL_2_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL2CLOUDTYPE"), kFmi_FL_2_CloudType));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL2CloudType"), kFmi_FL_2_CloudType));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fl2CloudType"), kFmi_FL_2_CloudType));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl2cloudtype"), kFmi_FL_2_CloudType));

		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL3BASE"), kFmi_FL_3_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL3Base"), kFmi_FL_3_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fl3Base"), kFmi_FL_3_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl3base"), kFmi_FL_3_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL3TOP"), kFmi_FL_3_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL3Top"), kFmi_FL_3_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fl3Top"), kFmi_FL_3_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl3top"), kFmi_FL_3_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL3COVER"), kFmi_FL_3_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL3Cover"), kFmi_FL_3_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fl3Cover"), kFmi_FL_3_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl3cover"), kFmi_FL_3_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL3CLOUDTYPE"), kFmi_FL_3_CloudType));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL3CloudType"), kFmi_FL_3_CloudType));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fl3CloudType"), kFmi_FL_3_CloudType));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl3cloudtype"), kFmi_FL_3_CloudType));

		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL4BASE"), kFmi_FL_4_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL4Base"), kFmi_FL_4_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fl4Base"), kFmi_FL_4_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl4base"), kFmi_FL_4_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL4TOP"), kFmi_FL_4_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL4Top"), kFmi_FL_4_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fl4Top"), kFmi_FL_4_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl4top"), kFmi_FL_4_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL4COVER"), kFmi_FL_4_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL4Cover"), kFmi_FL_4_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fl4Cover"), kFmi_FL_4_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl4cover"), kFmi_FL_4_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL4CLOUDTYPE"), kFmi_FL_4_CloudType));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL4CloudType"), kFmi_FL_4_CloudType));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fl4CloudType"), kFmi_FL_4_CloudType));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl4cloudtype"), kFmi_FL_4_CloudType));

		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL5BASE"), kFmi_FL_5_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL5Base"), kFmi_FL_5_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fl5Base"), kFmi_FL_5_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl5base"), kFmi_FL_5_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL5TOP"), kFmi_FL_5_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL5Top"), kFmi_FL_5_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fl5Top"), kFmi_FL_5_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl5top"), kFmi_FL_5_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL5COVER"), kFmi_FL_5_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL5Cover"), kFmi_FL_5_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fl5Cover"), kFmi_FL_5_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl5cover"), kFmi_FL_5_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL5CLOUDTYPE"), kFmi_FL_5_CloudType));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL5CloudType"), kFmi_FL_5_CloudType));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fl5CloudType"), kFmi_FL_5_CloudType));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl5cloudtype"), kFmi_FL_5_CloudType));

		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL6BASE"), kFmi_FL_6_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL6Base"), kFmi_FL_6_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fl6Base"), kFmi_FL_6_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl6base"), kFmi_FL_6_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL6TOP"), kFmi_FL_6_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL6Top"), kFmi_FL_6_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fl6Top"), kFmi_FL_6_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl6top"), kFmi_FL_6_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL6COVER"), kFmi_FL_6_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL6Cover"), kFmi_FL_6_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fl6Cover"), kFmi_FL_6_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl6cover"), kFmi_FL_6_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL6CLOUDTYPE"), kFmi_FL_6_CloudType));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL6CloudType"), kFmi_FL_6_CloudType));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fl6CloudType"), kFmi_FL_6_CloudType));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl6cloudtype"), kFmi_FL_6_CloudType));

		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL7BASE"), kFmi_FL_7_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL7Base"), kFmi_FL_7_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fl7Base"), kFmi_FL_7_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl7base"), kFmi_FL_7_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL7TOP"), kFmi_FL_7_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL7Top"), kFmi_FL_7_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fl7Top"), kFmi_FL_7_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl7top"), kFmi_FL_7_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL7COVER"), kFmi_FL_7_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL7Cover"), kFmi_FL_7_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fl7Cover"), kFmi_FL_7_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl7cover"), kFmi_FL_7_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL7CLOUDTYPE"), kFmi_FL_7_CloudType));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL7CloudType"), kFmi_FL_7_CloudType));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fl7CloudType"), kFmi_FL_7_CloudType));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl7cloudtype"), kFmi_FL_7_CloudType));

		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL8BASE"), kFmi_FL_8_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL8Base"), kFmi_FL_8_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fl8Base"), kFmi_FL_8_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl8base"), kFmi_FL_8_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL8TOP"), kFmi_FL_8_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL8Top"), kFmi_FL_8_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fl8Top"), kFmi_FL_8_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl8top"), kFmi_FL_8_Top));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL8COVER"), kFmi_FL_8_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL8Cover"), kFmi_FL_8_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fl8Cover"), kFmi_FL_8_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl8cover"), kFmi_FL_8_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL8CLOUDTYPE"), kFmi_FL_8_CloudType));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FL8CloudType"), kFmi_FL_8_CloudType));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("Fl8CloudType"), kFmi_FL_8_CloudType));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("fl8cloudtype"), kFmi_FL_8_CloudType));

		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FLCBBASE"), kFmi_FL_Cb_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FlCbBase"), kFmi_FL_Cb_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("flcbbase"), kFmi_FL_Cb_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FLCBCOVER"), kFmi_FL_Cb_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FlCbCover"), kFmi_FL_Cb_Cover));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("flcbcover"), kFmi_FL_Cb_Cover));

		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FLMINBASE"), kFmi_FL_Min_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FlMinBase"), kFmi_FL_Min_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("flminbase"), kFmi_FL_Min_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FLMAXBASE"), kFmi_FL_Max_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("FlMaxBase"), kFmi_FL_Max_Base));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("flmaxbase"), kFmi_FL_Max_Base));

		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("AVIVIS"), kFmiAviationVisibility));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("AviVis"), kFmiAviationVisibility));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("avivis"), kFmiAviationVisibility));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("VERVIS"), kFmiVerticalVisibility));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("VerVis"), kFmiVerticalVisibility));
		itsTokenParameterNamesAndIds.insert(ParamMap::value_type(string("vervis"), kFmiVerticalVisibility));
		
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

		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("TOPO"), kFmiTopoGraf));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("Topo"), kFmiTopoGraf));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("topo"), kFmiTopoGraf));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("SLOPE"), kFmiTopoSlope));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("Slope"), kFmiTopoSlope));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("slope"), kFmiTopoSlope));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("SLOPEDIR"), kFmiTopoAzimuth));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("SlopeDir"), kFmiTopoAzimuth));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("slopedir"), kFmiTopoAzimuth));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("DISTSEA"), kFmiTopoDistanceToSea));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("DistSea"), kFmiTopoDistanceToSea));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("distsea"), kFmiTopoDistanceToSea));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("DIRSEA"), kFmiTopoDirectionToSea));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("DirSea"), kFmiTopoDirectionToSea));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("dirsea"), kFmiTopoDirectionToSea));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("DISTLAND"), kFmiTopoDistanceToLand));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("DistLand"), kFmiTopoDistanceToLand));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("distland"), kFmiTopoDistanceToLand));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("DIRLAND"), kFmiTopoDirectionToLand));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("DirLand"), kFmiTopoDirectionToLand));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("dirland"), kFmiTopoDirectionToLand));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("LANDSEEMASK"), kFmiLandSeaMask));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("LandSeeMask"), kFmiLandSeaMask));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("landseemask"), kFmiLandSeaMask));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("RELTOPO"), kFmiTopoRelativeHeight));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("RelTopo"), kFmiTopoRelativeHeight));
		itsTokenStaticParameterNamesAndIds.insert(ParamMap::value_type(string("reltopo"), kFmiTopoRelativeHeight));

		itsTokenCalculatedParameterNamesAndIds.insert(ParamMap::value_type(string("LAT"), kFmiLatitude));
		itsTokenCalculatedParameterNamesAndIds.insert(ParamMap::value_type(string("Lat"), kFmiLatitude));
		itsTokenCalculatedParameterNamesAndIds.insert(ParamMap::value_type(string("lat"), kFmiLatitude));
		itsTokenCalculatedParameterNamesAndIds.insert(ParamMap::value_type(string("LON"), kFmiLongitude));
		itsTokenCalculatedParameterNamesAndIds.insert(ParamMap::value_type(string("Lon"), kFmiLongitude));
		itsTokenCalculatedParameterNamesAndIds.insert(ParamMap::value_type(string("lon"), kFmiLongitude));
		itsTokenCalculatedParameterNamesAndIds.insert(ParamMap::value_type(string("EANGLE"), kFmiElevationAngle));
		itsTokenCalculatedParameterNamesAndIds.insert(ParamMap::value_type(string("EAngle"), kFmiElevationAngle));
		itsTokenCalculatedParameterNamesAndIds.insert(ParamMap::value_type(string("eangle"), kFmiElevationAngle));
		itsTokenCalculatedParameterNamesAndIds.insert(ParamMap::value_type(string("JDAY"), kFmiDay)); // julian day oikeasti
		itsTokenCalculatedParameterNamesAndIds.insert(ParamMap::value_type(string("JDay"), kFmiDay)); // julian day oikeasti
		itsTokenCalculatedParameterNamesAndIds.insert(ParamMap::value_type(string("Jday"), kFmiDay)); // julian day oikeasti
		itsTokenCalculatedParameterNamesAndIds.insert(ParamMap::value_type(string("jday"), kFmiDay)); // julian day oikeasti
		itsTokenCalculatedParameterNamesAndIds.insert(ParamMap::value_type(string("LHOUR"), kFmiHour)); // local hour oikeasti
		itsTokenCalculatedParameterNamesAndIds.insert(ParamMap::value_type(string("LHour"), kFmiHour)); // local hour oikeasti
		itsTokenCalculatedParameterNamesAndIds.insert(ParamMap::value_type(string("Lhour"), kFmiHour)); // local hour oikeasti
		itsTokenCalculatedParameterNamesAndIds.insert(ParamMap::value_type(string("lhour"), kFmiHour)); // local hour oikeasti
		itsTokenCalculatedParameterNamesAndIds.insert(ParamMap::value_type(string("FHOUR"), kFmiForecastPeriod)); // forecast hour pikaviritys forperiodia käytetty, koska ei ollut valmista parametria
		itsTokenCalculatedParameterNamesAndIds.insert(ParamMap::value_type(string("FHour"), kFmiForecastPeriod)); // forecast hour pikaviritys forperiodia käytetty, koska ei ollut valmista parametria
		itsTokenCalculatedParameterNamesAndIds.insert(ParamMap::value_type(string("Fhour"), kFmiForecastPeriod)); // forecast hour pikaviritys forperiodia käytetty, koska ei ollut valmista parametria
		itsTokenCalculatedParameterNamesAndIds.insert(ParamMap::value_type(string("fhour"), kFmiForecastPeriod)); // forecast hour pikaviritys forperiodia käytetty, koska ei ollut valmista parametria

		itsTokenProducerNamesAndIds.insert(ProducerMap::value_type(string("HIR"), kFmiMTAHIRMESO));
		itsTokenProducerNamesAndIds.insert(ProducerMap::value_type(string("Hir"), kFmiMTAHIRMESO));
		itsTokenProducerNamesAndIds.insert(ProducerMap::value_type(string("hir"), kFmiMTAHIRMESO));
		itsTokenProducerNamesAndIds.insert(ProducerMap::value_type(string("EC"), kFmiMTAECMWF));
		itsTokenProducerNamesAndIds.insert(ProducerMap::value_type(string("Ec"), kFmiMTAECMWF));
		itsTokenProducerNamesAndIds.insert(ProducerMap::value_type(string("ec"), kFmiMTAECMWF));
//		itsTokenProducerNamesAndIds.insert(ProducerMap::value_type(string("SYNOP"), kFmiSYNOP));
//		itsTokenProducerNamesAndIds.insert(ProducerMap::value_type(string("Synop"), kFmiSYNOP));
//		itsTokenProducerNamesAndIds.insert(ProducerMap::value_type(string("synop"), kFmiSYNOP));
		itsTokenProducerNamesAndIds.insert(ProducerMap::value_type(string("MET"), static_cast<FmiProducerName>(999))); // 999 on määritelty helpdatainfo.dat tiedostossa viimeksi editoidun datan kohdalla (feikki tuottaja id:n kohdalla)
		itsTokenProducerNamesAndIds.insert(ProducerMap::value_type(string("Met"), static_cast<FmiProducerName>(999)));
		itsTokenProducerNamesAndIds.insert(ProducerMap::value_type(string("met"), static_cast<FmiProducerName>(999)));
		itsTokenProducerNamesAndIds.insert(ProducerMap::value_type(string("ORIG"), kFmiMETEOR)); // tuottaja id:llä ei ole väliä tässä oikeasti
		itsTokenProducerNamesAndIds.insert(ProducerMap::value_type(string("Orig"), kFmiMETEOR));
		itsTokenProducerNamesAndIds.insert(ProducerMap::value_type(string("orig"), kFmiMETEOR));

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


		itsTokenLevelNamesIdentsAndValues.insert(LevelMap::value_type(string("925"), make_pair(kFmiPressureLevel, 925)));
		itsTokenLevelNamesIdentsAndValues.insert(LevelMap::value_type(string("850"), make_pair(kFmiPressureLevel, 850)));
		itsTokenLevelNamesIdentsAndValues.insert(LevelMap::value_type(string("700"), make_pair(kFmiPressureLevel, 700)));
		itsTokenLevelNamesIdentsAndValues.insert(LevelMap::value_type(string("500"), make_pair(kFmiPressureLevel, 500)));
		itsTokenLevelNamesIdentsAndValues.insert(LevelMap::value_type(string("300"), make_pair(kFmiPressureLevel, 300)));

		
		itsTokenCalculationBlockMarkers.push_back(string("{"));
		itsTokenCalculationBlockMarkers.push_back(string("}"));

		itsTokenMaskBlockMarkers.push_back(string("("));
		itsTokenMaskBlockMarkers.push_back(string(")"));

		itsTokenFunctions.insert(FunctionMap::value_type(string("AVG"), NFmiAreaMask::Avg));
		itsTokenFunctions.insert(FunctionMap::value_type(string("Avg"), NFmiAreaMask::Avg));
		itsTokenFunctions.insert(FunctionMap::value_type(string("avg"), NFmiAreaMask::Avg));
		itsTokenFunctions.insert(FunctionMap::value_type(string("MIN"), NFmiAreaMask::Min));
		itsTokenFunctions.insert(FunctionMap::value_type(string("Min"), NFmiAreaMask::Min));
		itsTokenFunctions.insert(FunctionMap::value_type(string("min"), NFmiAreaMask::Min));
		itsTokenFunctions.insert(FunctionMap::value_type(string("MAX"), NFmiAreaMask::Max));
		itsTokenFunctions.insert(FunctionMap::value_type(string("Max"), NFmiAreaMask::Max));
		itsTokenFunctions.insert(FunctionMap::value_type(string("max"), NFmiAreaMask::Max));
		itsTokenFunctions.insert(FunctionMap::value_type(string("SUM"), NFmiAreaMask::Sum));
		itsTokenFunctions.insert(FunctionMap::value_type(string("Sum"), NFmiAreaMask::Sum));
		itsTokenFunctions.insert(FunctionMap::value_type(string("sum"), NFmiAreaMask::Sum));
//		itsTokenFunctions.insert(FunctionMap::value_type(string("WAVG"), NFmiAreaMask::WAvg));
//		itsTokenFunctions.insert(FunctionMap::value_type(string("WAvg"), NFmiAreaMask::WAvg));
//		itsTokenFunctions.insert(FunctionMap::value_type(string("Wavg"), NFmiAreaMask::WAvg));
//		itsTokenFunctions.insert(FunctionMap::value_type(string("wavg"), NFmiAreaMask::WAvg));

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

		itsTokenTMFs.push_back(string("TMF"));
		itsTokenTMFs.push_back(string("Tmf"));
		itsTokenTMFs.push_back(string("tmf"));

		itsMathFunctions.insert(MathFunctionMap::value_type(string("EXP"), NFmiAreaMask::Exp));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("Exp"), NFmiAreaMask::Exp));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("exp"), NFmiAreaMask::Exp));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("SQRT"), NFmiAreaMask::Sqrt));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("Sqrt"), NFmiAreaMask::Sqrt));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("sqrt"), NFmiAreaMask::Sqrt));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("LN"), NFmiAreaMask::Log));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("Ln"), NFmiAreaMask::Log));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("ln"), NFmiAreaMask::Log));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("LG"), NFmiAreaMask::Log10));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("Lg"), NFmiAreaMask::Log10));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("lg"), NFmiAreaMask::Log10));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("SIN"), NFmiAreaMask::Sin));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("Sin"), NFmiAreaMask::Sin));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("sin"), NFmiAreaMask::Sin));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("COS"), NFmiAreaMask::Cos));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("Cos"), NFmiAreaMask::Cos));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("cos"), NFmiAreaMask::Cos));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("TAN"), NFmiAreaMask::Tan));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("Tan"), NFmiAreaMask::Tan));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("tan"), NFmiAreaMask::Tan));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("SINH"), NFmiAreaMask::Sinh));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("Sinh"), NFmiAreaMask::Sinh));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("sinh"), NFmiAreaMask::Sinh));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("COSH"), NFmiAreaMask::Cosh));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("Cosh"), NFmiAreaMask::Cosh));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("cosh"), NFmiAreaMask::Cosh));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("TANH"), NFmiAreaMask::Tanh));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("Tanh"), NFmiAreaMask::Tanh));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("tanh"), NFmiAreaMask::Tanh));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("ASIN"), NFmiAreaMask::Asin));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("ASin"), NFmiAreaMask::Asin));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("Asin"), NFmiAreaMask::Asin));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("asin"), NFmiAreaMask::Asin));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("ACOS"), NFmiAreaMask::Acos));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("ACos"), NFmiAreaMask::Acos));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("Acos"), NFmiAreaMask::Acos));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("acos"), NFmiAreaMask::Acos));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("ATAN"), NFmiAreaMask::Atan));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("ATan"), NFmiAreaMask::Atan));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("Atan"), NFmiAreaMask::Atan));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("atan"), NFmiAreaMask::Atan));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("CEIL"), NFmiAreaMask::Ceil));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("Ceil"), NFmiAreaMask::Ceil));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("ceil"), NFmiAreaMask::Ceil));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("FLOOR"), NFmiAreaMask::Floor));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("Floor"), NFmiAreaMask::Floor));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("floor"), NFmiAreaMask::Floor));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("ROUND"), NFmiAreaMask::Round));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("Round"), NFmiAreaMask::Round));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("round"), NFmiAreaMask::Round));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("ABS"), NFmiAreaMask::Abs));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("Abs"), NFmiAreaMask::Abs));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("abs"), NFmiAreaMask::Abs));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("RAND"), NFmiAreaMask::Rand));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("Rand"), NFmiAreaMask::Rand));
		itsMathFunctions.insert(MathFunctionMap::value_type(string("rand"), NFmiAreaMask::Rand));

	}
}


