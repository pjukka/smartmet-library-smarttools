//**********************************************************
// C++ Class Name : NFmiSmartToolCalculation 
// ---------------------------------------------------------
// Filetype: (SOURCE)
// Filepath: G:/siirto/marko/oc/NFmiSmartToolCalculation.cpp 
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
#ifdef WIN32
#pragma warning(disable : 4786) // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

#include "NFmiSmartToolCalculation.h"
#include "NFmiAreaMaskInfo.h"
#include "NFmiSmartInfo.h"
#include "NFmiCalculationChangeFactorArray.h"
#include <algorithm>
#include <cassert>

using namespace std;

//--------------------------------------------------------
// Constructor/Destructor 
//--------------------------------------------------------
NFmiSmartToolCalculation::NFmiSmartToolCalculation(void)
:itsLowerLimit(0)
,itsUpperLimit(1)
,itsResultInfo(0)
,itsCalculations()
//,itsOperators()
,itsModificationFactors(0)
,fAllowMissingValueAssignment(false)
{
}

NFmiSmartToolCalculation::~NFmiSmartToolCalculation(void)
{
	Clear();
}
//--------------------------------------------------------
// Calculate 
//--------------------------------------------------------
void NFmiSmartToolCalculation::Calculate(const NFmiPoint &theLatlon, unsigned long theLocationIndex, const NFmiMetTime &theTime, int theTimeIndex)
{

	double value = eval_exp(theLatlon, theTime, theTimeIndex);
	if(value != kFloatMissing) // tuli ongelmia missing asetuksissa, pit‰‰ mietti vaikka jokin funktio, jolla asetetaan puuttuva arvo // pit‰‰ pysty‰ sittenkin asettamaan arvoksi kFloatMissing:in!!!
	{
		itsResultInfo->LocationIndex(theLocationIndex); // kohde dataa juoksutetaan, joten lokaatio indeksien pit‰‰ olla synkassa!!!
		value = GetInsideLimitsValue(static_cast<float>(value)); // asetetaan value viel‰ drawparamista satuihin rajoihin, ettei esim. RH voi olla alle 0 tai yli 100 %

		itsResultInfo->FloatValue(static_cast<float>(value)); // miten info saadaan osoittamaan oikeaan kohtaan?!?
	}
	else
	{
		if(fAllowMissingValueAssignment)
		{
			itsResultInfo->LocationIndex(theLocationIndex); // kohde dataa juoksutetaan, joten lokaatio indeksien pit‰‰ olla synkassa!!!
			itsResultInfo->FloatValue(static_cast<float>(value)); // nyt voidaan asettaa puuttuva arvo dataan
		}
	}
}

// ei ota huomioon missing arvoa, koska se pit‰‰ ottaa huomioon jo ennen t‰m‰n kutsua.
float NFmiSmartToolCalculation::GetInsideLimitsValue(float theValue)  
{
	if(theValue == kFloatMissing)
		return theValue;			

	if(theValue < itsLowerLimit)
		return itsLowerLimit;
	else if(theValue > itsUpperLimit)
		return itsUpperLimit;
	return theValue;			
}

void NFmiSmartToolCalculation::SetLimits(float theLowerLimit, float theUpperLimit)
{
	if(theLowerLimit >= theUpperLimit)
		 throw NFmiSmartToolCalculation::Exception("Parametrin max ja min rajat ovat v‰‰rin p‰in.");
	else
	{
		itsLowerLimit = theLowerLimit;
		itsUpperLimit = theUpperLimit;
	}
}

bool NFmiSmartToolCalculation::IsMasked(const NFmiPoint &theLatlon, int theLocationIndex, const NFmiMetTime &theTime, int theTimeIndex)
{
	return bin_eval_exp(theLatlon, theTime, theTimeIndex);
}

void NFmiSmartToolCalculation::Clear(void)
{
	delete itsResultInfo;
	itsResultInfo = 0;
	std::for_each(itsCalculations.begin(), itsCalculations.end(), PointerDestroyer());
	itsCalculations.clear();
//	itsOperators.clear();
}

//void NFmiSmartToolCalculation::AddCalculation(NFmiAreaMask* theCalculation, NFmiSmartToolCalculation::FmiCalculationOperators theOperation)
void NFmiSmartToolCalculation::AddCalculation(NFmiAreaMask* theCalculation)
{
	if(theCalculation)
	{
		itsCalculations.push_back(theCalculation);
//		itsOperators.push_back(theOperation);
	}
}

// globaali asetus luokka for_each-funktioon
struct TimeSetter
{
	TimeSetter(const NFmiMetTime &theTime):itsTime(theTime){}
	void operator()(NFmiAreaMask* theMask){theMask->Time(itsTime);}

	const NFmiMetTime &itsTime;
};

void NFmiSmartToolCalculation::SetTime(const NFmiMetTime &theTime)
{
	if(itsResultInfo) // maskin tapauksessa t‰m‰ on 0-pointteri (ja tulevaisuudessa t‰m‰ sijoitus info poistuu kokonaisuudessaan)
		itsResultInfo->Time(theTime); // t‰m‰n ajan asetuksen pit‰‰ onnistua editoitavalle datalle

	std::for_each(itsCalculations.begin(), itsCalculations.end(), TimeSetter(theTime));
}

// eval_exp-metodit otettu H. Schilbertin  C++: the Complete Refeference third ed.
// jouduin muuttamaan niit‰ v‰h‰n sopimaan t‰h‰n ymp‰ristˆˆn.
double NFmiSmartToolCalculation::eval_exp(const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex)
{
	double result = kFloatMissing;

//  exp_ptr = exp;
	token = 0; // nollataan aluksi 'token'
	itsCalcIterator = itsCalculations.begin(); 

	get_token();
	if(!token || token->GetCalculationOperationType() == NFmiAreaMask::NoType || token->GetCalculationOperationType() == NFmiAreaMask::EndOfOperations) 
		throw  NFmiSmartToolCalculation::Exception(string("Virheellinen laskusuoritus yritys, ei laskuja ollenkaan: \n") + GetCalculationText());

	eval_exp1(result, theLatlon, theTime, theTimeIndex);
	if(token->GetCalculationOperationType() != NFmiAreaMask::EndOfOperations) 
		throw  NFmiSmartToolCalculation::Exception(string("Virheellinen laskusuoritus yritys, lasku ei loppunut kuin olisi pit‰nyt: \n") + GetCalculationText());
	return result;
}

// Process an assignment.
// T‰m‰ on jo hoidettu toisella tavalla omassa koodissa, joten ohitan t‰m‰n,
// mutta j‰t‰n kommentteihin, jos tarvitsen tulevaisuudessa.
void NFmiSmartToolCalculation::eval_exp1(double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex)
{
/*
	int slot;
	char ttok_type;
	char temp_token[80];

	if(tok_type==VARIABLE) 
	{
		// save old token
		strcpy(temp_token, token);
		ttok_type = tok_type;

		// compute the index of the variable
		slot = toupper(*token) - 'A';

		get_token();
		if(*token != '=') 
		{
			putback(); // return current token
			// restore old token - not assignment
			strcpy(token, temp_token);
			tok_type = ttok_type;
		}
		else 
		{
			get_token(); // get next part of exp
			eval_exp2(result);
			vars[slot] = result;
			return;
		}
	}
*/
	eval_exp2(result, theLatlon, theTime, theTimeIndex);
}

// Add or subtract two terms.
void NFmiSmartToolCalculation::eval_exp2(double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex)
{
//	register char op;
	NFmiAreaMask::CalculationOperator op;
	double temp;

	eval_exp3(result, theLatlon, theTime, theTimeIndex);
	while((op = token->GetCalculationOperator()) == NFmiAreaMask::Add || op == NFmiAreaMask::Sub) 
	{
		get_token();
		eval_exp3(temp, theLatlon, theTime, theTimeIndex);

		if(result == kFloatMissing || temp==kFloatMissing)
		  result = kFloatMissing;
		else if(op == NFmiAreaMask::Sub)
		  result = result - temp;
		else	// Add
		  result = result + temp;
	}
}

// Multiply or divide two factors.
void NFmiSmartToolCalculation::eval_exp3(double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex)
{
//	register char op;
	NFmiAreaMask::CalculationOperator op;
	double temp;

	eval_exp4(result, theLatlon, theTime, theTimeIndex);
//	while((op = *token) == '*' || op == '/' || op == '%') 
	while((op = token->GetCalculationOperator()) == NFmiAreaMask::Mul || op == NFmiAreaMask::Div || op == NFmiAreaMask::Mod) 
	{
		get_token();
		eval_exp4(temp, theLatlon, theTime, theTimeIndex);

		if(result == kFloatMissing || temp==kFloatMissing)
		  result = kFloatMissing;
		else if(op == NFmiAreaMask::Mul)
		  result = result * temp;
		else if(op == NFmiAreaMask::Div)
		  result = (temp == 0 ? kFloatMissing : result / temp);
		else // NFmiAreaMask::Mod:
		  result = static_cast<int>(result) % static_cast<int>(temp);
	}
}

// Process an exponent
void NFmiSmartToolCalculation::eval_exp4(double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex)
{
	double temp, ex;

	eval_exp5(result, theLatlon, theTime, theTimeIndex);
//	if(*token== '^')
	if(token->GetCalculationOperator() == NFmiAreaMask::Pow)
	{
		get_token();
		eval_exp4(temp, theLatlon, theTime, theTimeIndex);
		ex = result;
		if(temp==0.0) 
		{
			result = 1.0;
			return;
		}
//		for(t=(int)temp-1; t>0; --t) 
//			result = result * (double)ex;
		if(result == kFloatMissing || temp==kFloatMissing)
		  result = kFloatMissing;
		else
		  result = pow(result, temp);
	}
}

// Evaluate a unary + or -.
void NFmiSmartToolCalculation::eval_exp5(double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex)
{
	NFmiAreaMask::CalculationOperator op = token->GetCalculationOperator();
	if(op == NFmiAreaMask::Add || op == NFmiAreaMask::Sub) 
		get_token();
	eval_exp6(result, theLatlon, theTime, theTimeIndex);

	if(op == NFmiAreaMask::Sub && result!=kFloatMissing)
		result = -result;
}

// Process a parenthesized expression.
void NFmiSmartToolCalculation::eval_exp6(double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex)
{
//	if((*token == '(')) 
	if(token->GetCalculationOperationType() == NFmiAreaMask::StartParenthesis)
	{
		get_token();
		eval_exp2(result, theLatlon, theTime, theTimeIndex);
//		if(*token != ')')
		if(token->GetCalculationOperationType() != NFmiAreaMask::EndParenthesis)
			throw  NFmiSmartToolCalculation::Exception(string("Sulut eiv‰t ole oikein laskussa: \n") + GetCalculationText());
		get_token();
	}
	else if(token->GetCalculationOperationType() == NFmiAreaMask::MathFunctionStart)
	{
		NFmiAreaMask::MathFunctionType function = token->GetMathFunctionType();
		get_token();
		eval_exp2(result, theLatlon, theTime, theTimeIndex);
//		if(*token != ')')
		if(token->GetCalculationOperationType() != NFmiAreaMask::EndParenthesis) // MathFunctionStart:in p‰‰tt‰‰ normaali lopetus sulku!
			throw  NFmiSmartToolCalculation::Exception(string("Sulut eiv‰t ole oikein laskussa matemaattisen funktion yhteydess‰: \n") + GetCalculationText());
		eval_math_function(result, function);
		get_token();
	}
	else 
		atom(result, theLatlon, theTime, theTimeIndex);
}

// HUOM! trigonometriset funktiot tehd‰‰n asteille, joten annettu luku pit‰‰ konvertoida
// c++ funktioille jotka odottavat kulmaa radiaaneille.
void NFmiSmartToolCalculation::eval_math_function(double &result, int theFunction)
{
  static const double trigFactor = 2 * kPii / 360;

  if(result != kFloatMissing)
	{
	  switch(NFmiAreaMask::MathFunctionType(theFunction))
		{
		case NFmiAreaMask::Exp:
		  result = exp(result);
		  break;
		case NFmiAreaMask::Sqrt:
		  result = sqrt(result);
		  break;
		case NFmiAreaMask::Log:
		  result = log(result);
		  break;
		case NFmiAreaMask::Log10:
		  result = log10(result);
		  break;
		case NFmiAreaMask::Sin:
		  result = sin(result * trigFactor); // konversio asteista radiaaneiksi teht‰v‰
		  break;
		case NFmiAreaMask::Cos:
			result = cos(result * trigFactor); // konversio asteista radiaaneiksi teht‰v‰
			break;
		case NFmiAreaMask::Tan:
		  result = tan(result * trigFactor); // konversio asteista radiaaneiksi teht‰v‰
		  break;
		case NFmiAreaMask::Sinh:
		  result = sinh(result);
		  break;
		case NFmiAreaMask::Cosh:
		  result = cosh(result);
			break;
		case NFmiAreaMask::Tanh:
		  result = tanh(result);
		  break;
		case NFmiAreaMask::Asin:
		  if(result >= -1 && result <= 1)
			result = asin(result)/trigFactor; // konversio radiaaneista asteiksi teht‰v‰
		  else
			result = kFloatMissing;
		  break;
		case NFmiAreaMask::Acos:
		  if(result >= -1 && result <= 1)
			result = acos(result)/trigFactor; // konversio radiaaneista asteiksi teht‰v‰
		  else
			result = kFloatMissing;
		  break;
		case NFmiAreaMask::Atan:
		  result = atan(result)/trigFactor; // konversio radiaaneista asteiksi teht‰v‰
		  break;
		case NFmiAreaMask::Ceil:
		  result = ceil(result);
		  break;
		case NFmiAreaMask::Floor:
		  result = floor(result);
		  break;
		case NFmiAreaMask::Round:
		  result = FmiRound(result);
		  break;
		case NFmiAreaMask::Abs:
		  result = fabs(result);
		  break;
		case NFmiAreaMask::Rand:
		  result = (static_cast<double>(rand()) / RAND_MAX) * result; // palauttaa luvun 0 ja result:in v‰lilt‰
		  break;
		default:
		  throw  NFmiSmartToolCalculation::Exception(string("Outo matemaattinen funktio, ohjelmointi vika: \n") + GetCalculationText());
		}
	}
}

void NFmiSmartToolCalculation::atom(double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex)
{
	NFmiAreaMask::CalculationOperationType opType = token->GetCalculationOperationType();
//	switch(tok_type) 
	switch(opType)
	{
	case NFmiAreaMask::InfoVariable:
	case NFmiAreaMask::Constant:
	case NFmiAreaMask::CalculatedVariable:
	case NFmiAreaMask::ModifyFactor:
	case NFmiAreaMask::RampFunction:
	case NFmiAreaMask::FunctionAreaIntergration:
	case NFmiAreaMask::FunctionTimeIntergration:
//		result = find_var(token);
		result = token->Value(theLatlon, theTime, theTimeIndex);
		get_token();
		return;
	default:
		throw  NFmiSmartToolCalculation::Exception(string("Muutuja/funktio puuttuu laskusta: ") + GetCalculationText());
	}
}

// ottaa seuraavan 'tokenin' kohdalle, mutta koska aluksi 
// itsCalcIterator osoittaa jo 1. tokeniin, tehd‰‰n ensin
// sijoitus tokeniin ja sitten siirret‰‰n iteraattoria eteenp‰in.
// T‰ll‰ lailla C++: Compl. Ref. kirjasta kopioitu koodi toimii 'sellaisenaan'.
void NFmiSmartToolCalculation::get_token(void)
{
	token = *itsCalcIterator;
	++itsCalcIterator;
}

// Laskun alustus, alustetaan iteraattori ja token.
bool NFmiSmartToolCalculation::bin_eval_exp(const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex)
{
	bool maskresult = true;
	double result = kFloatMissing;

//  exp_ptr = exp;
	token = 0; // nollataan aluksi 'token'
	itsCalcIterator = itsCalculations.begin(); 

	get_token();
	if(!token || token->GetCalculationOperationType() == NFmiAreaMask::NoType || token->GetCalculationOperationType() == NFmiAreaMask::EndOfOperations) 
		throw  NFmiSmartToolCalculation::Exception(string("Virheellinen laskusuoritus yritys, ei laskuja ollenkaan: \n") + GetCalculationText());

	bin_eval_exp1(maskresult, result, theLatlon, theTime, theTimeIndex);
	if(token->GetCalculationOperationType() != NFmiAreaMask::EndOfOperations) 
		throw  NFmiSmartToolCalculation::Exception(string("Virheellinen maskin-laskusuoritus yritys, Lasku ei loppunutkaan kuin olisi pit‰nyt: \n") + GetCalculationText());
	return maskresult;
}

// Process an assignment.
// T‰m‰ on jo hoidettu toisella tavalla omassa koodissa, joten ohitan t‰m‰n,
// mutta j‰t‰n kommentteihin, jos tarvitsen tulevaisuudessa.
void NFmiSmartToolCalculation::bin_eval_exp1(bool &maskresult, double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex)
{
/*
	int slot;
	char ttok_type;
	char temp_token[80];

	if(tok_type==VARIABLE) 
	{
		// save old token
		strcpy(temp_token, token);
		ttok_type = tok_type;

		// compute the index of the variable
		slot = toupper(*token) - 'A';

		get_token();
		if(*token != '=') 
		{
			putback(); // return current token
			// restore old token - not assignment
			strcpy(token, temp_token);
			tok_type = ttok_type;
		}
		else 
		{
			get_token(); // get next part of exp
			eval_exp2(result);
			vars[slot] = result;
			return;
		}
	}
*/
	bin_eval_exp1_1(maskresult, result, theLatlon, theTime, theTimeIndex);
}

// T‰m‰ pit‰‰ siirt‰‰ prioriteetissa alle yhteen laskun
// Process a binary expression. && ja || eli AND ja OR
void NFmiSmartToolCalculation::bin_eval_exp1_1(bool &maskresult, double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex)
{
//	register char op;
	NFmiAreaMask::BinaryOperator op;
	bool tempmask;
	double temp;

	bin_eval_exp1_2(maskresult, result, theLatlon, theTime, theTimeIndex);
	while((op = token->PostBinaryOperator()) == NFmiAreaMask::kAnd || op == NFmiAreaMask::kOr) 
	{
		get_token();
		bin_eval_exp1_2(tempmask, temp, theLatlon, theTime, theTimeIndex);
		switch(op) 
		{
		case NFmiAreaMask::kAnd:
			maskresult = (maskresult && tempmask);
			break;
		case NFmiAreaMask::kOr:
			maskresult = (maskresult || tempmask);
			break;
		default:
			throw  NFmiSmartToolCalculation::Exception(string("Outo  bin‰‰ri-operaattori maskissa:\n") + GetCalculationText());
		}
	}
}

// T‰m‰ pit‰‰ siirt‰‰ prioriteetissa alle yhteen laskun
// Process a comparison expression <, >, ==, <=, >=
void NFmiSmartToolCalculation::bin_eval_exp1_2(bool &maskresult, double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex)
{
//	register char op;
//	using NFmiAreaMask::CalculationOperator;
	FmiMaskOperation op;
	bool tempmask;
	double temp;

	bin_eval_exp2(maskresult, result, theLatlon, theTime, theTimeIndex);
//	while((op = *token) == '>' || op == '<' || op == '=') 
	while((op = token->Condition().Condition()) != kFmiNoMaskOperation) 
	{
		get_token();
		bin_eval_exp2(tempmask, temp, theLatlon, theTime, theTimeIndex);

		if(result == kFloatMissing || temp == kFloatMissing)
		  maskresult = false;
		else
		  {
			switch(op) 
			  {
			  case kFmiMaskEqual:
				maskresult = (result == temp);
				break;
			  case kFmiMaskGreaterThan:
				maskresult = (result > temp);
				break;
			  case kFmiMaskLessThan:
				maskresult = (result < temp);
				break;
			  case kFmiMaskGreaterOrEqualThan:
				maskresult = (result >= temp);
				break;
			  case kFmiMaskLessOrEqualThan:
				maskresult = (result <= temp);
				break;
			  case kFmiMaskNotEqual:
				maskresult = (result != temp);
				break;
			  default:
				throw  NFmiSmartToolCalculation::Exception(string("Outo vertailu operaattori maskissa:\n") + GetCalculationText());
			  }
		  }
	}
}

// Add or subtract two terms.
void NFmiSmartToolCalculation::bin_eval_exp2(bool &maskresult, double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex)
{
//	register char op;
	NFmiAreaMask::CalculationOperator op;
	double temp;

	bin_eval_exp3(maskresult, result, theLatlon, theTime, theTimeIndex);
	while((op = token->GetCalculationOperator()) == NFmiAreaMask::Add || op == NFmiAreaMask::Sub) 
	{
		get_token();
		bin_eval_exp3(maskresult, temp, theLatlon, theTime, theTimeIndex);
		if(result == kFloatMissing || temp == kFloatMissing)
		  result = kFloatMissing;
		else if(op == NFmiAreaMask::Sub)
		  result = result - temp;
		else // NFmiAreaMask::Add
		  result = result + temp;
	}
}

// Multiply or divide two factors.
void NFmiSmartToolCalculation::bin_eval_exp3(bool &maskresult, double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex)
{
//	register char op;
	NFmiAreaMask::CalculationOperator op;
	double temp;

	bin_eval_exp4(maskresult, result, theLatlon, theTime, theTimeIndex);
//	while((op = *token) == '*' || op == '/' || op == '%') 
	while((op = token->GetCalculationOperator()) == NFmiAreaMask::Mul || op == NFmiAreaMask::Div || op == NFmiAreaMask::Mod) 
	{
		get_token();
		bin_eval_exp4(maskresult, temp, theLatlon, theTime, theTimeIndex);
		if(result == kFloatMissing || temp == kFloatMissing)
		  result = kFloatMissing;
		else if(op == NFmiAreaMask::Mul)
		  result = result * temp;
		else if(op == NFmiAreaMask::Div)
		  result = (temp==0 ? kFloatMissing : result / temp);
		else // NFmiAreaMask::Mod
		  result = static_cast<int>(result) % static_cast<int>(temp);
	}
}

// Process an exponent
void NFmiSmartToolCalculation::bin_eval_exp4(bool &maskresult, double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex)
{
	double temp, ex;

	bin_eval_exp5(maskresult, result, theLatlon, theTime, theTimeIndex);
//	if(*token== '^')
	if(token->GetCalculationOperator() == NFmiAreaMask::Pow)
	{
		get_token();
		bin_eval_exp4(maskresult, temp, theLatlon, theTime, theTimeIndex);
		ex = result;
		if(temp==0.0) 
		{
			result = 1.0;
			return;
		}
//		for(t=(int)temp-1; t>0; --t) 
//			result = result * (double)ex;
		if(result == kFloatMissing || temp == kFloatMissing)
		  result = kFloatMissing;
		else
		  result = pow(result, temp);
	}
}

// Evaluate a unary + or -.
void NFmiSmartToolCalculation::bin_eval_exp5(bool &maskresult, double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex)
{
	NFmiAreaMask::CalculationOperator op = token->GetCalculationOperator();
	if(op == NFmiAreaMask::Add || op == NFmiAreaMask::Sub) 
		get_token();
	bin_eval_exp6(maskresult, result, theLatlon, theTime, theTimeIndex);

	if(op == NFmiAreaMask::Sub && result != kFloatMissing)
		result = -result;
}

// Process a parenthesized expression.
void NFmiSmartToolCalculation::bin_eval_exp6(bool &maskresult, double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex)
{
//	if((*token == '(')) 
	if(token->GetCalculationOperationType() == NFmiAreaMask::StartParenthesis)
	{
		get_token();
		bin_eval_exp1_1(maskresult, result, theLatlon, theTime, theTimeIndex);
//		if(*token != ')')
		if(token->GetCalculationOperationType() != NFmiAreaMask::EndParenthesis)
			throw  NFmiSmartToolCalculation::Exception(string("Sulut eiv‰t ole oikein laskussa: ") + GetCalculationText());
		get_token();
	}
	else if(token->GetCalculationOperationType() == NFmiAreaMask::MathFunctionStart)
	{
		NFmiAreaMask::MathFunctionType function = token->GetMathFunctionType();
		get_token();
		bin_eval_exp1_1(maskresult, result, theLatlon, theTime, theTimeIndex);
//		if(*token != ')')
		if(token->GetCalculationOperationType() != NFmiAreaMask::EndParenthesis) // MathFunctionStart:in p‰‰tt‰‰ normaali lopetus sulku!
			throw  NFmiSmartToolCalculation::Exception(string("Sulut eiv‰t ole oikein laskussa matemaattisen funktion yhteydess‰: \n") + GetCalculationText());
		eval_math_function(result, function);
		get_token();
	}
	else 
		bin_atom(maskresult, result, theLatlon, theTime, theTimeIndex);
}


void NFmiSmartToolCalculation::bin_atom(bool &maskresult, double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex)
{
	NFmiAreaMask::CalculationOperationType opType = token->GetCalculationOperationType();
//	switch(tok_type) 
	switch(opType)
	{
	case NFmiAreaMask::InfoVariable:
	case NFmiAreaMask::Constant:
	case NFmiAreaMask::CalculatedVariable:
	case NFmiAreaMask::ModifyFactor:
	case NFmiAreaMask::RampFunction:
	case NFmiAreaMask::FunctionAreaIntergration:
	case NFmiAreaMask::FunctionTimeIntergration:
//		result = find_var(token);
		result = token->Value(theLatlon, theTime, theTimeIndex);
		get_token();
		return;
	default:
		throw  NFmiSmartToolCalculation::Exception(string("Muutuja/funktio puuttuu laskusta: ") + GetCalculationText());
	}
}

void NFmiSmartToolCalculation::SetModificationFactors(std::vector<double> *theFactors)
{
	itsModificationFactors = theFactors;
	if(theFactors)
	{
		int size = itsCalculations.size();
		for(int i=0; i<size; i++)
		{
			if(itsCalculations[i] && itsCalculations[i]->GetCalculationOperationType() == NFmiAreaMask::ModifyFactor)
				static_cast<NFmiCalculationChangeFactorArray*>(itsCalculations[i])->SetChangeFactors(*theFactors);
		}
	}
}
