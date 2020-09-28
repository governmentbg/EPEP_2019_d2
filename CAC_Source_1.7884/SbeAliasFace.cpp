//{{TSbeAliasFace Implementation}}

TSbeAliasFace::TSbeAliasFace(TWindow* parent, int resourceId, const char *name, TGArray<SBE> *tExclude, NPR *npr,
	TIntegerArray *tInclude)
:
	TEisAliasFace(parent, resourceId, name, nmk_sbevid, npr), exclude(tExclude),
		fzlCount(npr->dlo->fzl->Count()), include(tInclude)
{
	initialRefresh = true;
}

void TSbeAliasFace::Refresh()
{
	lset.Clear();

	for (int i = 0; i < include->Count(); i++)
	{
		long vid = (*include)[i];
		int count = 0;
		int limit = EisVal->Contains(nmk_sbeednfzl, vid) ? fzlCount : 1;

		for (int n = 0; n < exclude->Count(); n++)
			if ((*exclude)[n]->vid == vid)
				count++;

		if (count < limit)
			lset.Add(vid);
	}

	if (!lset.Count())
		lset.Add(0);

	TEisAliasFace::Refresh();
}
