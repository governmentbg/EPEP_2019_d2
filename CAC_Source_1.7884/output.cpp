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
		dprintf("%s няма приключващо заседание или съдебен акт в периода\n", sLawKey);
	else if (session.final != lawsuit.finished)
	{
		dprintf("%s има вписана дата на свършване %Dг. която не съвпада с тази на %s от %Dг. с резултат %s\n",
			sLawKey, &lawsuit.finished, Kind->Seek(session.kind), &session.final, Result->Seek(session.result));
	}

	return found;
}

void TOutput::SetLawKey(const TLawsuit &lawsuit)
{
	sprintf(sLawKey, "%s %ld/%ldг.", Type->Seek(lawsuit.key.type), lawsuit.key.no, lawsuit.key.year);
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
			dprintf("Данните за внесено след доразследване за %s %ld/%ldг. са непълни\n", Type->Seek(connect.key.type), connect.key.no, connect.key.year);
			break;
		}

		connect.key.no = atoi(connect.connectNo);
		connect.key.year = connect.connectYear;

		if (connect.key == start)
			fatal("%s е циклично внесено след доразследване.", sLawKey);

		if (count++ == TRACKER_MAX)
			fatal("%s е внесено след доразследване повече от %ld пъти.", sLawKey, TRACKER_MAX);
	}
}

void TOutput::HandleRelated(const TConnect &)
{
	fatal("%s: abstract call to HandleRelated", Name);
}
