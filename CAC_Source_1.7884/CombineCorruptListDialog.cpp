DEFINE_RESPONSE_TABLE1(TCombineCorruptListDialog, TGroupListDialog)
//{{TSubjectListDialogRSP_TBL_BEGIN}}
	EV_COMMAND(IDADD, AddBNClicked),
	EV_COMMAND_ENABLE(IDOK, OKBNEnable),
//{{TSubjectListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TCombineCorruptListDialog Implementation}}

TCombineCorruptListDialog::TCombineCorruptListDialog(TWindow* parent, TCombineCorruptGroup *group)
:
	TGroupListDialog(parent, IDD_COMBINE_CORRUPT_LIST)
{
	minMonth = new TStatic(this, IDC_COMBINE_CORRUPT_LIST_MIN_MONTH);
	minYear = new TStatic(this, IDC_COMBINE_CORRUPT_LIST_MIN_YEAR);
	maxMonth = new TStatic(this, IDC_COMBINE_CORRUPT_LIST_MAX_MONTH);
	maxYear = new TStatic(this, IDC_COMBINE_CORRUPT_LIST_MAX_YEAR);
	corrupts = new TCombineCorruptListWindow(this, IDC_GROUP_LIST_WINDOW, group);
}

void TCombineCorruptListDialog::AddBNClicked()
{
	TGroupListDialog::AddBNClicked();

	const TCombineCorruptGroup *group = corrupts->Group();

	if (group->array.Count() == 1)
	{
		mstr m;

		minMonth->SetCaption(group->minMonth ? Month->Seek(group->minMonth) : "");
		itom(group->minYear, m);
		minYear->SetCaption(str(m));
		maxMonth->SetCaption(group->maxMonth ? Month->Seek(group->maxMonth) : "");
		m.clear();
		itom(group->maxYear, m);
		maxYear->SetCaption(str(m));
	}
}

void TCombineCorruptListDialog::OKBNEnable(TCommandEnabler &tce)
{
	tce.Enable(corrupts->GetItemCount());
}
