//**********************************************************
// C++ Class Name : NFmiMetEditorCoordinatorMapOptions 
// ---------------------------------------------------------
// Filetype: (SOURCE)
// Filepath: D:/projekti/GDPro/GDTemp/NFmiMetEditorCoordinatorMapOptions.cpp 
// 
// 
// GDPro Properties 
// ---------------------------------------------------
//  - GD Symbol Type    : CLD_Class 
//  - GD Method         : UML ( 4.0 ) 
//  - GD System Name    : editori virityksiä 2000 syksy 
//  - GD View Type      : Class Diagram 
//  - GD View Name      : koordinaatio optiot 
// ---------------------------------------------------  
//  Author         : pietarin 
//  Creation Date  : Thur - Sep 14, 2000 
// 
//  Change Log     : 
// 
//**********************************************************
#include "NFmiMetEditorCoordinatorMapOptions.h"
#include "NFmiLocationBag.h"
//--------------------------------------------------------
// Constructor/Destructor 
//--------------------------------------------------------
NFmiMetEditorCoordinatorMapOptions::NFmiMetEditorCoordinatorMapOptions (void)
:itsCoordinationState(0) // mitä tämä muuttuja tarkoittikaan????
,itsDefaultViewState(0) // 0=mean
,itsCoordinatioTimeScaleInMinutes(6*60)
,itsTimeScaleUsage(1) // 1=annetusta ajasta eteenpäin
,itsLocationViewingSetting(0) // 0=hilamuodossa
,itsAreaCheckingSize(0,0)
,fShowDataInTerseMode(false)
,itsLocationBagVector(0)
,itsMinOffSet(0, 0.5)
,itsMaxOffSet(0, -0.5)
,itsMeanOffSet(0, 0)
,itsSumOffSet(0.5, 0)
,fCoordinatorMapMode(false)
{
}
NFmiMetEditorCoordinatorMapOptions::~NFmiMetEditorCoordinatorMapOptions (void)
{
}
//--------------------------------------------------------
// GetWantedTimeBag 
//--------------------------------------------------------
// Laskee annettuun aikaan säätöihin sopivan timebagin.
NFmiTimeBag NFmiMetEditorCoordinatorMapOptions::GetWantedTimeBag (const NFmiMetTime& theTime, int theTimeResolution) // in minutes
{
	NFmiMetTime start(theTime);
	NFmiMetTime end(theTime);
	if(itsTimeScaleUsage == 0)
	{
		start.ChangeByMinutes(-itsCoordinatioTimeScaleInMinutes);
		end.ChangeByMinutes(itsCoordinatioTimeScaleInMinutes);
	}
	else if(itsTimeScaleUsage == 1)
	{
		end.ChangeByMinutes(itsCoordinatioTimeScaleInMinutes);
	}
	else if(itsTimeScaleUsage == 2)
	{
		start.ChangeByMinutes(-itsCoordinatioTimeScaleInMinutes);
	}

	NFmiTimeBag returnVal(start, end, theTimeResolution);
	return returnVal;
}
//--------------------------------------------------------
// GetGridAreaInRectForm 
//--------------------------------------------------------
// Antaa halutun hilahakualueen rectinä esim. jos pointin arvo oli 1,1 tämä palauttaa rectin jonka arvo on -1,-1 ja 1,1
NFmiRect NFmiMetEditorCoordinatorMapOptions::GetGridAreaInRectForm (void)
{
   NFmiRect returnVal;
   return returnVal;
}
//--------------------------------------------------------
// IsLocationBagUsed 
//--------------------------------------------------------
bool NFmiMetEditorCoordinatorMapOptions::IsLocationBagUsed (void)
{
   return (itsLocationViewingSetting > 0);
}
//--------------------------------------------------------
// WantedLocations 
//--------------------------------------------------------
// Palauttaa locationbagin johon asetus osoittaa
NFmiLocationBag* NFmiMetEditorCoordinatorMapOptions::WantedLocations (void)
{
	if(itsLocationBagVector && static_cast<long>(itsLocationBagVector->size()) >= itsLocationViewingSetting)
		return (*itsLocationBagVector)[itsLocationViewingSetting-1];
	else
		return 0;
}
//--------------------------------------------------------
// ReadLocationBags 
//--------------------------------------------------------
bool NFmiMetEditorCoordinatorMapOptions::ReadLocationBags (std::vector<NFmiString> & theFileNameList)
{
	DestroyLocationBags();
	itsLocationBagVector = new std::vector<NFmiLocationBag*>;

	bool status = false;
	for(unsigned int i=0; i < theFileNameList.size(); i++)
		status |= ReadLocationBagToVector(theFileNameList[i]);
	return false;
}

bool NFmiMetEditorCoordinatorMapOptions::ReadLocationBagToVector(const NFmiString& theFileName)
{
	using namespace std;
	ifstream in(theFileName);
	if(in)
	{
		NFmiLocationBag* locats = new NFmiLocationBag;
		in >> *locats;
		in.close();
		itsLocationBagVector->push_back(locats);
	}
	return false;
}

//--------------------------------------------------------
// DestroyLocationBags 
//--------------------------------------------------------
// Pitää tuhota erillisellä funktiolla, koska locationbagvektori voi olla monessa paikassa käytössä.
void NFmiMetEditorCoordinatorMapOptions::DestroyLocationBags (void)
{
	if(itsLocationBagVector)
	{
		NFmiLocationBag* locs = 0;
		for(unsigned int i=0; i < itsLocationBagVector->size(); i++)
		{
			locs = itsLocationBagVector->at(i);
			delete locs;
		}
		delete itsLocationBagVector;
		itsLocationBagVector = 0;
	}
}
//--------------------------------------------------------
// LocationBagCount 
//--------------------------------------------------------
int NFmiMetEditorCoordinatorMapOptions::LocationBagCount (void)
{
   return itsLocationBagVector->size();
}
//--------------------------------------------------------
// CurrentSymbolOffSet 
//--------------------------------------------------------
// Palauttaa halutun symboli offsetin ottaen huomioon, mikä moodi on päällä (min, max ...)
NFmiPoint NFmiMetEditorCoordinatorMapOptions::CurrentSymbolOffSet (void)
{
	switch(itsDefaultViewState)
	{
	case 0: // mean
		return itsMeanOffSet;
	case 1: // min
		return itsMinOffSet;
	case 2: // max
		return itsMaxOffSet;
	case 3: // sum
		return itsSumOffSet;
	}
   return NFmiPoint();
}

void NFmiMetEditorCoordinatorMapOptions::Write(std::ostream& os) const
{
//	os << itsValue;
}

void NFmiMetEditorCoordinatorMapOptions::Read(std::istream& is)
{
//	is >> itsValue;
}

bool NFmiMetEditorCoordinatorMapOptions::CoordinatorMapMode(void)
{
	return fCoordinatorMapMode;
}

void NFmiMetEditorCoordinatorMapOptions::CoordinatorMapMode(bool newState)
{
	fCoordinatorMapMode = newState;
}
