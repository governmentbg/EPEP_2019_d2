DEFINE_RESPONSE_TABLE1(TQueryWebDialog, TPeriodDialog)
//{{TQueryWebDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDC_QUERY_WEB_FIXED, VariantBNClicked),
	EV_BN_CLICKED(IDC_QUERY_WEB_PUBLIC, VariantBNClicked),
	EV_BN_CLICKED(IDC_QUERY_WEB_FORCED, VariantBNClicked),
	EV_BN_CLICKED(IDC_QUERY_WEB_POLICE, VariantBNClicked),
	EV_BN_CLICKED(IDC_QUERY_WEB_PROSEC, VariantBNClicked),
	EV_BN_CLICKED(IDC_QUERY_WEB_ACQUIT, VariantBNClicked),
	EV_BN_CLICKED(IDC_QUERY_WEB_REINVEST, VariantBNClicked),
	EV_BN_CLICKED(IDC_QUERY_WEB_PROTOCOL, VariantBNClicked),
//{{TQueryWebDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TQueryWebDialog Implementation}}

TQueryWebDialog::TQueryWebDialog(TWindow* parent, TQueryWebGroup *webGroup, int resId)
:
	TPeriodDialog(parent, webGroup, resId)
{
	new TRadioFace(this, IDC_QUERY_WEB_FIXED, "F_FIXED");
	new TRadioFace(this, IDC_QUERY_WEB_PUBLIC, "F_PUBLIC");
	forced = new TRadioFace(this, IDC_QUERY_WEB_FORCED, "F_FORCED");
	new TRadioFace(this, IDC_QUERY_WEB_POLICE, "F_POLICE");
	new TRadioFace(this, IDC_QUERY_WEB_PROSEC, "F_PROSEC");
	kinds = new TCharAutoListFace(this, IDC_QUERY_WEB_KINDS, "F_KINDS", Kind, Default->xKinds, SIZE_OF_KINDS);
	new TCheckFace(this, IDC_QUERY_WEB_JUDGES, "F_JUDGES");
	important = new TCheckFace(this, IDC_QUERY_WEB_IMPORTANT, "F_IMPORTANT");
	filtered = new TCheckFace(this, IDC_QUERY_WEB_FILTERED, "F_FILTERED");
	orderByTYN = new TRadioFace(this, IDC_QUERY_WEB_BY_TYN, "F_BY_TYN");
	orderByDTYN = new TRadioFace(this, IDC_QUERY_WEB_BY_DTYN, "F_BY_DTYN");
	name = new TStringFace(this, IDC_QUERY_WEB_NAME, "F_NAME", SIZE_OF_OFFICE_TARGET);
	attached = new TCheckFace(this, IDC_QUERY_WEB_ATTACHED, "F_ATTACHED");
	text = new TCheckFace(this, IDC_QUERY_WEB_TEXT, "F_TEXT");
	limited = new TCheckFace(this, IDC_QUERY_WEB_LIMITED, "F_LIMITED");
	appeal = new TCheckFace(this, IDC_QUERY_WEB_APPEAL, "F_APPEAL");
	acquit = new TRadioFace(this, IDC_QUERY_WEB_ACQUIT, "F_ACQUIT");
	reinvest = new TRadioFace(this, IDC_QUERY_WEB_REINVEST, "F_REINVEST");
	protocol = new TRadioFace(this, IDC_QUERY_WEB_PROTOCOL, "F_PROTOCOL");
	new TCharAutoListFace(this, IDC_QUERY_WEB_COMPOSITIONS, "F_COMPOSITIONS", CharComposition, QUERY_COMPOSITIONS,
		SIZE_OF_COMPOSITIONS);
	minDateLabel = new TStatic(this, IDC_QUERY_WEB_MIN_DATE_LABEL);
	maxDateLabel = new TStatic(this, IDC_QUERY_WEB_MAX_DATE_LABEL);
}

void TQueryWebDialog::SetupWindow()
{
	TPeriodDialog::SetupWindow();

#if COURT_TYPE == COURT_APPEAL
	acquit->EnableWindow(false);
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN
	acquit->ShowWindow(SW_HIDE);
	protocol->ShowWindow(SW_SHOW);
#endif  // ADMIN
	reinvest->EnableWindow(strchr(Default->colleges, TYPE_PUNISHMENT_LAW) && any(RESULT_REINVESTS));
	VariantBNClicked();
}

bool TQueryWebDialog::IsValid()
{
	bool isValid = TPeriodDialog::IsValid();

	if (isValid)
	{
		const char *non_pun;
		const char *tName = name->S();

		*Group() << this;
		for (non_pun = WebGroup()->kinds; *non_pun; non_pun++)
			if (kind2type(*non_pun) != TYPE_PUNISHMENT_LAW)
				break;

		if (!any(tName))
			isValid = error("Името на файла е задължително.");
		else if (!TOffice::ValidTargetName(tName))
			isValid = false;
		else if ((WebGroup()->variant == WEB_QUERY_ACQUIT || WebGroup()->variant == WEB_QUERY_REINVEST) && any(non_pun))
		{
			isValid = error("Справките за дела с оправдателна присъда или върнати за доразследване са само за "
				"наказателни дела");
		}
		else if (limited->IsWindowEnabled() && limited->GetCheck() == BF_CHECKED)
		{
			isValid = warn("Внимание! Избрали сте справка за публикуване в WEB, съдържаща дела с ограничен "
				"достъп.\n\nЖелаете ли да продължите?");
		}
	}

	return isValid;
}

void TQueryWebDialog::VariantBNClicked()
{
	*Group() << this;
	long variant = WebGroup()->variant;
	bool fixed = variant == WEB_QUERY_FIXED;
	CDate start = Today, final = Today;
	bool inspect = variant == WEB_QUERY_ACQUIT || variant == WEB_QUERY_REINVEST;

	if (fixed)
	{
		if (!WEB_FIXED_START_DATE.Empty())
			start = WEB_FIXED_START_DATE;
		else
			start += 1;

		if (!WEB_FIXED_FINAL_DATE.Empty())
			final = WEB_FIXED_FINAL_DATE;
		else
			final += 8;
	}
	else if (inspect)
	{
		start.Clear();
		final.Clear();
	}
	else
	{
		if (!WEB_QUERY_START_DATE.Empty())
			start = WEB_QUERY_START_DATE;
		else
			start += -8;

		if (!WEB_QUERY_FINAL_DATE.Empty())
			final = WEB_QUERY_FINAL_DATE;
		else
			final += -1;

		limited->SetCheck(BF_UNCHECKED);
	}

	minDate->SetDate(start);
	maxDate->SetDate(final);

	{
		static constant names[] =
		{
			"WEB_FIX.html",    // WEB_QUERY_FIXED
			"WEB_DEC.html",    // WEB_QUERY_PUBLIC
			"WEB_VALID.html",  // WEB_QUERY_FORCED
			"POLICE.html",
			"PROSEC.html",
			"Acquit.htm",
			"Reinvest.htm",
			"WEB_SES.html"    // WEB_QUERY_PROTOC
		};
		char path[SIZE_OF_PATH];

		build_fn(OUT_PATH, names[variant], path);
		name->SetS(path);

		// 2014:024
		struct TEndis
		{
			bool enable;
			int check;
		};

		const long &wa = WEB_ATTACH;
		TEndis Endis[WEB_QUERY_COUNT][CB_COUNT] =
		{
			//  important     filtered      attached      text          appeal        limited
			{ { true, -1 }, { true,  1 }, { false, 0 }, { false, 0 }, { false, 0 }, { true,  0 } },  // fixed
			{ { true, -1 }, { true,  1 }, { true, wa }, { true,  1 }, { true,  1 }, { false, 1 } },  // finished
			{ { true, -1 }, { true,  1 }, { true, wa }, { true,  1 }, { true,  1 }, { false, 1 } },  // forced
			{ { true, -1 }, { false, 0 }, { false, 0 }, { false, 0 }, { false, 0 }, { false, 1 } },  // police
			{ { false, 0 }, { false, 0 }, { false, 1 }, { false, 0 }, { false, 0 }, { false, 1 } },  // prosec
			{ { true, -1 }, { false, 0 }, { false, 1 }, { true,  1 }, { false, 1 }, { false, 1 } },  // acquit
			{ { true, -1 }, { false, 0 }, { false, 1 }, { true,  1 }, { false, 0 }, { false, 1 } },  // reinvest
			{ { true, -1 }, { true,  1 }, { true, wa }, { true,  1 }, { true,  1 }, { false, 0 } }   // protocol
		};
		TCheckFace *checks[CB_COUNT] = { important, filtered, attached, text, appeal, limited };

		for (int i = 0; i < CB_COUNT; i++)
		{
			const TEndis &ed = Endis[variant][i];

			checks[i]->EnableWindow(ed.enable);
			if (ed.check != -1)
				checks[i]->SetCheck(ed.check ? BF_CHECKED : BF_UNCHECKED);
		}
	}

	orderByDTYN->SetCheck(fixed ? BF_CHECKED : BF_UNCHECKED);
	orderByTYN->SetCheck(fixed ? BF_UNCHECKED : BF_CHECKED);
	attached->SetCaption(inspect || variant == WEB_QUERY_PROSEC ? "С присъединени документи" : "С обезличени документи");

	const char *dateName;

	switch (variant)
	{
		case WEB_QUERY_FIXED : dateName = "насрочване"; break;
		case WEB_QUERY_PROTOCOL : dateName = "провеждане"; break;
		case WEB_QUERY_FORCED :	dateName = "влизане в з.с."; break;
		case WEB_QUERY_PROSEC : dateName = "мотиви"; break;
		case WEB_QUERY_PUBLIC : dateName = "публикуване"; break;
		default : dateName = "постановяване";
	}

	mstr m("От дата на ");

	m.cat(dateName);
	minDateLabel->SetCaption(str(m));
	m.cpy("До дата на ");
	m.cat(dateName);
	maxDateLabel->SetCaption(str(m));
}
