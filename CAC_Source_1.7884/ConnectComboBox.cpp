DEFINE_RESPONSE_TABLE1(TConnectComboBox, TGroupComboBox)
//{{TConnectComboBoxRSP_TBL_BEGIN}}
	EV_MESSAGE(WM_FACE_VALID, EvFaceValid),
//{{TConnectComboBoxRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TConnectComboBox Implementation}}

TConnectComboBox::TConnectComboBox(TWindow* parent, int resourceId, SBE *sbe, TRCDKeyContainer *origin)
:
	TGroupComboBox(parent, resourceId, new TGArray<TConnect>, 0x100)
{
	loadData = false;

	if (origin->key.type == TYPE_OUTREG)
	{
		TConnect *connect = new TConnect;

		connectType = '\034';

		if (connect->TryBy(origin, CONNECT_TYPE_VZ_LAWSUIT) || connect->TryBy(origin, CONNECT_TYPE_KS_LAWSUIT))
			array->Add(connect);
		else
			delete connect;
	}
	else
	{
		load_connects(origin, (TGArray<TConnect> *) array);

		switch (sbe->vid)
		{
			case sbe_vnedrsprk : connectType = CONNECT_TYPE_PROSEC_REQUEST; break;
			case sbe_vnerzgpis : connectType = CONNECT_TYPE_LOWER_INSTANCE; break;
			case sbe_olqpdt : connectType = '\033'; break;
			case sbe_plcdlo : connectType = '\035'; break;
			default : connectType = '\0';
		}

		for (int i = 0; i < array->Count(); )
		{
			TConnect *connect = (TConnect *) GroupAt(i);
			ConnectType->Seek(connect->connectType);

			if (connectType == '\033' ? (strchr(CONNECT_TYPE_STARTUPS, connect->connectType) ||
				connect->connectType == CONNECT_TYPE_HIGH_JURISDICTION) :
				// 2013:010 LRQ: // ConnectType->eispp == EISPP_COURT_CASE
				// 2014:036 LPR/IRQ: && ConnectType->eispp
				connectType == '\035' ? (connect->xferKind && ConnectType->eispp) :
				connectType ? connect->connectType == connectType : ConnectType->eispp)
			{
				i++;
			}
			else
				array->Remove(i);
		}
	}
}

const char *TConnectComboBox::Label()
{
	switch (connectType)
	{
		case '\033' : return "Дело по подсъдност";
		case '\034' : return "Дело на висша инстанция";
		case '\035' : return "Получено дело";
		default : return connectType ? ConnectType->Seek(connectType) : "Дело по което се образува";
	}
}

long TConnectComboBox::Eispp()
{
	long eispp = 0;
	TConnect *connect = Connect();

	if (connect)
	{
		TSender sender;
		sender << connect;

		if (sender.Try())
			eispp = sender.eispp;
	}

	return eispp;
}

TGroup *TConnectComboBox::NewGroup() const
{
	return new TConnect;
}

void TConnectComboBox::Refresh()
{
	TGroupComboBox::Refresh();

	if (array->Count())
		SetVisualIndex(0);
}

long TConnectComboBox::EvFaceValid(TParam1, TParam2)
{
	if (!Group())
	{
		error("Данните за %s са задължителни.", Label());
		return MW_INVALID_FACE;
	}

	return 0;
}
