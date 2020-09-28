DEFINE_RESPONSE_TABLE1(TSelectUCNBaseDialog, TGroupDialog)
//{{TSelectUCNBaseDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
	EV_CBN_SELCHANGE(IDC_SELECT_UCN_UXN_TYPE, UXNTypeCBNSelChange),
	EV_CBN_SELCHANGE(IDC_SELECT_UCN_INVOLVEMENT, InvolvementCBNSelChange),
//{{TSelectUCNBaseDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSelectUCNBaseDialog Implementation}}

TSelectUCNBaseDialog::TSelectUCNBaseDialog(TWindow* parent, TFoundGroup *ucnGroup, const char *tUCNTypes,
	const char *tInvolvements, bool active, bool stat, int resId)
:
	TGroupDialog(parent, ucnGroup, resId), ucnTypes(tUCNTypes), involvements(tInvolvements), name(NULL), lastUXNType('\0')
{
	*uxnTypes = '\0';
	for (const char *s = ucnTypes; *s; s++)
		strcach(uxnTypes, ucn2uxn(*s));
	uxnType = new TCharAliasFace(this, IDC_SELECT_UCN_UXN_TYPE, "F_UCN_TYPE", UXNType, uxnTypes);
	ucn = new TUCNFace(this, IDC_SELECT_UCN_UCN, "F_UCN", uxnType);
	citizenName = new TCitizenNameFace(this, IDC_SELECT_UCN_CITIZEN_NAME, "F_CITIZEN_NAME", SIZE_OF_CITIZEN_NAME);
	reName = new TCitizenNameFace(this, IDC_SELECT_UCN_RENAME, "F_RENAME", SIZE_OF_CITIZEN_RENAME);
	family = new TCitizenNameFace(this, IDC_SELECT_UCN_FAMILY, "F_FAMILY", SIZE_OF_CITIZEN_FAMILY);
	groups = new TSelectUCNListBox(this, IDC_SELECT_UCN_GROUPS, ucnTypes, active, stat);
	firmName = new TFirmNameFace(this, IDC_SELECT_UCN_FIRM_NAME, "F_FIRM_NAME", SIZE_OF_FIRM_NAME);
	damnName = new TDamnNameFace(this, IDC_SELECT_UCN_DAMN_NAME, "F_DAMN_NAME", SIZE_OF_DAMN_NAME);
	senderName = new TSenderNameFace(this, IDC_SELECT_UCN_SENDER_NAME, "F_SENDER_NAME", SIZE_OF_NAME);
	if (involvements == NULL)
	{
		involvement = NULL;
		fromSide = NULL;
		fromSideLabel = NULL;
	}
	else
	{
		involvement = new TCharAliasFace(this, IDC_SELECT_UCN_INVOLVEMENT, "F_INVOLVEMENT", Involvement, involvements);
		fromSide = new TCharAliasFace(this, IDC_SELECT_UCN_FROM_SIDE, "F_FROM_SIDE", Involvement, INVOLVEMENT_FROM_SIDES);
		fromSideLabel = new TStatic(this, IDC_SELECT_UCN_FROM_SIDE_LABEL);
	}
	ok = new TPushButton(this, IDOK);
	search = new TPushButton(this, IDSEARCH);
	research = new TButton(this, IDC_SELECT_UCN_RESEARCH);
	reNameLabel = new TStatic(this, IDC_SELECT_UCN_RENAME_LABEL);
	familyLabel = new TStatic(this, IDC_SELECT_UCN_FAMILY_LABEL);
	// 2004:253 LPR: bulstat support
	bulstatLabel = new TStatic(this, IDC_SELECT_UCN_BULSTAT_LABEL);
	bulstat = new TBulStatFace(this, IDC_SELECT_UCN_BULSTAT, "F_BULSTAT");
	// 2012:214 LPR: lnc support
	lncLabel = new TStatic(this, IDC_SELECT_UCN_LNC_LABEL);
	lnc = new TLNCFace(this, IDC_SELECT_UCN_LNC, "F_LNC");
}

bool TSelectUCNBaseDialog::Search(bool extra)
{
	TNamedGroup *const ucnGroup = TNamedGroup::NewXGroup(lastUXNType);
	TVirtPtr VP(ucnGroup);

	*ucnGroup << this;
	ucnGroup->nameField->SetS(name->S());

	bool valid = ucnGroup->ValidCriteria();
	char tInvolvement = involvement ? involvement->C() : INVOLVEMENT_INTERNAL;

	if (valid)
	{
		if (lastUXNType == UXN_CITIZEN)
		{
			groups->SetUCNGroup(ucnGroup, extra, tInvolvement);

			TOldCitizen oldCitizen;
			oldCitizen << this;
			// 2004:182 VRQ: BUGFIX: was missing
			oldCitizen.nameField->SetS(name->S());

			if (oldCitizen.ValidCriteria())
				groups->AddUCNGroup(&oldCitizen, extra, tInvolvement);
		}
		else if (lastUXNType == UXN_FIRM)
		{
			TOldFirm oldFirm;

			groups->SetUCNGroup(ucnGroup, extra, tInvolvement);
			oldFirm << this;
			oldFirm.nameField->SetS(name->S());

			if (oldFirm.ValidCriteria())
				groups->AddUCNGroup(&oldFirm, extra, tInvolvement);
		}
		else
		{
			// 2009:175 LPR: ucnType for senders, ignored for others
			ucnGroup->ucnType = *ucnTypes;
			// 2010:309 FIX: involvement for known firms, ignored for others
			groups->SetUCNGroup(ucnGroup, extra, tInvolvement);
			// 2016:270 handle 2+ types for country exec lists 
			for (const char *tUCNType = ucnTypes + 1; *tUCNType; tUCNType++)
			{
				ucnGroup->ucnType = *tUCNType;
				ucnGroup->nameField->SetS(name->S());
				groups->AddUCNGroup(ucnGroup, extra, tInvolvement);
			}
		}
	}
	else
		error("Критерия за търсене е невалиден или прекалено общ. Коригирайте го.");

	return valid;
}

bool TSelectUCNBaseDialog::UCNAvailable()
{
	return uxnType->C() != UXN_SENDER;
}

void TSelectUCNBaseDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	uxnType->SetC(*uxnTypes);
	UXNTypeCBNSelChange();
	// 2004:166 these are always used together
	if (involvement != NULL)
	{
		// 2004:336 LPR: disabled in involvement change -> needed here
		// 2004:372 LPR/LRQ/BUGFIX: COURT_CODE -> COURT_AREA +related
		// 2004:287 LPR: lock involvement/fromSide if 1 involvement
		involvement->EnableWindow(strlen(involvements) > 1);
		InvolvementCBNSelChange();
	}
}

void TSelectUCNBaseDialog::PrintBNClicked()
{
	groups->Print(lastUXNType);
}

void TSelectUCNBaseDialog::UXNTypeCBNSelChange()
{
	char tUXNType = uxnType->C();

	if (tUXNType != lastUXNType)
	{
		mstr title;
		bool citizen = false, firm = false;

		if (name)
		{
			name->EnableWindow(false);
			name->ShowWindow(SW_HIDE);
		}

		lastUXNType = tUXNType;

		switch (lastUXNType)
		{
			case UXN_CITIZEN :
				name = citizenName;
				citizen = true;
				break;
			case UXN_FIRM :
				name = firmName;
				firm = true;
				break;
			case UXN_DAMN : name = damnName; break;
			case UXN_UNKNOWN :
			case UXN_ANONYMOUS :
			case UXN_SENDER : name = senderName; break;
			default : fatal("%d: uxnType not supported by select_ucn", lastUXNType);
		}

		title.printf("Избор на %s", UXNType->Seek(lastUXNType));
		strlwr(ncstr(title) + 1);
		SetCaption(str(title));

		name->ShowWindow(SW_SHOW);
		name->EnableWindow(true);
		reName->ShowWindow(citizen ? SW_SHOW : SW_HIDE);
		reName->EnableWindow(citizen);
		reNameLabel->ShowWindow(citizen ? SW_SHOW : SW_HIDE);
		family->ShowWindow(citizen ? SW_SHOW : SW_HIDE);
		family->EnableWindow(citizen);
		familyLabel->ShowWindow(citizen ? SW_SHOW : SW_HIDE);
		// 2004:253 LPR: bulstat support
		bulstat->ShowWindow(firm ? SW_SHOW : SW_HIDE);
		bulstat->EnableWindow(firm);
		bulstatLabel->ShowWindow(firm ? SW_SHOW : SW_HIDE);
		// 2012:214 LPR: lnc support
		lnc->ShowWindow(citizen ? SW_SHOW : SW_HIDE);
		lnc->EnableWindow(citizen);
		lncLabel->ShowWindow(citizen ? SW_SHOW : SW_HIDE);
		// 2004:254 VRQ/LPR: clear the list
		groups->FlushList();
		ucn->SetS("");
		// 2016:070 LPR: no search / add with code
		ucn->EnableWindow(UCNAvailable());
	}
}

void TSelectUCNBaseDialog::InvolvementCBNSelChange()
{
	bool fromable = strchr(INVOLVEMENT_FROMABLE_SIDES, involvement->C()) != NULL;

	fromSide->EnableWindow(fromable && strlen(involvements) > 1);
	fromSide->ShowWindow(fromable ? SW_SHOW : SW_HIDE);
	if (!fromable)
		fromSide->SetC(INVOLVEMENT_FROM_NULL);
	fromSideLabel->EnableWindow(fromable);
	fromSideLabel->ShowWindow(fromable ? SW_SHOW : SW_HIDE);
}
