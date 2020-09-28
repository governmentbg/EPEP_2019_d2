//{{TMoneyValidator Implementation}}

TMoneyValidator::TMoneyValidator(int nLen)
:
	TFilterValidator("0-9."), len(nLen)
{
}

void TMoneyValidator::Error(TWindow*)
{
	error("Невалидно число.");
}

bool TMoneyValidator::IsValid(const char far* input)
{
	bool result = TFilterValidator::IsValid(input);

	if (result)
		result = money_valid(input, len);

	return result;
}
