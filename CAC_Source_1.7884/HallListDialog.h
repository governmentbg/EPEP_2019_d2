//{{TDialog = THallListDialog}}
class THallListDialog : public TGroupListDialog
{
public:
	THallListDialog(TWindow* parent, int resId = IDD_HALL_LIST);

protected:
	THallListWindow *halls;

//{{THallListDialogRSP_TBL_BEGIN}}
	void LoadWeekBNClicked();
	void LoadDateBNClicked();
	void PrintBNClicked();
//{{THallListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(THallListDialog);
};    //{{THallListDialog}}
