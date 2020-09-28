DEFINE_RESPONSE_TABLE1(TFirmLawsuitSideListDialog, TSideWindListDialog)
//{{TFirmLawsuitSideListDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDFIRM, FirmBNClicked),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
	EV_BN_CLICKED(IDLIST, ListBNClicked),
	EV_BN_CLICKED(IDCANCEL, CancelBNClicked),
	EV_WM_CLOSE,
//{{TFirmLawsuitSideListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TFirmLawsuitSideListDialog Implementation}}

TFirmLawsuitSideListDialog::TFirmLawsuitSideListDialog(TWindow* parent, TLawsuit *lawsuit, long sideMode)
:
	TSideWindListDialog(parent, lawsuit, lawsuit, sideMode, "делото", IDD_FIRM_LAWSUIT_SIDE_LIST)
{
	firm = new TButton(this, IDFIRM);
}

void TFirmLawsuitSideListDialog::SetupWindow()
{
	TSideWindListDialog::SetupWindow();

	// 2004:167 LPR: IMHO even existing firms should be shown for edit
	PostMessage(WM_COMMAND, MAKEWPARAM(IDFIRM, BN_CLICKED), (LPARAM) firm->GetHandle());
}

bool TFirmLawsuitSideListDialog::CanClose()
{
	TDamn damn;

	if (edit && !find_damn(Lawsuit(), &damn, false))
		return error("Няма фирма.");

	return true;
}

void TFirmLawsuitSideListDialog::FirmBNClicked()
{
	TDamn damn;
	bool found;

	damn.ucnType = UCN_DAMN_CODE;
	damn.key = Lawsuit()->key;	// 2004:264 BUGFIX: was missing
	found = find_damn(Lawsuit(), &damn, false);
	// 2004:352 BIGFIX: was missing, from special
	// 2007:208 LPR: show header2 if found/edited
	if (found)
		header2->SetCaption(damn.name);
	else
	{
		msql m(CompanyType->Name);

		m.AddLong("F_SUBJECT", Lawsuit()->subject);
		m.AddOrder("F_ORDER");

		TQuery q(str(m));
		if (q.Read())
		{
			*CompanyType << q;
			damn.companyType = CompanyType->companyType;
			strcpy(damn.name, CompanyType->name);
		}
	}

	if (found ? edit_ucn(this, &damn, Lawsuit(), false) : add_ucn(this, &damn))
	{
		damn.key.no = 0;	// 2004:264 BUGFIX: was xfered
		edit_addresses(this, &damn, ADDRESS_ALLOW_FOREIGN);
		header2->SetCaption(damn.name);
	}
}

void TFirmLawsuitSideListDialog::PrintBNClicked()
{
	TSideWind *sideWind = sideWinds->SideWind();
	print_apfish(Lawsuit(), sideWind ? sideWind : sideWinds->SideWindAt(0));
}

void TFirmLawsuitSideListDialog::PrintBNEnable(TCommandEnabler &tce)
{
	tce.Enable(sideWinds->Count());
}

void TFirmLawsuitSideListDialog::ListBNClicked()
{
	sideWinds->List(false);
}

void TFirmLawsuitSideListDialog::CancelBNClicked()
{
	// 2013:210 LPR: EvClose() is not virtual and must be reinvoked
	EvClose();
}

void TFirmLawsuitSideListDialog::EvClose()
{
	TDamn damn;

	if (CanClose())
	{
		if (find_damn(Lawsuit(), &damn, false))
			TSideWindListDialog::EvClose();
		else if (!edit && warn("Няма фирма. Да изтрия ли %s?", what))
		{
			if (!delete_incomplete(container))
				error("Грешка при изтриване на %s.", what);

			TGroupListDialog::EvClose();
		}
	}
}
