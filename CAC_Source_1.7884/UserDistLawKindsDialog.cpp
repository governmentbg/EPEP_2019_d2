//{{TUserDistLawKindsDialog Implementation}}

TUserDistLawKindsDialog::TUserDistLawKindsDialog(TWindow* parent, TUser *user, int resId)
:
	TEditDialog(parent, user, true, resId)
{
	new TCharListFace(this, IDC_USER_DIST_LAW_KINDS_KINDS, "F_LAW_KINDS", Kind, KIND_LAWSUITS, SIZE_OF_LAW_KINDS);
}
