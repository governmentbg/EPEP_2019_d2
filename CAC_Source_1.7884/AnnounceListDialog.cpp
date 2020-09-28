DEFINE_RESPONSE_TABLE1(TAnnounceListDialog, TGroupListDialog)
//{{TAnnounceListDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_ANNOUNCE_LIST_TYPE, DropCBNSelChange),
	EV_CBN_SELCHANGE(IDC_ANNOUNCE_LIST_SENDER, DropCBNSelChange),
	EV_BN_CLICKED(IDPREV, PrevBNClicked),
	EV_COMMAND_ENABLE(IDPREV, FindBNEnable),
	EV_BN_CLICKED(IDNEXT, NextBNClicked),
	EV_COMMAND_ENABLE(IDNEXT, FindBNEnable),
	EV_BN_CLICKED(IDSEARCH, SearchBNClicked),
	EV_BN_CLICKED(IDADD, AddBNClicked),
	EV_BN_CLICKED(IDCOPY, CopyBNClicked),
	EV_BN_CLICKED(IDEDIT, EditBNClicked),
	EV_BN_CLICKED(IDSERVE, ServeBNClicked),
	EV_BN_CLICKED(IDASSIGN, AssignBNClicked),
	EV_EN_SETFOCUS(IDC_ANNOUNCE_LIST_DATE, SetSearch),
	EV_CBN_SETFOCUS(IDC_ANNOUNCE_LIST_TYPE, SetSearch),
	EV_CBN_SETFOCUS(IDC_ANNOUNCE_LIST_SENDER, SetSearch),
	EV_LBN_SETFOCUS(IDC_ANNOUNCE_LIST_DELIVERERS, SetSearch),
	EV_COMMAND_ENABLE(IDCOPY, SelectEnable),
	EV_COMMAND_ENABLE(IDEDIT, SelectEnable),
	EV_COMMAND_ENABLE(IDDELETE, DeleteBNEnable),
	EV_COMMAND_ENABLE(IDSERVE, SelectEnable),
	EV_COMMAND_ENABLE(IDASSIGN, AssignBNEnable),
	EV_MESSAGE(WM_FOCUS_FACE, EvFocusFace),
	EV_COMMAND_ENABLE(IDEXPORT, ExportBNEnable),
	EV_BN_CLICKED(IDEXPORT, ExportBNClicked),
	EV_COMMAND_ENABLE(IDIMPORT, ImportBNEnable),
	EV_BN_CLICKED(IDIMPORT, ImportBNClicked),
//{{TAnnounceListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TAnnounceListDialog Implementation}}

TAnnounceListDialog::TAnnounceListDialog(TWindow* parent, TAnnounceGroup *tGroup)
:
	TGroupListDialog(parent, IDD_ANNOUNCE_LIST), group(tGroup), export(NULL)
{
	dateLabel = new TStatic(this, IDC_ANNOUNCE_LIST_DATE_LABEL);
	date = new TDateFace(this, IDC_ANNOUNCE_LIST_DATE, "F_DATE");
	new TDateFaceUpDown(this, IDC_ANNOUNCE_LIST_DATE_UPDN, date);
	build_ctypes(types, TYPE_ANNOUNCABLES, false);

	if (group->type)
	{
		type = new TCharAliasFace(this, IDC_ANNOUNCE_LIST_TYPE, "F_TYPE", Type, types);
		sender = NULL;
		assign = new TButton(this, IDASSIGN);
	}
	else
	{
		type = NULL;
		sender = new TReceiverAliasFace(this, IDC_ANNOUNCE_LIST_SENDER, "F_SENDER", Receiver, C2S[UCN_INSTANCE_EXTERNAL]);
		assign = NULL;
	}

	if (COURT_SIME)
	{
		export = new TButton(this, IDEXPORT);
		import = new TButton(this, IDIMPORT);
	}

	dropLabel = new TStatic(this, IDC_ANNOUNCE_LIST_DROP_LABEL);
	deliverers = new TDelivererListBox(this, IDC_ANNOUNCE_LIST_DELIVERERS);
	findText = new TStringFace(this, IDC_ANNOUNCE_LIST_FIND_TEXT, "F_FIND_TEXT", SIZE_OF_FIND_ANNOUNCE_TEXT);
	announces = new TAnnounceListWindow(this, IDC_GROUP_LIST_WINDOW, group, types, deliverers->Array());
	search = new TPushButton(this, IDSEARCH);
	add = new TPushButton(this, IDADD);
	edit = new TPushButton(this, IDEDIT);
	delivered = new TCheckFace(this, IDC_ANNOUNCE_LIST_DELIVERED, "F_DELIVERED");
}

void TAnnounceListDialog::SetSearch()
{
	SetDefault(edit, search);
}

void TAnnounceListDialog::Add(bool copy)
{
	if (IsValid())
	{
		*group << this;
		group->court = atoi(group->sender);
		announces->AddRepeat(copy);
	}
}

void TAnnounceListDialog::DropCBNSelChange()
{
	SearchBNClicked();
}

void TAnnounceListDialog::PrevBNClicked()
{
	announces->FindText(findText->S(), -1);
}

void TAnnounceListDialog::NextBNClicked()
{
	announces->FindText(findText->S(), +1);
}

void TAnnounceListDialog::FindBNEnable(TCommandEnabler &tce)
{
	tce.Enable(*findText->S());
}

void TAnnounceListDialog::SearchBNClicked()
{
	if (IsValid())
	{
		*group << this;
		group->court = atoi(group->sender);
		deliverers->GetMark(&group->deliverers);

		announces->Refresh();

		if (assign)
			assign->EnableWindow(announces->Assignable());
	}
}

void TAnnounceListDialog::AddBNClicked()
{
	Add(false);
}

void TAnnounceListDialog::CopyBNClicked()
{
	Add(true);
}

void TAnnounceListDialog::EditBNClicked()
{
	*group << this;
	TGroupListDialog::EditBNClicked();
}

void TAnnounceListDialog::ServeBNClicked()
{
	announces->ServeAnnounce();
}

void TAnnounceListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();

	if (type)
	{
		dateLabel->SetText("Дата на изготвяне");
		type->ShowWindow(SW_SHOW);
		dropLabel->SetText("Вид д&ело");
		SetCaption("Призовки/съобщения на текущия съд");
		assign->ShowWindow(SW_SHOW);
	}
	else
		sender->ShowWindow(SW_SHOW);

	if (COURT_SIME)
	{
		export->ShowWindow(SW_SHOW);
		import->ShowWindow(SW_SHOW);
	}

	delivered->SetCheck(BF_GRAYED);
}

void TAnnounceListDialog::DeleteBNEnable(TCommandEnabler &tce)
{
	TAnnounce *announce = announces->Announce();
	tce.Enable(announce && !announce->subpoena && can_func(RIGHT_DELETE));
}

void TAnnounceListDialog::AssignBNClicked()
{
	announces->Assign();
}

void TAnnounceListDialog::AssignBNEnable(TCommandEnabler &tce)
{
	tce.Enable(announces->Assignable());
}

long TAnnounceListDialog::EvFocusFace(TParam1 wParam, TParam2)
{
	if (wParam)
		SetDefault(search, announces->Array()->Count() ? add : edit);

	return MW_FACE_FOCUSED;
}

void TAnnounceListDialog::ExportBNEnable(TCommandEnabler &tce)
{
	bool result = false;

	if (COURT_SIME)
	{
		if (type)
		{
			int fake = 0;
			deliverers->GetMark(&group->deliverers);

			for (int i = 0; i < group->deliverers.Count(); i++)
				if (!is_real_ucn(UCN_CITIZEN_UCN, group->deliverers[0]->ucn))
					fake++;

			result = group->deliverers.Count() > 0 && fake <= 1;
		}
		else
			result = atob(sender);
	}

	tce.Enable(result);
}

void TAnnounceListDialog::ExportBNClicked()
{
	if (IsValid())
	{
		*group << this;

		if (type)
			export_subpoenas(this, group->date, group->deliverers);
		else
		{	// 2018-06-11 returned date support
			TDummyGroup tGroup;
			CDate subpoenaReturned;

			tGroup.Add(new TDate("F_DATE", &subpoenaReturned, true));
			do subpoenaReturned += -1; while (!subpoenaReturned.Work());

			if (get_date(this, &tGroup, "Дата на връщане", true, false))
				export_announces(this, subpoenaReturned, atoi(group->sender));
		}
	}
}

void TAnnounceListDialog::ImportBNEnable(TCommandEnabler &tce)
{
	tce.Enable(COURT_SIME);
}

void TAnnounceListDialog::ImportBNClicked()
{
	if (type)
	{
		if (import_subpoenas(this))
		{
			CDate prevWork = Today;

			do
				prevWork += -1;
			while (!prevWork.Work());

			date->SetDate(prevWork);
			deliverers->SelectAll(false);
			findText->SetS("");
			delivered->SetCheck(BF_CHECKED);
			SearchBNClicked();
		}
	}
	else
	{
		long court = import_announces(this, date->Date(), deliverers->Array());

		if (court)
		{
			date->SetDate(Today);
			sprintf(group->sender, "%010d", court);
			sender->SetUCN(group->sender);
			deliverers->SelectAll(false);
			findText->SetS("");
			delivered->SetCheck(BF_GRAYED);
			SearchBNClicked();
		}
	}
}
