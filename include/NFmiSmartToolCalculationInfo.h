#pragma once
//**********************************************************
// C++ Class Name : NFmiSmartToolCalculationInfo 
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

#include <NFmiDataMatrix.h>
#include <boost/shared_ptr.hpp>
#include <string>

class NFmiAreaMaskInfo;

class NFmiSmartToolCalculationInfo 
{

public:

	NFmiSmartToolCalculationInfo(void);
	~NFmiSmartToolCalculationInfo(void);

	void SetResultDataInfo(boost::shared_ptr<NFmiAreaMaskInfo> &value) {itsResultDataInfo = value;}
	boost::shared_ptr<NFmiAreaMaskInfo> GetResultDataInfo(void) {return itsResultDataInfo;}
	void AddCalculationInfo(boost::shared_ptr<NFmiAreaMaskInfo> &theAreaMaskInfo); 
	checkedVector<boost::shared_ptr<NFmiAreaMaskInfo> >& GetCalculationOperandInfoVector(void) {return itsCalculationOperandInfoVector;}
	const std::string& GetCalculationText(void){return itsCalculationText;}
	void SetCalculationText(const std::string& theText){itsCalculationText = theText;}
	void CheckIfAllowMissingValueAssignment(void);
	bool AllowMissingValueAssignment(void){return fAllowMissingValueAssignment;};

private:
	// HUOM!! T‰m‰ erillinen ResultInfo-systeemi oli huono ratkaisu, laita ne mielluummin
	// osaksi laskentaketjua (itsCalculationOperandInfoVector:iin).
	// Silloin voi mm. ottaa t‰m‰n luokan k‰yttˆˆn NFmiAreaMaskSectionInfo-luokan sijasta.
	boost::shared_ptr<NFmiAreaMaskInfo> itsResultDataInfo; // omistaa+tuhoaa
	checkedVector<boost::shared_ptr<NFmiAreaMaskInfo> > itsCalculationOperandInfoVector;  // omistaa+tuhoaa
	std::string itsCalculationText; // originaali teksti, mist‰ t‰m‰ lasku on tulkittu
	bool fAllowMissingValueAssignment; 
};

