DEFINE_RESPONSE_TABLE1(TFindEisPlaceAliasFace, TEisAliasFace)
//{{TFindEisPlaceAliasFaceRSP_TBL_BEGIN}}
	EV_NOTIFY_AT_CHILD(CBN_DROPDOWN, CBNDropDown),
//{{TFindEisPlaceAliasFaceRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TFindEisPlaceAliasFace Implementation}}

TFindEisPlaceAliasFace::TFindEisPlaceAliasFace(TWindow* parent, int resourceId, const char *name,
	TEisRegionAliasFace *tRegion, TXMLObject *object)
:
	TEisAliasFace(parent, resourceId, name, 0, object, ALIAS_ABCSORT | ALIAS_DROPDOWN), region(tRegion)
{
	initialRefresh = false;
	flags &= ~ALIAS_EMPTY;
	alias = EisPlace;
}

void TFindEisPlaceAliasFace::Reset(long place)
{
	if (place)
	{
		EisPlace->code = place;
		flags &= ~ADDR_NAMED;
		TEisAliasFace::Refresh();
		SetL(place);
	}
	else
		FlushList();
}

long TFindEisPlaceAliasFace::LNow()
{
	int index = GetSelIndex();
	return index >= 0 ? LongAt(index) : 0;
}

int TFindEisPlaceAliasFace::GetVisualIndex()
{
	return FindVisualIndex();
}

void TFindEisPlaceAliasFace::Criteria(msql &m, TGroup *group)
{
#if RANGECHECK
	if (!region->L())
		fatal("%s: criteria: no region", name);
#endif  // RANGECHECK
	EisPlace->nom = region->L();
	TLongAliasFace::Criteria(m, group);
}

void TFindEisPlaceAliasFace::SetupWindow()
{
	TAliasFace::SetupWindow();

	if (!initialRefresh)
		autoEnable = false;
}

void TFindEisPlaceAliasFace::CBNDropDown()
{
	TEisAliasFace::CBNDropDown();

	if (FindVisualIndex() < 0 || !(flags & ADDR_NAMED))
	{
		if (strchr(text, '%') || strchr(text, '_'))
		{
			error("Критерия за търсене съдържа невалидни символи.");
			FlushList();
		}
		else
		{
			strcpy(EisPlace->name, text);
			flags |= ADDR_NAMED;
			Refresh();
		}
	}
}
