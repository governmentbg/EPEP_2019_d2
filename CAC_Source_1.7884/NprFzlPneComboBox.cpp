//{{TNprFzlPneComboBox Implementation}}

TNprFzlPneComboBox::TNprFzlPneComboBox(TWindow* parent, int resourceId, FZL *tFZL, TXArray<NPRFZLPNE> *tAllLinks, NPR *tNpr)
:
	TGroupComboBox(parent, resourceId, new TGArray<NPRFZLPNE>, 0x100), fzl(tFZL), allLinks(tAllLinks), npr(tNpr),
		exclude(npr->dlo->nprfzlpne)
{
}

void TNprFzlPneComboBox::GetGroupText(mstr &m, TGroup *group, int)
{
	print_nprfzlpne(m, group, npr);
}

TGroup *TNprFzlPneComboBox::NewGroup() const
{
	return exclude->NewObject();
}

void TNprFzlPneComboBox::LoadData(const char *)
{
	for (int i = 0; i < allLinks->Count(); i++)
	{
		NPRFZLPNE *nprfzlpne = (*allLinks)[i];

		if (nprfzlpne->fzlsid == fzl->sid)
		{
			int n;

			for (n = 0; n < exclude->Count(); n++)
			{
				NPRFZLPNE *tNprFzlPne = (*exclude)[n];

				if (tNprFzlPne->fzlsid == fzl->sid && tNprFzlPne->pnesid == nprfzlpne->pnesid)
					break;
			}

			if (n == exclude->Count())
			{
				NPRFZLPNE *tNprFzlPne = (NPRFZLPNE *) NewGroup();
				*tNprFzlPne << nprfzlpne;
				InsertGroup(tNprFzlPne);
			}
		}
	}
}

void TNprFzlPneComboBox::Refresh()
{
	TGroupComboBox::Refresh();

	if (array->Count())
		SetVisualIndex(0);
}
