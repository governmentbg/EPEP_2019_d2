//{{TLongValidator Implementation}}

TLongValidator::TLongValidator(bool tPositive)
:
	TFilterValidator(tPositive ? "0-9" : "0-9-"), positive(tPositive)
{
}

void TLongValidator::Error(TWindow*)
{
	error("Невалидно число.");
}

bool TLongValidator::IsValid(const char far* input)
{
	bool result = TFilterValidator::IsValid(input + (!positive && *input == '-'));

	if (result)
		result = long_valid(input);

	return result;
}

void TLongValidator::SetPositive(bool tPositive)
{
	positive = tPositive;
	SetValidChars(positive ? "0-9" : "0-9-");
}
