//{{TEditJudicialDialog Implementation}}

TEditJudicialDialog::TEditJudicialDialog(TWindow* parent, TNamedGroup *ucnGroup, bool edit, bool full, bool tStat,
	int resId)
:
	TEditNamedDialog(parent, ucnGroup, edit, full, resId), stat(tStat), country(NULL)
{
}

bool TEditJudicialDialog::IsValid()
{
	bool isValid = TEditNamedDialog::IsValid();

	if (isValid)
	{
		const char *tBulStat = bulstat->S();

		if (any(tBulStat))
		{
			msql m(UCNGroup()->Name, "F_UCN");

			m.AddString("F_BULSTAT", tBulStat);

			if (edit)
			{
				m.AddString("(NOT F_UCN", UCNGroup()->ucn);
				m.NextOr();
				m.AddChar("NOT F_UCN_TYPE", UCNGroup()->ucnType);
				m.cat(")");
			}

			TQuery q(str(m));

			if (q.Read())
			{
				isValid = ask("Вече съществува юридическо лице с ЕИК/БУЛСТАТ %s. Желаете ли да продължите?",
					tBulStat);
			}
		}
		else if (country && !strcmp(country->S(), BUILDIN_COUNTRY) && stat)
			isValid = error("Полето ЕИК/БУЛСТАТ е задължително.");
	}

	return isValid;
}
