class T2RadiosDialog : public TScaleDialog
{
	protected:
	RadioCluster cluster;
    int& choice;
    const int dfltChoice;
    const char* const caption;
    const char* const groupText;
    const char* const radio1Text;
    const char* const radio2Text;

    void OKBNClicked();
	public:
  	T2RadiosDialog(TWindow* parent, int& choosen, const char* const title,
    	const char* const groupTitle, const char* const radio1Txt,
	    const char* const radio2Txt, const int dflt = 0,
      int resId = IDD_TWO_RADIOS);
    virtual void SetupWindow();
  DECLARE_RESPONSE_TABLE(T2RadiosDialog);
};  // class T2RadiosDialog

class T3RadiosDialog: public T2RadiosDialog
{
	public:  // Construction / Destruction
	T3RadiosDialog(TWindow* parent, int& choosen, const char* const title,
	const char* const groupTitle, const char* const radio1Txt,
	    const char* const radio2Txt, const char* const radio3Txt,
	const int dflt = 0, int resId = IDD_THREE_RADIOS);
	public:  // Methods
    virtual void SetupWindow();
	protected:  // Data
    const char* const radio3Text;
};  // class T3RadiosDialog
