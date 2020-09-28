#include "cac.h"

// ----- TTable --------------------------------------------------------------
TTable::TTable(const char *name)
	: TVObject(name), descs(NULL), DataBase(dataBase), dataBase(TCACDataBase::DefaultDB)
{
}

TIArray<FLDDesc> TTable::Descs[TABLE_COUNT];

void TTable::GetDescs() const
{
	if (!descs)
		fatal("%s: not a table", Name);

	if (!descs->Count() && !dataBase->GetDescs(Name, descs))
		fatal("%s: %s", Name, descs->Count() ? "no primary key" : "no fields");
}
