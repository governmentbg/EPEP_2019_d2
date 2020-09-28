#include "cac.h"

class TJoinStreet : public TStreet
{
public:
	char prefix[10];
	char just_name[SIZE_OF_STREET_NAME];

	void Revised();
	bool operator<=(const TJoinStreet &that) const;
};

void TJoinStreet::Revised()
{
	char *ch;
	int len;
	strcpy(prefix, "");
	strcpy(just_name, name);

	for (constant *pref = FILTER_STREET_PREFIXES; *pref; pref++)
	{
		len = strlen(*pref);
		if (!strncmp(just_name, *pref, len)  && !isalnum(just_name[len]))
		{
			char *s = just_name + len;

			strcpy(prefix, *pref);
			strmove(just_name, s);
			break;
		}
	}

	for (ch = just_name; *ch; ch++)
		if (ispunct(*ch))
			*ch = ' ';
	strimall(just_name);
	compress(just_name);
}

bool TJoinStreet::operator<=(const TJoinStreet &that) const
{
	return strcmp(name, that.name) <= 0;
}

#include "JoinStreetsListWindow.h"
#include "JoinStreetsListWindow.cpp"

#include "JoinStreetsDialog.h"
#include "JoinStreetsDialog.cpp"

static void join_equal_streets(TWindow *parent, long uclp, bool execute)
{
	THTMLFile h;
	TGSArray<TJoinStreet> tSources;
	TGSArray<TJoinStreet> tTargets;
	TGSArray<TJoinStreet> *sources = &tSources;	// !!TMP!!
	TGSArray<TJoinStreet> *targets = &tTargets;	// !!TMP!!
	bool exist;
	int count, count_join, count_err_join, count_stay;

	count = count_join = count_err_join = count_stay = 0;

	h.OpenTemp();
	TWaitWindow wait(parent, WAIT);
	msql m("T_STREET");
	m.AddLong("F_UCLP", uclp);

	TQuery q(str(m));
	while (q.Read())
	{
		TJoinStreet *street = new TJoinStreet;

		*street << q;
		street->Revised();

		if (street->street > 0)
			targets->Add(street);
		else
			sources->Add(street);
	}

	h.printf("Присъединяване на еднакви улици - %s\n\n", UCLP->Seek(uclp), sources->Count(), targets->Count());

	for (int i = 0; i < sources->Count(); i++)
	{
		h.printf("%ld \"%s\" --> ", (*sources)[i]->street, (*sources)[i]->name);
		exist = false;

		for(int j = 0; j < targets->Count(); j++)
		{
			if (!strcmp((*sources)[i]->just_name, (*targets)[j]->just_name)
				&& (!strncmp((*sources)[i]->prefix, (*targets)[j]->prefix, 2)
				|| !any((*sources)[i]->prefix) || !any((*targets)[j]->prefix)))
			{
				if (execute)
				{
					if (join((*sources)[i], (*targets)[j]))
					{
						h.printf("%ld \"%s\"\n", (*targets)[j]->street, (*targets)[j]->name);
						sources->Remove(i);
						i--;
						count_join++;
					}
					else
					{
						h.printf("грешка при присъединяване\n");
						count_err_join++;
					}
				}
				else
					h.printf("%ld \"%s\"\n", (*targets)[j]->street, (*targets)[j]->name);

				exist = true;
				break;
			}
		}
		if (!exist)
		{
			h.printf("няма подходяща улица за присъединяване\n");
			count_stay++;
		}

		wait.TickCount(++count, "проверени улици");
	}

	h.printf("\nПроверени: %d\nПрисъединиени: %d\nОстанали: %d\nГрешки: %d",
		count, count_join, count_stay, count_err_join);
	h.Show(&wait);

	if (execute)
		TJoinStreetsDialog(parent, sources, targets).Execute();
}

#include "LaunchJoinStreetsDialog.h"
#include "LaunchJoinStreetsDialog.cpp"

void join_equal_streets(TWindow *parent)
{
	TLaunchJoinStreetsDialog(parent).Execute();
}
