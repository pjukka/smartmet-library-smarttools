//**********************************************************
// C++ Class Name : NFmiDrawParamList 
// ---------------------------------------------------------
// Filetype: (HEADER)
// Filepath: D:/projekti/GDPro/GDTemp/NFmiDrawParamList.h 
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
//   Voi olla ett‰ t‰ss‰ riitt‰‰ suoraan uuden 
//   template lista luokan ilmentym‰.
// 
//
//  Change Log: 
// 
//  22.02.-99 lis‰ttiin bool fDirtyList, joka saa arvon
//  metodeissa Add, Remove ja Clear true. Oletusarvo on false. 
// Changed 26.08.1999/Marko		Lis‰sin uuden Find()-metodin.
//
//**********************************************************
#ifndef  NFMIDRAWPARAMLIST_H
#define  NFMIDRAWPARAMLIST_H

#include "NFmiSortedPtrList.h"
#include "NFmiGlobals.h"
#include "NFmiParameterName.h"
//#include "FmiNMeteditLibraryDefinitions.h"
#include <list>
#include <vector>
#include "NFmiInfoData.h"

class NFmiMetTime;
class NFmiLevel;
class NFmiDrawParam;
class NFmiProducer;
class NFmiLevel;
class NFmiDataIdent;

class NFmiDrawParamList 
{

public:

	NFmiDrawParamList (void);
	~NFmiDrawParamList (void);

	NFmiDrawParam*  Current (void);
	bool      Reset (void);
	bool      Next (void);
	bool      Previous (void);
	void            Clear (bool fDeleteData = false);
	void            Clear (const NFmiProducer& theProducer, std::vector<int>& theParamIdsNotRemoved, NFmiLevel* theLevel = 0, bool fDeleteData = false);
	void            Clear (const NFmiProducer& theProducer, std::list<std::pair<int, NFmiLevel> >& theParamIdsAndLevelsNotRemoved, bool fDeleteData = false);
	bool      Add (NFmiDrawParam * theParam);
	bool      Add (NFmiDrawParam * theParam, unsigned long theIndex);
	bool      Replace(NFmiDrawParam * theParam, bool fUseOnlyParamId, bool fInitNewDrawParamFromListFirst); // tuhoaa vanhan, jos lˆytyy, ja lis‰‰ per‰‰n
	bool      Remove (bool fDeleteData = false);
	bool      Index(unsigned long index);
	bool		Find(NFmiDrawParam* item);

	void			HideAllParams(bool newState);
	void			DisableEditing(void);
	void			DeactivateAll(void);
	bool		Find(const NFmiDataIdent& theParam, bool fIgnoreProducer = false);
	bool		Find(const NFmiDataIdent& theParam, const NFmiLevel* theLevel);
	bool		Find(const NFmiDataIdent& theParam, const NFmiLevel* theLevel, NFmiInfoData::Type theDataType, bool fUseOnlyParamId = false);
	bool		SyncronizeTimes(const NFmiMetTime& theTime);

	void            Update(void);
	bool      IsDirty (void){ return fDirtyList; }; 
	void            Dirty (bool fDirty){ fDirtyList = fDirty; }; 
	unsigned long NumberOfItems(void){return itsList.NumberOfItems();};
	bool RemoveMacroParam(const std::string &theName);
	bool MoveActiveParam(int theMovement);
	int FindActive(void);

private:
	NFmiSortedPtrList < NFmiDrawParam > itsList;
	NFmiPtrList < NFmiDrawParam > :: Iterator itsIter;
	bool fDirtyList;
};

#endif
