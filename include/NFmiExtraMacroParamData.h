#pragma once

#include "NFmiProducer.h"
#include "NFmiLevelType.h"
#include "NFmiDataMatrix.h"

#include "boost/shared_ptr.hpp"

class NFmiFastQueryInfo;
class NFmiInfoOrganizer;
class NFmiArea;

// Kun smarttool:ia tulkitaan, siin‰ saattaa olla osia, joita voi k‰ytt‰‰ vain macroParamien yhteydess‰.
// T‰h‰n luokkaan talletetaan kaikkea, mit‰ extra tietoa voi lˆyty‰ annetusta skriptist‰.
class NFmiExtraMacroParamData
{
public:
    NFmiExtraMacroParamData();

    void FinalizeData(NFmiInfoOrganizer &theInfoOrganizer);
    bool UseSpecialResolution() const;
    static void SetUsedAreaForData(boost::shared_ptr<NFmiFastQueryInfo> &theData, const NFmiArea *theUsedArea);

    float GivenResolutionInKm() const { return itsGivenResolutionInKm; }
    void GivenResolutionInKm(float newValue) {itsGivenResolutionInKm = newValue;}
    const NFmiProducer& Producer() const { return itsProducer; }
    void Producer(const NFmiProducer &newValue) { itsProducer = newValue; }
    FmiLevelType LevelType() const { return itsLevelType; }
    void LevelType(FmiLevelType newValue) { itsLevelType = newValue; }

    boost::shared_ptr<NFmiFastQueryInfo> ResolutionMacroParamData() { return itsResolutionMacroParamData; }
    static void AdjustValueMatrixToMissing(const boost::shared_ptr<NFmiFastQueryInfo> &theData, NFmiDataMatrix<float> &theValueMatrix);

private:
    void InitializeResolutionData(NFmiInfoOrganizer &theInfoOrganizer, float usedResolutionInKm);
    void InitializeDataBasedResolutionData(NFmiInfoOrganizer &theInfoOrganizer, const NFmiProducer &theProducer, FmiLevelType theLevelType);

    // Jos skriptiss‰ on annettu haluttu laskenta resoluutio tyyliin "resolution = 12.5", talletetaan kyseinen luku t‰h‰n. 
    // Jos ei ole asetettu, on arvo missing.
    float itsGivenResolutionInKm;
    // Jos skriptiss‰ on annettu resoluutio data muodossa tyyliin "resolution = hir_pressure",
    // talletetaan lausekkeen tuottaja ja level tyyppi n‰ihin kahteen arvoon.
    NFmiProducer itsProducer; // puuttuva arvo on kun id = 0
    FmiLevelType itsLevelType; // puuttuva arvo on kFmiNoLevelType eli 0

    // t‰h‰n lasketaan datasta haluttu resoluutio  makro-parametrien laskuja varten pit‰‰ pit‰‰ yll‰ yhden 
    // hilan kokoista dataa  (yksi aika,param ja level, editoitavan datan hplaceDesc). T‰h‰n
    // dataan on laskettu haluttu resoluutio t‰m‰n macroParamin laskujen ajaksi.    
    float itsDataBasedResolutionInKm; 
    boost::shared_ptr<NFmiFastQueryInfo> itsResolutionMacroParamData;
};

