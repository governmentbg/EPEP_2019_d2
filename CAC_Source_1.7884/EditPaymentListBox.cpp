DEFINE_RESPONSE_TABLE1(TEditPaymentListBox, TCloneListBox)
//{{TEditPaymentListBoxRSP_TBL_BEGIN}}
	EV_WM_SETFOCUS,
//{{TEditPaymentListBoxRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditPaymentListBox Implementation}}

TEditPaymentListBox::TEditPaymentListBox(TWindow* parent, int resourceId)
:
	TCloneListBox(parent, resourceId, NULL, TSideWind::Clone)
{
	initialRefresh = false;
	key.year = 0;
}

bool TEditPaymentListBox::Search(const TRCDKey &tKey)
{
	if (tKey != key)
	{
		key = tKey;
		loadData = true;
		Refresh();
		return true;
	}

	return false;
}

void TEditPaymentListBox::Search(TEditPaymentListBox *that)
{
	array->Flush();
	loadData = false;

	for (int i = 0; i < that->array->Count(); i++)
	{
		TGroup *group = NewGroup();
		*group << (*that->array)[i];
		array->Add(group);
	}

	Refresh();
}

void TEditPaymentListBox::Select(const char *ucn, char ucnType)
{
	if (any(ucn) && ucnType)
	{
		for (int i = 0; i < array->Count(); i++)
		{
			if (!strcmp(UCNGroupAt(i)->ucn, ucn) && UCNGroupAt(i)->ucnType == ucnType)
			{
				SetVisualIndex(i);
				return;
			}
		}

		if (!multiSelect)
		{
			FlushVisuals();
			array->Flush();
			key.year = 0;	// refresh on events
		}

		TSideWind *sideWind = new TSideWind;

		sideWind->ucnType = ucnType;
		strcpy(sideWind->ucn, ucn);

		int index = InsertGroup(sideWind);

		if (multiSelect)
			SetSel(index, true);
		else
			SetVisualIndex(index);

		UpdateVisual();
	}
}

void TEditPaymentListBox::Criteria(msql &m, TGroup *group)
{
	m.Begin(group->Name, "DISTINCT F_UCN, F_UCN_TYPE");

	m.AddLong("F_NO", key.no);
	m.AddLong("F_YEAR", key.year);
	m.AddChar("F_TYPE", key.type);
	m.AddChars("F_UCN_TYPE", UCN_REAL_SIDES);
}

void TEditPaymentListBox::EvSetFocus(THandle hWndLostFocus)
{
	TCloneListBox::EvSetFocus(hWndLostFocus);

	if (GetSelIndex() == -1 && GetCaretIndex() != -1)
		SetSelIndex(GetCaretIndex());
}
