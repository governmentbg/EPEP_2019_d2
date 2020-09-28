//{{TGetDiscardDialog Implementation}}

TGetDiscardDialog::TGetDiscardDialog(TWindow* parent, TPayment *payment, int resId)
:
	TGroupDialog(parent, payment, resId)
{
	new TLongFace(this, IDC_GET_DISCARD_DISCARD, "F_DISCARD", SIZE_OF_DISCARD);
}
