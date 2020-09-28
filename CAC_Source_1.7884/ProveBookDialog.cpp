//{{TProveBookDialog Implementation}}

TProveBookDialog::TProveBookDialog(TWindow* parent, TProveBookGroup *group, int resId)
	:TBookDialog(parent, group, Type, resId)
{
	newFormat = new TCheckFace(this, IDC_BOOK_NEW_FORMAT, "F_NEW_FORMAT");
}
