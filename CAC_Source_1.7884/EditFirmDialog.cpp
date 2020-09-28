DEFINE_RESPONSE_TABLE1(TEditFirmDialog, TEditNamedDialog)
//{{TEditFirmDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TEditFirmDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditFirmDialog Implementation}}

TEditFirmDialog::TEditFirmDialog(TWindow* parent, TFirm *firm, bool edit, bool full, bool stat)
:
	TEditJudicialDialog(parent, firm, edit, full, stat, IDD_EDIT_FIRM)
{
	new TCharAliasFace(this, IDC_EDIT_FIRM_TYPE, "F_UCN_TYPE", UCNType, NULL);
	new TStringFace(this, IDC_EDIT_FIRM_UCN, "F_UCN", SIZE_OF_UCN);
	name = new TFirmNameFace(this, IDC_EDIT_FIRM_NAME, "F_NAME", SIZE_OF_FIRM_NAME);

	new TUCNFace(this, IDC_EDIT_FIRM_UFN, "F_UFN", UCN_FIRM_UFN);
	bulstat = new TBulStatFace(this, IDC_EDIT_FIRM_BULSTAT, "F_BULSTAT");
	country = new TStringAliasFace(this, IDC_EDIT_FIRM_COUNTRY, "F_COUNTRY", Country, NULL);
	backup = new TCheckFace(this, IDC_EDIT_FIRM_BACKUP, "F_BACKUP");
	oldFirm << firm;
}

bool TEditFirmDialog::CommitBackup()
{
	if (backup->GetCheck() == BF_CHECKED)
	{
		TOldFirm tOldFirm;
		tOldFirm << oldFirm;

		if (tOldFirm.Try())
		{
			if (ask("Вече има предишно име за лицето: %s. Желаете ли да го заместите?", tOldFirm.name))
			{
				tOldFirm << oldFirm;
				return tOldFirm.Update();
			}
		}
		else
			return oldFirm.Insert();
	}

	return true;
}

void TEditFirmDialog::SetupWindow()
{
	TEditJudicialDialog::SetupWindow();

	backup->SetCheck(BF_UNCHECKED);
	backup->EnableWindow(full);
}

void TEditFirmDialog::OKBNClicked()
{
	if (IsValid())
	{
		if (CommitBackup())
		{
			*Group() << this;

			if (Group()->Save(edit))
				CmOk();
			else
				backup->SetCheck(BF_UNCHECKED);
		}
	}
}
