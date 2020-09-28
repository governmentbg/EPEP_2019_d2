DEFINE_RESPONSE_TABLE1(TSubpoenaVariantDialog, TPostalDialog)
//{{TSubpoenaVariantDialogRSP_TBL_BEGIN}}
	EV_LBN_SELCHANGE(IDC_SUBPOENA_VARIANT_SUBPOENA_KIND, SubpoenaKindLBNSelChange),
	EV_EN_UPDATE(IDC_SUBPOENA_VARIANT_SUBPOENA_DATE, SubpoenaDateENUpdate),
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TSubpoenaVariantDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSubpoenaVariantDialog Implementation}}
TSubpoenaVariantDialog::TSubpoenaVariantDialog(TWindow *parent, TSubpoenaVariantGroup *variant, const char *tSubpoenaKinds,
	bool tAllowLinks)
:
	TPostalDialog(parent, variant, IDD_SUBPOENA_VARIANT), subpoenaKinds(tSubpoenaKinds), allowLinks(tAllowLinks),
		linkedWinds(NULL), validDateActive(false)
{
	subpoenaKindLabel = new TStatic(this, IDC_SUBPOENA_VARIANT_SUBPOENA_KIND_LABEL);
	subpoenaKind = new TCharListFace(this, IDC_SUBPOENA_VARIANT_SUBPOENA_KIND, "F_SUBPOENA_KIND", SubpoenaKind,
		subpoenaKinds, SIZE_OF_SUBPOENA_KINDS);
	info = new TStatic(this, IDC_SUBPOENA_VARIANT_INFO);
	linksLabel = new TStatic(this, IDC_SUBPOENA_VARIANT_LINKS_LABEL);
	// LPR: modifying attributes of resource controls does not work
	linkedWinds = singleSelect = new TSelectLinkedWindsListBox(this, IDC_SUBPOENA_VARIANT_LINKS_SINGLE, &links);
	multpleSelect = new TSelectLinkedWindsListBox(this, IDC_SUBPOENA_VARIANT_LINKS_MULTIPLE, &links);
	subpoenaDateLabel = new TStatic(this, IDC_SUBPOENA_VARIANT_SUBPOENA_DATE_LABEL);
	subpoenaDate = new TDateFace(this, IDC_SUBPOENA_VARIANT_SUBPOENA_DATE, "F_SUBPOENA_DATE", true);
	validDateLabel = new TStatic(this, IDC_SUBPOENA_VARIANT_VALID_DATE_LABEL);
	validDate = new TDateFace(this, IDC_SUBPOENA_VARIANT_VALID_DATE, "F_VALID_DATE", false);
}

const char *TSubpoenaVariantDialog::SubpoenaDateName()
{
#if COURT_TYPE == COURT_REGIONAL
	if (subpoenaKind->C() == SUBPOENA_KIND_EUHER_COPY)
		return "Дата на издаване";
#endif  // REGIONAL
	return "Дата";
}

bool TSubpoenaVariantDialog::IsValid()
{
	bool isValid = TPostalDialog::IsValid();

	if (isValid)
	{
		char tSubpoenaKind = subpoenaKind->C();

		if (!tSubpoenaKind)
			isValid = error("Не е избран вид уведомяване.");
		else if (linkedWinds == singleSelect && links.Count() && !linkedWinds->Group())
			isValid = error("Не е избран начин на уведомяване.");
		// 2017:096 LPR/IRQ/TRQ: too many conflicts, disabled
		//else if (subpoenaDate->IsWindowEnabled() && subpoenaDate->Date() > Today)
		//	isValid = error("Датата е по-голяма от текущата.");
		//else if (subpoenaDate->IsWindowEnabled() && subpoenaDate->Date() < Variant()->sideWind->date)
		//	isValid = error("Датата е по-малка от тази на събитието.");
		else
		{
			TGArray<TLinkWind> tLinks;
			linkedWinds->GetMark(&tLinks);

			bool notification = Variant()->notifySelect != NOTIFY_SELECT_LEGAL_AID &&
				Variant()->notifySelect != NOTIFY_SELECT_EXEC_LIST;

			for (int i = 0; i < tLinks.Count() - 1; i++)
			{
				TLinkWind *linkWind = tLinks[i];

				for (int n = i + 1; n < tLinks.Count(); n++)
				{
					TLinkWind *tLinkWind = tLinks[n];
				#if 20170289
					bool ucnEQ = !strcmp(linkWind->ucn, tLinkWind->ucn) && linkWind->ucnType == tLinkWind->ucnType;
					bool duplicate = ucnEQ;

					if (!TNotifying::AlternateInsert(Variant(), tSubpoenaKind) && ucnEQ &&
						linkWind->involvement == tLinkWind->involvement)
					{
						duplicate = true;
					}
					else if (notification || tSubpoenaKind == SUBPOENA_KIND_EXEC_COUNTRY)
						duplicate = (*linkWind == *tLinkWind);

					if (duplicate)
					{
						mstr m;

						linkWind->PrintNames(m);
						isValid = error(notification ? "Уведомявате %s два или повече пъти." :
							"Лицето %s е избрано два или повече пъти.", str(m));
						break;
					}
				#else  // 20170289
					if (notification || tSubpoenaKind == SUBPOENA_KIND_EXEC_COUNTRY ? *linkWind == *tLinkWind :
						!strcmp(linkWind->ucn, tLinkWind->ucn) && linkWind->ucnType == tLinkWind->ucnType)
					{
						mstr m;

						linkWind->PrintNames(m);
						isValid = error(notification ? "Уведомявате %s два или повече пъти" :
							"Лицето %s е избрано два или повече пъти.", str(m));
						break;
					}
				#endif  // 20170289
				}

				if (!isValid)
					break;
			}
		}
	}

	return isValid;
}

void TSubpoenaVariantDialog::SetupWindow()
{
	TPostalDialog::SetupWindow();

	if (subpoenaKind->GetCount() == 1)
		subpoenaKind->SetSelIndex(0);

	SubpoenaKindLBNSelChange();
}

void TSubpoenaVariantDialog::SubpoenaKindLBNSelChange()
{
	bool postal = false;	// 2009:348
	char tSubpoenaKind = subpoenaKind->C();

	if (tSubpoenaKind)
	{
		TSubpoenaVariantGroup *variant = Variant();
		TSelectLinkedWindsListBox *hiddenWinds;

		variant->subpoenaKind = tSubpoenaKind;
		variant->FillLinks(&links, allowLinks);

		if (variant->notifySelect == NOTIFY_SELECT_SINGLE_LINK || variant->notifySelect == NOTIFY_SELECT_LAWER_SINGLE)
		{
			hiddenWinds = multpleSelect;
			linkedWinds = singleSelect;
		}
		else
		{
			hiddenWinds = singleSelect;
			linkedWinds = multpleSelect;
		}

		hiddenWinds->ShowWindow(SW_HIDE);
		hiddenWinds->EnableWindow(false);
		linkedWinds->UpdateSelect(variant->notifySelect);
		linkedWinds->EnableWindow(true);
		linkedWinds->ShowWindow(SW_SHOW);

		if (variant->notifySelect == NOTIFY_SELECT_EXEC_LIST && tSubpoenaKind != SUBPOENA_KIND_EXEC_COUNTRY)
			linkedWinds->SetSel(0, true);

		SetCaption(linkedWinds == singleSelect ? "Вид и начин на уведомяване" : "Вид уведомяване и лица");
		linksLabel->SetCaption(LinkLabels[variant->notifySelect]);

		SubpoenaKind->Seek(tSubpoenaKind);
		info->SetCaption(SubpoenaKind->info);
		postal = SubpoenaKind->PostalAllowed();

		// 2017:213 LPR: support for MayEnterDate()
		validDateActive = SubpoenaKind->EnterValidDate();
		bool enterDate = validDateActive;

		if (can_back_date())	// 2017:059
		{
			enterDate |= (SubpoenaKind->genericKind || strchr(SUBPOENA_KIND_EXEC_LISTS, tSubpoenaKind) ||
				SubpoenaKind->proc == SK_PROC_LETTER_TO) && any(SubpoenaKind->htemplate);
		}

		if (enterDate)
		{
			subpoenaDateLabel->SetCaption(SubpoenaDateName());
			subpoenaDateLabel->ShowWindow(SW_SHOW);
			subpoenaDate->EnableWindow(true);
			subpoenaDate->ShowWindow(SW_SHOW);
		}
		else
		{
			subpoenaDateLabel->ShowWindow(SW_HIDE);
			subpoenaDate->ShowWindow(SW_HIDE);
			subpoenaDate->SetDate(Today);
			subpoenaDate->EnableWindow(false);
		}

		if (SubpoenaKind->EnterValidDate())
		{
			validDateLabel->ShowWindow(SW_SHOW);
			validDate->EnableWindow(true);
			validDate->ShowWindow(SW_SHOW);
			SubpoenaDateENUpdate();
		}
		else
		{
			validDateLabel->ShowWindow(SW_HIDE);
			validDate->ShowWindow(SW_HIDE);
			validDate->SetDate(Today);
			validDate->EnableWindow(false);
		}
	}

	for (int i = 0; i < IDC_POSTAL_INDEX_COUNT; i++)
		buttons[i]->EnableWindow(postal);

	if (!postal)
	{
		buttons[IDC_POSTAL_INDEX_NONE]->SetCheck(BF_CHECKED);
		buttons[IDC_POSTAL_INDEX_COURIER]->SetCheck(BF_UNCHECKED);
		buttons[IDC_POSTAL_INDEX_FAX]->SetCheck(BF_UNCHECKED);
		buttons[IDC_POSTAL_INDEX_EMAIL]->SetCheck(BF_UNCHECKED);
	}

	if (!links.Count())
		linkedWinds->EnableWindow(false);
}

void TSubpoenaVariantDialog::SubpoenaDateENUpdate()
{
	if (!validDateActive)
		validDate->SetDate(CDate());
#if COURT_TYPE == COURT_REGIONAL
	else if (subpoenaKind->C() == SUBPOENA_KIND_EUHER_COPY)
	{
		CDate euherValid = subpoenaDate->Date();

		if (euherValid.Empty())
			euherValid = Today;

		euherValid.AddMonths(6);
		validDate->SetDate(euherValid);
	}
#endif  // REGIONAL
}

void TSubpoenaVariantDialog::OKBNClicked()
{
	if (IsValid())
	{
		TSubpoenaVariantGroup *variant = Variant();
		*variant << this;

		if (linkedWinds == singleSelect && links.Count())
			variant->preLink << linkedWinds->Group();
		else
		{
			variant->preLink << variant->sideWind;
			linkedWinds->GetMark(&variant->links);

			if (variant->notifySelect == NOTIFY_SELECT_LEGAL_AID)
			{
				variant->laids.Flush();

				while (variant->links.Count())
				{
					TLegalSide *laid = new TLegalSide;

					*laid << variant->links[0];
					laid->TryAddress();
					variant->laids.Add(laid);
					variant->links.Remove(0);
				}
			}
			else if (variant->notifySelect == NOTIFY_SELECT_EXEC_LIST)
			{
				for (int i = 0; i < variant->links.Count(); i++)
				{
					TExecLink *xlink = new TExecLink();

					*xlink << variant->links[i];
					xlink->TryAddress();
					variant->links.Remove(i);
					variant->links.Insert(xlink, i);
				}
			}
		}

		CmOk();
	}
}
