DEFINE_RESPONSE_TABLE1(TFzlComboBox, TGroupComboBox)
//{{TFzlComboBoxRSP_TBL_BEGIN}}
	EV_MESSAGE(WM_FACE_VALID, EvFaceValid),
//{{TFzlComboBoxRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TFzlComboBox Implementation}}

TFzlComboBox::TFzlComboBox(TWindow* parent, int resourceId, SBE *tSbe, TXArray<FZL> *tInclude, TGArray<SBE> *tExcludeSbe,
	TXArray<FZL> *tExcludeFzl)
:
	TGroupComboBox(parent, resourceId, new TGArray<FZL>, 0x100), sbe(tSbe), include(tInclude), excludeSbe(tExcludeSbe),
		excludeFzl(tExcludeFzl)
{
}

TGroup *TFzlComboBox::NewGroup() const
{
	return sbe->npr->dlo->fzl->NewObject();
}

void TFzlComboBox::LoadData(const char *)
{
	for (int i = 0; i < include->Count(); i++)
	{
		FZL *fzl = (FZL *) NewGroup();
		bool add = true;

		*fzl << (*include)[i];
		sbe->npr->dlo->fzl->ApplyDescs(fzl);

		if (excludeSbe)
		{
			for (int n = 0; n < excludeSbe->Count(); n++)
			{
				SBE *tSbe = (*excludeSbe)[n];

				if (tSbe->vid == sbe->vid && (*tSbe->npr->dlo->fzl)[0]->sid == fzl->sid)
				{
					add = false;
					break;
				}
			}
		}
		else if (excludeFzl)
		{
			for (int n = 0; n < excludeFzl->Count(); n++)
			{
				FZL *tFzl = (*excludeFzl)[n];

				if (tFzl->sid == fzl->sid)
				{
					add = false;
					break;
				}
			}
		}

		if (add)
			InsertGroup(fzl);
		else
			delete fzl;
	}
}

void TFzlComboBox::Refresh()
{
	TGroupComboBox::Refresh();

	if (array->Count())
		SetVisualIndex(0);
}

long TFzlComboBox::EvFaceValid(TParam1, TParam2)
{
	if (!Group())
	{
		error("Данните за лице по НП са задължителни.");
		return MW_INVALID_FACE;
	}

	return 0;
}
