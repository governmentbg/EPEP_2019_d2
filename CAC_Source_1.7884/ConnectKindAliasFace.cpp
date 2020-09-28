//{{TConnectKindAliasFace Implementation}}

TConnectKindAliasFace::TConnectKindAliasFace(TWindow* parent, int resourceId, const char *name, char connectType, long flags)
:
	TCharAliasFace(parent, resourceId, name, ConnectKind, connectKinds, flags)
{
	if (connectType)
		SetConnectKinds(connectType);
}

void TConnectKindAliasFace::SetConnectType(char connectType)
{
	SetConnectKinds(connectType);
	SetXCrit(connectKinds);
}

void TConnectKindAliasFace::SetDefaultKind(char originType)
{
	if (originType)
	{
		char defaultKind = type2ConnectKind(originType);

		if (!strchr(connectKinds, defaultKind) && strchr(TYPE_CITIZALIKE_LAWS, originType))
			defaultKind = CONNECT_KIND_CITIZEN_LAWSUIT;
		SetC(defaultKind);
	}
}

void TConnectKindAliasFace::SetConnectKinds(char connectType)
{
	*connectKinds = '\0';

	if (connectType)
	{
		constant constKinds = strchr(CONNECT_TYPE_SELFS, connectType) ? CONNECT_KIND_SELFS : CONNECT_KIND_ALLS;

		ConnectType->Seek(connectType);

		for (const char *s = ConnectType->connectKinds; *s; s++)
			if (strchr(constKinds, *s))
				strcach(connectKinds, *s);
	}
}
