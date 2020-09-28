DEFINE_RESPONSE_TABLE1(TLoadDateDialog, TLoadHallDialog)
//{{TLoadDateDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TLoadDateDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TLoadDateDialog Implementation}}

TLoadDateDialog::TLoadDateDialog(TWindow* parent, TLoadDate *loadDate, bool tEdit, int resId)
:
	TLoadHallDialog(parent, loadDate, resId), edit(tEdit)
{
	new TDateFace(this, IDC_LOAD_DATE_DATE, "F_DATE");
}

void TLoadDateDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Group() << this;
		
		if (LoadHall()->Save(edit))
			CmOk();
	}
}
