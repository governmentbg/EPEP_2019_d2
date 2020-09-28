//{{TBanConSideListBox Implementation}}

TBanConSideListBox::TBanConSideListBox(TWindow* parent, int resourceId, TGArray<TBankWind> *bankWinds)
:
	TGroupListBox(parent, resourceId, bankWinds)
{
}

void TBanConSideListBox::Cert(const char *title)
{
	bank_cert(this, (TGArray<TBankWind> *) array, title);
}

void TBanConSideListBox::Query()
{
	query_key(this, ((TSideWind *) Group())->key);
}

void TBanConSideListBox::Print()
{
	print_ucn_groups(array, NULL, "BankSide.htm", "Дела по несъстоятелност");
}

TGroup *TBanConSideListBox::NewGroup() const
{
	return NULL;
}

void TBanConSideListBox::GetGroupText(mstr &m, TGroup *group, int)
{
	TBankWind *bankWind = (TBankWind *) group;
	TSession session;

	m.printf("%-15s  %5ld/%4ld  %-10s", Type->Seek(bankWind->key.type), bankWind->key.no, bankWind->key.year,
		Involvement->Seek(bankWind->involvement));

	if (bankWind->session.key.no)
	{
		m.printf("  %-40s", Result->Seek(bankWind->session.result));

		for (int i = 0; i < bankWind->decisions.Count(); i++)
			m.printf("%c %s", i ? ',' : ' ', Kind->Seek(bankWind->decisions[i]->kind));
	}
}
