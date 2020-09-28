TSlowBookDialog::TSlowBookDialog(TWindow* parent, TSlowBookGroup *group, int resId)
	: TFloatDialog(parent, group, resId)
{
	types = new TCharListFace(this, IDC_SDBK_LAW_TYPE, "F_TYPES", Type, TYPE_TRADE_OR_CITIZEN_LAWS, SIZE_OF_TYPES);
	minDate = new TDateFace(this, IDC_SDBK_BEG_DATE, "F_MIN_DATE");
	new TDateFaceUpDown(this, IDC_SDBK_BEG_DATE_UPDN, minDate);
	maxDate = new TDateFace(this, IDC_SDBK_END_DATE, "F_MAX_DATE");
	new TDateFaceUpDown(this, IDC_SDBK_END_DATE_UPDN, maxDate);
	new TLongFace(this, IDC_SDBK_BEG_SERIAL_NO, "F_AUTO_SERIAL_NO", SIZE_OF_EVENT_NO);
}

bool TSlowBookDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (!*types->S())
			isValid = error("Ќе е избран нито един вид дело.");
		else if (invalid_range(minDate, maxDate))
			isValid = error("Ќачалната дата на разглеждане е по-гол€ма от крайната.");
	}

	return isValid;
}
