DEFINE_RESPONSE_TABLE1(TNumapDialog, TGroupDialog)
//{{TNumapDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDADD, AddBNClicked),
	EV_BN_CLICKED(IDDELETE, DeleteBNClicked),
//{{TNumapDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TNumapDialog Implementation}}

TNumapDialog::TNumapDialog(TWindow* parent, TStreetMap *streetMap, int resId)
:
	TGroupDialog(parent, streetMap, resId)
{
	uclp = new TUCLPAliasFace(this, IDC_NUMAP_UCLP, "F_UCLP", streetMap->uclp);
	street = new TEdit(this, IDC_NUMAP_STREET);
	numbers = new TNumapListFace(this, IDC_NUMAP_NUMBERS, "F_MAP", SIZE_OF_STREET_MAP);
}

void TNumapDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Group() << this;
		if (Group()->Update())
			CmOk();
	}
}

void TNumapDialog::AddBNClicked()
{
	numbers->Advanced(true);
}

void TNumapDialog::DeleteBNClicked()
{
	numbers->Advanced(false);
}

void TNumapDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	*Street << StreetMap();
	if (Street->Get())
		street->SetText(Street->name);
}
