//{{TBankValidator Implementation}}

TBankValidator::TBankValidator()
:
	TFilterValidator("A-Z0-9 ")	
{
}

void TBankValidator::Error(TWindow* )
{
	error("Невалиден текст.");
}
