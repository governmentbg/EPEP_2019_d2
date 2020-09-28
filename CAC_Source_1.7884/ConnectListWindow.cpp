#include "cac.h"

DEFINE_RESPONSE_TABLE1(TConnectListWindow, TGroupListWindow)
//{{TGroupListWindowRSP_TBL_BEGIN}}
	EV_WM_LBUTTONDBLCLK,
//{{TGroupListWindowRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TConnectListWindow Implementation}}

TConnectListWindow::TConnectListWindow(TWindow* parent, int resourceId, TGArray<TConnect> *tArray, const char *tPredefined, TEisNmrFace *tEisppNmr)
:
	TGroupListWindow(parent, resourceId, tArray ? tArray : new TGArray<TConnect>, 268), dirty(false), indocKind(KIND_NULL),
		lawKind(KIND_NULL), predefined(tPredefined), eispp_nmr(tEisppNmr)
{
	Add(new TGroupListColumn("Дело на друга институция/инстанция", group_print, 256));
	loadData = false;
	dynamic = !tArray;
}

void TConnectListWindow::SetIndocKind(char tIndocKind)
{
	indocKind = tIndocKind;
}

void TConnectListWindow::AdjustJurisd()
{
	// 2009:258 LPR: auto convert normal/high jurisdiction
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	for (int index = 0; index < Connects()->Count(); index++)
	{
		TConnect *connect = (*Connects())[index];

		if (connect->connectType == CONNECT_TYPE_JURISDICTION || connect->connectType == CONNECT_TYPE_HIGH_JURISDICTION)
			if ((strchr(KIND_1ST_INDOCS, indocKind) != NULL) != (connect->connectType == CONNECT_TYPE_JURISDICTION))
				connect->connectType = strchr(KIND_1ST_INDOCS, indocKind) ? CONNECT_TYPE_JURISDICTION : CONNECT_TYPE_HIGH_JURISDICTION;
	}
#endif  // ADMIN || AREAL || MILITARY
}

void TConnectListWindow::SetLawKind(char tLawKind)
{
	lawKind = tLawKind;
}

char TConnectListWindow::MainType()
{
	for (int index = 0; index < Connects()->Count(); index++)
		if (strchr(CONNECT_TYPE_DECISIVES, (*Connects())[index]->connectType))
			return (*Connects())[index]->connectType;

	return '\0';
}

void TConnectListWindow::ResetConnects(TGArray<TConnect> *connects)
{
	delete array;
	array = connects;
	Refresh();
}

void TConnectListWindow::UpdateConnects(TGArray<TConnect> &connects)
{
	while (connects.Count())
	{
		TConnect *newConnect = connects[0];
		TConnect *oldConnect;
		bool matchBasic, matchExtra;
		int index;

		for (index = 0; index < array->Count(); index++)
		{
			oldConnect = (*Connects())[index];
			matchBasic = oldConnect->connectType == newConnect->connectType;

			matchExtra = !strcmp(oldConnect->ucn, newConnect->ucn) && oldConnect->ucnType == newConnect->ucnType &&
				!strcmp(oldConnect->connectNo, newConnect->connectNo) &&
				oldConnect->connectYear == newConnect->connectYear &&
				(oldConnect->connectKind == newConnect->connectKind || !oldConnect->connectKind ||
					!newConnect->connectKind);

			if (matchBasic)
				break;
		}

		if (!matchBasic)
		{
			newConnect->dirty = true;
			array->Add(newConnect);
			connects.Detach(0);
		}
		else if (matchExtra)
		{
			if (!oldConnect->connectKind)
				oldConnect->connectKind = newConnect->connectKind;

			if (!oldConnect->composition)
				oldConnect->composition = newConnect->composition;

			if (!oldConnect->decisionKind && oldConnect->decisionDate.Empty())
			{
				oldConnect->decisionKind = newConnect->decisionKind;
				oldConnect->decisionDate = newConnect->decisionDate;
			}

			if (!oldConnect->outKind && !oldConnect->outNo && !oldConnect->outYear)
			{
				oldConnect->outKind = newConnect->outKind;
				oldConnect->outNo = newConnect->outNo;
				oldConnect->outYear = newConnect->outYear;
			}

			if (!any(oldConnect->text))
				strcpy(oldConnect->text, newConnect->text);

			oldConnect->dirty = true;
			connects.Remove(0);
		}
		else
		{
			Connects()->Remove(index);
			newConnect->dirty = true;
			array->Insert(newConnect, index);
			connects.Detach(0);
		}
	}

	dirty = true;
	Refresh();
}

bool TConnectListWindow::MayEdit()
{
	TConnect *connect = (TConnect *) Group();

	if (!connect)
		return false;

#if COURT_TYPE == COURT_ADMIN
	if (connect->connectType == CONNECT_TYPE_LOWER_INSTANCE && connect->ucnType == UCN_INSTANCE_SELF &&
		!strcmp(connect->ucn, INSTANCE_SELF))
	{
		return false;
	}
#endif  // ADMIN

	return true;
}

bool TConnectListWindow::MayDelete()
{
	TConnect *connect = (TConnect *) Group();

	if (!connect)
		return false;

#if COURT_TYPE == COURT_APPEAL
	if (predefined && connect->ReturnedTo())
		return false;
#endif  // APPEAL

#if COURT_TYPE == COURT_ADMIN
	if (connect->connectType == CONNECT_TYPE_LOWER_INSTANCE && connect->ucnType == UCN_INSTANCE_SELF &&
		!strcmp(connect->ucn, INSTANCE_SELF))
	{
		return false;
	}
#endif  // ADMIN

	return !predefined || !strchr(predefined, connect->connectType);
}

bool TConnectListWindow::IsValid()
{
	char entered[SIZE_OF_CONNECT_TYPES];
#if COURT_TYPE == COURT_APPEAL
	const char *receivers[2] = { NULL, NULL };
#endif  // APPEAL

	*entered = '\0';

	for (int index = 0; index < array->Count(); index++)
	{
		TConnect *connect = (*Connects())[index];

		if (*eispp_nmr->S())
		{
			ConnectType->Seek(connect->connectType);

			if (ConnectType->eispp && !connect->xferKind)
				return error("При въведен ЕИСПП номер се изисква точен вид дело/документ за %s.", ConnectType->name);
		}

	#if COURT_TYPE == COURT_APPEAL
		bool lower = connect->connectType == CONNECT_TYPE_LOWER_INSTANCE;

		if (lower || connect->connectType == CONNECT_TYPE_RECALL_LAWSUIT)
		{
			receivers[lower] = connect->ucn;

			if (receivers[0] && receivers[1] && !strcmp(receivers[0], receivers[1]))
				return error("Първоинстанционния и въззивния съд трябва да са различни.");
		}
	#endif  // APPEAL

		strcach(entered, connect->connectType);
	}

	return connect_types_valid(indocKind, lawKind, entered, predefined);
}

TGroup *TConnectListWindow::NewGroup() const
{
	return new TConnect;
}

bool TConnectListWindow::AddGroup(TGroup *group)
{
	char already[SIZE_OF_CONNECT_TYPES];
	char possible[SIZE_OF_CONNECT_TYPES];
	TConnect *connect = (TConnect *) group;

	*already = '\0';
	for (int i = 0; i < array->Count(); i++)
		strcach(already, (*Connects())[i]->connectType);
	adjust_possible_connects(possible, indocKind, lawKind, already, predefined);

	if (!any(possible))
		error("Вече са добавени всички възможни видове дела и документи.");
	else if (TEditConnectDialog(this, connect, possible, kind2type(lawKind), *eispp_nmr->S()).Execute() == IDOK)
	{
	#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
		// 2013:239
		int i;

		for (i = 0; i < Connects()->Count(); i++)
			if ((*Connects())[i]->connectType == CONNECT_TYPE_368_NPC_LAWSUIT)
				break;

		#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
		if (i == Connects()->Count() && strchr(KIND_1ST_INDOCS, indocKind) &&
		#endif  // AREAL || MILITARY
		#if COURT_TYPE == COURT_REGIONAL
		if (i == Connects()->Count() && strchr(KIND_INDOCS, indocKind) &&
		#endif  // REGIONAL
			lawKind == KIND_COMMON_PUNISHMENT_LAW && connect->connectType == CONNECT_TYPE_PRECOURT_LAWSUIT)
		{
			msql m("T_CONNECT C JOIN T_LAWSUIT L ON L.F_NO = C.F_NO AND L.F_YEAR = C.F_YEAR "
				"AND L.F_TYPE = C.F_TYPE", "L.*");

			m.Add(connect, "F_CONNECT_NO, F_CONNECT_YEAR, F_CONNECT_TYPE, F_UCN, F_UCN_TYPE");
			m.AddChar("F_KIND", KIND_PRIVATE_PUNISHMENT_LAW);
			m.AddChar("F_SOURCE_KIND", KIND_REQUEST_368);
			TQuery q(str(m));

			if (q.Read())
			{
				TLawsuit lawsuit;
				TConnect *tConnect = new TConnect;

				lawsuit << q;
				tConnect->key = connect->key;
				strcpy(tConnect->ucn, INSTANCE_SELF);
				tConnect->ucnType = UCN_INSTANCE_SELF;
				itoa(lawsuit.key.no, tConnect->connectNo, 10);
				tConnect->connectYear = lawsuit.key.year;
				tConnect->connectType = CONNECT_TYPE_368_NPC_LAWSUIT;
				tConnect->connectKind = CONNECT_KIND_CRIME_LAWSUIT;

				if (has_eispp_nmr)
					tConnect->xferKind = Kind->Xfer(lawsuit.kind);

				InsertGroup(tConnect);
				info("За посоченото ДП има образувано ЧНД %ld/%ldг. по постъпило \"%s\". Резултат - %s.",
					lawsuit.key.no, lawsuit.key.year, Kind->Seek(lawsuit.sourceKind),
					Result->Seek(lawsuit.Status()));
			}
		}
	#endif  // AREAL || MILITARY || REGIONAL
		return dirty = connect->dirty = true;
	}

	return false;
}

bool TConnectListWindow::EditGroup(TGroup *group)
{
	TConnect *connect = (TConnect *) group;

	// 2009:106 FIX: connect->dirty was missing
	if (TEditConnectDialog(this, connect, C2S[connect->connectType], TYPE_NULL, any(eispp_nmr->S())).Execute() == IDOK)
		return dirty = connect->dirty = true;

	return false;
}

bool TConnectListWindow::DeleteGroup(TGroup *)
{
	return true;
}

void TConnectListWindow::EvLButtonDblClk(uint modKeys, TPoint& point)
{
	TListWindow::EvLButtonDblClk(modKeys, point);
	ParentNotify(IDEDIT);
}
