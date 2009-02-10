//**********************************************************
// C++ Class Name : NFmiDrawParamList
// ---------------------------------------------------------
// Filetype: (SOURCE)
// Filepath: D:/projekti/GDPro/GDTemp/NFmiDrawParamList.cpp
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
//  Creation Date  : Thur - Jan 28, 1999
//
//
//  Description:
//   Voi olla että tässä riittää suoraan uuden
//   template lista luokan ilmentymä.
//
//  Change Log:
//
//**********************************************************
#ifdef _MSC_VER
#pragma warning(disable : 4786 4512) // poistaa n kpl VC++ kääntäjän "liian pitkä tyyppi nimi" varoitusta
#endif

#include "NFmiDrawParamList.h"
#include "NFmiDrawParam.h"
#include <algorithm>
#include <list>

//--------------------------------------------------------
// Constructor  NFmiDrawParamList
//--------------------------------------------------------
NFmiDrawParamList::NFmiDrawParamList(void):
itsList(),
itsIter(itsList.Start()),
fDirtyList(true),
fHasBorrowedParams(false)
{
}

//--------------------------------------------------------
// Destructor  ~NFmiDrawParamList
//--------------------------------------------------------
NFmiDrawParamList::~NFmiDrawParamList(void)
{
	Clear(true);
}

//--------------------------------------------------------
// Add
//--------------------------------------------------------
bool NFmiDrawParamList::Add (NFmiDrawParam* theParam)
{
	if(theParam)
	{
		if(theParam->DataType() == NFmiInfoData::kSatelData) // kSatelData tarkoittaa oikeasti vain kuva muotoista dataa eli ei välttämättä satelliitti dataa siis
		{ // satelliitti-kuvat pitää laitaa aina 1. näytölle, koska ne peittävät muut
			itsList.AddStart(theParam);
			fDirtyList = true;
			return true;
		}
		else if(itsList.AddEnd(theParam))
		{
			fDirtyList = true;
			return true;
		}
	}
	return false;
}
//--------------------------------------------------------
// Add
//--------------------------------------------------------
bool NFmiDrawParamList::Add(NFmiDrawParam * theParam, unsigned long theIndex)
{
   if(theParam)
   {
	   NFmiPtrList < NFmiDrawParam > :: Iterator iter = itsList.Index(theIndex);
	   if(iter.AddBefore(theParam))
	   {
			fDirtyList = true;
			return true;
	   }
   }
   return false;
}

// ottaa toiselta listalta parametrit lainaan (tekee kopiot, mutta merkitää lainatuiksi)
void NFmiDrawParamList::BorrowParams(NFmiDrawParamList &theList)
{
	for(theList.Reset(); theList.Next(); )
	{
		NFmiDrawParam *tmp = new NFmiDrawParam(*(theList.Current()));
		tmp->BorrowedParam(true);
		Add(tmp);
		HasBorrowedParams(true);
	}
}

// poistaa listasta lainatut parametrit
void NFmiDrawParamList::ClearBorrowedParams(void)
{
	for(Reset(); Next(); )
	{
		if(Current()->BorrowedParam())
			Remove(true);
	}
	HasBorrowedParams(false);
}

//--------------------------------------------------------
// Current
//--------------------------------------------------------
NFmiDrawParam* NFmiDrawParamList::Current (void)
{
	return itsIter.CurrentPtr();
}
//--------------------------------------------------------
// Reset
//--------------------------------------------------------
bool NFmiDrawParamList::Reset (void)
{
   itsIter = itsList.Start();
   return true;
}
//--------------------------------------------------------
// Next
//--------------------------------------------------------
bool NFmiDrawParamList::Next (void)
{
   return itsIter.Next();
}
//--------------------------------------------------------
// Previous
//--------------------------------------------------------
bool NFmiDrawParamList::Previous (void)
{
   return itsIter.Previous();
}
//--------------------------------------------------------
// Clear
//--------------------------------------------------------
void NFmiDrawParamList::Clear(bool fDeleteData)
{
	itsList.Clear(fDeleteData);
	fDirtyList = true;
}
//--------------------------------------------------------
// Remove
//--------------------------------------------------------
bool NFmiDrawParamList::Remove(bool fDeleteData)
{
   if(itsIter.Remove(fDeleteData))
   {
		fDirtyList = true;
		return true;
   }
   return false;
}
//--------------------------------------------------------
// Index
//--------------------------------------------------------
bool NFmiDrawParamList::Index(unsigned long index)
{
	itsIter = itsList.Index(index);
	if(Current())
		return true;
	return false;
}
//--------------------------------------------------------
// Find
//--------------------------------------------------------
bool NFmiDrawParamList::Find(NFmiDrawParam* item)
{
	if(item)
	{
		itsIter = itsList.Find(item);
		if(Current())
			return true;
		Reset();
	}
	return false;
}

//--------------------------------------------------------
// Update
//--------------------------------------------------------
void NFmiDrawParamList::Update(void)
{

}

void NFmiDrawParamList::HideAllParams(bool newState)
{
	for(Reset(); Next();)
		Current()->HideParam(newState);
}

// asettaa kaikkien listalla olevien tilan 'ei editoitaviksi' (toisin päin ei ole järkevää tehdä)
void NFmiDrawParamList::DisableEditing(void)
{
	for(Reset(); Next();)
		Current()->EditParam(false);
}

void NFmiDrawParamList::DeactivateAll(void)
{
	for(Reset(); Next();)
		Current()->Activate(false);
}
/*
bool NFmiDrawParamList::Find(const NFmiDataIdent& theParam, bool fIgnoreProducer)
{
	for(Reset(); Next();)
	{
		if(fIgnoreProducer)
		{
			if(*(Current()->Param().GetParam()) == *(theParam.GetParam()))
				return true;
		}
		else
			if(Current()->Param() == theParam)
				return true;
	}
	return false;
}
*/
/*
bool NFmiDrawParamList::Find(const NFmiDataIdent& theParam, const NFmiLevel* theLevel)
{
	for(Reset(); Next();)
	{
		NFmiSmartInfo* info = Current()->Info();
		if(info && info->Param() == theParam)
		{
			if(!theLevel && info->SizeLevels() <= 1)
				return true;
			if(theLevel && info->Level() && (*(theLevel) == *(info->Level())))
				return true;
		}
	}
	return false;
}
*/

bool NFmiDrawParamList::Find(const NFmiDataIdent& theParam, const NFmiLevel* theLevel, NFmiInfoData::Type theDataType, bool fUseOnlyParamId)
{
	for(Reset(); Next();)
	{
		NFmiDrawParam * drawParam = Current();
		if(theParam.GetParamIdent() == 997) // pirun synop-parametrille pitää taas tehdä virityksiä
		{
			if(drawParam->Param() == theParam)
				return true;
		}
		else
		{
			if(fUseOnlyParamId ? drawParam->Param().GetParamIdent() == theParam.GetParamIdent() : drawParam->Param() == theParam)
			{
				if(theDataType == NFmiInfoData::kAnyData || drawParam->DataType() == theDataType)
				{
					if(theLevel == 0 && drawParam->Level().LevelType() == 0)
						return true;
					if(drawParam->Level().LevelType() == 0 && (theLevel->LevelType() == kFmiAnyLevelType || theLevel->LevelType() == kFmiMeanSeaLevel))
						return true; // tämä case tulee kun nykyään tehdään pinta parametreja
					if(theLevel && (*(theLevel) == drawParam->Level()))
						return true;
				}
			}
		}
	}
	return false;
}

// Poistaa listalta kaikki halutun tuottajan ja mahd. annetun levelin parametrit.
// Ei poista niitä parametreja, jotka ovat theParamIdsNotRemoved-listalla. Löy-
// tynyt paramId poistetaan theParamIdsNotRemoved-listalta, että niitä ei
// lisättäisi myöhemmin tähän listaan.
void NFmiDrawParamList::Clear(const NFmiProducer& theProducer, checkedVector<int>& theParamIdsNotRemoved, NFmiLevel* theLevel, bool fDeleteData)
{
	std::list<int> tmpParIdList;
//	std::copy(theParamIdsNotRemoved.begin(), theParamIdsNotRemoved.end(), tmpParIdList.begin());
	int size = theParamIdsNotRemoved.size();
	for(int i=0; i<size; i++)
		tmpParIdList.push_back(theParamIdsNotRemoved[i]);
//	int size2 = tmpParIdList.size();
	std::list<int>::iterator it;
	if(theLevel)
	{
		for(Reset(); Next();)
		{
			if(*(Current()->Param().GetProducer()) == theProducer)
			{
				if(Current()->Level() == *theLevel)
				{
					it = std::find(tmpParIdList.begin(), tmpParIdList.end(), static_cast<int>(Current()->Param().GetParamIdent()));
					if(it == tmpParIdList.end())
						Remove(fDeleteData);
					else
						tmpParIdList.erase(it);
				}
			}
		}
	}
	else
	{
		for(Reset(); Next();)
		{
			if(*(Current()->Param().GetProducer()) == theProducer)
			{
				if(Current()->Level().LevelType() == 0) // jos drawParamilla on joku 'oikea' leveli, ei kosketa siihen, koska nyt käsittelemme pinta datoja
				{
					it = std::find(tmpParIdList.begin(), tmpParIdList.end(), static_cast<int>(Current()->Param().GetParamIdent()));
					if(it == tmpParIdList.end())
						Remove(fDeleteData);
					else
						tmpParIdList.erase(it);
				}
			}
		}
	}
	theParamIdsNotRemoved.clear();
//	std::copy(tmpParIdList.begin(), tmpParIdList.end(), theParamIdsNotRemoved.begin());
	for(it=tmpParIdList.begin(); it != tmpParIdList.end(); ++it)
		theParamIdsNotRemoved.push_back(*it);
}
/*
struct ParamIdLevelSearcher
{
	ParamIdLevelSearcher(std::pair<int, NFmiLevel>& thePair)
	:itsSearchPair(thePair)
	{
	}

	bool operator(std::pair<int, NFmiLevel>& theListPair)
	{
		if(itsSearchPair.first == theListPair.first && itsSearchPair.second == theListPair.second)
			return true;
		return false;
	}

	std::pair<int, NFmiLevel> itsSearchPair;
}
*/
/*!
 * Poistaa listalta kaikki halutun tuottajan ja mahd. annetun levelin parametrit.
 * Ei poista niitä parametreja, jotka ovat theParamIdsNotRemoved-listalla. Löy-
 * tynyt paramId poistetaan theParamIdsNotRemoved-listalta, että niitä ei
 * lisättäisi myöhemmin tähän listaan.
 */
void NFmiDrawParamList::Clear(const NFmiProducer& theProducer, std::list<std::pair<int, NFmiLevel> >& theParamIdsAndLevelsNotRemoved, bool fDeleteData)
{
	std::list<std::pair<int, NFmiLevel> >::iterator it;
	for(Reset(); Next();)
	{
		if(*(Current()->Param().GetProducer()) == theProducer)
		{
			if(Current()->Level().LevelType() == kFmiPressureLevel) // koska kyse vain painepinta parametreista, pitää level tyypin olla painepinta
			{
				std::pair<int, NFmiLevel> tmp(Current()->Param().GetParamIdent(), Current()->Level());
				it = std::find(theParamIdsAndLevelsNotRemoved.begin(), theParamIdsAndLevelsNotRemoved.end(), tmp);
				if(it == theParamIdsAndLevelsNotRemoved.end())
					Remove(fDeleteData);
				else
					theParamIdsAndLevelsNotRemoved.erase(it);
			}
		}
	}
}

/*!
 * Poistaa halutun macroParamin listalta. Eli DrawParamin tyyppi pitää olla oikein
 * Ja sitten tutkitaan drawParamin nimen lyhennystä, joka on macroparametrien tunnus.
 * Palauttaa true, jos poistettiin mitään, muuten false.
 */
bool NFmiDrawParamList::RemoveMacroParam(const std::string &theName)
{
	bool status = false;
	std::string wantedName(theName);
	for(Reset(); Next();)
	{
		if(Current()->DataType() == NFmiInfoData::kMacroParam)
		{
			if(Current()->ParameterAbbreviation() == wantedName)
			{
				status = true;
				Remove(true); // true=deletoi drawparamin
			}
		}
	}
	return status;
}

/*!
 * Liikuttaa aktiivista parametriä listalla halutun verran als/ylös päin.
 * Tarkoitus on muuttaa esim. karttanäytöllä parametrien piirtojärjestystä.
 * Palauttaa true jos lista meni likaiseksi eli siirto tapahtui.
 */
bool NFmiDrawParamList::MoveActiveParam(int theMovement)
{
	int paramCount = NumberOfItems();
	if(theMovement && paramCount > 1)
	{
		int index = FindActive();
		int oldIndex = index;
		if(index)
		{
			index += theMovement;
			if(index < 1)
				index = (index + paramCount); // mennään listan ympäri tarvittaessa
			else if(index > paramCount)
				index = (index - paramCount); // mennään listan ympäri tarvittaessa

			// tarkistus koodia sille jos theMovement on suurempi kuin paramCount
			index = FmiMax(index, 1);
			index = FmiMin(index, paramCount);
			if(index != oldIndex) // jos todella tapahtuu siirto, tehdään se ja laitetaan lista likaiseksi
			{
				itsList.Swap(index, oldIndex);
				fDirtyList = true;
				return true;
			}
		}
	}
	return false;
}

/*!
 * Etsii aktiivisen parametrin listalta. Jos löytyy, Currentti osoittaa siihen
 * ja sen indeksi palautetaan (indeksit alkavat 1:stä). Jos ei ole aktiivista
 * palautetaan 0.
 */
int NFmiDrawParamList::FindActive(void)
{
	int index = 1;
	for(Reset(); Next(); index++)
	{
		if(Current()->IsActive())
			return index;
	}
	return 0;
}

/*!
 * Tekee annetusta listasta kopioidut drawParamit omaan listaansa. clearList parametrilla voidaan
 * 'this' -lista tyhjentää ennen kopiointia.
*/
void NFmiDrawParamList::CopyList(NFmiDrawParamList &theList, bool clearFirst)
{
	if(clearFirst)
		Clear(true);

	for(theList.Reset(); theList.Next(); )
	{
		Add(new NFmiDrawParam(*theList.Current()));
	}
}
