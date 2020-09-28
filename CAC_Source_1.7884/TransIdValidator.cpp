//{{TTransIdValidator Implementation}}

TTransIdValidator::TTransIdValidator(bool tPartial)
:
	TFilterValidator(tPartial ? "0-9A-Z-" : "0-9A-Z"), partial(tPartial)
{
}

void TTransIdValidator::Error(TWindow* )
{
	error("Невалиден номер на транзакция.");
}

bool TTransIdValidator::IsValid(const char far* input)
{
	bool result = TFilterValidator::IsValid(input);

	if (result)
		result = trans_id_valid(input, partial);

	return result;
}
