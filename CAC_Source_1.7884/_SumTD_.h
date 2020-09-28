class TBaseTextDialog : public TScaleDialog  // 26.sep.2003
 {
  protected :
    char * const textReceiver ;
    const size_t textReceiverSz ;
    TCatchPasteEdit * pSummTxt ;
    const bool noTextBnToBreak ;
    char spare [2] ;
    const int columnWidth ;
    const int columnRows ;
    virtual void OKBNClicked () ;
    virtual bool DataOK () ;
    HBRUSH EvCtlColor(HDC hDC, HWND hWndChild, uint ctlType);
  public :
    TBaseTextDialog (TWindow * parent, char * const txtRcv,
                     size_t txtRcvSz,
                     const bool noTxtBnToBreak = false,
                     int clmnWidth = 0, int clmnRows = 0, // 27.aug.2003
                     int resId = IDD_SUMMONS_TEXT) ;
    virtual void SetupWindow () ;
  DECLARE_RESPONSE_TABLE (TBaseTextDialog) ;
 } ;  // class TBaseTextDialog

class TAnyLetterTextDialog : public TBaseTextDialog
 {
  protected :
    const char * const title ;
  public :
    TAnyLetterTextDialog (TWindow * parent, char * const txtRcv,
                          size_t txtRcvSz,
                          const char * const capt = NULL,
                          const bool noTxtBnToBreak = false,
                          int clmnWidth = 0, int clmnRows = 0, // 27.aug.2003
				  int resId = IDD_SUMMONS_TEXT) ;
    virtual void SetupWindow () ;
 } ;  // class TAnyLetterTextDialog

class TAnyShortTextDialog : public TAnyLetterTextDialog
 {
  public :
    TAnyShortTextDialog (TWindow * parent, char * const txtRcv,
				 size_t txtRcvSz,
				 const char * const capt = NULL,
				 const bool noTxtBnToBreak = false,
				 int resId = IDD_SHORT_TEXT) ;
 } ;  // class TAnyLetterTextDialog
