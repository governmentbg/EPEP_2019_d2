DEFINE_RESPONSE_TABLE1(TQueryUnattachedDialog, TGroupDialog)
//{{TQueryUnattachedDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TQueryUnattachedDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TQueryUnattachedDialog Implementation}}

TQueryUnattachedDialog::TQueryUnattachedDialog(TWindow* parent, TQueryUnattachedGroup *group, int resId)
:
	TGroupDialog(parent, group, resId)
{
	judges = new TUserListBox(this, IDC_QUERY_UNATTACHED_JUDGES, POST_VISIBLE_JUDGES);
}

bool TQueryUnattachedDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		TGroupArray tJudges;

		judges->GetMark(&tJudges);

		if (!tJudges.Count())
			isValid = error("Няма избран нито един съдия.");
	}

	return isValid;
}

void TQueryUnattachedDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();
	judges->SelectAll();
}

void TQueryUnattachedDialog::OKBNClicked()
{
	if (IsValid())
	{
		judges->GetMark(&QueryUnattachedGroup()->judges);
		CmOk();
	}
}
