#include "cac.h"

// ----- TTitleGroup ---------------------------------------------------------
TTitleGroup::TTitleGroup(const char *tTitle)
{
	Add(new TString("F_TITLE", title, sizeof title));
	strcpy(title, tTitle);
}

#define counter OFFICE_GROUP_CAST(TCounterGroup)

void counter_matches(mstr &m, TGroup *group)
{
	if (counter->count == 0)
		m.printf("%s îòãîâàðÿùè íà çàäàäåíèòå êðèòåðèè.", counter->none);
	else if (counter->count == 1)
		m.printf("%s îòãîâàðÿ íà çàäàäåíèòå êðèòåðèè.", counter->one);
	else if (counter->count <= counter->maximum)
		m.printf("%ld %s îòãîâàðÿò íà çàäàäåíèòå êðèòåðèè", counter->count, counter->many);
	else
		m.printf("Ïîâå÷å îò %ld %s îòãîâàðÿò íà çàäàäåíèòå êðèòåðèè", counter->maximum, counter->many);
}

#undef counter

const offield counter_fields[] =
{
	{ "F_COUNT", NULL },
	{ "F_MATCHES", counter_matches },
	{ NULL, NULL }
};

// ----- individual items and functions --------------------------------------
char *AppNameCourt = NULL;
static char CacheCourtArea[SIZE_OF_UCLP_NAME];
static char CacheCourtRegion[SIZE_OF_UCLP_NAME];
static char CacheCourtUCLP[SIZE_OF_UCLP_NAME];
char AppNameTitle[SIZE_OF_APPNAME_TITLE];
static char CacheCourtAddress[SIZE_OF_PARTIAL_ADDRESS];
static char CacheCourtPhone[SIZE_OF_PHONE];
static char CacheCourtEmail[SIZE_OF_EMAIL];
static char CacheCourtPresident[LENGTH_OF_USER_NAME];
static char CacheUserName[SIZE_OF_USER_NAME];
static char CacheUserInitials[SIZE_OF_USER_INITIALS];
static char CacheFullUserName[SIZE_OF_CITIZEN_FULL_NAME];
char InstanceIII[SIZE_OF_COURT_NAME];
char InstanceSuper[SIZE_OF_COURT_NAME];
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
char InstanceSpeal[SIZE_OF_COURT_NAME];
#endif  // APPEAL || ADMIN || AREAL || REGIONAL

void reset_user_names()
{
	*CacheUserName = *CacheUserInitials = *CacheFullUserName = '\0';
}

// 2008:014 LPR: common/unified to avoid bugs
static void cache_higher(char *s, const char *ucn)
{
	TSender sender;

	sender.ucnType = UCN_INSTANCE_HIGHER;
	strcpy(sender.ucn, ucn);

	if (sender.Try("F_NAME"))
		strcpy(s, sender.name);
	else
	{
		mstr m;
		sender.Print(m);
		strcpy(s, str(m));
	}
}

void reset_cache_data()
{
	mstr m;

	//m.cpy(Court->Seek(COURT_CODE));
	get_sencor_name(UCN_INSTANCE_SELF, COURT_CODE, m);
	strzcpy(AppNameCourt, str(m), SIZE_OF_COURT_NAME);
	strcpy(CacheCourtUCLP, UCLP->name);
	strcpy(CacheCourtArea, Area->Seek(UCLP->area));
	strcpy(CacheCourtRegion, Region->Seek(UCLP->area, UCLP->region));
	sprintf(AppNameTitle, "%s - %s", AppNameName, AppNameCourt);
	strupr(AppNameTitle + strlen(AppNameName));
#if TESTVER
	strcat(AppNameTitle, " - dev");
#endif  // TESTVER
	// N.B. don't touch HigherCourtName
	*CacheCourtAddress = *CacheCourtPhone = '\0';
	*CacheCourtEmail = *CacheCourtPresident = '\0';
	reset_user_names();

	// 2008:085
	cache_higher(InstanceSuper, INSTANCE_SUPER);
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
	cache_higher(InstanceSpeal, INSTANCE_SPEAL);
#endif  // APPEAL || ADMIN || AREAL || REGIONAL
	// 2017:088 LPR: also for areal, regional
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	cache_higher(InstanceIII, INSTANCE_III);
#endif  // MILITARY
	// 2015:251 TRQ/VRQ
#if COURT_TYPE == COURT_APPEAL
	cache_higher(InstanceIII, INSTANCE_SUPER);
#endif  // APPEAL
}

static void cat_plural(mstr &m, const char *tk)
{
	// 2017:111 testver s[] -> mstr; 2017:137 official
	mstr w(tk);
	char *s = ncstr(w);

	if ((s = strstr(s, "äåëî")) != NULL)
		s[3] = 'à';

	for (s = ncstr(w); (s = strstr(s, "î ")) != NULL; s += 2)
		s[0] = 'è';

	m.cat(str(w));
}

// ----- print ---------------------------------------------------------------
void print_types_list(mstr &m, const char *types, bool college)
{
	while (*types)
	{
		if (college)
		{
			m.cat(College->Seek(*types));
			if (!strchr(COLLEGE_COMMONS, *types))
				m.printf(" %s", COLLEGE);
		}
		else
			cat_plural(m, Type->Seek(*types));

		if (*++types)
			m.cat(", ");
	}
}

void print_compositions_list(mstr &m, const char *compositions, const char *suffix)
{
	size_t len = strlen(compositions);

	if (len && !strequal(compositions, COMPOSITION_ALLS))
	{
		while (*compositions)
		{
			m.cat(Composition->Seek(*compositions));

			if (*++compositions)
				m.cat(", ");
		}

		m.printf(" %s%s", suffix ? suffix : "ñúñòàâ", "è" + (len == 1));
	}
}

void print_kinds_list(mstr &m, const char *kinds, bool types, bool college)
{
	if (types)
	{
		char types[SIZE_OF_TYPES];	// LAW_TYPES

		*types = '\0';
		for (const char *kind = kinds; *kind; kind++)
		{
			char type = kind2type(*kind);

			if (!strchr(types, type))
				strcach(types, type);
		}
		print_types_list(m, types, college);
	}
	else
	{
		while (*kinds)
		{
			Kind->Seek(*kinds);
			if (strchr(KIND_LAWSUITS, *kinds))
				cat_plural(m, Kind->name);
			else
				m.cat(Kind->name);

			if (*++kinds)
				m.cat(", ");
		}
	}
}

void print_college(mstr &m, char college)
{
	if (PRINT_COLLEGE && college)
		m.printf("%s %s", College->Seek(college), COLLEGE);
}

void print_composition(mstr &m, long composition, const char *suffix)
{
	if (PRINT_COMPOSITIONS && composition)
	{
		m.cat(Composition->Seek(composition));

		if (!suffix)
			suffix = "ñúñòàâ";

		if (any(suffix))
			m.printf(" %s", suffix);
	}
}

void print_sender(mstr &m, char ucnType, const char *ucn)
{
	if (atob(ucn))
	{
		TSender sender;

		sender.ucnType = ucnType;
		strcpy(sender.ucn, ucn);
		if (sender.Try())
			m.cat(sender.name);
		else
			print_missing(m, ucnType, ucn);
	}
}

void print_type_or_kind(mstr &m, TRCDKey &key)
{
	TRCDKeyContainer *container = type2RCDKeyContainer(key.type);

	container->key = key;
	m.cat(container->Try("F_KIND") ? Kind->Seek(container->kind) : Type->Seek(key.type));
	delete container;
}

// 2008:022 LPR: replaces link_address_to_string()
void print_link_address(mstr &m, const TLinkWind &link, long address, long flags)
{
	TSideWind sideWind;

	if (link.GetAddressSide(sideWind))
		print_address(m, &sideWind, address, flags);
}

void print_link_addresses(mstr &m, const TLinkWind &link, const char *sep, long flags)
{
	TSideWind sideWind;

	if (link.GetAddressSide(sideWind))
		print_addresses(m, &sideWind, sep, flags);
}

void print_links_names(mstr &m, TSideWind *sideWind, bool useNewLines, bool ordinaryName)
{
	TLinkWind linkWind;
	msql w;
	bool links = false;

	linkWind.SetOrdinary(sideWind);
	linkWind.FloatCriteriaByOrdinary(w);
	TQuery q(str(w));

	while (q.Read())
	{
		linkWind << q;
		m.sep("\v");
		print_side_links(m, &linkWind, useNewLines, ordinaryName);
		links = true;
	}

	if (ordinaryName && !links)
		add_side_name(m, sideWind, useNewLines ? "\v" : ", ");
}

void print_court_account(mstr &m, char accountType, const char *fieldName)
{
	TAccount account;
	TString *field = (TString *) account.Find(fieldName);

	strcpy(account.ucn, INSTANCE_SELF);
	account.ucnType = UCN_INSTANCE_SELF;
	account.accountType = accountType;

	if (!account.Get(fieldName))
		failure();

	m.cat(field->S());
}

void print_exec_list_no(mstr &m, long execListNo, long execListLKind)
{
	if (PRINT_EXEC_LIST_NO == 1 || PRINT_EXEC_LIST_NO == 2 + (execListLKind == EXEC_LIST_PRIVATE))
		itom(execListNo, m);
}

void print_connects(mstr &m, const TRCDKey &key, bool alias, bool cyrno, constant compos)
{
	TGArray<TConnect> connects;
	load_connects(key, &connects);

	for (int i = 0; i < connects.Count(); i++)
	{
		m.sep("\v");
		connects[i]->Print(m, alias, cyrno, compos);
	}
}

void print_quick_short(mstr &m, long quick)
{
	if (quick & QUICK_CONVERTED)
	{
		char symbol = quick & QUICK_IMMEDIATE ? 'Í' : 'Á';
		m.printf(quick & QUICK_QUICK ? "Î%c" : "%cÎ", symbol);
	}
	else if (quick & QUICK_QUICK)
	{
		m.cach('Á');
		if (quick & QUICK_IMMEDIATE)
			m.cach('Í');
	}
}

void print_recipient(mstr &m, long recipient)
{
	static constant recipients[4] = { "áþäæåòà íà ñúäåáíàòà âëàñò", "äúðæàâàòà", "áþäæåòà íà ñúäåáíàòà âëàñò",
		"äúðæàâàòà / áþäæåòà íà ñúäåáíàòà âëàñò" };
	m.cat(recipients[recipient]);
}

void print_known_person_csjid(mstr &m, const char *ucn, char ucnType)
{
	TKnownPerson person;

	strcpy(person.ucn, ucn);
	person.ucnType = ucnType;
	person.involvement = INVOLVEMENT_EXECUTOR; 

	person.Try("F_CSJID");
	m.cat("Ðåã.¹ ");
	itom(person.csjid, m);
}

void print_exec_list_sendrecv(mstr &m, const char *ucn, char ucnType)
{
	bool privExec = strchr(UCN_CITIZENS, ucnType);

	if (privExec)
		m.cat("×ÑÈ ");

	print_name(m, ucn, ucnType);

	if (privExec)
	{
		m.cat(", ");
		print_known_person_csjid(m, ucn, ucnType);
	}
}

void print_sum_type_lower(mstr &m, const TExecListSum *sum)
{
	size_t len = m.len();

	SumType->Seek(sum->sumType);
	m.cat(SumType->name);
	strlwr(ncstr(m) + len);
}

void print_old_subject(mstr &m, long oldSubject)
{
	if (oldSubject)
		m.printf("\v(%ld)", oldSubject);
}

void print_college_initials(mstr &m, char college)
{
	if (PRINT_COLLEGE && college)
		m.printf("%c%c", *College->Seek(college), toupper(*COLLEGE));
}

static void print_decoration(mstr &m, char decoration, bool open)
{
	if (decoration == 'b' || decoration == 'i')
		m.printf("\a%s%c\b", "/" + open, decoration);
	else
	{
		if (open)
		{
			int font_size = decoration - '0';

			if (decoration <= '5')
				font_size += 10;

			m.printf("\aspan style='font-size:%d.0pt'\b", font_size);
		}
		else
			m.cat("\a/span\b");
	}
}

void print_with_directives(mstr &m, const char *text)
{
	TMemBlock decorations;

	for (const char *s = text; *s; s++)
	{
		if (*s != '\\')
			m.cach(*s);
		else
		{
			char c = s[1];

			switch (c)
			{
				case 'c' :
				{
					cac_assert(decorations.size);
					print_decoration(m, decorations.data[0], false);
					mbk_remove(&decorations, 0, 1);
					break;
				}
				case '\\' : m.cach('\\'); break;
				case 'v' : m.cach('\v'); break;
				case 't' : m.cat(HTML_COLUMN_INDENT); break;
				case '\0' : cac_assert(!"backslash at string end"); return;
				default :
				{
					if (strchr("bi789012", c))
					{
						print_decoration(m, c, true);
						mbk_insert(&decorations, &c, 0, 1);
					}
					else
					{
						cac_assert(!"unknown backslash directive");
						m.cach('?');
						m.cach(c);
					}
				}
			}

			s++;
		}
	}

	for (size_t n = 0; n < decorations.size; n++)
		print_decoration(m, decorations.data[n], false);
}

void print_without_directives(mstr &m, const char *text)
{
	for (const char *s = text; *s; s++)
	{
		if (*s != '\\')
			m.cach(*s);
		else
		{
			switch (s[1])
			{
				case '\\' : m.cach('\\'); break;
				case '\0' : return;
			}

			s++;
		}
	}
}

// ----- generic -------------------------------------------------------------
void generic_court_type(mstr &m, TGroup *)
{
#if COURT_TYPE == COURT_APPEAL
	m.cat("ÀÏÅËÀÒÈÂÅÍ");
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN
	m.cat("ÀÄÌÈÍÈÑÒÐÀÒÈÂÅÍ");
#endif  // ADMIN
#if COURT_TYPE == COURT_AREAL
	m.cat("ÎÊÐÚÆÅÍ");
#endif  // AREAL
#if COURT_TYPE == COURT_MILITARY
#if SPECIAL
	m.cat(CourtType == COURT_MILITARY ? "ÑÏÅÖÈÀËÈÇÈÐÀÍ ÍÀÊÀÇÀÒÅËÅÍ" : "ÑÏÅÖÈÀËÈÇÈÐÀÍ ÀÏÅËÀÒÈÂÅÍ");
#else  // SPECIAL
	m.cat(CourtType == COURT_MILITARY ? "ÂÎÅÍÅÍ" : "ÂÎÅÍÍÎ-ÀÏÅËÀÒÈÂÅÍ");
#endif  // SPECIAL
#endif  // MILITARY
#if COURT_TYPE == COURT_REGIONAL
	m.cat("ÐÀÉÎÍÅÍ");
#endif  // REGIONAL
}

void generic_judge_type(mstr &m, TGroup *)
{
	if (PRINT_JUDGE_TYPE == 1)
	{
		generic_court_type(m, NULL);
		m.cat(" ");
	}

	m.cat("ÑÚÄÈß");

	if (PRINT_JUDGE_TYPE == 2)
	{
		m.cat(" ïðè ");
		generic_court_name(m, NULL);
	}
}

void generic_judge_type_court(mstr &m, TGroup *)
{
	m.cat("ÑÚÄÈß ïðè ");
	generic_court_name(m, NULL);
}

void generic_court_name(mstr &m, TGroup *)
{
	size_t len = m.len();

	m.cat(AppNameCourt);

	if (PRINT_COURT_UPPERCASE)
		strupr(ncstr(m) + len);
}

void generic_court_uclp(mstr &m, TGroup *) { m.cat(CacheCourtUCLP); }
void generic_camel_uclp(mstr &m, TGroup *) { camel_case(m, CacheCourtUCLP, FILTER_UCLP_PREFIXES); }

static void cache_court_address()
{
	if (!any(CacheCourtAddress))
	{
		TAddress address;

		address.ucnType = UCN_INSTANCE_SELF;
		strcpy(address.ucn, INSTANCE_SELF);
		address.Get();

	#if TESTVER
		address.MakeLines(ADDRESS_SKIP_LN1);
	#else  // TESTVER
		address.MakeLines();
	#endif  // TESTVER
		strcpy(CacheCourtAddress, address.line2);
		if (*address.line3)
			scprintf(CacheCourtAddress, "  %s", address.line3);
	}
}

void generic_court_address(mstr &m, TGroup *)
{
	cache_court_address();
	m.cat(CacheCourtAddress);
}

void generic_court_phone(mstr &m, TGroup *)
{
	cache_court_address();
	m.cat(CacheCourtPhone);
}

void generic_court_email(mstr &m, TGroup *)
{
	cache_court_address();
	m.cat(CacheCourtEmail);
}

void generic_court_president(mstr &m, TGroup *)
{
	if (any(CacheCourtPresident))
		m.cat(CacheCourtPresident);
	else
	{
		TGArray<TUserAliasGroup> array;
		UserAliasGroup->LoadData(&array, C2S[POST_CHIEF], FLAG_ACTIVE);

		if (array.Count())
			print_user(m, array[0]->ucn);
	}
}

void generic_court_area(mstr &m, TGroup *) { m.cat(CacheCourtArea); }
void generic_court_region(mstr &m, TGroup *) { m.cat(CacheCourtRegion); }

void generic_user_post(mstr &m, TGroup *)
{
	size_t len = m.len();

	m.cat(Post->Seek(Default->post));
	strupr(ncstr(m) + len);
}

static void print_adjusted_size(mstr &m, const char *name, long value, int index)
{
	if (value)
	{
		if (HTML_LOGO_SCALE)
		{
			HDC screen = GetDC(NULL);

			if (screen)
			{
				int dpi = GetDeviceCaps(screen, index);

				if (dpi > 96)
					value *= 96.0 / dpi;

				ReleaseDC(NULL, screen);
			}
		}

		m.printf(" %s=%ld", name, value);
	}
}

void generic_court_logo(mstr &m, TGroup *)
{
	if (any(HTML_OVER_PATH) && any(HTML_LOGO_NAME))
	{
		char name[SIZE_OF_PATH];
		build_fn(HTML_OVER_PATH, HTML_LOGO_NAME, name);

		if (exist(name))
		{
			m.cat("\aimg");
			strrep(name, '\\', '/');
			print_adjusted_size(m, "width", HTML_LOGO_WIDTH, LOGPIXELSX);
			print_adjusted_size(m, "height", HTML_LOGO_HEIGHT, LOGPIXELSY);
			m.printf(" src=\"file:%s\" alt=\"Ëîãî íà ñúäà\"\b", name);
		}
	}
}

static void cache_user_name()
{
	if (!any(CacheUserName))
	{
		// 2008:075 LRQ: N.FAMILY
		TCitizen citizen;

		citizen.ucnType = UCN_CITIZEN_UCN;
		strcpy(citizen.ucn, Default->ucn);
		citizen.Get();
		sprintf(CacheUserName, "%c.%s", *citizen.name, citizen.family);
		sprintf(CacheUserInitials, "%c%c", *citizen.name, *citizen.family);
		sprintf(CacheFullUserName, "%s%s%s %s%s%s", citizen.name, " " + !any(citizen.reName), citizen.reName,
			citizen.family, " " + !any(citizen.reFamily), citizen.reFamily);
	}
}

void generic_user_name(mstr &m, TGroup *)
{
	cache_user_name();
	m.cat(CacheUserName);
}

void generic_user_initials(mstr &m, TGroup *)
{
	cache_user_name();
	m.cat(CacheUserInitials);
}

void generic_full_user_name(mstr &m, TGroup *)
{
	cache_user_name();
	m.cat(CacheFullUserName);
}

void generic_instance_III(mstr &m, TGroup *) { m.cpy(InstanceIII); }
void generic_today(mstr &m, TGroup *) { Today.Print(m); }

void generic_now(mstr &m, TGroup *)
{
	CTime time = Now();
	m.printf("%d:%02d÷.", time.hour, time.minute);
}

void generic_login_name(mstr &m, TGroup *) { m.cat(Default->login); }
void generic_null(mstr &m, TGroup *) { m.cat(""); }
void generic_judge(mstr &m, TGroup *) { m.cat("Ñúäèÿ"); }
void generic_slash(mstr &m, TGroup *) { m.cat("/"); }
void generic_service_phone(mstr &m, TGroup *) { m.cat(SERVICE_PHONE); }
void generic_service_fax(mstr &m, TGroup *) { m.cat(SERVICE_FAX); }
void generic_college_type(mstr &m, TGroup *) { m.cat(COLLEGE); }
void generic_god_suffix(mstr &m, TGroup *) { m.cat(GOD_SUFFIX); }

void generic_court_address_1(mstr &m, TGroup *)
{
	cache_court_address();

	if (any(PRINT_COURT_ADDRESS_1))
		m.cat(PRINT_COURT_ADDRESS_1);
	else
		generic_court_address(m, NULL);
}

void generic_court_address_2(mstr &m, TGroup *)
{
	cache_court_address();

	if (any(PRINT_COURT_ADDRESS_2))
		m.cat(PRINT_COURT_ADDRESS_2);
	else
		m.printf("òåë: %s, ôàêñ %s", SERVICE_PHONE, SERVICE_FAX);
}

void generic_court_addr_skip_uclp(mstr &m, TGroup *)
{
	TAddress address;

	address.ucnType = UCN_INSTANCE_SELF;
	strcpy(address.ucn, INSTANCE_SELF);

	if (address.Get())
		address.Print(m, ADDRESS_DISTINCT | ADDRESS_REMARK | ADDRESS_SKIP_UCLP);
}

void generic_nothing(mstr &, TGroup *)
{
}

void group_print(mstr &m, TGroup *group)
{
	group->Print(m);
}

void group_active(mstr &m, TGroup *group)
{
	TLong *flags = (TLong *) group->Find("F_FLAGS");
	m.cpy(flags->L() & FLAG_ACTIVE ? "Äà" : "Íå");
}

void group_inactive(mstr &m, TGroup *group)
{
	TLong *flags = (TLong *) group->Find("F_FLAGS");
	m.cpy(flags->L() & FLAG_ACTIVE ? "" : "í");
}

void group_bulpref(mstr &m, TGroup *group)
{
	TString *bulstat = (TString *) group->Find("F_BULSTAT");
	if (bulstat && *bulstat->S())
		m.printf(", ÅÈÊ/ÁÓËÑÒÀÒ %s", bulstat->S());
}

static void group_string(mstr &m, TGroup *group, const char *name)
{
	TBaseString *field = (TBaseString *) group->Find(name);
	m.cpy(field ? field->S() : "");
}

void group_bulstat(mstr &m, TGroup *group) { group_string(m, group, "F_BULSTAT"); }
void group_title(mstr &m, TGroup *group) { group_string(m, group,	"F_TITLE"); }
void group_extra_text(mstr &m, TGroup *group) { group_string(m, group, "F_EXTRA_TEXT"); }

#define ucnGroup OFFICE_GROUP_CAST(TUCNGroup)

void ucngroup_name(mstr &m, TGroup *group) { print_name(m, ucnGroup->ucn, ucnGroup->ucnType); }
void ucngroup_ucn_type(mstr &m, TGroup *group) { m.cpy(UCNType->Seek(ucnGroup->ucnType)); }
void ucngroup_ucn(mstr &m, TGroup *group) { m.cpy(ucnGroup->ucn); }

void ucngroup_extra(mstr &m, TGroup *group)
{
	TString *field = NULL;
	const char *descript;

	if (strchr(UCN_CITIZENS, ucnGroup->ucnType))
	{
		field = (TString *) ucnGroup->Find("F_LNC");
		descript = "ËÍ×";
	}
	else if (strchr(UCN_FIRMS, ucnGroup->ucnType))
	{
		field = (TString *) ucnGroup->Find("F_BULSTAT");
		descript = "ÅÈÊ/ÁÓËÑÒÀÒ";
	}

	if (field && *field->S())
		m.printf(" %s %s", descript, field->S());
}

#undef ucnGroup

#define baseWind OFFICE_GROUP_CAST(TBaseWind)

void basewind_key_no(mstr &m, TGroup *group) { itom(baseWind->key.no, m); }
void basewind_flag_no(mstr &m, TGroup *group) { itom(flag_key_no(&baseWind->key), m); }
void basewind_key_year(mstr &m, TGroup *group) { itom(baseWind->key.year, m); }
void basewind_key_type(mstr &m, TGroup *group) { m.cpy(Type->Seek(baseWind->key.type)); }
void basewind_kind(mstr &m, TGroup *group) { m.cpy(Kind->Seek(baseWind->kind)); }
void basewind_ucn_type(mstr &m, TGroup *group) { m.cpy(UCNType->Seek(baseWind->ucnType)); }
void basewind_date(mstr &m, TGroup *group) { baseWind->date.Print(m); }
void basewind_involvement(mstr &m, TGroup *group) { m.cpy(Involvement->Seek(baseWind->involvement)); }
void basewind_key_no_year(mstr &m, TGroup *group) { m.printf("%d/%d", baseWind->key.no, baseWind->key.year); }
void basewind_names(mstr &m, TGroup *group) { baseWind->PrintNames(m); }

#undef baseWind

#define keygroup  OFFICE_GROUP_CAST(TRCDKeyGroup)

void keygroup_key_no(mstr &m, TGroup *group) { itom(keygroup->key.no, m); }
void keygroup_flag_no(mstr &m, TGroup *group) { itom(flag_key_no(&keygroup->key), m); }
void keygroup_key_year(mstr &m, TGroup *group) { itom(keygroup->key.year, m); }
void keygroup_key_type(mstr &m, TGroup *group) { m.cpy(Type->Seek(keygroup->key.type)); }
void keygroup_key_no_year(mstr &m, TGroup *group) { m.printf("%d/%d", keygroup->key.no, keygroup->key.year); }
void keygroup_result_status(mstr &m, TGroup *group) { m.cat(Result->Seek(keygroup->Status())); }

#undef keygroup

#define container OFFICE_GROUP_CAST(TRCDKeyContainer)

void container_kind(mstr &m, TGroup *group) { m.cpy(Kind->Seek(container->kind)); }
void container_date(mstr &m, TGroup *group) { container->date.Print(m); }

// from query_lawsuit_event()
void container_time(mstr &m, TGroup *group)
{
	TTime *time = (TTime *) container->Find("F_TIME");

	if (time)
		time->Time().Print(m);
}

void container_event_no(mstr &m, TGroup *group)
{
	TLong *eventNo = (TLong *) container->Find("F_EVENT_NO");

	if (PRINT_JUDGEMENTS && eventNo)
		itom(eventNo->L(), m);
}

void container_event_date(mstr &m, TGroup *group)
{
	container_event_date1((TRCDKeyContainer *) group).Print(m, "%-D");
}

void container_event_text(mstr &m, TGroup *group)
{
	TKind tKind;	// 2010:162 LPR: electric JIT
	TString *extraText = container->FindExtraText();
	m.cpy(extraText ? extraText->S() : container->kind ? tKind.Seek(container->kind) : "íÿìà òåêñò");
}

void container_text(mstr &m, TGroup *group)
{
	group_string(m, group, "F_TEXT");
}

void container_attached(mstr &m, TGroup *group)
{
	for (int type = 0; type < TYPE_COUNT; type++)
	{
		char name[SIZE_OF_PATH];
		electric_name(container, name, type);

		if (exist(name))
		{
			m.sep(", ");
			m.cat(Electrics[type].what);
		}
	}
}

#undef container

#define sideWind OFFICE_GROUP_CAST(TSideWind)

void sidewind_involvement(mstr &m, TGroup *group)
{
	m.cpy(Involvement->Seek(sideWind->involvement));
	m.cut(22);
}

void sidewind_state(mstr &m, TGroup *group)
{
	sideWind->PrintState(m);
}

#undef sideWind

#define subpoena OFFICE_GROUP_CAST(TSubpoena)

void subpoena_side_no(mstr &m, TGroup *group) { itom(subpoena->sideNo, m); }
void subpoena_kind(mstr &m, TGroup *group) { m.cpy(SubpoenaKind->Seek(subpoena->subpoenaKind)); }
void subpoena_receiver(mstr &m, TGroup *group) { print_side_links(m, subpoena); }
void subpoena_subpoena_date(mstr &m, TGroup *group) { subpoena->subpoenaDate.Print(m); }
void subpoena_lreceiver(mstr &m, TGroup *group) { print_name(m, subpoena->lReceiverUcn, subpoena->lReceiverUcnType); }

#undef subpoena

#define origin OFFICE_GROUP_CAST(TOriginContainer)

void origin_quick_short(mstr &m, TGroup *group) { print_quick_short(m, origin->quick); }

#undef origin

static void add_uclp_name(mstr &m, TUCNGroup *group)
{
	TAddress address;

	address.ucnType = group->ucnType;
	strcpy(address.ucn, group->ucn);
	// 2008:317 LRQ/FIX: stricter catell, all tries only
	if (address.Try())
	{
		UCLP->uclp = address.uclp;
		if (UCLP->Try())
			m.printf(", %s", UCLP->name);
	}
}

#define outReg OFFICE_GROUP_CAST(TOutReg)

void outreg_receivers(mstr &m, TGroup *group)
{
	TSender sender;

	if (get_receiver(outReg, &sender))
	{
		// 2008:317 LRQ/FIX: stricter uclp_ell, tried sender only
		m.cpy(sender.name);
		add_uclp_name(m, &sender);
	}
	else
	{
		msql w;
		TGArray<TSideWind> sideWinds;

		load_sidebasicv_criteria(w, outReg);
		load_sidebasicv(sideWinds, str(w), UCN_EXEC_RECEIVERS);

		for (int i = 0; i < sideWinds.Count(); i++)
		{
			TSideWind *sideWind = sideWinds[i];

			if (sideWind->involvement == INVOLVEMENT_RECEIVER)
			{
				add_side_name(m, sideWind, "\v");
				add_uclp_name(m, sideWind);
			}
		}
	}
}

void outreg_lawsuit(mstr &m, TGroup *group)
{
	if (outReg->lawsuit.type)
	{
		print_type_or_kind(m, outReg->lawsuit);
		m.printf(" ¹ %ld/%ldã.", flag_key_no(&outReg->lawsuit), outReg->lawsuit.year);
	}
}

#undef outReg

static void address_or_none(mstr &m, TAnyAddress *address)
{
	if (address)
		address->Print(m, ADDRESS_DISTINCT);
	else
		m.cat("Íÿìà àäðåñ");
}

void legalside_address(mstr &m, TGroup *group)
{
	address_or_none(m, TYPECHECK_PTR_CAST(TLegalSide, group)->address);
}

void execlink_address(mstr &m, TGroup *group)
{
	address_or_none(m, TYPECHECK_PTR_CAST(TExecLink, group)->address);
}
