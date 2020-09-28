#include "cac.h"

enum
{
	NOTIFY_SELECT_SINGLE_LINK,	// single by itself or via link
	NOTIFY_SELECT_LAWER_SINGLE,	// single lawer / represented
	NOTIFY_SELECT_LAWER_GROUP,	// multi under the same lawer
	NOTIFY_SELECT_LEGAL_AID,	// multi same category no represent
	NOTIFY_SELECT_EXEC_LIST		// multi with all possible links
};

static constant LinkLabels[] = { "Начин на &уведомяване", "&Лице и начин на уведомяване", "&Лица и начини на уведомяване",
	"&Лица за правна помощ", "&Лица по изпълнителен лист" };

// ----- TSubpoenaVariantGroup -----------------------------------------------
TSubpoenaVariantGroup::TSubpoenaVariantGroup(TSideWind *tSideWind, TGArray<TLinkWind> *tAllLinks,
	TGArray<TSideWind> *tAllSides, TGArray<TUCNGroup> *tAffecteds, TRCDKeyContainer *container, const TLawsuit *tLawsuit)
:
	sideWind(tSideWind), allLinks(tAllLinks), allSides(tAllSides), affecteds(tAffecteds),
		categories(new char[tAllSides->Count()]), execList(NULL), category(container->Category(tSideWind->involvement)),
		lawer(strchr(SUMMONS_FOR_SIDE_INDIRECT_INVS, tSideWind->involvement)),
		crime(find_college(container) == TYPE_PUNISHMENT_LAW), lawsuit(tLawsuit)
{
	Add(
		new TListAlias("F_SUBPOENA_KIND", &subpoenaKind),
		new TRadio("F_FLAGS_NONE", &flags, OUTREG_NONE),
		new TRadio("F_FLAGS_REGULAR", &flags, OUTREG_REGULAR),
		new TRadio("F_FLAGS_R", &flags, OUTREG_R),
		new TRadio("F_FLAGS_AR", &flags, OUTREG_AR),
		new TRadio("F_FLAGS_PACKAGE", &flags, OUTREG_PACKAGE),
		new TCheck("F_FLAGS_COURIER", &flagsX, OUTREG_X_COURIER),
		new TCheck("F_FLAGS_FAX", &flagsX, OUTREG_X_FAX),
		new TCheck("F_FLAGS_EMAIL", &flagsX, OUTREG_X_EMAIL),
		new TDate("F_VALID_DATE", &validDate, false),
		new TDate("F_SUBPOENA_DATE", &subpoenaDate, true),
		NULL
	);

	// 2014:343 FIX: categories are always needed
	//if (strchr(SUBPOENA_KIND_LEGAL_AIDS, *sk) || strchr(SUBPOENA_KIND_EXEC_LISTS, *sk))
	// 2015:076 former Classify()
	char opposite = category == SIDEWIND_LEFT ? SIDEWIND_RIGHT : category == SIDEWIND_RIGHT ? SIDEWIND_LEFT : '\0';

	for (int i = 0; i < allSides->Count(); i++)
	{
		TSideWind *tSideWind = (*allSides)[i];
		char tCategory = categories[i] = container->Category(tSideWind->involvement);

		if (tCategory == category)		// may be 3rd
			sameSides.Add(tSideWind);
		else if (tCategory == opposite)
			opposites.Add(tSideWind);

		if (tCategory == SIDEWIND_THIRD)
			thirdSides.Add(tSideWind);
	}
}

TSubpoenaVariantGroup::~TSubpoenaVariantGroup()
{
	sameSides.Clear();
	opposites.Clear();
	thirdSides.Clear();
	delete[] categories;
	delete execList;
}

void TSubpoenaVariantGroup::OppositeLawers(mstr &m)
{
	for (int i = 0; i < opposites.Count(); i++)
	{
		for (int n = 0; n < thirdSides.Count(); n++)
		{
			TSideWind *sideWind = thirdSides[n];

			if (sideWind->Represents(opposites[i], allLinks))
			{
				add_side_name(m, sideWind);
				break;
			}
		}
	}

	m.cutell(LENGTH_OF_LEGAL_AID_TEXT);
}

void TSubpoenaVariantGroup::Affected(TGArray<TUCNGroup> *affecteds, const TBaseWind *baseWind)
{
	for (int i = 0; i < affecteds->Count(); i++)
		if ((*affecteds)[i]->ucnType == baseWind->ucnType && !strcmp((*affecteds)[i]->ucn, baseWind->ucn))
			return;

	TFoundGroup *ucnGroup = new TFoundGroup;
	*ucnGroup << baseWind;
	affecteds->Add(ucnGroup);
}

// from _ so don't ask
static void cleanup_specific_links(TGArray<TLinkWind> *tLinks)
{
	int i = 0;

	while (i < tLinks->Count())
	{
		TLinkWind *linkWind = (*tLinks)[i];

		if ((linkWind->proxyKind != SIDE_MARK_CONSENT_OF_INDIRECT || linkWind->Redirected()) && linkWind->Proxied())
			tLinks->Detach(i);
		else
			i++;
	}
}

void TSubpoenaVariantGroup::FillLinks(TGArray<TLinkWind> *tLinks, bool allowLinks)
{
	tLinks->Flush();

	if (strchr(SUBPOENA_KIND_LEGAL_AIDS, subpoenaKind))
		notifySelect = NOTIFY_SELECT_LEGAL_AID;
	else if (strchr(SUBPOENA_KIND_EXEC_LISTS, subpoenaKind))
		notifySelect = NOTIFY_SELECT_EXEC_LIST;
	else if (lawer)
	{
		SubpoenaKind->Seek(subpoenaKind);
		notifySelect = crime || !SubpoenaKind->LawerGroup() ? NOTIFY_SELECT_LAWER_SINGLE : NOTIFY_SELECT_LAWER_GROUP;
	}
	else	// default
		notifySelect = NOTIFY_SELECT_SINGLE_LINK;

	if (notifySelect == NOTIFY_SELECT_LEGAL_AID || notifySelect == NOTIFY_SELECT_EXEC_LIST)
	{
		bool involvement = notifySelect == NOTIFY_SELECT_EXEC_LIST && category == SIDEWIND_THIRD;

		for (int i = 0; i < sameSides.Count(); i++)
		{
			TSideWind *tSideWind = sameSides[i];
			TLinkWind *tLinkWind = new TLinkWind;
			int index = -1;

			*tLinkWind << tSideWind;

			if (tSideWind == sideWind)
				index = 0;
			else if ((subpoenaKind != SUBPOENA_KIND_LEGAL_AID_TYPED || strchr(UCN_CITIZENS, tSideWind->ucnType)) &&
				(!involvement || tSideWind->involvement == sideWind->involvement))

			{
				index = tLinks->Count();
			}

			if (index >= 0)
			{
				tLinks->Insert(tLinkWind, index);

				if (notifySelect == NOTIFY_SELECT_EXEC_LIST && allowLinks &&
					subpoenaKind != SUBPOENA_KIND_EXEC_COUNTRY)	// 2015:077 IRQ: not for country
				{
					for (int i = 0; i < allLinks->Count(); i++)
						if ((*allLinks)[i]->SameWind(tSideWind))
							tLinks->InsertCopy((*allLinks)[i], ++index);
				}
			}
			else
				delete tLinkWind;
		}
	}
	else if (allowLinks)
	{
		if (lawer)
		{
			for (int i = 0; i < allLinks->Count(); i++)
				if (sideWind->Represents((*allLinks)[i]))
					tLinks->AddCopy((*allLinks)[i]);
		}
		else
		{
			for (int i = 0; i < allLinks->Count(); i++)
				if ((*allLinks)[i]->SameWind(sideWind))
					tLinks->AddCopy((*allLinks)[i]);
		}
	}

	if (!SubpoenaKind->LinksAllowed())
		cleanup_specific_links(&links);

	if ((notifySelect == NOTIFY_SELECT_LAWER_SINGLE || notifySelect == NOTIFY_SELECT_SINGLE_LINK) &&
		tLinks->Count())
	{
		TLinkWind *tLinkWind = new TLinkWind;
		*tLinkWind << sideWind;
		tLinks->Insert(tLinkWind, 0);
	}
}

#include "SelectLinkedWindsListBox.h"
#include "SelectLinkedWindsListBox.cpp"

#include "PostalDialog.h"
#include "PostalDialog.cpp"

#include "SubpoenaVariantDialog.h"
#include "SubpoenaVariantDialog.cpp"

class TDoubleSubpoena : public TSubpoena
{
public:
	TDoubleSubpoena(TSubpoena *subpoena);

	bool operator<=(TDoubleSubpoena &that) const;

	char address1[0x100];
	char *address2;
	char result1[SIZE_OF_LONGER_NAME];
	char *result2;
	char proxy1[0x100];
	char *proxy2;
};

TDoubleSubpoena::TDoubleSubpoena(TSubpoena *subpoena)
{
	*this << subpoena;

	PrintAddress(address1, ADDRESS_SKIP_LN1);
	address2 = split_half(address1);
	strcpy(result1, rrseek(subpoena->key.type, subpoena->eventStatus));
	result2 = split_half(result1);

	if (subpoena->ProxyPresent())
		subpoena->PrintProxy(proxy1, sizeof proxy1);
	else
		*proxy1 = '\0';

	proxy2 = split_half(proxy1);
}

bool TDoubleSubpoena::operator<=(TDoubleSubpoena &that) const
{
	return eventStatus != that.eventStatus ? eventStatus < that.eventStatus : subpoenaNo <= that.subpoenaNo;
}

#define subpoena OFFICE_GROUP_CAST(TDoubleSubpoena)

static void subpoena_address(mstr &m, TGroup *group) { m.cpy(subpoena->address1); }
static void subpoena_result(mstr &m, TGroup *group) { m.cpy(subpoena->result1); }
static void subpoena_official(mstr &m, TGroup *group) { m.cpy(subpoena->OfficiallyNotified() ? "Да" : ""); }
static void subpoena_proxy(mstr &m, TGroup *group) { m.cpy(subpoena->proxy1); }

#undef subpoena

#include "SubpoenaListWindow.h"
#include "SubpoenaListWindow.cpp"

#include "AnnounceNumberDialog.h"
#include "AnnounceNumberDialog.cpp"

#include "SubpoenaDialog.h"
#include "SubpoenaDialog.cpp"

void process_subpoenas(TWindow *parent, TRCDKeyContainer *container, char status, TSideWind *sideWind, int sideIndex,
	TGArray<TLinkWind> *links, TGArray<TSideWind> *allSides, TGArray<TUCNGroup> *affecteds, const TLawsuit *lawsuit)
{
	TSubpoenaDialog(parent, container, status, sideWind, sideIndex, links, allSides, affecteds, lawsuit).Execute();
}
