#include "cac.h"

TGroupListColumn::TGroupListColumn(const char *tTitle, const char *name, int tWidth, int tFormat)
	: TObject(name), title(tTitle), width(tWidth), format(tFormat), print(NULL)
{
	Init();

}

TGroupListColumn::TGroupListColumn(const char *tTitle, void (*tPrint)(mstr &m, TGroup *group), int tWidth, int tFormat)
	: TObject("F_LIST_COLUMN"), title(tTitle), width(tWidth), format(tFormat), print(tPrint)
{
	Init();
}

void TGroupListColumn::Init()
{
	search = !(format & LVCFMT_NO_SEARCH);
	format &= ~LVCFMT_NO_SEARCH;
	resize = width > 12;
}

void TGroupListColumn::Print(mstr &m, TGroup *group)
{
	if (print)
		print(m, group);
	else
	{
	#if RANGECHECK
		TField *field = group->Find(Name);

		if (!field)
			fatal("%s: lacks column %s", group->Name, Name);

		field->Print(m);
	#else  // RANGECHECK
		group->Find(Name)->Print(m);
	#endif  // RANGECHECK
	}

	m.cut(0xFF);
}
