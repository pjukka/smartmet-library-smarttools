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
   double Value(const NFmiCalculationParams &theCalculationParams, bool fUseTimeInterpolationAlways);

   NFmiCalculationConstantValue(double value = 0);
   ~NFmiCalculationConstantValue();
   NFmiCalculationConstantValue(const NFmiCalculationConstantValue &theOther);
   NFmiAreaMask* Clone(void) const;

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
   double Value(const NFmiCalculationParams &theCalculationParams, bool fUseTimeInterpolationAlways)
   {
	   return NFmiCalculationDeltaZValue::itsHeightValue;
   }

   NFmiCalculationDeltaZValue(void);
   ~NFmiCalculationDeltaZValue(){};
   NFmiCalculationDeltaZValue(const NFmiCalculationDeltaZValue &theOther);
   NFmiAreaMask* Clone(void) const;

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
	NFmiCalculationSpecialCase(const NFmiCalculationSpecialCase &theOther);
	NFmiAreaMask* Clone(void) const;


private:

};

// Ramppifunktioiden laskut.
class NFmiCalculationRampFuction : public NFmiInfoAreaMask
{

public:
   double Value(const NFmiCalculationParams &theCalculationParams, bool fUseTimeInterpolationAlways);

   NFmiCalculationRampFuction(const NFmiCalculationCondition& theOperation, Type theMaskType, NFmiInfoData::Type theDataType, boost::shared_ptr<NFmiFastQueryInfo> &theInfo, BinaryOperator thePostBinaryOperator = kNoValue);
   ~NFmiCalculationRampFuction(void);
   NFmiCalculationRampFuction(const NFmiCalculationRampFuction &theOther);
   NFmiAreaMask* Clone(void) const;

private:

};

// Ramppifunktioiden laskut AreaMask:ien avulla (mm. lat, lon ja elevationangle tapaukset).
class NFmiCalculationRampFuctionWithAreaMask : public NFmiAreaMaskImpl
{

public:
   double Value(const NFmiCalculationParams &theCalculationParams, bool fUseTimeInterpolationAlways);

   NFmiCalculationRampFuctionWithAreaMask(const NFmiCalculationCondition & theOperation,
										   Type theMaskType,
										   NFmiInfoData::Type theDataType,
										   boost::shared_ptr<NFmiAreaMask> &theAreaMask,
										   BinaryOperator thePostBinaryOperator = kNoValue);
   ~NFmiCalculationRampFuctionWithAreaMask(void);
   NFmiCalculationRampFuctionWithAreaMask(const NFmiCalculationRampFuctionWithAreaMask &theOther);
   NFmiAreaMask* Clone(void) const;

private:
	boost::shared_ptr<NFmiAreaMask> itsAreaMask;
	bool fIsTimeIntepolationNeededInValue; // erikois optimointia Value-metodin ja Time-metodin k‰ytˆss‰
};

// Avg, min ja max funktioiden laskut.
class NFmiCalculationIntegrationFuction : public NFmiInfoAreaMask
{

public:
   double Value(const NFmiCalculationParams &theCalculationParams, bool fUseTimeInterpolationAlways);

   NFmiCalculationIntegrationFuction(boost::shared_ptr<NFmiDataIterator> &theDataIterator, boost::shared_ptr<NFmiDataModifier> &theDataModifier, Type theMaskType, NFmiInfoData::Type theDataType, boost::shared_ptr<NFmiFastQueryInfo> &theInfo);
   ~NFmiCalculationIntegrationFuction(void);

private:
   NFmiCalculationIntegrationFuction(const NFmiCalculationIntegrationFuction &theOther);

	boost::shared_ptr<NFmiDataModifier> itsDataModifier;
	boost::shared_ptr<NFmiDataIterator> itsDataIterator;
};

// t‰m‰ luokka on ik‰v‰sti riippuvainen 
// 1. NFmiEditMapGeneralDataDoc -luokasta (smartmetbizcode\generaldoc -moduuli) 
// 2. NFmiStationView -luokasta (smartmetbizcode\stationviews -moduuli)
// 3. AVS ToolMaster-kirjastosta (joka tekee varsinaisen griddauksen)
// Siksi se on mukana vain optionaalisesti, eli m‰‰rit‰ FMI_SUPPORT_STATION_DATA_SMARTTOOL
// jos haluat t‰m‰n k‰yttˆˆn.
#ifdef FMI_SUPPORT_STATION_DATA_SMARTTOOL

class NFmiEditMapGeneralDataDoc;

// t‰m‰ maski osaa laskea halutulle asemadatalle hilatut arvot halutulle alueelle
// Jos maskin itsInfo on station-dataa, sen laskut tehd‰‰n toisella tavalla kuin 'normaalin' hila-datan kanssa
// 1. Se pit‰‰ initilisoida kerran joka erillist‰ aikaa kohden eli lasketaan matriisiin valmiiksi kaikki arvot kerralla
// 2. Kun maskin arvoja pyydet‰‰n Value-metodissa, ne saadaan valmiiksi lasketusta taulukosta (aika initialisointi voi tapahtua myˆs siell‰)
class NFmiStation2GridMask : public NFmiInfoAreaMask
{
	typedef std::map<NFmiMetTime, NFmiDataMatrix<float> > DataCache;

public:
   NFmiStation2GridMask(Type theMaskType, NFmiInfoData::Type theDataType, boost::shared_ptr<NFmiFastQueryInfo> &theInfo);
   ~NFmiStation2GridMask(void);
   NFmiStation2GridMask(const NFmiStation2GridMask &theOther);
   NFmiAreaMask* Clone(void) const;

   double Value(const NFmiCalculationParams &theCalculationParams, bool fUseTimeInterpolationAlways);
   void SetGriddingHelpers(NFmiArea *theArea, NFmiEditMapGeneralDataDoc *theDoc, const NFmiPoint &theStation2GridSize);

private:
  void DoGriddingCheck(const NFmiCalculationParams &theCalculationParams);

//  NFmiDataMatrix<float> itsGriddedStationData; // t‰ss‰ on asemadatasta lasketut hilatut arvot
  DataCache itsGriddedStationData;
  NFmiDataMatrix<float> *itsCurrentGriddedStationData; // t‰h‰n on laitettu se matriisi, joka sis‰lt‰‰ halutun ajan asemadatasta lasketut hilatut arvot
  NFmiMetTime itsLastCalculatedTime; // t‰lle ajanhetkelle on station data laskettu (tai puuttuva aika), mutta onko se sama kuin itsTime, jos ei ole, pit‰‰ laskea juuri t‰lle ajalle

  // N‰ille muuttujille pit‰‰ asettaa arvot erillisell‰ SetGridHelpers-funktiolla
  std::auto_ptr<NFmiArea> itsAreaPtr; // omistaa ja tuhoaa!!
  NFmiEditMapGeneralDataDoc *itsDoc;
  NFmiPoint itsStation2GridSize; // t‰m‰n kokoiseen hilaan asema data lasketaan (itsGriddedStationData -koko)

};

#endif // FMI_SUPPORT_STATION_DATA_SMARTTOOL
