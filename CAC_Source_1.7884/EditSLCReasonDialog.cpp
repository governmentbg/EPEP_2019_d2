//{{TEditSLCReasonDialog Implementation}}

TEditSLCReasonDialog::TEditSLCReasonDialog(TWindow* parent, TSLCReason *reason, bool edit, int resId)
:
	TEditDialog(parent, reason, edit, resId)
{
	new TCharAliasFace(this, IDC_EDIT_SLC_REASON_SLC_TYPE, "F_SLC_TYPE", SLCType, C2S[reason->slcType]);
	new TCheckFace(this, IDC_EDIT_SLC_REASON_ACTIVE, "F_FLAGS_ACTIVE");
	new TNameFace(this, IDC_EDIT_SLC_REASON_NAME, "F_NAME", SIZE_OF_SLC_REASON_NAME);
	new TCharListFace(this, IDC_EDIT_SLC_REASON_LAW_KINDS, "F_LAW_KINDS", Kind, KIND_LAWSUITS, SIZE_OF_LAW_KINDS);
	new TLongFace(this, IDC_EDIT_SLC_REASON_ORDER, "F_ORDER", SIZE_OF_ORDER);
}
