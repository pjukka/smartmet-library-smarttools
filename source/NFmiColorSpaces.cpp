// NFmiColorSpaces.cpp 
//
// NFmiColorSpaces-namespacen ja siinä olevien luokkien toteutus.

#include "NFmiColorSpaces.h"

NFmiColor NFmiColorSpaces::GetBrighterColor(const NFmiColor &theColor, double theBrightningFactor)
{
	RGB_color rgbCol(theColor);
	HSL_color hslCol = NFmiColorSpaces::RGBtoHSL(rgbCol.r, rgbCol.g, rgbCol.b);
	double lightness = hslCol.l;
	if(lightness)
		lightness = lightness + lightness * (theBrightningFactor/100.);
	else // jos lightness oli 0, pitää vain lisätä kirkastus kerroin
		lightness = theBrightningFactor;

	if(lightness > 100)
		lightness = 100;
	if(lightness < 0)
		lightness = 0;

	RGB_color rgbCol2 = NFmiColorSpaces::HSLtoRGB(hslCol.h, hslCol.s/100., lightness/100.);
	return NFmiColor(rgbCol2.r/255.f, rgbCol2.g/255.f, rgbCol2.b/255.f, theColor.Alpha());
}
