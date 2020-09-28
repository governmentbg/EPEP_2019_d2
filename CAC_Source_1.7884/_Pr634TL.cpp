#include "cac.h"  // 18.may.2001

#if COURT_TYPE == COURT_AREAL
DEFINE_RESPONSE_TABLE1 (TPrint634TLBookDialog, TScaleDialog)
  EV_BN_CLICKED(IDOK, OKBNClicked),
//  EV_BN_CLICKED(IDC_634BK_CHAIN, ChainBNClicked),  // 03.oct.2001
//  EV_BN_CLICKED(IDC_634BK_SEARCH, SearchBNClicked),  // 18.dec.2000
END_RESPONSE_TABLE ;

TPrint634TLBookDialog :: TPrint634TLBookDialog (TWindow * parent) :
			   TScaleDialog (parent, IDD_PRINT_TRADE_634_BOOK),
                     pBegDate (NULL), pEndDate (NULL), pLawType (NULL),
                     pLawNo (NULL), pLawYear (NULL), pBegSerialNo (NULL)
 {
  pBegDate = new TDateFace (this, IDC_634BK_BEG_DATE,
				    "F_634_DATE_BEG", true) ;
  new TDateFaceUpDown(this, IDC_634BK_BEG_DATE_UPDN, pBegDate);

  pEndDate = new TDateFace (this, IDC_634BK_END_DATE,
				    "F_634_DATE_END", true) ;
  new TDateFaceUpDown(this, IDC_634BK_END_DATE_UPDN, pEndDate);

  pLawType = new TTypeAliasFace (this, IDC_634BK_TYPE, "F_634_TYPE", true,
                                 TYPE_TRADE_OR_CITIZEN_LAWS) ;
  pLawNo = new TLongFace (this, IDC_634BK_NO, "F_634_NO",
                          SIZE_OF_NO);
  pLawYear = new TLongFace (this, IDC_634BK_YEAR, "F_634_YEAR",
                            SIZE_OF_YEAR);
  pBegSerialNo = new TLongFace (this, IDC_634BK_BEG_SERIAL_NO,
                                "F_634_BEG_SERIAL_NO",
                                SIZE_OF_NO);
  html = new TCheckFace(this, IDC_634BK_NEW_FORMAT, "F_634_NewFormat");
 }  // TPrint634TLBookDialog :: TPrint634TLBookDialog

void TPrint634TLBookDialog :: SetupWindow ()
{

  TScaleDialog :: SetupWindow () ;

  {
    pBegDate -> SetDate (Today) ;
    pEndDate -> SetDate (Today) ;
    pLawYear -> SetL (Today.year) ;
  }
  pLawType -> SetSelIndex (0) ;

#if COURT_TYPE == COURT_AREAL   // 20070614
  if (pLawType -> C () != TYPE_TRADE_LAW)
#endif

  {  // Try to tickle
     for (int i = pLawType -> GetCount () - 1 ; i >= 0 ; i --)
     {
        pLawType -> SetSelIndex (i) ;

#if COURT_TYPE == COURT_AREAL   // 20070614
        if (pLawType -> C () == TYPE_TRADE_LAW)
       	  break ;
#endif
     }
  }
  pLawNo -> SetL (0) ;
  pBegSerialNo -> SetL (1) ;

	if (COURT_LOCK)
	{
		html->SetCheck(BF_UNCHECKED);
		html->EnableWindow(false);
	}
	else
		html->SetCheck(BF_CHECKED);
	
}  // TPrint634TLBookDialog :: SetupWindow

void TPrint634TLBookDialog :: SearchBNClicked ()
 {
  message ("%s", "Търсенето не е реализирано засега.") ;
  pLawNo -> SetFocus () ;
 }  // TPrint634TLBookDialog :: SearchBNClicked

bool TPrint634TLBookDialog :: IsFilledOK () const
 {
  bool valid = true ;
  const char * const format =
    "Моля, попълнете правилно данните за %s." ;

   {
    const CDate bD = pBegDate -> Date () ;
    const CDate eD = pEndDate -> Date () ;

    if (bD.Empty () || eD.Empty () || bD > eD)
     {
      valid = false ;
      error (format, "периода") ;  // message --> error  30.mar.2001
     }
   }
  if (valid)
   {
    if (TRCDKey_ (pLawType -> C (), pLawNo -> L (),
                  pLawYear -> L ()).FilledOK () == false)
     {
      valid = false ;
      error (format, "делото") ;  // message --> error  30.mar.2001
     }
   }
  return valid ;
 }  // TPrint634TLBookDialog :: IsFilledOK

void TPrint634TLBookDialog :: OKBNClicked ()
 {
  if (IsFilledOK ())
   {
    PrintLaws (true) ;
    pLawNo -> SetFocus () ;
   }
 }  // TPrint634TLBookDialog :: OKBNClicked

void TPrint634TLBookDialog :: ChainBNClicked ()
 {
  if (IsFilledOK ())
   {
    PrintLaws (false) ;
    pLawNo -> SetFocus () ;
   }
 }  // TPrint634TLBookDialog :: ChainBNClicked

void TPrint634TLBookDialog ::
       ExtractDialogData (Trade634DlgData & dd) const
 {
  dd.Clear () ;
  dd.begD = pBegDate -> Date () ;
  dd.endD = pEndDate -> Date () ;
  dd.lawKey.type = pLawType -> C () ;
  dd.lawKey.no = pLawNo -> L () ;
  dd.lawKey.year = pLawYear -> L () ;
  dd.begSerialNo = pBegSerialNo -> L () ;
 }  // TPrint634TLBookDialog :: ExtractDialogData

void TPrint634TLBookDialog :: PrintLaws (const bool truncChain)
 {
	ShowWindow(SW_HIDE);	// 2015:271

	if (html->GetCheck() == BF_UNCHECKED)
	{
		Trade634DlgData dd ;
		ExtractDialogData (dd) ;
		dd.truncChainAtEndD = truncChain ;
		{
			char t[2];
			t[0] = dd.lawKey.type;
			t[1] = '\0';
			register_usage(REGIME_TRADE_634_BOOK, truncChain ? 1 : 2, t,
			"Кн. чл.634В ТЗ");
		}
		TradeLaw634Book (this).PrintTradeLaw634Book (dd) ;
	}
	else
		print_book634(this, pBegDate->Date(), pEndDate->Date(), pLawType->C(), pLawNo->L(), pLawYear->L(), pBegSerialNo->L());

	ShowWindow(SW_SHOW);
 }  // TPrint634TLBookDialog :: PrintLaws
#endif  // AREAL
