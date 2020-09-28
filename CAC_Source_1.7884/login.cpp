#include "cac.h"

// wkb-mini кода в login и LoginDialog е безплатно лицензиран за използване
// в проекти на "Информационно обслужване - АД".

void set_layout_ll(long layout, bool reorder)
{
	if ((get_layout() != LAYOUT_EN_US) != (layout != LAYOUT_EN_US))
		ActivateKeyboardLayout((HKL) layout, reorder ? KLF_REORDER : 0);
}

void set_layout(long layout)
{
	if (CHROME_KEYBOARD_LAYOUT)
		set_layout_ll(layout, CHROME_KEYBOARD_LAYOUT != 1);
}

#include "LoginDialog.h"
#include "LoginDialog.cpp"

struct TLoginError
{
	DWORD code;
	const char *message;
};

static const TLoginError loginErrors[] =
{
	{ ERROR_NO_LOGON_SERVERS, "Не е намерен сървър за проверка на името и паролата" },
	{ ERROR_INVALID_ACCOUNT_NAME, "Неправилно формирано потребителско име" },
	{ ERROR_NO_SUCH_USER, NULL },
	{ ERROR_LOGON_FAILURE, NULL },
	{ ERROR_ACCOUNT_RESTRICTION, NULL },
	{ ERROR_INVALID_LOGON_HOURS, NULL },
	{ ERROR_INVALID_WORKSTATION, NULL },
	{ ERROR_PASSWORD_EXPIRED, NULL },
	{ ERROR_ACCOUNT_DISABLED, NULL },
	{ ERROR_NOT_LOGON_PROCESS, NULL },
	{ ERROR_SPECIAL_ACCOUNT, NULL },
	{ ERROR_SPECIAL_GROUP, NULL },
	{ ERROR_SPECIAL_USER, NULL },
	{ ERROR_LOGON_NOT_GRANTED, NULL },
	{ ERROR_LOGON_TYPE_NOT_GRANTED, NULL },
	{ ERROR_SERVER_DISABLED, "Сървъра за проверка на името и паролата е неактивен" },
	{ ERROR_CANT_ACCESS_DOMAIN_INFO, "Грешка при четене на информация от домейн контролера" },
	{ ERROR_NO_SUCH_DOMAIN, "Липсва домейна за проверка на името и паролата" },
	{ 0, NULL }
};

bool get_login(TWindow *parent, char *password)
{
	DWORD nSize = SIZE_OF_NAME;	// 2008:162 FIX: OF_ALIAS -> OF_NAME

	if (GetUserName(Default->login, &nSize))
		strupr(Default->login);

	char s[SIZE_OF_CONFIG_VALUE];
	TConfigure::GetGlobal("serial", "LOGIN_LIMIT", s);
	LOGIN_LIMIT = atoi(s);

	if (LOGIN_LIMIT < 0)
		LOGIN_LIMIT = 0;
	else if (LOGIN_LIMIT == 1 || LOGIN_LIMIT == 2)
		LOGIN_LIMIT = 3;
	else if (LOGIN_LIMIT > 5)
		LOGIN_LIMIT = 5;

	set_layout(LAYOUT_EN_US);
	// 2005:103 LRQ/LPR: if -> while, abort on cancel only
	while (TLoginDialog(parent, Default).Execute() == IDOK)
	{
		msql m(Default->Name);

		m.AddString("F_LOGIN", Default->login);
		// INTER etc. not allowed; even DOMAIN must be active
		m.AddLong("F_FLAGS", FLAG_ACTIVE);

		if (LOGIN_LIMIT)
		{
			m.Add("");
			m.printf("(F_FAILOGS IS NULL OR F_FAILOGS < %ld)", LOGIN_LIMIT);
		}

		TQuery q(str(m));
		if (q.Read())
		{
			*User << q;

			if (User->actype == USER_DOMAIN)
			{
				if (!TConfigure::GetCurrent("serial", "DOMAIN_NAME", true))
					continue;

				if (!any(DOMAIN_NAME))
				{
					error("За потребителя е включено разписване чрез домейн, но в конфигурацията не е "
						"зададено име на домейн.\n\nОбърнете се към системния администратор.");
					continue;
				}

				HANDLE hLogon;

				if (LogonUser(Default->login, DOMAIN_NAME, password, LOGON32_LOGON_INTERACTIVE,
					LOGON32_PROVIDER_DEFAULT, &hLogon))
				{
					CloseHandle(hLogon);
					return true;
				}

				DWORD lastError = GetLastError();
				const TLoginError *loginError;

				for (loginError = loginErrors; loginError->code; loginError++)
					if (loginError->code == lastError)
						break;

				if (loginError->code)
				{
					mstr m(loginError->message ? loginError->message : "Невалидно име/парола, или потребителя "
						"е неактивен, или има други ограничения върху разписването");

					if (loginError->message || is_sysadmin())
						m.printf(" (%lu)", lastError);

					error("%s.", str(m));
				}
				else
					error("Грешка %lu при опит за разписване в домейн.", (unsigned long) lastError);
			}
			else
			{
				TUser tUser;

				strcpy(tUser.ucn, User->ucn);
				calc_pass(&tUser, password);

				if (!strcmp(tUser.md5sum, User->md5sum))
				{
					if (User->post == POST_INTERNAL)
						error("Името и паролата са резервирани за служебни нужди.");
					else
					{
						if (User->failogs)
						{
							User->failogs = 0;
							User->Update("F_FAILOGS");
						}

						return true;
					}
				}
				else
				{
					User->failogs++;
					User->Update("F_FAILOGS");
					log_inout("Невалидна парола", User->ucn);

					if (User->failogs == LOGIN_LIMIT)
						log_inout("Лимит за брой разписвания", User->ucn);

					error("Невалидно име/парола или потребителя е неактивен.");
				}
			}
		}
		else
		{
			// 2016:032 LPR: don't display login, may be a password
			//log_inout("Невалиден потребител", EMPTY, Default->login);
			log_inout("Невалиден потребител", EMPTY);
			error("Невалидно име/парола или потребителя е неактивен.");
		}

		Sleep(500);
	}

	return false;
}

bool get_login(TWindow *parent)
{
	if (SERIAL_VALID && COURT_MINI && (COURT_MINI == COURT_FLAT || COURT_SIME == 0))
	{
		strcpy(Default->ucn, EMPTY);
		strcpy(Default->menus, RIGHT_QUERIES);
		strcpy(Default->functions, RIGHT_MINIMALS);
		strcpy(Default->colleges, COLLEGE_ALLS);
		strcpy(Default->xColleges, COLLEGE_XALLS);

		Default->post = POST_INTERNAL;
		Default->kind = *KIND_LAWSUITS;
		strcpy(Default->compositions, COMPOSITION_ALLS);
		build_ckinds(Default->xKinds, KIND_LAWSUITS, false);
		log_inout("Начало", Default->ucn);
		return true;
	}

	char password[SIZE_OF_ALIAS];
	DtorZero pwd(password, sizeof password);
	bool result;

	Default->Add(new TString("F_PASSWORD", password, sizeof password));
	result = get_login(parent, password);

	if (result)
	{
		*Default << User;
		// 2008:043 LPR: common college support
		// 2008:246 LPR: multi common colleges
		strcpy(Default->xColleges, Default->colleges);
		stremove(Default->colleges, COLLEGE_COMMONS);

	#if TESTVER
	#else  // TESTVER
		if (COURT_CODE != COURT_SOFGS)
		{
			stremove(Default->functions, RIGHT_ATTACH);
			stremove(Default->functions, RIGHT_EXTERN);
		}
	#endif  // TESTVER

		build_ckinds(Default->xKinds, KIND_LAWSUITS, false);
		log_inout("Начало", Default->ucn);

		if (is_sysadmin())
			update_mini(parent);
	}

	Default->Remove("F_PASSWORD");
	return result;
}
