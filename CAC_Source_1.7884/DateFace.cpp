DEFINE_RESPONSE_TABLE1(TDateFace, TStringFace)
//{{TDateFaceRSP_TBL_BEGIN}}
	EV_MESSAGE(WM_FACE_VALID, EvFaceValid),
	EV_WM_CHAR,
//{{TDateFaceRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TDateFace Implementation}}

TDateFace::TDateFace(TWindow* parent, int resourceId, const char *name, sign mandatory)
	: TStringFace(parent, resourceId, name, SIZE_OF_DATE)
{
	SetValidator(new TDateValidator(mandatory));
}

bool TDateFace::IsValid(bool reportErr)
{
	Date();
	return (date.Empty() && !IsWindowEnabled()) || TStringFace::IsValid(reportErr);
}

const CDate &TDateFace::Date() const
{
	date.Clear();
	S();

	if (any(buffer))
		sscanf(buffer, "%d.%d.%ld", &date.day, &date.month, &date.year);

	return date;
}

void TDateFace::SetDate(const CDate &tDate)
{
	date = tDate;

	if (date.Empty())
		*buffer = '\0';
	else
		sprintf(buffer, "%0D", &date);

	SetS(buffer);
}

void TDateFace::SetupWindow()
{
	TStringFace::SetupWindow();
	SetDate(Today);
}

long TDateFace::EvFaceValid(TParam1, TParam2)
{
	return IsValid(true) ? 0 : MW_INVALID_FACE;
}

void TDateFace::EvChar(uint key, uint repeatCount, uint flags)
{
	int len = GetLineLength(0);
	int tFlags = (flags << 16) + 1;

	switch (key)
	{
		case ',' : SendMessage(WM_CHAR, '.', tFlags); break;
		case 'd' : case 'D' :
		case 'ä' : case 'Ä' : if (len == 0) SetDate(Today); break;
		case '-' :
		case '+' :
		{
			if (date_valid(S(), true))
			{
				Date();

				if (key == '+' ? date < DATE_MAX : date > DATE_MIN)
					date += key == '+' ? 1 : -1;

				sprintf(buffer, "%0D", &date);
				SetS(buffer);
			}
		}
		default :
		{
			TStringFace::EvChar(key, repeatCount, flags);

			if (CHROME_AUTO_COMPLETE && isdigit(key) && (len == 1 || len == 4))
			{
				// 2012:202 FIX: send '.' at end of text
				SendMessage(WM_KEYDOWN, VK_END, tFlags);
				SendMessage(WM_KEYUP, VK_END, tFlags);
				SendMessage(WM_CHAR, '.', tFlags);
			}
		}
	}
}
