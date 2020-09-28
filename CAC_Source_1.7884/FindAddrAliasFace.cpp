DEFINE_RESPONSE_TABLE1(TFindAddrAliasFace, TFindLongAliasFace)
//{{TFindAddrAliasFaceRSP_TBL_BEGIN}}
	EV_NOTIFY_AT_CHILD(CBN_DROPDOWN, CBNDropDown),
//{{TFindAddrAliasFaceRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TFindAddrAliasFace Implementation}}

TFindAddrAliasFace::TFindAddrAliasFace(TWindow* parent, int resourceId, const char *name, uint textLimit, TFindAddrAliasGroup *alias)
:
	TFindLongAliasFace(parent, resourceId, name, textLimit, alias, NULL, ALIAS_ABCSORT)
{
	initialRefresh = false;
}

void TFindAddrAliasFace::Reset(long value)
{
	if (value)
	{
		LongAliasGroup()->ValueField()->SetL(value);
		flags &= ~ADDR_NAMED;
		TFindLongAliasFace::Refresh();
		SetL(value);
	}
	else
		FlushList();
}

void TFindAddrAliasFace::Edit()
{
	if (IsValid())
	{
		TGroup *group = Group();

		if (group)
		{
			TGroup *tGroup = NewGroup();

			*tGroup << group;
			if (EditGroup(tGroup))
			{
				*group << tGroup;
				ResortGroup();
				UpdateVisual();
			}

			delete tGroup;
		}
		else
			say_no_selection();
	}
}

void TFindAddrAliasFace::CBNDropDown()
{
	TFindLongAliasFace::CBNDropDown();

	if (FindVisualIndex() < 0 || !(flags & ADDR_NAMED))
	{
		if (strchr(text, '%') || strchr(text, '_'))
		{
			error("Критерия за търсене съдържа невалидни символи.");
			FlushList();
		}
		else
		{
			LongAliasGroup()->NameField->SetS(text);
			flags |= ADDR_NAMED;
			Refresh();
		}
	}
}
