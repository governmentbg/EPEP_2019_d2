//{{TDialog = TConfigNameListDialog}}
class TConfigNameListDialog : public TGroupListDialog
{
public:
	TConfigNameListDialog(TWindow* parent, int resId = IDD_CONFIG_NAME_LIST);

protected:
	TConfigNameListWindow *configs;

	void LocalEnable(TCommandEnabler &tce);

//{{TConfigNameListDialogRSP_TBL_BEGIN}}
	void EditBNClicked();
	void CopyBNClicked();
	void RenameBNClicked();
//{{TConfigNameListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TConfigNameListDialog);
};    //{{TConfigNameListDialog}}
