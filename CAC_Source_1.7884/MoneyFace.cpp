DEFINE_RESPONSE_TABLE1(TMoneyFace, TStringFace)
//{{TMoneyFaceRSP_TBL_BEGIN}}
	EV_WM_CHAR,
//{{TMoneyFaceRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TMoneyFace Implementation}}

TMoneyFace::TMoneyFace(TWindow* parent, int resourceId, const char *name, uint textLimit)
:
	TStringFace(parent, resourceId, name, textLimit)
{
	SetTextLimit(textLimit);
	SetValidator(new TMoneyValidator(textLimit - 1));
}

double TMoneyFace::M() const
{
	S();
	return any(buffer) ? atof(buffer) : 0.0;
}

void TMoneyFace::SetM(double m)
{
	if (m)
		sprintf(buffer, "%.2lf", m);
	else
		*buffer = '\0';

	SetS(buffer);
}

void TMoneyFace::EvChar(uint key, uint repeatCount, uint flags)
{
	// possibly add '0' if 1st char dec point
	if (!strchr(".,", key) || !strchr(S(), '.'))
	{
		if (key == ',')
			SendMessage(WM_CHAR, '.', (flags << 16) + 1);
		else
			TStringFace::EvChar(key, repeatCount, flags);

		if (CHROME_AUTO_COMPLETE && GetLineLength(0) == 1 && key == '0')
			SendMessage(WM_CHAR, '.', (flags << 16) + 1);
	}
}
