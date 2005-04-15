//**********************************************************
// C++ Class Name : NFmiSmartToolCalculation 
// ---------------------------------------------------------
// Filetype: (HEADER)
// Filepath: G:/siirto/marko/oc/NFmiSmartToolCalculation.h 
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
// T‰m‰ luokka hoitaa yhden laskurivin esim. T = T + 1
//**********************************************************
#ifndef  NFMISMARTTOOLCALCULATION_H
#define  NFMISMARTTOOLCALCULATION_H

#include <string>
#include "NFmiAreaMask.h"
#include "NFmiDataMatrix.h"

class NFmiPoint;
class NFmiMetTime;
class NFmiSmartInfo;
class NFmiDataModifier;

class NFmiSmartToolCalculation 
{
public:
	bool IsMasked(const NFmiPoint &theLatlon, int theLocationIndex, const NFmiMetTime &theTime, int theTimeIndex);
	void Calculate(const NFmiPoint &theLatlon, unsigned long theLocationIndex, const NFmiMetTime &theTime, int theTimeIndex);
	void SetTime(const NFmiMetTime &theTime); // optimointia laskuja varten

	NFmiSmartToolCalculation(void);
	~NFmiSmartToolCalculation(void);
	void Clear(void);

	void SetResultInfo(NFmiSmartInfo* value) {itsResultInfo = value; CheckIfModularParameter();}
	NFmiSmartInfo* GetResultInfo(void) {return itsResultInfo;}
	checkedVector<NFmiAreaMask*>* GetCalculations(void) {return &itsCalculations;}
	void AddCalculation(NFmiAreaMask* theCalculation);
	const std::string& GetCalculationText(void){return itsCalculationText;}
	void SetCalculationText(const std::string& theText){itsCalculationText = theText;}
	void SetLimits(float theLowerLimit, float theUpperLimit);
	bool AllowMissingValueAssignment(void){return fAllowMissingValueAssignment;};
	void AllowMissingValueAssignment(bool newState){fAllowMissingValueAssignment = newState;};

private:
	std::string itsCalculationText; // originaali teksti, mist‰ t‰m‰ lasku on tulkittu
	typedef checkedVector<NFmiAreaMask*>::iterator CalcIter;

	float GetInsideLimitsValue(float theValue);
	float itsLowerLimit; // n‰iden avulla kontrolloidaan mahdollisia min ja max arvoja
	float itsUpperLimit;

// eval_exp-metodit otettu H. Schilbertin  C++: the Complete Refeference third ed.
// jouduin muuttamaan niit‰ v‰h‰n sopimaan t‰h‰n ymp‰ristˆˆn.
	double eval_exp(const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);
	void eval_exp1(double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);
	void eval_exp2(double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);
	void eval_exp3(double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);
	void eval_exp4(double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);
	void eval_exp5(double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);
	void eval_exp6(double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);
	void eval_math_function(double &result, int theFunction);
	void eval_ThreeArgumentFunction(double &result, double argument1, double argument2, NFmiAreaMask::FunctionType func, int theIntegrationFunctionType, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);
	void eval_ThreeArgumentFunctionZ(double &result, double argument1, double argument2, NFmiAreaMask::FunctionType func, int theIntegrationFunctionType, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);
	NFmiDataModifier* CreateIntegrationFuction(NFmiAreaMask::FunctionType func);
	void atom(double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);
	void get_token(void);
	NFmiAreaMask* token; // t‰ss‰ on kulloinenkin laskun osa tarkastelussa
	CalcIter itsCalcIterator; // get_token siirt‰‰ t‰t‰
	// Lis‰ksi piti maskia varten bin‰‰ri versio evaluaatio systeemist‰
	bool bin_eval_exp(const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);
	void bin_eval_exp1(bool &maskresult, double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);
	void bin_eval_exp1_1(bool &maskresult, double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);
	void bin_eval_exp1_2(bool &maskresult, double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);
	void bin_eval_exp2(bool &maskresult, double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);
	void bin_eval_exp3(bool &maskresult, double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);
	void bin_eval_exp4(bool &maskresult, double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);
	void bin_eval_exp5(bool &maskresult, double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);
	void bin_eval_exp6(bool &maskresult, double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);
	void bin_atom(bool &maskresult, double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);

	NFmiSmartInfo* itsResultInfo; // omistaa+tuhoaa
	checkedVector<NFmiAreaMask*> itsCalculations; // omistaa+tuhoaa
	float itsHeightValue;

	bool fUseTimeInterpolationAlways; // uudet MINT, MAXT, jne vaativat aina aikainterpolointia, ja t‰m‰ flagi laitetaan silloin p‰‰lle 
									  // (t‰m‰ on jo k‰ytˆss‰ olevan optimoinnin toimivuuden kannalta pakko tehd‰ n‰in)
	bool fUseHeightCalculation; // atom-metodi kutsuu HeightValue-metodia, jos t‰m‰ on p‰‰ll‰
	bool fAllowMissingValueAssignment; 

	// tuulen suuntaa varten pit‰‰ tehd‰ virityksi‰, ett‰ esim. 350 + 20 olisi 10 eik‰ 360 (eli maksimi) jne.
	bool fCircularValue;
	double itsCircularValueModulor;
	void CheckIfModularParameter(void);
	double FixCircularValues(double theValue);
};
#endif
