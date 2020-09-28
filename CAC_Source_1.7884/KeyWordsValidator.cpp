//{{TKeyWordsValidator Implementation}}

TKeyWordsValidator::TKeyWordsValidator()
:
	TFilterValidator("�-�0-9 ,.;")
{
}

void TKeyWordsValidator::Error(TWindow* )
{
	error("��������� ��� ��������� ����� ������� ����.");
}

bool TKeyWordsValidator::IsValid(const char far* input)
{
	bool result = TFilterValidator::IsValid(input);

	if (result)
		result = keywords_valid(input);

	return result;
}
