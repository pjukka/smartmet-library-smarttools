//**********************************************************
// C++ Class Name : NFmiProducerList 
// ---------------------------------------------------------
// Filetype: (SOURCE)
// Filepath: D:/projekti/GDPro/GDTemp/NFmiProducerList.cpp 
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
#include "NFmiProducerList.h"
#include "NFmiParam.h"
//--------------------------------------------------------
// Constructor/Destructor 
//--------------------------------------------------------
NFmiProducerList::NFmiProducerList() :
itsList(),
itsIter(itsList.Start())
{
}

NFmiProducerList::~NFmiProducerList()
{
	Clear(kTrue);
}
//--------------------------------------------------------
// Remove 
//--------------------------------------------------------
FmiBoolean NFmiProducerList::Remove(FmiBoolean fDeleteData)
{
	return itsIter.Remove(fDeleteData);
}
//--------------------------------------------------------
// Clear 
//--------------------------------------------------------
void NFmiProducerList::Clear(FmiBoolean fDeleteData)
{
	itsList.Clear(fDeleteData);
}
//--------------------------------------------------------
// Add 
//--------------------------------------------------------
FmiBoolean NFmiProducerList::Add(NFmiProducer* theProducer)
{
	return itsList.AddEnd(theProducer);
}
//--------------------------------------------------------
// Current 
//--------------------------------------------------------
NFmiProducer * NFmiProducerList::Current()
{
	return itsIter.CurrentPtr();
}
//--------------------------------------------------------
// Reset 
//--------------------------------------------------------
FmiBoolean NFmiProducerList::Reset(void)
{
   itsIter = itsList.Start();
   return kTrue;
}
//--------------------------------------------------------
// Next 
//--------------------------------------------------------
FmiBoolean NFmiProducerList::Next(void)
{
   return itsIter.Next();
}
//--------------------------------------------------------
// Previous 
//--------------------------------------------------------
FmiBoolean NFmiProducerList::Previous (void)
{
   return itsIter.Previous();
}
//--------------------------------------------------------
// Index
//--------------------------------------------------------
FmiBoolean NFmiProducerList::Index(unsigned long index)
{
	itsIter = itsList.Index(index);
	if(Current())
		return kTrue;
	return kFalse;
}
//--------------------------------------------------------
// Find
//--------------------------------------------------------
FmiBoolean NFmiProducerList::Find(NFmiProducer* item)
{
	itsIter = itsList.Find(item);
	if(Current())
		return kTrue;
	Reset();
	return kFalse;
}