DEFINE_RESPONSE_TABLE1(TSelectUCNMultiDialog, TSelectUCNBaseDialog)
//{{TSelectUCNMultiDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_COMMAND_ENABLE(IDOK, OKBNEnable),
	EV_BN_CLICKED(IDSEARCH, SearchBNClicked),
	EV_BN_CLICKED(IDADDRESS, AddressBNClicked),
	EV_COMMAND_ENABLE(IDADDRESS, AddressBNEnable),
	EV_BN_CLICKED(IDC_SELECT_UCN_RESEARCH, ResearchBNClicked),
//{{TSelectUCNMultiDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSelectUCNMultiDialog Implementation}}

TSelectUCNMultiDialog::TSelectUCNMultiDialog(TWindow* parent, const char *ucnTypes, bool tAllowEmpty,
	const char *involvements, ucn_multi_action tAction)
:
	TSelectUCNBaseDialog(parent, &ucnGroup, ucnTypes, involvements, false, false, IDD_SELECT_UCN_MULTI),
		allowEmpty(tAllowEmpty), action(tAction)
{
}

bool TSelectUCNMultiDialog::UCNAvailable()
{
	return false;
}

void TSelectUCNMultiDialog::OKBNClicked()
{
	if (IsValid())
	{
		TGArray<TUCNGroup> tUCNGroups;
		groups->GetMark(&tUCNGroups);

		if (tUCNGroups.Count())
		{
			// 2013:186 FIX: remove multi old duplicates
			for (int i = 0; i < tUCNGroups.Count() - 1; i++)
			{
				TUCNGroup *tUCNGroup = tUCNGroups[i];

				for (int n = i + 1; n < tUCNGroups.Count(); )
				{
					if (!strcmp(tUCNGroup->ucn, tUCNGroups[n]->ucn) && tUCNGroup->ucnType == tUCNGroups[n]->ucnType)
					{
						tUCNGroups.Remove(n);
						break;	// max 1 duplicate
					}
					else
						n++;
				}
			}
		}
		else
		{
			TUCNGroup *tUCNGroup = TNamedGroup::NewXGroup(lastUXNType);
			*tUCNGroup << this;
			tUCNGroups.Add(tUCNGroup);
		}

		action(this, &tUCNGroups);
	}
}

void TSelectUCNMultiDialog::OKBNEnable(TCommandEnabler &tce)
{
	tce.Enable(allowEmpty || groups->GetSelCount() >= 1);
}

void TSelectUCNMultiDialog::SearchBNClicked()
{
	Search(false);
}

void TSelectUCNMultiDialog::AddressBNClicked()
{
	int index;

	groups->GetSelIndexes(&index, 1);
	view_addresses(this, groups->UCNGroupAt(index));
}

void TSelectUCNMultiDialog::AddressBNEnable(TCommandEnabler &tce)
{
	tce.Enable(groups->GetSelCount() == 1);
}

void TSelectUCNMultiDialog::ResearchBNClicked()
{
	Search(true);
}
