DEFINE_RESPONSE_TABLE1 (T2ChkBoxesDialog, TScaleDialog)
  EV_BN_CLICKED (IDOK, OKBNClicked),
END_RESPONSE_TABLE ;

T2ChkBoxesDialog :: T2ChkBoxesDialog (TWindow * parent,
                                      const char * const title,
                                      const char * const nameGrp,
                                      const char * const name1,
                                      const char * const name2,
                                      bool & firstChecked,
                                      bool & secondChecked,
                                      int resId) :
                      TScaleDialog (parent, resId),
                      pChkBx1 (NULL), pChkBx2 (NULL),
                      cb1Checked (firstChecked),
                      cb2Checked (secondChecked),
                      capt (title), txtGrp (nameGrp),
                      txt1 (name1), txt2 (name2)
 {
//  TGroupBox * pGrp = new TGroupBox (this, IDC_2CB_GRP) ;

  pChkBx1 = new TCheckBox (this, IDC_2CB_1, NULL) ;
  pChkBx2 = new TCheckBox (this, IDC_2CB_2, NULL) ;
 }  // T2ChkBoxesDialog :: T2ChkBoxesDialog

void T2ChkBoxesDialog :: SetupWindow ()
 {
  TScaleDialog :: SetupWindow () ;
  if (capt)
    SetCaption (capt) ;
  if (txtGrp)  // 27.aug.2002
    SetDlgItemText (IDC_2CB_GRP, txtGrp) ;  // 27.aug.2002
  if (txt1)  // 15.feb.2002
    SetDlgItemText (IDC_2CB_1, txt1) ;
  if (txt2)  // 15.feb.2002
    SetDlgItemText (IDC_2CB_2, txt2) ;
  pChkBx1 -> SetCheck (cb1Checked ? BF_CHECKED : BF_UNCHECKED) ;  // BF_CHECKED  // 15.feb.2002
  pChkBx2 -> SetCheck (cb2Checked ? BF_CHECKED : BF_UNCHECKED) ;  // BF_CHECKED  // 15.feb.2002
 }  // T2ChkBoxesDialog :: SetupWindow

void T2ChkBoxesDialog :: OKBNClicked ()
 {
  cb1Checked = pChkBx1 -> GetCheck () == BF_CHECKED ;
  cb2Checked = pChkBx2 -> GetCheck () == BF_CHECKED ;
  CmOk () ;
 }  // T2ChkBoxesDialog :: OKBNClicked
