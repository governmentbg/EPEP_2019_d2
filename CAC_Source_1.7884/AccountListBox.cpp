//{{TAccountListBox Implementation}}

TAccountListBox::TAccountListBox(TWindow* parent, int resourceId)
:
	TAliasListBox(parent, resourceId, Account, C2S[UCN_EXEC_LIST], FLAG_NULL)
{
	delete array;
	array = new TCharAliasArray;
}
