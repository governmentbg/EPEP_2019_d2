DEFINE_RESPONSE_TABLE1(TNewLawsuitDialog, TTroubleDialog)
//{{TNewLawsuitDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_CBN_SELCHANGE(IDC_NEW_LAWSUIT_KIND, KindCBNSelChange),
	EV_CBN_SELCHANGE(IDC_NEW_LAWSUIT_SOURCE_KIND, SourceKindCBNSelChange),
	EV_CBN_SELCHANGE(IDC_NEW_LAWSUIT_COMPOSITION, CompositionCBNSelChange),
	EV_BN_CLICKED(IDC_NEW_LAWSUIT_QUICK, QuickBNClicked),
	EV_BN_CLICKED(IDADD, AddBNClicked),
	EV_BN_CLICKED(IDEDIT, EditBNClicked),
	EV_COMMAND_ENABLE(IDEDIT, EditBNEnable),
	EV_BN_CLICKED(IDDELETE, DeleteBNClicked),
	EV_COMMAND_ENABLE(IDDELETE, DeleteBNEnable),
	EV_LVN_ITEMCHANGING(IDC_GROUP_LIST_WINDOW, ListLVNItemChanging),
	EV_BN_CLICKED(IDIMPORTANCE, ImportanceBNClicked),
	EV_CBN_SELCHANGE(IDC_NEW_LAWSUIT_SUBJECT, SubjectCBNSelChange),
	EV_CBN_SELCHANGE(IDC_NEW_LAWSUIT_COLLEGE, CollegeCBNSelChange),
	EV_CBN_SELCHANGE(IDC_NEW_LAWSUIT_JUDGE, JudgeCBNSelChange),
	EV_EN_UPDATE(IDC_NEW_LAWSUIT_DATE, DateENUpdate),
	EV_BN_CLICKED(IDC_NEW_LAWSUIT_NEWGPK, NewGPKBNClicked),
//{{TNewLawsuitDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TNewLawsuitDialog Implementation}}

TNewLawsuitDialog::TNewLawsuitDialog(TWindow* parent, TLawsuit *lawsuit, TGArray<TConnect> *tConnects, const char *tLawKinds,
	const char *sourceKinds, const char *predefined, TRequest *tRequest)
:
	TTroubleDialog(parent, lawsuit, false, TYPE_IMAGE, IDD_NEW_LAWSUIT), lawKinds(tLawKinds), newGPKChanged(false),
		request(tRequest), has_eispp_nmr(any(lawsuit->eispp_nmr)),
		assigned(request && atob(lawsuit->judge) && strcmp(lawsuit->judge, INTER) && lawsuit->subject)
{
	kind = new TCharAliasFace(this, IDC_NEW_LAWSUIT_KIND, "F_KIND", Kind, lawKinds);
	sourceKind = new TCharAliasFace(this, IDC_NEW_LAWSUIT_SOURCE_KIND, "F_SOURCE_KIND", Kind, sourceKinds);
	subject = new TSubjectAliasFace(this, IDC_NEW_LAWSUIT_SUBJECT, "F_SUBJECT", *lawKinds);
	composition = new TLongAliasFace(this, IDC_NEW_LAWSUIT_COMPOSITION, "F_COMPOSITION", Composition, Default->compositions,
		FLAG_NULL);
	college = new TCharAliasFace(this, IDC_NEW_LAWSUIT_COLLEGE, "F_COLLEGE", College, Default->colleges);
	// 2013:346 LRQ/URQ: allow INTER for all law types
	judge = new TUserAliasFace(this, IDC_NEW_LAWSUIT_JUDGE, "F_JUDGE", POST_VISIBLE_JUDGES,
		FLAG_ACTIVE | USER_INTER | ALIAS_EMPTY | USER_INSECT);
	date = new TDateFace(this, IDC_NEW_LAWSUIT_DATE, "F_DATE");
	previousDate = new TDateFace(this, IDC_NEW_LAWSUIT_PREVIOUS_DATE, "F_PREVIOUS_DATE");
	// 2007:219 !strchr(KIND_INDOCS, *sourceKinds) -> predefined
	receivedDate = predefined ? new TDateFace(this, IDC_NEW_LAWSUIT_RECEIVED_DATE, "F_RECEIVED_DATE") : NULL;
	quick = new TQuickFace(this, IDC_NEW_LAWSUIT_QUICK, "F_QUICK");
	// 2012:005 LRQ: support name/attach if predefined -> always

	no = new TLongFace(this, IDC_NEW_LAWSUIT_NO, "F_NO", SIZE_OF_NO);
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

void TNewLawsuitDialog::TypeChanged()
{
	char tType = kind2type(kind->C());
	// 2008:076
	if (strchr(TYPE_NEWGPK_LAWS, tType))
	{
		newGPK->EnableWindow(true);
		newGPK->ShowWindow(SW_SHOW);

		if (newGPKChanged)
			newGPK->SetCheck(Lawsuit()->flags & LAWSUIT_NEWGPK ? BF_CHECKED : BF_UNCHECKED);
		else
			DateENUpdate();
	}
	else
	{
		newGPK->ShowWindow(SW_HIDE);
		newGPK->EnableWindow(false);
		newGPK->SetCheck(BF_UNCHECKED);
	}

	if (tType == TYPE_PUNISHMENT_LAW)
	{
		corrupt->EnableWindow(true);
		corrupt->ShowWindow(SW_SHOW);
		pt329a3->ShowWindow(SW_HIDE);
		pt329a3->EnableWindow(false);
		pt329a3->SetCheck(BF_UNCHECKED);
	}
	else
	{
		corrupt->ShowWindow(SW_HIDE);
		corrupt->EnableWindow(false);
		corrupt->SetCheck(BF_UNCHECKED);
		pt329a3->EnableWindow(true);
		pt329a3->ShowWindow(SW_SHOW);
	#if COURT_TYPE == COURT_REGIONAL
		if (sourceKind->C() == KIND_EXECUTIVE_REQUEST)
			pt329a3->SetCheck(BF_CHECKED);
	#endif  // REGIONAL

	}

	if (strchr(TYPE_CHILDREN_LAWS, tType))
	{
		children->EnableWindow(true);
	#if COURT_TYPE == COURT_REGIONAL
		children->SetCaption("Специализиран състав");
	#endif  // REGIONAL
	}
	else
	{
		children->ShowWindow(SW_HIDE);
		children->EnableWindow(false);
		children->SetCheck(BF_UNCHECKED);
	}
}

void TNewLawsuitDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Lawsuit() << this;	// for danger_valid()

		if (lawsuit_danger_valid(Lawsuit(), FLAG_ACTIVE) &&
			// 2017:087 check college for laws starting now 
			(request || ask_college(kind->C(), college->C(), "делото")) &&
			(date->Date() >= NewGPKDate || newGPK->GetCheck() == BF_UNCHECKED ||
			ask("Датата на образуване е преди %Dг. Желаете ли да образувате делото по новия ГПК?", &NewGPKDate)))
		{
			Lawsuit()->key.type = kind2type(Lawsuit()->kind);
			Lawsuit()->autoNumbering = !no->L();

			if (Lawsuit()->Insert())
			{
				write_connects(Lawsuit(), connects->Connects(), "новообразуваното дело");
				trouble.Move(Lawsuit(), date->Date());
				CmOk();
			}
		}
	}
}

void TNewLawsuitDialog::SetupWindow()
{
	TTroubleDialog::SetupWindow();

	if (receivedDate)
		receivedDate->EnableWindow(true);

	if (strchr(lawKinds, Default->kind))
		kind->SetC(Default->kind);

	// 2006:312 LPR: forward logic, change kind not source kind
	KindCBNSelChange();
	// 2007:036 LPR: done thru KindCBN -> CollegeCBN -> CompositionCBN
	//CompositionCBNSelChange();
	// 2007:017 FIX: init quick, it may be present from the request
	quick->SetL(Lawsuit()->quick);
	QuickBNClicked();		// 2011:004 FIX: was missing

#if COURT_TYPE == COURT_ADMIN
	eispp_nmr->ShowWindow(SW_HIDE);
	eispp_nmr_label->ShowWindow(SW_HIDE);
#endif  // ADMIN

	if (assigned)
		judge->EnableWindow(false);
}

bool TNewLawsuitDialog::IsValid()
{
	bool isValid = TTroubleDialog::IsValid();

	if (isValid)
	{
		const char *s;

		if (EXTERNAL_LAWSUIT_NO && !no->L())
			isValid = error("Номерът на дело е задължителен.");
		else if (!atob(judge->S()) || !subject->L())
			isValid = error("Съдията и предмета са задължителни.");
		else if (date->Date() > Today)
			isValid = error("Дата на образуване е по-голяма от текущата дата.");
		else if (!previousDate->Date().Empty() && previousDate->Date() > date->Date())
			isValid = error("Дата на разпореждане за образуване е след датата на образуване.");
		else if (receivedDate && !receivedDate->Date().Empty() && !previousDate->Date().Empty() &&
			receivedDate->Date() > previousDate->Date())
		{
			isValid = error("Дата на получаване в съда е след датата на разпореждане за образуване.");
		}
		else if (receivedDate != NULL && !receivedDate->Date().Empty() && receivedDate->Date() > date->Date())
			isValid = error("Дата на получаване в съда е след датата на образуване.");
		else if (quick->GetCheck() == BF_CHECKED && doublex->GetCheck() == BF_CHECKED)
			isValid = error("Бързо/незабавно производство и двойна размяна са несъвместими.");
		else if (!connects->IsValid())
			isValid = false;
		else if (request)
		{
			if ((s = request_lawable(request, RESULT_NULL)) != NULL)
				isValid = error("%s.", s);
			else if (!previousDate->Date().Empty() && previousDate->Date() < request->date)
				isValid = error("Дата на разпореждане за образуване е преди датата на входящия документ.");
			else if (date->Date() < request->date)
				isValid = error("Дата на образуване е преди датата на входящия документ.");
		}
	}

	return isValid;
}

void TNewLawsuitDialog::KindCBNSelChange()
{
	char tKind = kind->C();

	adjust_law_college(tKind, college);
	CollegeCBNSelChange();
	subject->SetKind(tKind);
	// 2006:312 LPR: forward logic: kind -> source kinds
	// 2007:214 LPR: new connect scheme: only touch connects lawKind
	connects->SetLawKind(tKind);
	if (connects->predefined)
		sourceKind->SetXCrit(adjust_source_kinds(tKind, connects->predefined));
	SourceKindCBNSelChange();
	// 2008:162 LPR: only adjust if subject (which may come from request)
	if (Lawsuit()->subject)
	{
		adjust_lawsuit_danger(Lawsuit(), FLAG_ACTIVE, "вида на делото");
		adjust_lawsuit_limited(Lawsuit(), limited, true);
	}
	TypeChanged();
}

void TNewLawsuitDialog::SourceKindCBNSelChange()
{
	char source = sourceKind->C();
	// 2006:312 LPR: comout backward (kind by sourceKind) change logic
	// 2007:214 LPR: new connect scheme, don't touch connects here
	quick->SetKind(source);
	// 2007:246 FIX: separated set indoc & law kinds
	connects->SetIndocKind(source);
	connects->AdjustJurisd();
#if COURT_TYPE == COURT_AREAL
	// 2009:201
	if (strchr(KIND_DOUBLE_EXCH_INDOCS, source))
	{
		doublex->ShowWindow(SW_SHOW);
		doublex->EnableWindow(true);
	}
	else
	{
		doublex->ShowWindow(SW_HIDE);
		doublex->SetCheck(BF_UNCHECKED);
		doublex->EnableWindow(false);
	}
#endif  // AREAL
	eispp_nmr->SetKinds(source, kind->C(), has_eispp_nmr);
}

void TNewLawsuitDialog::CompositionCBNSelChange()
{
	if (!atof(Lawsuit()->judge))
	{
		FineComposition->college = college->C();
		FineComposition->Seek(composition->L());
		// 2007:218 LPR: new judge rules: always set at this point
		//if (strcmp(FineComposition->judge, SUPER))
		//	judge->SetS(FineComposition->judge);
		// 2010:151 LPR: compositions are now initialized w/ EMPTY
		//judge->SetS(strcmp(FineComposition->judge, SUPER) ? FineComposition->judge : EMPTY);
		if (!assigned)
			judge->SetS(FineComposition->judge);
	}
}

void TNewLawsuitDialog::QuickBNClicked()
{
	quick->AlterText();
}

void TNewLawsuitDialog::AddBNClicked()
{
	connects->Add();
}

void TNewLawsuitDialog::EditBNClicked()
{
	connects->Edit();
}

void TNewLawsuitDialog::DeleteBNClicked()
{
	connects->Delete();
}

void TNewLawsuitDialog::ImportanceBNClicked()
{
	if (subject->L())
	{
		*Lawsuit() << this;
		TImportantDialog(this, Lawsuit(), false).Execute();
	}
	else
		error("Предметът е задължителен.");
}

void TNewLawsuitDialog::SubjectCBNSelChange()
{
	// 2008:281 LRQ: also check/xfer type, used by adjusts
	char type = kind2type(kind->C());

	if (subject->L() && (type != Lawsuit()->key.type || subject->L() != Lawsuit()->subject))
	{
		Lawsuit()->key.type = type;
		Lawsuit()->subject = subject->L();
		adjust_lawsuit_danger(Lawsuit(), FLAG_ACTIVE, "предмета");
		adjust_lawsuit_limited(Lawsuit(), limited, true);
	}
}

void TNewLawsuitDialog::CollegeCBNSelChange()
{
	CompositionCBNSelChange();
}

void TNewLawsuitDialog::JudgeCBNSelChange()
{
	// (un)block automatic set on composition(/college) change
	strcpy(Lawsuit()->judge, judge->S());
}

void TNewLawsuitDialog::DateENUpdate()
{
	if (newGPK->IsWindowEnabled() && !newGPKChanged && date_valid(date->S(), true))
		newGPK->SetCheck(date->Date() >= NewGPKDate ? BF_CHECKED : BF_UNCHECKED);
}

void TNewLawsuitDialog::NewGPKBNClicked()
{
	newGPKChanged = true;
	Lawsuit()->flags &= ~LAWSUIT_NEWGPK;
	if (newGPK->GetCheck() == BF_CHECKED)
		Lawsuit()->flags |= LAWSUIT_NEWGPK;
}

void TNewLawsuitDialog::EditBNEnable(TCommandEnabler &tce)
{
	tce.Enable(connects->MayEdit());
}

void TNewLawsuitDialog::DeleteBNEnable(TCommandEnabler &tce)
{
	tce.Enable(connects->MayDelete());
}

bool TNewLawsuitDialog::ListLVNItemChanging(TLwNotify &lwn)
{
	  return connects->ItemChanging(lwn);
}
