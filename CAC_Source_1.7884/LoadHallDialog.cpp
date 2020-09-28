DEFINE_RESPONSE_TABLE1(TLoadHallDialog, TGroupDialog)
//{{TLoadHallDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDC_LOAD_HALL_BEF_RESERVED, BefReservedBNClicked),
	EV_BN_CLICKED(IDC_LOAD_HALL_AFT_RESERVED, AftReservedBNClicked),
	EV_CBN_SELCHANGE(IDC_LOAD_HALL_BEF_COLLEGE, BefCollegeCBNSelChange),
	EV_CBN_SELCHANGE(IDC_LOAD_HALL_AFT_COLLEGE, AftCollegeCBNSelChange),
//{{TLoadHallDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TLoadHallDialog Implementation}}

TLoadHallDialog::TLoadHallDialog(TWindow* parent, TLoadHall *loadHall, int resId)
:
	TGroupDialog(parent, loadHall, resId)
{
	hall = new TEdit(this, IDC_LOAD_HALL_HALL);

	bef.college = new TCharAliasFace(this, IDC_LOAD_HALL_BEF_COLLEGE, "F_BEF_COLLEGE", College, Default->colleges, ALIAS_EMPTY);
	bef.composition = new TLongAliasFace(this, IDC_LOAD_HALL_BEF_COMPOSITION, "F_BEF_COMPOSITION", Composition, Default->compositions, ALIAS_EMPTY);
	bef.reserved = new TCheckFace(this, IDC_LOAD_HALL_BEF_RESERVED, "F_BEF_FLAGS_RESERVED");
	bef.text = new TStringFace(this, IDC_LOAD_HALL_BEF_TEXT, "F_BEF_TEXT", SIZE_OF_LOAD_HALL_TEXT);
	bef.origin = &loadHall->bef;

	aft.college = new TCharAliasFace(this, IDC_LOAD_HALL_AFT_COLLEGE, "F_AFT_COLLEGE", College, Default->colleges, ALIAS_EMPTY);
	aft.composition = new TLongAliasFace(this, IDC_LOAD_HALL_AFT_COMPOSITION, "F_AFT_COMPOSITION", Composition, Default->compositions, ALIAS_EMPTY);
	aft.reserved = new TCheckFace(this, IDC_LOAD_HALL_AFT_RESERVED, "F_AFT_FLAGS_RESERVED");
	aft.text = new TStringFace(this, IDC_LOAD_HALL_AFT_TEXT, "F_AFT_TEXT", SIZE_OF_LOAD_HALL_TEXT);
	aft.origin = &loadHall->aft;
}

void TLoadHallDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	hall->SetText(Hall->Seek(LoadHall()->hall));
	BefReservedBNClicked();
	AftReservedBNClicked();
}

void TLoadHallDialog::ReservedBNClicked(TLoadHallSet *hs)
{
	if (hs->reserved->GetCheck() == BF_CHECKED)
	{
		hs->college->EnableWindow(false);
		hs->college->SetC('\0');
	}
	else
	{
		hs->college->SetC(hs->origin->college);
		hs->college->EnableWindow(true);
	}

	CollegeCBNEvent(hs);
}

void TLoadHallDialog::CollegeCBNEvent(TLoadHallSet *hs)
{
	if (!hs->college->C())
	{
		hs->composition->EnableWindow(false);
		hs->composition->SetL(0);
	}
	else
	{
		hs->composition->SetL(hs->origin->composition);
		hs->composition->EnableWindow(true);
	}

	if (hs->reserved->GetCheck() == BF_UNCHECKED && !hs->college->C())
	{
		hs->text->EnableWindow(false);
		hs->text->SetS("");
	}
	else
	{
		hs->text->SetS(hs->origin->text);
		hs->text->EnableWindow(true);
	}
}

void TLoadHallDialog::BefReservedBNClicked()
{
	ReservedBNClicked(&bef);
}

void TLoadHallDialog::BefCollegeCBNSelChange()
{
	CollegeCBNEvent(&bef);
}

void TLoadHallDialog::AftReservedBNClicked()
{
	ReservedBNClicked(&aft);
}

void TLoadHallDialog::AftCollegeCBNSelChange()
{
	CollegeCBNEvent(&aft);
}
