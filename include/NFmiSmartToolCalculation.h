#pragma once
//**********************************************************
// C++ Class Name : NFmiSmartToolCalculation 
// ---------------------------------------------------------
//  Author         : pietarin 
//  Creation Date  : Thur - Jun 20, 2002 
// 
// T‰m‰ luokka hoitaa yhden laskurivin esim. T = T + 1
//**********************************************************

#include <string>
#include "NFmiAreaMask.h"
#include "NFmiPoint.h"
#include "NFmiDataMatrix.h"
#include "NFmiMetTime.h"
#include "boost/shared_ptr.hpp"

class NFmiFastQueryInfo;
class NFmiDataModifier;

class NFmiMacroParamValue
{
public:

	NFmiMacroParamValue(void):itsLatlon(),itsTime(),itsValue(kFloatMissing),itsPressureHeight(kFloatMissing),fSetValue(false),fDoCrossSectionCalculations(false)
	{}

	NFmiPoint itsLatlon;
	NFmiMetTime itsTime;
	float itsValue; // t‰h‰n talletetaan value
	float itsPressureHeight; // poikkileikkaus laskuissa k‰ytet‰‰n t‰t‰
	bool fSetValue; // t‰t‰ luokkaa k‰ytet‰‰n vain jos t‰m‰ on true
	bool fDoCrossSectionCalculations;
};

class NFmiSmartToolCalculation 
{
public:
	bool IsMasked(const NFmiCalculationParams &theCalculationParams);
	void Calculate(const NFmiCalculationParams &theCalculationParams, NFmiMacroParamValue &theMacroParamValue);
	void Calculate_ver2(const NFmiCalculationParams &theCalculationParams);
	void Time(const NFmiMetTime &theTime); // optimointia laskuja varten

	NFmiSmartToolCalculation(void);
	NFmiSmartToolCalculation(const NFmiSmartToolCalculation &theOther);
	~NFmiSmartToolCalculation(void);

	void SetResultInfo(const boost::shared_ptr<NFmiFastQueryInfo> &value) {itsResultInfo = value; CheckIfModularParameter();}
	boost::shared_ptr<NFmiFastQueryInfo> GetResultInfo(void) {return itsResultInfo;}
	checkedVector<boost::shared_ptr<NFmiAreaMask> >& GetCalculations(void) {return itsCalculations;}
	void AddCalculation(const boost::shared_ptr<NFmiAreaMask> &theCalculation);
	const std::string& GetCalculationText(void){return itsCalculationText;}
	void SetCalculationText(const std::string& theText){itsCalculationText = theText;}
	void SetLimits(float theLowerLimit, float theUpperLimit, bool theDoLimitCheck);
	bool AllowMissingValueAssignment(void){return fAllowMissingValueAssignment;};
	void AllowMissingValueAssignment(bool newState){fAllowMissingValueAssignment = newState;};

private:
	std::string itsCalculationText; // originaali teksti, mist‰ t‰m‰ lasku on tulkittu
	typedef checkedVector<boost::shared_ptr<NFmiAreaMask> >::iterator CalcIter;

	float GetInsideLimitsValue(float theValue);
	float itsLowerLimit; // n‰iden avulla kontrolloidaan mahdollisia min ja max arvoja
	float itsUpperLimit;
	bool fDoLimitCheck; // kaikille parametreille ei tehd‰ rajojen tarkistusta, esim. TotalWind ja W&C

// eval_exp-metodit otettu H. Schilbertin  C++: the Complete Refeference third ed.
// jouduin muuttamaan niit‰ v‰h‰n sopimaan t‰h‰n ymp‰ristˆˆn.
	double eval_exp(const NFmiCalculationParams &theCalculationParams);
	void eval_exp1(double &result, const NFmiCalculationParams &theCalculationParams);
	void eval_exp2(double &result, const NFmiCalculationParams &theCalculationParams);
	void eval_exp3(double &result, const NFmiCalculationParams &theCalculationParams);
	void eval_exp4(double &result, const NFmiCalculationParams &theCalculationParams);
	void eval_exp5(double &result, const NFmiCalculationParams &theCalculationParams);
	void eval_exp6(double &result, const NFmiCalculationParams &theCalculationParams);
	void eval_math_function(double &result, int theFunction);
	void eval_ThreeArgumentFunction(double &result, double argument1, double argument2, NFmiAreaMask::FunctionType func, int theIntegrationFunctionType, const NFmiCalculationParams &theCalculationParams);
	void eval_ThreeArgumentFunctionZ(double &result, double argument1, double argument2, NFmiAreaMask::FunctionType func, int theIntegrationFunctionType, const NFmiCalculationParams &theCalculationParams);
	boost::shared_ptr<NFmiDataModifier> CreateIntegrationFuction(NFmiAreaMask::FunctionType func);
	void atom(double &result, const NFmiCalculationParams &theCalculationParams);
	void get_token(void);
	boost::shared_ptr<NFmiAreaMask> token; // t‰ss‰ on kulloinenkin laskun osa tarkastelussa
	CalcIter itsCalcIterator; // get_token siirt‰‰ t‰t‰
	// Lis‰ksi piti maskia varten bin‰‰ri versio evaluaatio systeemist‰
	bool bin_eval_exp(const NFmiCalculationParams &theCalculationParams);
	void bin_eval_exp1(bool &maskresult, double &result, const NFmiCalculationParams &theCalculationParams);
	void bin_eval_exp1_1(bool &maskresult, double &result, const NFmiCalculationParams &theCalculationParams);
	void bin_eval_exp1_2(bool &maskresult, double &result, const NFmiCalculationParams &theCalculationParams);
	void bin_eval_exp2(bool &maskresult, double &result, const NFmiCalculationParams &theCalculationParams);
	void bin_eval_exp3(bool &maskresult, double &result, const NFmiCalculationParams &theCalculationParams);
	void bin_eval_exp4(bool &maskresult, double &result, const NFmiCalculationParams &theCalculationParams);
	void bin_eval_exp5(bool &maskresult, double &result, const NFmiCalculationParams &theCalculationParams);
	void bin_eval_exp6(bool &maskresult, double &result, const NFmiCalculationParams &theCalculationParams);
	void bin_atom(bool &maskresult, double &result, const NFmiCalculationParams &theCalculationParams);
	void CalcThreeArgumentFunction(double &result, const NFmiCalculationParams &theCalculationParams);
	void CalcVertFunction(double &result, const NFmiCalculationParams &theCalculationParams);

	boost::shared_ptr<NFmiFastQueryInfo> itsResultInfo; // omistaa+tuhoaa
	checkedVector<boost::shared_ptr<NFmiAreaMask> > itsCalculations; // omistaa+tuhoaa
	float itsHeightValue;
	float itsPressureHeightValue;

	bool fUseTimeInterpolationAlways; // uudet MINT, MAXT, jne vaativat aina aikainterpolointia, ja t‰m‰ flagi laitetaan silloin p‰‰lle 
									  // (t‰m‰ on jo k‰ytˆss‰ olevan optimoinnin toimivuuden kannalta pakko tehd‰ n‰in)
	bool fUseHeightCalculation; // atom-metodi kutsuu HeightValue-metodia, jos t‰m‰ on p‰‰ll‰
	bool fUsePressureLevelCalculation; // atom-metodi kutsuu PressureValue-metodia, jos t‰m‰ on p‰‰ll‰
	bool fAllowMissingValueAssignment; 

	// tuulen suuntaa varten pit‰‰ tehd‰ virityksi‰, ett‰ esim. 350 + 20 olisi 10 eik‰ 360 (eli maksimi) jne.
	bool fCircularValue;
	double itsCircularValueModulor;
	void CheckIfModularParameter(void);
	double FixCircularValues(double theValue);
};

