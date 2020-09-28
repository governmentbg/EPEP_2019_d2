DEFINE_RESPONSE_TABLE1(TEditInformedDialog, TGroupDialog)
//{{TEditInformedDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TEditInformedDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditInformedDialog Implementation}}

TEditInformedDialog::TEditInformedDialog(TWindow* parent, TInformed *informed, bool tEdit, int resId)
:
	TGroupDialog(parent, informed, resId), edit(tEdit)
{
	new TCharAliasFace(this, IDC_EDIT_INFORMED_TYPE, "F_TYPE", Type, TYPE_LAWSUITS);
	new TLongFace(this, IDC_EDIT_INFORMED_NO, "F_NO", SIZE_OF_NO);
	new TYearFace(this, IDC_EDIT_INFORMED_YEAR, "F_YEAR");
}

void TEditInformedDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Group() << this;

		TLawsuit lawsuit;

		lawsuit.key = ((TInformed *) Group())->key;

		if (lawsuit.Get("F_TYPE") && Group()->Save(edit))
			CmOk();
	}
}
