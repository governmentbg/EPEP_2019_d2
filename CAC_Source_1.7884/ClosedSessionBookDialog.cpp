DEFINE_RESPONSE_TABLE1(TClosedSessionBookDialog, TFloatDialog)
	EV_BN_CLICKED(IDC_CLOSED_SESSION_BOOK_ALL, AllResultsBNClicked),
	EV_BN_CLICKED(IDC_CLOSED_SESSION_BOOK_CREATE, CreateBookBNClicked),
	EV_COMMAND_ENABLE(IDOK, OKBNEnable),
	EV_CBN_SELCHANGE(IDC_CLOSED_SESSION_BOOK_TYPE, TypeCBNSelChange),
	EV_BN_CLICKED(IDC_CLOSED_SESSION_BOOK_RECAP, CreateReCapBNClicked),
	EV_BN_CLICKED(IDC_CLOSED_SESSION_BOOK_RESULTS, CreateResultsBNClicked),
	EV_BN_CLICKED(IDC_CLOSED_SESSION_BOOK_BY_RETURNED, ByReturnedBNClicked),
END_RESPONSE_TABLE;

TClosedSessionBookDialog::TClosedSessionBookDialog(TWindow* parent, TClosedSessionBookGroup *group)
	: TFloatDialog(parent, group, IDD_CLOSED_SESSION_BOOK)
{
	build_ctypes(types, TYPE_LAWSUITS, true);
	type = new TCharAliasFace(this, IDC_CLOSED_SESSION_BOOK_TYPE, "F_TYPE", Type, types);
	minDate = new TDateFace(this, IDC_CLOSED_SESSION_BOOK_MIN_DATE, "F_MIN_DATE", true);
	new TDateFaceUpDown(this, IDC_CLOSED_SESSION_BOOK_MIN_UPDN, minDate);
	maxDate = new TDateFace(this, IDC_CLOSED_SESSION_BOOK_MAX_DATE, "F_MAX_DATE", true);
	new TDateFaceUpDown(this, IDC_CLOSED_SESSION_BOOK_MAX_UPDN, maxDate);
	compositions = new TCharAutoListFace(this, IDC_CLOSED_SESSION_BOOK_COMPOSIT, "F_COMPOSITIONS", CharComposition,
		QUERY_COMPOSITIONS, SIZE_OF_COMPOSITIONS);
	new TLongFace(this, IDC_CLOSED_SESSION_BOOK_SERIAL, "F_AUTO_SERIAL_NO", SIZE_OF_NO);
	allResults = new TCheckFace(this, IDC_CLOSED_SESSION_BOOK_ALL, "F_ALL_RESULTS");
	newFormat = new TCheckFace(this, IDC_CLOSED_SESSION_BOOK_NEW, "F_NEW_FORMAT");
	sessions = new TCheckFace(this, IDC_CLOSED_SESSION_BOOK_SESSIONS, "F_SESSIONS");
	inregs = new TCheckFace(this, IDC_CLOSED_SESSION_BOOK_INREGS, "F_INREGS");
	createBook = new TCheckFace(this, IDC_CLOSED_SESSION_BOOK_CREATE, "F_CREATE_BOOK");
	createReCap = new TCheckFace(this, IDC_CLOSED_SESSION_BOOK_RECAP, "F_RECAP");
	createResults = new TCheckFace(this, IDC_CLOSED_SESSION_BOOK_RESULTS, "F_RESULTS");
	orderByDecisions = new TRadioFace(this, IDC_CLOSED_SESSION_BOOK_DECISION, "F_ORDER_BY_DECISIONS");
	orderByLawsuits = new TRadioFace(this, IDC_CLOSED_SESSION_BOOK_LAWSUIT, "F_ORDER_BY_LAWSUITS");
	commonOutput = new TCheckFace(this, IDC_CLOSED_SESSION_BOOK_COMMON_OUTPUT, "F_COMMON_OUTPUT");
	byReturned = new TPushButton(this, IDC_CLOSED_SESSION_BOOK_BY_RETURNED);
	new TCheckFace(this, IDC_CLOSED_SESSION_BOOK_BY_INTERVAL, "F_BY_INTERVAL");
}

unsigned TClosedSessionBookDialog::CreateCount() const
{
	return (createBook->GetCheck() == BF_CHECKED) + (createReCap->GetCheck() == BF_CHECKED) +
		(createResults->GetCheck() == BF_CHECKED);
}

void TClosedSessionBookDialog::CommonOutputUpdate()
{
	if (CreateCount() >= 2)
		commonOutput->EnableWindow(true);
	else
	{
		commonOutput->SetCheck(BF_UNCHECKED);
		commonOutput->EnableWindow(false);
	}
}

void TClosedSessionBookDialog::ByReturnedUpdate()
{
	static constant CAPTIONS[] = { "Дата на заседание", "Дата на връщане" };
	byReturned->SetCaption(CAPTIONS[ClosedSessionBookGroup()->byReturned]);
}

bool TClosedSessionBookDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (minDate->Date().Empty() || maxDate->Date().Empty())
			isValid = error("Невалидна или непопълнена дата.");
		else if (invalid_range(minDate, maxDate))
			isValid = error("Началната дата е по-голяма от крайната.");
	}

	return isValid;
}

void TClosedSessionBookDialog::Action(bool execute)
{
	if (execute && (REPEAT_BOOKS_A & REPEAT_BOOK_A_CLOSED))
		closed_session_book(this, (TClosedSessionBookGroup *) Group());
	else
		TFloatDialog::Action(execute);
}

void TClosedSessionBookDialog::SetupWindow()
{
	TFloatDialog::SetupWindow();

	type->SetC(kind2type(Default->kind));	// 2018-04-13
	TypeCBNSelChange();
	ByReturnedUpdate();
}

void TClosedSessionBookDialog::CreateBookBNClicked()
{
	orderByDecisions->EnableWindow(createBook->GetCheck() == BF_CHECKED);
	orderByLawsuits->EnableWindow(createBook->GetCheck() == BF_CHECKED);
	CommonOutputUpdate();
}

void TClosedSessionBookDialog::AllResultsBNClicked()
{
	inregs->EnableWindow(allResults->GetCheck() == BF_CHECKED);
	inregs->SetCheck(BF_UNCHECKED);
}

void TClosedSessionBookDialog::OKBNEnable(TCommandEnabler &tce)
{
	tce.Enable(CreateCount() && (sessions->GetCheck() == BF_CHECKED || inregs->GetCheck() == BF_CHECKED));
}

void TClosedSessionBookDialog::TypeCBNSelChange()
{
#if COURT_TYPE == COURT_AREAL
	if (type->C() == TYPE_FIRM_LAW)
	{
		createReCap->EnableWindow(false);
		createReCap->SetCheck(BF_UNCHECKED);
	}
	else
	{
		createReCap->SetCheck(BF_CHECKED);
		createReCap->EnableWindow(true);
	}
#endif  // AREAL
	CommonOutputUpdate();
}

void TClosedSessionBookDialog::CreateReCapBNClicked()
{
	CommonOutputUpdate();
}

void TClosedSessionBookDialog::CreateResultsBNClicked()
{
	CommonOutputUpdate();
}

void TClosedSessionBookDialog::ByReturnedBNClicked()
{
	ClosedSessionBookGroup()->byReturned ^= 1;
	ByReturnedUpdate();
}
