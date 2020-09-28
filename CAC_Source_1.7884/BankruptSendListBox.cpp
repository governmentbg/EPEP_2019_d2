//{{TBankruptSendListBox Implementation}}

TBankruptSendListBox::TBankruptSendListBox(TWindow* parent, int resourceId, TLawsuit *tLawsuit)
:
	TGroupListBox(parent, resourceId, new TGSArray<TRCDKeyContainer>), lawsuit(tLawsuit)
{
	loadData = false;

	TSession session;
	session.key = lawsuit->key;
	container_events(&session, (TGSArray<TRCDKeyContainer> *) array);

	TSurround surround;
	surround.lawsuit = lawsuit->key;
	container_events(&surround, (TGSArray<TRCDKeyContainer> *) array);

	xfer_bank_filter((TGSArray<TRCDKeyContainer> *) array);
}

void TBankruptSendListBox::Export()
{
	TGArray<TRCDKeyContainer> containers;
	GetMark(&containers);
	export_bankrupt(this, lawsuit, &containers);
}

void TBankruptSendListBox::Query()
{
	THTMLFile h;
	bool first = true;

	h.OpenTemp();

	for (int index = 0; index < array->Count(); index++)
	{
		if (GetSel(index))
		{
			if (first)
				first = false;
			else
				h.puts("---\n\n");

			query_event(h, ContainerAt(index), true);
		}
	}

	h.Show();
}

void TBankruptSendListBox::GetGroupText(mstr &m, TGroup *group, int)
{
	TRCDKeyContainer *container = (TRCDKeyContainer *) group;

	m.printf("%-D  %s", &container->date, strchr(KIND_TEXTUAL_SURROUNDS, container->kind) ?
		((TSurround *) container)->text : Kind->Seek(container->kind));
}
