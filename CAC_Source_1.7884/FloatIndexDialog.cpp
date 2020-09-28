DEFINE_RESPONSE_TABLE1(TFloatIndexDialog, TGroupDialog)
//{{TFloatDecisionDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TFloatDecisionDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TFloatDecisionDialog Implementation}}

TFloatIndexDialog::TFloatIndexDialog(TWindow* parent, TFloatIndexGroup *group, int resId)
:
	TGroupDialog(parent, group, resId)
{
	types = new TTypeListFace(this, IDC_FLOAT_INDEX_TYPES, "F_TYPES", QUERY_COLLEGES, false);
	decKinds = new TCharAutoListFace(this, IDC_FLOAT_INDEX_DEC_KINDS, "F_DEC_KINDS", Kind, KIND_VISIBLE_EVENT_INDEXES,
		SIZE_OF_KINDS);
	minFromDate = new TDateFace(this, IDC_FLOAT_INDEX_MIN_FROM_DATE, "F_MIN_FROM_DATE", false);
	maxFromDate = new TDateFace(this, IDC_FLOAT_INDEX_MAX_FROM_DATE, "F_MAX_FROM_DATE", false);
	minEventNo = new TLongFace(this, IDC_FLOAT_INDEX_MIN_EVENT_NO, "F_MIN_EVENT_NO", SIZE_OF_EVENT_NO);
	maxEventNo = new TLongFace(this, IDC_FLOAT_INDEX_MAX_EVENT_NO, "F_MAX_EVENT_NO", SIZE_OF_EVENT_NO);
}

bool TFloatIndexDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minEventNo, maxEventNo))
			isValid = error("Началния номер на съдебен акт е по-голям от крайния.");
		else if (invalid_range(minFromDate, maxFromDate))
			isValid = error("Началната дата на съдебния акт е по-голяма от крайната.");
	}

	return isValid;
}

void TFloatIndexDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Group() << this;
		query_index(this, FloatIndexGroup());
	}
}
