//{{TLegalResponseListWindow Implementation}}

TLegalResponseListWindow::TLegalResponseListWindow(TWindow* parent, int resourceId, TGArray<TLegalResponse> *responses)
:
	TGroupListWindow(parent, resourceId, responses, 352)
{
	Add(new TGroupListColumn("Вид дело, номер/година и лице", group_print, 340));
}

bool TLegalResponseListWindow::LoadGroup()
{
	TLegalResponse *response = (TLegalResponse *) Group();

	if (legalaid_import(this, response))
	{
		RemoveGroup(IDLOAD);
		return true;
	}

	return false;
}
