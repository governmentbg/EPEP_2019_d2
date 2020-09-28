DEFINE_RESPONSE_TABLE1(TEditLawerDataDialog, TGroupDialog)
//{{TPersonalNoDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TPersonalNoDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditLawerDataDialog Implementation}}

TEditLawerDataDialog::TEditLawerDataDialog(TWindow* parent, TKnownPerson *person, bool tEPortal)
:
	TGroupDialog(parent, person, IDD_EDIT_LAWER_DATA), eportal(tEPortal), realUCN(is_real_ucn(person))
{
	personalNo = new TUCNFace(this, IDC_EDIT_LAWER_DATA_PERSONAL_NO, "F_PERSONAL_NO", UCN_INSTANCE_RESERVED);
	birthDate = new TDateFace(this, IDC_EDIT_LAWER_DATA_BIRTH_DATE, "F_BIRTH_PART", -1);
}

bool TEditLawerDataDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (!*personalNo->S())
			isValid = error("Личния номер е задължителен.");
		else if (eportal && !realUCN && birthDate->Date().Empty())
			isValid = error("Датата на раждане е задължителна.");
	}

	return isValid;
}

void TEditLawerDataDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	if (realUCN)
		birthDate->EnableWindow(false);
}

void TEditLawerDataDialog::OKBNClicked()
{
	if (IsValid())
	{
		msql m(Person()->Name, "F_PERSONAL_NO");

		*Person() << this;
		m.AddString("F_PERSONAL_NO", Person()->personalNo);
		m.AddString("NOT (F_UCN", Person()->ucn);
		m.AddChar("F_UCN_TYPE", Person()->ucnType);
		m.cat(")");

		TQuery q(str(m));
		if (q.Read())
			error("Вече има лице с този личен номер.");
		else if (Person()->Update())
				CmOk();
	}
}
