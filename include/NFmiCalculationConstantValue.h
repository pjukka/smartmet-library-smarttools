//**********************************************************
// C++ Class Name : NFmiCalculationConstantValue 
// ---------------------------------------------------------
// Filetype: (HEADER)
// Filepath: G:/siirto/marko/oc/NFmiCalculationConstantValue.h 
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
//**********************************************************
#ifndef  NFMICALCULATIONCONSTANTVALUE_H
#define  NFMICALCULATIONCONSTANTVALUE_H

#include "NFmiAreaMaskImpl.h"
#include "NFmiSmartToolCalculation.h"
#include "NFmiInfoAreaMask.h"

class NFmiDataModifier;
class NFmiDataIterator;

// Tämä luokka antaa laskuihin mukaan ajan mukana muuttuvat kertoimet. Aikasarja-editorista huom!
class NFmiCalculationConstantValue : public NFmiAreaMaskImpl
{

public:
   double Value (const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex) ;

   NFmiCalculationConstantValue(double value = 0);
   ~NFmiCalculationConstantValue();

   void SetValue(double value){itsValue = value;}
   double GetValue(void) const {return itsValue;}

private:
   double itsValue;

};

// Tämä on taas pika suunnittelua/toteutusta, mutta laitoin tälläisen luokan
// jonka avulla hoidetaan smarttools laskuissa mm. sulut ja lasku- ja vertailu operaattorit.
// Kun laitoin uuden smarttool systeemin laskemaan rekursiivisesti laskut ja
// kaikki (muuttujat, operatorit, sulut jne.) pitää laittaa samaan putkeen.
class NFmiCalculationSpecialCase : public NFmiAreaMaskImpl
{

public:
	NFmiCalculationSpecialCase(NFmiAreaMask::CalculationOperator theValue = NotOperation);
	~NFmiCalculationSpecialCase(void){};


private:

};

// Ramppifunktioiden laskut.
class NFmiCalculationRampFuction : public NFmiInfoAreaMask
{

public:
   double Value(const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);

   NFmiCalculationRampFuction(const NFmiCalculationCondition& theOperation, Type theMaskType, DataType theDataType, NFmiQueryInfo* theInfo, bool ownsInfo = false, BinaryOperator thePostBinaryOperator = kNoValue);
   ~NFmiCalculationRampFuction(void);

private:

};

// Ramppifunktioiden laskut AreaMask:ien avulla (mm. lat, lon ja elevationangle tapaukset).
class NFmiCalculationRampFuctionWithAreaMask : public NFmiAreaMaskImpl
{

public:
   double Value(const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);

   NFmiCalculationRampFuctionWithAreaMask(const NFmiCalculationCondition & theOperation,
										   Type theMaskType,
										   DataType theDataType,
										   NFmiAreaMask * theAreaMask,
										   bool ownsAreaMask = false,
										   BinaryOperator thePostBinaryOperator = kNoValue);
   ~NFmiCalculationRampFuctionWithAreaMask(void);

private:
	NFmiAreaMask* itsAreaMask;
	bool fOwnsAreaMask;
	bool fDestroySmartInfoData; // Jossain tilanteissa tälle luokalle annetaan smartinfo, josta pitää tuhota datat erikseen.
	bool fIsTimeIntepolationNeededInValue; // erikois optimointia Value-metodin ja Time-metodin käytössä
};

// Avg, min ja max funktioiden laskut.
class NFmiCalculationIntegrationFuction : public NFmiInfoAreaMask
{

public:
   double Value(const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);

   NFmiCalculationIntegrationFuction(NFmiDataIterator *theDataIterator, NFmiDataModifier *theDataModifier, Type theMaskType, DataType theDataType, NFmiQueryInfo* theInfo, bool ownsInfo = false, bool destroySmartInfoData = false);
   ~NFmiCalculationIntegrationFuction(void);

private:

	NFmiDataModifier *itsDataModifier; // omistaa ja tuhoaa
	NFmiDataIterator *itsDataIterator; // omistaa ja tuhoaa
};

#endif
