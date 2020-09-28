//{{TEditProveDialog Implementation}}

TEditProveDialog::TEditProveDialog(TWindow* parent, TProve *prove, TRCDKeyContainer *tContainer, bool edit, int resId)
:
	TTroubleDialog(parent, prove, edit, TYPE_IMAGE, resId), container(tContainer)
{
	new TCharAliasFace(this, IDC_EDIT_PROVE_LAWSUIT_TYPE, "F_LAWSUIT_TYPE", Type, NULL);
	new TLongFace(this, IDC_EDIT_PROVE_LAWSUIT_NO, "F_LAWSUIT_NO", SIZE_OF_NO);
	new TYearFace(this, IDC_EDIT_PROVE_LAWSUIT_YEAR, "F_LAWSUIT_YEAR");

	no = new TLongFace(this, IDC_EDIT_PROVE_NO, "F_NO", SIZE_OF_NO);
	date = new TDateFace(this, IDC_EDIT_PROVE_DATE, "F_DATE");
	new TStringFace(this, IDC_EDIT_PROVE_DESCRIPTION, "F_DESCRIPTION", SIZE_OF_PROVE_DESCRIPTION);

	// 2006:040 LRQ: fuse dialogs
	//if (edit)
		new TStringFace(this, IDC_EDIT_PROVE_REMARK, "F_REMARK", SIZE_OF_PROVE_REMARK);
}

bool TEditProveDialog::IsValid()
{
	bool isValid = TTroubleDialog::IsValid();

	if (isValid)
	{
		if (!date->Date().Empty() && date->Date() > Today)
			isValid = error("Дата и текуща дата не си съответстват.");
		else if (container && !date->Date().Empty() && date->Date() < container->date)
			isValid = error(
				"Датата на вписване на вещественото доказателство "
				"не може да е преди датата на %s (%Dг.)",
				container->key.type == TYPE_REQUEST
				? "постъпване на входящия документ"
				: "образуване на делото",
				&container->date
			);
	}

	return isValid;
}

void TEditProveDialog::SetupWindow()
{
	TTroubleDialog::SetupWindow();

	no->EnableWindow(!edit);
	date->EnableWindow(!edit);
}
