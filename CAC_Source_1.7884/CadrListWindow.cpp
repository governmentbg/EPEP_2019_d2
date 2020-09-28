DEFINE_RESPONSE_TABLE1(TCadrListWindow, TGroupListWindow)
//{{TCadrpListWindowRSP_TBL_BEGIN}}
	EV_WM_KEYDOWN,
//{{TCadrListWindowRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TCadrListWindow Implementation}}

TCadrListWindow::TCadrListWindow(TWindow* parent, int resourceId, const long &tCodeValue)
:
	TGroupListWindow(parent, resourceId, NULL, 172), codeValue(tCodeValue)
{
	Add(
		new TGroupListColumn("", cadr_added, 12, LVCFMT_CENTER | LVCFMT_NO_SEARCH),
		new TGroupListColumn("»ÏÂ", "F_NAME", 148),
		NULL
	);

	initialRefresh = false;
	//qsMode = QUICK_SEARCH_ABLE;
	sortcol = 2;
}

void TCadrListWindow::Activate()
{
	Refresh();

	if (!initialRefresh)
	{
		for (int index = 0; index < array->Count(); index++)
		{
			if (((TLongAliasGroup *) GroupAt(index))->ValueField()->L() == codeValue)
			{
				SetVisualIndex(index);
				break;
			}
		}

		initialRefresh = true;
	}
}

void TCadrListWindow::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	if (!qs && (key == VK_LEFT || key == VK_RIGHT))
		ParentNotify(key == VK_LEFT ? IDPREV : IDNEXT);
	else
		TGroupListWindow::EvKeyDown(key, repeatCount, flags);
}
