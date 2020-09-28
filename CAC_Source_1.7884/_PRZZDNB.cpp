DEFINE_RESPONSE_TABLE1(TPrintBookZZDNDialog, TGroupDialog)
  EV_BN_CLICKED(IDOK, OKBNClicked),
  EV_CHILD_NOTIFY_ALL_CODES(IDC_PRINT_BOOK_ZZDN_FIND_BY, HandleGroupBoxMsg),
END_RESPONSE_TABLE;

void TPrintBookZZDNDialog::HandleGroupBoxMsg (uint)
{
    if (rb_SelectCrit.GetSelectionIdc() == IDC_PRINT_BOOK_ZZDN_BY_NO_YEAR)
    {
      pFromNo->EnableWindow (true);
      pToNo->EnableWindow (true);
      pYear->EnableWindow (true);
      pBegD->EnableWindow (false);
      pEndD->EnableWindow (false);
    }
    else
    {
      pFromNo->EnableWindow (false);
      pToNo->EnableWindow (false);
      pYear->EnableWindow (false);
      pBegD->EnableWindow (true);
      pEndD->EnableWindow (true);
    }
}  // TPrintBookZZDNDialog::HandleGroupBoxMsg

TPrintBookZZDNDialog :: TPrintBookZZDNDialog (TWindow * parent,
				 ZZDN_Dlg_data * aZZDN_Dlg_dataPtr,
				 int resId) :

				 TGroupDialog (parent, NULL, resId),
				 pZZDN_Dlg_data (aZZDN_Dlg_dataPtr)
 {
  //TTypeAliasFace (TWindow * parent, int resourceId, char * name,
  //                const bool followRights = true,
  //                const char * const typesAllowed = NULL) ;  // Empty // string is not allowed
  //
  pClg = new TTypeAliasFace (this, IDC_PRINT_BOOK_ZZDN_TYPE, "F_TYPE",
				     true, TYPE_TRADE_OR_CITIZEN_LAWS) ;

  // IDC_PRINT_BOOK_ZZDN_BY_NO_YEAR  / IDC_PRINT_BOOK_ZZDN_BY_DATE
  TGroupBox * pSelectCrit = new TGroupBox (this, IDC_PRINT_BOOK_ZZDN_FIND_BY) ;
  rb_SelectCrit.SetGrpBox (pSelectCrit) ;
  rb_SelectCrit.Add(new TRadioButton (this, IDC_PRINT_BOOK_ZZDN_BY_NO_YEAR, pSelectCrit),
						  IDC_PRINT_BOOK_ZZDN_BY_NO_YEAR) ;
  rb_SelectCrit.Add(new TRadioButton (this, IDC_PRINT_BOOK_ZZDN_BY_DATE,    pSelectCrit),
						  IDC_PRINT_BOOK_ZZDN_BY_DATE) ;

  pFromNo = new TLongFace (this, IDC_PRINT_BOOK_ZZDN_NO_MIN, "F_FROM_NO",
				 SIZE_OF_NO) ;
  pToNo   = new TLongFace (this, IDC_PRINT_BOOK_ZZDN_NO_MAX, "F_TO_NO",
				 SIZE_OF_NO) ;
  pYear   = new TLongFace (this, IDC_PRINT_BOOK_ZZDN_YEAR, "F_YEAR",
				 SIZE_OF_NO) ;

  pBegD = new TDateFace (this, IDC_PRINT_BOOK_ZZDN_MIN_DATE, "F_DATE_BEGIN",
				 true) ;
  new TDateFaceUpDown(this, IDC_PRINT_BOOK_ZZDN_MIN_DATE_UPDN, pBegD);

  pEndD = new TDateFace (this, IDC_PRINT_BOOK_ZZDN_MAX_DATE, "F_DATE_END",
				 true) ;
  new TDateFaceUpDown(this, IDC_PRINT_BOOK_ZZDN_MAX_DATE_UPDN, pEndD);

  pBN   = new TLongFace (this, IDC_PRINT_BOOK_ZZDN_START_NO, "F_NO_BEGIN",
				 SIZE_OF_NO) ;
 }  // TPrintBookZZDNDialog :: TPrintBookZZDNDialog

bool TPrintBookZZDNDialog :: isValid ()
{
  bool rT = true ;

  const char lawType = pClg -> C () ;
  if ( lawType != TYPE_CITIZEN_LAW )
     rT = error("\"Вид дело\" трябва да е Гражданско дело") ;
  if (rT)
  {  if ( (pFromNo->L()>0.00) && (pToNo->L()>0.00) )
	  if (pFromNo->L()>pToNo->L())
	     rT = incorrect_period_given (this, IDC_PRINT_BOOK_ZZDN_NO_MIN) ;
  }
  if (rT)
  {
     if ( (pBegD -> Date ().Empty() == false)&&(pEndD -> Date ().Empty() == false) )
	  if (pBegD -> Date() > pEndD -> Date() )
	     rT = incorrect_period_given (this, IDC_PRINT_BOOK_ZZDN_MIN_DATE) ;
  }
  if (rT)
  {
     if ( pBN->L() < 1 )
	 rT = error("\"Пореден номер\" трябва да е по-голям или равен на 1.") ;
  }
  return rT ;
}  // TPrintTimeBookDialog :: isValid

void TPrintBookZZDNDialog :: SetupWindow ()
 {
  TGroupDialog :: SetupWindow () ;
    {
    pBegD->SetDate (Today) ;
    pEndD->SetDate (Today) ;
    pFromNo->SetL(1);
    pToNo->SetL(0);
    pYear->SetL(Today.year);
    pBN->SetL(1);

    rb_SelectCrit.SetSelectionIdc (IDC_PRINT_BOOK_ZZDN_BY_NO_YEAR) ;
    }

   pClg -> SetSelIndex (0) ;
   pClg -> EnableWindow (false) ;

 }  // TPrintBookZZDNDialog :: SetupWindow

void TPrintBookZZDNDialog :: OKBNClicked ()
 {
    if (isValid () == false)
	 return ;
    if ( rb_SelectCrit.GetSelectionIdc () == IDC_PRINT_BOOK_ZZDN_BY_NO_YEAR )
	pZZDN_Dlg_data -> selectCrit1 = true;
    else pZZDN_Dlg_data -> selectCrit1 = false;

    pZZDN_Dlg_data -> FromNo = pFromNo -> L () ;
    pZZDN_Dlg_data -> ToNo   = pToNo -> L () ;
    pZZDN_Dlg_data -> Year   = pYear -> L () ;

    pZZDN_Dlg_data -> begD = pBegD -> Date () ;
    pZZDN_Dlg_data -> endD = pEndD -> Date () ;
    pZZDN_Dlg_data -> begN = pBN -> L () ;

    CmOk () ;

 }  // TPrintBookZZDNDialog :: OKBNClicked
// 20051010 - end
