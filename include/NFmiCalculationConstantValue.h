#pragma once
//**********************************************************
// C++ Class Name : NFmiCalculationConstantValue 
// ---------------------------------------------------  
//  Author         : pietarin 
//  Creation Date  : 9.11. 2010 
// 
//**********************************************************

#include "NFmiAreaMaskImpl.h"
#include "NFmiInfoAreaMask.h"
#include <boost/shared_ptr.hpp>

class NFmiDataModifier;
class NFmiDataIterator;
class NFmiFastQueryInfo;

// T‰m‰ luokka antaa laskuihin mukaan ajan mukana muuttuvat kertoimet. Aikasarja-editorista huom!
class NFmiCalculationConstantValue : public NFmiAreaMaskImpl
{

public:
   double Value(const NFmiPoint & theLatlon, const NFmiMetTime & theTime, int theTimeIndex, bool fUseTimeInterpolationAlways);

   NFmiCalculationConstantValue(double value = 0);
   ~NFmiCalculationConstantValue();

   void SetValue(double value){itsValue = value;}
   double GetValue(void) const {return itsValue;}

private:
   double itsValue;

};

// T‰m‰ luokka antaa laskuihin mukaan korkeus laskuissa (esim. SumZ tai MinH tyyliset funktiot)
// k‰ytetyn kerroksen paksuuden. Korkeus laskut jakavat laskuissaan korkeus akselin vaihtuvan 
// paksuisiin kerroksiin siten ett‰ alhaalla on tarkempi resoluutio kuin ylh‰‰ll‰. T‰t‰ voi k‰ytt‰‰
// vain ed. mainittujen korkeus laskujen yhteydess‰.
class NFmiCalculationDeltaZValue : public NFmiAreaMaskImpl
{

public:
   double Value(const NFmiPoint & theLatlon, const NFmiMetTime & theTime, int theTimeIndex, bool fUseTimeInterpolationAlways)
   {
	   return NFmiCalculationDeltaZValue::itsHeightValue;
   }

   NFmiCalculationDeltaZValue(void);
   ~NFmiCalculationDeltaZValue(){};

   // t‰t‰ funktiota k‰ytt‰m‰ll‰ asetetaan korkeus 'siivun' paksuus. HUOM! se on staattinen kuten on
   // itsHeightValue-dataosakin, joten se tulee kaikille 'DeltaZ':oille yhteiseksi arvoksi.
   static void SetDeltaZValue(double value){itsHeightValue = value;}

private:
   static double itsHeightValue;

};


// T‰m‰ on taas pika suunnittelua/toteutusta, mutta laitoin t‰ll‰isen luokan
// jonka avulla hoidetaan smarttools laskuissa mm. sulut ja lasku- ja vertailu operaattorit.
// Kun laitoin uuden smarttool systeemin laskemaan rekursiivisesti laskut ja
// kaikki (muuttujat, operatorit, sulut jne.) pit‰‰ laittaa samaan putkeen.
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
   double Value(const NFmiPoint & theLatlon, const NFmiMetTime & theTime, int theTimeIndex, bool fUseTimeInterpolationAlways);

   NFmiCalculationRampFuction(const NFmiCalculationCondition& theOperation, Type theMaskType, NFmiInfoData::Type theDataType, boost::shared_ptr<NFmiFastQueryInfo> &theInfo, BinaryOperator thePostBinaryOperator = kNoValue);
   ~NFmiCalculationRampFuction(void);

private:

};

// Ramppifunktioiden laskut AreaMask:ien avulla (mm. lat, lon ja elevationangle tapaukset).
class NFmiCalculationRampFuctionWithAreaMask : public NFmiAreaMaskImpl
{

public:
   double Value(const NFmiPoint & theLatlon, const NFmiMetTime & theTime, int theTimeIndex, bool fUseTimeInterpolationAlways);

   NFmiCalculationRampFuctionWithAreaMask(const NFmiCalculationCondition & theOperation,
										   Type theMaskType,
										   NFmiInfoData::Type theDataType,
										   boost::shared_ptr<NFmiAreaMask> &theAreaMask,
										   BinaryOperator thePostBinaryOperator = kNoValue);
   ~NFmiCalculationRampFuctionWithAreaMask(void);

private:
	boost::shared_ptr<NFmiAreaMask> itsAreaMask;
	bool fIsTimeIntepolationNeededInValue; // erikois optimointia Value-metodin ja Time-metodin k‰ytˆss‰
};

// Avg, min ja max funktioiden laskut.
class NFmiCalculationIntegrationFuction : public NFmiInfoAreaMask
{

public:
   double Value(const NFmiPoint & theLatlon, const NFmiMetTime & theTime, int theTimeIndex, bool fUseTimeInterpolationAlways);

   NFmiCalculationIntegrationFuction(boost::shared_ptr<NFmiDataIterator> &theDataIterator, boost::shared_ptr<NFmiDataModifier> &theDataModifier, Type theMaskType, NFmiInfoData::Type theDataType, boost::shared_ptr<NFmiFastQueryInfo> &theInfo);
   ~NFmiCalculationIntegrationFuction(void);

private:

	boost::shared_ptr<NFmiDataModifier> itsDataModifier;
	boost::shared_ptr<NFmiDataIterator> itsDataIterator;
};
