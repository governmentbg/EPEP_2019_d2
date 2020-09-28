DEFINE_RESPONSE_TABLE1(TLoadWeekDialog, TLoadHallDialog)
//{{TLoadWeekDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_CBN_SELCHANGE(IDC_LOAD_WEEK_WEEKDAY, WeekdayCBNSelChange),
//{{TLoadWeekDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TLoadWeekDialog Implementation}}

TLoadWeekDialog::TLoadWeekDialog(TWindow* parent, TLoadWeek *loadWeek, int resId)
:
	TLoadHallDialog(parent, loadWeek, resId)
{
	weekday = new TCharAliasFace(this, IDC_LOAD_WEEK_WEEKDAY, "F_WEEKDAY", Weekday, NULL);
}

void TLoadWeekDialog::OKBNClicked()
{
	*LoadWeek() << this;
	days[day - '1'] << LoadWeek();

	bool okay = true;

	for (int i = 0; i < 7; i++)
		if (!days[i].Save(edits[i]))
			okay = false;

	if (okay)
		CmOk();
}

void TLoadWeekDialog::SetupWindow()
{
	for (int i = 0; i < 7; i++)
	{
		days[i] << LoadWeek();
		days[i].weekday = (char) (i + '1');
		edits[i] = days[i].Try();
	}

	*LoadWeek() << days[0];
	day = '1';

	TLoadHallDialog::SetupWindow();
}

void TLoadWeekDialog::WeekdayCBNSelChange()
{
	if (weekday->C() != day)
	{
		*LoadWeek() << this;
		days[day - '1'] << LoadWeek();
		days[day - '1'].weekday = day;

		day = weekday->C();
		*LoadWeek() << days[day - '1'];
		*LoadWeek() >> this;

		BefReservedBNClicked();
		AftReservedBNClicked();
	}
}
