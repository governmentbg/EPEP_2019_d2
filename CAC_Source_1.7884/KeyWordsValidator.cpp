//{{TKeyWordsValidator Implementation}}

TKeyWordsValidator::TKeyWordsValidator()
:
	TFilterValidator("А-Я0-9 ,.;")
{
}

void TKeyWordsValidator::Error(TWindow* )
{
	error("Невалидни или прекалено дълги ключови думи.");
}

bool TKeyWordsValidator::IsValid(const char far* input)
{
	bool result = TFilterValidator::IsValid(input);

	if (result)
		result = keywords_valid(input);

	return result;
}
