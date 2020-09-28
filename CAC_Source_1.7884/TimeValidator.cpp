//{{TTimeValidator Implementation}}

TTimeValidator::TTimeValidator(bool tMandatory)
:
	TFilterValidator("0-9:"), mandatory(tMandatory)
{
}

void TTimeValidator::Error(TWindow* )
{
	error("��������� ��� ����������� �����.");
}

bool TTimeValidator::IsValid(const char far* input)
{
	bool result = TFilterValidator::IsValid(input);

	if (result)
		result = time_valid(input, mandatory);

	return result;
}
