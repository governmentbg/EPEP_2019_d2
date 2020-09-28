#include "cac.h"

// 2007:010 rewritten, accepts any alphanumeric
bool bulstat_valid(const char *value)
{
	return !any(value) || (*value >= 'A' && *value <= 'Z') || (*value >= 'А' && *value <= 'Я') ||
		(*value >= '0' && *value <= '9');
}

bool connect_no_valid(char connectType, const char *value)
{
	if (strchr(CONNECT_TYPE_15_ANYS, connectType))	// including '\0'
	{
		// 2007:053 LRQ: 1st may be non-digit
		//return !any(value) || isdigit(*value);
		return *value != '-';
	}

	// 2008:288 LRQ
	if (strchr(CONNECT_TYPE_14_DIGITS, connectType) && strlen(value) == 14)
	{
		for (const char *s = value; *s; s++)
			if (!isdigit(*s))
				return false;

		long year = four_teen_year(value);
		if (year < YEAR_MIN || year > YEAR_MAX)
			return false;

		return four_teen_court(value) && four_teen_type(value) && four_teen_no(value);
	}

	// 2012:034
	if (connectType == CONNECT_TYPE_ELECTRIC_APFISH)
	{
		for (const char *s = value; *s; s++)
			if (!isalnum(*s) && (*s != '-' || s == value))	// 2012:069 digit -> alnum
				return false;

		return true;
	}

	// 2012:053
	if (connectType == CONNECT_TYPE_BOOK_JUDGEMENTS)
	{
		for (const char *s = value; *s; s++)
			if (!isalnum(*s))
				return false;

		return true;
	}

	return CONNECT_TYPE_EISPP_LAWSUIT || (strlen(value) <= LENGTH_OF_NO && long_valid(value));  // 2017:286 XNO->NO
}

bool country_valid(const char *value)
{
	return !any(value) || strlen(value) == 2;
}

bool date_valid(const char *value, sign mandatory)
{
	int day, month;
	long year;

	if (strlen(value) != LENGTH_OF_DATE)
		return !*value && mandatory != true;

	if (sscanf(value, "%2d.%2d.%4ld", &day, &month, &year) != 3)
		return false;

	CDate date(day, month, year);
	return mandatory == -1 ? date.ValidEx() && date <= Today : CDate(day, month, year).Valid();
}

bool eispp_nmr_valid(const char *value, char letter)
{
	if (!any(value))
		return true;

	if (strlen(value) != LENGTH_OF_EISPP_NMR)
		return false;

	msql m(EisDepart->Name);

	memcpy(EisDepart->tlacr, value, 3);
	EisDepart->tlacr[3] = '\0';
	m.Add(EisDepart, "F_TLACR");

	TQuery q(str(m));
	if (!q.Read())
		return false;

	if (!atoi(value + 6))
		return false;

	if (value[EISPP_LETTER_POS] < letter || value[EISPP_LETTER_POS] > letter + 1)
		return false;

	char s[3];

	calc_eispp(value, s);
	if (strcmp(value + LENGTH_OF_EISPP_MULTI, s))
		return false;

	return true;
}

static const char *find_replace_valid(const char *find, const char *replace)
{
	static constant text_too_common = "Текстът за търсене е прекалено общ.";
	int len = strlen(find);

	if (!len)
		return "Текстът за търсене е задължителен.";
	if (strlen(replace) > len)
		return "Текстът за замяна не може да е по-дълъг от този за търсене.";

	if (*find == '*' || find[len - 1] == '*')
		return "Текстът за търсене не може да започва с или да завършва на \"*\".";

	if (find[len - 1] == '#')
		return "Текстът за търсене не може да завършва на \"#\".";

	const char *s;
	int i;

	for (i = 0, s = find; *s; s++)
		if (!strchr("&$?*~ ", *s))
			i++;
	if (i < 2)
		return text_too_common;

	for (i = 1, s = find; (s = strchr(s, '*')) != NULL; s++)
	{
		int n;

		for (n = 10; s[1] == '*'; n += 10, s++)
			if (n == 50)
				return text_too_common;

		if ((i *= n) > FIND_CYCLES_MAX)
			return text_too_common;
	}

	return NULL;
}

bool find_replace_valid(const char *find, const char *replace, bool complain)
{
	const char *s = find_replace_valid(find, replace);
	return s ? complain ? error(s) : false : true;
}

bool firm_name_valid(const char *value)
{
	return !any(value) || isdigit(*value) || isupper(*value);
}

bool fzl_name_valid(sign cyrillic, const char *value)
{
	const char *s;
	int seps = 0;

	for (s = value; *s; s++)
	{
		if (*s == '-' || *s == ' ')
		{
			if (s == value || !isalpha(s[-1]) || !isalpha(s[1]))
				break;

			if (cyrillic && ++seps > (cyrillic == -1 ? 5 : 1))
				break;
		}
	}

	return !any(s);
}

bool time_valid(const char *value, bool mandatory)
{
	int hour, minute, second = 0;

	if (strlen(value) < 4)
		return *value ? false : !mandatory;

	const char *colon = strchr(value, ':');

	if (!colon || strlen(colon) != 3)
		return false;	

	// 2005:108 LPR: only hour and minute are valid
	//if (sscanf(value, "%2d:%2d.%2d", &hour, &minute, &second) < 2)
	if (sscanf(value, "%2d:%2d", &hour, &minute) != 2)
		return false;

	return CTime(hour, minute, second).Valid();
}

bool lnc_valid(const char *value)
{
	int sum = 0;
	char multi[9] = { 21, 19, 17, 13, 11, 9, 7, 3, 1 };

	if (!any(value))
		return true;

	// full officeal LNC description?		
	//if (*value != '1' && *value != '7')
	//	return false;

	for (int i = 0; i < 9; i++)
		sum += multi[i] * (value[i] - '0');

	return sum % 10 == value[9] - '0';
}

bool long_set_valid(const char *value)
{
	TIntegerArray lset;
	return !any(value) || strvalue_to_lset(value, &lset, NULL);
}

bool long_valid(const char *value)
{
	long num;
	int n;

	if (!any(value))
		return true;

	if (sscanf(value, "%ld%n", &num, &n) != 1 || n != strlen(value))
		return false;

	return true;
}

bool index_valid(const char *value, const char *kinds)
{
	return atol(value) <= strlen(kinds);
}

bool money_valid(const char *value, int len)
{
	double num;
	int n;
	const char *dp = strchr(value, '.');
	int valen = strlen(value);

	if (!valen)
		return true;

	if (dp)
	{
		if (dp - value > len - 3 || value + valen - dp > 3)
			return false;
	}
	else if (valen > len - 3)
		return false;

	if (sscanf(value, "%lf%n", &num, &n) != 1 || n != valen)
		return false;

	return true;
}

bool trans_id_valid(const char *value, bool partial)
{
	return !partial || !strstr(value, TRANS_ID_PENDING) || !strcmp(value, TRANS_ID_PENDING);
}

bool ucn_valid(char type, const char *value)
{
	if (strlen(value) != LENGTH_OF_UCN)
		return *value ? false : true;

	int sum = 0;

	switch (type)
	{
		case UCN_CITIZEN_UCN :
		{
			static char multi[9] = { 2, 4, 8, 5, 10, 9, 7, 3, 6 };

			for (int i = 0; i < 9; i++)
				sum += multi[i] * (value[i] - '0');

			sum = sum % 11 % 10;

			// 2012:283
			break;
		}
		case UCN_FIRM_TAX_NO :
		{
			static char multi[9] = { 4, 3, 2, 7, 6, 5, 4, 3, 2 };

			for (int i = 0; i < 9; i++)
				sum += multi[i] * (value[i] - '0');

			sum %= 11;
			if (sum == 1)
				return false;

			if (sum)
				sum = 11 - sum;

			break;
		}
		default:
			return true;
	}

	return (sum == value[9] - '0') ? true : false;
}

bool name_valid(const char *value)
{
	return !any(value) || (*value >= 'A' && *value <= 'Z') || (*value >= 'А' && *value <= 'Я');
}

bool year_valid(const char *value)
{
	return !any(value) || atol(value) >= YEAR_MIN && atol(value) <= YEAR_MAX;
}

bool keywords_valid(const char *value)
{
	char buffer[SIZE_OF_EXPANDED_KEYWORDS];

	strcpy(buffer, value);
	return kowpress(buffer);
}
