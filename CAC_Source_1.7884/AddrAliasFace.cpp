//{{TAddrAliasFace Implementation}}

TAddrAliasFace::TAddrAliasFace(TWindow* parent, int resourceId, const char *name, TFindAddrAliasGroup *alias)
:
	TLongAliasFace(parent, resourceId, name, alias, NULL, ALIAS_ABCSORT)
{
	initialRefresh = false;
}

void TAddrAliasFace::Reset(long value)
{
	if (value)
	{
		LongAliasGroup()->ValueField()->SetL(value);
		TAliasFace::Refresh();
		SetL(value);
	}
	else
		FlushList();
}

void TAddrAliasFace::SetupWindow()
{
	TLongAliasFace::SetupWindow();
	flags |= IsWindowEnabled() ? ADDR_FLOAT : ADDR_EXACT;
}
