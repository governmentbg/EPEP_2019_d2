class TNFBPuniAdm2ndDialog : public TScaleDialog
 {
  protected :
    TLongFace * pNFBPA2 ;
    long int & nFBPA2cnt ;
  public:
    TNFBPuniAdm2ndDialog (long int & cntNFBPA2,
				  TWindow * parent);

//{{TNFBPuniAdm2ndDialogVIRTUAL_BEGIN}}
  public:
    virtual void SetupWindow();
//{{TNFBPuniAdm2ndDialogVIRTUAL_END}}

//{{TNFBPuniAdm2ndDialogRSP_TBL_BEGIN}}
  protected:
    void OKBNClicked();
//{{TNFBPuniAdm2ndDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TNFBPuniAdm2ndDialog);
 };    //{{TNFBPuniAdm2ndDialog}}
