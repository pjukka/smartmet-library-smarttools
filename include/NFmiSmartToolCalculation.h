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
// Tämä luokka hoitaa yhden laskurivin esim. T = T + 1
//**********************************************************
#ifndef  NFMISMARTTOOLCALCULATION_H
#define  NFMISMARTTOOLCALCULATION_H

#include <vector>
#include <string>

class NFmiPoint;
class NFmiMetTime;
//class NFmiFastQueryInfo;
class NFmiSmartInfo;
class NFmiAreaMask;

class NFmiSmartToolCalculation 
{
public:
	class Exception
	{
	public:
		Exception(const std::string &theText):itsText(theText){}
		const std::string& What(void){return itsText;}
	private:
		const std::string itsText;
	};

	bool IsMasked(const NFmiPoint &theLatlon, int theLocationIndex, const NFmiMetTime &theTime, int theTimeIndex);
	void Calculate(const NFmiPoint &theLatlon, unsigned long theLocationIndex, const NFmiMetTime &theTime, int theTimeIndex);
	void SetTime(const NFmiMetTime &theTime); // optimointia laskuja varten
	void SetModificationFactors(std::vector<double> *theFactors);

	NFmiSmartToolCalculation(void);
	~NFmiSmartToolCalculation(void);
	void Clear(void);

	void SetResultInfo(NFmiSmartInfo* value) {itsResultInfo = value;}
	NFmiSmartInfo* GetResultInfo(void) {return itsResultInfo;}
	std::vector<NFmiAreaMask*>* GetCalculations(void) {return &itsCalculations;}
//	void AddCalculation(NFmiAreaMask* theCalculation, CalculationOperators theOperation);
	void AddCalculation(NFmiAreaMask* theCalculation);
//	std::vector<CalculationOperators>* GetOperators(void) {return &itsOperators;}
	const std::string& GetCalculationText(void){return itsCalculationText;}
	void SetCalculationText(const std::string& theText){itsCalculationText = theText;}
	void SetLimits(float theLowerLimit, float theUpperLimit) throw (NFmiSmartToolCalculation::Exception);

private:
	std::string itsCalculationText; // originaali teksti, mistä tämä lasku on tulkittu
	typedef std::vector<NFmiAreaMask*>::iterator CalcIter;

	float GetInsideLimitsValue(float theValue);
	float itsLowerLimit; // näiden avulla kontrolloidaan mahdollisia min ja max arvoja
	float itsUpperLimit;

// eval_exp-metodit otettu H. Schilbertin  C++: the Complete Refeference third ed.
// jouduin muuttamaan niitä vähän sopimaan tähän ympäristöön.
	double eval_exp(const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex) throw (NFmiSmartToolCalculation::Exception);
	void eval_exp1(double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);
	void eval_exp2(double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);
	void eval_exp3(double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);
	void eval_exp4(double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);
	void eval_exp5(double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);
	void eval_exp6(double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex) throw (NFmiSmartToolCalculation::Exception);
	void eval_math_function(double &result, int theFunction) throw (NFmiSmartToolCalculation::Exception);
	void atom(double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex) throw (NFmiSmartToolCalculation::Exception);
	void get_token(void);
	NFmiAreaMask* token; // tässä on kulloinenkin laskun osa tarkastelussa
	CalcIter itsCalcIterator; // get_token siirtää tätä
	// Lisäksi piti maskia varten binääri versio evaluaatio systeemistä
	bool bin_eval_exp(const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex) throw (NFmiSmartToolCalculation::Exception);
	void bin_eval_exp1(bool &maskresult, double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);
	void bin_eval_exp1_1(bool &maskresult, double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex) throw (NFmiSmartToolCalculation::Exception);
	void bin_eval_exp1_2(bool &maskresult, double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex) throw (NFmiSmartToolCalculation::Exception);
	void bin_eval_exp2(bool &maskresult, double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);
	void bin_eval_exp3(bool &maskresult, double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);
	void bin_eval_exp4(bool &maskresult, double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);
	void bin_eval_exp5(bool &maskresult, double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);
	void bin_eval_exp6(bool &maskresult, double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex) throw (NFmiSmartToolCalculation::Exception);
	void bin_atom(bool &maskresult, double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex) throw (NFmiSmartToolCalculation::Exception);

	NFmiSmartInfo* itsResultInfo; // omistaa+tuhoaa
	std::vector<NFmiAreaMask*> itsCalculations; // omistaa+tuhoaa
//	std::vector<CalculationOperators> itsOperators;

	std::vector<double> *itsModificationFactors; // mahdolliset aikasarja muokkaus kertoimet (ei omista, ei tuhoa)
};
#endif
