DEFINE_RESPONSE_TABLE1(TSelectStringDialog, TQuickDialog)
//{{TSelectContainerDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDOK, SelectEnable),
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TSelectContainerDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSelectStringDialog Implementation}}

TSelectStringDialog::TSelectStringDialog(TWindow* parent, TStringArray *tStrings, const char *tTitle, const char *tLabel,
	int &tChoice, int resId)
:
	TQuickDialog(parent, resId), strings(tStrings), title(tTitle), label(tLabel), choice(tChoice)
{
	listLabel = new TStatic(this, IDC_SELECT_STRING_LIST_LABEL);
	list = new TStringListBox(this, IDC_SELECT_STRING_LIST);
}

void TSelectStringDialog::SetupWindow()
{
	TQuickDialog::SetupWindow();

	if (title)
		SetCaption(title);
	if (label)
		listLabel->SetCaption(label);

	for (int i = 0; i < strings->Count(); i++)
		list->AddString((*strings)[i]);

	list->SetHorizExtent();
}

void TSelectStringDialog::SelectEnable(TCommandEnabler &tce)
{
	tce.Enable(list->GetSelIndex() >= 0);
}

void TSelectStringDialog::OKBNClicked()
{
	choice = list->GetSelIndex();
	CmOk();
}
