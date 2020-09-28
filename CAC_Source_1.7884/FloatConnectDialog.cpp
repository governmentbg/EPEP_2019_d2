//{{TFloatConnectDialog Implementation}}
DEFINE_RESPONSE_TABLE1(TFloatConnectDialog, TFloatDialog)
	EV_EN_KILLFOCUS(IDC_FLOAT_CONNECT_MIN_FINISHED, BClicked),
	EV_EN_KILLFOCUS(IDC_FLOAT_CONNECT_MAX_FINISHED, BClicked),
	EV_CBN_SELCHANGE(IDC_FLOAT_CONNECT_TYPES, BChange),
	EV_BN_CLICKED(IDC_FLOAT_CONNECT_QUERY_ACTS, QueryActsBNClicked),
END_RESPONSE_TABLE;


TFloatConnectDialog::TFloatConnectDialog(TWindow* parent, TFloatConnectGroup *group, int resId)
:
	TFloatDialog(parent, group, resId)
{
	types = new TTypeListFace(this, IDC_FLOAT_CONNECT_TYPES, "F_TYPES", TYPE_CONNECTABLES, false);
	minNo = new TLongFace(this, IDC_FLOAT_CONNECT_MIN_NO, "F_MIN_NO", SIZE_OF_NO);
	maxNo = new TLongFace(this, IDC_FLOAT_CONNECT_MAX_NO, "F_MAX_NO", SIZE_OF_NO);
	minYear = new TYearFace(this, IDC_FLOAT_CONNECT_MIN_YEAR, "F_MIN_YEAR");
	maxYear = new TYearFace(this, IDC_FLOAT_CONNECT_MAX_YEAR, "F_MAX_YEAR");
	connectTypes = new TCharListFace(this, IDC_FLOAT_CONNECT_CONNECT_TYPES, "F_CONNECT_TYPES", ConnectType, CONNECT_TYPE_VISIBLES, SIZE_OF_CONNECT_TYPES);
	sendersLabel = new TStatic(this, IDC_FLOAT_CONNECT_SENDERS_LABEL);
	senders = new TSenderListBox(this, IDC_FLOAT_CONNECT_SENDERS, UCN_FLOATCON_SENDERS);
	generalKinds = new TCharListFace(this, IDC_FLOAT_CONNECT_GENERAL_KINDS, "F_GENERAL_KINDS", Kind, KIND_ALL_GENERALS, SIZE_OF_KINDS);
	connectKinds = new TCharListFace(this, IDC_FLOAT_CONNECT_CONNECT_KINDS, "F_CONNECT_KINDS", ConnectKind, CONNECT_KIND_ALLS, SIZE_OF_CONNECT_KINDS);
	connectNo = new TConnectNoFace(this, IDC_FLOAT_CONNECT_CONNECT_NO, "F_CONNECT_NO");
	exactNo = new TCheckFace(this, IDC_FLOAT_CONNECT_EXACT_NO, "F_EXACT_NO");
	minDate = new TDateFace(this, IDC_FLOAT_CONNECT_MIN_DATE, "F_MIN_DATE", false);
	maxDate = new TDateFace(this, IDC_FLOAT_CONNECT_MAX_DATE, "F_MAX_DATE", false);
	minConnectYear = new TYearFace(this, IDC_FLOAT_CONNECT_MIN_CONNECT_YEAR, "F_MIN_CONNECT_YEAR");
	maxConnectYear = new TYearFace(this, IDC_FLOAT_CONNECT_MAX_CONNECT_YEAR, "F_MAX_CONNECT_YEAR");
	minFinished = new TDateFace(this, IDC_FLOAT_CONNECT_MIN_FINISHED, "F_MIN_FINISHED", false);
	maxFinished = new TDateFace(this, IDC_FLOAT_CONNECT_MAX_FINISHED, "F_MAX_FINISHED", false);
	queryActs = new TCheckFace(this, IDC_FLOAT_CONNECT_QUERY_ACTS, "F_QUERY_ACTS");
	results = new TCharListNameFace(this, IDC_FLOAT_CONNECT_RESULTS, "F_RESULTS", Result, RESULT_ENDOCS, SIZE_OF_RESULTS);
	text = new TStringFace(this, IDC_FLOAT_CONNECT_TEXT, "F_TEXT", SIZE_OF_CONNECT_TEXT);
}

void TFloatConnectDialog::SetupWindow()
{
	TFloatDialog::SetupWindow();
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_REGIONAL
	sendersLabel->SetCaption("&Институции/инстанции");
#endif  // ADMIN || REGIONAL
	QueryActsBNClicked();
}

bool TFloatConnectDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (!*types->S())
			isValid = error("Не е избран нито един вид дело.");
		else if (invalid_range(minNo, maxNo))
			isValid = error("Началния номер дело е по-голям от крайния.");
		else if (invalid_range(minYear, maxYear))
			isValid = error("Началната година е по-голяма от крайната.");
		else if (invalid_range(minDate, maxDate))
			isValid = error("Началната дата на образуване е по-голяма от крайната.");
		else if (invalid_range(minConnectYear, maxConnectYear))
			isValid = error("Началната година на съдебно дело е по-голяма от крайната.");
		else if (invalid_range(minFinished, maxFinished))
			isValid = error("Началната дата на свършване е по-голяма от крайната.");
		else if (queryActs->GetCheck() == BF_CHECKED && minFinished->Date().Empty() && maxFinished->Date().Empty())
			isValid = error("Периода на свършване е задължителен при справка за съдебни актове.");
	}

	return isValid;
}

void TFloatConnectDialog::Action(bool execute)
{
	msql m;

	senders->GetMark(&FloatConnectGroup()->senders);
	FloatConnectGroup()->FormCriteria(m);

	if (execute)
		query_connect(this, str(m), FloatConnectGroup()->queryActs);
	else
		Message(m);
}

void TFloatConnectDialog::BClicked()
{
	if (strchr(types->S(), TYPE_OUTREG) || strchr(types->S(), TYPE_INREG) || strchr(types->S(), TYPE_REQUEST))
	{
		types->SetS(TYPE_LAWSUITS);
	}
}
void TFloatConnectDialog::BChange()
{
	if (strchr(types->S(), TYPE_OUTREG) || strchr(types->S(), TYPE_INREG) || strchr(types->S(), TYPE_REQUEST))
	{
		minFinished->SetDate(CDate());
		maxFinished->SetDate(CDate());
	}
}

void TFloatConnectDialog::QueryActsBNClicked()
{
	results->EnableWindow(queryActs->GetCheck() == BF_CHECKED);
}
