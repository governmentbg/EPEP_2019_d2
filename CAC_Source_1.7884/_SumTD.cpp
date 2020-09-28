#include "cac.h"

DEFINE_RESPONSE_TABLE1 (TBaseTextDialog, TScaleDialog)// 26.sep.2003
  EV_BN_CLICKED (IDOK, OKBNClicked),
END_RESPONSE_TABLE ;

TBaseTextDialog :: TBaseTextDialog (
			 TWindow * parent, char * const txtRcv,
			 size_t txtRcvSz, const bool noTxtBnToBreak,
                   int clmnWidth, int clmnRows, // 27.aug.2003
			 int resId) :
    TScaleDialog (parent, resId),  // 26.sep.2003
    textReceiver (txtRcv), textReceiverSz (txtRcvSz), pSummTxt (NULL),
    noTextBnToBreak (noTxtBnToBreak),
    columnWidth (clmnWidth), columnRows (clmnRows)
 {
  spare [0] = '\0' ;
  if (textReceiver && textReceiverSz)
    pSummTxt = new
                   TCatchPasteEdit  // TEdit  // 10.dec.2001
                         (this, IDC_SUMM_TXT, textReceiverSz) ;
  else
    pSummTxt = new
                   TCatchPasteEdit  // TEdit  // 10.dec.2001
                         (this, IDC_SUMM_TXT, sizeof (spare)) ;
 }  // TBaseTextDialog :: TBaseTextDialog

void TBaseTextDialog :: SetupWindow ()
 {
  TScaleDialog :: SetupWindow () ;  // 26.sep.2003 
  if (textReceiver && textReceiverSz)
   {
    pSummTxt -> Transfer (textReceiver, tdSetData) ;
    setmem (textReceiver, textReceiverSz, '\0') ;
   }
  else
    pSummTxt -> EnableWindow (false) ;
  SetCaption ("Текст на писмото") ;
  if (noTextBnToBreak)
    SetDlgItemText (IDCANCEL, "Прекъсване") ;
 }  // TBaseTextDialog :: SetupWindow


bool TBaseTextDialog :: DataOK ()
 {  // An old varianth in VersionArchives\1820\Preliminary\_appname.j
  // --> _sumtd.cpp
  bool res = (textReceiver && textReceiverSz > 1) ;

  if (res)
    if (columnWidth > 0 && columnRows > 0)
     {
      int actualRows ;
      CharPtr beforeLast (columnWidth + 1) ;
      CharPtr last (columnWidth + 1) ;

      pSummTxt -> Transfer (textReceiver, tdGetData) ;
       {
        column c (columnWidth, textReceiver) ;

        actualRows = c.getRowsCount () ;
        if (actualRows > columnRows)
         {
          ostrstream (last, last.Size ())
            << c [columnRows - 1] << ends ;
          last [last.Size () - 1] = '\0' ;
          if (columnRows > 1)
           {
            ostrstream (beforeLast, beforeLast.Size ())
              << c [columnRows - 2] << ends ;
            beforeLast [beforeLast.Size () - 1] = '\0' ;
           }
         }
       }
      textReceiver [0] = '\0' ;
      if (actualRows > columnRows)
        res = ask ("Текстът е %d реда дълъг, а на бланката "
                   "има място само за %d.\n"
                   "Последните думи, за които има място са:\n"
                   "\n"
                   "%-*s\n"
                   "%-*s\n"
                   "\n"
                   "Ще го редактирате ли?",
                   actualRows, columnRows,
                   beforeLast.Size () <= 255 ?
                     beforeLast.Size () - 1 : 255,
                   beforeLast.Str (),
                   last.Size () <= 255 ? last.Size () - 1 : 255,
                   last.Str ()
                  ) == false ;
     }
  return res ;
 }  // TBaseTextDialog :: DataOK

void TBaseTextDialog :: OKBNClicked ()
 {
  if (DataOK ())
   {
    pSummTxt -> Transfer (textReceiver, tdGetData) ;
    CmOk () ;
   }
 }  // TBaseTextDialog :: OKBNClicked

TAnyLetterTextDialog :: TAnyLetterTextDialog (
				TWindow * parent, char * const txtRcv,
				size_t txtRcvSz, const char * const capt,
                        const bool noTxtBnToBreak, int clmnWidth, int clmnRows,
                        int resId) :
    TBaseTextDialog (parent, txtRcv, txtRcvSz, noTxtBnToBreak,
                     clmnWidth, clmnRows, resId),
    title (capt) 
 {
 }  // TAnyLetterTextDialog :: TAnyLetterTextDialog

void TAnyLetterTextDialog :: SetupWindow ()
 {
  TBaseTextDialog :: SetupWindow () ;
  if (title && title [0])
    SetCaption (title) ;
  else
    SetCaption ("Текст на съобщението") ;
 }  // TAnyLetterTextDialog :: SetupWindow


TAnyShortTextDialog :: TAnyShortTextDialog (
			     TWindow * parent, char * const txtRcv,
			     size_t txtRcvSz, const char * const capt,
                       const bool noTxtBnToBreak,
                       int resId) :
    TAnyLetterTextDialog (parent, txtRcv, txtRcvSz, capt, noTxtBnToBreak,
                          0, 0,  // 27.aug.2003
                          resId)
 {
 }  // TAnyShortTextDialog :: TAnyShortTextDialog
