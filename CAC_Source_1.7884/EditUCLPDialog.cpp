DEFINE_RESPONSE_TABLE1(TEditUCLPDialog, TEditDialog)
//{{TEditUCLPDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_EDIT_UCLP_AREA, AreaCBNSelChange),
//{{TEditUCLPDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditUCLPDialog Implementation}}

TEditUCLPDialog::TEditUCLPDialog(TWindow* parent, TUCLP *tUCLP, bool full, bool edit, int resId)
:
	TEditDialog(parent, tUCLP, edit, resId)
{
	area = new TAreaAliasFace(this, IDC_EDIT_UCLP_AREA, "F_AREA", full ? ALIAS_ENABLE : FLAG_NULL);
	region = new TRegionAliasFace(this, IDC_EDIT_UCLP_REGION, "F_REGION", area, full ? ALIAS_ENABLE : FLAG_NULL);
	uclp = new TLongFace(this, IDC_EDIT_UCLP_UCLP, "F_UCLP_LONG", SIZE_OF_UCLP);
	new TStringFace(this, IDC_EDIT_UCLP_NAME, "F_NAME", SIZE_OF_UCLP_NAME);
}

void TEditUCLPDialog::SetupWindow()
{
	TEditDialog::SetupWindow();

	area->SetL(UCLP()->area);
	region->Reset(UCLP()->region);
	uclp->EnableWindow(!edit);
}

void TEditUCLPDialog::AreaCBNSelChange()
{
	region->Reset(0);
}
