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
#include "NFmiRect.h"
#include "NFmiMetEditorTypes.h"

#include <assert.h>
//--------------------------------------------------------
// Constructor/Destructor 
//--------------------------------------------------------

NFmiMultiLevelMask::NFmiMultiLevelMask (unsigned long theSize)
:itsSelectionMask(theSize)
,itsActivationMask(theSize)
,itsDisplayedMask(theSize)
,itsMaskType(NFmiMetEditorTypes::kFmiNoMask)
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
void NFmiMultiLevelMask::MaskType (unsigned long theMaskType)
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
void NFmiMultiLevelMask::Mask (unsigned long theIndex, bool theNewState)
{
	if((itsMaskType & NFmiMetEditorTypes::kFmiNoMask) == NFmiMetEditorTypes::kFmiNoMask)
		return;
	if((itsMaskType & NFmiMetEditorTypes::kFmiActivationMask) == NFmiMetEditorTypes::kFmiActivationMask)
		itsActivationMask.Mask(theIndex, theNewState);
	if((itsMaskType & NFmiMetEditorTypes::kFmiSelectionMask) == NFmiMetEditorTypes::kFmiSelectionMask)
		itsSelectionMask.Mask(theIndex, theNewState);
	if((itsMaskType & NFmiMetEditorTypes::kFmiDisplayedMask) == NFmiMetEditorTypes::kFmiDisplayedMask)
		itsDisplayedMask.Mask(theIndex, theNewState);
	return;
}
//--------------------------------------------------------
// IsMasked 
//--------------------------------------------------------

//   Palauttaa indeksin osoittaman kohdan, itsMaskType:n 
//   osoittamien 
//   maskien arvon. Jos kFmiNoMask 'p‰‰ll‰', palauttaa 
//   aina true. 
//   Tarkista indeksin koko ja jos se ei sovi, 
//   palauta false.
//   
bool NFmiMultiLevelMask::IsMasked (unsigned long theIndex) const
{
	if((itsMaskType & NFmiMetEditorTypes::kFmiNoMask) == NFmiMetEditorTypes::kFmiNoMask)
		return true;
	if((itsMaskType & NFmiMetEditorTypes::kFmiActivationMask) == NFmiMetEditorTypes::kFmiActivationMask)
		if(!itsActivationMask.IsMasked(theIndex))
			return false;
	if((itsMaskType & NFmiMetEditorTypes::kFmiSelectionMask) == NFmiMetEditorTypes::kFmiSelectionMask)
		if(!itsSelectionMask.IsMasked(theIndex))
			return false;
	if((itsMaskType & NFmiMetEditorTypes::kFmiDisplayedMask) == NFmiMetEditorTypes::kFmiDisplayedMask)
		if(!itsDisplayedMask.IsMasked(theIndex))
			return false;
	if((itsMaskType & 0xffffffff) == 0)		// Jos mit‰‰n maaskityyppi‰ ei ole valittu.
		return false;			// T‰m‰ on k‰ytt‰j‰n aiheuttama virhetila.
	return true;
}
//--------------------------------------------------------
// Mask 
//--------------------------------------------------------
//   Asettaa vain theMaskTypen:n osoittamat maskit 
//   .
void NFmiMultiLevelMask::Mask (unsigned long theIndex, bool theNewState, unsigned long theMaskType)
{
	if((theMaskType & NFmiMetEditorTypes::kFmiNoMask) == NFmiMetEditorTypes::kFmiNoMask)
		return;
	if((theMaskType & NFmiMetEditorTypes::kFmiActivationMask) == NFmiMetEditorTypes::kFmiActivationMask)
		itsActivationMask.Mask(theIndex, theNewState);
	if((theMaskType & NFmiMetEditorTypes::kFmiSelectionMask) == NFmiMetEditorTypes::kFmiSelectionMask)
		itsSelectionMask.Mask(theIndex, theNewState);
	if((theMaskType & NFmiMetEditorTypes::kFmiDisplayedMask) == NFmiMetEditorTypes::kFmiDisplayedMask)
		itsDisplayedMask.Mask(theIndex, theNewState);
	return;
}
//--------------------------------------------------------
// IsMasked 
//--------------------------------------------------------
bool NFmiMultiLevelMask::IsMasked (unsigned long theIndex, unsigned long theMaskType) const
{
	if((theMaskType & NFmiMetEditorTypes::kFmiNoMask) == NFmiMetEditorTypes::kFmiNoMask)
		return true;
	if((theMaskType & NFmiMetEditorTypes::kFmiActivationMask) == NFmiMetEditorTypes::kFmiActivationMask)
		if(!itsActivationMask.IsMasked(theIndex))
			return false;
	if((theMaskType & NFmiMetEditorTypes::kFmiSelectionMask) == NFmiMetEditorTypes::kFmiSelectionMask)
		if(!itsSelectionMask.IsMasked(theIndex))
			return false;
	if((theMaskType & NFmiMetEditorTypes::kFmiDisplayedMask) == NFmiMetEditorTypes::kFmiDisplayedMask)
		if(!itsDisplayedMask.IsMasked(theIndex))
			return false;
	if((theMaskType & 0xffffffff) == 0)
		return false;			// T‰m‰ on k‰ytt‰j‰n aiheuttama virhetila.
	return true;

}
//--------------------------------------------------------
// MaskAll 
//--------------------------------------------------------

//   Maskaa kaikki maskit itsMaskType:n osoittaman 
//   arvon mukaisesti
//   eli asettaa kaikkien itsMaskTypen osoittamien 
//   NFmiBitMask:ien
//   kaikki arvot newStaten mukaisiksi.
void NFmiMultiLevelMask::MaskAll (bool theNewState)
{	
	if((itsMaskType & NFmiMetEditorTypes::kFmiNoMask) == NFmiMetEditorTypes::kFmiNoMask)
		return;
	if((itsMaskType & NFmiMetEditorTypes::kFmiActivationMask) == NFmiMetEditorTypes::kFmiActivationMask)
		itsActivationMask.Mask(theNewState);
	if((itsMaskType & NFmiMetEditorTypes::kFmiSelectionMask) == NFmiMetEditorTypes::kFmiSelectionMask)
		itsSelectionMask.Mask(theNewState);
	if((itsMaskType & NFmiMetEditorTypes::kFmiDisplayedMask) == NFmiMetEditorTypes::kFmiDisplayedMask)
		itsDisplayedMask.Mask(theNewState);
	return;
}
//--------------------------------------------------------
// MaskAll 
//--------------------------------------------------------
void NFmiMultiLevelMask::MaskAll (bool theNewState, unsigned long theMaskType)
{
	if ((theMaskType & NFmiMetEditorTypes::kFmiNoMask) == NFmiMetEditorTypes::kFmiNoMask)
		return;
	if ((theMaskType & NFmiMetEditorTypes::kFmiActivationMask) == NFmiMetEditorTypes::kFmiActivationMask)
		itsActivationMask.Mask(theNewState);
	if ((theMaskType & NFmiMetEditorTypes::kFmiSelectionMask) == NFmiMetEditorTypes::kFmiSelectionMask)
		itsSelectionMask.Mask(theNewState);
	if ((theMaskType & NFmiMetEditorTypes::kFmiDisplayedMask) == NFmiMetEditorTypes::kFmiDisplayedMask)
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
bool NFmiMultiLevelMask::Mask (const NFmiBitMask& theMask)
{
	if ((itsMaskType & NFmiMetEditorTypes::kFmiNoMask) == NFmiMetEditorTypes::kFmiNoMask)
		return true;
	if (itsMaskSize == static_cast<unsigned long>(theMask.Size()))
	{
		if ((itsMaskType & NFmiMetEditorTypes::kFmiActivationMask) == NFmiMetEditorTypes::kFmiActivationMask)
			itsActivationMask = theMask;
		if ((itsMaskType & NFmiMetEditorTypes::kFmiSelectionMask) == NFmiMetEditorTypes::kFmiSelectionMask)
			itsSelectionMask = theMask;
		if ((itsMaskType & NFmiMetEditorTypes::kFmiDisplayedMask) == NFmiMetEditorTypes::kFmiDisplayedMask)
			itsDisplayedMask = theMask;
		return true;
	}
	else
		return false;
}
//--------------------------------------------------------
// Mask 
//--------------------------------------------------------

//   Kopioi maskin vain theMaskType:n osoittamiin 
//   maskeihin.
bool NFmiMultiLevelMask::Mask (const NFmiBitMask& theMask, unsigned long theMaskType)
{
	if ((theMaskType & NFmiMetEditorTypes::kFmiNoMask) == NFmiMetEditorTypes::kFmiNoMask)
		return true;
	if (itsMaskSize == static_cast<unsigned long>(theMask.Size()))
	{
		if ((theMaskType & NFmiMetEditorTypes::kFmiActivationMask) == NFmiMetEditorTypes::kFmiActivationMask)
			itsActivationMask = theMask;
		if ((theMaskType & NFmiMetEditorTypes::kFmiSelectionMask) == NFmiMetEditorTypes::kFmiSelectionMask)
			itsSelectionMask = theMask;
		if ((theMaskType & NFmiMetEditorTypes::kFmiDisplayedMask) == NFmiMetEditorTypes::kFmiDisplayedMask)
			itsDisplayedMask = theMask;
		return true;
	}
	else
		return false;
}
//--------------------------------------------------------
// Mask
//--------------------------------------------------------
//   palauttaa ensimmaisen itsMaskTypen osoittaman 
//   maskin.
const NFmiBitMask& NFmiMultiLevelMask::Mask (void) const
{
	if ((itsMaskType & NFmiMetEditorTypes::kFmiNoMask) == NFmiMetEditorTypes::kFmiNoMask)
		return itsActivationMask;						// ???????
	if ((itsMaskType & NFmiMetEditorTypes::kFmiActivationMask) == NFmiMetEditorTypes::kFmiActivationMask)
		return itsActivationMask;
	if ((itsMaskType & NFmiMetEditorTypes::kFmiSelectionMask) == NFmiMetEditorTypes::kFmiSelectionMask)
		return itsSelectionMask;
	if ((itsMaskType & NFmiMetEditorTypes::kFmiDisplayedMask) == NFmiMetEditorTypes::kFmiDisplayedMask)
		return itsDisplayedMask;
	return itsActivationMask;							// ???????
}
//--------------------------------------------------------
// Mask
//--------------------------------------------------------
const NFmiBitMask& NFmiMultiLevelMask::Mask (unsigned long theMaskType) const
{
	if ((theMaskType & NFmiMetEditorTypes::kFmiNoMask) == NFmiMetEditorTypes::kFmiNoMask)
		return itsActivationMask;						// ???????
	if ((theMaskType & NFmiMetEditorTypes::kFmiActivationMask) == NFmiMetEditorTypes::kFmiActivationMask)
		return itsActivationMask;
	if ((theMaskType & NFmiMetEditorTypes::kFmiSelectionMask) == NFmiMetEditorTypes::kFmiSelectionMask)
		return itsSelectionMask;
	if ((theMaskType & NFmiMetEditorTypes::kFmiDisplayedMask) == NFmiMetEditorTypes::kFmiDisplayedMask)
		return itsDisplayedMask;
	return itsActivationMask;							// ???????
}

unsigned long NFmiMultiLevelMask::MaskedCount(unsigned long theMaskType)
{
	if (theMaskType == NFmiMetEditorTypes::kFmiNoMask)
		return itsMaskSize;
	if (theMaskType == NFmiMetEditorTypes::kFmiActivationMask)
		return itsActivationMask.MaskedCount();
	if (theMaskType == NFmiMetEditorTypes::kFmiSelectionMask)
		return itsSelectionMask.MaskedCount();
	if (theMaskType == NFmiMetEditorTypes::kFmiDisplayedMask)
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
	int endRow = static_cast<int>(indexYPos + theSearchArea.Bottom());
	int startRow = static_cast<int>(indexYPos + theSearchArea.Top());
	int startColumn = static_cast<int>(indexXPos + theSearchArea.Left());
	int endColumn = static_cast<int>(indexXPos + theSearchArea.Right());

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
