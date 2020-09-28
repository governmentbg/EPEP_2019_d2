//{{TPostalDialog Implementation}}

TPostalDialog::TPostalDialog(TWindow* parent, TGroup *group, int resId)
:
	TGroupDialog(parent, group, resId)
{
	add_postal_buttons(this, buttons);
}
