//{{TOutRegBookDialog Implementation}}

TOutRegBookDialog::TOutRegBookDialog(TWindow* parent, TOutRegBookGroup *group, int resId)
:
	TBookDialog(parent, group, College, resId)
{
	newFormat = new TCheckFace(this, IDC_OUTREG_BOOK_NEW_FORMAT, "F_NEW_FORMAT");
}
