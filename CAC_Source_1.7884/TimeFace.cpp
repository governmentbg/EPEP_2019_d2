DEFINE_RESPONSE_TABLE1(TTimeFace, TStringFace)
//{{TTimeFaceRSP_TBL_BEGIN}}
	EV_WM_CHAR,
//{{TTimeFaceRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TTimeFace Implementation}}

TTimeFace::TTimeFace(TWindow* parent, int resourceId, const char *name, bool mandatory)
:
	TStringFace(parent, resourceId, name, SIZE_OF_TIME), zeros(true)
{
	SetValidator(new TTimeValidator(mandatory));
}

const CTime &TTimeFace::Time() const
{
	time.Clear();
	S();

	if (any(buffer))
	{
		// 2005:108 LPR: only parse hh:mm
		sscanf(buffer, "%d:%d", &time.hour, &time.minute);
	}

	return time;
}

void TTimeFace::SetTime(const CTime &tTime)
{
	time = tTime;

	// 2004:385 LPR: keep 00:00 for enabled; 2014:170 LPR: if zeros
	if (time.Empty() && (!IsWindowEnabled() || !zeros))
		*buffer = '\0';
	else
	{
		// hour and minute are required
		// 2005:108 LPR: only display hh:mm
		sprintf(buffer, "%0T", &time);
		//if (time.second)
		//	sprintf(s + strlen(s), ".%02d", time.second);
	}
	// 2004:363 LPR: transfer even if empty
	SetS(buffer);
}

void TTimeFace::EvChar(uint key, uint repeatCount, uint flags)
{
	if (key != ':' || !strchr(S(), ':'))
	{
		TStringFace::EvChar(key, repeatCount, flags);

		if (CHROME_AUTO_COMPLETE)
		{
			int len = strlen(S());

			// 2012:256 FIX: send ':' at end of text; send ':' after 2 digits
			if ((key == ' ' && (len == 1 || len == 2)) || (isdigit(key) && len == 2))
			{
				int tFlags = (flags << 16) + 1;

				SendMessage(WM_KEYDOWN, VK_END, tFlags);
				SendMessage(WM_KEYUP, VK_END, tFlags);
				SendMessage(WM_CHAR, ':', tFlags);
			}
		}
	}
}
