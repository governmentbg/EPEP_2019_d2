DEFINE_RESPONSE_TABLE1(TSelectEventDialog, TSelectStringDialog)
//{{TSelectEventDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
	EV_BN_CLICKED(IDOTHER, OtherBNClicked),
//{{TSelectEventDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSelectEventDialog Implementation}}

TSelectEventDialog::TSelectEventDialog(TWindow* parent, const char *label, TCollectEvents *te, int &choice, TGArray<TRCDKeyContainer> *tArray, int resId)
:
	TSelectStringDialog(parent, &te->strings, NULL, label, choice, resId), e(te), array(tArray)
{
	other = new TButton(this, IDOTHER);
}

void TSelectEventDialog::SetupWindow()
{
	TSelectStringDialog::SetupWindow();

	other->EnableWindow(any(e->m));

	if (CHROME_AUTO_SELECT && list->GetCount() == 1)
		list->SetSelIndex(0);
}

void TSelectEventDialog::PrintBNClicked()
{
	print_containers(array, str(e->stats), label);
}

void TSelectEventDialog::OtherBNClicked()
{
	message("%s.", str(e->m));
}
