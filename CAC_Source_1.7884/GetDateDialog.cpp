DEFINE_RESPONSE_TABLE1(TGetDateDialog, TGroupDialog)
//{{TGetDateDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TGetDateDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TGetDateDialog Implementation}}

TGetDateDialog::TGetDateDialog(TWindow* parent, TGroup *group, char *tTitle, bool mandatory, bool tFuture)
:
	TGroupDialog(parent, group, IDD_GET_DATE), title(tTitle), future(tFuture)
{
	label = new TStatic(this, IDC_GET_DATE_LABEL);
	date = new TDateFace(this, IDC_GET_DATE_DATE, "F_DATE", mandatory);
}

void TGetDateDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	if (title)
		label->SetText(title);
}

void TGetDateDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Group() << this;
		CmOk();
	}
}

bool TGetDateDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (!date->Date().Empty() && !future && date->Date() > Today)
			isValid = error("Въведената дата е по-голяма от текущата.");
	}

	return isValid;
}
