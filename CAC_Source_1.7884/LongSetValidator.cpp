//{{TLongSetValidator Implementation}}

TLongSetValidator::TLongSetValidator()
:
	TFilterValidator("0-9 ,.")
{
}

void TLongSetValidator::Error(TWindow* )
{
	error("Невалидно множество.");
}

bool TLongSetValidator::IsValid(const char far* input)
{
	bool result = TFilterValidator::IsValid(input);

	if (result)
		result = long_set_valid(input);

	return result;
}
