//{{TDialog = TSelectDialog}}
class TSelectDialog : public TGroupDialog
{
public:
	TSelectDialog(TWindow* parent, TGroup *group, int resId);

protected:
	TGroupListBox *glist;

//{{TSelectDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TSelectDialogVIRTUAL_END}}

//{{TSelectDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void OKBNEnable(TCommandEnabler &tce);
//{{TSelectDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSelectDialog);
};    //{{TSelectDialog}}
