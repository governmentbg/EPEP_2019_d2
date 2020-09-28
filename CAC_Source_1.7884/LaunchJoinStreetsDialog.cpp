DEFINE_RESPONSE_TABLE1(TLaunchJoinStreetsDialog, TQuickDialog)
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDQUERY, QueryBNClicked),
	EV_CBN_SELCHANGE(IDC_ALTER_STREET_AREA, AreaCBNSelChange),
	EV_CBN_SELCHANGE(IDC_ALTER_STREET_REGION, RegionCBNSelChange),
END_RESPONSE_TABLE;

TLaunchJoinStreetsDialog::TLaunchJoinStreetsDialog(TWindow *parent, int resId)
	: TQuickDialog(parent, resId)
{
	area = new TAreaAliasFace(this, IDC_ALTER_STREET_AREA, "F_AREA");
	region = new TRegionAliasFace(this, IDC_ALTER_STREET_REGION, "F_REGION", area);
	uclp = new TFindUCLPAliasFace(this, IDC_ALTER_STREET_UCLP, "F_UCLP", area, region);
}

void TLaunchJoinStreetsDialog::OKBNClicked()
{
	if (IsValid())
	{
		ShowWindow(SW_HIDE);
		join_equal_streets(this, uclp->L(), true);
		CmOk();
	}
}

void TLaunchJoinStreetsDialog::QueryBNClicked()
{
	if (IsValid())
		join_equal_streets(this, uclp->L(), false);
}

bool TLaunchJoinStreetsDialog::IsValid()
{
	bool isValid =  TQuickDialog::IsValid();

	if (isValid)
	{
		if (uclp->L() == 0)
			isValid = error("Населеното място е задължително.");
	}

	return isValid;
}

void TLaunchJoinStreetsDialog::AreaCBNSelChange()
{
	region->Reset(0);
	RegionCBNSelChange();
}

void TLaunchJoinStreetsDialog::RegionCBNSelChange()
{
	uclp->Reset(0);
}


void TLaunchJoinStreetsDialog::SetupWindow()
{
	TQuickDialog::SetupWindow();

	area->SetL(DEFAULT_AREA);
	region->Reset(DEFAULT_REGION);
	uclp->Reset(COURT_UCLP);
}