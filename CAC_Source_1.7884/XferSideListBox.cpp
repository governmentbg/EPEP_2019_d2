//{{TXferSideListBox Implementation}}

TXferSideListBox::TXferSideListBox(TWindow* parent, int resourceId, TXferDocument *tDocument, TGSArray<TXferSide> *tArray, const char *tInvolvements)
:
	TGroupListBox(parent, resourceId, tArray), document(tDocument), involvements(tInvolvements)
{
	loadData = dynamic = false;
}

void TXferSideListBox::ApplyInvolvement(char involvement)
{
	if (involvement != INVOLVEMENT_INTERNAL)
		for (int index = 0; index < array->Count(); index++)
			((TXferSide *) (*array)[index])->involvement = involvement;
}

void TXferSideListBox::LoadSides(TGroup *(*tClone)())
{
	msql m;
	TXferSide *side = (TXferSide *) tClone();

	side->key = document->key;
	Criteria(m, side);

	TQuery q(str(m));

	while (q.Read())
	{
		*side << q;
		side->Init(involvements);
		InsertGroup(side);
		side = (TXferSide *) tClone();
	}

	delete side;
}

TGroup *TXferSideListBox::NewGroup() const
{
	return NULL;
}

void TXferSideListBox::Refresh()
{
	LoadSides(TXferSideCitizen::Clone);
	LoadSides(TXferSideFirm::Clone);

	TGroupListBox::Refresh();
}
