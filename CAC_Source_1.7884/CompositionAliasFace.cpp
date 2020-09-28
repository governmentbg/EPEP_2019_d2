//{{TCompositionAliasFace Implementation}}

TCompositionAliasFace::TCompositionAliasFace(TWindow* parent, int resourceId, const char *name, const char *compositions, long flags)
:
	TLongAliasFace(parent, resourceId, name, Composition, compositions, flags), court(COURT_CODE)
{
}

void TCompositionAliasFace::SetSender(char ucnType, const char *ucn)
{
	TSender sender;

	sender.ucnType = ucnType;
	strcpy(sender.ucn, ucn);

	if (atob(ucn) && sender.Get())
	{
		if (sender.court != court)
		{
			court = sender.court;
			Refresh();
		}
	}
}

void TCompositionAliasFace::InsertVisual(int index)
{
	if (court == COURT_CODE)
		TLongAliasFace::InsertVisual(index);
	else
		InsertString(romanian_name(LongAt(index)), index);
}
