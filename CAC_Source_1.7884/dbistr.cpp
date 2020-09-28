#include "cac.h"

constant
	DBISTR_RECLOCKED			= "%s: ������� �� ��������� � ���������� �� %s (%d).",
	DBISTR_DETAILRECORDSEXIST	= "%s: ������� �� �������� �� %s ������ ��������� �� ��������� ����� (%d).",
	DBISTR_NOTSUFFTABLERIGHTS	= "%s: ���������� %s � �������� ������ ������� �� ����������� ����� �� ������ � ��������� (%d).",
	DBISTR_NOTSUFFSQLRIGHTS		= "%s: ���������� %s � �������� ������ ������� �� ����������� ����� �� ������ � ������� (%d).",
	DBISTR_RECLOCKFAILED		= "%s: ������� �� ����� �� ����� ��������� �� %s ������ ��������� �� ����������� (%d).",
	DBISTR_KEYVIOL			= "%s: ���������� %s �� ������ �� ��������� �� ���� (%d).",
	DBISTR_FILELOCKED			= "%s: ����� � �������� � ���������� �� %s (%d).",
	DBISTR_MINVALERR			= "%s: ���� �� ������� � ��� ���������� ��������� �������� � ���������� �� %s (%d).",
	DBISTR_MAXVALERR			= "%s: ���� �� ������� � ��� ���������� ���������� �������� � ���������� �� %s (%d).",
	DBISTR_REQDERR			= "%s: ���� �� ������� � ������ (��� ����� ���������� ���������) � ���������� �� %s (%d).",
	DBISTR_NULLERROR			= "%s: ���� �� ������� � ������ � ���������� �� %s (%d).",
	DBISTR_RANGEERROR			= "%s: ���� �� ������� � ������ � ���������� �� %s (%d).",
	DBISTR_TABLEREADONLY		= "%s: ��������� � ���� �� ������ � ���������� �� %s (%d).",
	DBISTR_NODISKSPACE		= "%s: ���������� %s �� ���� �� �� ������� ������ ������� �� ���������� ������� ������������ (%d).",
	DBISTR_ALREADYLOCKED		= "%s: ������ � �������� � ���������� �� %s (%d).",
	DBISTR_UNKNOWNSQL			= "%s: ���������� %s � ��������, ���-�������� ������ ���������� �� ������ ���� (%d).",
	DBISTR_INTEGRITY			= "%s: ���������� %s � ��������, ������ �� �������� �������� �� �������. (%d)",
	DBISTR_FOREIGNKEY			= "%s: ���������� %s � �������� ������ ���������� �� ������ ���� (%d).",
	DBISTR_NONE 			= "%s: %s: ������ (%d).";

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
