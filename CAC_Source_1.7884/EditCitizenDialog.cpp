DEFINE_RESPONSE_TABLE1(TEditCitizenDialog, TEditNamedDialog)
//{{TEditCitizenDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TEditCitizenDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditCitizenDialog Implementation}}

TEditCitizenDialog::TEditCitizenDialog(TWindow* parent, TCitizen *citizen, bool edit, bool full, bool tMustIdentify)
:
	TEditNamedDialog(parent, citizen, edit, full, IDD_EDIT_CITIZEN), mustIdentify(tMustIdentify)
{
	ucnType = new TCharAliasFace(this, IDC_EDIT_CITIZEN_TYPE, "F_UCN_TYPE", UCNType, NULL);
	new TStringFace(this, IDC_EDIT_CITIZEN_UCN, "F_UCN", SIZE_OF_UCN);
	name = new TCitizenNameFace(this, IDC_EDIT_CITIZEN_NAME, "F_NAME", SIZE_OF_CITIZEN_NAME);
	reName = new TCitizenNameFace(this, IDC_EDIT_CITIZEN_RENAME, "F_RENAME", SIZE_OF_CITIZEN_RENAME);
	family = new TCitizenNameFace(this, IDC_EDIT_CITIZEN_FAMILY, "F_FAMILY", SIZE_OF_CITIZEN_FAMILY);
	reFamily = new TCitizenNameFace(this, IDC_EDIT_CITIZEN_REFAMILY, "F_REFAMILY", SIZE_OF_CITIZEN_REFAMILY);
	country = new TStringAliasFace(this, IDC_EDIT_CITIZEN_COUNTRY, "F_COUNTRY", Country, NULL);
	reCountry = new TStringAliasFace(this, IDC_EDIT_CITIZEN_RECOUNTRY, "F_RECOUNTRY", Country, NULL, ALIAS_EMPTY);
	lnc = new TLNCFace(this, IDC_EDIT_CITIZEN_LNC, "F_LNC");
	backup = new TCheckFace(this, IDC_EDIT_CITIZEN_BACKUP, "F_BACKUP");
	oldCitizen << citizen;

	// 2008:353; 2011:349 LPR: disable for special
	if ((CourtType == COURT_MILITARY || CourtType == COURT_MILITARY_APPEAL) && COURT_CODE > COURT_SPECIAL_MAX)
	{
		rankLabel = new TStatic(this, IDC_EDIT_CITIZEN_RANK_LABEL);
		rank = new TFindLongAliasFace(this, IDC_EDIT_CITIZEN_RANK, "F_LONG_RANK", SIZE_OF_NAME, Rank, NULL, FLAG_NULL);
	}
	else
	{
		rankLabel = NULL;
		rank = NULL;
	}
}

bool TEditCitizenDialog::IsValid()
{
	bool isValid = TEditNamedDialog::IsValid();

	if (isValid)
	{
		if (!strcmp(country->S(), reCountry->S()))
			isValid = error("Гражданство и второ гражданство се дублират.");
		else if (mustIdentify && ucnType->C() != UCN_CITIZEN_UCN && !*lnc->S())
			isValid = error("Полето ЛНЧ е задължително.");
		else
		{
			TOldCitizen tOldCitizen;

			*Citizen() << this;
			tOldCitizen << this;

			// 2013:186 FIX: // || tOldCitizen.FindKey("F_LNC")
			if (Citizen()->FindKey("F_LNC"))
				isValid = ask("Вече съществува физическо лице с ЛНЧ %s. Желаете ли да продължите?", lnc->S());
		}
	}

	return isValid;
}

bool TEditCitizenDialog::CommitBackup()
{
	if (backup->GetCheck() == BF_CHECKED)
	{
		TOldCitizen tOldCitizen;
		tOldCitizen << oldCitizen;

		if (tOldCitizen.Try())
		{
			if (ask("Вече има предишни имена за лицето: %s %s %s %s. Желаете ли да ги заместите?",
				tOldCitizen.name, tOldCitizen.reName, tOldCitizen.family, tOldCitizen.reFamily))
			{
				tOldCitizen << oldCitizen;
				return tOldCitizen.Update();
			}
		}
		else
			return oldCitizen.Insert();
	}

	return true;
}

void TEditCitizenDialog::OKBNClicked()
{
	if (IsValid() && Citizen()->CheckLiterate())
	{
		if (CommitBackup())
		{
			if (rank && rank->L())
			{
				Rank->Seek(rank->L());
				strcpy(Citizen()->rank, Rank->alias);
			}

			if (Citizen()->Save(edit))
				CmOk();
			else
				backup->SetCheck(BF_UNCHECKED);
		}
	}
}

void TEditCitizenDialog::SetupWindow()
{
	TEditNamedDialog::SetupWindow();

	reName->EnableWindow(full);
	family->EnableWindow(full);
	reFamily->EnableWindow(full);

	if (rankLabel)
		rankLabel->ShowWindow(SW_SHOW);

	if (rank)
	{
		rank->ShowWindow(SW_SHOW);
		if (any(Citizen()->rank) && Rank->Seek(Citizen()->rank))
			rank->SetL(Rank->rank);
	}

	if (edit && mustIdentify && *lnc->S())
		lnc->EnableWindow(false);

	backup->SetCheck(BF_UNCHECKED);
	backup->EnableWindow(full);
}
