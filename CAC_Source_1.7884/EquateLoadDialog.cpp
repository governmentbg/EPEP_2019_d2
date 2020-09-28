DEFINE_RESPONSE_TABLE1(TEquateLoadDialog, TGroupDialog)
//{{TEquateLoadDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDEQUATE, EquateBNClicked),
//{{TEquateLoadDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEquateLoadDialog Implementation}}

TEquateLoadDialog::TEquateLoadDialog(TWindow* parent, TGArray<TUser> *tUsers, int resId)
:
	TGroupDialog(parent, NULL, resId), users(tUsers)
{
	load = new TUserLoadListBox(this, IDC_EQUATE_LOAD_LOAD, users, false);
}

void TEquateLoadDialog::OKBNClicked()
{
	for (int i = 0; i < users->Count(); i++)
		(*users)[i]->Update("F_PRELOAD");

	CmOk();
}

void TEquateLoadDialog::EquateBNClicked()
{
	TGArray<TUser> tUsers;
	bool equated = false;

	load->GetMark(&tUsers);

	// 2007:270
	if (tUsers.Count() == 1)
	{
		calculate_preload(tUsers[0], users);
		tUsers.Detach(0);
		equated = true;
	}
	else if (tUsers.Count())
	{
		equate_load(&tUsers);
		tUsers.Clear();
		equated = true;
	}
	else if (ask("Изравняване натоварването на всички съдии?"))
	{
		equate_load(users);
		equated = true;
	}

	if (equated)
		load->Refresh();
}
