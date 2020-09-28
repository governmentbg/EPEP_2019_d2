//{{TFloatMovementDialog Implementation}}

TFloatMovementDialog::TFloatMovementDialog(TWindow* parent, TFloatMovementGroup *group, int resId)
:
	TFloatDialog(parent, group, resId)
{
	users = new TUserListBox(this, IDC_FLOAT_MOVEMENT_USER, POST_USERS);
	types = new TTypeListFace(this, IDC_FLOAT_MOVEMENT_TYPES, "F_TYPES", TYPE_MOVABLES, false);
	kinds = new TCharListFace(this, IDC_FLOAT_MOVEMENT_KINDS, "F_KINDS", Kind, KIND_MOVEMENTS, SIZE_OF_KINDS);
	minDate = new TDateFace(this, IDC_FLOAT_MOVEMENT_MIN_DATE, "F_MIN_DATE", false);
	maxDate = new TDateFace(this, IDC_FLOAT_MOVEMENT_MAX_DATE, "F_MAX_DATE", false);
	new TRadioFace(this, IDC_FLOAT_MOVEMENT_BY_TKYN, "F_BY_TKYN");
	new TRadioFace(this, IDC_FLOAT_MOVEMENT_BY_DTTYN, "F_BY_DTTYN");
	text = new TStringFace(this, IDC_FLOAT_MOVEMENT_TEXT, "F_TEXT", SIZE_OF_ORIGIN_TEXT);
}

bool TFloatMovementDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minDate, maxDate))
			isValid = error("Ќачалната дата е по-гол€ма от крайната.");
	}

	return isValid;
}

void TFloatMovementDialog::Action(bool execute)
{
	msql m;

	users->GetMark(&FloatMovementGroup()->users);
	FloatMovementGroup()->FormCriteria(m);
	
	if (execute)
		query_movement(this, str(m), FloatMovementGroup()->order);
	else
		Message(m);
}
