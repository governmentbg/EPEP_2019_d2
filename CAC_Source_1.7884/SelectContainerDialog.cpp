DEFINE_RESPONSE_TABLE1(TSelectContainerDialog, TSelectEventDialog)
//{{TSelectContainerDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
	EV_BN_CLICKED(IDQUERY, QueryBNClicked),
	EV_COMMAND_ENABLE(IDQUERY, SelectEnable),
//{{TSelectContainerDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSelectContainerDialog Implementation}}

TSelectContainerDialog::TSelectContainerDialog(TWindow* parent, const char *label, TCollectEvents *e, int &choice,
	TGArray<TRCDKeyContainer> *basics, TGArray<TBaseWind> *tSideWinds, int resId)
:
	TSelectEventDialog(parent, label, e, choice, basics, resId), sideWinds(tSideWinds)
{
	query = basics ? new TPushButton(this, IDQUERY) : NULL;
}

bool TSelectContainerDialog::IsValid()
{
	bool isValid = TSelectEventDialog::IsValid();

	if (isValid)
	{
		if (array)
		{
			TString *extraText = Container()->FindExtraText();
			isValid = !extraText || ask(extraText->S());
		}
	}

	return isValid;
}

void TSelectContainerDialog::SetupWindow()
{
	TSelectEventDialog::SetupWindow();

	if (query)
	{
		query->ShowWindow(can_query() ? SW_SHOW : SW_HIDE);
		query->EnableWindow(can_query());
	}
}

void TSelectContainerDialog::PrintBNClicked()
{
	print_ucn_groups(sideWinds, str(e->stats), "PrintSide.htm", NULL);
}

void TSelectContainerDialog::QueryBNClicked()
{
	query_group(this, Container());
}
