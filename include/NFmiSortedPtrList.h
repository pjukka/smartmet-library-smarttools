//**********************************************************
// C++ Class Name : NFmiSortedPtrList 
// ---------------------------------------------------------
// Filetype: (HEADER)
// Filepath: D:/projekti/GDPro/GDTemp/NFmiSortedPtrList.h 
// 
// 
// GDPro Properties 
// ---------------------------------------------------
//  - GD Symbol Type    : CLD_TemplateClass 
//  - GD Method         : UML ( 2.1.4 ) 
//  - GD System Name    : ptrlist 
//  - GD View Type      : Class Diagram 
//  - GD View Name      : Class Diagram 
// ---------------------------------------------------  
//  Author         : pietarin 
//  Creation Date  : Tues - Feb 2, 1999 
// 
// 
//  Description: 
//   Pointeri lista jossa itemit ovat j‰rjestetty 
//   haluttuun j‰rjestykseen.
//   T‰h‰n luokkaan laitettavilla olioiden luokilla 
//   on oltava operaattorit < ja ==
// 
//  Change Log: 
// 
//**********************************************************
#ifndef  NFMISORTEDPTRLIST_H
#define  NFMISORTEDPTRLIST_H

#include "NFmiPtrList.h"

template <class Type>
class NFmiSortedPtrList : public NFmiPtrList<Type>
{

public:

	NFmiSortedPtrList (bool fAscendingOrder = true)
					: NFmiPtrList<Type>()
	{};

	virtual ~NFmiSortedPtrList (void)
	{};

	bool InsertionSort (Type* theItem)
	{
		typename NFmiPtrList<Type>::Iterator tempIter = Start();
		while(tempIter.Next())
		{
			if(*theItem < tempIter.Current())
				return tempIter.AddBefore(theItem);
		}
		return  AddEnd(theItem);
	};

	bool Sort(bool fAscendingOrder = true)
	{
		typename NFmiPtrList<Type>::Iterator theIter1 = Start();
		typename NFmiPtrList<Type>::Iterator theIter2;
		while(theIter1.Next())
		{
			theIter2 = theIter1;
			while(theIter2.Next())
			{
				if(  (theIter2.Current()<theIter1.Current() && fAscendingOrder)
				  || (theIter1.Current()<theIter2.Current() && !fAscendingOrder) )
				{
					Swap(theIter1, theIter2);
				}
			}
		}
		return true;
	};

	NFmiPtrList<Type>::Iterator Find(Type* theItem)
	{
		typename NFmiPtrList<Type>::Iterator theIter = Start();
		while(theIter.Next())
		{

			if(theIter.Current() == *theItem)
				return theIter; 
		}
		return theIter;
	};


private:



private: 
	bool fAscending; //   M‰‰r‰‰ listan j‰rjestyksen (true = nouseva).
};


#endif
