//{{TYearValidator Implementation}}

TYearValidator::TYearValidator()
{
}

void TYearValidator::Error(TWindow*)
{
	error("Невалидна година.");
}

bool TYearValidator::IsValid(const char far* input)
{
	bool result = TLongValidator::IsValid(input);

	if (result)
		result = year_valid(input);

	return result;
}
