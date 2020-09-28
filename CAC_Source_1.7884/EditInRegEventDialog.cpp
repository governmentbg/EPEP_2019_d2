DEFINE_RESPONSE_TABLE1(TEditInRegEventDialog, TTroubleDialog)
//{{TEditInRegEventDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDRIGHT, RightBNClicked),
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TEditInRegEventDialogDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditInRegEventDialog Implementation}}

TEditInRegEventDialog::TEditInRegEventDialog(TWindow* parent, TInReg *tInReg, TInRegEvent *inRegEvent, bool edit,
	TSideWind &tSideWind)
:
	TTroubleDialog(parent, inRegEvent, edit | ATTACH_RIGHT, TYPE_TEXT, IDD_EDIT_INREG_EVENT), inReg(tInReg),
		sideWind(tSideWind), judge(NULL), judgeLabel(NULL)
{
	inRegKind = new TCharAliasFace(this, IDC_EDIT_INREG_EVENT_KIND, "F_INREG_KIND", Kind, NULL);
	new TLongFace(this, IDC_EDIT_INREG_EVENT_NO, "F_NO", SIZE_OF_NO);
	new TYearFace(this, IDC_EDIT_INREG_EVENT_YEAR, "F_YEAR");
	new TCharAliasFace(this, IDC_EDIT_INREG_EVENT_LAWSUIT_TYPE, "F_LAWSUIT_TYPE", Type, NULL, ALIAS_EMPTY);
	new TLongFace(this, IDC_EDIT_INREG_EVENT_LAWSUIT_NO, "F_LAWSUIT_NO", SIZE_OF_NO);
	new TYearFace(this, IDC_EDIT_INREG_EVENT_LAWSUIT_YEAR, "F_LAWSUIT_YEAR");

	const char *results;

	if (inReg->kind == KIND_EPORTAL_REQUEST)
	{
		results = RESULT_EPORTAL_RESOLUTIONS;

		if (inReg->eportalMode == EPORTAL_REGISTER)
		{
			judge = new TEPorUserAliasFace(this, IDC_EDIT_INREG_EVENT_JUDGE, "F_JUDGE",
				edit ? FLAG_ACTIVE : FLAG_NULL);
			judgeLabel = new TStatic(this, IDC_EDIT_INREG_EVENT_JUDGE_LABEL);
		}
	}
	else if (strchr(KIND_TEXTUAL_IN_DOCS, inReg->kind))
		results = RESULT_TEXTUAL_RESOLUTIONS;
	else if (inReg->lawsuit.type == TYPE_PUNISHMENT_LAW)
		results = RESULT_PUNISHMENT_RESOLUTIONS;
	else
		results = RESULT_CITIZALIKE_RESOLUTIONS;

	result = new TCharAliasFace(this, IDC_EDIT_INREG_EVENT_RESULT, "F_RESULT", Result, results);
	eventNo = new TLongFace(this, IDC_EDIT_INREG_EVENT_EVENT_NO, "F_EVENT_NO", SIZE_OF_EVENT_NO);
	date = new TDateFace(this, IDC_EDIT_INREG_EVENT_DATE, "F_DATE");
	if (!judge)
	{
		judge = new TUserAliasFace(this, IDC_EDIT_INREG_EVENT_JUDGE, "F_JUDGE", POST_VISIBLE_JUDGES,
			edit ? ALIAS_EMPTY | USER_INSECT : FLAG_ACTIVE | ALIAS_EMPTY | USER_INSECT);
	}
	returned = new TDateFace(this, IDC_EDIT_INREG_EVENT_RETURNED, "F_RETURNED");
	new TStringFace(this, IDC_EDIT_INREG_EVENT_TEXT, "F_TEXT", SIZE_OF_INREG_EVENT_TEXT);
	index = new TIndexFace(this, IDC_EDIT_INREG_EVENT_INDEX, "F_INDEX", KIND_INREG_USER_EVENTS, SIZE_OF_INREG_INDEX);
}

bool TEditInRegEventDialog::IsEPortalAllowed()
{
	return inReg->kind == KIND_EPORTAL_REQUEST && result->C() == RESULT_EPORTAL_ALLOWED;
}

void TEditInRegEventDialog::SetupWindow()
{
	TTroubleDialog::SetupWindow();

	*inReg >> this;		// for the lawsuit fields
	*InRegEvent() >> this;
	inRegKind->SetC(inReg->kind);

	if (edit)
		index->SetL(kind2index(InRegEvent()->kind, KIND_INREG_USER_EVENTS));
	else
	{
		adjust_xxx_index(InRegEvent()->kind, index, KIND_INREG_USER_EVENTS);
		// 2010:260
		returned->SetDate(date->Date());
		date->SetS("");
		date->EnableWindow(true);
		eventNo->EnableWindow(true);
	}

	returned->EnableWindow(!LOCK_RETURN);

	if (judgeLabel)
		judgeLabel->SetCaption("С&ъдия/длъжностно лице");
}

bool TEditInRegEventDialog::IsValid()
{
	bool isValid = TTroubleDialog::IsValid();

	if (isValid)
	{
		if (date->Date() > Today)
			isValid = error("Датата на разпореждане е по-голяма от текущата дата.");
		else if (date->Date() < inReg->date)
			isValid = error("Датата на разпореждане е по-малка от тази на документа.");
		else if (!atob(judge))
			isValid = error("Съдията е задължителен.");
		else if (returned->Date() > Today)
			isValid = error("Датата на връщане е по-голяма от текущата дата.");
		else if (returned->Date() < date->Date())
			isValid = error("Датата на връщане е по-малка от тази на разпореждането.");
		else if (IsEPortalAllowed())
		{
			if (!XFER_EPORTAL)
				isValid = error("Разпореждането изисква включен обмен с %s.", EPORTAL_NAME);
			else if (inReg->eportalMode == EPORTAL_REGISTER)
			{
				if (already_used(inReg->eportalMail))
					isValid = error("Електронната поща е дублирана.");
			}
			else if (!check_registration(sideWind, sideWind.involvement))
				isValid = error("Лицето няма регистрация в %s, нито одобрена заявка за регистрация.", EPORTAL_NAME);
		}
	}

	return isValid;
}

void TEditInRegEventDialog::RightBNClicked()
{
	if (IsValid())
	{
		attach_right(this, inReg->lawsuit, trouble, InRegEvent()->IndexKind(), date->Date(), judge->S());
		date->EnableWindow(!trouble.act);
		judge->EnableWindow(!trouble.act);
	}
}

void TEditInRegEventDialog::OKBNClicked()
{
	if (!edit)
		adjust_xxx_kind(InRegEvent(), index, KIND_INREG_USER_EVENTS);

	if (IsValid())
	{
		// 2018-10-26: LPR: ask() moved here from IsValid()
		if (!IsEPortalAllowed() || ask("Към %s ще бъде изпратено съобщение за %s. %s",
			EPORTAL_NAME, inReg->EPortalModeText(), Q_CONTINUE))
		{
			TTroubleDialog::OKBNClicked();
		}
	}
}
