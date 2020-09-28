void eportal_query_access(TWindow *parent, const char *criteria, TEPortGuid *group)
{
	TGArray<TEPortGuid> guids;
	TWaitWindow wait(parent, WAIT);

	TEPortGuid().LoadArray(guids, criteria);

	TTextFile tf;
	FILE *f = tf.OpenTemp();

	if (guids.Count())
	{
		for (int i = 0; i < guids.Count(); i++)
		{
			TEPortGuid *guid = guids[i];

			fprintf(f, "%c %ld/%ld %s %d\n", guid->key.type, guid->key.no, guid->key.year, guid->ucn,
				einvoke_granted(guid->key, guid->ucnType, guid->ucn, guid->involvement));
		}
	}
	else if (atob(group->ucn))
		fprintf(f, "%s\niyia\n", group->ucn);
	else if (group->key.no)
		fprintf(f, "%c %ld/%ld\niyia\n", group->key.type, group->key.no, group->key.year);

	tf.Show(&wait);
}

