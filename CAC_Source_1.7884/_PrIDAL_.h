class TPrintInputDiaryApplLawsDialog : public TPrintInputDiaryDialog
 {
  protected :
    int flyingDocsIndex ;
//    TCheckBox * pWantAllColumns ; //05.sep.2000, 27.sep.2000 commented
    begNEndNYearTypesAC & rBNENYTAC ;  // 05.sep.2000
  public :
    TPrintInputDiaryApplLawsDialog (TWindow * parent,
                                    begNEndNYearTypesAC & rBNENYT,
                                    int resId =
                 IDD_INPUT_DIARY  // IDD_APL_LAWS_INPUT_DIARY
                                   // 05.sep.2000, 27.sep.2000 back again
                                    ) ;
//{{TPrintInputDiaryApplLawsDialogVIRTUAL_BEGIN}}
  public :
    virtual void SetupWindow () ;
//{{TPrintInputDiaryApplLawsDialogVIRTUAL_END}}

//{{TPrintInputDiaryApplLawsDialogRSP_TBL_BEGIN}}
  protected :
    void OKBNClicked () ;
//{{TPrintInputDiaryApplLawsDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TPrintInputDiaryApplLawsDialog);
};    //{{TPrintInputDiaryApplLawsDialog}}
