//{{TDialog = TDangerListDialog}}
class TDangerListDialog : public TGroupListDialog
{
public:
	TDangerListDialog(TWindow* parent, int resId = IDD_DANGER_LIST);

protected:
	TDangerListWindow *dangers;

//{{TDangerListDialogRSP_TBL_BEGIN}}
	void SubjectBNClicked();
	void SubjectBNEnable(TCommandEnabler &tce);
	void PrintBNClicked();
//{{TDangerListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TDangerListDialog);
};    //{{TDangerListDialog}}
