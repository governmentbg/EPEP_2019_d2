#include "cac.h"

#include "ChooseRelationDialog.h"
#include "ChooseRelationDialog.cpp"

static TLinkWind *create_link_wind(TWindow *parent, TLinkWindArray *linkWinds, TSideWind *ordinary, TSideWind *proxy)
{
	TLinkWind *linkWind = new TLinkWind;
	int relation;

	linkWind->SetOrdinary(ordinary);
	linkWind->SetProxy(proxy);

	if (linkWinds->FindLink(linkWind))
	{
		mstr m;

		linkWind->Print(m);
		error("Връзката\n""%s""\nне може да бъде добавена (участвуващите в нея страни са вече свързани).", str(m));
		relation = IDCANCEL;
	}
	// 2014:216 support for with invs
	else if (strchr(SUMMONS_FOR_SIDE_WITH_INVS, proxy->involvement))
	{
		relation = TChooseRelationDialog(parent, ordinary, proxy, IDD_CHOOSE_RELATION_WITH_INV).Execute();

		if (relation == IDC_CHOOSE_RELATION_NORMAL_BY_INDIRECT)
			linkWind->SetProxyKind(false);
		else
			linkWind->SetProxyKind(false, 7);
	}
	else if (strchr(SUMMONS_FOR_SIDE_LEGAL_SPOK_INVS, proxy->involvement))
	{
		// За тези участия се изисква уточнявяне на вида на връзката
		// IDD_CHOOSE_RELATION DIALOG
		int which_any = 0;

		if (proxy->involvement == INVOLVEMENT_GUARDIAN)
			relation = IDC_CHOOSE_RELATION_INDIRECT_AS_ANY;
		//else if (proxy->involvement == INVOLVEMENT_TRUSTEE)
		//	relation = IDC_CHOOSE_RELATION_CONSENT_OF_INDIRECT;
		else
		{
			// диалог "Начин на упълномощаване" - за уточняване на вида на връзката
			relation = TChooseRelationDialog(parent, ordinary, proxy).Execute();

			switch (relation)
			{
				case IDC_CHOOSE_RELATION_INDIRECT_AS_ANY : which_any = 1; break;
				case IDC_CHOOSE_RELATION_INDIRECT_AS_ANY_1 : which_any = 2; break;
				case IDC_CHOOSE_RELATION_INDIRECT_AS_ANY_2 : which_any = 3; break;	// 20080221
				case IDC_CHOOSE_RELATION_INDIRECT_AS_ANY_3 : which_any = 4; break;	// 20080221
			}
		}

		linkWind->SetProxyKind(relation == IDC_CHOOSE_RELATION_CONSENT_OF_INDIRECT, which_any);	// 20070730
	}
	else
	{
		linkWind->SetProxyKind(false);	// 20070730
		relation = IDOK;
	}

	if (relation != IDCANCEL)
		return linkWind;

	delete linkWind;
	return NULL;
}

#include "SideWindListBox.h"
#include "SideWindListBox.cpp"

#include "CreateLinksDialog.h"
#include "CreateLinksDialog.cpp"

#include "ChooseRedirectDialog.h"
#include "ChooseRedirectDialog.cpp"

#include "LinkWindListWindow.h"
#include "LinkWindListWindow.cpp"

#include "LinkWindListDialog.h"
#include "LinkWindListDialog.cpp"

void process_links(TWindow *parent, TLinkWindArray *linkWinds)
{
	TGArray<TSideWind> ordinary;
	TGArray<TSideWind> proxy;
	TGArray<TSideWind> redirect;

	for (int i = 0; i < linkWinds->Sides()->Count(); i++)
	{
		TSideWind *sideWind = (*linkWinds->Sides())[i];

		if (strchr(SUMMONS_FOR_SIDE_INDIRECT_INVS, sideWind->involvement))
		{
			proxy.Add(sideWind);

			if (!strchr(SUMMONS_FOR_SIDE_LEGAL_SPOK_INVS, sideWind->involvement))
				redirect.Add(sideWind);
		}
		else
			ordinary.Add(sideWind);
	}

	TLinkWindListDialog(parent, linkWinds, &ordinary, &proxy, &redirect).Execute();

	ordinary.Clear();
	proxy.Clear();
	redirect.Clear();
}
