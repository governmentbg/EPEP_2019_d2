#include "cac.h"

static void compressKind(TRCDKeyContainer *container)
{
	container->kind = (char) (KIND_SURROUENDOC_BASE + (strchr(KIND_SESSIONS, container->kind) - KIND_SESSIONS));
}

static bool compressedKind(const TRCDKeyContainer *container)
{
	return container->kind >= KIND_SURROUENDOC_BASE && container->kind < KIND_SURROUENDOC_BASE + strlen(KIND_SESSIONS) &&
		strchr(RESULT_SURROUENDOCS, container->Status());
}

static bool decompressKind(TRCDKeyContainer *container)
{
	if (compressedKind(container))
	{
		container->kind = KIND_SESSIONS[container->kind - KIND_SURROUENDOC_BASE];
		return true;
	}

	return false;
}

class DtorCompress
{
public:
	DtorCompress(TRCDKeyContainer *tContainer) : container(tContainer) { }
	~DtorCompress() { compressKind(container); }

private:
	TRCDKeyContainer *const container;
};

// ----- TElectricCollect ----------------------------------------------------
TElectricCollect::TElectricCollect(TRCDKeyContainer *container)
	: request(NULL), lawsuit(NULL), inReg(NULL), outReg(NULL), prove(NULL)
{
	switch (container->key.type)
	{
		case TYPE_REQUEST :
		{
			request = (TRequest *) container;
			strcpy(kinds, KIND_INDOCS);
			strcat(kinds, KIND_PROVES);
			strcat(kinds, KIND_IN_REG_DOCS);
			strcat(kinds, KIND_OUT_REG_DOCS);
			strcat(kinds, KIND_REQUEST_RESOLUTIONS);
			strcat(kinds, KIND_SURROUNDS);
			break;
		}
		case TYPE_INREG :
		{
			inReg = (TInReg *) container;
			strcpy(kinds, KIND_IN_REG_DOCS);
			strcat(kinds, KIND_INREG_EVENTS);
			strcat(kinds, KIND_SURROUNDS);
			break;
		}
		case TYPE_OUTREG :
		{
			outReg = (TOutReg *) container;
			strcpy(kinds, KIND_OUT_REG_DOCS);
			break;
		}
		case TYPE_PROVE :
		{
			prove = (TProve *) container;
			strcpy(kinds, KIND_PROVE_ACTIONS);
			break;
		}
		default :
		{
			if (strchr(TYPE_LAWSUITS, container->key.type))
			{
				lawsuit = (TLawsuit *) container;
				strcpy(kinds, KIND_INDOCS);
				strcat(kinds, KIND_PROVES);
				strcat(kinds, KIND_SESSIONS);
				strcat(kinds, KIND_ENDOCS);
				strcat(kinds, KIND_SURROUNDS);
				strcat(kinds, KIND_IN_REG_DOCS);
				strcat(kinds, KIND_OUT_REG_DOCS);
			}
			else
				fatal("%d: document type not supported by electric collect", container->key.type);
		}
	}
}

void TElectricCollect::AddProc(TRCDKeyContainer *container)
{
	// 2010:166 LPR: back-resolution for main extra text
	if (request && strchr(KIND_INDOCS, container->kind))
		container->AddExtraText("Постъпване в съда");
	else if (lawsuit && strchr(KIND_LAWSUITS, container->kind))
		container->AddExtraText("Образуване в дело");
	else if (inReg && strchr(KIND_IN_REG_DOCS, container->kind))
		container->AddExtraText("Постъпване в съда");
	else if (prove && strchr(KIND_PROVES, container->kind))
		container->AddExtraText("Постъпване в съда");
	else if (strchr(KIND_OPEN_SESSIONS, container->kind))
	{
		// 2018-03-26: end date support
		TSession *session = (TSession *) container;

		if (strchr(RESULT_UNLINKEDS, session->result))
			session->AddExtraText("Отменено открито съдебно заседание");
		else if (session->end > session->date)
		{
			mstr m(Kind->Seek(session->kind));

			m.printf(", до %D", &session->end);
			session->AddExtraText(str(m));
		}
		//else	// 2016:084 LPR/IRQ: exact kind
		//	container->AddExtraText("Открито съдебно заседание");
	}
	// 2013:071; 2016:084 LPR/IRQ: exact status
	//else if (strchr(KIND_INREG_EVENTS, container->kind) && strchr(RESULT_IOREG_RETURNEDS, container->Status()))
	//	container->AddExtraText("Връщане");
	else if (strchr(KIND_INREG_EVENTS, container->kind))
	{
		TInRegEvent *inRegEvent = (TInRegEvent *) container;
		mstr m(Result->Seek(inRegEvent->result));

		if (inRegEvent->eventNo && PRINT_JUDGEMENTS)
			m.printf(" № %ld", inRegEvent->eventNo);

		container->AddExtraText(str(m));
	}
	// 2015:174
	else if (container->kind == KIND_OUT_LETTER || container->kind == KIND_IN_LETTER)
	{
		mstr m;

		m.printf("Писмо %s. № %ld/%ld", container->kind == KIND_OUT_LETTER ? "изх" : "вх", container->key.no,
			container->key.year);
		container->AddExtraText(str(m));
	}
	// 2016:111 LPR: copied from inreg event above
	else if (strchr(KIND_REQUEST_RESOLUTIONS, container->kind))
	{
		TRequestResolution *resolution = (TRequestResolution *) container;
		mstr m(Resolution->Seek(resolution->resolution));

		if (resolution->eventNo && PRINT_JUDGEMENTS)
			m.printf(" № %ld", resolution->eventNo);

		container->AddExtraText(str(m));
	}
	else if (container->key.type == TYPE_INREG || container->key.type == TYPE_OUTREG ||
		container->key.type == TYPE_SURROUND)
	{
		TBaseString *text = (TString *) container->Find("F_TEXT");
		mstr m(strchr(KIND_ANY_OTHER_DOCUMENTS, container->kind) && text && *text->S() ? text->S() :
			Kind->Seek(container->kind));

		m.printf(" № %ld/%ld", container->key.no, container->key.year);
		container->AddExtraText(str(m));
	}

	AddItem(container);

	// post-processing
	if (container->key.type == TYPE_OUTREG && !((TOutReg *) container)->returned.Empty())
	{
		TOutReg *outReg = new TOutReg;

		*outReg << container;
		outReg->AddExtraText("Връщане");
		outReg->aret = true;
		AddItem(outReg);
	}
	else if (strchr(KIND_ENDOCS, container->kind))
	{
		TDecision *decision = (TDecision *) container;

		if (decision->Motivated())
			AddItem(new TDecisionRider(decision, KIND_MOTIVES));

		if (any(decision->opinion))
			AddItem(new TDecisionRider(decision, KIND_OPINION));
	}
	// 2013:071
	else if (strchr(KIND_INREG_EVENTS, container->kind))
	{
		TInRegEvent *inRegEvent = (TInRegEvent *) container;

		if (inRegEvent->outNo)
		{
			TOutReg *outReg = new TOutReg;

			outReg->key.no = inRegEvent->outNo;
			outReg->key.year = inRegEvent->outYear;

			if (outReg->Get())
				AddItem(outReg);
			else
				delete outReg;
		}
	}
	// 2016:124
	else if (strchr(KIND_SESSIONS, container->kind) && strchr(RESULT_SURROUENDOCS, container->Status()))
	{
		TSession *session = new TSession;

		*session << container;
		compressKind(session);
		session->AddExtraText("Актове по възражения");
		AddItem(session);
	}
}

void TElectricCollect::AddCopy(const TRCDKeyContainer *container)
{
	TRCDKeyContainer *tContainer = kind2RCDKeyContainer(container->kind);
	*tContainer << container;
	AddProc(tContainer);
}

void TElectricCollect::Collect()
{
	TGSArray<TRCDKeyContainer> array;

	// Add basic
	if (request)
	{
		AddCopy(request);

		TProve *prove = new TProve;

		if (prove->TryBy(request))
			array.Add(prove);
		else
			delete prove;

		query_events(request, &array);
	}
	else if (lawsuit)
	{
		TRequest *tRequest = new TRequest;

		tRequest->key = lawsuit->source;
		tRequest->kind = lawsuit->sourceKind;

		// N.B. all request fields are required
		if (lawsuit->source.type == TYPE_REQUEST)
			tRequest->Get();
		else
			tRequest->date = lawsuit->date;

		AddItem(tRequest);
		AddCopy(lawsuit);

		// 2006:187 Prove support
		TProve *prove = new TProve;

		if (prove->TryBy(lawsuit))
			array.Add(prove);
		else
			delete prove;

		query_events(lawsuit, &array);
	}
	else if (inReg)
	{
		AddCopy(inReg);
		query_events(inReg, &array);
	}
	else if (outReg)
		AddCopy(outReg);
	else if (prove)
	{
		AddCopy(prove);
		query_events(prove, &array);
	}

	// Add movement
	for (int type = 0; type < array.Count(); type++)
		AddProc(array[type]);

	array.Clear();
}

// ----- TElectric -----------------------------------------------------------
#define electric OFFICE_GROUP_CAST(TElectric)

static void electric_extra_user_print(mstr &m, TGroup *group)
{
	if (electric->ExtraUser())
		m.cat("д");
}

static void electric_extra_type_print(mstr &m, TGroup *group)
{
	size_t len = m.len();
	m.cat(Electrics[electric->extraType].what);
	ncstr(m)[len] = toupper(str(m)[len]);
}

static void electric_extra_ext_print(mstr &m, TGroup *group)
{
	size_t len = m.len();
	m.cat(file_ext(electric->extraName));
	strupr(ncstr(m) + len);
}

#undef electric

static void electric_items_collect2(TRCDKeyContainer *container, TGSArray<TElectric> &items)
{
	TGArray<TSubpoena> subpoenas;
	bool loaded = false;

	for (int type = 0; type < TYPE_COUNT; type++)
	{
		char tName[SIZE_OF_PATH];
		WIN32_FIND_DATA find;
		constant snum = find.cFileName + 8;

		electric_name(container, Electrics[type].docketPath, tName, -1, 0);
		HANDLE hFind = FindFirstFile(tName, &find);
		*basename(tName) = '\0';

		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (!electric_matches(find.cFileName, type, true, false))
					continue;

				char *end;
				long extraNo = strtol(snum, &end, 0x10);

				if (*(end > snum ? end : snum) != '.')
					continue;

				if (TElectric::ExtraSurrouend(extraNo))
					continue;

				TElectric *electric = new TElectric;

				*electric << container;
				electric->extraNo = extraNo;
				electric->extraType = type;
				build_fn(tName, find.cFileName, electric->extraName);
				items.Add(electric);

				if (electric->ExtraSubpoena())
				{
					if (!loaded)
					{
						msql m;
						TBaseWind baseWind("T_SUBPOENA", NULL);

						baseWind << container;
						baseWind.FloatCriteria(m, "F_SUBPOENA_NO, F_SUBPOENA_KIND, F_UCN, F_UCN_TYPE, "
							"F_INVOLVEMENT, F_EVENT_STATUS");
						loaded = true;

						TQuery q(str(m));
						while (q.Read())
						{
							TSubpoena *subpoena = new TSubpoena;
							*subpoena << q;
							subpoenas.Add(subpoena);
						}
					}


					for (int i = 0; i < subpoenas.Count(); i++)
					{
						TSubpoena *subpoena = subpoenas[i];

						if (subpoena->subpoenaNo == extraNo)
						{
							mstr m;

							m.printf("%s %ld - ", SubpoenaKind->Seek(subpoena->subpoenaKind),
								subpoena->subpoenaNo);
							subpoena->PrintNames(m);
							strzcpy(electric->descript, str(m), sizeof electric->descript);
							*electric << subpoena;	// 2016:319 for e-portal delete
							break;
						}
					}

					if (!any(electric->descript))
						sprintf(electric->descript, "Призовка/съобщение #%ld", extraNo);
				}
				else if (electric->extraNo)
				{
					if (!electric->Try("F_DESCRIPT"))
						sprintf(electric->descript, "Допълнителен документ #%ld", extraNo - SUBPOENA_NO_MAX);
				}
			} while (FindNextFile(hFind, &find));
		}

		finish_find(hFind, tName, false);
	}
}

static void electric_surrouendoc_collect(const TSurroundment &surrouendoc, TGSArray<TElectric> &items)
{
	// 2016:172 LPR/JRQ: support for portreg
	int collectTypes[] = { TYPE_TEXT, TYPE_IMAGE, TYPE_HTML, TYPE_PORTREG, TYPE_COUNT };
	int type;

	if (!can_portable())
		collectTypes[TYPE_COUNT] = TYPE_COUNT;

	for (int i = 0; (type = collectTypes[i]) != TYPE_COUNT; i++)
	{
		char name[SIZE_OF_PATH];
		electric_name(&surrouendoc, name, type);

		if (exist(name))
		{
			TElectric *electric = new TElectric;
			mstr m;

			*electric << surrouendoc;
			electric->extraType = type;
			strcpy(electric->extraName, name);
			m.printf("%s ", Kind->Seek(surrouendoc.decisionKind));

			if (surrouendoc.eventNo && PRINT_JUDGEMENTS)
				m.printf("№ %ld/", surrouendoc.eventNo);
			else
				m.cat("от ");

			m.printf("%Dг. по %s %ld/%ldг.", &surrouendoc.decisionDate, Kind->Seek(surrouendoc.kind),
				surrouendoc.key.no, surrouendoc.key.year);
			strzcpy(electric->descript, str(m), sizeof electric->descript);
			items.Add(electric);
		}
	}
}

static void surrouendoc_collect(TRCDKeyContainer *container, TGSArray<TElectric> &items)
{
	TSurroundment surrouendoc;
	msql m(surrouendoc.Name);

	surrouendoc.SessionCriteria(m, TYPECHECK_PTR_CAST(TSession, container));
	m.AddChars("F_KIND", KIND_SURROUENDOCS);
	m.AddIsNull("F_EXTRA_NO", false);

	TQuery q(str(m));
	while (q.Read())
	{
		surrouendoc << q;
		electric_surrouendoc_collect(surrouendoc, items);
	}
}

void electric_items_collect(TRCDKeyContainer *container, TGSArray<TElectric> &items)
{
	if (decompressKind(container))
	{
		DtorCompress cps(container);
		surrouendoc_collect(container, items);
	}
	else
	{
		electric_items_collect2(container, items);
		// 2016:173 PRQ: comout
		//if (strchr(KIND_SESSIONS, container->kind) && strchr(RESULT_SURROUENDOCS, container->Status()))
		//	surrouendoc_collect(container, items);
	}
}

#include "AttachExtraDialog.h"
#include "AttachExtraDialog.cpp"

static bool attach_extra(TWindow *parent, TElectric *electric)
{
	return TAttachExtraDialog(parent, electric).Execute() == IDOK;
}

#include "ElectricItemListWindow.h"
#include "ElectricItemListWindow.cpp"

#include "ElectricItemListDialog.h"
#include "ElectricItemListDialog.cpp"

void electric_items_interact(TWindow *parent, TRCDKeyContainer *container, bool attach, const TLawsuit *lawsuit)
{
	TGSArray<TElectric> items;

//	FILE *f = fopen("c:\\tmp\\active.txt", "w");
//	fprintf(f, "before: active = 0x%lx\n", GetActiveWindow());

	{
		TWaitWindow wait(parent, WAIT);
		electric_items_collect(container, items);
	}

//	fprintf(f, "after: active = 0x%lx\n", GetActiveWindow());
//	fclose(f);

	if (items.Count() || attach)
		TElectricItemListDialog(parent, container, &items, attach, lawsuit).Execute();
	else
		error("Няма присъединени документи.");
}

// ----- electra -----------------------------------------------------------
#include "electra2.h"
#include "electra2.cpp"

#include "ElectricPushButton.h"
#include "ElectricPushButton.cpp"

static const TElectra electra_surrouendocs[] =
{
	{ "Вписване/редактиране на съдебни актове", CM_CLERK_NEW_JUDICIAL_ACT,
		TYPE_LAWSUITS, NULL,
		NULL, NULL,
		MENU_CLERK MENU_SECR MENU_JUDGE },
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL
	{ "Обжалване на актове по възражения", CM_REG_IN_SURROUENDOCS,
		TYPE_LAWSUITS, NULL,
		NULL, NULL,
		MENU_REG },
#endif  // APPEAL || AREAL
	{ NULL, 0, NULL, NULL, NULL, NULL, NULL }
};

class TElectricListDialog;

#include "ElectricListWindow.h"
#include "ElectricListDialog.h"

#include "ElectricListWindow.cpp"
#include "ElectricListDialog.cpp"

void electric_blue(TWindow *parent, TRCDKeyContainer *container, TPoint *where)
{
	if (LOG_QUERIES)
		log(LOG_QUERY, 0, &container->key, "електронна папка");

	(new TElectricListDialog(parent, container, where))->Create();
}

void electric_signal(int sigtype, const TRCDKeyContainer *container, const void *data)
{
	TElectricListDialog::Signal(sigtype, container, data);
}

int electric_foreach(int command, void *argument)
{
	return TElectricListDialog::ForEach(command, argument);
}
