//**********************************************************
// C++ Class Name : NFmiUndoableMultiLevelMask 
// ---------------------------------------------------------
// Filetype: (HEADER)
// Filepath: d:/projekti/GDPro/GDTemp/NFmiUndoableMultiLevelMask.h 
// 
// 
// GDPro Properties 
// ---------------------------------------------------
//  - GD Symbol Type    : CLD_Class 
//  - GD Method         : UML ( 2.1.4 ) 
//  - GD System Name    : Met-editor Plan 3 
//  - GD View Type      : Class Diagram 
//  - GD View Name      : undoable multilevelmask 
// ---------------------------------------------------  
//  Author         : pietarin 
//  Creation Date  : Tues - Jun 22, 1999 
// 
// 
//  Description: 
//   Halusin tehd� NFmiMultiLevelMask:iin undo/redo 
//   mahdollisuuden. Tein NFmiMultiLevelMask- 
//   smartpointerin, jolla on multilevelmask ja 
//   lis�ksi undo/redo-lista, jossa on multilevelmask-pointereita.
//   
// 
//  Change Log: 
// 
//**********************************************************
#ifndef  NFMIUNDOABLEMULTILEVELMASK_H
#define  NFMIUNDOABLEMULTILEVELMASK_H

#include "NFmiMultiLevelMask.h"
#include "NFmiPtrList.h"

class NFmiUndoableMultiLevelMask 
{

 public:
   NFmiUndoableMultiLevelMask(unsigned long theSize = 32);
   NFmiUndoableMultiLevelMask(const NFmiUndoableMultiLevelMask& theMask);
   NFmiUndoableMultiLevelMask& operator=(const NFmiUndoableMultiLevelMask& theMask);
   ~NFmiUndoableMultiLevelMask(void);
   NFmiMultiLevelMask* operator->(void){return itsMultiLevelMask;};
   operator NFmiMultiLevelMask*(void){return itsMultiLevelMask;};
   FmiBoolean SnapShotData(void);	// ota maskit talteen
   FmiBoolean Undo(void);							// kysyy onko undo mahdollinen
   FmiBoolean Redo(void);							// kysyy onko redo mahdollinen
   void CommitData(void);							// hyv�ksy tehdyt muutokset (eli tuhoaa undo mahdollisuudet)
   FmiBoolean UndoData(void);						// suorittaa todellisen undon
   FmiBoolean RedoData(void);						// suorittaa todellisen redon

   // HUOM! ei toimi aivan oikein, jos undoleveliksi annetaan 5, on todellinen undo 
   // mahdollista vain 4 kertaa. En jaksa nyt ihmetell� sit� nyt (T:Marko).
   void UndoLevel(int theNewUndoLevel);				// undolevel asetetaan t�ll�

 private:
   void RearrangeUndoTable(void);

   NFmiMultiLevelMask* itsMultiLevelMask; // oikeastaan masterpointer, koska data tuhotaan
   NFmiPtrList<NFmiMultiLevelMask> itsUndoList;
   int itsMaxUndoLevel;
   int itsMaxRedoLevel; // mihin tarvitaan (mervi teki vastaavat smartinfoon)
   int itsCurrentUndoLevel;
   int itsCurrentRedoLevel;

};

#endif
