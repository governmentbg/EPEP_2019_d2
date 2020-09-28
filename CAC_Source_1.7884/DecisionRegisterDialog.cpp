TDecisionRegisterDialog::TDecisionRegisterDialog(TWindow* parent, TDecisionRegisterGroup *group, int resId)
	: TFloatDialog(parent, group, resId)
{
	new TTypeListFace(this, IDC_DECISION_REGISTER_BOOK_LAW_TYPE, "F_TYPES", TYPE_LAWSUITS, false);
	minDate = new TDateFace(this, IDC_DECISION_REGISTER_BOOK_MIN_DATE, "F_MIN_DATE", false);
	new TDateFaceUpDown(this, IDC_DECISION_REGISTER_BOOK_MIN_DATE_UPDN, minDate);
	maxDate = new TDateFace(this, IDC_DECISION_REGISTER_BOOK_MAX_DATE, "F_MAX_DATE", false);
	new TDateFaceUpDown(this, IDC_DECISION_REGISTER_BOOK_MAX_DATE_UPDN, maxDate);
	new TRadioFace(this, IDC_DECISION_REGISTER_BOOK_DEC_TEXT_NONE, "F_DEC_TEXT_NONE");
	new TRadioFace(this, IDC_DECISION_REGISTER_BOOK_DEC_TEXT_FILTERED, "F_DEC_TEXT_FILTERED");
	new TRadioFace(this, IDC_DECISION_REGISTER_BOOK_DEC_TEXT_FULL, "F_DEC_TEXT_FULL");
	autoSerialNo  = new TLongFace(this, IDC_DECISION_REGISTER_BOOK_SERIAL_NO, "F_AUTO_SERIAL_NO", SIZE_OF_NO);
}

bool TDecisionRegisterDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minDate, maxDate))
			isValid = error("Началната дата е по-голяма от крайната.");
		else if (*autoSerialNo->S() && !autoSerialNo->L())
			isValid = error("Невалиден начален номер.");
	}

	return isValid;
}
