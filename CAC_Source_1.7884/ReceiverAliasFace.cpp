DEFINE_RESPONSE_TABLE1(TReceiverAliasFace, TStringAliasFace)
//{{TReceiverAliasFaceRSP_TBL_BEGIN}}
	EV_NOTIFY_AT_CHILD(CBN_DROPDOWN, CBNDropDown),
//{{TReceiverAliasFaceRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TReceiverAliasFace Implementation}}

TReceiverAliasFace::TReceiverAliasFace(TWindow* parent, int resourceId, const char *name, TReceiver *receiver,
	const char *xCrit, const char *tWhat)
:
	TStringAliasFace(parent, resourceId, name, receiver, xCrit, ALIAS_ABCSORT | ALIAS_DROPDOWN),
		what(tWhat ? tWhat : "Адресантът")
{
	initialRefresh = false;
	SetTextLimit(SIZE_OF_NAME);
	*receiver->name = '\0';		// 2014:213 FIX: for the global Receiver
}

TReceiverAliasFace::~TReceiverAliasFace()
{
	*((TReceiver *) alias)->name = '\0';	// 2014:213 FIX: for the global Receiver
}

const char *TReceiverAliasFace::SNow()
{
	int index = GetSelIndex();
	return index >= 0 ? StringAt(index) : "";
}

char TReceiverAliasFace::UCNType()
{
	TReceiver *receiver = (TReceiver *) Group();
	return receiver ? receiver->ucnType : '\0';
}

bool TReceiverAliasFace::IsValid()
{
	return TStringAliasFace::IsValid() ? atob(SNow()) ? true : error("%s е задължителен.", what) : false;
}

void TReceiverAliasFace::Criteria(msql &m, TGroup *group)
{
	alias->NameField->SetS(text);
	TStringAliasFace::Criteria(m, group);
}

int TReceiverAliasFace::GetVisualIndex()
{
	return FindVisualIndex();
}

void TReceiverAliasFace::CBNDropDown()
{
	TAliasFace::CBNDropDown();
	GetText();
	Refresh();
}

void TReceiverAliasFace::SetUCN(const char *ucn)
{
	*text = '\0';
	Refresh();
	SetS(ucn);
}
