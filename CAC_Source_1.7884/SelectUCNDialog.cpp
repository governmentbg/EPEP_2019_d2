DEFINE_RESPONSE_TABLE1(TSelectUCNDialog, TSelectUCNBaseDialog)
//{{TSelectUCNDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_LBN_SELCHANGE(IDC_SELECT_UCN_GROUPS, GroupsLBNSelChange),
	EV_BN_CLICKED(IDSEARCH, SearchBNClicked),
	EV_BN_CLICKED(IDADDRESS, AddressBNClicked),
	EV_COMMAND_ENABLE(IDADDRESS, AddressBNEnable),
	EV_BN_CLICKED(IDC_SELECT_UCN_RESEARCH, ResearchBNClicked),
	EV_EN_UPDATE(IDC_SELECT_UCN_CITIZEN_NAME, NameENUpdate),
	EV_EN_UPDATE(IDC_SELECT_UCN_RENAME, NameENUpdate),
	EV_EN_UPDATE(IDC_SELECT_UCN_FAMILY, NameENUpdate),
	EV_EN_UPDATE(IDC_SELECT_UCN_FIRM_NAME, NameENUpdate),
	EV_EN_UPDATE(IDC_SELECT_UCN_DAMN_NAME, NameENUpdate),
	EV_EN_UPDATE(IDC_SELECT_UCN_SENDER_NAME, NameENUpdate),
	EV_EN_UPDATE(IDC_SELECT_UCN_UCN, UCNENUpdate),
//{{TSelectUCNDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSelectUCNDialog Implementation}}

TSelectUCNDialog::TSelectUCNDialog(TWindow* parent, TFoundGroup *ucnGroup, const char *ucnTypes, bool tAllowAdd,
	const char *involvements, bool active, ucn_action tAction, bool stat)
:
	TSelectUCNBaseDialog(parent, ucnGroup, ucnTypes, involvements, active, stat, IDD_SELECT_UCN), allowAdd(tAllowAdd),
		action(tAction)
{
}

void TSelectUCNDialog::OKBNClicked()
{
	if (IsValid())
	{
		// 2008:057 LPR: new search: selected group or fields
		TUCNGroup *tUCNGroup = groups->UCNGroup();
		char tInvolvement = involvement ? involvement->C() : '\0';
		bool checknown = tInvolvement && strchr(INVOLVEMENT_CHECK_PERSONS, tInvolvement);
		char lastUCNCode = ucn2code(lastUXNType);
		// 2016:259 nasty hacks for e-portal
		bool eportSide = involvements == INVOLVEMENT_EPORTALS && involvement->C() == INVOLVEMENT_EPORT_SIDE;
		bool found, empty;

		// N.B. keeping all data in ucn groups is slower than re-Try()
		*UCNGroup() << this;
		if (tUCNGroup)
		{
			*UCNGroup() << tUCNGroup;
			empty = false;
		}
		else
			empty = *UCNGroup()->ucn == '\0';

		if (empty)
			found = false;
		else
		{
			tUCNGroup = TNamedGroup::NewXGroup(lastUXNType);
			TVirtPtr VP(tUCNGroup);

			*tUCNGroup << UCNGroup();
			found = tUCNGroup->Try();

			if (found)
				*UCNGroup() << tUCNGroup;
		}

		if (found)
		{
			if (checknown)
			{
				TKnownPerson person;

				person << UCNGroup();
				person.involvement = involvement2known(person.involvement);

				if (!person.Try())
				{
					found = error("��������� ���� (� ������� %s) ����������, �� �� � ������������ � "
						"������������ \"��������� � �������� ������\". �������� �� ��� �������������� �� "
						"���������.", Involvement->Seek(tInvolvement));
				}
				else if (!(person.flags & FLAG_ACTIVE))
				{
					found = ask("��������� ���� (� ������� %s) ����������, �� � ������� ���� ��������� � "
						"������������ \"��������� � �������� ������\". %s", Involvement->Seek(tInvolvement),
						Q_CONTINUE);
				}
			}
			else if (UCNGroup()->ucnType == UCN_CITIZEN_UCN)
			{
				if (is_internal(UCNGroup()->ucn))
					found = error("������ � ��������� �� ����������.");
			}
			else if (eportSide)
				found = error(EPORTAL_NAME " ������� ��� �� ���������.");

			if (found)
			{
				if (action)
				{
					// overlapping doesn't look good
					ShowWindow(SW_HIDE);
					action(this, UCNGroup());
					ShowWindow(SW_SHOW);
				}
				else
					CmOk();
			}
		}
		else if (checknown)
		{
			error("������ (� ������� %s) �� ����������, � �� �� �� ������ ������ ����� �� ���� ��������� ���� "
				"������������ \"��������� � �������� ������\". �������� �� ��� �������������� �� ���������.",
				Involvement->Seek(tInvolvement));
		}
		// 2008:057 LPR: don't say "not found" (below) if not selected
		// 2016:070 LPR: never add companies from here
		else if (empty && (!allowAdd || lastUXNType == UXN_DAMN))
			error("�������� ������������ ����.");
		// 2016:070 LPR: never add senders from here
		else if (empty && lastUXNType == UXN_SENDER)
			error("�������� ����������� ��������.");
		// 2004:335 LPR: the branches below had to be fully rewritten
		else if (!allowAdd)
			error("������ �� ���������� � �� � ��������� �������� �� ���� ����. �������� ������������ ����.");
		else if (empty && !strchr(ucnTypes, lastUCNCode))
			error("����� �� ������ � ������������.");
		else if (empty && eportSide)
			error(EPORTAL_NAME " ������� ��� �� ���������.");
		else if (ask(empty ? "������� �� �� ��������� ���� ����?" :
			"������ �� ����������. ������� �� �� ��������� ���� ����?"))
		{
			TString *nameField = (TString *) UCNGroup()->Find("F_NAME");
			// we already checked that a code is allowed
			if (empty)
				UCNGroup()->ucnType = lastUCNCode;
			if (nameField)
				nameField->SetS(name->S());
			CloseDialog(IDADD);
		}
	}
}

void TSelectUCNDialog::GroupsLBNSelChange()
{
	if (groups->UCNGroup())
		ucn->SetS("");
	else
		UpdateButtons(false);
}

void TSelectUCNDialog::SearchBNClicked()
{
	if (Search(false))
		ok->EnableWindow(true);
}

void TSelectUCNDialog::AddressBNClicked()
{
	groups->Address();
}

void TSelectUCNDialog::AddressBNEnable(TCommandEnabler &tce)
{
	tce.Enable(groups->UCNGroup());
}

void TSelectUCNDialog::ResearchBNClicked()
{
	if (Search(true))
		ok->EnableWindow(true);
}

void TSelectUCNDialog::NameENUpdate()
{
	groups->SetSelIndex(-1);
	UpdateButtons(true);
}

void TSelectUCNDialog::UCNENUpdate()
{
	if (*ucn->S())
		groups->SetSelIndex(-1);

	UpdateButtons(false);
}

void TSelectUCNDialog::UpdateButtons(bool forceSearch)
{
	const char *tUCN = ucn->S();
	bool ucnReady = groups->UCNGroup() || (any(tUCN) && ucn_valid(uxnType->C(), tUCN));

	ok->EnableWindow(ucnReady);
	if (forceSearch || !ucnReady)
	{
		SetDefaultId(search->GetId());
		search->SetStyle(search->GetStyle() | BS_DEFPUSHBUTTON, true);
		ok->SetStyle(ok->GetStyle() & ~BS_DEFPUSHBUTTON, true);
	}
	else
	{
		SetDefaultId(ok->GetId());
		search->SetStyle(search->GetStyle() & ~BS_DEFPUSHBUTTON, true);
		ok->SetStyle(ok->GetStyle() | BS_DEFPUSHBUTTON, true);
	}
}
