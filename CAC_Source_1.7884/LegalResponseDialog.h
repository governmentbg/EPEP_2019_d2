//{{TDialog = TLegalResponseDialog}}
class TLegalResponseDialog : public TQuickDialog
{
public:
	TLegalResponseDialog(TWindow* parent, TGArray<TLegalResponse> *tResponses, int resId = IDD_LEGAL_RESPONSE);

protected:
	TLegalResponseListWindow *responses;
	TEdit *text;

	void UpdateText();

//{{TLegalResponseDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TLegalResponseDialogVIRTUAL_END}}

//{{TLegalResponseDialogRSP_TBL_BEGIN}}
	void LoadBNClicked();
	void LoadBNEnable(TCommandEnabler &tce);
	void ViewBNClicked();
	void ViewBNEnable(TCommandEnabler &tce);
	bool ResponseLVNItemChanging(TLwNotify &lwn);
     	void ResponseLVNItemChanged(TLwNotify &);
//{{TLegalResponseDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TLegalResponseDialog);
};  //{{TLegalResponseDialog}}