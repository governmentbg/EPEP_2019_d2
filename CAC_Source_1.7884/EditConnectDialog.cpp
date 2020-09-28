#include "cac.h"

DEFINE_RESPONSE_TABLE1(TEditConnectDialog, TGroupDialog)
//{{TEditConnectDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_CBN_SELCHANGE(IDC_EDIT_CONNECT_CONNECT_TYPE, ConnectTypeCBNSelChange),
	EV_CBN_SELCHANGE(IDC_EDIT_CONNECT_CONNECT_KIND, ConnectKindCBNSelChange),
	EV_EN_KILLFOCUS(IDC_EDIT_CONNECT_CONNECT_NO, ConnectNoKillFocus),
	EV_CBN_SELCHANGE(IDC_EDIT_CONNECT_UCN, SenderCBNSelChange),
	EV_CBN_SELCHANGE(IDC_EDIT_CONNECT_XFER_KIND, XferKindCBNSelChange),
//{{TEditConnectDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditConnectDialog Implementation}}

TEditConnectDialog::TEditConnectDialog(TWindow* parent, TConnect *connect, const char *connectTypes, char tType, bool xfer,
	int resId)
:
	TGroupDialog(parent, connect, resId), type(tType), xferKind(NULL)
{
	connectType = new TCharAliasFace(this, IDC_EDIT_CONNECT_CONNECT_TYPE, "F_CONNECT_TYPE", ConnectType, connectTypes);
	ConnectType->Seek(*connectTypes);
	senderTypes[0] = ConnectType->senderType;
	senderTypes[1] = '\0';
	sender = new TReceiverAliasFace(this, IDC_EDIT_CONNECT_UCN, "F_UCN", Receiver,
		connect->connectKind == CONNECT_KIND_PRIVATE_EXECUTOR ? UCN_EXECUTORS : senderTypes);
	connectNo = new TConnectNoFace(this, IDC_EDIT_CONNECT_CONNECT_NO, "F_CONNECT_NO", connectType);
	connectYear = new TYearFace(this, IDC_EDIT_CONNECT_CONNECT_YEAR, "F_CONNECT_YEAR");
	// 2012:289 LPR/LRQ: +related
	// 2013:051 FIX: // || connect->xferKind, we don't support non-eispp xfer kinds
	if (xfer)
	{
		xferKind = new TXferKindAliasFace(this, IDC_EDIT_CONNECT_XFER_KIND, "F_XFER_KIND", *connectTypes,
			ALIAS_OPTION | ALIAS_ENABLE);
	}
	// 2010:344 FIX: connectKind, composition, decisionKind "" -> NULL
	connectKind = new TConnectKindAliasFace(this, IDC_EDIT_CONNECT_CONNECT_KIND, "F_CONNECT_KIND", *connectTypes,
		xferKind ? ALIAS_OPTION : ALIAS_OPTION | ALIAS_ENABLE);
	composition = new TCompositionAliasFace(this, IDC_EDIT_CONNECT_COMPOSITION, "F_COMPOSITION", NULL, ALIAS_EMPTY);
	decisionKind = new TCharAliasFace(this, IDC_EDIT_CONNECT_DECISION_KIND, "F_DECISION_KIND", Kind, NULL, ALIAS_EMPTY);
	decisionDate = new TDateFace(this, IDC_EDIT_CONNECT_DECISION_DATE, "F_DECISION_DATE", false);
	new TStringFace(this, IDC_EDIT_CONNECT_TEXT, "F_TEXT", SIZE_OF_CONNECT_TEXT);
	new TCharAliasFace(this, IDC_EDIT_CONNECT_OUT_KIND, "F_OUT_KIND", Kind, KIND_OUT_REG_APPS, ALIAS_EMPTY);
	new TLongFace(this, IDC_EDIT_CONNECT_OUT_NO, "F_OUT_NO", SIZE_OF_EVENT_NO);
	outYear = new TYearFace(this, IDC_EDIT_CONNECT_OUT_YEAR, "F_OUT_YEAR");
}

void TEditConnectDialog::ConnectTypeChanged(bool setDefaultKind)
{
	ConnectType->Seek(connectType->C());

	if (strchr(CONNECT_TYPE_COMPOSITABLES, connectType->C()))
	{
		composition->SetXCrit(NULL);
		composition->SetL(Connect()->composition);
	}
	else
		composition->SetXCrit("");

	if (xferKind)
	{
		xferKind->SetConnectType(connectType->C());
		if (setDefaultKind)
			xferKind->SetDefaultKind(type);
	}

	// 2009:203 LPR: ctor -> here; 2011:356 class method
	connectKind->SetConnectType(connectType->C());
	// 2010:274; 2011:356 class method
	if (setDefaultKind)
		connectKind->SetDefaultKind(type);
	ConnectKindCBNSelChange();
}

void TEditConnectDialog::OKBNClicked()
{
	if (IsValid())
	{
		Connect()->ucnType = sender->UCNType();
		*Group() << this;
		CmOk();
	}
}

void TEditConnectDialog::ConnectTypeCBNSelChange()
{
	ConnectTypeChanged(true);
}

void TEditConnectDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	ConnectTypeChanged(type);
	// 2007:023 LPR: reset after initial/destructive initial type change
	sender->SetS(Connect()->ucn);

	if (xferKind)
	{
		connectKind->ShowWindow(SW_HIDE);
		xferKind->ShowWindow(SW_SHOW);
	}
}

bool TEditConnectDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (!any(connectNo) || !connectYear->L())
			isValid = error("Номерът и годината са задължителни.");
		else if (connectYear->L() > Today.year)
			isValid = error("Годината на съдебното дело е по-голяма от текущата.");
		else if ((decisionKind->C() == '\0') != decisionDate->Date().Empty())
			isValid = error("Видът и датата на обжалвания акт не са комплектни.");
		else if (outYear->L() > Today.year)
			isValid = error("Годината на изходящия документ е по-голяма от текущата.");
	#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
		else if (connectType->C() == CONNECT_TYPE_AFTER_ADMINISTER)
		{
			char type = connectKind2Type(connectKind->C());
			TOriginContainer *container = (TOriginContainer *) type2RCDKeyContainer(type);
			const char *format = NULL;

			container->key.no = atoi(connectNo->S());
			container->key.year = connectYear->L();

			if (container->Try("F_KIND"))
			{
			#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
				if (strchr(KIND_1ST_INDOCS, container->kind))
					format = "%s %ld/%ld е първоинстанционен. %s";
				else if (strchr(KIND_1ST_LAWSUITS, container->kind))
					format = "%s %ld/%ld е първоинстанционно. %s";
				else
			#endif  // ADMIN || AREAL || MILITARY
				if (type == TYPE_REQUEST)
				{
					if (container->Status() != RESOLUTION_REJECTED)
						format = "%s %ld/%ld не е със статус върнат. %s";
				}
				else
				{
					TSession session;

					format = find_finalizer((const TLawsuit *) container, session) &&
						strchr(RESULT_ALL_CANCELEDS, session.result) ? NULL : "%s %ld/%ld не е прекратено. %s";
				}

			}
			else
				format = "Липсва %s %ld/%ld. %s";

			if (format)
				isValid = ask(format, Type->Seek(type), container->key.no, container->key.year, Q_CONTINUE);

			delete container;
		}
	#endif  // APPEAL || ADMIN || AREAL || MILITARY
	}

	return isValid;
}

void TEditConnectDialog::ConnectKindCBNSelChange()
{
	// 2007:023 LPR: clear now incompatible or non-first sender
	// 2007:032 LRQ: clear only if incompatible, otherwise leave
	if (connectKind->C() == CONNECT_KIND_PRIVATE_EXECUTOR)
	{
		if (strcmp(sender->XCrit(), UCN_EXECUTORS))
			sender->SetS(EMPTY);
		sender->SetXCrit(UCN_EXECUTORS);
		SenderCBNSelChange();
	}
	else
	{
		if (*sender->XCrit() != ConnectType->senderType)
			sender->SetS(EMPTY);
		senderTypes[0] = ConnectType->senderType;
		sender->SetXCrit(senderTypes);
		SenderCBNSelChange();
	}

	// 2009:224; 2009:254 LOWER_INSTANCE + RECALL_LAWSUIT -> LOWER_INSTANCES
	if (connectType->C() && strchr(CONNECT_TYPE_LOWER_INSTANCES, connectType->C()) && connectKind->C())
	{
		decisionKind->SetXCrit(connectKind->C() == CONNECT_KIND_CRIME_LAWSUIT ?
			KIND_CRIME_CONNECT_ENDOCS : KIND_OTHER_CONNECT_ENDOCS);

		decisionKind->EnableWindow(true);
		decisionKind->SetC(Connect()->decisionKind);
		decisionDate->EnableWindow(true);
	}
	else
	{
		decisionKind->SetXCrit("");
		decisionKind->EnableWindow(false);
		decisionDate->EnableWindow(false);
	}
}

void TEditConnectDialog::ConnectNoKillFocus()
{
	if (strchr(CONNECT_TYPE_14_DIGITS, connectType->C()) && strlen(connectNo->S()) == 14 &&
		connect_no_valid(connectType->C(), connectNo->S()))
	{
		connectYear->SetL(four_teen_year(connectNo->S()));
	}
}

void TEditConnectDialog::SenderCBNSelChange()
{
	if (strchr(CONNECT_TYPE_COMPOSITABLES, connectType->C()) && *sender->XCrit())
		composition->SetSender(*sender->XCrit(), sender->S());
}

void TEditConnectDialog::XferKindCBNSelChange()
{
	XferKind->Seek(xferKind->L());
	connectKind->SetC(XferKind->connectKind);
	ConnectKindCBNSelChange();
}
