// ======================================================================
/*!
 *
 * C++ Class Name : NFmiSmartToolCalculation 
 * ---------------------------------------------------------
 * Filetype: (SOURCE)
 * Filepath: G:/siirto/marko/oc/NFmiSmartToolCalculation.cpp 
 * 
 * 
 * GDPro Properties 
 * ---------------------------------------------------
 *  - GD Symbol Type    : CLD_Class 
 *  - GD Method         : UML ( 4.0 ) 
 *  - GD System Name    : aSmartTools 
 *  - GD View Type      : Class Diagram 
 *  - GD View Name      : smarttools 1 
 * ---------------------------------------------------  
 *  Author         : pietarin 
 *  Creation Date  : Thur - Jun 20, 2002 
 * 
 *  Change Log     : 
 * 
 */
// ======================================================================

#ifdef WIN32
#pragma warning(disable : 4786) // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

#include "NFmiSmartToolCalculation.h"
#include "NFmiAreaMaskInfo.h"
#include "NFmiSmartInfo.h"
#include "NFmiDataModifierClasses.h"
#include "NFmiCalculationConstantValue.h"
#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <float.h>
#include "NFmiDictionaryFunction.h"


using namespace std;

//--------------------------------------------------------
// Constructor/Destructor 
//--------------------------------------------------------
NFmiSmartToolCalculation::NFmiSmartToolCalculation(void)
:itsLowerLimit(0)
,itsUpperLimit(1)
,fDoLimitCheck(true)
,itsResultInfo(0)
,itsCalculations()
,itsHeightValue(0)
//,itsOperators()
,fUseTimeInterpolationAlways(false)
,fUseHeightCalculation(false)
,fAllowMissingValueAssignment(false)
,fCircularValue(false)
,itsCircularValueModulor(kFloatMissing)
{
}

NFmiSmartToolCalculation::~NFmiSmartToolCalculation(void)
{
	Clear();
}

template<typename T>
static bool IsValidNumber(T theValue)
{
#ifdef UNIX
	if(isnan(theValue))
		return false;
	if(!finite(theValue))
		return false;
#else
	if(_isnan(theValue))
		return false;
	if(!_finite(theValue))
		return false;
#endif
	return true;
}

template<typename T>
static T MakeValidNumber(T theValue)
{
	if(IsValidNumber(theValue))
		return theValue;
	return kFloatMissing;
}


//--------------------------------------------------------
// Calculate 
//--------------------------------------------------------
void NFmiSmartToolCalculation::Calculate(const NFmiPoint &theLatlon, unsigned long theLocationIndex, const NFmiMetTime &theTime, int theTimeIndex)
{
	double value = eval_exp(theLatlon, theTime, theTimeIndex);

	value = MakeValidNumber(value); // muuttaa luvun missingiksi, jos nan tai +-inf
	if(value != kFloatMissing) // tuli ongelmia missing asetuksissa, pit‰‰ mietti vaikka jokin funktio, jolla asetetaan puuttuva arvo // pit‰‰ pysty‰ sittenkin asettamaan arvoksi kFloatMissing:in!!!
	{
		itsResultInfo->LocationIndex(theLocationIndex); // kohde dataa juoksutetaan, joten lokaatio indeksien pit‰‰ olla synkassa!!!
		value = FixCircularValues(value); // ensin tehd‰‰n circular tarkistus ja sitten vasta min/max
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

	if(fDoLimitCheck)
	{
		if(theValue < itsLowerLimit)
			return itsLowerLimit;
		else if(theValue > itsUpperLimit)
			return itsUpperLimit;
	}
	return theValue;			
}

void NFmiSmartToolCalculation::SetLimits(float theLowerLimit, float theUpperLimit, bool theDoLimitCheck)
{
	fDoLimitCheck = theDoLimitCheck;
	if(fDoLimitCheck)
	{
		if(theLowerLimit >= theUpperLimit)
			throw runtime_error(::GetDictionaryString("SmartToolCalculationErrorMinMaxLimits"));
		else
		{
			itsLowerLimit = theLowerLimit;
			itsUpperLimit = theUpperLimit;
		}
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
		throw runtime_error(::GetDictionaryString("SmartToolCalculationErrorNoCalculations") + ":\n" + GetCalculationText());

	eval_exp1(result, theLatlon, theTime, theTimeIndex);
	if(token->GetCalculationOperationType() != NFmiAreaMask::EndOfOperations) 
		throw runtime_error(::GetDictionaryString("SmartToolCalculationErrorInvalidEnding") + ":\n" + GetCalculationText());
	return result;
}

// Process an assignment.
// T‰m‰ on jo hoidettu toisella tavalla omassa koodissa, joten ohitan t‰m‰n,
// mutta j‰t‰n kommentteihin, jos tarvitsen tulevaisuudessa.
void NFmiSmartToolCalculation::eval_exp1(double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex)
{
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
		  result = temp == 0 ? kFloatMissing : static_cast<int>(result) % static_cast<int>(temp);
	}
}

// Process an exponent
void NFmiSmartToolCalculation::eval_exp4(double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex)
{
	double temp;

	eval_exp5(result, theLatlon, theTime, theTimeIndex);
	if(token->GetCalculationOperator() == NFmiAreaMask::Pow)
	{
		get_token();
		eval_exp4(temp, theLatlon, theTime, theTimeIndex);
		if(temp==0.0) 
		{
			result = 1.0;
			return;
		}
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
			throw runtime_error(::GetDictionaryString("SmartToolCalculationErrorInvalidParenthesis") + ":\n" + GetCalculationText());
		get_token();
	}
	else if(token->GetCalculationOperationType() == NFmiAreaMask::MathFunctionStart)
	{
		NFmiAreaMask::MathFunctionType function = token->GetMathFunctionType();
		get_token();
		eval_exp2(result, theLatlon, theTime, theTimeIndex);
//		if(*token != ')')
		if(token->GetCalculationOperationType() != NFmiAreaMask::EndParenthesis) // MathFunctionStart:in p‰‰tt‰‰ normaali lopetus sulku!
			throw runtime_error(::GetDictionaryString("SmartToolCalculationErrorInvalidMathParenthesis") + ":\n" + GetCalculationText());
		eval_math_function(result, function);
		get_token();
	}
	else if(token->GetCalculationOperationType() == NFmiAreaMask::ThreeArgumentFunctionStart)
	{
		NFmiAreaMask::FunctionType func = token->GetFunctionType(); // eli onko kyse min, max vai mist‰ funktiosta
		int integrationFunctionType = token->IntegrationFunctionType();
		get_token();
		double argument1 = kFloatMissing;
		eval_exp2(argument1, theLatlon, theTime, theTimeIndex);
		if(token->GetCalculationOperationType() != NFmiAreaMask::CommaOperator) // n‰iden funktioiden argumentit erotetaan pilkuilla
			throw runtime_error(::GetDictionaryString("SmartToolCalculationErrorInvalidParamSeparation") + ":\n" + GetCalculationText());
		get_token();
		double argument2 = kFloatMissing;
		eval_exp2(argument2, theLatlon, theTime, theTimeIndex);
		if(token->GetCalculationOperationType() != NFmiAreaMask::CommaOperator) // n‰iden funktioiden argumentit erotetaan pilkuilla
			throw runtime_error(::GetDictionaryString("SmartToolCalculationErrorInvalidParamSeparation") + ":\n" + GetCalculationText());
		eval_ThreeArgumentFunction(result, argument1, argument2, func, integrationFunctionType, theLatlon, theTime, theTimeIndex);

		if(token->GetCalculationOperationType() != NFmiAreaMask::EndParenthesis) // MathFunctionStart:in p‰‰tt‰‰ normaali lopetus sulku!
			throw runtime_error(::GetDictionaryString("SmartToolCalculationErrorInvalidMathParenthesis") + ":\n" + GetCalculationText());
		get_token();
	}
	else 
		atom(result, theLatlon, theTime, theTimeIndex);
}

void NFmiSmartToolCalculation::eval_ThreeArgumentFunction(double &result, double argument1, double argument2, NFmiAreaMask::FunctionType func, int theIntegrationFunctionType, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex)
{
	result = kFloatMissing;
	if(theIntegrationFunctionType == 2 || theIntegrationFunctionType == 3)
		eval_ThreeArgumentFunctionZ(result, argument1, argument2, func, theIntegrationFunctionType, theLatlon, theTime, theTimeIndex);
	else
	{
		if(argument1 != kFloatMissing && argument2 != kFloatMissing)
		{
			double value = kFloatMissing;
			// 1. ota talteen token tai se iteraattori, ett‰ samoja laskuja voidaan k‰yd‰ l‰pi uudestaan ja uudestaan
			CalcIter startCalcIterator = itsCalcIterator; // pit‰‰kˆ olla edellinen??, pit‰‰ olla ep‰selv‰‰, mutta pakko
			// 2. katso onko kyseess‰ aika- vai korkeus lasku ja haaraudu
			// 3. jos aikalasku, laske alkuaika ja loppu aika
			NFmiMetTime startTime(theTime);
			startTime.ChangeByHours(static_cast<long>(argument1));
			NFmiMetTime endTime(theTime);
			endTime.ChangeByHours(static_cast<long>(argument2));
			NFmiMetTime currentTime(startTime);
			// 4. mieti mik‰ on aikahyppy (oletus 1h) jos pelkk‰‰ EC-dataa, voisi aikahyppy olla 3h tai 6h
			int usedTimeResolutionInMinutes = 60;
			if(startTime <= endTime)
			{
				if(endTime.DifferenceInMinutes(startTime)/usedTimeResolutionInMinutes > 250)
					throw runtime_error(::GetDictionaryString("SmartToolCalculationErrorTimeCalcOverRun"));
				// 5. funktiosta riippuva datamodifier min, max jne.
				NFmiDataModifier* modifier = CreateIntegrationFuction(func); // t‰m‰ palauttaa aina jotain, tai heitt‰‰ poikkeuksen
				try
				{
					fUseTimeInterpolationAlways = true;
					do
					{
						// 6. muista aina asettaa token/laskuiteraattori 'alkuun'
						itsCalcIterator = startCalcIterator;
						get_token(); // pit‰‰ tehd‰ viel‰ t‰m‰, muuten osoittaa edelliseen pilkku-operaattoriin
						// 7. k‰y aika-loopissa l‰pi eval_exp2-laskut
						eval_exp2(value, theLatlon, currentTime, theTimeIndex); // theTimeIndex on nyt puppua
						// 8. sijoita tulos datamodifier:iin
						modifier->Calculate(static_cast<float>(value));
						// 9. 'next'
						currentTime.ChangeByMinutes(usedTimeResolutionInMinutes); 
					} while(currentTime <= endTime);
					// 10. loopin lopuksi pyyd‰ result datamodifier:ilta
					result = modifier->CalculationResult();
					delete modifier;
					fUseTimeInterpolationAlways = false;
				}
				catch(...)
				{
					fUseTimeInterpolationAlways = false;
					delete modifier;
					throw ;
				}
			}
			// Jos kyse level laskuista, juoksuta korkeuksia/leveleit‰ jotenkin ja tee samaa
		}
	}
}

static float GetCurrentHeightStep(float theHeight)
{
	float step = 100;
	if(theHeight < 150)
		step = 30;
	else if(theHeight < 300)
		step = 50;
	else if(theHeight < 1000)
		step = 100;
	else if(theHeight < 3000)
		step = 200;
	else if(theHeight < 10000)
		step = 500;
	else if(theHeight < 16000)
		step = 1000;
	else
		step = 2000;

	return step;
}

void NFmiSmartToolCalculation::eval_ThreeArgumentFunctionZ(double &result, double argument1, double argument2, NFmiAreaMask::FunctionType func, int theIntegrationFunctionType, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex)
{
	result = kFloatMissing;
	if(argument1 != kFloatMissing && argument2 != kFloatMissing)
	{
		double value = kFloatMissing;
		double heightValue = kFloatMissing; // t‰h‰n talletetaan h-funktion tapauksessa minimin/maksimin korkeus
		// 1. ota talteen token tai se iteraattori, ett‰ samoja laskuja voidaan k‰yd‰ l‰pi uudestaan ja uudestaan
		CalcIter startCalcIterator = itsCalcIterator; // pit‰‰kˆ olla edellinen??, pit‰‰ olla ep‰selv‰‰, mutta pakko
		// 3. jos korkeus lasku
		itsHeightValue = static_cast<float>(argument1);
		// 4. mieti mik‰ on aikahyppy (oletus 1h) jos pelkk‰‰ EC-dataa, voisi aikahyppy olla 3h tai 6h
		// HUOM!! Muuta resoluutio siten, ett‰ 50, kun korkeus alle 500 m, 100, kun korkeus alle 3000 m, 200 kun korkeus alle 10000 m ja 500 kun korkeus yli sen
		float usedHeightResolution = 100; 
		if(argument1 >= 0 && argument1 <= argument2)
		{
			if((argument2 - argument2) > 35000)
				throw runtime_error(::GetDictionaryString("SmartToolCalculationErrorHeightCalcOverRun"));
			// 5. funktiosta riippuva datamodifier min, max jne.
			NFmiDataModifier* modifier = CreateIntegrationFuction(func); // t‰m‰ palauttaa aina jotain, tai heitt‰‰ poikkeuksen
			try
			{
				fUseHeightCalculation = true;
				do
				{
					usedHeightResolution = GetCurrentHeightStep(itsHeightValue);
					NFmiCalculationDeltaZValue::SetDeltaZValue(usedHeightResolution);
					// 6. muista aina asettaa token/laskuiteraattori 'alkuun'
					itsCalcIterator = startCalcIterator;
					get_token(); // pit‰‰ tehd‰ viel‰ t‰m‰, muuten osoittaa edelliseen pilkku-operaattoriin
					// 7. k‰y aika-loopissa l‰pi eval_exp2-laskut
					eval_exp2(value, theLatlon, theTime, theTimeIndex); // theTimeIndex on nyt puppua
					// 8. sijoita tulos datamodifier:iin
					modifier->Calculate(static_cast<float>(value));
					if(theIntegrationFunctionType == 3)
					{
						if(value == modifier->CalculationResult())
							heightValue = itsHeightValue;
					}
					// 9. 'next'
					itsHeightValue += usedHeightResolution;
				} while(itsHeightValue <= argument2);
				// 10. loopin lopuksi pyyd‰ result datamodifier:ilta
				if(theIntegrationFunctionType == 3)
					result = heightValue; // eli sijoitetaan min/max arvon korkeus tulokseen jos kyseess‰ oli minh/maxh -funktio
				else
					result = modifier->CalculationResult();
				delete modifier;
				fUseHeightCalculation = false;
			}
			catch(...)
			{
				fUseHeightCalculation = false;
				delete modifier;
				throw ;
			}
		}
		// Jos kyse level laskuista, juoksuta korkeuksia/leveleit‰ jotenkin ja tee samaa
	}
}

// Muista jos tulee p‰ivityksi‰, smanlainen funktio lˆytyy myˆs NFmiSmartToolModifier-luokasta
NFmiDataModifier* NFmiSmartToolCalculation::CreateIntegrationFuction(NFmiAreaMask::FunctionType func)
{
	NFmiDataModifier* modifier = 0;
	switch(func)
	{
	case NFmiAreaMask::Avg:
		modifier = new NFmiDataModifierAvg;
		break;
	case NFmiAreaMask::Min:
		modifier = new NFmiDataModifierMin;
		break;
	case NFmiAreaMask::Max:
		modifier = new NFmiDataModifierMax;
		break;
	case NFmiAreaMask::Sum:
		modifier = new NFmiDataModifierSum;
		break;
		// HUOM!!!! Tee WAvg-modifier myˆs, joka on peritty Avg-modifierist‰ ja tee joku kerroin juttu painotukseen.
	default:
		throw runtime_error(::GetDictionaryString("SmartToolCalculationErrorIntegrationFunction"));
	}
	return modifier;
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
		  if(result >= 0) // neliˆjuurta ei saa ottaa negatiivisesta luvusta
		    result = sqrt(result);
		  else
			result = kFloatMissing;
		  break;
		case NFmiAreaMask::Log:
		  if(result > 0) // logaritmi pit‰‰ ottaa positiivisesta luvusta
		    result = log(result);
		  else
			result = kFloatMissing;
		  break;
		case NFmiAreaMask::Log10:
		  if(result > 0) // logaritmi pit‰‰ ottaa positiivisesta luvusta
		    result = log10(result);
		  else
			result = kFloatMissing;
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
		  throw  runtime_error(::GetDictionaryString("SmartToolCalculationErrorMathFunction") + ":\n" + GetCalculationText());
		}
	}
}

void NFmiSmartToolCalculation::atom(double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex)
{
	if(fUseHeightCalculation)
		result = token->HeightValue(itsHeightValue, theLatlon, theTime);
	else
		result = token->Value(theLatlon, theTime, theTimeIndex, fUseTimeInterpolationAlways);
	get_token();
}

// ottaa seuraavan 'tokenin' kohdalle, mutta koska aluksi 
// itsCalcIterator osoittaa jo 1. tokeniin, tehd‰‰n ensin
// sijoitus tokeniin ja sitten siirret‰‰n iteraattoria eteenp‰in.
// T‰ll‰ lailla C++: Compl. Ref. kirjasta kopioitu koodi toimii 'sellaisenaan'.
void NFmiSmartToolCalculation::get_token(void)
{
	if(itsCalcIterator != itsCalculations.end())
	{
		token = *itsCalcIterator;
		++itsCalcIterator;
	}
	else
	{
		throw  runtime_error(::GetDictionaryString("SmartToolCalculationErrorWrongEnding") + ":\n" + GetCalculationText());
	}
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
		throw  runtime_error(::GetDictionaryString("SmartToolCalculationErrorNoCalculations") + ":\n" + GetCalculationText());

	bin_eval_exp1(maskresult, result, theLatlon, theTime, theTimeIndex);
	if(token->GetCalculationOperationType() != NFmiAreaMask::EndOfOperations) 
		throw  runtime_error(::GetDictionaryString("SmartToolCalculationErrorConditionalEnding") + ":\n" + GetCalculationText());
	return maskresult;
}

// Process an assignment.
// T‰m‰ on jo hoidettu toisella tavalla omassa koodissa, joten ohitan t‰m‰n,
// mutta j‰t‰n kommentteihin, jos tarvitsen tulevaisuudessa.
void NFmiSmartToolCalculation::bin_eval_exp1(bool &maskresult, double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex)
{
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
			throw runtime_error(::GetDictionaryString("SmartToolCalculationErrorStrangeConditional") + ":\n" + GetCalculationText());
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
	NFmiAreaMask::CalculationOperationType opType1 = token->GetCalculationOperationType();

	bin_eval_exp2(maskresult, result, theLatlon, theTime, theTimeIndex);
//	while((op = *token) == '>' || op == '<' || op == '=') 
	while((op = token->Condition().Condition()) != kFmiNoMaskOperation) 
	{
		get_token();
		NFmiAreaMask::CalculationOperationType opType2 = token->GetCalculationOperationType();
		bin_eval_exp2(tempmask, temp, theLatlon, theTime, theTimeIndex);

		// resultit eiv‰t saa olla missin-arvoja, paitsi jos ne ovat Constant-maskista, eli halutaan nimenomaan verrata jotain missing-arvoon
		bool allowMissingComparison = (result == kFloatMissing && opType1 == NFmiAreaMask::Constant) || (temp == kFloatMissing && opType2 == NFmiAreaMask::Constant);
		bool missingValuesExist = (result == kFloatMissing) || (temp == kFloatMissing);
		if((!allowMissingComparison) && missingValuesExist)
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
				throw  runtime_error(::GetDictionaryString("SmartToolCalculationErrorStrangeComparison") + ":\n" + GetCalculationText());
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
		  result = temp==0 ? kFloatMissing : static_cast<int>(result) % static_cast<int>(temp);
	}
}

// Process an exponent
void NFmiSmartToolCalculation::bin_eval_exp4(bool &maskresult, double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex)
{
	double temp;

	bin_eval_exp5(maskresult, result, theLatlon, theTime, theTimeIndex);
	if(token->GetCalculationOperator() == NFmiAreaMask::Pow)
	{
		get_token();
		bin_eval_exp4(maskresult, temp, theLatlon, theTime, theTimeIndex);
		if(temp==0.0) 
		{
			result = 1.0;
			return;
		}
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
			throw runtime_error(::GetDictionaryString("SmartToolCalculationErrorInvalidParenthesis") + ":\n" + GetCalculationText());
		get_token();
	}
	else if(token->GetCalculationOperationType() == NFmiAreaMask::MathFunctionStart)
	{
		NFmiAreaMask::MathFunctionType function = token->GetMathFunctionType();
		get_token();
		bin_eval_exp1_1(maskresult, result, theLatlon, theTime, theTimeIndex);
//		if(*token != ')')
		if(token->GetCalculationOperationType() != NFmiAreaMask::EndParenthesis) // MathFunctionStart:in p‰‰tt‰‰ normaali lopetus sulku!
			throw runtime_error(::GetDictionaryString("SmartToolCalculationErrorInvalidMathParenthesis") + ":\n" + GetCalculationText());
		eval_math_function(result, function);
		get_token();
	}
	else if(token->GetCalculationOperationType() == NFmiAreaMask::ThreeArgumentFunctionStart)
	{ // huom! t‰ss‰ ei k‰ytet‰ bin_eval-kutsuja, koska t‰ss‰ lasketaan vain yksi luku, mik‰ palautetaan bin_eval-systeemiin
		NFmiAreaMask::FunctionType func = token->GetFunctionType(); // eli onko kyse min, max vai mist‰ funktiosta
		int integrationFunctionType = token->IntegrationFunctionType();
		get_token();
		double argument1 = kFloatMissing;
		eval_exp2(argument1, theLatlon, theTime, theTimeIndex);
		if(token->GetCalculationOperationType() != NFmiAreaMask::CommaOperator) // n‰iden funktioiden argumentit erotetaan pilkuilla
			throw  runtime_error(::GetDictionaryString("SmartToolCalculationErrorInvalidParamSeparation") + ":\n" + GetCalculationText());
		get_token();
		double argument2 = kFloatMissing;
		eval_exp2(argument2, theLatlon, theTime, theTimeIndex);
		if(token->GetCalculationOperationType() != NFmiAreaMask::CommaOperator) // n‰iden funktioiden argumentit erotetaan pilkuilla
			throw runtime_error(::GetDictionaryString("SmartToolCalculationErrorInvalidParamSeparation") + ":\n" + GetCalculationText());
		eval_ThreeArgumentFunction(result, argument1, argument2, func, integrationFunctionType, theLatlon, theTime, theTimeIndex);

		if(token->GetCalculationOperationType() != NFmiAreaMask::EndParenthesis) // MathFunctionStart:in p‰‰tt‰‰ normaali lopetus sulku!
			throw runtime_error(::GetDictionaryString("SmartToolCalculationErrorInvalidMathParenthesis") + ":\n" + GetCalculationText());
		get_token();
	}
	else 
		bin_atom(maskresult, result, theLatlon, theTime, theTimeIndex);
}


void NFmiSmartToolCalculation::bin_atom(bool &maskresult, double &result, const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex)
{
	if(fUseHeightCalculation)
		result = token->HeightValue(itsHeightValue, theLatlon, theTime);
	else
		result = token->Value(theLatlon, theTime, theTimeIndex, fUseTimeInterpolationAlways);
	get_token();
}

// tarkistaa onko resultinfon aktiivinen parametri kuten tuulen suunta
// ja tekee tarvittavat asetukset
void NFmiSmartToolCalculation::CheckIfModularParameter(void)
{
	fCircularValue = false;
	itsCircularValueModulor = kFloatMissing;
	if(itsResultInfo)
	{
		if(itsResultInfo->Param().GetParamIdent() == kFmiWindDirection)
		{
			fCircularValue = true;
			itsCircularValueModulor = 360;
		}
	}
}

double NFmiSmartToolCalculation::FixCircularValues(double theValue)
{
	if(fCircularValue && theValue != kFloatMissing)
	{
		if(theValue < 0)
			return itsCircularValueModulor - fmod(-theValue, itsCircularValueModulor);
		else
			return fmod(theValue, itsCircularValueModulor);
	}
	return theValue;
}

