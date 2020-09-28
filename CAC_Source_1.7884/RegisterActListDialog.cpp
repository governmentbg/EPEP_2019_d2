DEFINE_RESPONSE_TABLE1(TRegisterActListDialog, TGroupListDialog)
//{{TRegisterActListDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_REGISTER_ACT_LIST_KIND, KindCBNSelChange),
	EV_EN_UPDATE(IDC_REGISTER_ACT_LIST_DATE, DateENUpdate),
	EV_COMMAND_ENABLE(IDVIEW, UnattachedEnable),
	EV_BN_CLICKED(IDVIEW, ViewBNClicked),
	EV_COMMAND_ENABLE(IDEDIT, UnattachedEnable),
	EV_COMMAND_ENABLE(IDDELETE, UnattachedEnable),
//{{TRegisterActListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TRegisterActListDialog Implementation}}

TRegisterActListDialog::TRegisterActListDialog(TWindow* parent, int resId)
:
	TGroupListDialog(parent, resId),
		kinds(!strcmp(Default->colleges, C2S[TYPE_PUNISHMENT_LAW]) ? KIND_VISIBLE_CRIME_EVENT_INDEXES :
			strchr(Default->colleges, TYPE_PUNISHMENT_LAW) ? KIND_VISIBLE_EVENT_INDEXES :
			KIND_VISIBLE_NON_CRIME_EVENT_INDEXES)
{
	if (strchr(Default->colleges, TYPE_PUNISHMENT_LAW))
		kinds.cach(KIND_MOTIVES);

	kinds.cach(KIND_OPINION);			
	kind = new TCharAliasFace(this, IDC_REGISTER_ACT_LIST_KIND, "F_KIND", Kind, str(kinds), ALIAS_EMPTY);
	date = new TDateFace(this, IDC_REGISTER_ACT_LIST_DATE, "F_DATE", true);
	new TDateFaceUpDown(this, IDC_REGISTER_ACT_LIST_UPDN, date);
	registerActs = new TRegisterActListWindow(this, IDC_GROUP_LIST_WINDOW, date);
	new TPushButton(this, IDVIEW, IDQUERY);
}

void TRegisterActListDialog::UnattachedEnable(TCommandEnabler &tce)
{
	TRegisterAct *act = (TRegisterAct *) registerActs->Group();
	tce.Enable(act && !act->targetKind);
}

void TRegisterActListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();
	KindCBNSelChange();
}

void TRegisterActListDialog::KindCBNSelChange()
{
	registerActs->SetCrit(kind->C(), date->Date());
}

void TRegisterActListDialog::DateENUpdate()
{
	if (IsFlagSet(wfFullyCreated) && date_valid(date->S(), true))
		registerActs->SetCrit(kind->C(), date->Date());
}

void TRegisterActListDialog::ViewBNClicked()
{
	registerActs->View();
}
