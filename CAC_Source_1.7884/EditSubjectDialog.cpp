DEFINE_RESPONSE_TABLE1(TEditSubjectDialog, TGroupDialog)
//{{TEditSubjectDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDC_EDIT_SUBJECT_INDEXES, IndexesBNClicked),
	EV_BN_CLICKED(IDC_EDIT_SUBJECT_REALITIES, RealitiesBNClicked),
	EV_BN_CLICKED(IDEISPP, EisppBNClicked),
	EV_COMMAND_ENABLE(IDEISPP, EisppBNEnable),
//{{TEditSubjectDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditSubjectDialog Implementation}}

TEditSubjectDialog::TEditSubjectDialog(TWindow* parent, TSubject *tSubject, bool tEdit, TGArray<TArchiveIndex> *tIndexes)
:
	TGroupDialog(parent, tSubject, IDD_EDIT_SUBJECT), edit(tEdit), indexes(tIndexes)
{
	backup << tSubject;
	subject = edit ? new TStringFace(this, IDC_EDIT_SUBJECT_SUBJECT, "F_SUBJECT_CODE", SIZE_OF_SUBJECT_CODE_NAME) :
		(TStringFace *) new TLongFace(this, IDC_EDIT_SUBJECT_SUBJECT, "F_SUBJECT_LONG", SIZE_OF_SUBJECT_SUBJECT);
	// 2006:200 LPR: KIND_SUBJECTS -> KIND_LAWSUITS
	new TCharAliasFace(this, IDC_EDIT_SUBJECT_KIND, tSubject->FNI("F_KIND"), Kind, KIND_LAWSUITS);
	name = new TStringFace(this, IDC_EDIT_SUBJECT_NAME, "F_NAME", SIZE_OF_SUBJECT_NAME);
	// 2006:202 using ResultListFace; 2006:206 using TCharListNameFace
	results = new TCharListNameFace(this, IDC_EDIT_SUBJECT_RESULTS, tSubject->FNI("F_RESULTS"), Result, RESULT_OPENS,
		SIZE_OF_SUBJECT_RESULTS);
	attrib = new TCharListFace(this, IDC_EDIT_SUBJECT_ATTRIB, tSubject->FNI("F_ATTRIB"), Post,
		tSubject->type == TYPE_PUNISHMENT_LAW ? POST_CRIME_ATTRIBS : POST_OTHER_ATTRIBS, SIZE_OF_SUBJECT_ATTRIB);
	new TCheckFace(this, IDC_EDIT_SUBJECT_ACTIVE, tSubject->FNI("F_FLAGS_ACTIVE"));
	judgeRep = new TLongFace(this, IDC_EDIT_SUBJECT_JUDGEREP, tSubject->FNI("F_JUDGE_REPORT"), SIZE_OF_JUDGE_REPORT);
	closeds = new TCharListNameFace(this, IDC_EDIT_SUBJECT_CLOSEDS, tSubject->FNI("F_CLOSEDS"), Result, RESULT_CLOSEDS,
		SIZE_OF_SUBJECT_RESULTS);
	quickRepLabel = new TStatic(this, IDC_EDIT_SUBJECT_QUICKREP_LABEL);
	quickRep = new TLongFace(this, IDC_EDIT_SUBJECT_QUICKREP, tSubject->FNI("F_QUICK_REPORT"), SIZE_OF_JUDGE_REPORT);
	eispp = new TPushButton(this, IDEISPP);
	apcTmpl = new TCheckFace(this, IDC_EDIT_SUBJECT_APC_TMPL, "F_FLAGS_APC_TMPL");
}

bool TEditSubjectDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (!judgeRep->L())
			isValid = error("Отчетната колона е задължителна.");
		else if (judgeRep->L() > JUDGE_COLUMNS_MAX)
			isValid = error("Отчетната колона е извън обхвата.");
		else if (Subject()->type == TYPE_PUNISHMENT_LAW && !strchr(attrib->S(), POST_ALTERNATE))
		{
			isValid = ask("Предметът е наказателен, но атрибут %s е изключен. Желаете ли да продължите?",
				Post->Seek(POST_ALTERNATE));
		}
	}

	return isValid;
}

void TEditSubjectDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	if (edit)
		subject->SetS(Subject()->Code(Subject()->saas, true));
	else	// not currently allowed
		subject->EnableWindow(true);

	if (CourtType == COURT_REGIONAL)
	{
	#if COURT_TYPE == COURT_REGIONAL
		apcTmpl->ShowWindow(SW_SHOW);
		apcTmpl->EnableWindow(strchr(TYPE_APC_LAWS, Subject()->type));
	#endif  // REGIONAL

		if (Subject()->type != TYPE_PUNISHMENT_LAW)
		{
			quickRepLabel->ShowWindow(SW_SHOW);
			quickRep->ShowWindow(SW_SHOW);
		}
	}

	if (!strchr(TYPE_LAWSUITS, TYPE_PUNISHMENT_LAW))
		eispp->EnableWindow(false);
}

void TEditSubjectDialog::OKBNClicked()
{
	if (IsValid())
	{
		// 2005:356 New subject == edited by definition
		// only the fields which are set in court.sql
		TSubject *subject = Subject();
		*subject << this;

		bool edited = !edit ||
			strcmp(subject->name, backup.name) ||
			!strequal(subject->SaasD().results, backup.SaasD().results) ||
			subject->SaasD().judgeReport != backup.SaasD().judgeReport ||
			!strequal(subject->SaasD().closeds, backup.SaasD().closeds) ||
			!strequal(subject->SaasD().attrib, backup.SaasD().attrib);

		// 2003:055 Only if really edited.
		if (edited)
			subject->SaasD().flags |= SUBJECT_EDITED;

		if (subject->Save(edit))
			CmOk();
	}
}

void TEditSubjectDialog::IndexesBNClicked()
{
	TSubjectArchiveIndexDialog(this, indexes).Execute();
}

void TEditSubjectDialog::RealitiesBNClicked()
{
	TSubjectRealityTypesDialog(this, Subject()).Execute();
}

void TEditSubjectDialog::EisppBNClicked()
{
	TSubjectEisppDialog(this, Subject()).Execute();
}

void TEditSubjectDialog::EisppBNEnable(TCommandEnabler &tce)
{
	tce.Enable(Subject()->type == TYPE_PUNISHMENT_LAW);
}
