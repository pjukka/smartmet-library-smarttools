//**********************************************************
// C++ Class Name : NFmiAreaMaskSectionInfo 
// ---------------------------------------------------------
// Filetype: (HEADER)
// Filepath: G:/siirto/marko/oc/NFmiAreaMaskSectionInfo.h 
// 
// 
// GDPro Properties 
// ---------------------------------------------------
//  - GD Symbol Type    : CLD_Class 
//  - GD Method         : UML ( 4.0 ) 
//  - GD System Name    : aSmartTools 
//  - GD View Type      : Class Diagram 
//  - GD View Name      : smarttools 1 
// ---------------------------------------------------  
//  Author         : pietarin 
//  Creation Date  : Thur - Jun 20, 2002 
// 
//  Change Log     : 
// 
// Sisältää listan areaMaskInfoja, esim. IF-lauseen maskihan voi olla vaikka:
// IF(T>2) tai IF(T>2 && P<1012) jne.
//**********************************************************
#ifndef  NFMIAREAMASKSECTIONINFO_H
#define  NFMIAREAMASKSECTIONINFO_H

#include "NFmiDataMatrix.h" // täältä tulee myös checkedVector
#include <string>

class NFmiAreaMaskInfo;

class NFmiAreaMaskSectionInfo 
{

public:

	NFmiAreaMaskSectionInfo();
	~NFmiAreaMaskSectionInfo();

	NFmiAreaMaskInfo* MaskInfo(int theIndex);
	void Add(NFmiAreaMaskInfo* theMask);
	checkedVector<NFmiAreaMaskInfo*>* GetAreaMaskInfoVector(void) {return &itsAreaMaskInfoVector;}
	const std::string& GetCalculationText(void){return itsCalculationText;}
	void SetCalculationText(const std::string& theText){itsCalculationText = theText;}

private:
	void Clear(void);

	checkedVector<NFmiAreaMaskInfo*> itsAreaMaskInfoVector;
	std::string itsCalculationText; // originaali teksti, mistä tämä lasku on tulkittu

};

#endif
