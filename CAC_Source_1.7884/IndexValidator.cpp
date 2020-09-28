//{{TIndexValidator Implementation}}

TIndexValidator::TIndexValidator(const char *tKinds)
:
	kinds(tKinds)
{
}

void TIndexValidator::Error(TWindow*)
{
	error("Невалидна стойност.");
}

bool TIndexValidator::IsValid(const char far* input)
{
	bool result = TLongValidator::IsValid(input);

	if (result)
		result = index_valid(input, kinds);

	return result;
}
