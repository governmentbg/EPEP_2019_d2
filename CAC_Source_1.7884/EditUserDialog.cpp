DEFINE_RESPONSE_TABLE1(TEditUserDialog, TGroupDialog)
//{{TEditUserDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_CBN_SELCHANGE(IDC_EDIT_USER_POST, PostCBNSelChange),
	EV_BN_CLICKED(IDC_EDIT_USER_CLEAR, ClearBNClicked),
	EV_COMMAND_ENABLE(IDC_EDIT_USER_CLEAR, ClearBNEnable),
	EV_BN_CLICKED(IDC_EDIT_USER_DOMAIN, DomainBNClicked),
//{{TEditUserDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditUserDialog Implementation}}

TEditUserDialog::TEditUserDialog(TWindow* parent, TUser *user, int resId)
:
	TGroupDialog(parent, user, resId)
{
	name = new TStringFace(this, IDC_EDIT_USER_NAME, "F_NAME", SIZE_OF_USER_NAME);
	login = new TStringFace(this, IDC_EDIT_USER_LOGIN, "F_LOGIN", SIZE_OF_NAME);
	password = new TStringFace(this, IDC_EDIT_USER_PASSWORD, "F_PASSWORD", SIZE_OF_ALIAS);
	rePassword = new TStringFace(this, IDC_EDIT_USER_REPASSWORD, "F_REPASSWORD", SIZE_OF_ALIAS);

	menus = new TCharListFace(this, IDC_EDIT_USER_MENUS, "F_MENUS", Right, RIGHT_MENUS, SIZE_OF_MENUS);
	functions = new TCharListFace(this, IDC_EDIT_USER_FUNCTIONS, "F_FUNCTIONS", Right, RIGHT_FUNCTIONS, SIZE_OF_FUNCTIONS);
	colleges = new TCharListFace(this, IDC_EDIT_USER_COLLEGES, "F_COLLEGES", CollegeRight, NULL, SIZE_OF_COLLEGES);
	compositions = new TCharListFace(this, IDC_EDIT_USER_COMPOSITIONS, "F_COMPOSITIONS", CharComposition, NULL, SIZE_OF_COMPOSITIONS);

	post = new TCharAliasFace(this, IDC_EDIT_USER_POST, "F_POST", Post, POST_VISIBLES);
	new TCharAliasFace(this, IDC_EDIT_USER_KIND, "F_KIND", Kind, KIND_LAWSUITS);
	new TCheckFace(this, IDC_EDIT_USER_ACTIVE, "F_FLAGS_ACTIVE");

	new TDateFace(this, IDC_EDIT_USER_DATE, "F_DATE");	// 2003:051
	failogs = new TLongFace(this, IDC_EDIT_USER_FAILOGS, "F_FAILOGS", SIZE_OF_DIGIT);
	compos = new TLongAliasFace(this, IDC_EDIT_USER_COMPOS, "F_COMPOS", Composition, COMPOSITION_ALLS, ALIAS_EMPTY);
	domain = new TCheckFace(this, IDC_EDIT_USER_DOMAIN, "F_ACTYPE_DOMAIN");
}

void TEditUserDialog::OKBNClicked()
{
	if (IsValid())
	{
		bool tMenus = *menus->S() != '\0';
		bool tFunctions = *functions->S() != '\0';
		bool tColleges = *colleges->S() != '\0';
		bool tCompositions = *compositions->S() != '\0';
		char tPost = User()->post;
		long tFlags = User()->flags;	// 2004:131 LRQ: was missing
		// 2005:041 LPR: for md5 back
		char md5sum[SIZE_OF_MD5SUM];
		bool updated = false;

		strcpy(md5sum, User()->md5sum);
		*Group() << this;

		if ((tMenus && tFunctions && tColleges && tCompositions) ||
			ask("С тези настройки потребителя няма да може да ползва програмата. Желаете ли да продължите?"))
		{
			if (any(password->S()) && !User()->actype)
			{
				// 2006:080 LPR: no password in user
				calc_pass(User(), password->S());
			}
			else
			{
				// 2005:087 LRQ: BUGFIX: was missing
				strcpy(User()->md5sum, md5sum);
			}

			updated = Group()->Update();

			if (updated)
			{
				mstr m;
				m.printf("потребител: %s, %c, %ld", User()->ucn, User()->post, User()->flags);
				log(LOG_EDIT, CM_ADMIN_USERS, str(m));

				m.clear();
				m.printf("разпр.дела: %s", User()->lawKinds);
				log(LOG_EDIT, CM_ADMIN_USERS, str(m));
			}
		}

		if (updated)
			CmOk();
		else
		{
			User()->post = tPost;
			User()->flags = tFlags;
			strcpy(User()->md5sum, md5sum);
		}
	}
}

bool TEditUserDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		bool tDomain = domain->GetCheck() == BF_CHECKED;
		const char *tPassword = password->S();

		if (any(tPassword) && !tDomain)
		{
			if (strcmp(rePassword->S(), tPassword))
				isValid = error("Паролите не са еднакви.");
			else if (strlen(tPassword) < 5)
			{
			#if COURT_TYPE == COURT_AREAL
				// 2017:081 URQ: weak password
				if (COURT_CODE == COURT_VAROS && post->C() == POST_DELIVERER &&
					strlen(tPassword) == 4 && long_valid(tPassword))
				{
					return true;
				}
			#endif  // AREAL
				isValid = error("Паролата е трябва да е поне 5 символа.");
			}
		#if TESTVER
			else if (!strcmp(login->S(), "????????") && !strcmp(tPassword, "????????"))
				isValid = true;
		#endif  // TESTVER
			else
			{
				int has_digit = 0, has_punct = 0, has_alpha = 0;

				for (const char *s = tPassword; *s; s++)
				{
					if (isdigit(*s))
						has_digit = 1;
					else if (ispunct(*s))
						has_punct = 1;
					else if (isalpha(*s))
						has_alpha = 1;
				}

				if (has_digit + has_punct + has_alpha < 2)
					isValid = error("Паролата не е достатъчно сложна.");
			}
		}
		else if (tDomain && !TConfigure::GetCurrent("serial", "DOMAIN_NAME", true))
			isValid = false;
		else if (tDomain && !any(DOMAIN_NAME))
			isValid = error("Включено е разписване чрез домейн, но в конфигурацията не е зададено име на домейн.");
	}

	return isValid;
}

void TEditUserDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	if (!strcmp(User()->ucn, SYSAD))
	{
		post->EnableWindow(false);
		domain->EnableWindow(false);
	}
	else
		PostCBNSelChange();

	mstr m;
	print_user(m, User()->ucn);
	name->SetS(str(m));

	DomainBNClicked();
}

void TEditUserDialog::PostCBNSelChange()
{
	adjust_user_profile(post->C(), menus, functions, colleges);
}

void TEditUserDialog::ClearBNClicked()
{
	failogs->SetL(0);
}

void TEditUserDialog::ClearBNEnable(TCommandEnabler &tce)
{
	tce.Enable(failogs->L());
}

void TEditUserDialog::DomainBNClicked()
{
	password->EnableWindow(domain->GetCheck() == BF_UNCHECKED);
	rePassword->EnableWindow(domain->GetCheck() == BF_UNCHECKED);
}
