//© Ilmatieteenlaitos/Persa.

// 1.3.1994   Ja t‰n‰‰n se presidentti vannoi virkavalansa	Persa
// 13.11.1997 Ja t‰n‰‰n se suunnittelija lis‰s l‰pin‰kyvyyden (alpha channel) v‰reihin  EL

#ifndef __FCOLTYP_H__
#define __FCOLTYP_H__

//	typedef double FmiColorValue;
typedef float FmiColorValue;  // 10.10.2001/Marko Aikamoinen tarve ollut yhden v‰rin kertomiseen kun
                              // ollut 4 x double = 32 tavua = 256 bitti‰ (kun truecolor saadaan
                              // aikaan 32 bitill‰)

typedef struct
{
  FmiColorValue red, green, blue, alpha;
} FmiRGBColor;

#endif  //__FCOLTYP_H__
