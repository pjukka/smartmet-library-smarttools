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
//  22.02.-99 lis‰ttiin FmiBoolean fDirtyList, joka saa arvon
//  metodeissa Add, Remove ja Clear kTrue. Oletusarvo on kFalse. 
// Changed 26.08.1999/Marko		Lis‰sin uuden Find()-metodin.
//
//**********************************************************
#ifndef  NFMIDRAWPARAMLIST_H
#define  NFMIDRAWPARAMLIST_H

#include "NFmiSortedPtrList.h"
#include "NFmiGlobals.h"
#include "NFmiParameterName.h"
#include "NFmiDrawParam.h"
#include <list>

class NFmiMetTime;
class NFmiLevel;

class NFmiDrawParamList 
{

public:

	NFmiDrawParamList (void);
	~NFmiDrawParamList (void);

	NFmiDrawParam*  Current (void);
	FmiBoolean      Reset (void);
	FmiBoolean      Next (void);
	FmiBoolean      Previous (void);
	void            Clear (FmiBoolean fDeleteData = kFalse);
	void            Clear (const NFmiProducer& theProducer, std::vector<int>& theParamIdsNotRemoved, NFmiLevel* theLevel = 0, FmiBoolean fDeleteData = kFalse);
	void            Clear (const NFmiProducer& theProducer, std::list<std::pair<int, NFmiLevel> >& theParamIdsAndLevelsNotRemoved, FmiBoolean fDeleteData = kFalse);
	FmiBoolean      Add (NFmiDrawParam * theParam);
	FmiBoolean      Add (NFmiDrawParam * theParam, unsigned long theIndex);
	FmiBoolean      Remove (FmiBoolean fDeleteData = kFalse);
	FmiBoolean      Index(unsigned long index);
	FmiBoolean		Find(NFmiDrawParam* item);

	void			HideAllParams(FmiBoolean newState);
	void			DisableEditing(void);
	void			DeactivateAll(void);
	FmiBoolean		Find(const NFmiDataIdent& theParam, bool fIgnoreProducer = false);
	FmiBoolean		Find(const NFmiDataIdent& theParam, const NFmiLevel* theLevel);
	FmiBoolean		Find(const NFmiDataIdent& theParam, const NFmiLevel* theLevel, FmiQueryInfoDataType theDataType);
	FmiBoolean		SyncronizeTimes(const NFmiMetTime& theTime);

	void            Update(void);
	FmiBoolean      IsDirty (void){ return fDirtyList; }; 
	void            Dirty (FmiBoolean fDirty){ fDirtyList = fDirty; }; 
	unsigned long NumberOfItems(void){return itsList.NumberOfItems();};

private:
	NFmiSortedPtrList < NFmiDrawParam > itsList;
	NFmiPtrList < NFmiDrawParam > :: Iterator itsIter;
	FmiBoolean fDirtyList;
};

#endif
