DEFINE_RESPONSE_TABLE1(TSelectFirmsDialog, TGroupDialog)
//{{TSelectFirmsDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_LBN_SELCHANGE(IDC_SELECT_FIRMS_FIRMS, FirmsLBNSelChange),
	EV_BN_CLICKED(IDSEARCH, SearchBNClicked),
	EV_BN_CLICKED(IDADDRESS, AddressBNClicked),
	EV_COMMAND_ENABLE(IDADDRESS, AddressBNEnable),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
	EV_BN_CLICKED(IDC_SELECT_FIRMS_RESEARCH, ResearchBNClicked),
	EV_EN_UPDATE(IDC_SELECT_FIRMS_NAME, NameENUpdate),
//{{TSelectFirmsDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSelectFirmsDialog Implementation}}

TSelectFirmsDialog::TSelectFirmsDialog(TWindow* parent, firm_action tAction, int resId)
:
	TGroupDialog(parent, &firm, resId), action(tAction)
{
	new TCharAliasFace(this, IDC_SELECT_UCN_UXN_TYPE, "F_UCN_TYPE", UXNType, C2S[UXN_FIRM]);
	name = new TFirmNameFace(this, IDC_SELECT_FIRMS_NAME, "F_NAME", SIZE_OF_FIRM_NAME);
	bulstat = new TBulStatFace(this, IDC_SELECT_FIRMS_BULSTAT, "F_BULSTAT");
	firms = new TSelectUCNListBox(this, IDC_SELECT_FIRMS_FIRMS, UCN_FIRMS, false, false);
	ok = new TPushButton(this, IDOK);
	search = new TPushButton(this, IDSEARCH);
	research = new TButton(this, IDC_SELECT_FIRMS_RESEARCH);
}

void TSelectFirmsDialog::OKBNClicked()
{
	if (IsValid())
	{
		TGArray<TFirm> tFirms;

		for (int i = 0; i < firms->GetCount(); i++)
		{
			if (firms->GetSel(i))
			{
				TFirm *tFirm = new TFirm;
				*tFirm << firms->GroupAt(i);
				tFirms.Add(tFirm);
			}
		}

		if (!tFirms.Count())
		{
			TFirm *tFirm = new TFirm;
			*tFirm << this;
			tFirms.Add(tFirm);
		}

		action(this, &tFirms);
	}
}

void TSelectFirmsDialog::FirmsLBNSelChange()
{
	UpdateButtons(firms->GetSelCount() > 0);
}

void TSelectFirmsDialog::SearchBNClicked()
{
	if (Search(false))
		UpdateButtons(firms->GetCount() == 0);
}

bool TSelectFirmsDialog::Search(bool extra)
{
	firm << this;

	bool valid = firm.ValidCriteria();

	if (valid)
		firms->SetUCNGroup(&firm, extra, INVOLVEMENT_BANCONSIST);
	else
		error("Критерия за търсене е невалиден или прекалено общ. Коригирайте го.");

	return valid;
}

void TSelectFirmsDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();
	UpdateButtons(false);
}

void TSelectFirmsDialog::AddressBNClicked()
{
	int index;

#if RANGECHECK
	if (firms->GetSelIndexes(&index, 1) != 1)
		fatal("select firms: not single index");
#else  // RANGECHECK
	firms->GetSelIndexes(&index, 1);
#endif  // RANGECHECK
	view_addresses(this, (TUCNGroup *) firms->GroupAt(index));
}

void TSelectFirmsDialog::AddressBNEnable(TCommandEnabler &tce)
{
	tce.Enable(firms->GetSelCount() == 1);
}

void TSelectFirmsDialog::PrintBNClicked()
{
	firms->Print(UXN_FIRM);
}

void TSelectFirmsDialog::ResearchBNClicked()
{
	if (Search(true))
		UpdateButtons(firms->GetCount() == 0);
}

void TSelectFirmsDialog::NameENUpdate()
{
	firms->SetSelItemRange(0, firms->GetCount(), false);
	UpdateButtons(false);
}

void TSelectFirmsDialog::UpdateButtons(bool okay)
{
	if (okay)
	{
		SetDefault(search, ok);
		ok->EnableWindow(true);
	}
	else
	{
		SetDefault(ok, search);
		ok->EnableWindow(false);
	}
}
