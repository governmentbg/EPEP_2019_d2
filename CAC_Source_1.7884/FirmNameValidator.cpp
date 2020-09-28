//{{TFirmNameValidator Implementation}}

TFirmNameValidator::TFirmNameValidator()
:
	TNameValidator("À-ßA-Z0-9 .\"\\-:,&!/")	
{
}

bool TFirmNameValidator::IsValid(const char far* input)
{
	bool result = TFilterValidator::IsValid(input);

	if (result)
		result = firm_name_valid(input);

	return result;
}
