DEFINE_RESPONSE_TABLE1(TEditCSJIDDialog, TGroupDialog)
//{{TCSJIDDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TCSJIDDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditCSJIDDialog Implementation}}

TEditCSJIDDialog::TEditCSJIDDialog(TWindow* parent, TKnownPerson *person, int resId)
:
	TGroupDialog(parent, person, resId)
{
	csjid = new TLongFace(this, IDC_EDIT_CSJID_CSJID, "F_CSJID", SIZE_OF_CSJID);
}

void TEditCSJIDDialog::OKBNClicked()
{
	if (IsValid())
	{
		msql m(Person()->Name);

		*Person() << this;
		m.AddLong("F_CSJID", Person()->csjid);
		m.AddString("NOT (F_UCN", Person()->ucn);
		m.AddChar("F_UCN_TYPE", Person()->ucnType);
		m.cat(")");

		TQuery q(str(m));
		if (!q.Read() || ask("Вече има лице с този регистрационен номер.\nЖелаете ли да продължите?"))
			if (Person()->Update())
				CmOk();
	}
}

bool TEditCSJIDDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (!csjid->L())
			isValid = error("Регистрационния номер е задължителен.");
	}

	return isValid;
}
