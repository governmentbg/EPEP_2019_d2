DEFINE_RESPONSE_TABLE1(TGetExternContainerDialog, TGetGroupDialog)
//{{TGetExternContainerDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDC_GET_EXTERN_CONTAINER_EXTERNAL, ExternalBNClicked),
//{{TGetExternContainerDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TGetExternContainerDialog Implementation}}

TGetExternContainerDialog::TGetExternContainerDialog(TWindow* parent, TRCDKeyGroup **grouptr, match_func match,
	const TExternSession *tSuggest, bool tForceExcess, int getWhat, int resId)
:
	TGetGroupDialog(parent, &tGroup, grouptr, TYPE_LAWSUITS, KIND_LAWSUITS, NULL, match, getWhat, resId),
		suggest(tSuggest), forceExcess(tForceExcess), getExternLawsuit(getWhat == GET_GROUP_ARCHFN)
{
	external = new TCheckFace(this, IDC_GET_EXTERN_CONTAINER_EXTERNAL, "F_EXTERNAL");
}

void TGetExternContainerDialog::SetupWindow()
{
	TGetGroupDialog::SetupWindow();

	if (forceExcess)
	{
		external->SetCheck(BF_CHECKED);
		external->EnableWindow(false);
	}

	if (suggest)
	{
		no->SetL(suggest->key.no);
		year->SetL(suggest->key.year);
		type->SetC(suggest->key.type);
	}

	ExternalBNClicked();
}

void TGetExternContainerDialog::OKBNClicked()
{
	lawsuit << this;

	if (external->GetCheck() == BF_UNCHECKED)
		TGetGroupDialog::OKBNClicked();
	else if (!getExternLawsuit || lawsuit.Get())
	{
		const char *status = match(&lawsuit, lawsuit.Status());

		if (status)
			error("%s %ld/%ld - %s.", Type->Seek(lawsuit.key.type), lawsuit.key.no, lawsuit.key.year, status);
		else
		{
			*grouptr = new TExternLawsuit;
			**grouptr << lawsuit;
			CmOk();
		}
	}
}

void TGetExternContainerDialog::ExternalBNClicked()
{
	query->EnableWindow(external->GetCheck() == BF_UNCHECKED);
}
