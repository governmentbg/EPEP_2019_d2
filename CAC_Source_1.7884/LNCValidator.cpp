//{{TLNCValidator Implementation}}

TLNCValidator::TLNCValidator()
:
	TPXPictureValidator("##########")
{
}

void TLNCValidator::Error(TWindow* )
{
	error("��������� ���.");
}

bool TLNCValidator::IsValid(const char far* input)
{
	bool result = TPXPictureValidator::IsValid(input);

	if (result)
		result = lnc_valid(input);

	return result;
}
