//{{TUCNValidator Implementation}}

TUCNValidator::TUCNValidator(char tType)
	: TPXPictureValidator("##########"), type(tType), face(NULL)
{
}

TUCNValidator::TUCNValidator(TCharAliasFace *tFace)
	: TPXPictureValidator("##########"), type('\0'), face(tFace)
{
}

void TUCNValidator::Error(TWindow* )
{
	error("Невалиден код.");
}

bool TUCNValidator::IsValid(const char far* input)
{
	bool result = TPXPictureValidator::IsValid(input);

	if (result)
	{
		if (face)
			type = face->C();

		result = ucn_valid(type, input);
	}

	return result;
}
