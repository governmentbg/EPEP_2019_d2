DEFINE_RESPONSE_TABLE1(TPrintDescBookDialog, TGroupDialog)
  EV_BN_CLICKED(IDOK, OKBNClicked),
  EV_CHILD_NOTIFY_ALL_CODES(IDC_GR_BOX_DBK, HandleGroupBoxMsg),
END_RESPONSE_TABLE;

TPrintDescBookDialog :: TPrintDescBookDialog (TWindow * parent,
                              begNEndNYear * aBegNEndNYearPtr,
                              char * selCollegePtr,
                              int resId) :
				  TGroupDialog (parent, NULL, resId),
                          pBegNEndNYear (aBegNEndNYearPtr),
                          pSelCollege (selCollegePtr)


 {
  // INSERT>> Your constructor code here.
  pType = new TTypeAliasFace (this, IDC_PRINT_DB_COLLEGE, "F_TYPE",
                              true, NULL) ;
  // 20080605
  TGroupBox * pSelectCrit = new TGroupBox (this, IDC_GR_BOX_DBK) ;
  rb_SelectCrit.SetGrpBox (pSelectCrit) ;
  rb_SelectCrit.Add(new TRadioButton (this, IDC_RB_DB_BY_NO, pSelectCrit),
                                      IDC_RB_DB_BY_NO) ;
  rb_SelectCrit.Add(new TRadioButton (this, IDC_RB_DB_BY_DATE, pSelectCrit),
                                      IDC_RB_DB_BY_DATE) ;
  // 20080605
  pBN = new TLongFace (this, IDC_PRINT_DB_NO_BEGIN, "F_NO_BEGIN",
			     SIZE_OF_NO) ;
  pEN = new TLongFace (this, IDC_PRINT_DB_NO_END, "F_NO_END", SIZE_OF_NO) ;
  pY = new TLongFace (this, IDC_PRINT_DB_YEAR, "F_YEAR", SIZE_OF_YEAR) ;
  // 20080605
  pBegD = new TDateFace (this, IDC_E_DB_FDATE, "F_DATE_BEGIN",
				 true) ;
  pEndD = new TDateFace (this, IDC_E_DB_LDATE, "F_DATE_END",
				 true) ;
  // 20080605
  // 08.feb.2005 - begin
  pJan2005Format = new TCheckBox (this, IDC_PRINT_DB_NEW_FORMAT, NULL) ;
  pSplitIn2 = new TCheckBox (this, IDC_PRINT_DB_SPLIT_IN_2, NULL) ;
  // 08.feb.2005 - end
 }  // TPrintDescBookDialog :: TPrintDescBookDialog


void TPrintDescBookDialog::HandleGroupBoxMsg (uint)
{
    if (rb_SelectCrit.GetSelectionIdc() == IDC_RB_DB_BY_NO)
    {
      pBN->EnableWindow (true);
	pEN->EnableWindow (true);
      pY->EnableWindow (true);
      pBegD->EnableWindow (false);
      pEndD->EnableWindow (false);
    }
    else
    {
      pBN->EnableWindow (false);
      pEN->EnableWindow (false);
      pY->EnableWindow (false);
      pBegD->EnableWindow (true);
      pEndD->EnableWindow (true);
    }
}  // TPrintDescBookDialog::HandleGroupBoxMsg

bool TPrintDescBookDialog :: isValid ()
{

  bool rT = true ;
  CDate bD = pBegD -> Date () ;
  CDate eD = pEndD -> Date () ;


  if (rT)
  {  if ( (pBN->L()>0.00) && (pEN->L()>0.00) )
        if (pBN->L()>pEN->L())
           rT = incorrect_period_given (this, IDC_PRINT_DB_NO_BEGIN) ;
  }
  if (rT)
  {
     if ( (pBegD -> Date ().Empty() == false)&&(pEndD -> Date ().Empty() == false) )
        if (  (bD > eD) || (bD.year != eD.year)  )
           rT = incorrect_period_given (this, IDC_E_DB_FDATE) ;
  }

  return rT ;

}  // TPrintDescBookDialog :: isValid


void TPrintDescBookDialog :: SetupWindow ()
 {
  TGroupDialog :: SetupWindow () ;
  pBN -> SetL (1) ;
  pEN -> SetL (0) ;
  pY -> SetL (Today.year) ;
  pType -> SetSelIndex (0) ;

  // 20080605 ->
  rb_SelectCrit.SetSelectionIdc (IDC_RB_DB_BY_NO) ;

  pBegD->SetDate (Today) ;
  pEndD->SetDate (Today) ;
  // 20080605 <-

  if(default_split_wide_hyper_docs())  // 10.feb.2005 - begin
  {
    pSplitIn2->SetCheck(BF_CHECKED);
  }
  else
  {
    if (WINDOWS_PRINT == 0)
    {
      pSplitIn2->EnableWindow(false);
    }
  }
  pJan2005Format->SetCheck(BF_CHECKED);  // 10.feb.2005 - end

  pSplitIn2->Show(SW_HIDE); //20050314

 }  // TPrintDescBookDialog :: SetupWindow

void TPrintDescBookDialog :: OKBNClicked ()
 {
  if (isValid () == false)
       return ;

  if (pSelCollege)
   {
    * pSelCollege = pType -> C () ;
    // 10.feb.2005 - begin
    pBegNEndNYear->jan2005Format = (pJan2005Format->GetCheck() == BF_CHECKED);
    pBegNEndNYear->splitIn2 = (pSplitIn2->GetCheck() == BF_CHECKED);
    // 10.feb.2005 - end

    pBegNEndNYear -> selectCrit1 =
      bool (rb_SelectCrit.GetSelectionIdc () == IDC_RB_DB_BY_NO );

    if ( pBegNEndNYear -> selectCrit1 )
    {
       pBegNEndNYear -> begN = pBN -> L () ;
       pBegNEndNYear -> endN = pEN -> L () ;
       pBegNEndNYear -> year = pY -> L () ;
    }
    else
    {
       pBegNEndNYear -> begDate = pBegD -> Date () ;
       pBegNEndNYear -> endDate = pEndD -> Date () ;
    }
    CmOk () ;
   }
  else
    CmCancel () ;
 }  // TPrintDescBookDialog :: OKBNClicked

//

