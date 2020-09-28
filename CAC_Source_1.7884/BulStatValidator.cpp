//{{TBulStatValidator Implementation}}

TBulStatValidator::TBulStatValidator()
:
	TFilterValidator("¿-ﬂA-Z0-9")
{
}

void TBulStatValidator::Error(TWindow* )
{
	error("ÕÂ‚‡ÎË‰ÂÌ ≈» /¡”À—“¿“.");
}

bool TBulStatValidator::IsValid(const char far* input)
{
	bool result = TFilterValidator::IsValid(input);

	if (result)
		result = bulstat_valid(input);

	return result;
}
