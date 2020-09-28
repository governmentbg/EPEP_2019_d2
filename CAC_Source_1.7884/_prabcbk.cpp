DEFINE_RESPONSE_TABLE1(TPrintAbcBookDialog, TScaleDialog)// 200806M TPrintDescBookDialog)
//{{TPrintAbcBookDialogRSP_TBL_BEGIN}}
  EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TPrintAbcBookDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TPrintAbcBookDialog Implementation}}

TPrintAbcBookDialog :: TPrintAbcBookDialog (TWindow * parent,
					begNLEndNLYear * aBegNLEndNLYearPtr,
					char * selCollegePtr) :
				// 200806M  TPrintDescBookDialog
				// 200806M    (parent, aBegNLEndNLYearPtr,
				// 200806M    selCollegePtr, resId),
				// 200806M    pBegNLEndNLYear (aBegNLEndNLYearPtr)
				// 200806M    TGroupDialog

				// TGroupDialog(TWindow* parent, TGroup *group, int resId);
				 TScaleDialog (parent, IDD_PRINT_ABC_BOOK),
				  pBegNLEndNLYear (aBegNLEndNLYearPtr),
				  pSelCollege (selCollegePtr)

 {
  // INSERT>> Your constructor code here.

  pType = new TTypeAliasFace (this, IDC_PRINT_ABC_COLLEGE, "F_TYPE",
                              true, NULL) ;

  pBN = new TLongFace (this, IDC_PRINT_ABC_NO_BEGIN, "F_NO_BEGIN",
                       SIZE_OF_NO) ;
  pEN = new TLongFace (this, IDC_PRINT_ABC_NO_END, "F_NO_END",
                       SIZE_OF_NO) ;
  pY = new TLongFace (this, IDC_PRINT_ABC_YEAR, "F_YEAR", SIZE_OF_NO) ;
  pBL = new TCitizenNameFace (this, IDC_PRINT_ABCB_LETT_BEGIN,
                              "F_LETT_BEGIN", 1 + 1) ;
  pEL = new TCitizenNameFace (this, IDC_PRINT_ABCB_LETT_END,
					"F_LETT_END", 1 + 1) ;
  pWantCheckup = new TCheckBox (this, IDC_PRINT_ABCB_WANT_CHECKUP,NULL) ;
  pJan2005Format = new TCheckBox (this, IDC_PRINT_ABC_NEW_FORMAT, NULL) ;

   {
    TGroupBox * pPrintGroupBx =
      new TGroupBox (this, IDC_PRINT_ABCB_PRINTING) ;

   // new TRadioButton (this, IDC_PRINT_ABCB_NORMAL_PRINT, pPrintGroupBx) ;
    pWantBrokenPrint = new TRadioButton (this,
                                         IDC_PRINT_ABCB_BROKEN_PRINT,
						     pPrintGroupBx) ;
    pWantPrintByPages = new TRadioButton (this,
							IDC_PRINT_ABCB_PRINT_BY_PG,
							pPrintGroupBx) ;
    pWantUnbrokenPrint = new TRadioButton (this,
							 IDC_PRINT_ABCB_NORMAL_PRINT,
							 pPrintGroupBx);
   }
 }  // TPrintAbcBookDialog :: TPrintAbcBookDialog


void TPrintAbcBookDialog :: SetupWindow ()
 {
// 200806M  TPrintDescBookDialog :: SetupWindow () ;
// 200806M -->
  TScaleDialog :: SetupWindow () ;
  pBN -> SetL (1) ;
  pEN -> SetL (0) ;
  pY -> SetL (Today.year) ;
  pType -> SetSelIndex (0) ;
  pJan2005Format->SetCheck(BF_CHECKED);  // 10.feb.2005 - end
// 200806M <--

  pBL -> SetS ("À") ;
  pEL -> SetS ("ß") ;
  // 30.sep.2003 - begin
  if (WINDOWS_PRINT)
  {
    pWantUnbrokenPrint -> Check () ;
    pWantCheckup -> SetCheck (BF_UNCHECKED) ;
  }
  else  // 30.sep.2003 - end
  {
    pWantBrokenPrint -> Check () ; 
    pWantCheckup -> SetCheck (BF_CHECKED) ;
  }
 }  // TPrintAbcBookDialog :: SetupWindow

void TPrintAbcBookDialog :: OKBNClicked ()
 {
  pBegNLEndNLYear -> begN = pBN -> L () ;
  pBegNLEndNLYear -> endN = pEN -> L () ;
  pBegNLEndNLYear -> year = pY -> L () ;

  pBegNLEndNLYear -> begL = (pBL -> S ()) [0] ;
  pBegNLEndNLYear -> endL = (pEL -> S ()) [0] ;

  pBegNLEndNLYear -> wantCheckup =
    (pWantCheckup -> GetCheck () == BF_CHECKED) ;

  pBegNLEndNLYear -> jan2005Format =
    (pJan2005Format-> GetCheck() == BF_CHECKED);

  pBegNLEndNLYear -> wantBrokenPrint =
    (pWantBrokenPrint -> GetCheck () == BF_CHECKED) ;
  pBegNLEndNLYear -> wantPrintByPages =
    (pWantPrintByPages -> GetCheck () == BF_CHECKED) ;


// 200806M  TPrintDescBookDialog :: OKBNClicked () ;
// 200806M   ->
  if (pSelCollege)
   {
    * pSelCollege = pType -> C () ;

      CmOk () ;
   }
  else
    CmCancel () ;
// 200806   <-
//  CmOk () ;
 }  // TPrintAbcBookDialog :: OKBNClicked
