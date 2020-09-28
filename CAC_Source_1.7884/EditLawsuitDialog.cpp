DEFINE_RESPONSE_TABLE1(TEditLawsuitDialog, TTroubleDialog)
//{{TEditLawsuitDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDC_NEW_LAWSUIT_QUICK, QuickBNClicked),
	EV_BN_CLICKED(IDADD, AddBNClicked),
	EV_BN_CLICKED(IDEDIT, EditBNClicked),
	EV_COMMAND_ENABLE(IDEDIT, EditBNEnable),
	EV_BN_CLICKED(IDDELETE, DeleteBNClicked),
	EV_COMMAND_ENABLE(IDDELETE, DeleteBNEnable),
	EV_LVN_ITEMCHANGING(IDC_GROUP_LIST_WINDOW, ListLVNItemChanging),
	EV_BN_CLICKED(IDIMPORTANCE, ImportanceBNClicked),
	EV_CBN_SELCHANGE(IDC_NEW_LAWSUIT_SUBJECT, SubjectCBNSelChange),
//{{TEditLawsuitDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditLawsuitDialog Implementation}}

TEditLawsuitDialog::TEditLawsuitDialog(TWindow* parent, TLawsuit *lawsuit, TGArray<TConnect> *tConnects,
	const char *predefined)
:
	TTroubleDialog(parent, lawsuit, true, TYPE_IMAGE, IDD_EDIT_LAWSUIT), lawQuick(lawsuit->quick),
		has_eispp_nmr(any(lawsuit->eispp_nmr))
{
	kind = new TCharAliasFace(this, IDC_NEW_LAWSUIT_KIND, "F_KIND", Kind, NULL);
	new TCharAliasFace(this, IDC_NEW_LAWSUIT_SOURCE_KIND, "F_SOURCE_KIND", Kind, NULL);
	subject = new TSubjectAliasFace(this, IDC_NEW_LAWSUIT_SUBJECT, "F_SUBJECT", lawsuit->kind, lawsuit->subject);
	new TLongAliasFace(this, IDC_NEW_LAWSUIT_COMPOSITION, "F_COMPOSITION", Composition, Default->compositions, FLAG_NULL);
	college = new TCharAliasFace(this, IDC_NEW_LAWSUIT_COLLEGE, "F_COLLEGE", College, Default->colleges);
	judge = new TUserAliasFace(this, IDC_NEW_LAWSUIT_JUDGE, "F_JUDGE", POST_VISIBLE_JUDGES, ALIAS_EMPTY | USER_INSECT);
	reJudge = new TUserAliasFace(this, IDC_EDIT_LAWSUIT_REJUDGE, "F_REJUDGE", POST_VISIBLE_JUDGES,
		ALIAS_EMPTY | USER_INSECT);
	date = new TDateFace(this, IDC_NEW_LAWSUIT_DATE, "F_DATE");
	previousDate = new TDateFace(this, IDC_NEW_LAWSUIT_PREVIOUS_DATE, "F_PREVIOUS_DATE");

	if (lawsuit->source.type != TYPE_REQUEST)
		receivedDate = new TDateFace(this, IDC_NEW_LAWSUIT_RECEIVED_DATE, "F_RECEIVED_DATE");
	else
		receivedDate = NULL;

	if (lawsuit->quick)
		quick = new TCheckFace(this, IDC_NEW_LAWSUIT_QUICK, "F_QUICK_QUICK");
	else
		quick = new TQuickFace(this, IDC_NEW_LAWSUIT_QUICK, "F_QUICK");

	converted = new TCheckFace(this, IDC_EDIT_LAWSUIT_CONVERTED, "F_QUICK_CONVERTED");

	new TLongFace(this, IDC_NEW_LAWSUIT_NO, "F_NO", SIZE_OF_NO);
	new TYearFace(this, IDC_NEW_LAWSUIT_SOURCE_YEAR, "F_SOURCE_YEAR");
	new TLongFace(this, IDC_NEW_LAWSUIT_SOURCE_NO, "F_SOURCE_NO", SIZE_OF_NO);
	new TStringFace(this, IDC_NEW_LAWSUIT_TEXT, "F_TEXT", SIZE_OF_ORIGIN_TEXT);

	eispp_nmr = new TEisNmrFace(this, IDC_NEW_LAWSUIT_EISPP_NMR, "F_EISPP_NMR");
	eispp_nmr_label = new TStatic(this, IDC_NEW_LAWSUIT_EISPP_NMR_LABEL);
	connects = new TConnectListWindow(this, IDC_GROUP_LIST_WINDOW, tConnects, predefined, eispp_nmr);
	newGPK = new TCheckFace(this, IDC_NEW_LAWSUIT_NEWGPK, "F_FLAGS_NEWGPK");
	limited = new TCheckFace(this, IDC_NEW_LAWSUIT_LIMITED, "F_FLAGS_LIMITED");
	doublex = new TCheckFace(this, IDC_NEW_LAWSUIT_DOUBLEX, "F_FLAGS_DOUBLEX");

	children = new TCheckFace(this, IDC_NEW_LAWSUIT_CHILDREN, "F_FLAGS_CHILDREN");
	corrupt = new TCheckFace(this, IDC_NEW_LAWSUIT_CORRUPT, "F_FLAGS_CORRUPT");
	pt329a3 = new TCheckFace(this, IDC_NEW_LAWSUIT_PT329A3, "F_FLAGS_PT329A3");
}

bool TEditLawsuitDialog::IsValid()
{
	bool isValid = TTroubleDialog::IsValid();

	if (isValid)
	{
		if (!atob(judge->S()) || !atob(reJudge->S()) || !subject->L())
			isValid = error("Съдиите и предмета са задължителни.");
		else if (!previousDate->Date().Empty() && previousDate->Date() > date->Date())
			isValid = error("Полетата дата на образуване и дата на разпореждане за образуване не си съответстват.");
		else if (receivedDate != NULL && !receivedDate->Date().Empty() && receivedDate->Date() > date->Date())
			isValid = error("Полетата дата на получаване и дата на образуване не си съответстват.");
		else if (quick->GetCheck() == BF_CHECKED && doublex->GetCheck() == BF_CHECKED)
			isValid = error("Бързо/незабавно производство и двойна размяна са несъвместими.");
	}

	return isValid;
}

bool TEditLawsuitDialog::IsClean()
{
	bool isClean = TTroubleDialog::IsClean();

	if (isClean)
	{
		isClean = !connects->dirty || ask("Промените в адресантите се записват само при потвърждение. "
			"Желаете ли да прекъснете редактирането?");
	}

	return isClean;
}

void TEditLawsuitDialog::SetupWindow()
{
	TTroubleDialog::SetupWindow();

	TLawsuit *lawsuit = Lawsuit();

	if (lawQuick)
	{
		// 2006:073 former bloat adjust_doc_quick
		if (strchr(KIND_QUICKABLES, lawsuit->sourceKind))
		{
			quick->EnableWindow(true);
			if (lawQuick & QUICK_IMMEDIATE)
				quick->SetCaption("Неза&бавно производство");
		}
		else
		{
			quick->EnableWindow(false);
			quick->SetCheck(strchr(KIND_QUICKONLYS, lawsuit->sourceKind) ? BF_CHECKED : BF_UNCHECKED);
		}
	}
	else
		((TQuickFace *) quick)->SetKind(lawsuit->sourceKind);

	if (receivedDate)
		receivedDate->EnableWindow(true);

	// 2007:218 LPR: never changed, so init here
	connects->SetLawKind(kind->C());
	// 2007:244 LRQ: FIX: we need connects->decisionKinds as well
	connects->SetIndocKind(lawsuit->sourceKind);
	// 2008:162; 2015:013 !can_func(RIGHT_LIMITED) -> false, impossible
	adjust_lawsuit_limited(lawsuit, limited, false);
#if COURT_TYPE == COURT_AREAL
	// 2009:201
	if (strchr(KIND_DOUBLE_EXCH_INDOCS, lawsuit->sourceKind))
	{
		doublex->ShowWindow(SW_SHOW);
		doublex->EnableWindow(true);
	}
#endif  // AREAL

	if (strchr(TYPE_NEWGPK_LAWS, lawsuit->key.type))
	{
		newGPK->EnableWindow(true);
		newGPK->ShowWindow(SW_SHOW);
	}

	if (lawsuit->key.type == TYPE_PUNISHMENT_LAW)
	{
		corrupt->EnableWindow(true);
		corrupt->ShowWindow(SW_SHOW);
		pt329a3->ShowWindow(SW_HIDE);
	}
	else
	{
		pt329a3->EnableWindow(true);
		pt329a3->ShowWindow(SW_SHOW);
	}

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	eispp_nmr->SetKinds(lawsuit->sourceKind, Lawsuit()->kind, has_eispp_nmr);
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

#if COURT_TYPE == COURT_ADMIN
	eispp_nmr->ShowWindow(SW_HIDE);
	eispp_nmr_label->ShowWindow(SW_HIDE);
#endif  // ADMIN

	if (lawsuit->source.type == TYPE_REQUEST)
	{
		TRequest request;
		request.key = lawsuit->source;
		TRequestResolution *const resolution = request.LastResolution("F_ASSIGN");
		TVirtPtr VP(resolution);
		if (resolution && atob(resolution->assign))
			reJudge->EnableWindow(false);
	}

	if (strchr(TYPE_CHILDREN_LAWS, lawsuit->key.type))
	{
		children->EnableWindow(true);
	#if COURT_TYPE == COURT_REGIONAL
		children->SetCaption("Специализиран състав");
	#endif  // REGIONAL
	}
	else
		children->ShowWindow(SW_HIDE);
}

void TEditLawsuitDialog::OKBNClicked()
{
	bool newGPKChanged = newGPK->IsWindowEnabled() &&
		!(Lawsuit()->flags & LAWSUIT_NEWGPK) != (newGPK->GetCheck() == BF_UNCHECKED);

	if (IsValid() &&
		// 2010:202 LPR: FLAG_ACTIVE -> FLAG_NULL
		lawsuit_danger_valid(Lawsuit(), FLAG_ACTIVE) &&
		// 2017:087 comout to mimic previous versions
		//ask_college(kind->C(), college->C(), "делото") &&
		(!newGPKChanged || date->Date() >= NewGPKDate || newGPK->GetCheck() == BF_UNCHECKED ||
		ask("Датата на образуване е преди %Dг. Желаете ли да образувате делото по новия ГПК?", &NewGPKDate))
	)
	{
		*Lawsuit() << this;

		if (Lawsuit()->Update())
		{
			write_connects(Lawsuit(), connects->Connects(), "делото");
			connects->dirty = false;	// don't check/warn on close
			trouble.Move(Lawsuit(), date->Date());
			CmOk();
		}
	}
}

void TEditLawsuitDialog::QuickBNClicked()
{
	if (!(lawQuick & QUICK_CONVERTED))
	{
		long l = lawQuick ? quick->GetCheck() == BF_CHECKED ? QUICK_QUICK : 0 : ((TQuickFace *) quick)->L();
		converted->SetCheck((l ^ lawQuick) & QUICK_QUICK ? BF_CHECKED : BF_UNCHECKED);
		if (!lawQuick)
			((TQuickFace *) quick)->AlterText();
	}
}

void TEditLawsuitDialog::AddBNClicked()
{
	connects->Add();
}

void TEditLawsuitDialog::EditBNClicked()
{
	connects->Edit();
}

void TEditLawsuitDialog::DeleteBNClicked()
{
	connects->Delete();
}

void TEditLawsuitDialog::ImportanceBNClicked()
{
	if (subject->L())
	{
		*Lawsuit() << this;
		TImportantDialog(this, Lawsuit(), true).Execute();
	}
	else
		error("Предметът е задължителен.");
}

void TEditLawsuitDialog::SubjectCBNSelChange()
{
	if (subject->L() && subject->L() != Lawsuit()->subject)
	{
		Lawsuit()->subject = subject->L();
		adjust_lawsuit_danger(Lawsuit(), FLAG_NULL, "предмета");
		// 2015:013 true -> false, for better security
		adjust_lawsuit_limited(Lawsuit(), limited, false);
	}
}

void TEditLawsuitDialog::EditBNEnable(TCommandEnabler &tce)
{
	tce.Enable(connects->MayEdit());
}

void TEditLawsuitDialog::DeleteBNEnable(TCommandEnabler &tce)
{
	tce.Enable(connects->MayDelete());
}

bool TEditLawsuitDialog::ListLVNItemChanging(TLwNotify &lwn)
{
	return connects->ItemChanging(lwn);
}
