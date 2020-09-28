#include "cac.h"

constant
	MQ_INSERT = "INSERT INTO %s",
	MQ_UPDATE = "UPDATE %s SET ",
	MQ_DELETE = "DELETE FROM %s",
	MQ_SUBSEQ = "SELECT";

msql::msql()
	: isFirst(false), nextAnd(true)
{
}

msql::msql(const char *name, const char *fields)
{
	Begin(name, fields);
}

void msql::Add(const char *name)
{
	printf(" %s ", isFirst ? "WHERE" : nextAnd ? "AND" : "OR");
	if (any(name))
		printf("%s ", name);
	isFirst = false;
	nextAnd = true;
}

void msql::Add(TField *field, const char *dbName)
{
	Add(dbName ? dbName : field->Name);
	AddValue(field);
}

void msql::Add(const TGroup *group, const char *fields)
{
	TIArray<FLDDesc> *tDescs = group->Parse(fields);

	for (int i = 0; i < tDescs->Count(); i++)
		Add(group->Find((*tDescs)[i]->szName));

	group->Clear(tDescs);
}

void msql::AddAmPm(const char *name, const CTime &time, int dayType)
{
	if (dayType != DAY_WHOLE)
	{
		Add(name);
		printf("%s %ld", dayType == DAY_AFTERNOON ? ">=" : "<", time.Value());
	}
}

void msql::AddArray(const char *name, const TGroupArray &array, const char *dbName)
{
	if (!dbName)
	{
		dbName = name;
		name = strstr(name, "F_");	// 2015:169 LPR: skip X.
	#if RANGECHECK
		if (!name)
			fatal("msql: %s: no F_", dbName);
	#endif  // RANGECHECK
	}

	int hasNull = array.Count() && array[0]->Find(name)->IsNULL();
	int nonNull = array.Count() - hasNull;

	if (hasNull)
	{
		Add("");

		if (nonNull)
			cat("(");

		printf("%s IS NULL", dbName);

		if (nonNull)
			NextOr();
	}

	if (nonNull)
	{
		Add(dbName);

		if (nonNull == 1)
			AddValue(array[hasNull]->Find(name));
		else
		{
			cat("IN (");

			for (int i = hasNull; i < array.Count(); i++)
			{
				if (i > hasNull)
					cat(", ");

				array[i]->Find(name)->Quote(*this);
			}

			cat(")");
		}

		if (hasNull)
			cat(")");
	}
}

void msql::AddChar(const char *name, char c)
{
	Add(name);

	if (c)
		printf("= '%c'", c);
	else
		cat("IS NULL");
}

void msql::AddChars(const char *name, const char *chars)
{
	if (any(chars))
	{
		Add(name);
		catset(chars);
	}
}

void msql::AddChars(const char *name, const char *chars, const char *alls)
{
	if (!strequal(chars, alls))
		AddChars(name, chars);
}

void msql::AddCompos(const char *name, const char *compos)
{
	if (any(compos) && !strequal(compos, COMPOSITION_ALLS))
	{
		Add(name);

		if (strlen(compos) == 1)
			printf("= %ld", (long) *compos);
		else
		{
			cat("IN (");

			for (int i = 0; compos[i]; i++)
			{
				if (i)
					cat(", ");
				printf("%ld", (long) compos[i]);
			}

			cach(')');
		}
	}
}

void msql::AddDate(const char *name, const CDate &date)
{
	AddLong(name, date.Empty() ? 0 : date.Value());
}

void msql::AddExtra(const char *name, const char *text, const char *extra)
{
	if (any(extra))
		AddLike(name, text, false);
	else if (any(text))
		AddString(name, text);
}

void msql::AddFlags(const char *name, long mask, long value, long all)
{
	if (mask)
	{
		Add("");

		// 2008:070
		long count = 0, inset = 0;

		for (long i = 0; i <= all; i++)
		{
			if ((i & all) == i && (value == -1 ? (i & mask) : (i & mask) == value))
				count++;
		}

		if (!mask || !count)
			fatal("AddFlags: bad %s:%ld/%ld/%ld", name, mask, value, all);

		if (count > 1 && !value)
			cach('(');
		cat(name);
		if (!value)
		{
			cat(" IS NULL");
			if (count > 1)
				printf(" OR %s", name);
			count--;
		}

		for (long i = 1; i <= all; i++)
		{
			if ((i & all) == i && (value == -1 ? (i & all & mask) : (i & all & mask) == value))
				printf(count == 1 ? " = %ld" : inset++ ? ", %ld" : " IN (%ld", i);
		}

		if (count > 1)
			cach(')');
		if (count > 0 && !value)
			cach(')');
	}
}

void msql::AddIsNull(const char *name, bool isNull)
{
	Add(isNull ? "" : "NOT");
	printf("%s IS NULL", name);
}

void msql::AddJuSet(constant dbNames[], const TGroupArray &array, const char *grName, bool nonNull)
{
	if (array.Count())
	{
		mstr w("(");

		w.cat(dbNames[0]);
		AddArray(grName, array, w.str());

		for (int i = 1; dbNames[i]; i++)
		{
			NextOr();
			AddArray(grName, array, dbNames[i]);
		}

		cat(")");
	}
	else if (nonNull)
		AddNnSet(dbNames);
}

void msql::AddLike(const char *name, const char *text, bool full)
{
	if (any(text))
	{
		Add(name);
		printf("LIKE '%s", full ? "%" : "");
		cat_quot(text);
		cat("%'");
	}
}

void msql::AddILike1(const char *name, const char *text, bool full)
{
	AddLike(name, text, full);
}

void msql::AddLong(const char *name, long l)
{
	if (l)
		AddZLong(name, l);
	else
		AddIsNull(name, true);
}

void msql::AddLongs(const char *name, const TIntegerArray &array)
{
	if (array.Count())
	{
		Add(name);

		if (array.Count() == 1)
			printf("= %ld", array[0]);
		else
		{
			cat("IN (");

			for (int i = 0; i < array.Count(); i++)
			{
				if (i)
					cat(", ");
				printf("%ld", array[i]);
			}

			cach(')');
		}
	}
}

void msql::AddNnSet(constant dbNames[])
{
	Add("(NOT");
	printf("%s IS NULL", dbNames[0]);

	for (int i = 1; dbNames[i]; i++)
	{
		NextOr();
		Add("NOT");
		printf("%s IS NULL", dbNames[i]);
	}

	cat(")");
}

void msql::AddNonElectron(const char *prefix)
{
	mstr w(prefix);
	w.cat("F_BY_POST");


	// 2013:107 FIX: reject emailed subs (p 19.2); 2014:331 +FAX
	AddFlags(w.str(), OUTREG_ELECTRON, 0, OUTREG_POSTAL);
	w.cat("_X");
	AddFlags(w.str(), OUTREG_X_ELECTRON, 0, OUTREG_X_ALL);
}

void msql::AddOrder(const char *order, const char *prefix)
{
	cat(" ORDER BY ");

	if (prefix)
	{
		const char *s;

		while ((s = strstr(order, "F_")) != NULL)
		{
			catn(order, s - order);
			cat(prefix);
			cat("F_");
			order = s + 2;
		}
	}

	cat(order);
}

void msql::AddPairs(const char *grName1, const char *grName2, const TGroupArray &array, const char *dbName1,
	const char *dbName2, const char *prefix)
{
	if (array.Count())
	{
		Add("");

		if (prefix)
			cat(prefix);

		if (array.Count() > 1)
			cat("(");

		for (int i = 0; i < array.Count(); i++)
		{
			TGroup *group = array[i];

			if (i)
			{
				NextOr();
				Add("");
			}

			printf("(%s ", dbName1 ? dbName1 : grName1);
			AddValue(group->Find(grName1));
			Add(group->Find(grName2), dbName2);
			cat(")");
		}

		if (array.Count() > 1)
			cat(")");
	}
}

void msql::AddQuick(const char *name, long quick)
{
	if (quick != QUICK_ANY_SPEED)
		AddFlags(name, QUICK_QUICK | QUICK_IMMEDIATE, quick, QUICK_ALL);
}

void msql::AddRange(const char *name, long min, long max)
{
	if (min || max)
	{
		Add(name);

		if (min)
		{
			if (min == max)
				printf("= %ld", min);
			else if (max)
				printf("BETWEEN %ld AND %ld", min, max);
			else
				printf(">= %ld", min);
		}
		else
			printf("<= %ld", max);
	}
}

void msql::AddRange(const char *name, const CDate &min, const CDate &max)
{
	AddRange(name, min.Empty() ? 0 : min.Value(), max.Empty() ? 0 : max.Value());
}

void msql::AddRange(const char *name, const CTime &min, const CTime &max)
{
	AddRange(name, min.Empty() ? 0 : min.Value(), max.Empty() ? 0 : max.Value());
}

void msql::AddRange(const char *name, const CDateTime &min, const CDateTime &max)
{
	if (!min.Empty() || !max.Empty())
	{
		Add(name);

		if (!min.Empty())
		{
			if (min == max)
				min.Print(*this, "= '%d.%d.%ld %d:%d:%d'");
			else if (!max.Empty())
			{
				min.Print(*this, "BETWEEN '%d.%d.%ld %d:%d:%d' ");
				max.Print(*this, "AND '%d.%d.%ld %d:%d:%d'");
			}
			else
				min.Print(*this, ">= '%d.%d.%ld %d:%d:%d'");
		}
		else
			max.Print(*this, "<= '%d.%d.%ld %d:%d:%d'");
	}
}

void msql::AddString(const char *name, const char *s)
{
	Add(name);

	if (any(s))
		cat_quoted("= ", s);
	else
		cat("IS NULL");
}

void msql::AddTriad(const char *dbName1, const char *dbName2, const char *dbName3, const TGroupArray &array,
	const char *grName, bool nonNull)
{
	if (array.Count())
	{
		mstr w("(");

		w.cat(dbName1);
		AddArray(grName, array, w.str());
		NextOr();
		AddArray(grName, array, dbName2);
		NextOr();
		AddArray(grName, array, dbName3);
		cat(")");
	}
	else if (nonNull)
	{
		Add("(NOT");
		printf("%s IS NULL OR NOT %s IS NULL OR NOT %s IS NULL)", dbName1, dbName2, dbName3);
	}
}

void msql::AddValue(TField *field)
{
	cat(field->IsNULL() ? "IS " : "= ");
	field->Quote(*this);
}

void msql::AddWords(const char *name, const char *keyWords)
{
	const char *s;
	// note: keyWords must be kowpressed
	while ((s = strchr(keyWords + 1, ';')) != NULL)
	{
		char buffer[SIZE_OF_KEYWORDS];
		size_t len = s - keyWords + 1;

		memcpy(buffer, keyWords, len);
		buffer[len] = '\0';
		AddLike(name, buffer, true);
		keyWords = s;
	}
}

void msql::AddXferKinds(const char *name, const char *kinds)
{
	if (any(kinds))
	{
		Add(name);

		if (strlen(kinds) == 1)
			printf("= %ld", Kind->Xfer(*kinds));
		else
		{
			cat("IN (");
			for (const char *k = kinds; *k; k++)
			{
				if (k > kinds)
					cat(", ");
				printf("%ld", Kind->Xfer(*k));
			}
			cach(')');
		}
	}
}

void msql::AddZLong(const char *name, long l)
{
	Add(name);
	printf("= %ld", l);
}

void msql::Begin(const char *name, const char *fields)
{
	if (fields != MQ_SUBSEQ)
	{
		isFirst = nextAnd = true;
		clear();

		if (fields == MQ_INSERT || fields == MQ_UPDATE || fields == MQ_DELETE)
			printf(fields, name);
		else
			printf("SELECT %s FROM %s", fields ? fields : "*", name);
	}
#if RANGECHECK
	else
	{
		if (!len())
			fatal("msql SUBSEQ of empty statement");

		if (!isFirst)
			fatal("msql SUBSEQ of non-empty criteria");
	}
#endif  // RANGECHECK	
}

void msql::CutOrder()
{
	const char *order = strstr(str(), " ORDER BY ");

	if (order)
		cut(order - str());
}
