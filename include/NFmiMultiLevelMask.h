//**********************************************************
// C++ Class Name : NFmiMultiLevelMask 
// ---------------------------------------------------------
// Filetype: (HEADER)
// Filepath: D:/projekti/GDPro/GDTemp/NFmiMultiLevelMask.h 
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
//   Luokka joka pitää sisällään usean tason maskeja. 
//   Kaikki toiminnat
//   riippuvat aina itsMaskState:n arvosta. Tässä 
//   vaiheessa luokka
//   toteutetaan ei dynaamisesti, luomalla käyttöön 
//   kolme tasoinen
//   maski, jota tarvitaan MetEditorissa. Myöhemmin 
//   luokan voisi tehdä
//   siten, että eri bitmaskit laitetaan listaan.
//   
// 
//  Change Log: 
// 
//**********************************************************
#ifndef  NFMIMULTILEVELMASK_H
#define  NFMIMULTILEVELMASK_H

#include "NFmiGlobals.h"
#include "NFmiBitMask.h"

class NFmiRect;

class NFmiMultiLevelMask 
{

 public:
    NFmiMultiLevelMask (unsigned long theSize = 32);
    NFmiMultiLevelMask (const NFmiMultiLevelMask& theMask);
    NFmiMultiLevelMask& operator=(const NFmiMultiLevelMask& theMask);
    ~NFmiMultiLevelMask (void);
	unsigned long MaskType (void) const;
	void MaskType (const unsigned long& theMaskType);
	const unsigned long& MaskSize(void) const;
	void Mask (const unsigned long& theIndex, const FmiBoolean& theNewState);
	FmiBoolean IsMasked (const unsigned long& theIndex) const;
	void Mask (const unsigned long& theIndex, const FmiBoolean& theNewState, const unsigned long& theMaskType);
	FmiBoolean IsMasked (const unsigned long& theIndex, const unsigned long& theMaskType) const;
	void MaskAll (const FmiBoolean& theNewState);
	void MaskAll (const FmiBoolean& theNewState, const unsigned long& theMaskType);
	FmiBoolean Mask (const NFmiBitMask& theMask);
	FmiBoolean Mask (const NFmiBitMask& theMask, const unsigned long& theMaskType);
	const NFmiBitMask& Mask (void) const;
	const NFmiBitMask& Mask (const unsigned long& theMaskType) const;
	void MaskSize(unsigned long theMaskSize) {};
	unsigned long MaskedCount(unsigned long theMaskType);
	int MaskedCount(unsigned long theMaskType, unsigned long theIndex, const NFmiRect& theSearchArea, int theXGridSize, int theYGridSize);

 private:
	NFmiBitMask itsSelectionMask;
	NFmiBitMask itsActivationMask;
	NFmiBitMask itsDisplayedMask;	// Koskee vain aikasarjaikkunaa
	unsigned long itsMaskType;
	unsigned long itsMaskSize;		//    NFmiBitmask:ien koko.
};


#endif
