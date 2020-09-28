DEFINE_RESPONSE_TABLE1(TCalendarDialog, TEditDialog)
//{{TCalendarDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_CALENDAR_MONTH, MonthCBNSelChange),
//{{TCalendarDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TCalendarDialog Implementation}}

TCalendarDialog::TCalendarDialog(TWindow* parent, TCalendar *calendar)
:
	TEditDialog(parent, calendar, true, IDD_CALENDAR)
{
	year = new TYearFace(this, IDC_CALENDAR_YEAR, "F_YEAR");
	month = new TLongAliasFace(this, IDC_CALENDAR_MONTH, "F_MONTH", ::Month, NULL, FLAG_NULL);
	new TColorStatic(this, IDC_CALENDAR_LABEL_6, "Съб", COLOR_LIGHT_RED);
	new TColorStatic(this, IDC_CALENDAR_LABEL_7, "Нед", COLOR_LIGHT_RED);

	for (int i = 0; i < CALENDAR_DAYS_MAX; i++)
		buttons[i] = new TColorButton(this, IDC_CALENDAR_BUTTON_1 + i, COLOR_BUTTON_TEXT);
}

void TCalendarDialog::Mark(TColorButton *button, bool work)
{
	button->SetColor(work ? COLOR_BUTTON_TEXT : COLOR_LIGHT_RED);
}

void TCalendarDialog::SetupWindow()
{
	TEditDialog::SetupWindow();
	MonthCBNSelChange();
}

void TCalendarDialog::MonthCBNSelChange()
{
	int count = CDate::DaysInMonth(Year(), Month());

	offset = CDate(1, Month(), Year()).DayOfWeek() - 1;

	for (int i = 0; i < CALENDAR_DAYS_MAX; i++)
	{
		TColorButton *button = buttons[i];

		if (i < offset || i >= offset + count)
			button->ShowWindow(SW_HIDE);
		else
		{
			mstr m;
			int day = i - offset + 1;

			itom(day, m);
			button->SetCaption(str(m));
			Mark(button, Calendar()->Work(year->L(), Month(), day));
			button->ShowWindow(SW_SHOW);
		}
	}
}

TResult TCalendarDialog::EvCommand(uint id, THandle hWndCtl, uint notifyCode)
{
	if (notifyCode == BN_CLICKED && id >= IDC_CALENDAR_BUTTON_1 && id <= IDC_CALENDAR_BUTTON_37)
	{
		int day = id - IDC_CALENDAR_BUTTON_1 - offset + 1;
		bool work = !Calendar()->Work(year->L(), Month(), day);
		TColorButton *button = dynamic_cast<TColorButton *> (FirstThat(MatchesId, (void *) id));

		Calendar()->SetWork(Month(), day, work);
		Mark(button, work);
	}

	return TEditDialog::EvCommand(id, hWndCtl, notifyCode);
}

TResult TCalendarDialog::EvNotify(uint id, TNotify far& info)
{
	if (window_handle_color(info, IDC_CALENDAR_BUTTON_1, IDC_CALENDAR_BUTTON_37))
		return CDRF_NOTIFYPOSTPAINT;

	return TEditDialog::EvNotify(id, info);
}
