//{{TBulStatValidator Implementation}}

TBulStatValidator::TBulStatValidator()
:
	TFilterValidator("�-�A-Z0-9")
{
}

void TBulStatValidator::Error(TWindow* )
{
	error("��������� ���/�������.");
}

bool TBulStatValidator::IsValid(const char far* input)
{
	bool result = TFilterValidator::IsValid(input);

	if (result)
		result = bulstat_valid(input);

	return result;
}
