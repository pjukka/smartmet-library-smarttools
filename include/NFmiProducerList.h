//**********************************************************
// C++ Class Name : NFmiProducerList 
// ---------------------------------------------------------
// Filetype: (HEADER)
// Filepath: D:/projekti/GDPro/GDTemp/NFmiProducerList.h 
// 
// 
// GDPro Properties 
// ---------------------------------------------------
//  - GD Symbol Type    : CLD_Class 
//  - GD Method         : UML ( 2.1.4 ) 
//  - GD System Name    : drawparam jutut 
//  - GD View Type      : Class Diagram 
//  - GD View Name      : Class Diagram 
// ---------------------------------------------------  
//  Author         : pietarin 
//  Creation Date  : Wed - Mar 3, 1999 
// 
// 
//  Description: 
// 
//  Change Log: 
// 
//**********************************************************
#ifndef  NFMIPRODUCERLIST_H
#define  NFMIPRODUCERLIST_H

#include "NFmiSortedPtrList.h"
#include "NFmiGlobals.h"
#include "NFmiProducer.h"

class NFmiProducerList 
{

 public:
	NFmiProducerList ();
	~NFmiProducerList ();
	FmiBoolean		Remove (FmiBoolean fDeleteData = kFalse);
	void			Clear (FmiBoolean fDeleteData = kFalse);
	FmiBoolean		Add (NFmiProducer* theProducer);
	NFmiProducer*	Current ();
	FmiBoolean		Reset (void);
	FmiBoolean		Next (void);
	FmiBoolean		Previous (void);
	FmiBoolean		Index(unsigned long index);
	FmiBoolean		Find(NFmiProducer* item);

 private:
	NFmiSortedPtrList < NFmiProducer > itsList;
	NFmiPtrList < NFmiProducer > :: Iterator itsIter;
};

#endif
