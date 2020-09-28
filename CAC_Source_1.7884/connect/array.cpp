#include "cac.h"

// ----- TIntegerArray -------------------------------------------------------
static bool lesseqli(const long value1, const long value2)
{
	return value1 <= value2;
}

TIntegerArray::TIntegerArray(bool sorted)
	: lesseql(sorted ? lesseqli : NULL)
{
	mbk_init(this);
}

#if RANGECHECK
long TIntegerArray::ItemAt(int index) const
{
	if (index < 0 || index >= size / LP_SIZE)
		fatal("TIntegerArray::ItemAt: range check");

	return ((long *) data)[index];
}
#endif  // RANGECHECK

int TIntegerArray::Add(const long value)
{
	int l;

	if (lesseql)
	{
		int h = Count() - 1;

		l = 0;
		while (l < h)
		{
			int i = (l + h) / 2;

			if (lesseql(ItemAt(i), value))
				l = i + 1;
			else
				h = i;
		}

#if RANGECHECK
		if (l < Count())
		{
			bool tLessEq = lesseql(ItemAt(l), value);

			if (tLessEq)
				l++;
		}
#else  // RANGECHECK
		if (l < Count() && lesseql(ItemAt(l), value))
			l++;
#endif  // RANGECHECK
		Insert(value, l);
	}
	else
	{
		l = Count();
		mbk_append(this, &value, LP_SIZE);
	}

	return l;
}

void TIntegerArray::Add(const long value1, const long value2, ...)
{
	Add(value1);

	if (value2 != INT_MAX)
	{
		va_list ap;
		long value;

		Add(value2);

		va_start(ap, value2);
		while ((value = va_arg(ap, long)) != INT_MAX)
			Add(value);
		va_end(ap);
	}
}

long TIntegerArray::Detach(int index)
{
	long value = ItemAt(index);
	mbk_remove(this, index * LP_SIZE, LP_SIZE);
	return value;
}

int TIntegerArray::Find(const long value) const
{
	if (lesseql)
	{
		int l = 0, h = Count() - 1;

		while (l <= h)
		{
			int i = (l + h) / 2;

			if (lesseql(ItemAt(i), value))
			{
				if (lesseql(value, ItemAt(i)))
					return i;

				l = i + 1;
			}
			else
				h = i - 1;
		}
	}
	else
	{
		for (int i = 0; i < Count(); i++)
			if (ItemAt(i) == value)
				return i;
	}

	return INT_MAX;
}

void TIntegerArray::Insert(const long value, int index)
{
	mbk_insert(this, &value, index * LP_SIZE, LP_SIZE);
}

// ----- TPointerArray -------------------------------------------------------
TPointerArray::TPointerArray(void (*tDelete)(void *tData))
	: destroy(tDelete)
{
}

TPointerArray::~TPointerArray()
{
	if (destroy)
		for (int i = 0; i < Count(); i++)
			destroy(ItemAt(i));
}

void TPointerArray::Add(const void *data1, const void *data2, va_list ap)
{
	Add(data1);

	while (data2)
	{
		Add(data2);
		data2 = va_arg(ap, const void *);
	}
}

void TPointerArray::Flush()
{
#if RANGECHECK
	if (!destroy)
		fatal("TPointerArray::Flush: NULL destroy");
#endif  // RANGECHECK
	for (int i = 0; i < Count(); i++)
		destroy(ItemAt(i));

	mbk_clear(this);
}

void TPointerArray::Remove(int index)
{
#if RANGECHECK
	if (!destroy)
		fatal("TPointerArray::Remove: NULL destroy");
#endif  // RANGECHECK
	destroy(ItemAt(index));
	Detach(index);
}

// ----- TStringArray --------------------------------------------------------
static bool strlesseqli(const long value1, const long value2)
{
	return strcmpi((const char *) value1, (const char *) value2) <= 0; 
}

TStringArray::TStringArray(bool sorted)
	: TPointerArray(DeleteString)
{
	if (sorted)
		lesseql = strlesseqli;
}

void TStringArray::DeleteString(void *s)
{
	delete[] (char *) s;
}
