DEFINE_RESPONSE_TABLE1(TMovementDialog, TGroupDialog)
//{{TMovementDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TMovementDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TMovementDialog Implementation}}

TMovementDialog::TMovementDialog(TWindow* parent, TMovement *movement, TRCDKeyGroup *tGroup, int resId)
:
	TGroupDialog(parent, movement, resId), group(tGroup)
{
	new TCharAliasFace(this, IDC_IN_OUT_MOVEMENT_TYPE, "F_TYPE", Type, NULL);
	new TLongFace(this, IDC_IN_OUT_MOVEMENT_NO, "F_NO", SIZE_OF_NO);
	new TYearFace(this, IDC_IN_OUT_MOVEMENT_YEAR, "F_YEAR");
	date = new TDateFace(this, IDC_IN_OUT_MOVEMENT_DATE, "F_DATE");
	text = new TStringFace(this, IDC_IN_OUT_MOVEMENT_TEXT, "F_TEXT", SIZE_OF_MOVEMENT_TEXT);
	time = new TTimeFace(this, IDC_IN_OUT_MOVEMENT_TIME, "F_TIME");
	kind = new TCharAliasFace(this, IDC_IN_OUT_MOVEMENT_KIND, "F_KIND", Kind, KIND_MOVEMENTS);
}

void TMovementDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

#if COURT_TYPE == COURT_ADMIN
	// 2007:281 LPR: variantless dialog resources
	SetCaption("Предоставяне/връщане на документ");
#endif  // ADMIN

	CTime tTime(Now());
	char s[0x20];

	sprintf(s, "%02d:%02d", tTime.hour, tTime.minute);
	time->SetS(s);

	// 2006:016 LPR: unify move in/out
	TMovement *movement = group->LastMove();

	if (movement && movement->kind == KIND_OUT_MOVEMENT)
	{
		text->SetS(movement->text);
		kind->SetC(KIND_IN_MOVEMENT);
	}
	else
	{
		kind->SetC(KIND_OUT_MOVEMENT);
		if (!movement)
			kind->EnableWindow(false);
	}

	delete movement;
}

void TMovementDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Movement() << this;
		if (Movement()->Insert())
			CmOk();
	}
}

bool TMovementDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		TMovement *const movement = group->LastMove(date->Date(), time->Time());
		TVirtPtr VP(movement);
		TRequest request;

		if (kind->C() == KIND_OUT_MOVEMENT && group->key.type == TYPE_REQUEST &&
			(request.key = group->key, request.Get()) && request.Status() == RESOLUTION_LAWED)
		{
			isValid = error("Документа е образуван в дело и следва да се изпраща като част от делото.");
		}
		// 2001:263 LRQ
		else if (date->Date() > Today)
			isValid = error("Датата на на предоставяне/връщане е по-голяма от текущата.");
		else if (movement && movement->kind == kind->C())
			isValid = error("Документа вече е %s.", movement->kind == KIND_OUT_MOVEMENT ? "предоставен" : "върнат");
		else if (!movement && kind->C() == KIND_IN_MOVEMENT)
			isValid = error("Документа не е предоставян.");
		else if (!any(text))
			isValid = error("Текстът е задължителен.");
	}

	return isValid;
}
