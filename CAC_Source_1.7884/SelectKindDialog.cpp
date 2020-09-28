//{{TSelectKindDialog Implementation}}

TSelectKindDialog::TSelectKindDialog(TWindow* parent, TRCDKeyContainer *container, const char *kinds, int resId)
:
	TGroupDialog(parent, container, resId)
{
	new TCharAliasFace(this, IDC_SELECT_KIND_KIND, "F_KIND", Kind, kinds);
}
