class TPrintInputDiaryDialog : public TScaleDialog
 {
  protected :
    begNEndNYearTypes & const bNENYT ;
    TTypeListFace * pTypes ;
    TLongFace * pBN, * pEN, * pY ;
  public:
    TPrintInputDiaryDialog (TWindow * parent,
                            begNEndNYearTypes & rBNENYT,
				    int resId = IDD_INPUT_DIARY) ;

//{{TPrintInputDiaryDialogVIRTUAL_BEGIN}}
  public:
    virtual void SetupWindow();
//{{TPrintInputDiaryDialogVIRTUAL_END}}

//{{TPrintInputDiaryDialogRSP_TBL_BEGIN}}
  protected:
    void OKBNClicked();
//{{TPrintInputDiaryDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TPrintInputDiaryDialog);
 } ;    //{{TPrintInputDiaryDialog}}
