//{{TDialog = TCalendarDialog}}
class TCalendarDialog : public TEditDialog
{
public:
	TCalendarDialog(TWindow* parent, TCalendar *calendar);

protected:
	int offset;

	TYearFace *year;
	TLongAliasFace *month;
	TColorButton *buttons[CALENDAR_DAYS_MAX];

	DEFINE_GROUP_TYPE(TCalendar, Calendar)
	int Year() { return year->L(); }
	int Month() { return month->L(); }
	void Mark(TColorButton *button, bool work);

//{{TCalendarDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
	virtual TResult EvCommand(uint id, THandle hWndCtl, uint notifyCode);
	virtual TResult EvNotify(uint id, TNotify far& info);
//{{TCalendarDialogVIRTUAL_END}}

//{{TCalendarDialogRSP_TBL_BEGIN}}
	void MonthCBNSelChange();
//{{TCalendarDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TCalendarDialog);
};    //{{TCalendarDialog}}
