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
#ifdef WIN32
#pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän "liian pitkä tyyppi nimi" varoitusta
#endif

#include "NFmiDrawParamList.h"
#include "NFmiSmartInfo.h"
#include <algorithm>
#include <list>

//--------------------------------------------------------
// Constructor  NFmiDrawParamList
//--------------------------------------------------------
NFmiDrawParamList::NFmiDrawParamList(void):
itsList(),
itsIter(itsList.Start()),
fDirtyList(kTrue)
{
}

//--------------------------------------------------------
// Destructor  ~NFmiDrawParamList
//--------------------------------------------------------
NFmiDrawParamList::~NFmiDrawParamList(void)
{
	Clear(kTrue);
}

//--------------------------------------------------------
// Add 
//--------------------------------------------------------
FmiBoolean NFmiDrawParamList::Add (NFmiDrawParam* theParam)
{
   if(itsList.AddEnd(theParam))
   {
		fDirtyList = kTrue;
		return kTrue;
   }
   return kFalse;
}
//--------------------------------------------------------
// Add 
//--------------------------------------------------------
FmiBoolean NFmiDrawParamList::Add(NFmiDrawParam * theParam, unsigned long theIndex)
{
   NFmiPtrList < NFmiDrawParam > :: Iterator iter = itsList.Index(theIndex);
   if(iter.AddBefore(theParam))
   {
		fDirtyList = kTrue;
		return kTrue;
   }
   return kFalse;
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
FmiBoolean NFmiDrawParamList::Reset (void)
{
   itsIter = itsList.Start();
   return kTrue;
}
//--------------------------------------------------------
// Next 
//--------------------------------------------------------
FmiBoolean NFmiDrawParamList::Next (void)
{
   return itsIter.Next();
}
//--------------------------------------------------------
// Previous 
//--------------------------------------------------------
FmiBoolean NFmiDrawParamList::Previous (void)
{
   return itsIter.Previous();
}
//--------------------------------------------------------
// Clear 
//--------------------------------------------------------
void NFmiDrawParamList::Clear(FmiBoolean fDeleteData)
{
	itsList.Clear(fDeleteData);
	fDirtyList = kTrue;
}
//--------------------------------------------------------
// Remove 
//--------------------------------------------------------
FmiBoolean NFmiDrawParamList::Remove(FmiBoolean fDeleteData)
{
   if(itsIter.Remove(fDeleteData))
   {
		fDirtyList = kTrue;
		return kTrue;
   }
   return kFalse;
}
//--------------------------------------------------------
// Index
//--------------------------------------------------------
FmiBoolean NFmiDrawParamList::Index(unsigned long index)
{
	itsIter = itsList.Index(index);
	if(Current())
		return kTrue;
	return kFalse;
}
//--------------------------------------------------------
// Find
//--------------------------------------------------------
FmiBoolean NFmiDrawParamList::Find(NFmiDrawParam* item)
{
	itsIter = itsList.Find(item);
	if(Current())
		return kTrue;
	Reset();
	return kFalse;
}

//--------------------------------------------------------
// Update
//--------------------------------------------------------
void NFmiDrawParamList::Update(void)
{
	
}

void NFmiDrawParamList::HideAllParams(FmiBoolean newState)
{
	for(Reset(); Next();)
		Current()->HideParam(newState);
}

// asettaa kaikkien listalla olevien tilan 'ei editoitaviksi' (toisin päin ei ole järkevää tehdä)
void NFmiDrawParamList::DisableEditing(void)
{
	for(Reset(); Next();)
		Current()->EditParam(kFalse);
}

void NFmiDrawParamList::DeactivateAll(void)
{
	for(Reset(); Next();)
		Current()->Activate(kFalse);
}

FmiBoolean NFmiDrawParamList::Find(const NFmiDataIdent& theParam, bool fIgnoreProducer)
{
	for(Reset(); Next();)
		if(fIgnoreProducer)
		{
			if(*(Current()->EditParam().GetParam()) == *(theParam.GetParam()))
				return kTrue;
		}
		else
			if(Current()->EditParam() == theParam)
				return kTrue;
	return kFalse;
}

FmiBoolean NFmiDrawParamList::Find(const NFmiDataIdent& theParam, const NFmiLevel* theLevel)
{
	for(Reset(); Next();)
	{
		NFmiSmartInfo* info = Current()->Info();
		if(info->Param() == theParam)
		{
			if(!theLevel && info->SizeLevels() <= 1)
				return kTrue;
			if(theLevel && info->Level() && (*(theLevel) == *(info->Level())))
				return kTrue;
		}
	}
	return kFalse;
}

FmiBoolean NFmiDrawParamList::Find(const NFmiDataIdent& theParam, const NFmiLevel* theLevel, FmiQueryInfoDataType theDataType)
{
	for(Reset(); Next();)
	{
		NFmiSmartInfo* info = Current()->Info();
		if(info->Param() == theParam)
		{
			if(info->DataType() == theDataType)
			{
				if(!theLevel && info->SizeLevels() <= 1)
					return kTrue;
				if(theLevel && info->Level() && (*(theLevel) == *(info->Level())))
					return kTrue;
			}
		}
	}
	return kFalse;
}

FmiBoolean NFmiDrawParamList::SyncronizeTimes(const NFmiMetTime& theTime)
{
	for(Reset(); Next();)
		Current()->Info()->Time(theTime);
	return kTrue;
}

// Poistaa listalta kaikki halutun tuottajan ja mahd. annetun levelin parametrit.
// Ei poista niitä parametreja, jotka ovat theParamIdsNotRemoved-listalla. Löy-
// tynyt paramId poistetaan theParamIdsNotRemoved-listalta, että niitä ei
// lisättäisi myöhemmin tähän listaan.
void NFmiDrawParamList::Clear(const NFmiProducer& theProducer, std::vector<int>& theParamIdsNotRemoved, NFmiLevel* theLevel, FmiBoolean fDeleteData)
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
			if(*(Current()->EditParam().GetProducer()) == theProducer)
				if(*Current()->Info()->Level() == *theLevel)
				{
					it = std::find(tmpParIdList.begin(), tmpParIdList.end(), Current()->EditParam().GetParamIdent());
					if(it == tmpParIdList.end())
						Remove(fDeleteData);
					else
						tmpParIdList.erase(it);
				}
	}
	else
	{
		for(Reset(); Next();)
		{
			if(Current()->Info()->SizeLevels() == 1) // pitää olla 'ground' info (eli ei level-dataa), ennen kuin ruvetaan siivoamaan!!!
			{
				if(*(Current()->EditParam().GetProducer()) == theProducer)
				{
					it = std::find(tmpParIdList.begin(), tmpParIdList.end(), Current()->EditParam().GetParamIdent());
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
void NFmiDrawParamList::Clear(const NFmiProducer& theProducer, std::list<std::pair<int, NFmiLevel> >& theParamIdsAndLevelsNotRemoved, FmiBoolean fDeleteData)
{
	std::list<std::pair<int, NFmiLevel> >::iterator it;
	for(Reset(); Next();)
	{
		if(Current()->Info()->SizeLevels() > 1) // pitää olla level info, ennen kuin ruvetaan siivoamaan!!!
		{
			if(*(Current()->EditParam().GetProducer()) == theProducer)
			{
				std::pair<int, NFmiLevel> tmp(Current()->EditParam().GetParamIdent(), *Current()->Info()->Level());
				it = std::find(theParamIdsAndLevelsNotRemoved.begin(), theParamIdsAndLevelsNotRemoved.end(), tmp);
				if(it == theParamIdsAndLevelsNotRemoved.end())
					Remove(fDeleteData);
				else
					theParamIdsAndLevelsNotRemoved.erase(it);
			}
		}
	}
}
