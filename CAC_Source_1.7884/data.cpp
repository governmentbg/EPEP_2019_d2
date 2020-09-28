#include "cac.h"

// 2009:280 LPR: fixed alias names
#if COURT_TYPE == COURT_APPEAL
#define BASEALIAS "SAPPEAL"
#endif // APPEAL
#if COURT_TYPE == COURT_ADMIN
#define BASEALIAS "SADMIN"
#endif  // ADMIN
#if COURT_TYPE == COURT_AREAL
#define BASEALIAS "SAREAL"
#endif  // AREAL
#if COURT_TYPE == COURT_MILITARY
#if SPECIAL
#define BASEALIAS "SPECIAL"
#else  // SPECIAL
#define BASEALIAS "MILITARY"
#endif  // SPECIAL
#endif  // MILITARY
#if COURT_TYPE == COURT_REGIONAL
#define BASEALIAS "REGIONAL"
#endif  // REGIONAL
#define FALLBACK "REGCOURT"

// ----- Startup / Shutdown --------------------------------------------------
void initialize_base()
{
	char value[DBIMAXSCFLDLEN];
	TCACConnect *connect;

	TODBCDataBase::Initialize(false);

	for (int i = 0; ; i++)	// break on BDE FALLBACK
	{
		{	// ODBC DSN
			static constant odbc_sources[] = { "CAC_" BASEALIAS BASEVER, "CAC_" BASEALIAS, "CAC_" FALLBACK BASEVER,
				"CAC_" FALLBACK, NULL };
			constant source = odbc_sources[i];

		#if DBT_COUNT >= 3
			connect = new TPostGresConnect(source);

			if (connect->Load(value))
				break;

			delete connect;
		#endif  // DBT_COUNT >= 3
			connect = new TFireBirdConnect(source);

			if (connect->Load(value))
				break;

			delete connect;	// before trying a BDE alias

			if (TODBCConnect::Exist(source, value))
			{
				fatal("%s: намереният 32-битов ODBC System DSN е за база данни или с версия на драйвера която "
					"не се поддържа.", source);
			}
		}

		{	// BDE ALIAS
			static constant bde_aliases[] = { BASEALIAS BASEVER, BASEALIAS, FALLBACK BASEVER, FALLBACK, NULL };
			constant alias = bde_aliases[i];

			if (i == 0)
				TBDEDataBase::Initialize(tempdir, TEMPDIR, false);

			connect = new TBDEConnect(alias);

			if (connect->Load(value) || !strcmp(alias, FALLBACK))
				break;

			delete connect;
		}
	}

	const char *syntax = connect->SyntaxError(value);
	TVirtPtr VP(connect);

	if (syntax)
		fatal("%s: %s.", connect->Name, syntax);

	TCACDataBase::DefaultDB = connect->NewDataBase();
	TCACDataBase::DefaultDB->DefaultConnect(ET_FATAL);

	TAliasGroup::InitializeCache();
	TAliasGroup::InitializeBase();
}

void shutdown_base()
{
	TAliasGroup::ShutdownBase();
	TAliasGroup::ShutdownCache();
}

void initialize_noms()
{
	TAliasGroup::InitializeNoms();

	Default = new TDefault;
	Calendar = new TCalendar;
	StreetMap = new TStreetMap;
	RequestIndex = new TRequestIndex;		// all incoming, not requests only
	AnnounceIndex = new TAnnounceIndex;
	DecisionIndex = new TEventIndex;
	EisVal = new TEisVal;
	SbeDesc = new TSbeDesc;
	SessionPrice = new TSessionPrice;
	EisCrime = new TEisCrime;
	EisPlace = new TEisPlace;
	ReportRange = new TReportRange;
}

void shutdown_noms()
{
	delete ReportRange;
	ReportRange = NULL;
	delete EisPlace;
	EisPlace = NULL;
	delete EisCrime;
	EisCrime = NULL;
	delete SessionPrice;
	SessionPrice = NULL;
	delete SbeDesc;
	SbeDesc = NULL;
	delete EisVal;
	EisVal = NULL;
	delete DecisionIndex;
	DecisionIndex = NULL;
	delete AnnounceIndex;
	AnnounceIndex = NULL;
	delete RequestIndex;
	RequestIndex = NULL;
	delete StreetMap;
	StreetMap = NULL;
	delete Calendar;
	Calendar = NULL;
	delete Default;
	Default = NULL;

	TAliasGroup::ShutdownNoms();
}

void initialize_extra()
{
	// 2015:296 moved before SHOWUP-s
	if (can_func(RIGHT_ATTACH) && !ExtraDataBase)
	{
		if (any(EXTERNAL_SERVER))
		{
			TWaitWindow wait(NULL, "Връзка към базата данни за регистриране на съдебни актове...");
			mstr m;

			m.printf("Servername=%s;Database=CAC_EXTCOURT", EXTERNAL_SERVER);
			ExtraDataBase = new TPostGresExtraDataBase(str(m));

			if (!ExtraDataBase->DefaultConnect(ET_ERROR))
			{
				shutdown_extra();
				error("Няма достъп до базата данни за регистриране на съдебни актове.");
			}
		}
		else
			error("Не е указан сървър за регистриране на съдебни актове.");
	}
}

void shutdown_extra()
{
	delete ExtraDataBase;
	ExtraDataBase = NULL;
}
