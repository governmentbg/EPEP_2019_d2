//{{TXferKindAliasFace Implementation}}

TXferKindAliasFace::TXferKindAliasFace(TWindow* parent, int resourceId, const char *name, char connectType, long flags)
:
	TLongAliasFace(parent, resourceId, name, XferKind, connectKinds, flags)
{
	if (connectType)
		SetConnectKinds(connectType);
}

void TXferKindAliasFace::SetConnectType(char connectType)
{
	SetConnectKinds(connectType);
	SetXCrit(connectKinds);
}

void TXferKindAliasFace::SetDefaultKind(char originType)
{
	if (originType)
	{
		char defaultKind = type2ConnectKind(originType);

		if (!strchr(connectKinds, defaultKind) && strchr(TYPE_CITIZALIKE_LAWS, originType))
			defaultKind = CONNECT_KIND_CITIZEN_LAWSUIT;

		for (int index = 0; index < array->Count(); index++)
		{
			TXferKind *xferKind = (TXferKind *) (*array)[index];

			if (xferKind->connectKind == defaultKind)
			{
				SetVisualIndex(index);
				break;
			}
		}
	}
}

void TXferKindAliasFace::SetConnectKinds(char connectType)
{
	*connectKinds = '\0';

	if (connectType)
	{
		// 2013:029 LPR: EISPP only
		//constant constKinds = strchr(CONNECT_TYPE_SELFS, connectType) ? CONNECT_KIND_SELFS : CONNECT_KIND_ALLS;
		const char *constKinds;

		ConnectType->Seek(connectType);
		constKinds = ConnectType->eispp == EISPP_COURT_CASE ? CONNECT_KIND_EISPP_LAWS : CONNECT_KIND_ALLS;

		for (const char *s = ConnectType->connectKinds; *s; s++)
			if (strchr(constKinds, *s))
				strcach(connectKinds, *s);
	}
}
