DEFINE_RESPONSE_TABLE1(TCreateLinksDialog, TScaleDialog)
//{{TCreateLinksDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDOK, OKBNEnable),
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TCreateLinksDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TCreateLinksDialog Implementation}}

TCreateLinksDialog::TCreateLinksDialog(TWindow* parent, TLinkWindArray *tLinkWinds, TGArray<TSideWind> *tOrdinary,
	TGArray<TSideWind> *tProxy, TGArray<TLinkWind> *tNewLinks, int resId)
:
	TScaleDialog(parent, resId), linkWinds(tLinkWinds), newLinks(tNewLinks)
{
	ordinary = new TSideWindListBox(this, IDC_CREATE_LINKS_ORDINARY, tOrdinary);
	proxy = new TSideWindListBox(this, IDC_CREATE_LINKS_PROXY, tProxy);
}

void TCreateLinksDialog::SetupWindow()
{
	TScaleDialog::SetupWindow();
}

void TCreateLinksDialog::OKBNEnable(TCommandEnabler &tce)
{
	tce.Enable(ordinary->GetSelCount() > 0 && proxy->Group());
}

void TCreateLinksDialog::OKBNClicked()
{
	TSideWind *tProxy = proxy->SideWind();

	for (int i = 0; i < ordinary->Count(); i++)
	{
		if (ordinary->GetSel(i))
		{
			TLinkWind *tLinkWind = create_link_wind(this, linkWinds, ordinary->SideWindAt(i), tProxy);

			if (tLinkWind)
				newLinks->Add(tLinkWind);
		}
	}

	CmOk();
}
