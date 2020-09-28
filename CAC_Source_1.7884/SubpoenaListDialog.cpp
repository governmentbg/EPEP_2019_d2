//{{TSubpoenaListDialog Implementation}}
TSubpoenaListDialog::TSubpoenaListDialog(TWindow *parent, TSubpoenaListGroup *group, int resId)
:
	TGroupDialog(parent, group, resId)
{
	group->emit = SUBPOENA_LIST_SUMMARY | SUBPOENA_LIST_LETTERS;
	new TRadioFace(this, IDC_SUBPOENA_LIST_NOTIFIED, "F_NOTIFIED");
	new TRadioFace(this, IDC_SUBPOENA_LIST_IMPORTANT, "F_IMPORTANT");
	new TRadioFace(this, IDC_SUBPOENA_LIST_ALL, "F_ALL");
	new TCheckFace(this, IDC_SUBPOENA_LIST_SUMMARY, "F_SUMMARY");
	new TCheckFace(this, IDC_SUBPOENA_LIST_LETTERS, "F_LETTERS");
	new TLongFace(this, IDC_SUBPOENA_LIST_EMPTY, "F_EMPTY", SIZE_OF_EMPTY_LINES);
}
