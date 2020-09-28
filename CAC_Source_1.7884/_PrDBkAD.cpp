# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
DEFINE_RESPONSE_TABLE1(TPrintDescBookArealDialog, TPrintDescBookDialog)
  EV_BN_CLICKED(IDOK, OKBNClicked),
END_RESPONSE_TABLE;

TPrintDescBookArealDialog :: TPrintDescBookArealDialog (TWindow * parent,
                              begNEndNYear * aBegNEndNYearPtr,
                              char * selCollegePtr,
                              int resId) :
				  TPrintDescBookDialog (parent, aBegNEndNYearPtr,
								selCollegePtr, resId)
 {
  pPseudoLawsByInst = new TGroupBox (this,
						 IDC_PRINT_DB_LAWS_BY_INST_PSEUDO) ;
  pWant1stInstLaws = new TCheckBox (this, IDC_PRINT_DB_WANT_1ST_LAWS, NULL) ;
  pWant2ndInstLaws = new TCheckBox (this, IDC_PRINT_DB_WANT_2ND_LAWS, NULL) ;
 }  // TPrintDescBookArealDialog :: TPrintDescBookArealDialog


void TPrintDescBookArealDialog :: SetupWindow ()
 {
  TPrintDescBookDialog :: SetupWindow () ;
  pWant1stInstLaws -> SetCheck (BF_CHECKED) ;
  pWant2ndInstLaws -> SetCheck (BF_CHECKED) ;
  if (!AREA_SPLIT_DESC_BOOK)
   {
    pWant1stInstLaws -> EnableWindow (false) ;
    pWant2ndInstLaws -> EnableWindow (false) ;
    pPseudoLawsByInst -> EnableWindow (false) ;
   }
 }  // TPrintDescBookArealDialog :: SetupWindow

void TPrintDescBookArealDialog :: OKBNClicked ()
 {
  if (! pBegNEndNYear)
     CmCancel () ;
  if (pWant1stInstLaws -> GetCheck () == BF_UNCHECKED &&
      pWant2ndInstLaws -> GetCheck () == BF_UNCHECKED)
   {
    error (PLEASE_CHECK_ANY_INST) ;
      // message --> error  30.mar.2001
    return ;
   }
  if (pWant1stInstLaws -> GetCheck () == BF_UNCHECKED &&
      pWant2ndInstLaws -> GetCheck () == BF_CHECKED &&
      pType -> C () == TYPE_FIRM_LAW)
   {
    error (FIRM_LAWS_ARE_1ST_INSTANCE) ;
    return ;
   }
  // 22.feb.2005 - begin
  {
    const bool want1 = (pWant1stInstLaws -> GetCheck () == BF_CHECKED);
    const bool want2 = (pWant2ndInstLaws -> GetCheck () == BF_CHECKED);

    switch(pType->C())
    {
#if COURT_TYPE != COURT_MILITARY
	case TYPE_ADMIN_LAW:
	  break;
#endif  // !MILITARY	  
	case TYPE_FIRM_LAW:
	  if(want2)
	  {
	    error("Фирмените дела са само I инстанция.");
	    return;
	  }
	  break;
#if COURT_TYPE == COURT_ADMIN
	case TYPE_CITIZEN_LAW:
	  if(want2)
	  {
	    error("В Административен съд гражданските дела са само първоинстанционни.");
	    return;
	  }
	  break;
	case TYPE_PUNISHMENT_LAW:
	case TYPE_TRICKY_LAW:
	// 2008:320 LRQ: not any more
	//if(want1)
	//{
	//  error("В Административен съд наказателните дела са само касационни.");
	//  return;
	//}
	  break;
#endif
	default:
	  if(want1 && want2)
	  {
          if (pJan2005Format -> GetCheck () == BF_CHECKED)   //20050607
          {   error("Моля, изберете само I или II инстанция.");
              return;
          }
        }
        break;
    }
  }
  // 22.feb.2005 - end
  pBegNEndNYear -> want1stInstLaws =
    pWant1stInstLaws -> GetCheck () == BF_CHECKED ;
  pBegNEndNYear -> want2ndInstLaws =
    pWant2ndInstLaws -> GetCheck () == BF_CHECKED ;
  TPrintDescBookDialog :: OKBNClicked () ;
 }  // TPrintDescBookArealDialog :: OKBNClicked

#   endif  // of APPEAL
# endif  // of INSTANCE
//

