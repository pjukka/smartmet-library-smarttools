//**********************************************************
// C++ Class Name : NFmiSmartToolCalculationInfo 
// ---------------------------------------------------------
// Filetype: (HEADER)
// Filepath: G:/siirto/marko/oc/NFmiSmartToolCalculationInfo.h 
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
// Luokka sis‰lt‰‰ calculationSectionista yhden laskurivin esim.
// 
// T = T + 1
// 
// Lasku rivi sis‰lt‰‰ aina TulosParametrin ja sijoituksen. Lis‰ksi se sis‰lt‰‰ joukon 
// operandeja (n kpl) ja operaatioita (n-1 kpl).
//**********************************************************
#ifndef  NFMISMARTTOOLCALCULATIONINFO_H
#define  NFMISMARTTOOLCALCULATIONINFO_H

#include <vector>
#include <string>

class NFmiAreaMaskInfo;

class NFmiSmartToolCalculationInfo 
{

public:

	NFmiSmartToolCalculationInfo(void);
	~NFmiSmartToolCalculationInfo(void);

	void Clear(void);
	void SetResultDataInfo(NFmiAreaMaskInfo *value) {itsResultDataInfo = value;}
	NFmiAreaMaskInfo* GetResultDataInfo(void) {return itsResultDataInfo;}
	void AddCalculationInfo(NFmiAreaMaskInfo* theAreaMaskInfo); 
	std::vector<NFmiAreaMaskInfo*>* GetCalculationOperandInfoVector(void) {return &itsCalculationOperandInfoVector;}
//	std::vector<NFmiSmartToolCalculation::FmiCalculationOperators>* GetOperationVector(void) {return &itsOperationVector;}
	const std::string& GetCalculationText(void){return itsCalculationText;}
	void SetCalculationText(const std::string& theText){itsCalculationText = theText;}
	void CheckIfAllowMissingValueAssignment(void);
	bool AllowMissingValueAssignment(void){return fAllowMissingValueAssignment;};

private:
	// HUOM!! T‰m‰ erillinen ResultInfo-systeemi oli huono ratkaisu, laita ne mielluummin
	// osaksi laskentaketjua (itsCalculationOperandInfoVector:iin).
	// Silloin voi mm. ottaa t‰m‰n luokan k‰yttˆˆn NFmiAreaMaskSectionInfo-luokan sijasta.
	NFmiAreaMaskInfo *itsResultDataInfo; // omistaa+tuhoaa
	std::vector<NFmiAreaMaskInfo*> itsCalculationOperandInfoVector;  // omistaa+tuhoaa
//	std::vector<NFmiSmartToolCalculation::FmiCalculationOperators> itsOperationVector;
	std::string itsCalculationText; // originaali teksti, mist‰ t‰m‰ lasku on tulkittu
	bool fAllowMissingValueAssignment; 
};
#endif
