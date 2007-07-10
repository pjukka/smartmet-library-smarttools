// ======================================================================
/*!
 * \file NFmiSoundingFunctions.cpp
 *
 * Namespace, jossa erilaisia irto funktioita, joiden avulla lasketaan
 * erilaisia mm. luotauksiin liittvi‰ juttuja.
 */
// ======================================================================

#include "NFmiSoundingFunctions.h"
#include "NFmiSoundingData.h"
#include "NFmiAngle.h"
#include "NFmiValueString.h"
#include "NFmiInterpolation.h"

namespace NFmiSoundingFunctions
{

// Funktio laskee kastepisteen (DP) l‰mpˆtilan (T) ja suhteellisen
// kosteuden (RH) avulla. L‰mpˆtila ja kastepiste ovat kelvinein‰.
double CalcDP(double T, double RH)
{
	static bool firstTime = true;
	const int arraySize = 1001;
	static double gLogTable0_100[arraySize] = {0}; // taulukossa kymmenyksen tarkkuudella logaritmit (size 1001)
	if(firstTime)
	{
		for(int i = 0; i<arraySize; i++)
			gLogTable0_100[i] = ::log(i*0.1f/100.f);
		firstTime = false;
	}
	const double b = 17.27f;
	const double c = 237.3f;
//	double x = (log(RH/100.) + b * (T / (T + c))) / b;

	int logIndex = FmiRound(RH*10);
	if(T != kFloatMissing && RH != kFloatMissing && logIndex > 0 && logIndex < arraySize) // logIndex pit‰‰ olla > 0 koska 0-kohdassa on not a number!
	{
		double x = (gLogTable0_100[logIndex] + b * (T / (T + c))) / b;
		if(x != 1) // tarkistus ettei tule nollalla jakoa
		{
			double dp = c * x/(1-x);
			return dp;
		}
	}
	return kFloatMissing;
}

double ESW(double T)
{
	double ES0 = 6.1078;
    double POL = 0.99999683    + T*(-0.90826951E-02 +
           T*(0.78736169E-04   + T*(-0.61117958E-06 +
           T*(0.43884187E-08   + T*(-0.29883885E-10 +
           T*(0.21874425E-12   + T*(-0.17892321E-14 +
           T*(0.11112018E-16   + T*(-0.30994571E-19)))))))));
	double ESW = ES0/(::pow(POL, 8));
	return ESW;
}

double WMR(double T, double P)
{
	double EPS = 0.62197;
	double X = 0.02*(T-12.5+7500./P);
	double WFW = 1.+4.5E-06*P+1.4E-03*X*X;
	double FWESW = WFW*ESW(T);
	double R = EPS*FWESW/(P-FWESW);
	double WMR = 1000.*R;
	return WMR;
}

// http://www.iac.ethz.ch/staff/dominik/idltools/atmos_phys/
double ESAT(double T)
{
	double T0 = 0.;
	if(T < 105.)
		T0=273.15;

// Formula with T = temperature in K
//   esat = exp( -6763.6/(T+T0) - 4.9283*alog((T+T0)) + 54.2190 )

// Formula close to that of Magnus, 1844 with temperature TC in Celsius
//    ESAT = 6.1078 * EXP( 17.2693882 * TC / (TC + 237.3) ) ; TC in Celsius

// or Emanuel's formula (also approximation in form of Magnus' formula,
// 1844), which was taken from Bolton, Mon. Wea. Rev. 108, 1046-1053, 1980.
// This formula is very close to Goff and Gratch with differences of
// less than 0.25% between -50 and 0 deg C (and only 0.4% at -60degC)
//    esat=6.112*EXP(17.67*TC/(243.5+TC))

// WMO reference formula is that of Goff and Gratch (1946), slightly
// modified by Goff in 1965:

    double e1=1013.250;
    double TK=273.15;
	double esat=e1* ::pow(10., (10.79586*(1-TK/(T+T0))-5.02808* ::log10((T+T0)/TK)+
		1.50474*1e-4*(1- ::pow(10.,(-8.29692*((T+T0)/TK-1))))+
		0.42873*1e-3*(::pow(10., (4.76955*(1-TK/(T+T0))))-1)-2.2195983));

    return esat;
}

// http://www.iac.ethz.ch/staff/dominik/idltools/atmos_phys/
double MIXR_SAT(double T, double P)
{
   double Mw=18.0160; // molec mass of water
   double Md=28.9660; // molec mass of dry air

   double X = ESAT(T); // Note: ESAT accepts temperatures in Celsius or K
   double MIXR_SAT = Mw/Md* X*1000. / (P - X);
   return MIXR_SAT;
}

// l‰hde: http://www.iac.ethz.ch/staff/dominik/idltools/atmos_phys/skewt.pro
//;========================================================================
//;  FUNCTION TO COMPUTE THE TEMPERATURE (KELVIN) OF AIR AT A GIVEN
//;  PRESSURE AND WITH A GIVEN MIXING RATIO.
//;  TMR(KELVIN), W(GRAMS WATER VAPOR/KILOGRAM DRY AIR), P(MILLIBAR)
//      FUNCTION  TMR, W, P
//         X   =  ALOG10 ( W * P / (622.+ W) )
//         TMR = 10. ^ ( .0498646455 * X + 2.4082965 ) - 7.07475 + $
//               38.9114 * ( (10.^( .0915 * X ) - 1.2035 )^2 )
//      RETURN, TMR
//      END
//;========================================================================
double TMR(double W, double P)
{
	double X   =  ::log10( W * P / (622.+ W) );
	double TMR = ::pow(10., ( .0498646455 * X + 2.4082965 )) - 7.07475 + 38.9114 * ( ::pow((::pow(10.,( .0915 * X )) - 1.2035 ), 2 ));
	return TMR - 273.16; // HUOM! lopussa muutetaan kuitenkin celsiuksiksi!!
}

// l‰hde: http://www.iac.ethz.ch/staff/dominik/idltools/atmos_phys/skewt.pro
//;========================================================================
//;  FUNCTION TO COMPUTE SATURATION ADIABATIC TEMP AT 1000 MB GIVEN T & P.
//;  OS AND T (KELVIN or CELSIUS), P (MILLIBARS )
//      FUNCTION  OS, T, P
//	 TK = T + 273.15*(T LT 100.)	; convert to Kelvin if necessary
//         OS = TK * ((1000./P)^.286) / (EXP( -2.6518986*MIXR_SAT(TK,P)/TK) )
//      RETURN, OS
//      END
//;========================================================================
double OS(double T, double P)
{
	double TK = (T < 100) ? T + 273.15 : T;	//; convert to Kelvin if necessary
	double OS = TK * (::pow((1000./P), .286)) / (::exp( -2.6518986*MIXR_SAT(TK, P)/TK) );
    return OS - 273.16; // Muutetaan takaisin celsiuksiksi
}


// l‰hde: http://www.iac.ethz.ch/staff/dominik/idltools/atmos_phys/skewt.pro
//;========================================================================
//;  FUNCTION TO COMPUTE TEMPERATUE (KELVIN) OF A MOIST ADIABAT GIVEN
//;  OS(KELVIN), P(MILLIBARS)
//;  SIGN(A,B) REPLACES THE ALGEBRAIC SIGN OF A WITH THE SIGN OF B
//      FUNCTION TSA, OS, P
//          A  = OS
//          TQ = 253.16
//          D  = 120.
//          FOR  I = 1,12 DO BEGIN	; iterations
//             D = D/2.
//;  IF THE TEMPERATURE DIFFERENCE, X, IS SMALL, EXIT THIS LOOP.
//             X = A * EXP (-2.6518986*MIXR_SAT(TQ,P)/TQ)-TQ*((1000./P)^.286)
//             IF ( ABS(X) LT 0.01 ) THEN GOTO, JUMP2
//	     D = - (X LT 0)*ABS(D)+(X GT 0)*ABS(D)
//             TQ = TQ + D
//          ENDFOR
//JUMP2:    TSA=TQ
//      RETURN, TSA
//      END
//;========================================================================
double TSA(double OS, double P)
{
	double A = OS + 273.16; // muutetaan t‰ss‰ kelvineiksi
	double TQ = 253.16;
	double D = 120.;
	for(int I = 1; I < 12; I++)
	{
		D = D/2.;
		double X = A * ::exp(-2.6518986*MIXR_SAT(TQ,P)/TQ)-TQ*(::pow((1000./P), .286));
		//;  IF THE TEMPERATURE DIFFERENCE, X, IS SMALL, EXIT THIS LOOP.
		if( ::fabs(X) < 0.01 )
			break;
		D = (X < 0) ? -fabs(D) : fabs(D); // h‰m‰r‰‰ koodia alkuper‰isess‰ kieless‰
		TQ = TQ + D;
	}
	double TSA=TQ;
	return TSA - 273.16; // Muutetaan takaisin celsiuksiksi
}

// Laskee annetun potentiaalil‰mpˆtilan ja paineen avulla l‰mpˆtilan haluttuun korkeuteen.
// Se on k‰‰nteinen funktio CalcThetaT:n verrattuna.
// tpot annetaan celsiuksina.
// Oletus: kaikki parametrit ovat ei-puuttuvia!
double Tpot2t(double tpot, double p)
{
	const double T0 = 273.16; // kelvin asteikon muunnos
	double t1 = T0 + tpot;
	double t = t1 * ::pow(p/1000, 0.2854);
	return t - T0;
}

// Laskee potentiaalil‰mpˆtila theta kun annetaan l‰mpˆtila ja paine, miss‰ l‰mpˆtila on otettu.
// T annetaan celsiuksina.
// Oletus: kaikki parametrit ovat ei-puuttuvia!
double T2tpot(double T, double P)
{
	const double T0 = 273.16; // kelvin asteikon muunnos
	return ((T+T0) * ::pow(1000/P, 0.2854)) - T0;
}

// laskee suhteellisen kosteuden l‰mpˆtilan ja kastepisteen avulla.
// Oletus: kaikki parametrit ovat ei-puuttuvia!
// L‰mpˆtila ja kastepiste celsiuksina.
double CalcRH(double T, double Td)
{
	double RH = 100 * ::pow((112-0.1*T+Td)/(112+0.9*T) ,8);
	return RH;
}

// Laskee kyll‰stysvesihˆyrynpaineen es l‰mpˆtilan avulla.
// Oletus: kaikki parametrit ovat ei-puuttuvia!
// L‰mpˆtila celsiuksina.
double CalcEs(double T)
{
	const double T0 = 273.16; // kelvin asteikon muunnos
	const double Rv = 462; // vesihˆyryn kaasuvakio [J/kg/K]
	const double L = 2.5 * 1000000; // vesihˆyryn tiivistymisl‰mpˆ [J/kg]
	double es = 6.11 * ::exp(L/Rv*(1/T0 - 1/(T+T0)));
	return es;
}

// Laskee kyll‰stysvesihˆyrynpaineen es l‰mpˆtilan avulla. Saulin lˆyt‰m‰ 'tarkka' kaava.
// Oletus: kaikki parametrit ovat ei-puuttuvia!
// L‰mpˆtila celsiuksina.
double CalcEs2(double Tcelsius)
{
	const double b = 17.2694;
	const double e0 = 6.11; // 6.11 <- 0.611 [kPa]
	const double T1 = 273.16; // [K]
	const double T2 = 35.86; // [K]

	double T = Tcelsius + T1;
	double nume = b * (T-T1);
	double deno = (T-T2);

	double es = e0 * ::exp(nume/deno);
	return es;
}

// Laskee vesihˆyryn osapaineen (e)
// Oletus: kaikki parametrit ovat ei-puuttuvia!
double CalcE(double RH, double es)
{
	double e = RH * es / 100;
	return e;
}

// Laskee vesihˆyryn osapaineen (e) ja kyll‰styspaineen (es)
// avulla suhteellisen kosteuden RH.
// Oletus: kaikki parametrit ovat ei-puuttuvia!
double CalcRH2(double e, double es)
{
	double RH = e/es * 100;
	return RH;
}

// Laskee sekoitussuhteen w (mixing ratio) [g/kg]
// Oletus: kaikki parametrit ovat ei-puuttuvia!
double CalcW(double e, double P)
{
	double w = 0.622 * e/P * 1000;
	return w;
}

// Laskee sekoitussuhteen w (mixing ratio) [g/kg]
// ja paineen avulla vesihˆyryn paineen e
// Oletus: kaikki parametrit ovat ei-puuttuvia!
double CalcE2(double w, double P)
{
	double e = w*P/(0.622 * 1000);
	return e;
}

// Laskee sekoitussuhteen w (mixing ratio) [g/kg].
// Oletus: kaikki parametrit ovat ei-puuttuvia!
double CalcMixingRatio(double T, double Td, double P)
{
	double RH = CalcRH(T, Td);
	double es = CalcEs2(T);
	double e = CalcE(RH, es);
	double w = CalcW(e, P);
	return w;
}

// Laskee kastepisteen.
// Oletus: kaikki parametrit ovat ei-puuttuvia!
double CalcDewPoint(double T, double w, double P)
{
	double es = CalcEs2(T);
	double e = CalcE2(w, P);
	double RH = CalcRH2(e, es);
	double Td = CalcDP(static_cast<float>(T), static_cast<float>(RH));
	return Td;
}

// Laskee virtuaali l‰mpˆtilan Tv
// Oletus: kaikki parametrit ovat ei-puuttuvia!
double CalcTv(double T, double Td, double P)
{
	double w = CalcMixingRatio(T, Td, P);
	double Tv = (1. + 0.61 * w) * T;
	return Tv;
}


// Equivalent potential temperature (Theta-E)
double CalcThetaE(double T, double Td, double P)
{
	double tpot = T2tpot(T, P);
	double w = CalcMixingRatio(T, Td, P);
	double thetae = tpot + 3 * w;
	return thetae;
}

// Laskee LCL-levelin T, Td, ja 'aloitus' P:n avulla
double CalcLCLPressure(double T, double Td, double P)
{
	double lclPressure = kFloatMissing;
	// 2. Laske sekoitussuhde pinnalla
	double w = CalcMixingRatio(T, Td, P);
	double tpot = T2tpot(T, P); // pit‰‰ laskea mit‰ l‰mpˆtilaa vastaa pinnan 'potentiaalil‰mpˆtila'
	// 3. iteroi pinnasta ylˆsp‰in ja laske, milloin mixing-ratio k‰yr‰ ja l‰mpˆtilan kostea-adiapaatti leikkaavat
	double lastP=P;
	for(double currentP = P; currentP > 100; currentP -= 1)
	{
		double Tw = TMR(w, currentP);
		double Tdry = Tpot2t(tpot, currentP);

		if(Tdry < Tw)
			break;
		lastP = currentP; // viimeisinta painetta ennen kuin Tw ja Tdry ovat leikanneet, voidaan k‰ytt‰‰ tarkemman LCL painepinnan interpolointiin
	}

	// laske tarkempi paine jos viitsit lastP ja currentP;n avulla interpoloimalla
	lclPressure = lastP;
	return lclPressure;
}

// etsii monotonisen 'funktion' juurta kun sille on annettu kaksi funktion pistett‰
double FindRoot(double x1, double x2, double y1, double y2)
{
	double x0 = x1 - y1*((x1-x2)/(y1-y2));
	return x0;
}

// laskee kostea adiapaattisen arvon (iteroimalla), kun annetaan
// l‰mpˆtila (celcius) ja paine (mb).
// Oletus: parametrit ovat ei puuttuvia.
// Etsii kostea-adiapaattiarvoa alueelta -40 - 50 astetta celsiusta.
// HUOM!!!!! t‰m‰ voi varmaan optimoida jollain puolitus haulla!?!?, luulen ett‰ t‰m‰ hidastaa luotaus piirtoa rankasti!
double CalcMoistT(double T, double P)
{
	double minDiff = 999999.;
	double minDiffMoistT = kFloatMissing;
	double deltaT = 0.1;
	double moistT1 = -40;
	double moistT2 = 50;
	int counter = 0;
	do
	{
		double AOS1 = OS(moistT1, 1000.); // t‰m‰ on h‰m‰r‰, miksi t‰m‰ tehd‰‰n, mutta muutetaan kostea-l‰mpp‰si jotenkin pintaan
		double ATSA1  = TSA(AOS1, P); // lasketaan sen avulla l‰mpˆtila taas halutulle korkeudelle
		double diff1 = ATSA1 - T;
		double AOS2 = OS(moistT2, 1000.);
		double ATSA2  = TSA(AOS2, P);
		double diff2 = ATSA2 - T;
		double moistT0 = FindRoot(moistT1, moistT2, diff1, diff2);
		double AOS0 = OS(moistT0, 1000.);
		double ATSA0  = TSA(AOS0, P);
		double diff0 = ATSA0 - T;
		if(::fabs(diff0) < minDiff)
		{ // jos laskettu l‰mpˆtila oli tarpeeksi l‰hell‰ annettua, laitetaan arvo talteen
			minDiff = ::fabs(diff0);
			minDiffMoistT = moistT0;
		}
		if(diff1 < 0 && diff0 < 0) // katsotaan kumman kanssa etumerkki on sama ja alustetaan iterointi alkuarvot sen mukaan ett‰ ollaan eri puolella 0:aa
			moistT1 = moistT0;
		else
			moistT2 = moistT0;
		counter++;
	} while(minDiff > deltaT && counter < 10);

	if(minDiff <= deltaT) // jos l‰himm‰n lasketun l‰mpp‰rin ero oli tarpeeksi pieni, palautetaan sit‰ vastaava kostea-l‰mpˆtila
		return minDiffMoistT;
	return kFloatMissing;
}

// Laske ilmapaketin l‰mpˆtila nostamalla ilmapakettia
// Nosta kuiva-adiapaattisesti LCL-korkeuteen ja siit‰ eteenp‰in kostea-adiapaattisesti
double CalcTOfLiftedAirParcel(double T, double Td, double fromP, double toP)
{
	// 1. laske LCL kerroksen paine alkaen fromP:st‰
	double lclPressure = CalcLCLPressure(T, Td, fromP);
	if(lclPressure != kFloatMissing)
	{
		// Laske aloitus korkeuden l‰mpˆtilan potentiaali l‰mpˆtila.
		double TpotStart = T2tpot(T, fromP);
		if(TpotStart != kFloatMissing)
		{
			if(lclPressure <= toP) // jos lcl oli yli toP:n mb (eli pienempi kuin toP)
			{ // nyt riitt‰‰ pelkk‰ kuiva-adiapaattinen nosto fromP -> toP
				double Tparcel = Tpot2t(TpotStart, toP);
				return Tparcel;
			}
			else
			{
				// Laske ilmapaketin l‰mpˆ lcl-korkeudella  kuiva-adiapaattisesti nostettuna
				double Tparcel_LCL = Tpot2t(TpotStart, lclPressure);
				// laske kyseiselle korkeudelle vastaava kostea-adiapaatti arvo
				double TmoistLCL = CalcMoistT(Tparcel_LCL, lclPressure);
				// kyseinen kostea-adiapaatti pit‰‰ konvertoida viel‰ (ADL-kielest‰ kopioitua koodia, ks. OS- ja
				// TSA-funtioita) jotenkin 1000 mb:hen.
				if(TmoistLCL != kFloatMissing)
				{
					double AOS = OS(TmoistLCL, 1000.);
					// Sitten lasketaan l‰mpˆtila viimein 500 mb:hen
					double Tparcel = TSA(AOS, toP);
					return Tparcel;
				}
			}
		}
	}
	return kFloatMissing;
}

// SHOW Showalter index
// SHOW	= T500 - Tparcel
// T500 = Temperature in Celsius at 500 mb
// Tparcel = Temperature in Celsius at 500 mb of a parcel lifted from 850 mb
double CalcSHOWIndex(NFmiSoundingData &theData)
{
	double indexValue = kFloatMissing;
	double T_850 = theData.GetValueAtPressure(kFmiTemperature, 850);
	double Td_850 = theData.GetValueAtPressure(kFmiDewPoint, 850);
	if(T_850 != kFloatMissing && Td_850 != kFloatMissing)
	{
		double Tparcel_from850to500 = CalcTOfLiftedAirParcel(T_850, Td_850, 850, 500);
		// 3. SHOW = T500 - Tparcel_from850to500
		double T500 = theData.GetValueAtPressure(kFmiTemperature, 500);
		if(T500 != kFloatMissing && Tparcel_from850to500 != kFloatMissing)
			indexValue = T500 - Tparcel_from850to500;
	}
	return indexValue;
}

// LIFT Lifted index
// LIFT	= T500 - Tparcel
// T500 = temperature in Celsius of the environment at 500 mb
// Tparcel = 500 mb temperature in Celsius of a lifted parcel with the average pressure,
//			 temperature, and dewpoint of the layer 500 m above the surface.
double CalcLIFTIndex(NFmiSoundingData &theData)
{
	double indexValue = kFloatMissing;
	double P_500m_avg = kFloatMissing;
	double T_500m_avg = kFloatMissing;
	double Td_500m_avg = kFloatMissing;

	// HUOM! Pit‰‰ ottaa huomioon aseman korkeus kun tehd‰‰n laskuja!!!!
	double h1 = 0 + theData.ZeroHeight();
	double h2 = 500 + theData.ZeroHeight();

	if(theData.CalcLCLAvgValues(h1, h2, T_500m_avg, Td_500m_avg, P_500m_avg, false))
	{
		double Tparcel_from500mAvgTo500 = CalcTOfLiftedAirParcel(T_500m_avg, Td_500m_avg, P_500m_avg, 500);
		// 3. LIFT	= T500 - Tparcel_from500mAvgTo500
		double T500 = theData.GetValueAtPressure(kFmiTemperature, 500);
		if(T500 != kFloatMissing && Tparcel_from500mAvgTo500 != kFloatMissing)
			indexValue = T500 - Tparcel_from500mAvgTo500;
	}
	return indexValue;
}

// Laskee logaritmisessa asteikossa interpoloidun arvon.
// K‰ytet‰‰n esim. logaritmisen paine asteikon kanssa.
// Palauttaa x:‰‰ vastaavan y:n, kun x1 arvoa vastaa y1 ja x2:n arvoa vastaa y2.
float CalcLogInterpolatedValue(float x1, float x2, float x, float y1, float y2)
{
	float y = kFloatMissing;
	if(x1 != kFloatMissing && x2 != kFloatMissing && x != kFloatMissing)
	{
		if(x1 == x2)
			y = y1 != kFloatMissing ? y1 : y2;
		else if(y1 != kFloatMissing && y2 != kFloatMissing)
		{
			float w = (::log(x)-::log(x1)) / (::log(x2)-::log(x1));
			y = (1-w)*y1 + w*y2;
		}
		else if(y1 != kFloatMissing) // jos toinen -arvoista puuttuu annetaan arvoksi toinen
			y = y1;
		else if(y2 != kFloatMissing) // jos toinen -arvoista puuttuu annetaan arvoksi toinen
			y = y2;
	}
	return y;
}

// Laskee logaritmisessa asteikossa interpoloidun arvon.
// K‰ytet‰‰n esim. logaritmisen paine asteikon kanssa.
// Palauttaa x:‰‰ vastaavan y:n, kun x1 arvoa vastaa y1 ja x2:n arvoa vastaa y2.
float CalcLogModLinearInterpolatedValue(float x1, float x2, float x, float y1, float y2, unsigned int modulo)
{
	float y = kFloatMissing;
	if(x1 != kFloatMissing && x2 != kFloatMissing && x != kFloatMissing)
	{
		if(x1 == x2)
			y = y1 != kFloatMissing ? y1 : y2;
		else if(y1 != kFloatMissing && y2 != kFloatMissing)
		{
			float w = (::log(x)-::log(x1)) / (::log(x2)-::log(x1));
			y =  static_cast<float>(NFmiInterpolation::ModLinear(w, y1, y2, modulo));
		}
		else if(y1 != kFloatMissing) // jos toinen -arvoista puuttuu annetaan arvoksi toinen
			y = y1;
		else if(y2 != kFloatMissing) // jos toinen -arvoista puuttuu annetaan arvoksi toinen
			y = y2;
	}
	return y;
}

float CalcLogInterpolatedWindWectorValue(float x1, float x2, float x, float wv1, float wv2)
{
	float y = kFloatMissing;
	if(wv1 != kFloatMissing && wv2 != kFloatMissing)
	{
		float wd1 = (static_cast<int>(wv1)%100)*10.f;
		float ws1 = static_cast<float>(static_cast<int>(wv1)/100);
		float wd2 = (static_cast<int>(wv2)%100)*10.f;
		float ws2 = static_cast<float>(static_cast<int>(wv2)/100);

		float wdInterp = CalcLogModLinearInterpolatedValue(x1, x2, x, wd1, wd2, 360);
		float wsInterp = CalcLogInterpolatedValue(x1, x2, x, ws1, ws2);
		if(wdInterp != kFloatMissing && wsInterp != kFloatMissing)
		{
			y = static_cast<float>(FmiRound(wsInterp)*100 + FmiRound(wdInterp/10.));
		}
	}
	else if(wv1 != kFloatMissing)
		y = wv1;
	else if(wv2 != kFloatMissing)
		y = wv2;
	return y;
}


// KINX K index
// KINX = ( T850 - T500 ) + TD850 - ( T700 - TD700 )
//	T850 = Temperature in Celsius at 850 mb
//	T500 = Temperature in Celsius at 500 mb
//	TD850 = Dewpoint in Celsius at 850 mb
//	T700 = Temperature in Celsius at 700 mb
//	TD700 = Dewpoint in Celsius at 700 mb
double CalcKINXIndex(NFmiSoundingData &theData)
{
	double T850 = theData.GetValueAtPressure(kFmiTemperature, 850);
	double T500 = theData.GetValueAtPressure(kFmiTemperature, 500);
	double TD850 = theData.GetValueAtPressure(kFmiDewPoint, 850);
	double T700 = theData.GetValueAtPressure(kFmiTemperature, 700);
	double TD700 = theData.GetValueAtPressure(kFmiDewPoint, 700);
	if(T850 != kFloatMissing && T500 != kFloatMissing && TD850 != kFloatMissing && T700 != kFloatMissing && TD700 != kFloatMissing)
		return ( T850 - T500 ) + TD850 - ( T700 - TD700 );
	return kFloatMissing;
}

// CTOT	Cross Totals index
//	CTOT	= TD850 - T500
//		TD850 	= Dewpoint in Celsius at 850 mb
//		T500 	= Temperature in Celsius at 500 mb
double CalcCTOTIndex(NFmiSoundingData &theData)
{
	double T500 = theData.GetValueAtPressure(kFmiTemperature, 500);
	double TD850 = theData.GetValueAtPressure(kFmiDewPoint, 850);
	if(T500 != kFloatMissing && TD850 != kFloatMissing)
		return ( TD850 - T500 );
	return kFloatMissing;
}

// VTOT	Vertical Totals index
//	VTOT	= T850 - T500
//		T850	= Temperature in Celsius at 850 mb
//		T500	= Temperature in Celsius at 500 mb
double CalcVTOTIndex(NFmiSoundingData &theData)
{
	double T500 = theData.GetValueAtPressure(kFmiTemperature, 500);
	double T850 = theData.GetValueAtPressure(kFmiTemperature, 850);
	if(T500 != kFloatMissing && T850 != kFloatMissing)
		return ( T850 - T500 );
	return kFloatMissing;
}

// TOTL	Total Totals index
//	TOTL	= ( T850 - T500 ) + ( TD850 - T500 )
//		T850 	= Temperature in Celsius at 850 mb
//		TD850	= Dewpoint in Celsius at 850 mb
//		T500 	= Temperature in Celsius at 500 mb
double CalcTOTLIndex(NFmiSoundingData &theData)
{
	double T850 = theData.GetValueAtPressure(kFmiTemperature, 850);
	double T500 = theData.GetValueAtPressure(kFmiTemperature, 500);
	double TD850 = theData.GetValueAtPressure(kFmiDewPoint, 850);
	if(T850 != kFloatMissing && T500 != kFloatMissing && TD850 != kFloatMissing)
		return ( T850 - T500 ) + (TD850 - T500);
	return kFloatMissing;
}

// HUOM! Pit‰‰ ottaa huomioon aseman korkeus kun tehd‰‰n laskuja!!!!
bool GetValuesNeededInLCLCalculations(NFmiSoundingData &theData, FmiLCLCalcType theLCLCalcType, double &T, double &Td, double &P)
{
	// HUOM! Pit‰‰ ottaa huomioon aseman korkeus kun tehd‰‰n laskuja!!!!
	double h1 = 0 + theData.ZeroHeight();
	double h2 = 500 + theData.ZeroHeight();

	bool status = false;
	if(theLCLCalcType == kLCLCalc500m)
		status = theData.CalcLCLAvgValues(h1, h2, T, Td, P, false);
	else if(theLCLCalcType == kLCLCalc500m2)
		status = theData.CalcLCLAvgValues(h1, h2, T, Td, P, true);
	else if(theLCLCalcType == kLCLCalcMostUnstable)
	{
		double maxThetaE = 0;
		status = theData.FindHighestThetaE(T, Td, P, maxThetaE, 500); // rajoitetaan max thetan etsint‰ 500 mb:en asti
	}

	if(status == false) // jos muu ei auta, laske pinta suureiden avulla
	{
		P=1100;
		if(!theData.GetValuesStartingLookingFromPressureLevel(T, Td, P)) // sitten l‰himm‰t pinta arvot, jotka joskus yli 500 m
			return false;
	}
	return true;
}

// LCL-levelin painepinnan lasku k‰ytt‰en luotauksen haluttuja arvoja
double CalcLCLPressureLevel(NFmiSoundingData &theData, FmiLCLCalcType theLCLCalcType)
{
	// 1. calc T,Td,P values from 500 m layer avg or surface values
	double T=kFloatMissing,
		   Td=kFloatMissing,
		   P=kFloatMissing;
	if(!GetValuesNeededInLCLCalculations(theData, theLCLCalcType, T, Td, P))
		return kFloatMissing;

	return CalcLCLPressure(T, Td, P);
}

// Claculates LCL (Lifted Condensation Level)
// halutulla tavalla
double CalcLCLIndex(NFmiSoundingData &theData, FmiLCLCalcType theLCLCalcType)
{
	return CalcLCLPressureLevel(theData, theLCLCalcType);
}
// palauttaa LCL:n korkeuden metreiss‰
double CalcLCLHeightIndex(NFmiSoundingData &theData, FmiLCLCalcType theLCLCalcType)
{
	return theData.GetValueAtPressure(kFmiGeomHeight, static_cast<float>(CalcLCLPressureLevel(theData, theLCLCalcType)));
}

// Claculates LFC (Level of Free Convection)
// which is the level above which the lifted parcel is warmer than environment
// parcel is avg from 500 m layer at surface
// HUOM! Lis‰sin myˆs EL laskun eli EL on korkeus mill‰ nostettu ilmapaketti muuttuu
// j‰lleen kylmemm‰ksi kuin ymp‰ristˆ (jos se koskaan oli l‰mpim‰mp‰‰)
// Tied‰n t‰m‰ on ik‰v‰ kaksi vastuuta yhdell‰ metodilla, joista toinen ei edes n‰y metodin nimess‰.
double CalcLFCIndex(NFmiSoundingData &theData, FmiLCLCalcType theLCLCalcType, double &EL)
{
	// 1. calc T,Td,P values from 500 m layer avg or surface values
	double T=kFloatMissing,
		   Td=kFloatMissing,
		   P=kFloatMissing;
	if(!GetValuesNeededInLCLCalculations(theData, theLCLCalcType, T, Td, P))
		return kFloatMissing;

	double LCLValue = CalcLCLPressure(T, Td, P);
	// 2. lift parcel until its warmer than environment
	// 2.1 first adiabatically till LCL and than moist adiabatically
	// iterate with CalcTOfLiftedAirParcel from 500 m avg P to next sounding pressure level
	// until T-parcel is warmer than T at that pressure level in sounding
	checkedVector<float> &pValues = theData.GetParamData(kFmiPressure);
	checkedVector<float> &tValues = theData.GetParamData(kFmiTemperature);
	int ssize = pValues.size();
	double TofLiftedParcer_previous = kFloatMissing;
	double P_previous = kFloatMissing;

	double foundPValue = kFloatMissing;
	double durrentDiff = 0; // ilmapaketin ja ymp‰ristˆn T ero
	double lastDiff = 0; // ilmapaketin ja ymp‰ristˆn T ero viime kierroksella
	for(int i = 0; i < ssize; i++)
	{
		if(pValues[i] != kFloatMissing && pValues[i] <= P) // aloitetaan LFC etsint‰ vasta 'aloitus' korkeuden j‰lkeen
		{
			if(tValues[i] != kFloatMissing) // kaikilla painepinnoilla ei ole l‰mpˆtilaa
			{
				double TofLiftedParcer = CalcTOfLiftedAirParcel(T, Td, P, pValues[i]);

				TofLiftedParcer_previous = TofLiftedParcer;
				P_previous = pValues[i];

				durrentDiff = TofLiftedParcer - tValues[i];
				if(durrentDiff > 0 && foundPValue == kFloatMissing) // vain alin korkeus talteen
					foundPValue = pValues[i];
				if(durrentDiff < 0 && lastDiff > 0) // jos siis paketti muuttui l‰mpim‰mm‰st‰ kylmemm‰ksi (ymp‰ristˆˆn verrattuna, ota talteen korkeus)
					EL = pValues[i];
				lastDiff = durrentDiff;
			}
		}
	}
	if(foundPValue != kFloatMissing && LCLValue < foundPValue)
		foundPValue = LCLValue; // LFC:n pit‰‰ olla ainakin LCL korkeudessa tai korkeammalla eli kun paineesta kysymys LCL >= LFC
	return foundPValue;
}

// palauttaa LFC:n ja  EL:n korkeuden metreiss‰
double CalcLFCHeightIndex(NFmiSoundingData &theData, FmiLCLCalcType theLCLCalcType, double &ELheigth)
{
	double tmpValue = CalcLFCIndex(theData, theLCLCalcType, ELheigth);
	ELheigth = theData.GetValueAtPressure(kFmiGeomHeight, static_cast<float>(ELheigth));
	return theData.GetValueAtPressure(kFmiGeomHeight, static_cast<float>(tmpValue));
}

// Calculates CAPE (500 m mix)
// theHeightLimit jos halutaan, voidaan cape lasku rajoittaa alle jonkin korkeus arvon (esim. 3000 m)
double CalcCAPE500Index(NFmiSoundingData &theData, FmiLCLCalcType theLCLCalcType, double theHeightLimit)
{
	// 1. calc T,Td,P values from 500 m layer avg or surface values
	double T=kFloatMissing,
		   Td=kFloatMissing,
		   P=kFloatMissing;
	if(!GetValuesNeededInLCLCalculations(theData, theLCLCalcType, T, Td, P))
		return kFloatMissing;

	// HUOM! Pit‰‰ ottaa huomioon aseman korkeus kun tehd‰‰n laskuja!!!!
	if(theHeightLimit != kFloatMissing)
		theHeightLimit += theData.ZeroHeight();

	checkedVector<float> &pValues = theData.GetParamData(kFmiPressure);
	checkedVector<float> &tValues = theData.GetParamData(kFmiTemperature);
	int ssize = pValues.size();
	double CAPE = 0;
	double g = 9.81; // acceleration by gravity
	double Tparcel = 0;
	double Tenvi = 0;
	double currentZ = 0;
	double lastZ = kFloatMissing;
	double deltaZ = kFloatMissing;
    double TK=273.15;
	for(int i = 0; i < ssize; i++)
	{
		if(pValues[i] != kFloatMissing && pValues[i] < P) // aloitetaan LFC etsint‰ vasta 'aloitus' korkeuden j‰lkeen
		{
			if(tValues[i] != kFloatMissing) // kaikilla painepinnoilla ei ole l‰mpˆtilaa
			{
// !!!!!!!!		if((Tlow != kFloatMissing && Tlow > tValues[i]) || (Thigh != kFloatMissing && Thigh < tValues[i]))
//					continue;
				double TofLiftedParcer = CalcTOfLiftedAirParcel(T, Td, P, pValues[i]);
				currentZ = theData.GetValueAtPressure(kFmiGeomHeight, pValues[i]); // interpoloidaan jos tarvis

				// integrate here if T parcel is greater than T environment
				if(TofLiftedParcer > tValues[i] && currentZ != kFloatMissing && lastZ != kFloatMissing)
				{
					Tparcel = TofLiftedParcer + TK;
					Tenvi = tValues[i] + TK;
					deltaZ = currentZ - lastZ;
					CAPE += g * deltaZ * ((Tparcel - Tenvi)/Tenvi);
				}
				lastZ = currentZ;

				if(theHeightLimit != kFloatMissing && currentZ > theHeightLimit)
					break; // lopetetaan laskut jos ollaan menty korkeus rajan yli, kun ensin on laskettu t‰m‰ siivu viel‰ mukaan
			}
		}
	}
	return CAPE;
}

// Calculates CAPE in layer between two temperatures given
double CalcCAPE_TT_Index(NFmiSoundingData &theData, FmiLCLCalcType theLCLCalcType, double Thigh, double Tlow)
{
	// 1. calc T,Td,P values from 500 m layer avg or surface values
	double T=kFloatMissing,
		   Td=kFloatMissing,
		   P=kFloatMissing;
	if(!GetValuesNeededInLCLCalculations(theData, theLCLCalcType, T, Td, P))
		return kFloatMissing;

	checkedVector<float> &pValues = theData.GetParamData(kFmiPressure);
	checkedVector<float> &tValues = theData.GetParamData(kFmiTemperature);
	int ssize = pValues.size();
	double CAPE = 0;
	double g = 9.81; // acceleration by gravity
	double Tparcel = 0;
	double Tenvi = 0;
	double currentZ = 0;
	double lastZ = kFloatMissing;
	double deltaZ = kFloatMissing;
    double TK=273.15;
	for(int i = 0; i < ssize; i++)
	{
		if(pValues[i] != kFloatMissing && pValues[i] < P) // aloitetaan LFC etsint‰ vasta 'aloitus' korkeuden j‰lkeen
		{
			if(tValues[i] != kFloatMissing) // kaikilla painepinnoilla ei ole l‰mpˆtilaa
			{
				currentZ = theData.GetValueAtPressure(kFmiGeomHeight, pValues[i]); // interpoloidaan jos tarvis
				if(Tlow < tValues[i] && Thigh > tValues[i])
				{
					double TofLiftedParcer = CalcTOfLiftedAirParcel(T, Td, P, pValues[i]);

					// integrate here if T parcel is greater than T environment
					if(TofLiftedParcer > tValues[i] && currentZ != kFloatMissing && lastZ != kFloatMissing)
					{
						Tparcel = TofLiftedParcer + TK;
						Tenvi = tValues[i] + TK;
						deltaZ = currentZ - lastZ;
						CAPE += g * deltaZ * ((Tparcel - Tenvi)/Tenvi);
					}
				}
				lastZ = currentZ;
			}
		}
	}
	return CAPE;
}

// Calculates CIN
// first layer of negative (TP - TE (= T-parcel - T-envi)) unless its the last also
double CalcCINIndex(NFmiSoundingData &theData, FmiLCLCalcType theLCLCalcType)
{
	// 1. calc T,Td,P values from 500 m layer avg or surface values
	double T=kFloatMissing,
		   Td=kFloatMissing,
		   P=kFloatMissing;
	if(!GetValuesNeededInLCLCalculations(theData, theLCLCalcType, T, Td, P))
		return kFloatMissing;

	checkedVector<float> &pValues = theData.GetParamData(kFmiPressure);
	checkedVector<float> &tValues = theData.GetParamData(kFmiTemperature);
	int ssize = pValues.size();
	double CIN = 0;
	double g = 9.81; // acceleration by gravity
	double Tparcel = 0;
	double Tenvi = 0;
	double currentZ = 0;
	double lastZ = kFloatMissing;
	double deltaZ = kFloatMissing;
    double TK=273.15; // celsius/kelvin change
	bool firstCinLayerFound = false;
	bool capeLayerFoundAfterCin = false;
	for(int i = 0; i < ssize; i++)
	{
		if(pValues[i] != kFloatMissing && pValues[i] < P) // aloitetaan LFC etsint‰ vasta 'aloitus' korkeuden j‰lkeen
		{
			if(tValues[i] != kFloatMissing) // kaikilla painepinnoilla ei ole l‰mpˆtilaa
			{
				double TofLiftedParcer = CalcTOfLiftedAirParcel(T, Td, P, pValues[i]);
				currentZ = theData.GetValueAtPressure(kFmiGeomHeight, pValues[i]); // interpoloidaan jos tarvis

				// integrate here if T parcel is less than T environment
				if(TofLiftedParcer <= tValues[i] && currentZ != kFloatMissing && lastZ != kFloatMissing)
				{
					Tparcel = TofLiftedParcer + TK;
					Tenvi = tValues[i] + TK;
					deltaZ = currentZ - lastZ;
					CIN += g * deltaZ * ((Tparcel - Tenvi)/Tenvi);
					firstCinLayerFound = true;
				}
				else if(firstCinLayerFound)
				{
					capeLayerFoundAfterCin = true;
					break; // jos 1. CIN layer on lˆydetty ja osutaan CAPE layeriin, lopetetaan
				}
				lastZ = currentZ;
			}
		}
	}
	if(!(firstCinLayerFound && capeLayerFoundAfterCin))
		CIN = 0;
	return CIN;
}

// startH and endH are in kilometers
// HUOM! Pit‰‰ ottaa huomioon aseman korkeus kun tehd‰‰n laskuja!!!!
double CalcWindBulkShearComponent(NFmiSoundingData &theData, double startH, double endH, FmiParameterName theParId)
{
	// HUOM! Pit‰‰ ottaa huomioon aseman korkeus kun tehd‰‰n laskuja!!!!
	startH += theData.ZeroHeight()/1000.; // zero height pit‰‰ muuttaa t‰ss‰ metreist‰ kilometreiksi!
	endH += theData.ZeroHeight()/1000.; // zero height pit‰‰ muuttaa t‰ss‰ metreist‰ kilometreiksi!

	float startValue = theData.GetValueAtHeight(theParId, static_cast<float>(startH*1000));
	float endValue = theData.GetValueAtHeight(theParId, static_cast<float>(endH*1000));
	float shearComponent = endValue - startValue;
	return shearComponent;
}

// startH and endH are in kilometers
// HUOM! Pit‰‰ ottaa huomioon aseman korkeus kun tehd‰‰n laskuja!!!!
double CalcThetaEDiffIndex(NFmiSoundingData &theData, double startH, double endH)
{
	// HUOM! Pit‰‰ ottaa huomioon aseman korkeus kun tehd‰‰n laskuja!!!!
	startH += theData.ZeroHeight()/1000.; // zero height pit‰‰ muuttaa t‰ss‰ metreist‰ kilometreiksi!
	endH += theData.ZeroHeight()/1000.; // zero height pit‰‰ muuttaa t‰ss‰ metreist‰ kilometreiksi!

	float startT = theData.GetValueAtHeight(kFmiTemperature, static_cast<float>(startH*1000));
	float endT = theData.GetValueAtHeight(kFmiTemperature, static_cast<float>(endH*1000));
	float startTd = theData.GetValueAtHeight(kFmiDewPoint, static_cast<float>(startH*1000));
	float endTd = theData.GetValueAtHeight(kFmiDewPoint, static_cast<float>(endH*1000));
	float startP = theData.GetValueAtHeight(kFmiPressure, static_cast<float>(startH*1000));
	float endP = theData.GetValueAtHeight(kFmiPressure, static_cast<float>(endH*1000));
	double startThetaE = CalcThetaE(startT, startTd, startP);
	double endThetaE = CalcThetaE(endT, endTd, endP);
	double diffValue = startThetaE - endThetaE;
	return diffValue;
}

// Calculates Bulk Shear between two given layers
// startH and endH are in kilometers
double CalcBulkShearIndex(NFmiSoundingData &theData, double startH, double endH)
{
	// HUOM! asema korkeus otetaan huomioon CalcWindBulkShearComponent-funktiossa.
	double uTot = CalcWindBulkShearComponent(theData, startH, endH, kFmiWindUMS);
	double vTot = CalcWindBulkShearComponent(theData, startH, endH, kFmiWindVMS);
	double BS = ::sqrt(uTot*uTot + vTot*vTot);
	return BS;
}

/* // **********  SRH calculation help from Pieter Groenemeijer ******************

Some tips here on how tyo calculate storm-relative helciity

How to calculate storm-relative helicity

Integrate the following from p = p_surface to p = p_top (or in case of height coordinates from h_surface to h_top):

storm_rel_helicity -= ((u_ID-u[p])*(v[p]-v[p+1]))-((v_ID - v[p])*(u[p]-u[p+1]));

Here, u_ID and v_ID are the forecast storm motion vectors calculated with the so-called ID-method. These can be calculated as follows:

where

/average wind
u0_6 = average 0_6 kilometer u-wind component
v0_6 = average 0_6 kilometer v-wind component
(you should use a pressure-weighted average in case you work with height coordinates)

/shear
shr_0_6_u = u_6km - u_surface;
shr_0_6_v = v_6km - v_surface;

/ shear unit vector
shr_0_6_u_n = shr_0_6_u / ((shr_0_6_u^2 + shr_0_6_v^2)**0.5);
shr_0_6_v_n = shr_0_6_v / ((shr_0_6_u^2 + shr_0_6_v^2)** 0.5);

/id-vector components
u_ID = u0_6 + shr_0_6_v_n * 7.5;
v_ID = v0_6 - shr_0_6_u_n * 7.5;

(7.5 are meters per second... watch out when you work with knots instead)

*/ // **********  SRH calculation help from Pieter Groenemeijer ******************

// I use same variable names as with the Pieters help evem though calculations are not
// restricted to 0-6 km layer but they can be from any layer

// shear
// startH and endH are in kilometers
double CalcBulkShearIndex(NFmiSoundingData &theData, double startH, double endH, FmiParameterName theParId)
{
	// HUOM! asema korkeus otetaan huomioon CalcWindBulkShearComponent-funktiossa.
	double shr_0_6_component = CalcWindBulkShearComponent(theData, startH, endH, theParId);
	return shr_0_6_component;
}

// id-vector u-component 'right'
double CalcU_ID_right(double u0_6, double shr_0_6_v_n)
{
	double u_ID = u0_6 + shr_0_6_v_n  * 7.5;
	return u_ID;
}

// id-vector v-component 'left'
double CalcV_ID_left(double v0_6, double shr_0_6_u_n)
{
	double v_ID = v0_6 + shr_0_6_u_n  * 7.5;
	return v_ID;
}

// id-vector u-component 'left'
double CalcU_ID_left(double u0_6, double shr_0_6_v_n)
{
	double u_ID = u0_6 - shr_0_6_v_n  * 7.5;
	return u_ID;
}

// id-vector v-component 'right'
double CalcV_ID_right(double v0_6, double shr_0_6_u_n)
{
	double v_ID = v0_6 - shr_0_6_u_n  * 7.5;
	return v_ID;
}

// shear unit u-vector
double Calc_shear_unit_u_vector(double shr_0_6_u, double shr_0_6_v)
{
	double shr_0_6_u_n = shr_0_6_u / ::sqrt((shr_0_6_u*shr_0_6_u + shr_0_6_v*shr_0_6_v));
	return shr_0_6_u_n;
}

// shear unit v-vector
double Calc_shear_unit_v_vector(double shr_0_6_v, double shr_0_6_u)
{
	double shr_0_6_v_n = shr_0_6_v / ::sqrt((shr_0_6_v*shr_0_6_v + shr_0_6_u*shr_0_6_u));
	return shr_0_6_v_n;
}

// u_ID and vID are wind component vector calculated for helicity calculations
// uP1 is wind vector x (along latitude) component from lower layer and uP2 from higher layer
// vP1 are the same but for wind y (along longitude) component.
double CalcSRH(double u_ID, double v_ID, double uP1, double uP2, double vP1, double vP2)
{
	double SRH = ((u_ID - uP1) * (vP1 - vP2)) - ((v_ID - vP1)*(uP1-uP2));
	return SRH;
}

void Calc_U_V_helpers(NFmiSoundingData &theData, double &shr_0_6_u_n, double &shr_0_6_v_n, double &u0_6, double &v0_6)
{
	// HUOM! asema korkeus otetaan huomioon CalcWindBulkShearComponent-funktiossa.
	double shr_0_6_u = CalcWindBulkShearComponent(theData, 0, 6, kFmiWindUMS);
	double shr_0_6_v = CalcWindBulkShearComponent(theData, 0, 6, kFmiWindVMS);
	shr_0_6_u_n = Calc_shear_unit_v_vector(shr_0_6_u, shr_0_6_v);
	shr_0_6_v_n = Calc_shear_unit_v_vector(shr_0_6_v, shr_0_6_u);

	// HUOM! Pit‰‰ ottaa huomioon aseman korkeus kun tehd‰‰n laskuja!!!!
	double h1 = 0 + theData.ZeroHeight(); // 0 m + aseman korkeus
	double h2 = 6000 + theData.ZeroHeight(); // 6000 m + aseman korkeus
	theData.CalcAvgWindComponentValues(h1, h2, u0_6, v0_6);
}

// lasketaan u ja v ID:t 0-6km layeriss‰
// t‰m‰ on hodografissa 'left'
void Calc_U_and_V_IDs_left(NFmiSoundingData &theData, double &u_ID, double &v_ID)
{
	double shr_0_6_u_n=0, shr_0_6_v_n=0, u0_6=0, v0_6=0;
	Calc_U_V_helpers(theData, shr_0_6_u_n, shr_0_6_v_n, u0_6, v0_6);

	u_ID = CalcU_ID_left(u0_6, shr_0_6_v_n);
	v_ID = CalcV_ID_left(v0_6, shr_0_6_u_n);
}

// lasketaan u ja v ID:t 0-6km layeriss‰
// t‰m‰ on hodografissa 'right'
void Calc_U_and_V_IDs_right(NFmiSoundingData &theData, double &u_ID, double &v_ID)
{
	double shr_0_6_u_n=0, shr_0_6_v_n=0, u0_6=0, v0_6=0;
	Calc_U_V_helpers(theData, shr_0_6_u_n, shr_0_6_v_n, u0_6, v0_6);

	u_ID = CalcU_ID_right(u0_6, shr_0_6_v_n);
	v_ID = CalcV_ID_right(v0_6, shr_0_6_u_n);
}

// lasketaan u ja v mean 0-6km layeriss‰
void Calc_U_and_V_mean_0_6km(NFmiSoundingData &theData, double &u0_6, double &v0_6)
{
	// HUOM! Pit‰‰ ottaa huomioon aseman korkeus kun tehd‰‰n laskuja!!!!
	double h1 = 0 + theData.ZeroHeight(); // 0 m + aseman korkeus
	double h2 = 6000 + theData.ZeroHeight(); // 6000 m + aseman korkeus
	theData.CalcAvgWindComponentValues(h1, h2, u0_6, v0_6);
}

NFmiString Get_U_V_ID_IndexText(NFmiSoundingData &theData, const NFmiString &theText, FmiDirection theStormDirection)
{
	NFmiString str(theText);
	str += "=";
	double u_ID = kFloatMissing;
	double v_ID = kFloatMissing;
	if(theStormDirection == kRight)
		Calc_U_and_V_IDs_right(theData, u_ID, v_ID);
	else if(theStormDirection == kLeft)
		Calc_U_and_V_IDs_left(theData, u_ID, v_ID);
	else
		Calc_U_and_V_mean_0_6km(theData, u_ID, v_ID);

	if(u_ID == kFloatMissing || v_ID == kFloatMissing)
		str += " -/-";
	else
	{
		double WS = ::sqrt(u_ID*u_ID + v_ID*v_ID);
		NFmiWindDirection theDirection(u_ID, v_ID);
		double WD = theDirection.Value();

		if(WD != kFloatMissing)
			str += NFmiValueString::GetStringWithMaxDecimalsSmartWay(WD, 0);
		else
			str += "-";
		str += "/";
		str += NFmiValueString::GetStringWithMaxDecimalsSmartWay(WS, 1);
	}
	return str;
}

// Calculates Storm-Relative Helicity (SRH) between two given layers
// startH and endH are in kilometers
// k‰ytet‰‰n muuttujan nimin‰ samoja mit‰ on Pieterin helpiss‰, vaikka kyseess‰ ei olekaan laskut layerille 0-6km vaan mille v‰lille tahansa
// HUOM! Pit‰‰ ottaa huomioon aseman korkeus kun tehd‰‰n laskuja!!!!
double CalcSRHIndex(NFmiSoundingData &theData, double startH, double endH)
{
	checkedVector<float>&pV = theData.GetParamData(kFmiPressure);
	if(pV.size() > 0)
	{
		// HUOM! Pit‰‰ ottaa huomioon aseman korkeus kun tehd‰‰n laskuja!!!!
		startH += theData.ZeroHeight()/1000.; // zero height pit‰‰ muuttaa t‰ss‰ metreist‰ kilometreiksi!
		endH += theData.ZeroHeight()/1000.; // zero height pit‰‰ muuttaa t‰ss‰ metreist‰ kilometreiksi!

		int ssize = pV.size();
		checkedVector<float>&uV = theData.GetParamData(kFmiWindUMS);
		checkedVector<float>&vV = theData.GetParamData(kFmiWindVMS);
		checkedVector<float>&tV = theData.GetParamData(kFmiTemperature);
		checkedVector<float>&tdV = theData.GetParamData(kFmiDewPoint);

		double u_ID = kFloatMissing;
		double v_ID = kFloatMissing;
		Calc_U_and_V_IDs_right(theData, u_ID, v_ID);
		double SRH = 0;
		double lastU = kFloatMissing;
		double lastV = kFloatMissing;
		bool foundAnyData = false;
		for(int i = 0; i<ssize; i++)
		{
			double p = pV[i];
			double u = uV[i];
			double v = vV[i];
			double t = tV[i];
			double td = tdV[i];
			if(p != kFloatMissing && u != kFloatMissing && v != kFloatMissing && t != kFloatMissing && td != kFloatMissing)
			{
				double z = theData.GetValueAtPressure(kFmiGeomHeight, static_cast<float>(p));
				if(z != kFloatMissing && z >= startH*1000 && z <= endH*1000)
				{
					if(lastU != kFloatMissing && lastV != kFloatMissing)
					{
						SRH -= CalcSRH(u_ID, v_ID, lastU, u, lastV, v);
						foundAnyData = true;
					}
				}
				lastU = u;
				lastV = v;
			}
		}
		return foundAnyData ? SRH : kFloatMissing;
	}
	return kFloatMissing;
}

// theH in meters
double CalcWSatHeightIndex(NFmiSoundingData &theData, double theH)
{
	// HUOM! Pit‰‰ ottaa huomioon aseman korkeus kun tehd‰‰n laskuja!!!!
	theH += theData.ZeroHeight();

	return theData.GetValueAtHeight(kFmiWindSpeedMS, static_cast<float>(theH));
}

double FindNearestW(double T, double P)
{
	double minDiff = 999999.;
	double minDiffW = kFloatMissing;
	double deltaW = 0.1;
	double w1 = 0.01;
	double w2 = 50;
	int counter = 0;
	do
	{
		double t1 = TMR(w1, P);
		double diff1 = t1 - T;
		double t2 = TMR(w2, P);
		double diff2 = t2 - T;
		double w0 = FindRoot(w1, w2, diff1, diff2);
		double t0 = TMR(w0, P);
		double diff0 = t0 - T;
		if(::fabs(diff0) < minDiff)
		{ // jos laskettu l‰mpˆtila oli tarpeeksi l‰hell‰ annettua, laitetaan arvo talteen
			minDiff = ::fabs(diff0);
			minDiffW = w0;
		}
		if(diff1 < 0 && diff0 < 0) // katsotaan kumman kanssa etumerkki on sama ja alustetaan iterointi alkuarvot sen mukaan ett‰ ollaan eri puolella 0:aa
			w1 = w0;
		else
			w2 = w0;
		counter++;
	} while(minDiff > deltaW && counter < 50);

	if(minDiff <= deltaW) // jos l‰himm‰n lasketun W:n ero oli tarpeeksi pieni, palautetaan sit‰ vastaava arvo
		return minDiffW;
	return kFloatMissing;
}

} // end of namespace NFmiSoundingFunctions
