//**********************************************************
// C++ Class Name : NFmiMultiLevelMask 
// ---------------------------------------------------------
// Filetype: (SOURCE)
// Filepath: D:/projekti/GDPro/GDTemp/NFmiMultiLevelMask.cpp 
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
//  Creation Date  : Tues - Feb 9, 1999 
// 
// 
//  Description: 
//   Luokka joka pit‰‰ sis‰ll‰‰n usean tason maskeja. 
//   Kaikki toiminnat
//   riippuvat aina itsMaskState:n arvosta. T‰ss‰ 
//   vaiheessa luokka
//   toteutetaan ei dynaamisesti, luomalla k‰yttˆˆn 
//   kolme tasoinen
//   maski, jota tarvitaan MetEditorissa. Myˆhemmin 
//   luokan voisi tehd‰
//   siten, ett‰ eri bitmaskit laitetaan listaan.
//   
// 
//  Change Log: 
// 
//**********************************************************
#include "NFmiMultiLevelMask.h"
#include "FmiNMeteditLibraryDefinitions.h"
#include "NFmiRect.h"

#include <assert.h>
//--------------------------------------------------------
// Constructor/Destructor 
//--------------------------------------------------------

NFmiMultiLevelMask::NFmiMultiLevelMask (unsigned long theSize)
:itsSelectionMask(theSize)
,itsActivationMask(theSize)
,itsDisplayedMask(theSize)
,itsMaskType(kFmiNoMask)
,itsMaskSize(theSize)
{
}

NFmiMultiLevelMask::NFmiMultiLevelMask (const NFmiMultiLevelMask& theMask)
:itsSelectionMask(theMask.itsSelectionMask)
,itsActivationMask(theMask.itsActivationMask)
,itsDisplayedMask(theMask.itsDisplayedMask)
,itsMaskType(theMask.itsMaskType)
,itsMaskSize(theMask.itsMaskSize)
{
}

NFmiMultiLevelMask& NFmiMultiLevelMask::operator=(const NFmiMultiLevelMask& theMask)
{
	itsSelectionMask = theMask.itsSelectionMask;
	itsActivationMask = theMask.itsActivationMask;
	itsDisplayedMask = theMask.itsDisplayedMask;
	itsMaskType = theMask.itsMaskType;
	itsMaskSize = theMask.itsMaskSize;

	return *this;
}

NFmiMultiLevelMask::~NFmiMultiLevelMask (void)
{

}

//--------------------------------------------------------
// MaskType
//--------------------------------------------------------
unsigned long NFmiMultiLevelMask::MaskType(void) const
{
	return itsMaskType;
}
//--------------------------------------------------------
// MaskType
//--------------------------------------------------------
void NFmiMultiLevelMask::MaskType (const unsigned long& theMaskType)
{
	itsMaskType = theMaskType;
	return;
}
//--------------------------------------------------------
// MaskSize
//--------------------------------------------------------
const unsigned long& NFmiMultiLevelMask::MaskSize(void) const
{
	return itsMaskSize;
}

//--------------------------------------------------------
// Mask 
//--------------------------------------------------------
//   Asettaa indeksin osoittaman kohdan maskin 
//   newState:n mukaiseksi.
//   Se mik‰/mitk‰ maskit asetetaan , riippuu 
//   itsMaskType:n arvosta.
void NFmiMultiLevelMask::Mask (const unsigned long& theIndex, const FmiBoolean& theNewState)
{
	if((itsMaskType & kFmiNoMask) == kFmiNoMask)
		return;
	if((itsMaskType & kFmiActivationMask) == kFmiActivationMask)
		itsActivationMask.Mask(theIndex, theNewState);
	if((itsMaskType & kFmiSelectionMask) == kFmiSelectionMask)
		itsSelectionMask.Mask(theIndex, theNewState);
	if((itsMaskType & kFmiDisplayedMask) == kFmiDisplayedMask)
		itsDisplayedMask.Mask(theIndex, theNewState);
	return;
}
//--------------------------------------------------------
// IsMasked 
//--------------------------------------------------------

//   Palauttaa indeksin osoittaman kohdan, itsMaskType:n 
//   osoittamien 
//   maskien arvon. Jos kFmiNoMask 'p‰‰ll‰', palauttaa 
//   aina kTrue. 
//   Tarkista indeksin koko ja jos se ei sovi, 
//   palauta kFalse.
//   
FmiBoolean NFmiMultiLevelMask::IsMasked (const unsigned long& theIndex) const
{
	if((itsMaskType & kFmiNoMask) == kFmiNoMask)
		return kTrue;
	if((itsMaskType & kFmiActivationMask) == kFmiActivationMask)
		if(!itsActivationMask.IsMasked(theIndex))
			return kFalse;
	if((itsMaskType & kFmiSelectionMask) == kFmiSelectionMask)
		if(!itsSelectionMask.IsMasked(theIndex))
			return kFalse;
	if((itsMaskType & kFmiDisplayedMask) == kFmiDisplayedMask)
		if(!itsDisplayedMask.IsMasked(theIndex))
			return kFalse;
	if((itsMaskType & 0xffffffff) == 0)		// Jos mit‰‰n maaskityyppi‰ ei ole valittu.
		return kFalse;			// T‰m‰ on k‰ytt‰j‰n aiheuttama virhetila.
	return kTrue;
}
//--------------------------------------------------------
// Mask 
//--------------------------------------------------------
//   Asettaa vain theMaskTypen:n osoittamat maskit 
//   .
void NFmiMultiLevelMask::Mask (const unsigned long& theIndex, const FmiBoolean& theNewState, const unsigned long& theMaskType)
{
	if((theMaskType & kFmiNoMask) == kFmiNoMask)
		return;
	if((theMaskType & kFmiActivationMask) == kFmiActivationMask)
		itsActivationMask.Mask(theIndex, theNewState);
	if((theMaskType & kFmiSelectionMask) == kFmiSelectionMask)
		itsSelectionMask.Mask(theIndex, theNewState);
	if((theMaskType & kFmiDisplayedMask) == kFmiDisplayedMask)
		itsDisplayedMask.Mask(theIndex, theNewState);
	return;
}
//--------------------------------------------------------
// IsMasked 
//--------------------------------------------------------
FmiBoolean NFmiMultiLevelMask::IsMasked (const unsigned long& theIndex, const unsigned long& theMaskType) const
{
	if((theMaskType & kFmiNoMask) == kFmiNoMask)
		return kTrue;
	if((theMaskType & kFmiActivationMask) == kFmiActivationMask)
		if(!itsActivationMask.IsMasked(theIndex))
			return kFalse;
	if((theMaskType & kFmiSelectionMask) == kFmiSelectionMask)
		if(!itsSelectionMask.IsMasked(theIndex))
			return kFalse;
	if((theMaskType & kFmiDisplayedMask) == kFmiDisplayedMask)
		if(!itsDisplayedMask.IsMasked(theIndex))
			return kFalse;
	if((theMaskType & 0xffffffff) == 0)
		return kFalse;			// T‰m‰ on k‰ytt‰j‰n aiheuttama virhetila.
	return kTrue;

}
//--------------------------------------------------------
// MaskAll 
//--------------------------------------------------------

//   Maskaa kaikki maskit itsMaskType:n osoittaman 
//   arvon mukaisesti
//   eli asettaa kaikkien itsMaskTypen osoittamien 
//   NFmiBitMask:ien
//   kaikki arvot newStaten mukaisiksi.
void NFmiMultiLevelMask::MaskAll (const FmiBoolean& theNewState)
{	
	if((itsMaskType & kFmiNoMask) == kFmiNoMask)
		return;
	if((itsMaskType & kFmiActivationMask) == kFmiActivationMask)
		itsActivationMask.Mask(theNewState);
	if((itsMaskType & kFmiSelectionMask) == kFmiSelectionMask)
		itsSelectionMask.Mask(theNewState);
	if((itsMaskType & kFmiDisplayedMask) == kFmiDisplayedMask)
		itsDisplayedMask.Mask(theNewState);
	return;
}
//--------------------------------------------------------
// MaskAll 
//--------------------------------------------------------
void NFmiMultiLevelMask::MaskAll (const FmiBoolean& theNewState, const unsigned long& theMaskType)
{
	if ((theMaskType & kFmiNoMask) == kFmiNoMask)
		return;
	if ((theMaskType & kFmiActivationMask) == kFmiActivationMask)
		itsActivationMask.Mask(theNewState);
	if ((theMaskType & kFmiSelectionMask) == kFmiSelectionMask)
		itsSelectionMask.Mask(theNewState);
	if ((theMaskType & kFmiDisplayedMask) == kFmiDisplayedMask)
		itsDisplayedMask.Mask(theNewState);
	return;
}
//--------------------------------------------------------
// Mask 
//--------------------------------------------------------

//   Kopioi annetun maskin suoraan itsMaskTypen 
//   osoittamiin maskeihin.
//   Jos aneetun maskin ja itsMaskSize:n koot 
//   eiv‰t t‰sm‰‰, ep‰onnistuu
//   koko operaatio.
FmiBoolean NFmiMultiLevelMask::Mask (const NFmiBitMask& theMask)
{
	if ((itsMaskType & kFmiNoMask) == kFmiNoMask)
		return kTrue;
	if (itsMaskSize == unsigned long(theMask.Size()))
	{
		if ((itsMaskType & kFmiActivationMask) == kFmiActivationMask)
			itsActivationMask = theMask;
		if ((itsMaskType & kFmiSelectionMask) == kFmiSelectionMask)
			itsSelectionMask = theMask;
		if ((itsMaskType & kFmiDisplayedMask) == kFmiDisplayedMask)
			itsDisplayedMask = theMask;
		return kTrue;
	}
	else
		return kFalse;
}
//--------------------------------------------------------
// Mask 
//--------------------------------------------------------

//   Kopioi maskin vain theMaskType:n osoittamiin 
//   maskeihin.
FmiBoolean NFmiMultiLevelMask::Mask (const NFmiBitMask& theMask, const unsigned long& theMaskType)
{
	if ((theMaskType & kFmiNoMask) == kFmiNoMask)
		return kTrue;
	if (itsMaskSize == unsigned long(theMask.Size()))
	{
		if ((theMaskType & kFmiActivationMask) == kFmiActivationMask)
			itsActivationMask = theMask;
		if ((theMaskType & kFmiSelectionMask) == kFmiSelectionMask)
			itsSelectionMask = theMask;
		if ((theMaskType & kFmiDisplayedMask) == kFmiDisplayedMask)
			itsDisplayedMask = theMask;
		return kTrue;
	}
	else
		return kFalse;
}
//--------------------------------------------------------
// Mask
//--------------------------------------------------------
//   palauttaa ensimmaisen itsMaskTypen osoittaman 
//   maskin.
const NFmiBitMask& NFmiMultiLevelMask::Mask (void) const
{
	if ((itsMaskType & kFmiNoMask) == kFmiNoMask)
		return itsActivationMask;						// ???????
	if ((itsMaskType & kFmiActivationMask) == kFmiActivationMask)
		return itsActivationMask;
	if ((itsMaskType & kFmiSelectionMask) == kFmiSelectionMask)
		return itsSelectionMask;
	if ((itsMaskType & kFmiDisplayedMask) == kFmiDisplayedMask)
		return itsDisplayedMask;
	return itsActivationMask;							// ???????
}
//--------------------------------------------------------
// Mask
//--------------------------------------------------------
const NFmiBitMask& NFmiMultiLevelMask::Mask (const unsigned long& theMaskType) const
{
	if ((theMaskType & kFmiNoMask) == kFmiNoMask)
		return itsActivationMask;						// ???????
	if ((theMaskType & kFmiActivationMask) == kFmiActivationMask)
		return itsActivationMask;
	if ((theMaskType & kFmiSelectionMask) == kFmiSelectionMask)
		return itsSelectionMask;
	if ((theMaskType & kFmiDisplayedMask) == kFmiDisplayedMask)
		return itsDisplayedMask;
	return itsActivationMask;							// ???????
}

unsigned long NFmiMultiLevelMask::MaskedCount(unsigned long theMaskType)
{
	if (theMaskType == kFmiNoMask)
		return itsMaskSize;
	if (theMaskType == kFmiActivationMask)
		return itsActivationMask.MaskedCount();
	if (theMaskType == kFmiSelectionMask)
		return itsSelectionMask.MaskedCount();
	if (theMaskType == kFmiDisplayedMask)
		return itsDisplayedMask.MaskedCount();

	return 0;
}

// Laskee kuinka monta pistett‰ on maskattu annetun laatikon alueella, jonka vasen alakulma
// annetun indeksin kohdalla.
int NFmiMultiLevelMask::MaskedCount(unsigned long theMaskType, unsigned long theIndex, const NFmiRect& theSearchArea, int theXGridSize, int theYGridSize)
{
	assert(theXGridSize);
	int indexYPos = theIndex/theXGridSize;
	int indexXPos = theIndex%theXGridSize;
	int count = 0;
	int endRow = indexYPos + theSearchArea.Bottom();
	int startRow = indexYPos + theSearchArea.Top();
	int startColumn = indexXPos + theSearchArea.Left();
	int endColumn = indexXPos + theSearchArea.Right();

	for(int i=startRow; i<=endRow; i++)
	{
		for(int j=startColumn; j<=endColumn; j++)
		{
			if(i < 0 || i >= theYGridSize)
				count++;
			else if(j < 0 || j >= theXGridSize)
				count++;
			else
			{
				int index = i*theXGridSize + j;
				if(IsMasked(index, theMaskType))
					count++;
			}
		}
	}

	return count;
}
