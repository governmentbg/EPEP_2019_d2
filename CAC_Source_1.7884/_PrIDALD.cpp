DEFINE_RESPONSE_TABLE1 (TPrintInputDiaryApplLawsDialog,
                        TPrintInputDiaryDialog)
//{{TPrintInputDiaryApplLawsDialogRSP_TBL_BEGIN}}
  EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TPrintInputDiaryApplLawsDialogRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{TPrintInputDiaryApplLawsDialog Implementation}}


TPrintInputDiaryApplLawsDialog :: TPrintInputDiaryApplLawsDialog (
					    TWindow * parent,
					    begNEndNYearTypesAC & rBNENYT,
					    int resId) :
     TPrintInputDiaryDialog (parent, rBNENYT, resId),
     flyingDocsIndex (-1), rBNENYTAC (rBNENYT)
     // , pWantAllColumns (NULL)  // 27.sep.2000
 {
 }  // TPrintInputDiaryApplLawsDialog::TPrintInputDiaryApplLawsDialog


void TPrintInputDiaryApplLawsDialog :: SetupWindow ()
 {
  TPrintInputDiaryDialog :: SetupWindow () ;

  // INSERT>> Your code here.

  SetCaption ("Входящ регистър") ; 
 }  // TPrintInputDiaryApplLawsDialog :: SetupWindow

void TPrintInputDiaryApplLawsDialog :: OKBNClicked ()
 {
  // INSERT>> Your code here.

  if (pTypes -> GetSelCount () > 0)
   {  // -- 1 --  // If this check is OK,
      // I will delete from (* pTypes)
      // the string at flyingDocsIndex (see below)
    const bool flyingDocsSelected =
      flyingDocsIndex < 0 ? false :  // 27.sep.2000
                            pTypes -> GetSel (flyingDocsIndex) ;

    if (flyingDocsIndex >= 0)  // 27.sep.2000
	pTypes -> DeleteString (flyingDocsIndex) ;
    // Do not use flyingDocsIndex more !
    scSprintf (sizeof (rBNENYTAC.types), rBNENYTAC.types,
               "%s%s", pTypes -> S (),
               flyingDocsSelected ? TYPE_INREGS : "") ;
    if (strlen (rBNENYTAC.types) <
        (strlen (pTypes -> S ()) +
         flyingDocsSelected ? strlen (TYPE_INREGS) : 0)
       )
	error(TYPES_BUFF_TOO_SHORT) ;  // This
	// normally will never be displayed
    rBNENYTAC.begN = pBN -> L () ;
    rBNENYTAC.endN = pEN -> L () ;
    rBNENYTAC.year = pY -> L () ;
//      rBNENYTAC.allColumns =
//        pWantAllColumns -> GetCheck () ==
//          BF_CHECKED ;  // 05.sep.2000, 27.sep.2000 commented
    CmOk () ;
   }  // -- 1 --
  else
   {
    error(NO_TYPE_CHOOSEN) ;
    return ;
   }
 }  // TPrintInputDiaryApplLawsDialog :: OKBNClicked
