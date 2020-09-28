//{{TSLCReasonListBox Implementation}}

TSLCReasonListBox::TSLCReasonListBox(TWindow* parent, int resourceId, const char *tSLCTypes, long tFlags)
:
	TCloneListBox(parent, resourceId, NULL, TSLCReason::Clone), slcTypes(tSLCTypes), flags(tFlags)
{
}

void TSLCReasonListBox::SetMark(TGArray<TSLCReason> *slcReasons)
{
	for (int i = 0; i < slcReasons->Count(); i++)
	{
		TSLCReason *reason = (*slcReasons)[i];

		for (int n = 0; n < array->Count(); n++)
		{
			if (reason->slcReason == SLCReasonAt(n)->slcReason)
			{
				SetSel(n, true);
				break;
			}
		}
	}
}

void TSLCReasonListBox::Criteria(msql &m, TGroup *group)
{
	m.Begin(group->Name);
	m.AddChars("F_SLC_TYPE", slcTypes);

	if (flags)
		m.AddFlags("F_FLAGS", (flags & SLC_REASON_ALLS), (flags & SLC_REASON_ALLS), SLC_REASON_ALLS);

	m.AddOrder("F_ORDER");
}

void TSLCReasonListBox::LoadData(const char *s)
{
	TCloneListBox::LoadData(s);

	if (flags & ALIAS_EMPTY)
		InsertEmpty();
}

void TSLCReasonListBox::GetGroupText(mstr &m, TGroup *group, int)
{
	TYPECHECK_PTR_CAST(TSLCReason, group)->PrintPrefixed(m, slcTypes);
}
