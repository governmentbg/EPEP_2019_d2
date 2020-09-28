#include "cac.h"

const
	NUMAP_MARK_EVEN	= 0,
	NUMAP_MARK_ODD	= 1,
	NUMAP_MARK_ALL	= 2;

class TNumapMarkGroup : public TDummyGroup
{
public:
	TNumapMarkGroup();

	long start;
	long final;
	long mark;
};

TNumapMarkGroup::TNumapMarkGroup()
{
	Add(
		new TLong("F_START", &start),
		new TLong("F_FINAL", &final),
		new TRadio("F_MARK_ALL", &mark, NUMAP_MARK_ALL),
		new TRadio("F_MARK_EVEN", &mark, NUMAP_MARK_EVEN),
		new TRadio("F_MARK_ODD", &mark, NUMAP_MARK_ODD),
		NULL
	);

	mark = NUMAP_MARK_ALL;
}

#include "GetNumapMarkDialog.h"
#include "GetNumapMarkDialog.cpp"

#include "NumapListFace.h"
#include "NumapListFace.cpp"

#include "NumapDialog.h"
#include "NumapDialog.cpp"

static bool add_street_map(TWindow *parent, TStreetMap *streetMap)
{
	TStreet street;
	street << streetMap;

	if (select_street(parent, &street))
	{
		*streetMap << street;

		if (streetMap->Insert())
		{
			TNumapDialog(parent, streetMap).Execute();
			return true;
		}
	}

	return false;
}

#include "PrintMap.cpp"

#include "StreetMapListBox.h"
#include "StreetMapListBox.cpp"

#include "GetStreetMapDialog.h"
#include "GetStreetMapDialog.cpp"

#if TESTVER
// 2018-09-25: LPR/LRQ: copy full user map -> selection only
static bool combine_street_map(const TStreetMap *source, TStreetMap *target)
{
	bool changed = false;

	for (int k = 0; k < source->map.len(); k++)
	{
		if (source->map[k] == NUMAP_ACTIVE)
		{
			if (target->map.len() < k + 1)
				target->map.setlen(k + 1);

			if (target->map[k] != NUMAP_ACTIVE)
			{
				ncstr(target->map)[k] = NUMAP_ACTIVE;
				changed = true;
			}
		}
	}

	return changed;
}

static bool copy_street_map(TWindow *parent, TUser *user, TGArray<TStreetMap> *array)
{
	TGArray<TStreetMap> select;
	bool changed = false;

	if (TGetStreetMapDialog(parent, &select, user->ucn).Execute() == IDOK)
	{
		for (int i = 0; i < select.Count(); i++)
		{
			const TStreetMap *source = select[i];
			int n;

			for (n = 0; n < array->Count(); n++)
			{
				TStreetMap *target = (*array)[n];

				if (source->uclp == target->uclp && source->street == target->street)
				{
					if (combine_street_map(source, target) && target->Update())
						changed = true;

					break;
				}
			}

			if (n == array->Count())
			{
				TStreetMap target;

				target << source;
				strcpy(target.ucn, user->ucn);

				if (target.Insert())
					changed = true;
			}
		}
	}

	return changed;
}
#else  // TESTVER
static bool copy_street_map(TWindow *parent, TUser *target, TGArray<TStreetMap> *array)
{
	TUser source;

	if (!select_user(parent, C2S[POST_DELIVERER], FLAG_ACTIVE | USER_NOLIMIT, &source, target->ucn))
		return false;

	msql m;
	TStreetMap sourceMap;

	m.Begin("T_STREET_MAP");
	m.AddString("F_UCN", source.ucn);
	TQuery q(str(m));

	while (q.Read())
	{
		int i;

		sourceMap << q;

		for (i = 0; i < array->Count(); i++)
			if ((*array)[i]->uclp == sourceMap.uclp && (*array)[i]->street == sourceMap.street)
				break;

		if (i < array->Count())
		{
			TStreetMap *targetMap = (*array)[i];

			for (int i = 0; i < sourceMap.map.len(); i++)
			{
				if (sourceMap.map[i] == NUMAP_ACTIVE)
				{
					if (targetMap->map.len() < i + 1)
						targetMap->map.setlen(i + 1);

					ncstr(targetMap->map)[i] = NUMAP_ACTIVE;
				}
			}

			targetMap->Update();
		}
		else
		{
			TStreetMap targetMap;

			targetMap << sourceMap;
			strcpy(targetMap.ucn, target->ucn);
			targetMap.Insert();
		}
	}

	return true;
}
#endif  // TESTVER

#include "StreetMapListWindow.h"
#include "StreetMapListWindow.cpp"

#include "StreetMapListDialog.h"
#include "StreetMapListDialog.cpp"

void map_user(TWindow *parent, TUser *user)
{
	TStreetMapListDialog(parent, user).Execute();
}
