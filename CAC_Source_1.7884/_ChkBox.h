class T2ChkBoxesDialog : public TScaleDialog
 {
  protected :
    TCheckBox * pChkBx1 ;
    TCheckBox * pChkBx2 ;
    bool & cb1Checked ;
    bool & cb2Checked ;
    const char * const capt ;
    const char * const txtGrp ;  // 27.aug.2002
    const char * const txt1 ;
    const char * const txt2 ;
    void OKBNClicked () ;
  public :
    T2ChkBoxesDialog (TWindow * parent, const char * const title,
                      const char * const nameGrp,  // 27.aug.2002          
                      const char * const name1, const char * const name2,
                      bool & firstChecked, bool & secondChecked,
                      int resId = IDD_2_CHK_BOXES) ;
    virtual void SetupWindow () ;
  DECLARE_RESPONSE_TABLE (T2ChkBoxesDialog) ;
 } ;  // class T2ChkBoxesDialog
