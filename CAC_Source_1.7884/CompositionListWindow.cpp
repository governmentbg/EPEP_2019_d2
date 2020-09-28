//{{TCompositionListWindow Implementation}}

TCompositionListWindow::TCompositionListWindow(TWindow* parent, int resourceId)
:
	TGroupListWindow(parent, resourceId, new TLongAliasArray, COURT_FLAT ? 100 : 76), college('\0')
{
	if (COURT_FLAT)
	{
		Add(
			new TGroupListColumn("", group_inactive, 12, LVCFMT_CENTER | LVCFMT_NO_SEARCH),
			new TGroupListColumn("»ÏÂ", "F_NAME", 76),
			NULL
		);
	}
	else
		Add(new TGroupListColumn("»ÏÂ", "F_NAME", 64));

	initialRefresh = false;
}

void TCompositionListWindow::SetCollege(char tCollege)
{
	if (college != tCollege)
	{
		college = tCollege;
		Refresh();
	}
}

void TCompositionListWindow::Print()
{
	print_compositions((TGArray<TFineComposition> *) array, college);
}

bool TCompositionListWindow::Active()
{
	TFineComposition *composition = Composition();

	composition->flags ^= FLAG_ACTIVE;

	if (composition->Update("F_FLAGS"))
	{
		RedrawGroup();
		return true;
	}

	composition->flags ^= FLAG_ACTIVE;
	return false;
}

TGroup *TCompositionListWindow::NewGroup() const
{
	TFineComposition *composition = new TFineComposition;
	composition->college = college;
	return composition;
}

bool TCompositionListWindow::EditGroup(TGroup *group)
{
	TFineComposition *composition = (TFineComposition *) group;

	if (TEditCompositionDialog(this, composition).Execute() == IDOK)
	{
		if (strcmp(composition->name, Composition()->name))
		{
			TFineComposition tComposition;
			tComposition << composition;

			for (const char *college = COLLEGE_ALLS; *college; college++)
			{
				if (*college == composition->college)
					continue;

				tComposition.college = *college;
				strcpy(tComposition.name, composition->name);
				tComposition.Update();
			}
		}

		return true;
	}

	return false;
}
