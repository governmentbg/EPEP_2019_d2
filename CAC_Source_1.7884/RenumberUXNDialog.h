//{{TDialog = TRenumberUXNDialog}}
class TRenumberUXNDialog : public TAlterUXNDialog
{
public:
	TRenumberUXNDialog(TWindow* parent, TUCNGroup *tSource, TUCNGroup *tTarget, int resId = IDD_RENUMBER_UXN);

protected:
//{{TRenumberUXNDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TRenumberUXNDialogVIRTUAL_END}}

//{{TRenumberUXNDialogRSP_TBL_BEGIN}}
	void SourceTypeCBNSelChange();
//{{TRenumberUXNDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TRenumberUXNDialog);
};    //{{TRenumberUXNDialog}}
