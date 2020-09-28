DEFINE_RESPONSE_TABLE1(TUserDistributionDialog, TSelectSubjectsDialog)
//{{TUserDistributionDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDEQUATE, EquateBNClicked),
//{{TUserDistributionDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TUserDistributionDialog Implementation}}

TUserDistributionDialog::TUserDistributionDialog(TWindow* parent, TUser *user, TGArray<TSubject> *subjects)
:
	TSelectSubjectsDialog(parent, user, subjects, NULL, IDD_USER_DISTRIBUTION)
{
	new TCharListFace(this, IDC_USER_DISTRIBUTION_KINDS, "F_LAW_KINDS", Kind, KIND_LAWSUITS, SIZE_OF_LAW_KINDS);
	percent = new TLongFace(this, IDC_USER_DISTRIBUTION_PERCENT, "F_PERCENT", SIZE_OF_PERCENT);
	preload = new TLongFace(this, IDC_USER_DISTRIBUTION_PRELOAD, "F_PRELOAD", SIZE_OF_PRELOAD);
	fullLoad = new TLongFace(this, IDC_USER_DISTRIBUTION_FULL_LOAD, "F_FULL_LOAD", SIZE_OF_FULL_LOAD);
}

void TUserDistributionDialog::OKBNClicked()
{
	if (IsValid())
	{
		*User() << this;

		if (*fullLoad->S())
			User()->preload = fullLoad->L() - User()->RawLoadA();

		if (User()->Update("F_LAW_KINDS, F_PERCENT, F_PRELOAD"))
		{
			mstr m;
			m.printf("разпр.данни: %s %ld%% %ld", User()->ucn, User()->percent, User()->preload);
			log(LOG_EDIT, CM_ADMIN_USERS, str(m));

			m.clear();
			m.printf("разпр.дела: %s", User()->lawKinds);
			log(LOG_EDIT, CM_ADMIN_USERS, str(m));

			TSelectSubjectsDialog::OKBNClicked();
		}
	}
}

void TUserDistributionDialog::EquateBNClicked()
{
	if (IsValid())
	{
		*User() << this;
		subjects->GetMark(array);
		calculate_preload(this, User(), array);
		*User() >> this;
		fullLoad->SetL(round(User()->FullLoad()));
	}
}

bool TUserDistributionDialog::IsValid()
{
	bool isValid = TSelectSubjectsDialog::IsValid();

	if (isValid)
	{
		if (percent->L() < 1 || percent->L() > 100)
			isValid = error("Невалиден процент.");
	}

	return isValid;
}

void TUserDistributionDialog::SetupWindow()
{
	TSelectSubjectsDialog::SetupWindow();

	// 2007:257 LPR: rawload must be initialized
	User()->CalcLoad(false);
	fullLoad->SetL(round(User()->FullLoad()));
}
