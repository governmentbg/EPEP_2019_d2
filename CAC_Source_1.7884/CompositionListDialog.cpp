DEFINE_RESPONSE_TABLE1(TCompositionListDialog, TGroupListDialog)
//{{TCompositionListDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_COMPOSITION_LIST_COLLEGE, CollegeCBNSelChange),
	EV_LVN_ITEMCHANGED(IDC_GROUP_LIST_WINDOW, CompositionsLVNItemChanged),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
	EV_BN_CLICKED(IDACTIVE, ActiveBNClicked),
	EV_COMMAND_ENABLE(IDACTIVE, SelectEnable),
//{{TCompositionListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TCompositionListDialog Implementation}}

TCompositionListDialog::TCompositionListDialog(TWindow* parent, int resId)
:
	TGroupListDialog(parent, resId)
{
	college = new TCharAliasFace(this, IDC_COMPOSITION_LIST_COLLEGE, "F_COLLEGE", College, Default->colleges);
	compositions = new TCompositionListWindow(this, IDC_GROUP_LIST_WINDOW);
	active = new TButton(this, IDACTIVE);
}

void TCompositionListDialog::ActiveChanged()
{
	TFineComposition *composition = compositions->Composition();
	active->SetCaption(active_button_text(composition, composition->flags));
}

void TCompositionListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();
	CollegeCBNSelChange();
}

void TCompositionListDialog::CollegeCBNSelChange()
{
	compositions->SetCollege(college->C());
}

void TCompositionListDialog::PrintBNClicked()
{
	compositions->Print();
}

void TCompositionListDialog::CompositionsLVNItemChanged(TLwNotify &lwn)
{
	// 2015:135 FIX: uNewState & LVIS_DROPHILITED == 0 on Add
	if ((lwn.uChanged & LVIS_DROPHILITED) && compositions->Composition())
		ActiveChanged();
}

void TCompositionListDialog::ActiveBNClicked()
{
	if (compositions->Active())
		ActiveChanged();
}
