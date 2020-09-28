DEFINE_RESPONSE_TABLE1(TInRegNewBaseDialog, TEditIncomingDialog)
//{{TInRegNewBaseDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDQUERY, QueryBNEnable),
	EV_BN_CLICKED(IDQUERY, QueryBNClicked),
//{{TInRegNewBaseDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TInRegNewDialog Implementation}}

TInRegNewBaseDialog::TInRegNewBaseDialog(TWindow* parent, TInReg *inReg, const char *kinds, const char *surroundKinds,
	const char *sessionKinds, const TLawsuit *tLawsuit)
:
	TEditIncomingDialog(parent, inReg, kinds, Default->xColleges, false, IDD_INREG_NEW), lawsuit(tLawsuit)
{
	text = new TStringFace(this, IDC_INREG_NEW_TEXT, "F_TEXT", SIZE_OF_ORIGIN_TEXT);
	lawsuitType = new TCharAliasFace(this, IDC_INREG_NEW_LAWSUIT_TYPE, "F_LAWSUIT_TYPE", Type, TYPE_LAWSUITS, ALIAS_EMPTY);
	lawsuitNo = new TLongFace(this, IDC_INREG_NEW_LAWSUIT_NO, "F_LAWSUIT_NO", SIZE_OF_NO);
	lawsuitYear = new TYearFace(this, IDC_INREG_NEW_LAWSUIT_YEAR, "F_LAWSUIT_YEAR");
	sessionKind = new TCharAliasFace(this, IDC_INREG_NEW_SESSION_KIND, "F_SESSION_KIND", Kind, sessionKinds, ALIAS_EMPTY);
	sessionDate = new TDateFace(this, IDC_INREG_NEW_SESSION_DATE, "F_SESSION_DATE", false);
	sessionIndex = new TIndexFace(this, IDC_INREG_NEW_SESSION_INDEX, "F_SESSION_INDEX", KIND_CLOSED_SESSIONS,
		SIZE_OF_SESSION_INDEX);
	decisionKind = new TCharAliasFace(this, IDC_INREG_NEW_DECISION_KIND, "F_DECISION_KIND", Kind, KIND_IN_APPEALABLES,
		ALIAS_EMPTY);
	decisionNo = new TLongFace(this, IDC_INREG_NEW_DECISION_NO, "F_DECISION_NO", SIZE_OF_NO);
	surroundKind = new TCharAliasFace(this, IDC_INREG_NEW_SURROUND_KIND, "F_SURROUND_KIND", Kind, surroundKinds,
		ALIAS_EMPTY);
	surroundNo = new TLongFace(this, IDC_INREG_NEW_SURROUND_NO, "F_SURROUND_NO", SIZE_OF_NO);
	surroundYear = new TYearFace(this, IDC_INREG_NEW_SURROUND_YEAR, "F_SURROUND_YEAR");
	query = new TPushButton(this, IDQUERY);
	receiver = new TSenderAliasFace(this, IDC_INREG_NEW_RECEIVER, "F_RECEIVER", UCN_INSTANCE_RESERVED, ALIAS_OPTION);
	lawsuitTypeLabel = new TStatic(this, IDC_INREG_NEW_LAWSUIT_TYPE_LABEL);
	lawsuitNoLabel = new TStatic(this, IDC_INREG_NEW_LAWSUIT_NO_LABEL);
	lawsuitYearLabel = new TStatic(this, IDC_INREG_NEW_LAWSUIT_YEAR_LABEL);
	collegeLabel = new TStatic(this, IDC_INREG_NEW_LAWSUIT_COLLEGE_LABEL);
}

void TInRegNewBaseDialog::SetReceivers()
{
	if (lawsuit->key.type)
	{
		TInReg inReg;
		TFixeiver fixeiver(UCN_INSTANCE_HIGHER);
		char tReceiver[SIZE_OF_UCN];

		inReg.decision.type = inReg.lawsuit.type = lawsuit->key.type;
		inReg.kind = kind->C();
		inReg.decisionKind = decisionKind->C();

		if (fill_appeal_fixeivers(&inReg, fixeiver, tReceiver))
		{
			// partial copy from OutRegSendDialog
		#if COURT_TYPE == COURT_ADMIN
			if (inReg.kind == KIND_INREG_SLOWDOWN)
				strcpy(tReceiver, INSTANCE_SPEAL);
		#endif  // ADMIN

			// copy from OutRegSendDialog
		#if COURT_TYPE == COURT_REGIONAL
			if (lawsuit->key.type == TYPE_PUNISHMENT_LAW)
			{
				Subject->Seek(TYPE_PUNISHMENT_LAW, lawsuit->subject);

				if (strchr(Subject->sd[0].attrib, POST_SEND2ADMIN))
					strcpy(tReceiver, INSTANCE_ADMIN);
			}
		#endif  // REGIONAL
			receiver->SetS(tReceiver);
		}
	}
}

void TInRegNewBaseDialog::SetupWindow()
{
	TEditIncomingDialog::SetupWindow();

	set_direction(lawsuit->key, direction);	// 2016:083 moved in setup
	// 2016:083
	if (lawsuit->key.type)
	{
		// 2007:243 LRQ: don't check TEXTUAL
		//if (!strchr(KIND_TEXTUAL_IN_DOCS, kind->C()))
			college->SetC(lawsuit->key.type);
		college->EnableWindow(false);
		college->ShowWindow(SW_HIDE);
		collegeLabel->ShowWindow(SW_HIDE);
	}
	else
	{
		lawsuitType->ShowWindow(SW_HIDE);
		lawsuitNo->ShowWindow(SW_HIDE);
		lawsuitYear->ShowWindow(SW_HIDE);
		lawsuitTypeLabel->ShowWindow(SW_HIDE);
		lawsuitNoLabel->ShowWindow(SW_HIDE);
		lawsuitYearLabel->ShowWindow(SW_HIDE);
		//query->ShowWindow(SW_HIDE);
	}
}

void TInRegNewBaseDialog::QueryBNEnable(TCommandEnabler &tce)
{
	tce.Enable(lawsuit->key.type);
}

void TInRegNewBaseDialog::QueryBNClicked()
{
	query_group(this, lawsuit);
}
