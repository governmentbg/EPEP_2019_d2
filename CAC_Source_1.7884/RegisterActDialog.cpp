DEFINE_RESPONSE_TABLE1(TRegisterActDialog, TTroubleDialog)
//{{TAtachDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_REGISTER_ACT_TYPE, TypeCBNSelChange),
	EV_BN_CLICKED(IDATTACH, AttachBNClicked),
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TAtachDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TRegisterActDialog Implementation}}

TRegisterActDialog::TRegisterActDialog(TWindow* parent, TRegisterAct *act, bool edit, int resId)
:
	TTroubleDialog(parent, act, edit, TYPE_TEXT, resId)
{
	build_ctypes(types, TYPE_ORIGINS, true);
	type = new TCharAliasFace(this, IDC_REGISTER_ACT_TYPE, "F_TYPE", Type, types);
	no = new TLongFace(this, IDC_REGISTER_ACT_NO, "F_NO", SIZE_OF_NO);
	year = new TYearFace(this, IDC_REGISTER_ACT_YEAR, "F_YEAR");
	// 2016:056 LPR: was KIND_VISIBLE_ENDOCS, modified for DECREE
	kind = new TCharAliasFace(this, IDC_REGISTER_ACT_KIND, "F_KIND", Kind, C2S[KIND_DECISION]);
	date = new TDateFace(this, IDC_REGISTER_ACT_DATE, "F_DATE");
	new TStringFace(this, IDC_REGISTER_ACT_TEXT, "F_TEXT", SIZE_OF_REGISTER_ACT_TEXT);
}

bool TRegisterActDialog::IsValid()
{
	bool isValid = TTroubleDialog::IsValid();

	if (isValid)
	{
		TRCDKeyContainer *const container = type2RCDKeyContainer(type->C());
		TVirtPtr VP_C(container);
		mstr cause;

		*container << this;

		if (container->date > Today)
			isValid = error("Датата на акта е по-голяма от текущата дата.");
		else if (!container->key.no || !container->key.year)
			isValid = error("Номера и годината са задължителни.");
		else if (!container->Get())
			isValid = false;
		else if (!can_access_exact(container, true, true, &cause))
			isValid = special_match(container) ? warn("%s. %s", str(cause), Q_CONTINUE) : error("%s.", str(cause));
		else if (any(cause))
			isValid = ask_cause(cause);

		if (isValid)
		{
			int count = 0;

			for (int type = 0; type < TROUBLE_COUNT; type++)
			{
				if (any(trouble.name[type]))
				{
					TUserFile uf(Electrics[type].what);
					count++;

					if (!uf.ReadAll(trouble.name[type]->S(), Act()->fileData + type, false, FILE_SIZE_MAX[type],
						Act()->mtime + type))
					{
						isValid = false;
						break;
					}
				}
			}

			if (!count && !edit)
				isValid = error("Изисква се поне един присъединен файл.");
		}
	}

	return isValid;
}

void TRegisterActDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	TypeCBNSelChange();
	kind->SetC(Act()->kind);

	if (edit)
	{
		TWindow *controls[] = { no, year, type, kind, date, NULL };

		for (int i = 0; controls[i]; i++)
			controls[i]->EnableWindow(false);
	}
	else if (LOCK_REGISTER)
	{
		date->SetDate(Today);
		date->EnableWindow(false);
	}

	trouble.BackupNames();
}

void TRegisterActDialog::TypeCBNSelChange()
{
	kinds.cpy(type->C() == TYPE_REQUEST ? KIND_ORDERS :
		type->C() == TYPE_PUNISHMENT_LAW ? KIND_VISIBLE_CRIME_EVENT_INDEXES : KIND_VISIBLE_NON_CRIME_EVENT_INDEXES);

	if (type->C() == TYPE_PUNISHMENT_LAW)
		kinds.cach(KIND_MOTIVES);

	kinds.cach(KIND_OPINION);
	kind->SetXCrit(str(kinds));
}

void TRegisterActDialog::AttachBNClicked()
{
	trouble.Edit(false);
}

void TRegisterActDialog::OKBNClicked()
{
	if (IsValid())
	{
		TRegisterAct *act = Act();
		*act << this;

		if (edit)
		{
			mstr fields("F_TEXT");

			if (act->fileData[TYPE_TEXT].size)
				fields.cat(", F_TEXT_DATA");

			if (act->fileData[TYPE_IMAGE].size)
				fields.cat(", F_IMAGE_DATA");

			if (act->Update(str(fields)))
				CmOk();
		}
		else if (act->Insert())
			CmOk();
	}
}
