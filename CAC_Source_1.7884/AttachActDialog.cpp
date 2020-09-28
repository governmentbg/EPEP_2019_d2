DEFINE_RESPONSE_TABLE1(TAttachActDialog, THtmlBaseDialog)
//{{TAtachDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDATTACH, AttachBNClicked),
	EV_BN_CLICKED(IDSWAP, SwapBNClicked),
//{{TAtachDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TAttachActDialog Implementation}}

TAttachActDialog::TAttachActDialog(TWindow* parent, TIndexContainer *tContainer, const char *name, int resId)
:
	THtmlBaseDialog(parent, SHOW_SCALED, name, false, resId), container(tContainer), trouble(this, TYPE_TEXT)
{
}

void TAttachActDialog::SetupWindow()
{
	THtmlBaseDialog::SetupWindow();

	trouble.Name(container, container->date);
	trouble.BackupNames();
}

void TAttachActDialog::AttachBNClicked()
{
	trouble.Edit(true);
}

void TAttachActDialog::SwapBNClicked()
{
	trouble.Swap();
}

void TAttachActDialog::OKBNClicked()
{
	trouble.Move(container, container->date);
	CmOk();
}
