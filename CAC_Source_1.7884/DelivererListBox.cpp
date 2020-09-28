//{{TDelivererListBox Implementation}}

TDelivererListBox::TDelivererListBox(TWindow* parent, int resourceId)
:
	TAliasListBox(parent, resourceId, UserAliasGroup, C2S[POST_DELIVERER], FLAG_ACTIVE)
{
	abcskip = ABC_SKIP_LOWER;

	if (Default->post != POST_DELIVERER)
		flags |= USER_EMPTY;
}

void TDelivererListBox::SetupWindow()
{
	TAliasListBox::SetupWindow();

	if (Default->post == POST_DELIVERER)
	{
		SetSel(0, true);		// don't touch, must be selected
		EnableWindow(false);
	}
}
