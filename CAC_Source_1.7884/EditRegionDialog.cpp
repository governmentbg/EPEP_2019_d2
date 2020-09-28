//{{TEditRegionDialog Implementation}}

TEditRegionDialog::TEditRegionDialog(TWindow* parent, TRegion *tRegion, bool edit, int resId)
:
	TEditDialog(parent, tRegion, edit, resId)
{
	area = new TAreaAliasFace(this, IDC_EDIT_REGION_AREA, "F_AREA");
	region = new TLongFace(this, IDC_EDIT_REGION_REGION, "F_REGION_LONG", SIZE_OF_REGION);
	new TStringFace(this, IDC_EDIT_REGION_NAME, "F_NAME", SIZE_OF_UCLP_NAME);
}

void TEditRegionDialog::SetupWindow()
{
	TEditDialog::SetupWindow();

	if (!edit)
	{
		// 2007:258 LPR: enable, so we can move region to other area
		//area->EnableWindow(true);
		region->EnableWindow(true);
	}
}
