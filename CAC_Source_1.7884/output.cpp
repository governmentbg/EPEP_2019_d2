#include "cac.h"

#define TRACKER_MAX 100

TOutput::TOutput(TWindow *parent, const char *what, const char *wait)
	: TProcess(parent, what, wait)
{
}

bool TOutput::FindFinalizer(const TLawsuit &lawsuit, TSession &session, const CDate &start, const CDate &final)
{
	bool found = find_finalizer(&lawsuit, session, start, final);

	if (!found)
		dprintf("%s ���� ����������� ��������� ��� ������� ��� � �������\n", sLawKey);
	else if (session.final != lawsuit.finished)
	{
		dprintf("%s ��� ������� ���� �� ��������� %D�. ����� �� ������� � ���� �� %s �� %D�. � �������� %s\n",
			sLawKey, &lawsuit.finished, Kind->Seek(session.kind), &session.final, Result->Seek(session.result));
	}

	return found;
}

void TOutput::SetLawKey(const TLawsuit &lawsuit)
{
	sprintf(sLawKey, "%s %ld/%ld�.", Type->Seek(lawsuit.key.type), lawsuit.key.no, lawsuit.key.year);
}

void TOutput::TraceRelated(const TRCDKey &start, char connectType)
{
	TConnect connect;
	int count = 0;

	connect.key = start;
	while (connect.TryBy(connect.key, connectType, "F_CONNECT_NO, F_CONNECT_YEAR, F_CONNECT_TYPE"))
	{
		HandleRelated(connect);

		if (!atoi(connect.connectNo))
		{
			dprintf("������� �� ������� ���� ������������� �� %s %ld/%ld�. �� �������\n", Type->Seek(connect.key.type), connect.key.no, connect.key.year);
			break;
		}

		connect.key.no = atoi(connect.connectNo);
		connect.key.year = connect.connectYear;

		if (connect.key == start)
			fatal("%s � �������� ������� ���� �������������.", sLawKey);

		if (count++ == TRACKER_MAX)
			fatal("%s � ������� ���� ������������� ������ �� %ld ����.", sLawKey, TRACKER_MAX);
	}
}

void TOutput::HandleRelated(const TConnect &)
{
	fatal("%s: abstract call to HandleRelated", Name);
}
