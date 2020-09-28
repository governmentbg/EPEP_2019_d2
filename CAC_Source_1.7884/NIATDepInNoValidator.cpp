//{{TNIATDepInNoValidator Implementation}}

TNIATDepInNoValidator::TNIATDepInNoValidator()
	: TFilterValidator("0-9A-ZА-Я\\-")
{
}

void TNIATDepInNoValidator::Error(TWindow* )
{
	error("Невалиден номер.");
}
