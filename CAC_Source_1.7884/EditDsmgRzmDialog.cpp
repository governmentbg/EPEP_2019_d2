DEFINE_RESPONSE_TABLE1(TEditDsmgRzmDialog, TFzlDialog)
//{{TEditDsmgRzmDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_EDIT_NKZPNE_VID, VidCBNSelChange),
//{{TEditDsmgRzmDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditDsmgRzmDialog Implementation}}

TEditDsmgRzmDialog::TEditDsmgRzmDialog(TWindow *parent, FZL *fzl, TXMLGroup *group, const char *nmrnpr, int resId)
:
	TFzlDialog(parent, fzl, group, -1, nmrnpr, resId)
{
}

bool TEditDsmgRzmDialog::IsValid()
{
	bool isValid = TFzlDialog::IsValid();

	if (isValid)
	{
		long vidL = vid->L();

		if (EisVal->Contains(nmk_nkzvid_rzm_dsmg, vidL) && !den->L() && !sdc->L() && !msc->L() && !gdn->L())
			isValid = error("Размер на наказание е задължителен при наказание %s.", EisVal->Seek(nmk_nkzvid, vidL));
		else if (EisVal->Contains(nmk_nkzvid_rzm_leva, vid->L()) && rzm->M() == 0.0)
			isValid = error("Размер на глоба е задължителен при наказание %s.", EisVal->Seek(nmk_nkzvid, vidL));
	}

	return isValid;
}

void TEditDsmgRzmDialog::SetupWindow()
{
	TFzlDialog::SetupWindow();
	VidCBNSelChange();
}

void TEditDsmgRzmDialog::VidCBNSelChange()
{
	if (EisVal->Contains(nmk_nkzvid_rzm_leva, vid->L()))
		EnableEis(rzm);
	else
	{
		rzm->EnableWindow(false);
		rzm->SetM(0.0);
	}

	// 2012:335
	if (EisVal->Contains(nmk_nkzvid_rzm_dsmg, vid->L()))
	{
		EnableEis(den);
		EnableEis(sdc);
		EnableEis(msc);
		EnableEis(gdn);
	}
	else
	{
		den->EnableWindow(false);
		den->SetL(0);
		sdc->EnableWindow(false);
		sdc->SetL(0);
		msc->EnableWindow(false);
		msc->SetL(0);
		gdn->EnableWindow(false);
		gdn->SetL(0);
	}
}
