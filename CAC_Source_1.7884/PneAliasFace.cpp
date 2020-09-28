//{{TPneAliasFace Implementation}}

TPneAliasFace::TPneAliasFace(TWindow* parent, int resourceId, TGArray<PNE> *array)
:
	TGroupComboBox(parent, resourceId, array, SIZE_OF_EISPP_NAME)
{
	autoEnable = true;
	loadData = dynamic = false;
}

TPneAliasFace::~TPneAliasFace()
{
	array->Clear();
}

void TPneAliasFace::SetL(long l)
{
	int index;

	for (index = 0; index < array->Count(); index++)
	{
		PNE *pne = (PNE *) (*array)[index];

		if (pne->sid == l)
			break;
	}

	SetVisualIndex(index < array->Count() ? index : 0);
}

TGroup *TPneAliasFace::NewGroup() const
{
	return NULL;
}
