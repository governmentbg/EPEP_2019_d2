//{{TSubpoenaListWindow Implementation}}

TSubpoenaListWindow::TSubpoenaListWindow(TWindow* parent, int resourceId, TRCDKeyContainer *tContainer, TSideWind *tSideWind)
:
	TDoubleListWindow(parent, resourceId, &subpoenas, 432), container(tContainer), sideWind(tSideWind)
{
	Add(
		new TGroupListColumn("", generic_null, 0),
		new TGroupListColumn("No", "F_SUBPOENA_NO", 16, LVCFMT_RIGHT),
		new TGroupListColumn("Вид", subpoena_kind, 56),
		new TGroupListColumn("Адрес", subpoena_address, 84),
		new TGroupListColumn("Резултат", subpoena_result, 56),
		new TGroupListColumn("Изготвяне", "F_SUBPOENA_DATE", 40),
		new TGroupListColumn("Връчване/Уведомяване", "F_SUBPOENA_RECEIVED", 40),
		new TGroupListColumn("Официално уведомен", subpoena_official, 16),
		new TGroupListColumn("Представител", subpoena_proxy, 84),
		new TGroupListColumn("Връщане в деловодството", "F_SUBPOENA_RETURNED", 40),
		NULL
	);

	allowDeselect = true;

	TSubpoena subpoena;
	TBaseWind baseWind(subpoena.Name, NULL);
	msql m;

	baseWind << sideWind;
	baseWind.ExactCriteria(m);

	TQuery q(str(m));
	while (q.Read())
	{
		subpoena << q;
		subpoenas.Add(new TDoubleSubpoena(&subpoena));
	}
}

bool TSubpoenaListWindow::Delete(TSubpoenaVariantGroup *variant)
{
	TSubpoena *subpoena = Subpoena();

	if (subpoena)
	{
		const TSubpoena *letter = FindLetter(subpoena);

		if (letter)
		{
			constant what = strchr(SUBPOENA_KIND_STICKERS, letter->subpoenaKind) ? "уведомление по чл.47" :
				"придружително писмо";

			error("Има издадено %s № %ld.", what, letter->subpoenaNo);
		}
		else if (subpoena->AskDelete() && delete_subpoena(subpoena, variant->affecteds, variant->lawsuit))
		{
			variant->Affected(subpoena);
			RemoveGroup(IDDELETE);
			return true;
		}
	}

	return false;
}

void TSubpoenaListWindow::ServeStickers(const TSubpoena *subpoena)
{
	if (!strchr(SRS_DELIV_BASICS, subpoena->receivedStatus))
	{
		for (int index = 0; index < subpoenas.Count(); index++)
		{
			TSubpoena *sticker = subpoenas[index];

			if (strchr(SUBPOENA_KIND_STICKERS, sticker->subpoenaKind) && sticker->regard == subpoena->subpoenaNo)
				if (sticker->eventStatus == subpoena->eventStatus && serve_sticker(subpoena, sticker))
					DrawGroupAt(sticker, index << 1, false);
		}
	}
}

bool TSubpoenaListWindow::ReturnGroup(char originKind, char secondKind)
{
	TSubpoena *subpoena = Subpoena();
	TSubpoena *notified;

	for (int index = 0; index < subpoenas.Count(); index++)
	{
		notified = subpoenas[index];

		if (notified != subpoena && notified->eventStatus == subpoena->eventStatus && notified->OfficiallyNotified())
			break;

		notified = NULL;
	}

	if (serve_subpoena(this, subpoena, sideWind, container, originKind, secondKind, subpoena->eventStatus,
		notified != NULL))
	{
		DrawGroupAt(subpoena, GetVisualIndex(), false);
		ServeStickers(subpoena);
		return true;
	}

	return false;
}

TSubpoena *TSubpoenaListWindow::FindLetter(const TSubpoena *subpoena)
{
	for (int index = 0; index < subpoenas.Count(); index++)
	{
		TSubpoena *letter = subpoenas[index];
		// 2013:143 LPR: check eventStatus
		if (letter->eventStatus == subpoena->eventStatus && letter->regard == subpoena->subpoenaNo)
			return letter;
	}

	return NULL;
}

void TSubpoenaListWindow::DrawSecond(TListWindItem &lwi, TGroup *group)
{
	TDoubleSubpoena *subpoena = (TDoubleSubpoena *) group;

	lwi.iSubItem = 3;
	lwi.SetText(subpoena->address2);
	SetItem(lwi);

	lwi.iSubItem++;
	lwi.SetText(subpoena->result2);
	SetItem(lwi);

	lwi.iSubItem = 8;
	lwi.SetText(subpoena->proxy2);
	SetItem(lwi);
}
