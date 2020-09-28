//{{TEditSenderDialog Implementation}}

TEditSenderDialog::TEditSenderDialog(TWindow* parent, TSender *sender, bool edit, bool full, int resId)
:
	TEditNamedDialog(parent, sender, edit, full, resId)
{
	new TStringFace(this, IDC_EDIT_SENDER_UCN, "F_UCN", SIZE_OF_UCN);
	name = new TSenderNameFace(this, IDC_EDIT_SENDER_NAME, "F_NAME", SIZE_OF_NAME);
}
