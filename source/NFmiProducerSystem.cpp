//© Ilmatieteenlaitos/Marko.
//Original 20.6.2006
// 
// Luokka pitää huolta Editoriin konffatuista malli tuottajista.
// Voidaan konffata editorin kolme päätuottajat ja 10 sivu tuottajaa.
// Pää tuottajat näkyvät esim. luotaus valikoissa (toistaiseksi vain 3 radio buttonia käytössä).
// Pää ja Sivu tuottajat näkyvät esim. popup-valikoissa kun erilaisille näytöille valitaan dataa.
//---------------------------------------------------------- NFmiWarningCenterSystem.h

#include "NFmiProducerSystem.h"
#include "NFmiSettings.h"

// generoi vektorin NFmiProducer-otuksia. Useita tuottajia tulee jos id-listassa on useita id:tä
std::vector<NFmiProducer> NFmiProducerInfo::GetProducers(void)
{
	std::vector<NFmiProducer> producers;
	for(unsigned int i = 0; i<itsProducerIds.size(); i++)
		producers.push_back(NFmiProducer(itsProducerIds[i], itsName));
	return producers;
}

NFmiProducerInfo NFmiProducerSystem::GetProducerInfoFromSettings(const std::string &theUsedNameSpaceBase)
{
	NFmiProducerInfo prod;
	prod.Name(NFmiSettings::Require<std::string>(std::string(theUsedNameSpaceBase + "::Name").c_str()));
	prod.ShortName(NFmiSettings::Require<std::string>(std::string(theUsedNameSpaceBase + "::ShortName").c_str()));
	prod.UltraShortName(NFmiSettings::Require<std::string>(std::string(theUsedNameSpaceBase + "::UltraShortName").c_str()));
	prod.Description(NFmiSettings::Require<std::string>(std::string(theUsedNameSpaceBase + "::Description").c_str()));
	std::string idStr(NFmiSettings::Require<std::string>(std::string(theUsedNameSpaceBase + "::ProducerIds").c_str()));
	prod.ProducerIds(NFmiStringTools::Split<std::vector<int> >(idStr, ","));

	return prod;
}

// Settings should be following format
// ProducerSystem::Producer1
// {
//   Name = Hirlam
//   ShortName = Hir
//   Description = Hirlam RCR model
//   ProducerIds = 1,230
// }
// ProducerSystem::Producer2
// {
//   Name = Ecmwf
//   ShortName = Ec
//   Description = Ecmwf global model
//   ProducerIds = 240
// }
// and so on. System will try to find as many Producer# as it can. 
// You should put them in rising order in file to make it clearer.
void NFmiProducerSystem::InitFromSettings(const std::string &theInitNameSpace)
{
	itsProducers.clear();
	itsInitializeLogStr = "";

	int maxDirectoryCount = 20; // lets try to read 20 diffrent models producer infos
	int prodCounter = 0;
	for(int i=1; i <= maxDirectoryCount; i++)
	{
		try
		{
			std::string usedNameSpaceBase = theInitNameSpace + "::Producer";
			usedNameSpaceBase += NFmiStringTools::Convert(i);
			NFmiProducerInfo prodInfo(GetProducerInfoFromSettings(usedNameSpaceBase));
			Add(prodInfo);
			prodCounter++;
		}
		catch(std::exception & /* e */ )
		{
		}
		catch(...)
		{
		}
	}
	if(prodCounter > 0)
	{
		itsInitializeLogStr += "Producer initialization ok. There were ";
		itsInitializeLogStr += NFmiStringTools::Convert(prodCounter);
		itsInitializeLogStr += " model producer settings found in initialization.";
	}
	else
	{
		itsInitializeLogStr += "Error in NFmiProducerSystem::InitFromSettings: Producer initialization failed, no producers were found, check editor.conf -file.";
		throw std::runtime_error(itsInitializeLogStr);
	}
}

// kuten parametrin nimestäkin voi päätellä, on indeksi alkaa ykkösestä eteenpäin
bool NFmiProducerSystem::ExistProducer(unsigned int index1Based) const
{
	// jos annettu indeksi on 0, o-1 -> 4 miljardia unsigned maailmassa, joten riittää yksi testi
	if(index1Based-1 < itsProducers.size())
		return true;
	return false;
}

NFmiProducerInfo& NFmiProducerSystem::Producer(unsigned int index1Based)
{
	// jos annettu indeksi on 0, o-1 -> 4 miljardia unsigned maailmassa, joten riittää yksi testi
	if(index1Based-1 < itsProducers.size())
		return itsProducers[index1Based-1];
	else
	{
		static NFmiProducerInfo dummy;
		return dummy;
	}
}

// Etsi halutun tuottajan ProducerInfo indeksi, eli indeksi millä NFmiProducerInfo-otuksen saa ulos
// Producer-matodilla. Palautuva indeksi on siis 1:stä lähtevä ja 0 tarkoittaa että kyseitä tuottajaa ei löytynyt.
// Etsintä tehdään tuottaja id:tä vertailemalla.
unsigned int NFmiProducerSystem::FindProducerInfo(const NFmiProducer &theProducer)
{
	for(unsigned int index = 0; index < itsProducers.size(); index++)
	{
		NFmiProducerInfo &prodInfo = itsProducers[index];
		std::vector<int> &ids = prodInfo.ProducerIds();
		for(int i = 0; i < static_cast<int>(ids.size()); i++)
		{
			if(ids[i] == theProducer.GetIdent())
				return index+1; // palautetaan siis 1-pohjainen indeksi
		}
	}
	return 0; // ei löytynyt, palautetaan 0;
}
