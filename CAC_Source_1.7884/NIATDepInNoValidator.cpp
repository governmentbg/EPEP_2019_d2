//{{TNIATDepInNoValidator Implementation}}

TNIATDepInNoValidator::TNIATDepInNoValidator()
	: TFilterValidator("0-9A-Z�-�\\-")
{
}

void TNIATDepInNoValidator::Error(TWindow* )
{
	error("��������� �����.");
}
