//{{TNIATDepAliasFace Implementation}}

TNIATDepAliasFace::TNIATDepAliasFace(TWindow* parent, int resourceId, const char *name, long flags,
	TGArray<TReceiver> *receivers)
:
	TSenderAliasFace(parent, resourceId, name, UCN_NIA_TDEP, flags, receivers)
{
}

void TNIATDepAliasFace::SelectElement(const char *ucn, char ucnType)
{
	TAddress address;

	for (int index = 0; index < array->Count(); index++)
	{
		TReceiver *receiver = TYPECHECK_PTR_CAST(TReceiver, (*array)[index]);
		bool match = false;

		if (ucnType)
		{
			if (!strcmp(receiver->ucn, ucn) && receiver->ucnType == ucnType)
				match = true;
		}
		else
		{
			address.ucnType = receiver->ucnType;
			strcpy(address.ucn, receiver->ucn);

			if (address.Try("F_UCLP"))
			{
				UCLP->Seek(address.uclp);

				if (UCLP->area == DEFAULT_AREA)
					match = true;
			}
		}

		if (match)
		{
			SetVisualIndex(index);
			break;
		}
	}
}
