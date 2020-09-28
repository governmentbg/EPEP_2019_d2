//{{TFirmAcceptActDialog Implementation}}

TFirmAcceptActDialog::TFirmAcceptActDialog(TWindow* parent, TDecision *decision, TLawsuit *lawsuit, const char *kinds,
	TSession *session)
:
	TJudicialActDialog(parent, decision, lawsuit, kinds, false, session, IDD_FIRM_ACCEPT_ACT)
{
	enterDate = new TDateFace(this, IDC_FIRM_ACCEPT_ACT_ENTER_DATE, "F_ENTER_DATE");
	registry = new TLongFace(this, IDC_FIRM_ACCEPT_ACT_REGISTER, "F_REGISTER", SIZE_OF_DAMN_REGISTER);
	level = new TLongFace(this, IDC_FIRM_ACCEPT_ACT_LEVEL, "F_LEVEL", SIZE_OF_DAMN_LEVEL);
	contract = new TCheckFace(this, IDC_FIRM_ACCEPT_ACT_CONTRACT, "F_FLAGS_CONTRACT");
}

void TFirmAcceptActDialog::ResultChanged()
{
	TJudicialActDialog::ResultChanged();

	bool enable = strchr(RESULT_FIRM_ACCEPTS, result->C());

	//enterDate->EnableWindow(enable);
	registry->EnableWindow(enable);
	volume->EnableWindow(enable);
	page->EnableWindow(enable);
	partition->EnableWindow(enable);
	level->EnableWindow(enable);

	if (enable)
	{
		// 2005:185 LPR: regression restore
		registry->SetL(Decision()->registry);
		volume->SetL(Decision()->volume);
		page->SetL(Decision()->page);
		partition->SetL(Decision()->partition);
		level->SetL(Decision()->level);
		//if (enterDate->Date().Empty())
		//	enterDate->SetDate(Today);
	}
	else
	{
		// 2002:167
		registry->SetL(0);
		volume->SetL(0);
		page->SetL(0);
		partition->SetL(0);
		level->SetL(0);
		//enterDate->SetDate(Today);	// 2006:096 LRQ: comout
	}
}

void TFirmAcceptActDialog::SetupWindow()
{
	TJudicialActDialog::SetupWindow();

	// 2005:150 LRQ: not for edit, we have a date in existing acts
	if (!edit)
	{
		// the default init with today may be lost at this point
		enterDate->SetDate(Today);
	}
}

bool TFirmAcceptActDialog::IsValid()
{
	bool isValid = TJudicialActDialog::IsValid();

	if (isValid)
	{
		if (enterDate->Date().Empty())
			isValid = error("Датата на вписване в ТР е невалидна.");
		else if (strchr(RESULT_FIRM_ACCEPTS, result->C()) && (!registry->L() || !volume->L() || !page->L() || !partition->L() || !level->L()))
			isValid = error("Данните за вписване в ТР са невалидни.");
	}

	return isValid;
}
