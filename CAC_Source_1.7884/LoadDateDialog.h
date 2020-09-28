//{{TDialog = TLoadDateDialog}}
class TLoadDateDialog : public TLoadHallDialog
{
public:
	TLoadDateDialog(TWindow* parent, TLoadDate *loadDate, bool tEdit, int resId = IDD_LOAD_DATE);

protected:
	bool edit;

	DEFINE_GROUP_TYPE(TLoadDate, LoadDate)

//{{TLoadDateDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TLoadDateDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TLoadDateDialog);
};    //{{TLoadDateDialog}}
