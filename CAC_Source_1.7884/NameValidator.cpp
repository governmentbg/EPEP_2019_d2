//{{TNameValidator Implementation}}

TNameValidator::TNameValidator(const TCharSet& validChars)
:
	TFilterValidator(validChars)
{
}

void TNameValidator::Error(TWindow* )
{
	error("Невалидно име.");
}

bool TNameValidator::IsValid(const char far* input)
{
	bool result = TFilterValidator::IsValid(input);

	if (result)
		result = name_valid(input);

	return result;
}
