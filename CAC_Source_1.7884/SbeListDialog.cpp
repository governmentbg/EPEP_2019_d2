DEFINE_RESPONSE_TABLE1(TSbeListDialog, TGroupListDialog)
//{{TSbeListDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDADD, AddBNEnable),
	EV_BN_CLICKED(IDADD, AddBNClicked),
	EV_COMMAND_ENABLE(IDEDIT, EditBNEnable),
	EV_BN_CLICKED(IDEDIT, EditBNClicked),
	EV_COMMAND_ENABLE(IDDELETE, DeleteBNEnable),
	EV_BN_CLICKED(IDDELETE, DeleteBNClicked),
//{{TSbeListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSbeListDialog Implementation}}

TSbeListDialog::TSbeListDialog(TWindow* parent, NPR *tNPR, TLawsuit *tLawsuit, TDatedContainer *tContainer,
	TIntegerArray *lset, TGArray<SBE> *tSbes, int resId)
:
	TGroupListDialog(parent, resId), npr(tNPR), lawsuit(tLawsuit), container(tContainer)
{
	new TEisNmrFace(this, IDC_SBE_LIST_NPRNMR, "F_NPRNMR", EISPP_NPR_KERNEL);
	new TCharAliasFace(this, IDC_SBE_LIST_TYPE, "F_TYPE", Type, TYPE_LAWSUITS);
	new TLongFace(this, IDC_SBE_LIST_NO, "F_NO", SIZE_OF_NO);
	new TYearFace(this, IDC_SBE_LIST_YEAR, "F_YEAR");
	sbe = new TSbeAliasFace(this, IDC_SBE_LIST_SBE, "F_SBE", tSbes, npr, lset);
	sbes = new TSbeListWindow(this, IDC_GROUP_LIST_WINDOW, npr, lawsuit, container, lset, tSbes, sbe);
}

void TSbeListDialog::SbeEnable(TCommandEnabler &tce, long nmk)
{
	SBE *sbe = sbes->Sbe();
	tce.Enable(sbe && (sbe->dirty || EisVal->Contains(nmk, sbe->vid)));
}

void TSbeListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();

	*npr >> this;
	*lawsuit >> this;
}

void TSbeListDialog::AddBNEnable(TCommandEnabler &tce)
{
	tce.Enable(sbe->L());
}

void TSbeListDialog::AddBNClicked()
{
	TGroupListDialog::AddBNClicked();
	sbe->Refresh();
}

void TSbeListDialog::EditBNEnable(TCommandEnabler &tce)
{
	SbeEnable(tce, nmk_sbekor);
}

void TSbeListDialog::EditBNClicked()
{
	sbes->Edit();
}

void TSbeListDialog::DeleteBNEnable(TCommandEnabler &tce)
{
	SbeEnable(tce, nmk_sbeitv);
}

void TSbeListDialog::DeleteBNClicked()
{
	TGroupListDialog::DeleteBNClicked();
	sbe->Refresh();
}
