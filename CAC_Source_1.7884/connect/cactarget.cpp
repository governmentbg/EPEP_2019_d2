#include "cac.h"

int TCACTarget::GetVersion(TCACDataBase *dataBase) const
{
	static constant expr = "SELECT F_NAME FROM T_SENDER "
		"WHERE F_UCN_TYPE = 'B' AND F_UCN IN ('0000000001', '0000000002') "
		"ORDER BY F_UCN";
	const UINT len = strlen(SenderName);
	TQuery q(expr, dataBase);
	char courtype_version[NAME_SIZE];
	int base_version;

	if (!q.Read() || !q.GetField(q.Find("F_NAME"), (pBYTE) courtype_version) || !*courtype_version)
		return error("ВНИМАНИЕ! Версията на %s сървър липсва!", auserName);

	if (memcmp(courtype_version, SenderName, len) || memcmp(courtype_version + len, " 1.", 3) ||
		(base_version = atoi(courtype_version + len + 3)) <= 0)
	{
		return error("ВНИМАНИЕ! Версията на %s сървър е невалидна!", auserName);
	}

	char common_version[NAME_SIZE];
	int test_version;

	if (!q.Read() || !q.GetField(q.Find("F_NAME"), (pBYTE) common_version) || !*common_version)
		return error("ВНИМАНИЕ! Версията на сървъра на %s липсва!", AppNameName);

	if (memcmp(common_version, "COMMON", 6) || memcmp(common_version + 6, " 1.", 3) ||
		(test_version = atoi(common_version + 9)) <= 0)
	{
		return error("ВНИМАНИЕ! Версията на сървъра на %s липсва или е невалидна!", AppNameName);
	}

	if (base_version != test_version)
		return error("ВНИМАНИЕ! Версиите на сървъра на %s не съвпадат!", AppNameName);

	return base_version;
}
