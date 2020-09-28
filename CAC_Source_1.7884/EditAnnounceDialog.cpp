DEFINE_RESPONSE_TABLE1(TEditAnnounceDialog, TGroupDialog)
//{{TEditAnnounceDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDADD, AddStreetBNClicked),
	EV_BN_CLICKED(IDEDIT, EditStreetBNClicked),
	EV_CBN_SELCHANGE(IDC_EDIT_ANNOUNCE_SUBPOENA_KIND, SubpoenaKindCBNSelChange),
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_CBN_SELCHANGE(IDC_EDIT_ANNOUNCE_STREET, StreetCBNSelChange),
	EV_EN_KILLFOCUS(IDC_EDIT_ANNOUNCE_ADR_NO, AdrNoKillFocus),
	EV_EN_KILLFOCUS(IDC_EDIT_ANNOUNCE_BLOCK_NO, BlockNoKillFocus),
	EV_CBN_SELCHANGE(IDC_EDIT_ANNOUNCE_UCLP, UCLPCBNSelChange),
	EV_EN_KILLFOCUS(IDC_EDIT_ANNOUNCE_NO, NoKillFocus),
	EV_EN_KILLFOCUS(IDC_EDIT_ANNOUNCE_YEAR, YearKillFocus),
	EV_CBN_SELCHANGE(IDC_EDIT_ANNOUNCE_KIND, KindCBNSelChange),
//{{TEditAnnounceDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditAnnounceDialog Implementation}}

TEditAnnounceDialog::TEditAnnounceDialog(TWindow* parent, TAnnounce *announce, bool tEdit, TGroupArray *tDeliverers,
	const char *kinds, bool tScan)
:
	TGroupDialog(parent, announce, IDD_EDIT_ANNOUNCE), edit(tEdit), deliverers(tDeliverers),
		fixeiver(announce->SenderType()), subpoena(announce->subpoena), scan(tScan)
{
	if (scan && any(announce->barcode))
	{
		sender = new TReceiverAliasFace(this, IDC_EDIT_ANNOUNCE_RECEIVER, "F_SENDER", Receiver,
			announce->court ? C2S[announce->SenderType()] : UCN_INSTANCE_ANNOUNCES, "Съдът");
	}
	else
	{
		if (fixeiver.ucnType == UCN_INSTANCE_SELF)
			fixeiver.ucns[0] = INSTANCE_SELF;
		else
		{
			courtUCN.printf("%010d", Announce()->court);
			fixeiver.ucns[0] = str(courtUCN);
		}
		sender = new TStringAliasFace(this, IDC_EDIT_ANNOUNCE_SENDER, "F_SENDER", &fixeiver, C2S[announce->SenderType()]);
	}

	kind = new TCharAliasFace(this, IDC_EDIT_ANNOUNCE_KIND, "F_KIND", ConnectKind, kinds);
	no = new TLongFace(this, IDC_EDIT_ANNOUNCE_NO, "F_NO", SIZE_OF_NO);
	year = new TYearFace(this, IDC_EDIT_ANNOUNCE_YEAR, "F_YEAR");
	subpoenaKind = new TCharAliasFace(this, IDC_EDIT_ANNOUNCE_SUBPOENA_KIND, "F_SUBPOENA_KIND", SubpoenaKind,
		subpoena ? NULL : SUBPOENA_KIND_GENERICS);
	sessionDate = new TDateFace(this, IDC_EDIT_ANNOUNCE_SESSION_DATE, "F_SESSION_DATE", false);
	sideNo = new TLongFace(this, IDC_EDIT_ANNOUNCE_SIDE_NO, "F_SIDE_NO", SIZE_OF_NO);
	area = new TAreaAliasFace(this, IDC_EDIT_ANNOUNCE_AREA, "F_AREA");
	region = new TRegionAliasFace(this, IDC_EDIT_ANNOUNCE_REGION, "F_REGION", area);
	uclp = new TFindUCLPAliasFace(this, IDC_EDIT_ANNOUNCE_UCLP, "F_UCLP", area, region);
	street = new TFindStreetAliasFace(this, IDC_EDIT_ANNOUNCE_STREET, "F_STREET", uclp);
	addStreet = new TPushButton(this, IDADD);
	editStreet = new TPushButton(this, IDEDIT);
	noLabel = new TStatic(this, IDC_EDIT_ANNOUNCE_NO_LABEL);
	adrNo = new TStringFace(this, IDC_EDIT_ANNOUNCE_ADR_NO, "F_ADR_NO", SIZE_OF_ADR_NO);
	blockNo = new TStringFace(this, IDC_EDIT_ANNOUNCE_BLOCK_NO, "F_BLOCK_NO", SIZE_OF_BLOCK_NO);
	deliverer = new TUserAliasFace(this, IDC_EDIT_ANNOUNCE_DELIVERER, "F_DELIVERER_UCN", C2S[POST_DELIVERER]);
	subpoenaText = new TStringFace(this, IDC_EDIT_ANNOUNCE_SUBPOENA_TEXT, "F_SUBPOENA_TEXT", SIZE_OF_SUBPOENA_TEXT);
	composition = new TLongAliasFace(this, IDC_EDIT_ANNOUNCE_COMPOSITION, "F_COMPOSITION", Composition, Default->compositions, ALIAS_EMPTY);
	subNo = new TStringFace(this, IDC_EDIT_ANNOUNCE_SUB_NO, "F_SUB_NO", SIZE_OF_SUB_NO);
	entrance = new TStringFace(this, IDC_EDIT_ANNOUNCE_ENTRANCE, "F_ENTRANCE", SIZE_OF_ENTRANCE);
	floor = new TStringFace(this, IDC_EDIT_ANNOUNCE_FLOOR, "F_FLOOR", SIZE_OF_FLOOR);
	apartment = new TStringFace(this, IDC_EDIT_ANNOUNCE_APARTMENT, "F_APARTMENT", SIZE_OF_APARTMENT);
	subpoenaDate = new TDateFace(this, IDC_EDIT_ANNOUNCE_SUBPOENA_DATE, "F_SUBPOENA_DATE", !announce->subpoena);
	barcodeText = new TStringFace(this, IDC_EDIT_ANNOUNCE_BARCODE_TEXT, "F_BARCODE_TEXT", SIZE_OF_BARCODE_TEXT);
}

void TEditAnnounceDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	sender->ShowWindow(SW_SHOW);
	area->SetL(UCLP->area);
	region->Reset(UCLP->region);
	uclp->Reset(Announce()->uclp);
	street->Reset(Announce()->street);

	if (subpoena)
	{
		subpoenaKind->EnableWindow(false);
		sideNo->EnableWindow(false);
		uclp->EnableWindow(false);
		street->EnableWindow(false);
		adrNo->EnableWindow(false);
		blockNo->EnableWindow(false);
		subNo->EnableWindow(false);
		entrance->EnableWindow(false);
		floor->EnableWindow(false);
		apartment->EnableWindow(false);
		subpoenaDate->EnableWindow(false);
		// 2018-03-21 FIX: edit via Serve
		subpoenaText->EnableWindow(false);
	}
	else
	{
		if (!edit)
		{
			subpoenaDate->EnableWindow(scan);
			no->EnableWindow(true);
			year->EnableWindow(true);
		}

		if (can_direct(area->L(), region->L(), LOCK_STREET))
		{
			addStreet->EnableWindow(true);
			editStreet->EnableWindow(true);
		}

		KindCBNSelChange();
	}

	get_barcode_text(Announce(), barcodeText);
}

void TEditAnnounceDialog::AddStreetBNClicked()
{
	street->Add();
}

void TEditAnnounceDialog::EditStreetBNClicked()
{
	street->Edit();
}

void TEditAnnounceDialog::SubpoenaKindCBNSelChange()
{
	char tSubpoenaKind = subpoenaKind->C();

	if (strchr(SUBPOENA_KIND_GENERICS, tSubpoenaKind))
	{
		if (strchr(SUBPOENA_KIND_GENERIC_WRITS, tSubpoenaKind))
			sessionDate->EnableWindow(true);
		else
		{
			sessionDate->EnableWindow(false);
			sessionDate->SetS("");
		}
	}
	else
	{
		SubpoenaKind->Seek(tSubpoenaKind);
		if (!SubpoenaKind->genericKind || !strchr(SUBPOENA_KIND_GENERIC_WRITS, SubpoenaKind->genericKind))
			sessionDate->SetS("");
	}
}

void TEditAnnounceDialog::OKBNClicked()
{
	if (IsValid() && ask_cause(cause))
	{
		if (subpoena)
		{
			// 2018-03-21 FIX: edit via Serve
			//strcpy(subpoena->receivedText, Announce()->subpoenaText);
			strcpy(subpoena->delivererUcn, Announce()->delivererUcn);

			if (subpoena->Update("F_DELIVERER_UCN"))
				CmOk();
		}
		else if (Group()->Save(edit))
		{
			log_announce(edit ? LOG_EDIT : LOG_ADD, Announce(), "");
			CmOk();
		}
	}
}

bool TEditAnnounceDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		SetComposition();
		*Announce() << this;

		if (sender->IsWindowEnabled())  // 2018-07-20: FIX: wrong atof(INSTANCE_SELF)
			Announce()->court = strcmp(sender->S(), INSTANCE_SELF) ? atof(sender->S()) : COURT_CODE;

		if (!write)
			isValid = error("%s.", str(cause));
		else if (subpoena)
		{
			if (!atob(Announce()->delivererUcn) && (strchr(SRS_DELIVEREDS, Announce()->receivedStatus)
				|| Default->post != POST_ADMIN))
			{
				isValid = error("Призовкарят е задължителен.");
			}
		}
		else if (!Announce()->key.kind || !Announce()->key.no || !Announce()->key.year)
			isValid = error("Полетата вид дело, номер и година са задължителни.");
		else if (sessionDate->IsWindowEnabled() && Announce()->sessionDate.Empty())
			isValid = error("Датата на насрочване е задължителна.");
		else if (!Announce()->uclp || !Announce()->street || (!any(Announce()->adrNo) && !any(Announce()->blockNo)))
			isValid = error("Полетата населено място, улица и номер (или блок номер) са задължителни.");
		else if (!atob(Announce()->delivererUcn))
			isValid = error("Призовкарят е задължителен.");
		else if (!edit)
		{
			// 2010:048 LRQ: error() -> ask()
			if (sessionDate->IsWindowEnabled() && Announce()->sessionDate < Today &&
				!ask("Датата на насрочване е по-малка от текущата дата. Желаете ли да продължите?"))
			{
				isValid = false;
			}
			else
			{
				msql m;

				Announce()->FloatCriteria(m, "F_NO");
				m.Add(Announce(), "F_SUBPOENA_KIND, F_SESSION_DATE, F_UCLP, F_STREET, F_SIDE_NO");

				TQuery q(str(m));
				if (q.Read())
					isValid = ask("Вече има въведена такава призовка/съобщение. Желаете ли да продължите?");
			}
		}
	}

	return isValid;
}

void TEditAnnounceDialog::Assign()
{
	deliverer->SetS(obtain_deliverer(deliverers, uclp->L(), street->L(), adrNo->S(), blockNo->S()));
}

void TEditAnnounceDialog::StreetCBNSelChange()
{
	Assign();
}

void TEditAnnounceDialog::AdrNoKillFocus()
{
	Assign();
}

void TEditAnnounceDialog::BlockNoKillFocus()
{
	Assign();
}

void TEditAnnounceDialog::UCLPCBNSelChange()
{
	street->Reset(0);
}

void TEditAnnounceDialog::SetComposition()
{
	// 2010:187 FIX: used in valid/OK, always init
	write = true;
	cause.clear();

	if (!subpoena)
	{
		const char *s;
		TLawsuit lawsuit;

		for (s = TYPE_LAWSUITS; *s; s++)
			if (type2ConnectKind(*s) == kind->C())
				break;

		lawsuit.key.type = *s;
		lawsuit.key.no = no->L();
		lawsuit.key.year = year->L();

		if (Announce()->court == COURT_CODE && lawsuit.key.type && lawsuit.key.no && lawsuit.key.year &&
			lawsuit.Try("F_COMPOSITION"))
		{
			composition->EnableWindow(false);
			write = can_access_exact(&lawsuit, true, true, &cause);

			if (write)
				composition->SetL(lawsuit.composition);
			else
				composition->SetL(0);
		}
		else if (lawsuit.key.type)
			composition->EnableWindow(true);
		else
		{
			composition->EnableWindow(false);
			composition->SetL(0);
		}

		if (!composition->IsWindowEnabled() && !GetFocus())
			subpoenaKind->SetFocus();
	}
}

void TEditAnnounceDialog::NoKillFocus()
{
	SetComposition();
}

void TEditAnnounceDialog::YearKillFocus()
{
	SetComposition();
}

void TEditAnnounceDialog::KindCBNSelChange()
{
	subpoenaKind->SetXCrit(kind->C() == CONNECT_KIND_REGION_REQUEST ? SUBPOENA_KIND_GENERIC_NON_WRITS : SUBPOENA_KIND_GENERICS);
	SubpoenaKindCBNSelChange();
	SetComposition();
}
