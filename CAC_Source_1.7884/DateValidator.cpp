//{{TDateValidator Implementation}}

TDateValidator::TDateValidator(sign tMandatory)
:
	TPXPictureValidator("##.##.####"), mandatory(tMandatory)
{
}

void TDateValidator::Error(TWindow* )
{
	error("��������� ��� ����������� ����.");
}

bool TDateValidator::IsValid(const char far* input)
{
	bool result = TPXPictureValidator::IsValid(input);

	if (result)
		result = date_valid(input, mandatory);

	return result;
}
