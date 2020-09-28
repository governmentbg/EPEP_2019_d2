//{{TFzlNameValidator Implementation}}

TFzlNameValidator::TFzlNameValidator(sign tCyrillic, const char *tErrorMsg)
:
	TFilterValidator(tCyrillic ? "À-ß -" : "A-Z -"), cyrillic(tCyrillic), errorMsg(tErrorMsg)
{
}

void TFzlNameValidator::Error(TWindow* )
{
	error("%s.", errorMsg);
}

bool TFzlNameValidator::IsValid(const char far* input)
{
	bool result = TFilterValidator::IsValid(input);

	if (result)
		result = fzl_name_valid(cyrillic, input);

	return result;
}
