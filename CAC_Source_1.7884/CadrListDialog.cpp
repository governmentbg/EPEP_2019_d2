DEFINE_RESPONSE_TABLE1(TCadrListDialog, TGroupListDialog)
//{{TCadrListDialogRSP_TBL_BEGIN}}
	EV_LVN_ITEMCHANGING(IDC_CADR_LIST_AREAS, AreasLVNItemChanging),
	EV_LVN_ITEMCHANGING(IDC_CADR_LIST_REGIONS, RegionsLVNItemChanging),
	EV_LVN_ITEMCHANGING(IDC_CADR_LIST_UCLPS, UCLPSLVNItemChanging),
	EV_LVN_ITEMCHANGING(IDC_CADR_LIST_STREETS, StreetsLVNItemChanging),
	EV_TCN_SELCHANGE(IDC_CADR_LIST_PAGES, PagesTCNSelChange),
	EV_LVN_ITEMCHANGED(IDC_CADR_LIST_AREAS, AreasLVNItemChanged),
	EV_LVN_ITEMCHANGED(IDC_CADR_LIST_REGIONS, RegionsLVNItemChanged),
	EV_LVN_ITEMCHANGED(IDC_CADR_LIST_UCLPS, UCLPSLVNItemChanged),
	EV_COMMAND_ENABLE(IDADD, AddBNEnable),
	EV_BN_CLICKED(IDPREV, PrevBNClicked),
	EV_BN_CLICKED(IDNEXT, NextBNClicked),
//{{TCadrListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TCadrListDialog Implementation}}

TCadrListDialog::TCadrListDialog(TWindow* parent, int tStartTab, int resId)
:
	TGroupListDialog(parent, resId), startTab(tStartTab), newArea(DEFAULT_AREA), newRegion(DEFAULT_REGION), newUCLP(COURT_UCLP)
{
	range = new TStatic(this, IDC_CADR_LIST_RANGE);
	label = new TStatic(this, IDC_CADR_LIST_LABEL);
	glist = empty = new TEmptyListWindow(this, IDC_GROUP_LIST_WINDOW);
	lists[TAB_AREAS] = areas = new TAreaListWindow(this, IDC_CADR_LIST_AREAS, newArea);
	lists[TAB_REGIONS] = regions = new TRegionListWindow(this, IDC_CADR_LIST_REGIONS, newArea, newRegion);
	lists[TAB_UCLPS] = uclps = new TUCLPListWindow(this, IDC_CADR_LIST_UCLPS, newArea, newRegion, newUCLP);
	lists[TAB_STREETS] = streets = new TStreetListWindow(this, IDC_CADR_LIST_STREETS, newUCLP);
	pages = new TTabControl(this, IDC_CADR_LIST_PAGES);
}

void TCadrListDialog::Activate(int sel)
{
	static constant labels[TAB_COUNT] = { "&Списък на областите", "&Списък на общините", "&Списък на населените места",
		"&Списък на улиците" };
	static constant misses[TAB_COUNT] = { "", "Не е избрана област", "Не е избрана област или община",
		"Не е избрано населено място" };
	TGroupListWindow *tList = glist;
	bool selected = false;
	mstr m;

	switch (sel)
	{
		case TAB_REGIONS :
		{
			if (newArea)
			{
				m.printf("Област %s", Area->Seek(newArea));
				selected = true;
			}
			break;
		}
		case TAB_UCLPS :
		{
			if (newArea && newRegion)
			{
				m.printf("Област %s, Община %s", Area->Seek(newArea), Region->Seek(newArea, newRegion));
				selected = true;
			}
			break;
		}
		case TAB_STREETS :
		{
			if (newUCLP)
			{
				m.printf("Населено място %s", UCLP->Seek(newUCLP));
				selected = true;
			}
			break;
		}
		default :
		{
			m.printf("Държава %s", Country->Seek(BUILDIN_COUNTRY));
			selected = true;
		}
	}

	if (selected)
	{
		glist = lists[sel];
		range->SetCaption(str(m));
	}
	else
	{
		glist = empty;
		range->SetCaption(misses[sel]);
	}

	label->SetCaption(labels[sel]);

	if (glist != tList)
	{
		tList->ShowWindow(SW_HIDE);
		glist->ShowWindow(SW_SHOW);
		((TCadrListWindow *) glist)->Activate();
		glist->SetFocus();
	}
}

void TCadrListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();

	constant tabNames[TAB_COUNT] = { " Области", "Общини", "Населени места", "Улици" };

	for (int index = 0; index < TAB_COUNT; index++)
		pages->Add(tabNames[index]);

	pages->SetSel(startTab);
	Activate(startTab);
}

bool TCadrListDialog::AreasLVNItemChanging(TLwNotify &lwn)
{
	return areas->ItemChanging(lwn);
}

bool TCadrListDialog::RegionsLVNItemChanging(TLwNotify &lwn)
{
	return regions->ItemChanging(lwn);
}

bool TCadrListDialog::UCLPSLVNItemChanging(TLwNotify &lwn)
{
	return uclps->ItemChanging(lwn);
}

bool TCadrListDialog::StreetsLVNItemChanging(TLwNotify &lwn)
{
	return streets->ItemChanging(lwn);
}

void TCadrListDialog::PagesTCNSelChange(TNotify &)
{
	if (pages->GetSel() >= 0)
		Activate(pages->GetSel());
}

void TCadrListDialog::AreasLVNItemChanged(TLwNotify &lwn)
{
	if ((lwn.uChanged & LVIS_DROPHILITED) && areas->InitialRefresh())
	{
		newArea = areas->Area() ? areas->Area()->area : 0;
		newRegion = newUCLP = 0;
	}
}

void TCadrListDialog::RegionsLVNItemChanged(TLwNotify &lwn)
{
	if ((lwn.uChanged & LVIS_DROPHILITED) && regions->InitialRefresh())
	{
		newRegion = regions->Region() ? regions->Region()->region : 0;
		newUCLP = 0;
	}
}

void TCadrListDialog::UCLPSLVNItemChanged(TLwNotify &lwn)
{
	if ((lwn.uChanged & LVIS_DROPHILITED) && uclps->InitialRefresh())
		newUCLP = uclps->UCLP() ? uclps->UCLP()->uclp : 0;
}

void TCadrListDialog::AddBNEnable(TCommandEnabler &tce)
{
	tce.Enable(glist != empty);
}

void TCadrListDialog::PrevBNClicked()
{
	int sel = pages->GetSel();

	if (sel > 0)
	{
		pages->SetSel(sel - 1);
		Activate(sel - 1);
	}
}

void TCadrListDialog::NextBNClicked()
{
	int sel = pages->GetSel();

	if (sel >= 0 && sel < TAB_COUNT - 1)
	{
		pages->SetSel(sel + 1);
		Activate(sel + 1);
	}
}
