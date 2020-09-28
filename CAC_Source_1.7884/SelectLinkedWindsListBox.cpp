DEFINE_RESPONSE_TABLE1(TSelectLinkedWindsListBox, TGroupListBox)
//{{TSelectLinkedWindsListBoxRSP_TBL_BEGIN}}
	EV_WM_LBUTTONDOWN,
	EV_WM_RBUTTONDOWN,
	EV_WM_KEYDOWN,
//{{TSelectLinkedWindsListBoxRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSelectLinkedWindsListBox Implementation}}

TSelectLinkedWindsListBox::TSelectLinkedWindsListBox(TWindow* parent, int resourceId, TGArray<TLinkWind> *array)
:
	TGroupListBox(parent, resourceId, array), notifySelect(-1)
{
	loadData = initialRefresh = dynamic = false;
}

void TSelectLinkedWindsListBox::UpdateSelect(int tNotifySelect)
{
	notifySelect = tNotifySelect;
	Refresh();
	RestoreSelect();
}

void TSelectLinkedWindsListBox::RestoreSelect()
{
	if (notifySelect == NOTIFY_SELECT_LEGAL_AID && !GetSel(0))
		SetSel(0, true);
}

bool TSelectLinkedWindsListBox::IsValid()
{
	if (!IsWindowEnabled())
		return true;

	bool isValid = TGroupListBox::IsValid();

	if (isValid && (notifySelect == NOTIFY_SELECT_LEGAL_AID || notifySelect == NOTIFY_SELECT_EXEC_LIST))
	{
		TLinkWind *mandatory = LinkWindAt(0);

		for (int i = 0; i < array->Count(); i++)
			if (GetSel(i) && LinkWindAt(i)->SameWind(mandatory))
				return true;

		mstr m;
		mandatory->PrintNames(m);
		isValid = error("Лицето %s трябва задължително да е избрано.", str(m));
	}

	return isValid;
}

void TSelectLinkedWindsListBox::GetGroupText(mstr &m, TGroup *group, int)
{
	TLinkWind *linkWind = TYPECHECK_PTR_CAST(TLinkWind, group);

	if ((notifySelect == NOTIFY_SELECT_EXEC_LIST && !linkWind->Proxied()) || notifySelect == NOTIFY_SELECT_LEGAL_AID)
		m.printf("%s  ", Involvement->Seek(linkWind->involvement));

	print_side_links(m, linkWind);
}

void TSelectLinkedWindsListBox::EvLButtonDown(uint modKeys, TPoint& point)
{
	TGroupListBox::EvLButtonDown(modKeys, point);
	RestoreSelect();
}

void TSelectLinkedWindsListBox::EvRButtonDown(uint modKeys, TPoint& point)
{
	TGroupListBox::EvRButtonDown(modKeys, point);
	RestoreSelect();
}

void TSelectLinkedWindsListBox::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	TGroupListBox::EvKeyDown(key, repeatCount, flags);
	RestoreSelect();
}
