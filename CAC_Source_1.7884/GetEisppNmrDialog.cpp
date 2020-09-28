//{{TGetEisppNmrDialog Implementation}}

TGetEisppNmrDialog::TGetEisppNmrDialog(TWindow* parent, TGroup *group, int resId)
:
	TGroupDialog(parent, group, resId)
{
	eispp_nmr = new TEisNmrFace(this, IDC_EISPP_NMR_NMR, "F_EISPP_NMR");
}

bool TGetEisppNmrDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (!*eispp_nmr->S())
			isValid = error("Номера е задължителен.");
	}

	return isValid;
}
