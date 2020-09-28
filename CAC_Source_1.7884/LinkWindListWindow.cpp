//{{TLinkWindListWindow Implementation}}

TLinkWindListWindow::TLinkWindListWindow(TWindow* parent, int resourceId, TLinkWindArray *linkWinds,
	TGArray<TSideWind> *tOrdinary, TGArray<TSideWind> *tProxy, TGArray<TSideWind> *tRedirect)
:
	TGroupListWindow(parent, resourceId, linkWinds, 376), ordinary(tOrdinary), proxy(tProxy), redirect(tRedirect)
{
	dynamic = loadData = false;
	Add(new TGroupListColumn("Връзка", group_print, 364));
}

TGroup *TLinkWindListWindow::NewGroup() const
{
	return new TLinkWind;
}

bool TLinkWindListWindow::AddGroup(TGroup *)
{
	TGArray<TLinkWind> newLinks;

	TCreateLinksDialog(this, LinkWinds(), ordinary, proxy, &newLinks).Execute();

	if (newLinks.Count())
	{
		int index = -1;

		for (int i = 0; i < newLinks.Count(); i++)
		{
			TLinkWind *linkWind = newLinks[i];

			if (linkWind->Insert())
				index = InsertGroup(newLinks[i]);
			else
				delete linkWind;
		}

		if (index != -1)
		{
			SetVisualIndex(index);
			UpdateVisual();
		}

		newLinks.Clear();
	}

	return false;
}

bool TLinkWindListWindow::EditGroup(TGroup *group)
{
	TLinkWind *linkWind = (TLinkWind *) group;

	if (linkWind->Redirected())
	{
		linkWind->ClearRedirection();
		return true;
	}

	return TChooseRedirectDialog(this, linkWind, redirect).Execute() == IDOK && linkWind->Update();
}

bool TLinkWindListWindow::AskDelete(TGroup *)
{
	return ask("Желаете ли да изтриете избраната връзка?");
}
