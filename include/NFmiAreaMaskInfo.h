//**********************************************************
// C++ Class Name : NFmiAreaMaskInfo 
// ---------------------------------------------------------
// Filetype: (HEADER)
// Filepath: G:/siirto/marko/oc/NFmiAreaMaskInfo.h 
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
#ifndef  NFMIAREAMASKINFO_H
#define  NFMIAREAMASKINFO_H

#include "NFmiAreaMask.h"
#include "NFmiDataIdent.h"
#include "NFmiCalculationCondition.h"
//#include "FmiNMeteditLibraryDefinitions.h"
#include "NFmiPoint.h"

class NFmiLevel;

//template<typename T>
struct PointerDestroyer
{
	template<typename T>
	void operator()(T* thePtr)
	{delete thePtr;}
};

class NFmiAreaMaskInfo 
{

public:

	NFmiAreaMaskInfo(void);
	~NFmiAreaMaskInfo(void);

	void SetDataIdent(const NFmiDataIdent& value) {itsDataIdent = value;}
	const NFmiDataIdent& GetDataIdent(void) const {return itsDataIdent;}
	void SetUseDefaultProducer(bool value) {fUseDefaultProducer = value;}
	bool GetUseDefaultProducer(void) const {return fUseDefaultProducer;}
	void SetMaskCondition(const NFmiCalculationCondition &value){itsMaskCondition = value;}
	const NFmiCalculationCondition& GetMaskCondition(void) const {return itsMaskCondition;}
	NFmiAreaMask::CalculationOperationType GetOperationType(void) const {return itsOperationType;}
	void SetOperationType(NFmiAreaMask::CalculationOperationType newValue) {itsOperationType = newValue;}
	NFmiAreaMask::CalculationOperator GetCalculationOperator(void) const {return itsCalculationOperator;}
	void SetCalculationOperator(NFmiAreaMask::CalculationOperator newValue) {itsCalculationOperator = newValue;}
	NFmiAreaMask::BinaryOperator GetBinaryOperator(void) const{return itsBinaryOperator;}
	void SetBinaryOperator(NFmiAreaMask::BinaryOperator theValue) {itsBinaryOperator = theValue;}
	NFmiInfoData::Type GetDataType(void) const {return itsDataType;}
	void SetDataType(NFmiInfoData::Type newValue){itsDataType = newValue;}
	NFmiLevel* GetLevel(void) const {return itsLevel;}
	void SetLevel(NFmiLevel *theLevel);
	const std::string& GetMaskText(void) const{return itsMaskText;}
	void SetMaskText(const std::string& theText){itsMaskText = theText;}
	NFmiAreaMask::FunctionType GetFunctionType(void)const {return itsFunctionType;}
	void SetFunctionType(NFmiAreaMask::FunctionType newType){itsFunctionType = newType;}
	const NFmiPoint& GetOffsetPoint1(void) const{return itsOffsetPoint1;}
	void SetOffsetPoint1(const NFmiPoint& newValue){itsOffsetPoint1 = newValue;}
	const NFmiPoint& GetOffsetPoint2(void) const{return itsOffsetPoint2;}
	void SetOffsetPoint2(const NFmiPoint& newValue){itsOffsetPoint2 = newValue;}
	NFmiAreaMask::MathFunctionType GetMathFunctionType(void) const {return itsMathFunctionType;};
	void SetMathFunctionType(NFmiAreaMask::MathFunctionType newValue) {itsMathFunctionType = newValue;};
	int IntegrationFunctionType(void) const {return itsIntegrationFunctionType;}
	void IntegrationFunctionType(int newValue) {itsIntegrationFunctionType = newValue;}
private:
	NFmiDataIdent itsDataIdent;
	bool fUseDefaultProducer;
	NFmiCalculationCondition itsMaskCondition;
	NFmiAreaMask::CalculationOperationType itsOperationType; // tämä menee päällekkäin erilaisten maski ja info tyyppien kanssa, mutta piti tehdä 
						   // smarttooleja varten vielä tämmöinen mm. hoitamaan laskujärjestyksiä ja sulkuja jne.
	NFmiAreaMask::CalculationOperator itsCalculationOperator; // jos operation, tässä tieto mistä niistä on kyse esim. +, -, * jne.
	NFmiAreaMask::BinaryOperator itsBinaryOperator;
	NFmiInfoData::Type itsDataType; // jos kyseessä infoVariable, tarvitaan vielä datan tyyppi, että parametri saadaan tietokannasta (=infoOrganizerista)
	NFmiLevel *itsLevel; // mahd. level tieto, omistaa ja tuhoaa
	std::string itsMaskText; // originaali teksti, mistä tämä maskinfo on tulkittu
	NFmiAreaMask::FunctionType itsFunctionType; // onko mahd. funktio esim. min, max jne. (ei matemaattisia funktioita kuten sin, cos, pow, jne.)
	NFmiPoint itsOffsetPoint1; // esim. aikaoffset (x alku ja y loppu) tai paikkaoffset (alku x ja y offset)
	NFmiPoint itsOffsetPoint2; // paikkaoffset (loppu x ja y offset)
	NFmiAreaMask::MathFunctionType itsMathFunctionType;
	int itsIntegrationFunctionType; // 1=SumT tyylinen ja 2=SumZ tyylinen ja 3=MinH tyylinen funktio
};
#endif
