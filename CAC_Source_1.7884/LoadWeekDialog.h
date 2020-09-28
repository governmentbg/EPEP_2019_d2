//{{TDialog = TLoadWeekDialog}}
class TLoadWeekDialog : public TLoadHallDialog
{
public:
	TLoadWeekDialog(TWindow* parent, TLoadWeek *loadWeek, int resId = IDD_LOAD_WEEK);

protected:
	TCharAliasFace *weekday;
	TLoadWeek days[7];
	bool edits[7];
	char day;

	DEFINE_GROUP_TYPE(TLoadWeek, LoadWeek)

//{{TJudicialActDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TJudicialActDialogVIRTUAL_END}}

//{{TLoadWeekDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void WeekdayCBNSelChange();
//{{TLoadWeekDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TLoadWeekDialog);
};    //{{TLoadWeekDialog}}
