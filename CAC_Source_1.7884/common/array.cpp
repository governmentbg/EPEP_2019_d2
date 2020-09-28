#include "common.h"

// ----- TIntegerArray -------------------------------------------------------
static bool lesseqli(const intptr_t value1, const intptr_t value2)
{
	return value1 <= value2;
}

TIntegerArray::TIntegerArray(bool sorted)
	: lesseql(sorted ? lesseqli : NULL)
{
	mbk_init(this);
}

#if RANGECHECK
intptr_t TIntegerArray::ItemAt(int index) const
{
	if (index < 0 || (size_t) index >= size / LP_SIZE)
		fatal("TIntegerArray::ItemAt: range check");

	return ((intptr_t *) data)[index];
}
#endif  // RANGECHECK

int TIntegerArray::Add(const intptr_t value)
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

void TIntegerArray::Add(const intptr_t value1, const intptr_t value2, ...)
{
	Add(value1);

	if (value2 != INT_MAX)
	{
		va_list ap;
		intptr_t value;

		Add(value2);

		va_start(ap, value2);
		while ((value = va_arg(ap, intptr_t)) != INT_MAX)
			Add(value);
		va_end(ap);
	}
}

intptr_t TIntegerArray::Detach(int index)
{
	intptr_t value = ItemAt(index);

	mbk_remove(this, index * LP_SIZE, LP_SIZE);
	return value;
}

int TIntegerArray::Find(const intptr_t value) const
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

void TIntegerArray::Insert(const intptr_t value, int index)
{
	mbk_insert(this, &value, index * LP_SIZE, LP_SIZE);
}

TIntegerArray::TIntegerArray(const TIntegerArray &)
{
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
