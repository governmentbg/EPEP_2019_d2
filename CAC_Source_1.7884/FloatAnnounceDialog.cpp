//{{TFloatAnnounceDialog Implementation}}

TFloatAnnounceDialog::TFloatAnnounceDialog(TWindow* parent, TFloatAnnounceGroup *group, int resId)
:
	TFloatDialog(parent, group, resId)
{
	kinds = new TCharListFace(this, IDC_FLOAT_ANNOUNCE_KINDS, "F_KINDS", ConnectKind, CONNECT_KIND_ANNOUNCES,
		SIZE_OF_CONNECT_KINDS);
	minNo = new TLongFace(this, IDC_FLOAT_ANNOUNCE_MIN_NO, "F_MIN_NO", SIZE_OF_NO);
	maxNo = new TLongFace(this, IDC_FLOAT_ANNOUNCE_MAX_NO, "F_MAX_NO", SIZE_OF_NO);
	minYear = new TYearFace(this, IDC_FLOAT_ANNOUNCE_MIN_YEAR, "F_MIN_YEAR");
	maxYear = new TYearFace(this, IDC_FLOAT_ANNOUNCE_MAX_YEAR, "F_MAX_YEAR");
	senders = new TSenderListBox(this, IDC_FLOAT_ANNOUNCE_SENDERS, C2S[UCN_INSTANCE_EXTERNAL]);
	minSessionDate = new TDateFace(this, IDC_FLOAT_ANNOUNCE_MIN_SESSION_DATE, "F_MIN_SESSION_DATE", false);
	maxSessionDate = new TDateFace(this, IDC_FLOAT_ANNOUNCE_MAX_SESSION_DATE, "F_MAX_SESSION_DATE", false);
	minSubpoenaDate = new TDateFace(this, IDC_FLOAT_ANNOUNCE_MIN_SUBPOENA_DATE, "F_MIN_SUBPOENA_DATE", false);
	maxSubpoenaDate = new TDateFace(this, IDC_FLOAT_ANNOUNCE_MAX_SUBPOENA_DATE, "F_MAX_SUBPOENA_DATE", false);
	subpoenaKinds = new TCharListFace(this, IDC_FLOAT_ANNOUNCE_SUBPOENA_KINDS, "F_SUBPOENA_KINDS", SubpoenaKind,
		SUBPOENA_KIND_GENERICS, SIZE_OF_SUBPOENA_KINDS);
	receivedStatuses = new TCharListFace(this, IDC_FLOAT_ANNOUNCE_RECEIVED_STATUSES, "F_RECEIVED_STATUSES", ReceivedStatus,
		SRS_ANNOUNCES, SIZE_OF_RECEIVED_STATUSES);
	deliverers = new TUserListBox(this, IDC_FLOAT_ANNOUNCE_DELIVERERS, C2S[POST_DELIVERER]);
	minReturnedDate = new TDateFace(this, IDC_FLOAT_ANNOUNCE_MIN_RETURNED_DATE, "F_MIN_RETURNED_DATE", false);
	maxReturnedDate = new TDateFace(this, IDC_FLOAT_ANNOUNCE_MAX_RETURNED_DATE, "F_MAX_RETURNED_DATE", false);
	new TRadioFace(this, IDC_FLOAT_ANNOUNCE_BY_DKYN, "F_BY_DKYN");
	new TRadioFace(this, IDC_FLOAT_ANNOUNCE_BY_SDKYN, "F_BY_SDKYN");
	new TRadioFace(this, IDC_FLOAT_ANNOUNCE_BY_NMKYN, "F_BY_NMKYN");
}

bool TFloatAnnounceDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minNo, maxNo))
			isValid = error("Началния номер дело е по-голям от крайния.");
		else if (invalid_range(minYear, maxYear))
			isValid = error("Началната година е по-голяма от крайната.");
		else if (invalid_range(minSessionDate, maxSessionDate))
			isValid = error("Началната дата на насрочване е по-голяма от крайната.");
		else if (invalid_range(minSubpoenaDate, maxSubpoenaDate))
			isValid = error("Началната дата на постъпване е по-голяма от крайната.");
		else if (invalid_range(minReturnedDate, maxReturnedDate))
			isValid = error("Началната дата на връщане е по-голяма от крайната.");
	}

	return isValid;
}

void TFloatAnnounceDialog::Action(bool execute)
{
	msql m;

	senders->GetMark(&FloatAnnounceGroup()->senders);
	deliverers->GetMark(&FloatAnnounceGroup()->deliverers);
	FloatAnnounceGroup()->FormCriteria(m);

	if (execute)
		query_announce(this, str(m));
	else
		Message(m);
}
