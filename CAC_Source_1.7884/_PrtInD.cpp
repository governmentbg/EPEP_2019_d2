DEFINE_RESPONSE_TABLE1(TPrintInputDiaryDialog, TScaleDialog)
//{{TPrintInputDiaryDialogRSP_TBL_BEGIN}}
  EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TPrintInputDiaryDialogRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{TPrintInputDiaryDialog Implementation}}


TPrintInputDiaryDialog :: TPrintInputDiaryDialog (TWindow * parent,
						   begNEndNYearTypes & rBNENYT,
								  int resId) :
				    TScaleDialog (parent, resId),
                            bNENYT (rBNENYT), pTypes (NULL), pBN (NULL),
                            pEN (NULL), pY (NULL)
{
  // INSERT>> Your constructor code here.

  pTypes = new TTypeListFace (this, IDC_INP_DIA_LAW_TYPE,
                              "TYPES", Default->xColleges, false) ;
  pBN = new TLongFace (this, IDC_INP_DIA_BEG_NO, "F_NO_BEGIN",
                       SIZE_OF_NO) ;
  pEN = new TLongFace (this, IDC_INP_DIA_END_NO, "F_NO_END", SIZE_OF_NO) ;
  pY = new TLongFace (this, IDC_INP_DIA_YEAR, "F_YEAR", SIZE_OF_YEAR) ;
}


void TPrintInputDiaryDialog::OKBNClicked()
{
  // INSERT>> Your code here.

  if (! strlen (pTypes -> S ()))
   {
    error(NO_TYPE_CHOOSEN) ;
    return ;
   }
  scSprintf (sizeof (bNENYT.types), bNENYT.types,
             "%s", pTypes -> S ()) ;
  if (strlen (bNENYT.types) < strlen (pTypes -> S ()))
    error(TYPES_BUFF_TOO_SHORT) ;  // This
	// normally will never be displayed
  bNENYT.begN = pBN -> L () ;
  bNENYT.endN = pEN -> L () ;
  bNENYT.year = pY -> L () ;
  CmOk () ;
}


void TPrintInputDiaryDialog::SetupWindow()
{
  TScaleDialog::SetupWindow();

  // INSERT>> Your code here.

  pBN -> SetL (1) ;
  pEN -> SetL (0) ;
  pY -> SetL (Today.year) ;
}
