//**********************************************************
// C++ Class Name : NFmiMetEditorCoordinatorMapOptions 
// ---------------------------------------------------------
// Filetype: (HEADER)
// Filepath: D:/projekti/GDPro/GDTemp/NFmiMetEditorCoordinatorMapOptions.h 
// 
// 
// GDPro Properties 
// ---------------------------------------------------
//  - GD Symbol Type    : CLD_Class 
//  - GD Method         : UML ( 4.0 ) 
//  - GD System Name    : editori virityksi‰ 2000 syksy 
//  - GD View Type      : Class Diagram 
//  - GD View Name      : koordinaatio optiot 
// ---------------------------------------------------  
//  Author         : pietarin 
//  Creation Date  : Thur - Sep 14, 2000 
// 
//  Change Log     : 
// 
//**********************************************************
#ifndef  NFMIMETEDITORCOORDINATORMAPOPTIONS_H
#define  NFMIMETEDITORCOORDINATORMAPOPTIONS_H

#include "NFmiTimeBag.h"
#include "NFmiRect.h"
#include <vector>
#include <iosfwd> // esittelee ostream ja istream (mutta niit‰ ei voi k‰ytt‰‰!)

class NFmiLocationBag;

class NFmiMetEditorCoordinatorMapOptions 
{

public:
   NFmiMetEditorCoordinatorMapOptions (void) ;
   ~NFmiMetEditorCoordinatorMapOptions (void) ;
   NFmiTimeBag GetWantedTimeBag (const NFmiMetTime& theTime, int theTimeResolution) ;
   NFmiRect GetGridAreaInRectForm (void) ;
   bool IsLocationBagUsed (void) ;
   NFmiLocationBag* WantedLocations (void) ;
   bool ReadLocationBags (std::vector<NFmiString> & theFileNameList) ;
   void DestroyLocationBags (void) ;
   int LocationBagCount (void) ;
   NFmiPoint CurrentSymbolOffSet (void) ;
   inline void CoordinationState (int value) {itsCoordinationState = value;}
   inline int CoordinationState () const {return itsCoordinationState;}
   inline void DefaultViewState (int value) {itsDefaultViewState = value;}
   inline int DefaultViewState () const {return itsDefaultViewState;}
   inline void CoordinatioTimeScaleInMinutes (int value) {itsCoordinatioTimeScaleInMinutes = value;}
   inline int CoordinatioTimeScaleInMinutes () const {return itsCoordinatioTimeScaleInMinutes;}
   inline void TimeScaleUsage (int value) {itsTimeScaleUsage = value;}
   inline int TimeScaleUsage () const {return itsTimeScaleUsage;}
   inline void LocationViewingSetting (int value) {itsLocationViewingSetting = value;}
   inline int LocationViewingSetting () const {return itsLocationViewingSetting;}
   inline void AreaCheckingSize (NFmiPoint value) {itsAreaCheckingSize = value;}
   inline NFmiPoint AreaCheckingSize () const {return itsAreaCheckingSize;}
   inline void ShowDataInTerseMode (bool value) {fShowDataInTerseMode = value;}
   inline bool ShowDataInTerseMode () const {return fShowDataInTerseMode;}
   inline std::vector<NFmiLocationBag*> * LocationBagVector () const {return itsLocationBagVector;}
   inline void MinOffSet (NFmiPoint value) {itsMinOffSet = value;}
   inline NFmiPoint MinOffSet () const {return itsMinOffSet;}
   inline void MaxOffSet (NFmiPoint value) {itsMaxOffSet = value;}
   inline NFmiPoint MaxOffSet () const {return itsMaxOffSet;}
   inline void MeanOffSet (NFmiPoint value) {itsMeanOffSet = value;}
   inline NFmiPoint MeanOffSet () const {return itsMeanOffSet;}
   inline void SumOffSet (NFmiPoint value) {itsSumOffSet = value;}
   inline NFmiPoint get_itsSumOffSet () const {return itsSumOffSet;}
   bool CoordinatorMapMode(void);
   void CoordinatorMapMode(bool newState);

   void Write(std::ostream& os) const;
   void Read(std::istream& is);
 
private:
   bool ReadLocationBagToVector(const NFmiString& theFileName);

   int itsCoordinationState;
   // mihin tilaan avatut n‰ytˆt oletusarvoisesti laitetaan (0=mean,1=min,2=max,3=sum)
   int itsDefaultViewState;
   // milt‰ aika v‰lilt‰ dataa haetaan
   int itsCoordinatioTimeScaleInMinutes;
   // miten aika-akselia k‰ytet‰‰n (0=+-minuuttua alkaen annetusta ajasta, 1=vain eteenp‰in annetusta ajasta, 2=vain taaksep‰in)
   int itsTimeScaleUsage;
   // tarkastellaan dataa 0=hilamuodossa, 1= asemalista 1. mukaan, 2= asemalista 2. mukaan jne.
   int itsLocationViewingSetting;
   // T‰m‰ pit‰‰ sis‰ll‰‰ hila-alueen koon, mik‰ k‰yd‰‰n l‰pi kun haetaan johonkin pisteeseen esim. max arvoa. jos 0,0, haetaan arvo vain 
   // yhden hilapisteen alueelta. Jos Arvo on 1,1, haetaan arvoa ruudukosta -1,-1 ja 1,1 alueelta tutkittavan hilan ymp‰rilt‰ eli 9 hilapisteen 
   // alueelta.
   NFmiPoint itsAreaCheckingSize;
   // Jos itsAreaCheckingSize on suurempi kuin 0,0 eli dataa lasketaan monen hilan alueelta, ja parametri piirret‰‰n esim. symboleina, 
   // t‰llˆin voisi olla hyv‰ harventaa n‰ytˆss‰ olevia symboleja samassa suhteessa (jos true, harvennetaan ja false niin ei harvenneta).
   bool fShowDataInTerseMode;
   // T‰h‰n on talletettu kaiki mahdolliset locationbagit, joita editorin k‰ytt‰j‰ voi haluta k‰ytt‰‰ tarkasteluissaan. N‰it‰ ei omisteta, 
   // vaan ne tuhotaan erillisell‰ funktiolla.
   std::vector<NFmiLocationBag*> * itsLocationBagVector;
   NFmiPoint itsMinOffSet;
   NFmiPoint itsMaxOffSet;
   NFmiPoint itsMeanOffSet;
   NFmiPoint itsSumOffSet;

   bool fCoordinatorMapMode; // true=tila p‰‰ll‰ ja false = ei ole p‰‰ll‰

};

inline std::ostream& operator<<(std::ostream& os, NFmiMetEditorCoordinatorMapOptions& item){item.Write(os); return os;}
inline std::istream& operator>>(std::istream& is, NFmiMetEditorCoordinatorMapOptions& item){item.Read(is); return is;}

#endif
