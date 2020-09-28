DEFINE_RESPONSE_TABLE1(TKnownPersonListDialog, TGroupListDialog)
//{{TKnownPersonListDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_KNOWN_PERSON_LIST_INVOLVEMENT, InvolvementCBNSelChange),
	EV_LVN_ITEMCHANGED(IDC_GROUP_LIST_WINDOW, PersonsLVNItemChanged),
	EV_BN_CLICKED(IDREGISTRY, RegistryBNClicked),
	EV_BN_CLICKED(IDLIST, ListBNClicked),
	EV_BN_CLICKED(IDRIGHT, RightBNClicked),
	EV_BN_CLICKED(IDACTIVE, ActiveBNClicked),
	EV_BN_CLICKED(IDIMPORT, ImportBNClicked),
	EV_BN_CLICKED(IDEXPORT, ExportBNClicked),
	EV_COMMAND_ENABLE(IDREGISTRY, RegistryBNEnable),
	EV_COMMAND_ENABLE(IDACTIVE, SelectEnable),
	EV_BN_CLICKED(IDLOAD, LoadBNClicked),
	EV_COMMAND_ENABLE(IDADDRESS, SelectEnable),
	EV_BN_CLICKED(IDADDRESS, AddressBNClicked),
//{{TKnownPersonListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TKnownPersonListDialog Implementation}}

TKnownPersonListDialog::TKnownPersonListDialog(TWindow* parent, int resId)
:
	TGroupListDialog(parent, resId)
{
	involvement = new TCharAliasFace(this, IDC_KNOWN_PERSON_LIST_INVOLVEMENT, "F_INVOLVEMENT", Involvement,
		INVOLVEMENT_VISIBLE_PERSONS);
	persons = new TKnownPersonListWindow(this, IDC_GROUP_LIST_WINDOW);
	active = new TButton(this, IDACTIVE);
	registry = new TButton(this, IDREGISTRY);
	import = new TButton(this, IDIMPORT);
	export = new TButton(this, IDEXPORT);
	new TTwinButton(this, IDLIST, IDRIGHT);
	load = new TPushButton(this, IDLOAD);
}

void TKnownPersonListDialog::ActiveChanged()
{
	TKnownPerson *person = persons->Person();
	active->SetCaption(active_button_text(person, person->flags));
}

void TKnownPersonListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();

	InvolvementCBNSelChange();
	import->EnableWindow(can_admin(RIGHT_IMPORT));
	export->EnableWindow(can_admin(RIGHT_IMPORT));

	if (!CHROME_LOAD_KNOWNS)
	{
		load->EnableWindow(true);
		load->ShowWindow(SW_SHOW);
	}
}

void TKnownPersonListDialog::InvolvementCBNSelChange()
{
	persons->SetInvolvement(involvement->C());
	registry->SetCaption(involvement->C() == INVOLVEMENT_JUROR ? "&Педагог" :
		involvement->C() == INVOLVEMENT_LEGALESE ? "&Личен номер" : "&Регистр. номер");
}

void TKnownPersonListDialog::PersonsLVNItemChanged(TLwNotify &lwn)
{
	// 2015:135 FIX: uNewState & LVIS_DROPHILITED == 0 on Add
	if ((lwn.uChanged & LVIS_DROPHILITED) && persons->Person())
		ActiveChanged();
}

void TKnownPersonListDialog::RegistryBNClicked()
{
	persons->Registry();
}

void TKnownPersonListDialog::ListBNClicked()
{
	persons->List(FLAG_ACTIVE);
}

void TKnownPersonListDialog::RightBNClicked()
{
	persons->List(FLAG_NULL);
}

void TKnownPersonListDialog::ActiveBNClicked()
{
	if (persons->Active())
		ActiveChanged();
}

void TKnownPersonListDialog::ImportBNClicked()
{
	import_known(this);
	log(LOG_IMPORT, CM_IMPORT, "участници");
}

void TKnownPersonListDialog::ExportBNClicked()
{
	//if (ask("Еспортират се само лица регистрирани с ЕГН и участие Прокурор, Вещо лице или Лице с юридическа правоспособност. Желаете ли да продължите?"))
	{
		export_known(this);
		log(LOG_EXPORT, CM_EXPORT, "участници");
	}
}

void TKnownPersonListDialog::RegistryBNEnable(TCommandEnabler &tce)
{
	if (!involvement->GetDroppedState())
	{
		tce.Enable(persons->Person() && (strchr(INVOLVEMENT_CSJIDS, involvement->C()) ||
			involvement->C() == INVOLVEMENT_JUROR || involvement->C() == INVOLVEMENT_LEGALESE));
	}
}

void TKnownPersonListDialog::LoadBNClicked()
{
	persons->Refresh();
}

void TKnownPersonListDialog::AddressBNClicked()
{
	persons->Addresses();
}
