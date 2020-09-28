DEFINE_RESPONSE_TABLE1(TChooseRedirectDialog, TScaleDialog)
//{{TChooseRedirectDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TChooseRedirectDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TChooseRedirectDialog Implementation}}

TChooseRedirectDialog::TChooseRedirectDialog(TWindow* parent, TLinkWind *tLinkWind, TGArray<TSideWind> *tRedirect,
	int resId)
:
	TScaleDialog(parent, resId), linkWind(tLinkWind)
{
	header = new TStatic(this, IDC_CHOOSE_REDIRECT_HEADER);
	redirect = new TSideWindListBox(this, IDC_CHOOSE_REDIRECT_REDIRECT, tRedirect);
}

void TChooseRedirectDialog::OKBNClicked()
{
	TSideWind *tRedirect = redirect->SideWind();
	TSideWind sideWind;

	linkWind->GetProxy(sideWind);

	if (tRedirect->SameWind(sideWind))
		error("Избраното упълномощено лице вече присъства във връзката.");
	else
	{
		linkWind->SetRedirect(*tRedirect);
		linkWind->SetRedirectKind(false);
		CmOk();
	}
}

void TChooseRedirectDialog::SetupWindow()
{
	TScaleDialog::SetupWindow();

	mstr m;
	linkWind->Print(m);
	header->SetCaption(str(m));
}
