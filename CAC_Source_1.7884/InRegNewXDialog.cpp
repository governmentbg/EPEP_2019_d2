DEFINE_RESPONSE_TABLE1(TInRegNewXDialog, TEditIncomingDialog)
//{{TInRegNewXDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_EDIT_INCOMING_KIND, KindCBNSelChange),
	EV_CBN_SELCHANGE(IDC_INREG_NEW_X_LAWSUIT_TYPE, LawsuitTypeCBNSelChange),
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_CBN_SELCHANGE(IDC_INREG_NEW_X_DECISION_KIND, DecisionKindCBNSelChange),
	EV_EN_KILLFOCUS(IDC_INREG_NEW_X_DECISION_NO, DecisionNoKillFocus),
	EV_EN_KILLFOCUS(IDC_INREG_NEW_X_DECISION_YEAR, DecisionYearKillFocus),
//{{TInRegNewXDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TInRegNewXDialog Implementation}}

TInRegNewXDialog::TInRegNewXDialog(TWindow* parent, TInReg *inReg, const char *kinds, int resId)
:
	TEditIncomingDialog(parent, inReg, kinds, Default->colleges, false, resId, ALIAS_EMPTY)
{
	build_ctypes(types, TYPE_ORIGINS, true);
	lawsuitType = new TCharAliasFace(this, IDC_INREG_NEW_X_LAWSUIT_TYPE, "F_LAWSUIT_TYPE", Type, types);
	lawsuitNo = new TLongFace(this, IDC_INREG_NEW_X_LAWSUIT_NO, "F_LAWSUIT_NO", SIZE_OF_NO);
	lawsuitYear = new TYearFace(this, IDC_INREG_NEW_X_LAWSUIT_YEAR, "F_LAWSUIT_YEAR");
	sessionDate = new TDateFace(this, IDC_INREG_NEW_X_SESSION_DATE, "F_SESSION_DATE");
	decisionKind = new TCharAliasFace(this, IDC_INREG_NEW_X_DECISION_KIND, "F_DECISION_KIND", Kind, KIND_IN_APPEALABLES,
		ALIAS_EMPTY);
	decisionNo = new TLongFace(this, IDC_INREG_NEW_X_DECISION_NO, "F_DECISION_NO", SIZE_OF_NO);
	decisionYear = new TYearFace(this, IDC_INREG_NEW_X_DECISION_YEAR, "F_DECISION_YEAR");
}

void TInRegNewXDialog::OKBNClicked()
{
	if (IsValid())
	{
		*InReg() << this;
		InReg()->decisionDate = sessionDate->Date();

		if (decisionKind->C())
			InReg()->decision.type = kind2type(decisionKind->C());
		else
		{
			InReg()->decision.type = TYPE_REQUEST;
			InReg()->decisionKind = KIND_REQUEST_RESOLUTION;
		}

		if (InReg()->Insert())
		{
			trouble.Move(Incoming(), date->Date());
			CmOk();
		}
	}
}

void TInRegNewXDialog::SetupWindow()
{
	TEditIncomingDialog::SetupWindow();

	LawsuitTypeCBNSelChange();
	sessionDate->SetS("");
}

bool TInRegNewXDialog::IsValid()
{
	bool isValid = TEditIncomingDialog::IsValid();

	if (isValid)
	{
		int tDecisionCount = (decisionKind->C() != 0) + (decisionNo->L() != 0) + (decisionYear->L() != 0);
		TInReg inReg;
		TInRegEvent inRegEvent;
		TRequestResolution resolution;

		if (tDecisionCount != 0 && tDecisionCount != 3)
			isValid = error("Полетата разпореждане по документ, номер и година не са комплектни.");
		else if (strchr(TYPE_LAWSUITS, lawsuitType->C()) != NULL && tDecisionCount != 3)
			isValid = error("Полетата вид дело и разпореждане по документ не си съответстват.");
		else if
		(
			tDecisionCount == 3 &&
			(
				inReg.key.no = decisionNo->L(),
				inReg.key.year = decisionYear->L(),
				(
					!inReg.Try() ||
					inReg.lawsuit.type != lawsuitType->C() ||
					inReg.kind != decisionKind->C()
				)
			)
		)
			isValid = say_no_data(inReg.Name);
		else if
		(
			tDecisionCount == 3 &&
			(
				inRegEvent.key = inReg.key,
				inRegEvent.date = sessionDate->Date(),
				!inRegEvent.Try()
			)
		)
			isValid = say_no_data(inRegEvent.Name);
		else if (tDecisionCount == 3 && !strchr(RESULT_APPEALABLES, inRegEvent.result))
			isValid = error("Разпореждането не отговаря на необходимите критерии.");
		else if
		(
			tDecisionCount == 0 &&
			(
				resolution.key.no = lawsuitNo->L(),
				resolution.key.year = lawsuitYear->L(),
				resolution.key.type = lawsuitType->C(),
				resolution.kind = KIND_REQUEST_RESOLUTION,
				resolution.date = sessionDate->Date(),
				!resolution.Try()
			)
		)
			isValid = say_no_data(resolution.Name);
		else if (tDecisionCount == 0 && !strchr(RESOLUTION_APPEALABLES, resolution.resolution))
			isValid = error("Разпореждането не отговаря на необходимите критерии.");
		// 2001:092 To avoid re-reading in OK; 2001:099 And for checks
		// 2006:053 BUGFIX? "else" next line prefixed with "else"
		else if (tDecisionCount == 3)
		{
			strcpy(InReg()->judge, inRegEvent.judge);
			lawsuitNo->SetL(inReg.lawsuit.no);
			lawsuitYear->SetL(inReg.lawsuit.year);

			if (!obtain_appeal_instance(kind2type(decisionKind->C()), kind->C(), decisionKind->C()))
				isValid = error("Полетата вид документ и разпореждане по документ не си съответстват.");
		}
		else if (tDecisionCount == 0)
		{
			strcpy(InReg()->judge, resolution.judge);
			decisionKind->SetXCrit(KIND_INDOCS);
			decisionKind->SetC(resolution.kind);
			college->SetXCrit(COLLEGE_ALLS);

			TRequest request;

			request.key = resolution.key;
			request.Get("F_COLLEGE");
			college->SetC(request.college);

			if (!obtain_appeal_instance(TYPE_REQUEST, kind->C(), KIND_REQUEST_RESOLUTION))
				isValid = error("Полетата вид документ и разпореждане по документ не си съответстват.");
		}
		// 2002:028 PRQ: an attempt to protect archived whatever
		else if (tDecisionCount == 3 && !can_access_exact(&inReg, true, true, NULL))
			isValid = false;
		else
			isValid = CheckDirection();

		if (!isValid)
		{
			if (tDecisionCount == 3)
			{
				lawsuitNo->SetL(0);
				lawsuitYear->SetL(0);
			}
			else if (tDecisionCount == 0)
			{
				decisionKind->SetXCrit(KIND_IN_APPEALABLES);
				decisionKind->SetC('\0');
				// 2001:109
				college->SetXCrit(Default->colleges);
				college->SetC('\0');
			}
		}
	}

	return isValid;
}

void TInRegNewXDialog::KindCBNSelChange()
{
	if (strchr(TYPE_LAWSUITS, lawsuitType->C()))
		adjust_appeal_decisions(TYPE_INREG, kind, decisionKind);
}

void TInRegNewXDialog::LawsuitTypeCBNSelChange()
{
	if (strchr(TYPE_LAWSUITS, lawsuitType->C()))
	{
		decisionKind->EnableWindow(true);
		decisionNo->EnableWindow(true);
		decisionYear->EnableWindow(true);
		college->SetC(lawsuitType->C());
		lawsuitNo->EnableWindow(false);
		lawsuitNo->SetL(0);
		lawsuitYear->EnableWindow(false);
		lawsuitYear->SetL(0);
	}
	else
	{
		decisionKind->SetC('\0');
		decisionKind->EnableWindow(false);
		decisionNo->EnableWindow(false);
		decisionNo->SetL(0);
		decisionYear->EnableWindow(false);
		decisionYear->SetL(0);
		college->SetC('\0');
		lawsuitNo->EnableWindow(true);
		lawsuitYear->EnableWindow(true);
	}

	KindCBNSelChange();
}

void TInRegNewXDialog::SetDirection()
{
	TInReg inReg;

	inReg.key.no = decisionNo->L();
	inReg.key.year = decisionYear->L();

	if (decisionKind->C() && inReg.key.no && inReg.key.year && inReg.Try())
	{
		TRCDKey origin;
		
		if (find_origin(&inReg, origin) && strchr(TYPE_LAWSUITS, origin.type))
			set_direction(origin, direction);
	}
}

void TInRegNewXDialog::DecisionKindCBNSelChange()
{
	SetDirection();
}

void TInRegNewXDialog::DecisionNoKillFocus()
{
	SetDirection();
}

void TInRegNewXDialog::DecisionYearKillFocus()
{
	SetDirection();
}
