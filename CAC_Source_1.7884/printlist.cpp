#include "cac.h"

static void list_rr_status(mstr &m, TGroup *group)
{
	char type = ((TChar *) group->Find("F_TYPE"))->C();
	char status = ((TChar *) group->Find("F_STATUS"))->C();
	m.cpy(rrseek(type, status));
}

// ----- containers ----------------------------------------------------------
#define container OFFICE_GROUP_CAST(TRCDKeyContainer)

static void container_slash(mstr &m, TGroup *group)
{
	TTime *time = (TTime *) container->Find("F_TIME");

	if (time && !time->Time().Empty())
		m.cpy("/");
}

#undef container

static const offield container_fields[] =
{
	{ "F_TITLE",	group_title },
	{ "F_KIND",		container_kind },
	{ "F_DATE",		container_date },
	{ "F_SLASH",	container_slash },
	{ "F_TIME",		container_time },
	{ "F_STATUS",	list_rr_status },
	{ NULL, NULL }
};

void print_containers(TGArray<TRCDKeyContainer> *array, const char *stats, const char *tTitle)
{
	TOffice office;

	if (office.Open("PrintCont.htm"))
	{
		constant tr1start = office.Find("tr1 start");
		constant tr1final = office.Find("tr1 final");
		TTitleGroup title(tTitle);
		char status;

		office.Emit(NULL, tr1start, container_fields, &title);

		for (int i = 0; i < array->Count(); i++)
		{
			TRCDKeyContainer *container = (*array)[i];

			if (stats)
			{
				container->Add(new TCharAlias("F_STATUS", &status));
				status = stats[i];
			}
			office.Emit(tr1start, tr1final, container_fields, container);
			if (stats)
				container->Remove("F_STATUS");
		}

		office.Emit(tr1final, NULL, null_fields, NULL);
		office.Show(NULL);
	}
}

// ----- ucngroups -----------------------------------------------------------
#define sideWind OFFICE_GROUP_CAST(TBaseWind)

static void basewind_excess_key(mstr &m, TGroup *group)
{
	if (sideWind->Excess())
		m.cat("С ");

	basewind_key_no_year(m, group);
}

#undef sideWind

static const offield ucngroup_fields[] =
{
	{ "F_TITLE",	group_title },
	{ "F_TYPE",		basewind_key_type },
	{ "F_KIND",		basewind_kind },
	{ "F_NO",		basewind_key_no },
	{ "F_YEAR",		basewind_key_year },
	{ "F_KEY",		basewind_key_no_year },
	{ "F_EXCESS_KEY",	basewind_excess_key },
	{ "F_INVOLVEMENT",basewind_involvement },
	{ "F_DATE",		basewind_date },
	{ "F_STATUS",	list_rr_status },
	{ "F_NAME",		ucngroup_name },
	{ "F_UCN_TYPE",	ucngroup_ucn_type },
	{ "F_UCN",		ucngroup_ucn },
	{ "F_BULPREF",	group_bulpref },
	{ "F_BULSTAT",	group_bulstat },
	{ NULL, NULL }
};

void print_ucn_groups(TGroupArray *array, const char *stats, const char *name, const char *tTitle)
{
	TOffice office;

	if (office.Open(name))
	{
		constant tr1start = office.Find("tr1 start");
		constant tr1final = office.Find("tr1 final");
		char status;
		mstr m;

		if (!tTitle)
		{
			if (array->Count())
			{
				TUCNGroup *ucnGroup = (TUCNGroup *) (*array)[0];
				m.printf("%s %s ", UCNType->Seek(ucnGroup->ucnType), ucnGroup->ucn);
				print_name(m, ucnGroup->ucn, ucnGroup->ucnType);
			}
			tTitle = str(m);
		}
		TTitleGroup title(tTitle);

		office.Emit(NULL, tr1start, ucngroup_fields, &title);

		for (int i = 0; i < array->Count(); i++)
		{
			TGroup *group = (*array)[i];

			if (stats)
			{
				group->Add(new TCharAlias("F_STATUS", &status));
				status = stats[i];
			}
			office.Emit(tr1start, tr1final, ucngroup_fields, group);
			if (stats)
				group->Remove("F_STATUS");
		}

		office.Emit(tr1final, NULL, null_fields, NULL);
		office.Show(NULL);
	}
}

// ----- print link ----------------------------------------------------------
void print_side_links(mstr &m, TLinkWind *linkWind, bool useNewLines, bool ordinaryName, TGArray<TLinkWind> *linkWinds)
{
	bool useCommaChar = !useNewLines;
	TGArray<TLinkWind> tLinkWinds;
	int len = m.len();

	if (!linkWinds)
	{
		tLinkWinds.Add(linkWind);
		linkWinds = &tLinkWinds;
	}

	if (ordinaryName)
	{
		for (int index = 0; index < linkWinds->Count(); index++)
		{
			TLinkWind *tLinkWind = (*linkWinds)[index];

			if (index)	// avoid sep(), m may be non-empty
				m.cat(", ");
			// 2014:206 involvement
			if (linkWind->Proxied() && linkWind->proxyKind == SIDE_MARK_AS_WITH_INVOLVEMENT)
				m.printf("%s ", Involvement->Seek(tLinkWind->involvement));
			tLinkWind->PrintNames(m);
		}
	}

	if (linkWind->Proxied())
	{
		TSideWind proxy;
		TSideWind redirect;
		mstr proxyNamesM;
		const char *proxyNames;
		char proxyInvolvement[SIZE_OF_NAME];
		bool redirectSeparator = false;

		linkWind->GetProxy(proxy);
		proxy.PrintNames(proxyNamesM);
		proxyNames = str(proxyNamesM);
		strcpy(proxyInvolvement, Involvement->Seek(proxy.involvement));

		// 2014:216 support for with involvement
		if (strchr(SUMMONS_FOR_SIDE_LEGAL_SPOK_INVS, proxy.involvement) ||
			linkWind->proxyKind == SIDE_MARK_AS_WITH_INVOLVEMENT)
		{
			switch (linkWind->proxyKind)
			{
				case SIDE_MARK_INDIRECT_AS_ANY_1 :
				{
					if (useCommaChar)
						m.cat(",");

					m.printf("\nпредставляван%s от %s\n%s", linkWinds->Count() == 1 ? "(а)" : "и",
						proxyInvolvement, proxyNames);
					break;
				}
				case SIDE_MARK_INDIRECT_AS_ANY_2 :
				{
					m.printf("\nчрез %s\n%s", proxyInvolvement, proxyNames);
					break;
				}
				case SIDE_MARK_INDIRECT_AS_ANY_3 :
				{
					m.printf("\nсъс съгласието на %s\n%s", proxyInvolvement, proxyNames);
					break;
				}
				case SIDE_MARK_CONSENT_OF_INDIRECT :
				{
					m.printf("%c\nсъс съгласието на\n%s като %s", NBSP, proxyNames, proxyInvolvement);
					redirectSeparator = true;
					break;
				}
				default :
				{
					mstr prefix;

					prefix.printf("%s\nкато %s на%c\n", proxyNames, proxyInvolvement, NBSP);
					mbk_insert(&m, str(prefix), len, prefix.len());
					redirectSeparator = true;
				}
			}
		}
		else if (proxy.involvement == LINK_PARENT)	// can't happen, PARENT is LEGAL_SPOK
		{
			m.printf("\nсъс съгласието на %s%c\n%s", proxyInvolvement, NBSP, proxyNames);
			redirectSeparator = true;
		}
		else
		{
			m.printf("%c\nчрез %s\n%s", NBSP, proxyInvolvement, proxyNames);
			redirectSeparator = true;
		}

		if (linkWind->Redirected())
		{
			if (redirectSeparator)
				m.cach(NBSP);
			linkWind->GetRedirect(redirect);
			m.printf("\nчрез %s ", Involvement->Seek(redirect.involvement));
			redirect.PrintNames(m);
		}
	}

	if (useNewLines)
		m.cach('\n');
	else
		for (char *s = ncstr(m) + len; (s = strchr(s, '\n')) != NULL; s++)
			*s = ' ';

	if (tLinkWinds.Count())
		linkWinds->Detach(0);

	if (!ordinaryName)
	{
		if (m[len] == NBSP)
			ncstr(m)[len] = ' ';
		m.trim_at(len);
	}
}

// ----- print senders -------------------------------------------------------
#define sender OFFICE_GROUP_CAST(TSender)

static void sender_eispp_name(mstr &m, TGroup *group)
{
	if (sender->eispp)
		m.cat(EisDepart->Seek(sender->eispp));
}

#undef container

static const offield sender_fields[] =
{
	{ "F_TITLE",		group_title },
	{ "F_UCN",			NULL },
	{ "F_NAME",			NULL },
	{ "F_EISPP",		NULL },
	{ "F_EISPP_NAME",		sender_eispp_name },
	{ NULL, NULL }
};

void print_senders(TGroupArray *senders, char ucnType)
{
	TOffice office;

	if (office.Open("Senders.htm"))
	{
		constant tr1start = office.Find("tr1 start");
		constant tr1final = office.Find("tr1 final");
		TTitleGroup title(UCNType->Seek(ucnType));

		office.Emit(NULL, tr1start, sender_fields, &title);

		for (int i = 0; i < senders->Count(); i++)
			office.Emit(tr1start, tr1final, sender_fields, (*senders)[i]);

		office.Emit(tr1final, NULL, null_fields, NULL);
		office.Show(NULL);
	}
}

// ----- TObjectCountQuery ---------------------------------------------------
enum
{
	ELCTR_TOTAL = TYPE_EISPP + 1,
	ELCTR_COUNT
};

enum
{
	LOGGING_TOTAL = 0,
	LOGGING_ADD,
	LOGGING_ENTER,
	LOGGING_EDIT,
	LOGGING_DELETE,
	LOGGING_COUNT
};

class TObjectCountGroup : public TPeriodGroup
{
public:
	TObjectCountGroup();

	unsigned PageSize() const { return *(unsigned short *) &pageSize; }

	long byYears;
	long logging;
	long attached;
	double numPages;

	enum { INCOMING_COUNT = 3 };
	long incomingCount[INCOMING_COUNT];

	void Period(mstr &m) const;
	void TodayNow(mstr &m) const;
	void CountDates(mstr &m, const char *table, const char *field = NULL) const;
	void CountSides(mstr &m) const;
	void CountSubpoenas(mstr &m) const;
	void CountAnnounces(mstr &m) const;
	void CountIncoming(mstr &m);
	void CountOutSend2C(mstr &m) const;
	void CountJudgements(mstr &m) const;
	void CountUsers(mstr &m, const char *posts) const { CountActive(m, "T_USER", "F_POST", posts); }
	void CountKnowns(mstr &m, const char *knowns) const { CountActive(m, "T_KNOWN_PERSON", "F_INVOLVEMENT", knowns); }
	void CountLogging();
	void CountLogging(mstr &m, int what) const { m.printf("%ld", loggingCount[what]); }
	bool CountDataBase();
	void CountElectric(int type);
	void PrintCount(mstr &m, int type);
	void PrintSize(mstr &m, int type);

protected:
	short pageSize;
	long loggingCount[LOGGING_COUNT];

	struct TElectricCtr
	{
		TElectricCtr() : exist(false), count(0), size(0.0) { }

		bool exist;
		long count;
		double size;
	} elctr[ELCTR_COUNT];

	void CountCrit(mstr &m, const char *criteria) const;
	void CountActive(mstr &m, const char *table, const char *field, const char *values) const;
	void CountFilesLL(int type, const char *dir);
	void CountFile(int type, const char *name, char *base);
};

TObjectCountGroup::TObjectCountGroup()
	 : TPeriodGroup("T_OBJCOUNT")
{
	Add(
		new TRadio("F_BY_YEARS", &byYears, TRUE),
		new TRadio("F_BY_DATES", &byYears, FALSE),
		new TCheck("F_LOGGING", &logging, TRUE),
		new TCheck("F_ATTACHED", &attached, TRUE),
		new TShort("F_PAGE_SIZE", &pageSize),
		new TDouble("F_NUM_PAGES", &numPages),
		NULL
	);

	minDate = CDate(1, 1, Today.year);
	maxDate = Today;
	byYears = TRUE;
	memset(loggingCount, '\0', sizeof loggingCount);
}

void TObjectCountGroup::Period(mstr &m) const
{
	if (byYears)
	{
		m.printf("%ld", minDate.year);

		if (maxDate.year != minDate.year)
			m.printf("-%ld", maxDate.year);

		m.cat(GOD_SUFFIX);
	}
	else
		m.printf("%D-%D%s", &minDate, &maxDate, GOD_SUFFIX);
}

void TObjectCountGroup::TodayNow(mstr &m) const
{
	CDateTime::SyncServer(SYNC_PRECISE);
	m.printf("%D %lT", &(const CDate &) _CurrentDateTime, &(const CTime &) _CurrentDateTime);
}

void TObjectCountGroup::CountDates(mstr &m, const char *table, const char *field) const
{
	msql w(table, "COUNT(F_NO)");
	w.AddRange(field ? field : "F_DATE", minDate, maxDate);
	CountCrit(m, str(w));
}

void TObjectCountGroup::CountSides(mstr &m) const
{
	msql w("T_SIDE_WIND", "DISTINCT F_NO, F_YEAR, F_TYPE, F_UCN, F_UCN_TYPE, F_INVOLVEMENT");
	w.AddRange("F_DATE", minDate, maxDate);
	TQuery q(str(w));
	long count = 0;

	while (q.Read())
		count++;

	m.printf("%ld", count);
}

void TObjectCountGroup::CountSubpoenas(mstr &m) const
{
	msql w("T_SUBPOENA", "COUNT(F_NO)");
	w.AddRange("F_SUBPOENA_DATE", minDate, maxDate);
	long count = AppName->GetLong(str(w), "COUNT");

	w.Begin("T_ANNOUNCE", "COUNT(F_NO)");
	w.AddRange("F_SUBPOENA_DATE", minDate, maxDate);
	w.AddLong("F_COURT", COURT_CODE);
	count += AppName->GetLong(str(w), "COUNT");

	m.printf("%ld", count);
}

void TObjectCountGroup::CountAnnounces(mstr &m) const
{
	msql w("T_ANNOUNCE", "COUNT(F_NO)");

	w.AddRange("F_SUBPOENA_DATE", minDate, maxDate);
	w.AddLong("NOT F_COURT", COURT_CODE);
	m.printf("%ld", AppName->GetLong(str(w), "COUNT"));
}

void TObjectCountGroup::CountJudgements(mstr &m) const
{
	msql w("T_JUDGEMENT J JOIN T_DECISION D ON D.F_NO = J.F_NO AND D.F_YEAR = J.F_YEAR AND D.F_TYPE = J.F_TYPE "
		"AND D.F_KIND = J.F_KIND AND D.F_DATE = J.F_DATE", "COUNT(F_NO)");

	w.AddRange("D.F_FROM_DATE", minDate, maxDate);
	w.AddChar("NOT J.F_JUDGED", JUDGED_NOT_KNOWN);
	CountCrit(m, str(w));
}

void TObjectCountGroup::CountIncoming(mstr &m)
{
	static constant tables[INCOMING_COUNT] = { "T_REQUEST", "T_SURROUND", "T_INREG" };
	long count = 0;

	for (int index = 0; index < INCOMING_COUNT; index++)
	{
		msql w(tables[index], "COUNT(F_NO)");

		w.AddRange("F_DATE", minDate, maxDate);
		incomingCount[index] = AppName->GetLong(str(w), "COUNT");
		count += incomingCount[index];
	}

	m.printf("%ld", count);
}

void TObjectCountGroup::CountOutSend2C(mstr &m) const
{
	msql w("T_OUTREG", "COUNT(F_NO)");

	w.AddRange("F_DATE", minDate, maxDate);
	w.AddChars("F_RECEIVER_TYPE", UCN_RECEIVER_SENDERS);
	CountCrit(m, str(w));
}

void TObjectCountGroup::CountLogging()
{
	TLog tLog;
	msql w(tLog.Name, "COUNT(F_NO)");
	CDateTime minStamp(minDate.day, minDate.month, minDate.year, 0, 0, 0);
	CDateTime maxStamp(maxDate.day, maxDate.month, maxDate.year, 23, 59, 59);
	mstr gops;

	w.AddRange("F_STAMP", minStamp, maxStamp);
	loggingCount[LOGGING_TOTAL] = tLog.DataBase->GetLong(str(w), "COUNT");
	w.Begin(tLog.Name, "F_GOP");
	w.AddRange("F_STAMP", minStamp, maxStamp);
	
	gops.cach(LOG_ADD);
	gops.cach(LOG_ENTER);
	gops.cach(LOG_EDIT);
	gops.cach(LOG_DELETE);
	w.AddChars("F_GOP", str(gops));

	TQuery q(str(w));
	while (q.Read())
	{
		tLog << q;

		switch (tLog.gop)
		{
			case LOG_ADD    : loggingCount[LOGGING_ADD]++; break;
			case LOG_ENTER  : loggingCount[LOGGING_ENTER]++; break;
			case LOG_EDIT   : loggingCount[LOGGING_EDIT]++; break;
			case LOG_DELETE : loggingCount[LOGGING_DELETE]++; break;
		}
	}
}

bool TObjectCountGroup::CountDataBase()
{
	static constant GET_DB_PARAMS = "SELECT MON$PAGE_SIZE AS F_PAGE_SIZE, MON$PAGES AS F_NUM_PAGES FROM MON$DATABASE";

	if (AppName->Execute(GET_DB_PARAMS, ET_SILENT))
	{
		TQuery q(GET_DB_PARAMS);

		if (q.Read())
		{
			*this << q;
			return true;
		}
	}

	return false;
}

void TObjectCountGroup::CountElectric(int type)
{
	constant dir1 = Electrics[type].docketPath;

	if (any(dir1))
	{
		if (exist(dir1, false))
		{
			char path1[SIZE_OF_PATH];
			WIN32_FIND_DATA find1;
			HANDLE hFind1;

			build_fn(dir1, "*", path1);
			hFind1 = FindFirstFile(path1, &find1);

			if (hFind1 != INVALID_HANDLE_VALUE)
			{
				elctr[type].exist = true;
				elctr[ELCTR_TOTAL].exist = true;

				do
				{
					if (strlen(find1.cFileName) == 8)
					{
						char dir2[SIZE_OF_PATH];
						build_fn(dir1, find1.cFileName, dir2);
						CountFilesLL(type, dir2);
					}
				} while (FindNextFile(hFind1, &find1));

				FindClose(hFind1);
				elctr[ELCTR_TOTAL].count += elctr[type].count;
				elctr[ELCTR_TOTAL].size += elctr[type].size;
			}
		}
		else
			error("%s не съществува, или не е директория.", dir1);
	}
}

void TObjectCountGroup::PrintCount(mstr &m, int type)
{
	if (elctr[type].exist)
		m.printf("%ld", elctr[type].count);
}

void TObjectCountGroup::PrintSize(mstr &m, int type)
{
	if (elctr[type].exist)
		m.printf("%.2lf МБ", elctr[type].size / 1000000);
}

void TObjectCountGroup::CountActive(mstr &m, const char *table, const char *field, const char *values) const
{
	msql w(table, "COUNT(F_UCN)");

	w.Add("F_FLAGS");
	AppName->CondActive(w, "F_FLAGS");
	w.AddChars(field, values);
	CountCrit(m, str(w));
}

void TObjectCountGroup::CountCrit(mstr &m, const char *criteria) const
{
	m.printf("%ld", AppName->GetLong(criteria, "COUNT"));
}

void TObjectCountGroup::CountFilesLL(int type, const char *dir2)
{
	if (exist(dir2, false))
	{
		char path2[SIZE_OF_PATH];
		WIN32_FIND_DATA find2;
		HANDLE hFind2;

		build_fn(dir2, "*", path2);
		hFind2 = FindFirstFile(path2, &find2);

		if (hFind2 != INVALID_HANDLE_VALUE)
		{
			do
			{
				char name[SIZE_OF_PATH];
				build_fn(dir2, find2.cFileName, name);
				CountFile(type, name, find2.cFileName);
			} while (FindNextFile(hFind2, &find2));

			FindClose(hFind2);
		}
	}
}

void TObjectCountGroup::CountFile(int type, const char *name, char *base)
{
	HANDLE handle = CreateFileA(name, 0, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (handle != INVALID_HANDLE_VALUE)
	{
		BY_HANDLE_FILE_INFORMATION fileInfo;

		if (GetFileInformationByHandle(handle, &fileInfo) && !(fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			CDate date;

			if (fileTime2CDate(&fileInfo.ftLastWriteTime, date) && date >= minDate && date <= maxDate)
			{
				elctr[type].size += fileInfo.nFileSizeLow + fileInfo.nFileSizeHigh * 4294967296.0;

				if (strlen(base) <= SIZE_OF_BASE_NAME && electric_matches(base, type, true, false))
				{
					*strchr(base, '.') = '\0';

					if (strlen(base) == 8 || strlen(base) == 12)
						elctr[type].count++;
				}
			}
		}

		CloseHandle(handle);
	}
}

// ----- POST_ ---------------------------------------------------------------
static constant
	POST_OBJCOUNTS = "rcswo" "12jy",
	POST_CLERICALS = "rcswo";

#define objgroup OFFICE_GROUP_CAST(TObjectCountGroup)

static void objgroup_period(mstr &m, TGroup *group) { objgroup->Period(m); }
static void objgroup_today_now(mstr &m, TGroup *group) { objgroup->TodayNow(m); }

static void objgroup_lawsuit_count(mstr &m, TGroup *group) { objgroup->CountDates(m, "T_LAWSUIT"); }
static void objgroup_session_count(mstr &m, TGroup *group) { objgroup->CountDates(m, "T_SESSION"); }
static void objgroup_decision_count(mstr &m, TGroup *group) { objgroup->CountDates(m, "T_DECISION", "F_FROM_DATE"); }
static void objgroup_judgement_count(mstr &m, TGroup *group) { objgroup->CountJudgements(m); }
static void objgroup_incoming_count(mstr &m, TGroup *group) { objgroup->CountIncoming(m); }
static void objgroup_request_count(mstr &m, TGroup *group) { m.printf("%ld", objgroup->incomingCount[0]); }
static void objgroup_surround_count(mstr &m, TGroup *group) { m.printf("%ld", objgroup->incomingCount[1]); }
static void objgroup_inreg_count(mstr &m, TGroup *group) { m.printf("%ld", objgroup->incomingCount[2]); }
static void objgroup_outreg_count(mstr &m, TGroup *group) { objgroup->CountDates(m, "T_OUTREG"); }
static void objgroup_outreg_send2c(mstr &m, TGroup *group) { objgroup->CountOutSend2C(m); }
static void objgroup_sidewind_count(mstr &m, TGroup *group) { objgroup->CountSides(m); }
static void objgroup_subpoena_count(mstr &m, TGroup *group) { objgroup->CountSubpoenas(m); }
static void objgroup_announce_count(mstr &m, TGroup *group) { objgroup->CountAnnounces(m); }
static void objgroup_prove_count(mstr &m, TGroup *group) { objgroup->CountDates(m, "T_PROVE"); }
static void objgroup_prove_act_count(mstr &m, TGroup *group) { objgroup->CountDates(m, "T_PROVE_ACTION"); }
static void objgroup_archive_count(mstr &m, TGroup *group) { objgroup->CountDates(m, "T_ARCHIVE", "F_ARCHIVE_DATE"); }
static void objgroup_log_total_count(mstr &m, TGroup *group) { objgroup->CountLogging(m, LOGGING_TOTAL); }
static void objgroup_log_add_count(mstr &m, TGroup *group) { objgroup->CountLogging(m, LOGGING_ADD); }
static void objgroup_log_enter_count(mstr &m, TGroup *group) { objgroup->CountLogging(m, LOGGING_ENTER); }
static void objgroup_log_edit_count(mstr &m, TGroup *group) { objgroup->CountLogging(m, LOGGING_EDIT); }
static void objgroup_log_delete_count(mstr &m, TGroup *group) { objgroup->CountLogging(m, LOGGING_DELETE); }

static void objgroup_user_count(mstr &m, TGroup *group) { objgroup->CountUsers(m, POST_OBJCOUNTS); }
static void objgroup_judge_count(mstr &m, TGroup *group) { objgroup->CountUsers(m, POST_VISIBLE_JUDGES); }
static void objgroup_clerk_count(mstr &m, TGroup *group) { objgroup->CountUsers(m, POST_CLERICALS); }
static void objgroup_known_count(mstr &m, TGroup *group) { objgroup->CountKnowns(m, INVOLVEMENT_VISIBLE_PERSONS); }
static void objgroup_juror_count(mstr &m, TGroup *group) { objgroup->CountKnowns(m, INVOLVEMENT_JURORS); }
static void objgroup_expert_count(mstr &m, TGroup *group) { objgroup->CountKnowns(m, INVOLVEMENT_EXPERTS); }
static void objgroup_syndic_count(mstr &m, TGroup *group) { objgroup->CountKnowns(m, INVOLVEMENT_SYNDICS); }
static void objgroup_privex_count(mstr &m, TGroup *group) { objgroup->CountKnowns(m, C2S[INVOLVEMENT_EXECUTOR]); }
static void objgroup_legal_count(mstr &m, TGroup *group) { objgroup->CountKnowns(m, C2S[INVOLVEMENT_LEGALESE]); }

static void objgroup_text_count(mstr &m, TGroup *group) { objgroup->PrintCount(m, TYPE_TEXT); }
static void objgroup_text_size(mstr &m, TGroup *group) { objgroup->PrintSize(m, TYPE_TEXT); }
static void objgroup_image_count(mstr &m, TGroup *group) { objgroup->PrintCount(m, TYPE_IMAGE); }
static void objgroup_image_size(mstr &m, TGroup *group) { objgroup->PrintSize(m, TYPE_IMAGE); }
static void objgroup_filter_count(mstr &m, TGroup *group) { objgroup->PrintCount(m, TYPE_HTML); }
static void objgroup_filter_size(mstr &m, TGroup *group) { objgroup->PrintSize(m, TYPE_HTML); }
static void objgroup_eispp_count(mstr &m, TGroup *group) { objgroup->PrintCount(m, TYPE_EISPP); }
static void objgroup_total_count(mstr &m, TGroup *group) { objgroup->PrintCount(m, ELCTR_TOTAL); }
static void objgroup_total_size(mstr &m, TGroup *group) { objgroup->PrintSize(m, ELCTR_TOTAL); }

static void objgroup_database_size(mstr &m, TGroup *group)
{
	m.printf("%.2lf МБ", objgroup->numPages * objgroup->PageSize() / 1000000);
}

static void objgroup_page_size(mstr &m, TGroup *group)
{
	constant style = (objgroup->PageSize() == 8192) ? "\aspan\b" : "\aspan style=\"color:red\"\b";

	m.printf("%s%u\a/span\b байта", style, objgroup->PageSize());
}

#undef objgroup

static offield query_object_count_fields[] =
{
	{ "F_PERIOD",		objgroup_period },
	{ "F_TODAY_NOW",		objgroup_today_now },
	{ "F_LAWSUIT_COUNT",	objgroup_lawsuit_count },
	{ "F_SESSION_COUNT",	objgroup_session_count },
	{ "F_DECISION_COUNT",	objgroup_decision_count },
	{ "F_JUDGEMENT_COUNT",	objgroup_judgement_count },
	{ "F_INCOMING_COUNT",	objgroup_incoming_count },
	{ "F_REQUEST_COUNT",	objgroup_request_count },
	{ "F_SURROUND_COUNT",	objgroup_surround_count },
	{ "F_INREG_COUNT",	objgroup_inreg_count },
	{ "F_OUTREG_COUNT",	objgroup_outreg_count },
	{ "F_OUTREG_SEND2C",	objgroup_outreg_send2c },
	{ "F_SIDEWIND_COUNT",	objgroup_sidewind_count },
	{ "F_SUBPOENA_COUNT",	objgroup_subpoena_count },
	{ "F_ANNOUNCE_COUNT",	objgroup_announce_count },
	{ "F_PROVE_COUNT",	objgroup_prove_count },
	{ "F_PROVE_ACT_COUNT", 	objgroup_prove_act_count },
	{ "F_ARCHIVE_COUNT",	objgroup_archive_count },
	{ "F_LOG_TOTAL_COUNT",	objgroup_log_total_count },
	{ "F_LOG_ADD_COUNT",	objgroup_log_add_count },
	{ "F_LOG_ENTER_COUNT",	objgroup_log_enter_count },
	{ "F_LOG_EDIT_COUNT",	objgroup_log_edit_count },
	{ "F_LOG_DELETE_COUNT",	objgroup_log_delete_count },
	//
	{ "F_USER_COUNT",		objgroup_user_count },
	{ "F_JUDGE_COUNT",	objgroup_judge_count },
	{ "F_CLERK_COUNT",	objgroup_clerk_count },
	{ "F_KNOWN_COUNT",	objgroup_known_count },
	{ "F_JUROR_COUNT",	objgroup_juror_count },
	{ "F_EXPERT_COUNT",	objgroup_expert_count },
	{ "F_SYNDIC_COUNT",	objgroup_syndic_count },
	{ "F_PRIVEX_COUNT",	objgroup_privex_count },
	{ "F_LEGAL_COUNT",	objgroup_legal_count },	
	//
	{ "F_TEXT_COUNT",		objgroup_text_count },
	{ "F_TEXT_SIZE",		objgroup_text_size },
	{ "F_IMAGE_COUNT",	objgroup_image_count },
	{ "F_IMAGE_SIZE",		objgroup_image_size },
	{ "F_FILTER_COUNT",	objgroup_filter_count },
	{ "F_FILTER_SIZE",	objgroup_filter_size },
	{ "F_EISPP_COUNT",	objgroup_eispp_count },
	{ "F_TOTAL_COUNT",	objgroup_total_count },
	{ "F_TOTAL_SIZE",		objgroup_total_size },
	//
	{ "F_DATABASE_SIZE",	objgroup_database_size },
	{ "F_PAGE_SIZE",		objgroup_page_size },
	{ NULL, NULL }
};

#include "GetObjectCountDialog.h"
#include "GetObjectCountDialog.cpp"

void query_object_count(TWindow *parent)
{
	TObjectCountGroup group;

	while (TGetObjectCountDialog(parent, &group).Execute() == IDOK)
	{
		TWaitWindow wait(parent, WAIT);
		TOffice office;

		if (office.Open("ObjCount.htm"))
		{
			constant lg1start = office.Find("lg1 break");
			constant lg1final = office.Find("lg1 final");
			constant tb1break = office.Find("tb1 break");
			constant tb2break = office.Find("tb2 break");
			constant tb3break = office.Find("tb3 break");

			office.Emit(NULL, lg1start, query_object_count_fields, &group);

			if (group.logging)
			{
				group.CountLogging();
				office.Emit(lg1start, lg1final, query_object_count_fields, &group);
			}

			office.Emit(lg1final, tb1break, query_object_count_fields, &group);

			if (group.attached)
			{
				for (int type = 0; type < ELCTR_TOTAL; type++)
					group.CountElectric(type);

				office.Emit(tb1break, tb2break, query_object_count_fields, &group);
			}

			if (group.CountDataBase())
				office.Emit(tb2break, tb3break, query_object_count_fields, &group);

			office.Emit(tb3break, NULL, query_object_count_fields, &group);
			office.Show(&wait);
		}
	}
}
