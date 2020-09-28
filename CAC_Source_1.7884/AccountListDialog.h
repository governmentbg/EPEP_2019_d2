//{{TDialog = TAccountListDialog}}
class TAccountListDialog : public TGroupListDialog
{
public:
	TAccountListDialog(TWindow* parent);

protected:
	TCharAliasFace *ucnType;
	TAccountListWindow *accounts;

//{{TAccountListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TAccountListDialogVIRTUAL_END}}

//{{TAccountListDialogRSP_TBL_BEGIN}}
	void UCNTypeCBNSelChange();
	void AddBNEnable(TCommandEnabler &tce);
	void DeleteBNEnable(TCommandEnabler &tce);
	void PrintBNClicked();
//{{TAccountListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TAccountListDialog);
};    //{{TAccountListDialog}}
