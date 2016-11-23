#pragma once

#include "NFmiProducerName.h"
#include "NFmiLevelType.h"
#include "NFmiDataMatrix.h"

#include "boost/shared_ptr.hpp"

class NFmiFastQueryInfo;

// Kun smarttool:ia tulkitaan, siin‰ saattaa olla osia, joita voi k‰ytt‰‰ vain macroParamien yhteydess‰.
// T‰h‰n luokkaan talletetaan kaikkea, mit‰ extra tietoa voi lˆyty‰ annetusta skriptist‰.
class NFmiExtraMacroParamData
{
public:
    NFmiExtraMacroParamData();
private:
    // Jos skriptiss‰ on annettu haluttu laskenta resoluutio tyyliin "resolution = 12.5", talletetaan kyseinen luku t‰h‰n. 
    // Jos ei ole asetettu, on arvo missing.
    double itsGivenResolutionInKm;
    // Jos skriptiss‰ on annettu resoluutio data muodossa tyyliin "resolution = hir_pressure",
    // talletetaan lausekkeen tuottaja ja level tyyppi n‰ihin kahteen arvoon.
    FmiProducerName itsProducerId; // puuttuva arvo on kFmiNoProducer eli 0
    FmiLevelType itsLevelType; // puuttuva arvo on kFmiNoLevelType eli 0

    // t‰h‰n lasketaan datasta haluttu resoluutio  makro-parametrien laskuja varten pit‰‰ pit‰‰ yll‰ yhden 
    // hilan kokoista dataa  (yksi aika,param ja level, editoitavan datan hplaceDesc). T‰h‰n
    // dataan on laskettu haluttu resoluutio t‰m‰n macroParamin laskujen ajaksi.    
    double itsDataBasedResolutionInKm; 
    boost::shared_ptr<NFmiFastQueryInfo> itsResolutionMacroParamData;
    // t‰h‰n talletetaan editoitavan datan hilan suuruinen kFloatMissing:eilla
    // alustettu matriisi ett‰ sill‰ voi alustaa makroParam dataa ennen laskuja
    NFmiDataMatrix<float> itsResolutionMacroParamMissingValueMatrix;

};

