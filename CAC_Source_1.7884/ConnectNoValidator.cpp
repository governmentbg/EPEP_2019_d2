//{{TConnectNoValidator Implementation}}

TConnectNoValidator::TConnectNoValidator(TCharAliasFace *tFace)
	: TFilterValidator("�-�A-Z0-9\\- "), face(tFace)
{
}

void TConnectNoValidator::Error(TWindow* )
{
	error("��������� �����.");
}

bool TConnectNoValidator::IsValid(const char far* input)
{
	bool result = TFilterValidator::IsValid(input);

	if (result)
		result = connect_no_valid(face ? face->C() : '\0', input);

	return result;
}
