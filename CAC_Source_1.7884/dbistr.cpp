#include "cac.h"

constant
	DBISTR_RECLOCKED			= "%s: данните са заключени и недостъпни за %s (%d).",
	DBISTR_DETAILRECORDSEXIST	= "%s: данните не подлежат на %s поради наличието на подчинени данни (%d).",
	DBISTR_NOTSUFFTABLERIGHTS	= "%s: операцията %s е отказана поради липсата на съответните права за работа с таблицата (%d).",
	DBISTR_NOTSUFFSQLRIGHTS		= "%s: операцията %s е отказана поради липсата на съответните права за работа с данните (%d).",
	DBISTR_RECLOCKFAILED		= "%s: данните не могат да бъдат заключени за %s поради наличието на ограничения (%d).",
	DBISTR_KEYVIOL			= "%s: операцията %s би довела до дублиране на ключ (%d).",
	DBISTR_FILELOCKED			= "%s: файла е заключен и недостъпен за %s (%d).",
	DBISTR_MINVALERR			= "%s: поле от данните е под възможната минимална стойност и недостъпно за %s (%d).",
	DBISTR_MAXVALERR			= "%s: поле от данните е над възможната максимална стойност и недостъпно за %s (%d).",
	DBISTR_REQDERR			= "%s: поле от данните е празно (или извън възможните стойности) и недостъпно за %s (%d).",
	DBISTR_NULLERROR			= "%s: поле от данните е празно и недостъпно за %s (%d).",
	DBISTR_RANGEERROR			= "%s: поле от данните е празно и недостъпно за %s (%d).",
	DBISTR_TABLEREADONLY		= "%s: таблицата е само за четене и недостъпна за %s (%d).",
	DBISTR_NODISKSPACE		= "%s: операцията %s не може да се изпълни поради липсата на достатъчно дисково пространство (%d).",
	DBISTR_ALREADYLOCKED		= "%s: записа е заключен и недостъпен за %s (%d).",
	DBISTR_UNKNOWNSQL			= "%s: операцията %s е отказана, най-вероятно поради нарушаване на външен ключ (%d).",
	DBISTR_INTEGRITY			= "%s: операцията %s е токазана, защото би нарушира целостта на данните. (%d)",
	DBISTR_FOREIGNKEY			= "%s: операцията %s е отказана поради нарушаване на външен ключ (%d).",
	DBISTR_NONE 			= "%s: %s: грешка (%d).";

void dbi_message(DBIResult ErrorValue, mstr &m)
{
	DBIErrInfo ErrInfo;
	constant szContext[4] = { ErrInfo.szContext1, ErrInfo.szContext2, ErrInfo.szContext3, ErrInfo.szContext4 };

	DbiGetErrorInfo(true, &ErrInfo);

	if (ErrInfo.iError == ErrorValue)
	{
		m.cat(ErrInfo.szErrCode);
		for (int i = 0; i < 4; i++)
			if (any(szContext[i]))
				m.printf("\r\n\t%s", szContext[i]);
	}
	else
	{
		DBIMSG dbi_string = { '\0' };
		DbiGetErrorString(ErrorValue, dbi_string);
		m.cat(dbi_string);
	}

	if (!any(m))
		m.cat("Unknown BDE error");
}
