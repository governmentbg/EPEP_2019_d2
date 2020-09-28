DEFINE_RESPONSE_TABLE1(TFindStreetAliasFace, TFindAddrAliasFace)
//{{TFindStreetAliasFaceRSP_TBL_BEGIN}}
	EV_NOTIFY_AT_CHILD(CBN_SETFOCUS, EvSetFocus),
	EV_NOTIFY_AT_CHILD(CBN_DROPDOWN, CBNDropDown),
//{{TFindStreetAliasFaceRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TFindStreetAliasFace Implementation}}

TFindStreetAliasFace::TFindStreetAliasFace(TWindow* parent, int resourceId, const char *name, TFindUCLPAliasFace *tUCLP,
	long flags)
:
	TFindAddrAliasFace(parent, resourceId, name, flags & ADDR_ALTER ? SIZE_OF_STREET_NUMBERED : SIZE_OF_STREET_NAME,
		Street), uclp(tUCLP), numbered(flags & ADDR_ALTER)
{
}

void TFindStreetAliasFace::Reset(long street)
{
	if (!uclp->GetDroppedState())
		TFindAddrAliasFace::Reset(uclp->LNow() ? street : 0);
}

void TFindStreetAliasFace::Add()
{
	if (uclp->L())
	{
		TStreet *street = new TStreet;

		street->uclp = uclp->L();

		if (add_street(this, street))
		{
			SetIndexNotify(InsertGroup(street));
			UpdateVisual();
		}
		else
			delete street;
	}
	else
		error("Населеното място липсва или е невалидно.");
}

bool TFindStreetAliasFace::IsValid()
{
	bool isValid = TFindAddrAliasFace::IsValid();

	if (isValid)
	{
		if (L() && !uclp->L())
			isValid = error("Избрана е улица, но населеното място липсва или е невалидно.");
	}

	return isValid;
}

bool TFindStreetAliasFace::EditGroup(TGroup *group)
{
	return edit_street(this, (TStreet *) group);
}

void TFindStreetAliasFace::GetGroupText(mstr &m, TGroup *group, int column)
{
	TGroupList::GetGroupText(m, group, column);

	if (numbered)
		m.printf(" (%ld)", ((TStreet *) group)->street);
}

void TFindStreetAliasFace::Criteria(msql &m, TGroup *group)
{
#if RANGECHECK
	if (!uclp->L())
		fatal("%s: criteria: no uclp", name);
#endif  // RANGECHECK
	Street->uclp = uclp->L();
	TFindLongAliasFace::Criteria(m, group);
}

void TFindStreetAliasFace::EvSetFocus()
{
	if (!uclp->L())
		FlushVisuals();
}

void TFindStreetAliasFace::CBNDropDown()
{
	if (uclp->L())
		TFindAddrAliasFace::CBNDropDown();
	else
		TAliasFace::CBNDropDown();
}
