#include "cac.h"

class TConnectApp : public TApplication
{
public:
	TConnectApp(const TCACTarget *tTarget, int tVersion) : target(tTarget), version(tVersion) { }

	virtual int Run() { return TConnectDialog(NULL, target, version).Execute(); }

private:
	const TCACTarget *const target;
	const int version;
};

static bool ver_query_value(DWORD *ver_buff, const char *name, void **value)
{
	mstr m("\\StringFileInfo\\040904E4\\");
	UINT len;

	m.cat(name);
	return VerQueryValue(ver_buff, ncstr(m), value, &len) && len > 0;
}

constant AppNameName = "САС \"СЪДЕБНО ДЕЛОВОДСТВО\" - БАЗА ДАННИ";
static const UINT CP_REQUIRED = 1251;

static const TCACTarget CACTargets[] =
{
	{ "APPEAL",   "Appealative",    "SAPPEAL",  "Апелативен",      "апелативния" },
	{ "ADMIN",    "Administrative", "SADMIN",   "Административен", "административния" },
	{ "AREA",     "Areal/District", "SAREAL",   "Окръжен",         "окръжния" },
	{ "SPECIAL",  "Special",        "SPECIAL",  "Специализиран",   "специализирания" },
	{ "MILITARY", "Military",       "MILITARY", "Военен",          "военния" },
	{ "REGION",   "Regional",       "REGIONAL", "Районен",         "районния" },
	{ NULL, NULL, NULL, NULL }
};

int OwlMain(int argc, char *argv[])
{
	if (GetACP() != CP_REQUIRED)
		fatal("Code page %u required.\nИзисква се кодова страница %u.", CP_REQUIRED, CP_REQUIRED);

	if (argc < 1)
		fatal("Невалиден брой параметри.");

	if (argc > 1)
		error("Невалиден брой параметри.");

	mstr m(argv[0]);
	const char *base = basename(str(m));

	if (!any(base))
		fatal("%s: невалидно име на изпълнимия файл.", str(m));

	m.cut(base);
	m.cat("AppName.exe");

	const char *AppNameExe = str(m);

	if (!exist(AppNameExe))
		fatal("%s: грешка %d при достъп до изпълнимия файл на САС.", AppNameExe, errno);

	DWORD ver_buff[0x200];
	DWORD size, zero;
	char *file = "", *product = "", *type = "";

	if ((size = GetFileVersionInfoSize((char *) AppNameExe, &zero)) <= 0 || size > sizeof ver_buff ||
		!GetFileVersionInfo((char *) AppNameExe, zero, sizeof ver_buff, ver_buff) ||
		!ver_query_value(ver_buff, "FileVersion", (void **) &file) ||
		!ver_query_value(ver_buff, "ProductVersion", (void **) &product) ||
		!ver_query_value(ver_buff, "SpecialBuild", (void **) &type))
	{
		fatal("%s: информацията за версията на САС липсва или е недостъпна.", AppNameExe);
	}

	int version;

	if (strncmp(product, "1.", 2) || strncmp(file, "1.", 2) || (version = atoi(product + 2)) <= 0)
		fatal("%s: информацията за версията САС е невалидна.", AppNameExe);

	if (version < 6700)
		fatal("%s: изисква се САС версия 1.6700 или по-висока.", AppNameExe);

	if (version > 9999)
		fatal("%s: изисква се САС версия 1.9999 или по-ниска.", AppNameExe);

	const TCACTarget *target;

	for (target = CACTargets; target->SenderName; target++)
		if (!strcmp(target->ExecSBuild, type))
			break;

	if (!target->SenderName)
		fatal("%s: информацията за вида на съда е невалидна.", AppNameExe);

	obtain_tempdir();

	if (!strcmp(tempdir, TEMPDIR))
		message(MB_WARN, "ВНИМАНИЕ! Използва се локалната директория за временни файлове.");

	TBDEDataBase::Initialize(tempdir, TEMPDIR, false);
	TODBCDataBase::Initialize(false);
	TConnectApp(target, version).Run();
	TBDEDataBase::Shutdown();
	TODBCDataBase::Shutdown();
	return 0;
}
