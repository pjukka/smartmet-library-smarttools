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
#include "NFmiProducer.h"

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
	Clear(true);
}
//--------------------------------------------------------
// Remove 
//--------------------------------------------------------
bool NFmiProducerList::Remove(bool fDeleteData)
{
	return itsIter.Remove(fDeleteData);
}
//--------------------------------------------------------
// Clear 
//--------------------------------------------------------
void NFmiProducerList::Clear(bool fDeleteData)
{
	itsList.Clear(fDeleteData);
}
//--------------------------------------------------------
// Add 
//--------------------------------------------------------
bool NFmiProducerList::Add(NFmiProducer* theProducer)
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
bool NFmiProducerList::Reset(void)
{
   itsIter = itsList.Start();
   return true;
}
//--------------------------------------------------------
// Next 
//--------------------------------------------------------
bool NFmiProducerList::Next(void)
{
   return itsIter.Next();
}
//--------------------------------------------------------
// Previous 
//--------------------------------------------------------
bool NFmiProducerList::Previous (void)
{
   return itsIter.Previous();
}
//--------------------------------------------------------
// Index
//--------------------------------------------------------
bool NFmiProducerList::Index(unsigned long index)
{
	itsIter = itsList.Index(index);
	if(Current())
		return true;
	return false;
}
//--------------------------------------------------------
// Find
//--------------------------------------------------------
bool NFmiProducerList::Find(NFmiProducer* item)
{
	itsIter = itsList.Find(item);
	if(Current())
		return true;
	Reset();
	return false;
}
