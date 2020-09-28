//{{TGetNumapMarkDialog Implementation}}

TGetNumapMarkDialog::TGetNumapMarkDialog(TWindow* parent, TNumapMarkGroup *group, int resId)
:
	TGroupDialog(parent, group, resId)
{
	start = new TLongFace(this, IDC_GET_NUMAP_MARK_START, "F_START", SIZE_OF_ONE_ADDR_NO);
	final = new TLongFace(this, IDC_GET_NUMAP_MARK_FINAL, "F_FINAL", SIZE_OF_ONE_ADDR_NO);
	new TRadioFace(this, IDC_GET_NUMAP_MARK_ALL, "F_MARK_ALL");
	new TRadioFace(this, IDC_GET_NUMAP_MARK_EVEN, "F_MARK_EVEN");
	new TRadioFace(this, IDC_GET_NUMAP_MARK_ODD, "F_MARK_ODD");
}

bool TGetNumapMarkDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (!start->L() || !final->L())
			isValid = error("Началния и крайния номер са задължителни.");
		else if (start->L() > final->L())
			isValid = error("Началният номер е по-голям от крайния.");
	}

	return isValid;
}
