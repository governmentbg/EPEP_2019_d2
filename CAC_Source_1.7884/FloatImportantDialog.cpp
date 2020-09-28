//{{TFloatImportantDialog Implementation}}

TFloatImportantDialog::TFloatImportantDialog(TWindow* parent, TFloatImportantGroup *group, int resId)
:
	TFloatDialog(parent, group, resId)
{
	types = new TTypeListFace(this, IDC_FLOAT_IMPORTANT_TYPES, "F_TYPES", QUERY_COLLEGES, false);
	start = new TDateFace(this, IDC_FLOAT_IMPORTANT_START, "F_START", false);
	final = new TDateFace(this, IDC_FLOAT_IMPORTANT_FINAL, "F_FINAL", false);
}

bool TFloatImportantDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (start->Date().Empty() && final->Date().Empty())
			isValid = error("Изисква се поне една дата.");
	}

	return isValid;
}
