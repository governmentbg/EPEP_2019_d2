DEFINE_RESPONSE_TABLE1(TGetMoveJudicialActDialog, TGetGroupDialog)
//{{TGetMoveJudicialActDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_GET_GROUP_TYPE, TypeCBNSelChange),
	EV_CBN_SELCHANGE(IDC_GET_MOVE_JUDICIAL_ACT_SESSION_KIND, SessionKindCBNSelChange),
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TGetMoveJudicialActDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TGetMoveJudicialActDialog Implementation}}

TGetMoveJudicialActDialog::TGetMoveJudicialActDialog(TWindow* parent, TDecision &source, TSession &tSession)
:
	TGetGroupDialog(parent, &source, NULL, TYPE_LAWSUITS, KIND_ENDOCS, NULL, not_appealed_act, 0,
		IDD_GET_MOVE_JUDICIAL_ACT), session(tSession)
{
	sessionKind = new TCharAliasFace(this, IDC_GET_MOVE_JUDICIAL_ACT_SESSION_KIND, "F_SESSION_KIND", Kind, KIND_SESSIONS);
	sessionDate = new TDateFace(this, IDC_GET_MOVE_JUDICIAL_ACT_SESSION_DATE, "F_SESSION_DATE");
	sessionIndex = new TIndexFace(this, IDC_GET_MOVE_JUDICIAL_ACT_SESSION_INDEX, "F_SESSION_INDEX", KIND_CLOSED_SESSIONS,
		SIZE_OF_SESSION_INDEX);
}

void TGetMoveJudicialActDialog::SetupWindow()
{
	TGetGroupDialog::SetupWindow();

	TypeCBNSelChange();
}

bool TGetMoveJudicialActDialog::IsValid()
{
	bool isValid = TGetGroupDialog::IsValid();

	if (isValid)
	{
		TLawsuit lawsuit;
		mstr decisionKinds;
		char tKind = kind->C();

		lawsuit << this;
		session << this;
		session.kind = adjust_xxx_kind(sessionKind->C(), sessionIndex, KIND_CLOSED_SESSIONS);
		session.date = sessionDate->Date();

		if (!lawsuit.Get("F_KIND, F_SOURCE_KIND"))
			isValid = false;
		else if (!session.Get())
			isValid = false;
		else if (!strchr(session.DecisionKinds(decisionKinds, &lawsuit), tKind))
			isValid = error("Заседанието не допуска съдебен акт \"%s\".", Kind->Seek(tKind));
		else if (session.date != Source()->date)
		{
			TDecision tTarget;

			tTarget.key = lawsuit.key;
			tTarget.kind = tKind;
			tTarget.date = session.date;

			if (tTarget.Try("F_NO"))
			{
				TKind rKind;

				isValid = error("По %s от %Dг. вече има %s.", Kind->Seek(session.kind), &tTarget.date,
					rKind.Seek(tKind));
			}
		}
	}

	return isValid;
}

void TGetMoveJudicialActDialog::TypeCBNSelChange()
{
	TGetGroupDialog::TypeCBNSelChange();

	sesKinds.cpy(type2ses_kinds(type->C()));
	sesKinds.cat(KIND_VISIBLE_CLOSEDS);
	sessionKind->SetXCrit(str(sesKinds));
	SessionKindCBNSelChange();
}

void TGetMoveJudicialActDialog::SessionKindCBNSelChange()
{
	adjust_xxx_index(sessionKind->C(), sessionIndex, KIND_CLOSED_SESSIONS);
}
