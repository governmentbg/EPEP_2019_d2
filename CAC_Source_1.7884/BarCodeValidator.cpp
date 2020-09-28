//{{TBarCodeValidator Implementation}}

TBarCodeValidator::TBarCodeValidator()
	: TFilterValidator("0-9A-Z*")
{
}

void TBarCodeValidator::Error(TWindow*)
{
	error("Невалиден баркод.");
}
