DEFINE_RESPONSE_TABLE1(TDescBookDialog, TBookInstanceDialog)
	EV_CBN_SELCHANGE(IDC_BOOK_TYPES, TypeBNSelChange),
	EV_BN_CLICKED(IDC_DESC_BOOK_NEW_FORMAT, NewFormatBNClicked),
	EV_BN_CLICKED(IDC_BOOK_INSTANCE_1ST, Instance1stBNClicked),
END_RESPONSE_TABLE;

TDescBookDialog::TDescBookDialog(TWindow* parent, TDescBookGroup *group)
	: TBookInstanceDialog(parent, group, Type, IDD_DESC_BOOK, true)
{
	compositions = new TCharAutoListFace(this, IDC_DESC_BOOK_COMPOSITIONS, "F_COMPOSITIONS", CharComposition, QUERY_COMPOSITIONS, SIZE_OF_COMPOSITIONS);
	createBook = new TCheckFace(this, IDC_DESC_BOOK_CREATE_BOOK, "F_CREATE_BOOK");
	reCap = new TCheckFace(this, IDC_DESC_BOOK_CREATE_RECAP, "F_RECAP");
	newFormat = new TCheckFace(this, IDC_DESC_BOOK_NEW_FORMAT, "F_NEW_FORMAT");
	lawFormatLabel = new TStatic(this, IDC_DESC_BOOK_LAW_FORMAT_LABEL);
	adminLawFormat = new TRadioFace(this, IDC_DESC_BOOK_ADMIN_LAW, "F_ADMIN_LAW");
	citizenLawFromat = new TRadioFace(this, IDC_DESC_BOOK_CITIZEN_LAW, "F_CITIZEN_LAW");
	citizenLawUnitedFromat = new TRadioFace(this, IDC_DESC_BOOK_CITIZEN_LAW_UNITED, "F_CITIZEN_LAW_UNITED");
	eventText = new TCheckFace(this, IDC_DESC_BOOK_EVENT_TEXT, "F_EVENT_TEXT");
}

bool TDescBookDialog::IsValid()
{
	bool isValid = TBookInstanceDialog::IsValid();

#if COURT_TYPE == COURT_ADMIN
	if (isValid)
		if (adminLawFormat->GetCheck() == BF_UNCHECKED && citizenLawFromat->GetCheck() == BF_UNCHECKED && citizenLawUnitedFromat->GetCheck() == BF_UNCHECKED && type->C() == TYPE_ADMIN_LAW && !COURT_LOCK)
			isValid = error("Моля, изберете \"Формат\".");
#endif // ADMIN

	return isValid;
}

void TDescBookDialog::Action(bool execute)
{
	if (execute && (REPEAT_BOOKS_A & REPEAT_BOOK_A_DESC))
		desc_book(this, (TDescBookGroup *) Group());
	else
		TFloatDialog::Action(execute);
}

void TDescBookDialog::SetupWindow()
{
	TBookInstanceDialog::SetupWindow();

#if COURT_TYPE == COURT_AREAL
	TypeBNSelChange();
#endif

#if COURT_TYPE == COURT_ADMIN
	lawFormatLabel->ShowWindow(SW_SHOW);
	adminLawFormat->ShowWindow(SW_SHOW);
	citizenLawUnitedFromat->ShowWindow(SW_SHOW);
	citizenLawFromat->ShowWindow(SW_SHOW);

	if (type->C() == TYPE_ADMIN_LAW && !COURT_LOCK)
	{
		lawFormatLabel->EnableWindow(true);
		adminLawFormat->EnableWindow(true);
		citizenLawUnitedFromat->EnableWindow(true);
		citizenLawFromat->EnableWindow(true);
	}

	adminLawFormat->SetCheck(BF_UNCHECKED);
	citizenLawUnitedFromat->SetCheck(BF_UNCHECKED);
	citizenLawFromat->SetCheck(BF_UNCHECKED);
#endif

	newFormat->EnableWindow(false);
	createBook->SetCheck(BF_CHECKED);

#if COURT_TYPE == COURT_APPEAL
	reCap->EnableWindow(false);
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	reCap->SetCheck(BF_CHECKED);
#endif  // ADMIN || AREAL || MILITARY || REGIONAL

	Instance1stBNClicked();
}

void TDescBookDialog::TypeBNSelChange()
{
#if COURT_TYPE == COURT_AREAL
	newFormat->SetCheck(type->C() == TYPE_FIRM_LAW ? BF_UNCHECKED : BF_CHECKED);
	NewFormatBNClicked();
#endif // AREAL

#if COURT_TYPE == COURT_ADMIN
	if (!COURT_LOCK)
	{
		if (type->C() == TYPE_ADMIN_LAW)
		{
			lawFormatLabel->EnableWindow(true);
			adminLawFormat->EnableWindow(true);
			citizenLawUnitedFromat->EnableWindow(true);
			citizenLawFromat->EnableWindow(true);
		}
		else
		{
			lawFormatLabel->EnableWindow(false);
			adminLawFormat->EnableWindow(false);
			citizenLawUnitedFromat->EnableWindow(false);
			citizenLawFromat->EnableWindow(false);
		}
	}

	adminLawFormat->SetCheck(BF_UNCHECKED);
	citizenLawUnitedFromat->SetCheck(BF_UNCHECKED);
	citizenLawFromat->SetCheck(BF_UNCHECKED);
#endif // ADMIN
}

void TDescBookDialog::NewFormatBNClicked()
{
	if (newFormat->GetCheck() == BF_UNCHECKED)
	{
		instance_2nd->SetCheck(BF_UNCHECKED);
		instance_2nd->EnableWindow(false);
	}
	else
	{
		instance_2nd->SetCheck(BF_CHECKED);
		instance_2nd->EnableWindow(true);
	}
}

void TDescBookDialog::Instance1stBNClicked()
{
	if (instance_1st->GetCheck() == BF_CHECKED)
		eventText->EnableWindow(true);
	else
	{
		eventText->EnableWindow(false);
		eventText->SetCheck(BF_UNCHECKED);
	}
}
