//{{TEisNmrValidator Implementation}}

TEisNmrValidator::TEisNmrValidator(char tLetter)
:
	TPXPictureValidator(letter ? "???########???" : "[?][?][?][#][#][#][#][#][#][#][#][?][?][?]"), letter(tLetter)
{
}

void TEisNmrValidator::Error(TWindow* )
{
	error("Невалиден ЕИСПП номер.");
}

bool TEisNmrValidator::IsValid(const char far* input)
{
	bool result = TPXPictureValidator::IsValid(input);

	if (result && letter)
		result = eispp_nmr_valid(input, letter);

	return result;
}
