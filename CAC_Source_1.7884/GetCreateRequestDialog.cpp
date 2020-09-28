//{{TGetCreateRequestDialog Implementation}}

TGetCreateRequestDialog::TGetCreateRequestDialog(TWindow* parent, TRequest *request, int resId)
:
	TGetGroupDialog(parent, request, NULL, TYPE_REQUESTS, NULL, NULL, request_lawable, 0, resId)
{
}

bool TGetCreateRequestDialog::IsValid()
{
	bool isValid = TGetGroupDialog::IsValid();

	*Group() << this;

	if (isValid && (isValid = Group()->Try()) == false)
	{
		// 2009:280 former bloat create_inreg_request()
		TInReg inReg;

		inReg.key.no = no->L();
		inReg.key.year = year->L();

		// 2007:221 LPR: lawsuit type check
		if (!inReg.Try() || inReg.kind != KIND_PRIVATE_CONNECT_APPEAL || !inReg.lawsuit.type ||
			!strchr(TYPE_LAWSUITS, inReg.lawsuit.type)
		)
		{
			say_no_data(Group()->Name);
		}
		else if (ask("Няма такава преписка. Желаете ли да образувате дело по %s %ld/%ldг?", Kind->Seek(inReg.kind),
			inReg.key.no, inReg.key.year))
		{
			// using container to avoid indexing
			TCreateRequest request;

			request << this;
			request.kind = KIND_PRIVATE_CONNECT_INDOC;
			request.date = inReg.date;
			request.college = TYPE_ADMIN_LAW;
			request.time = inReg.time;
			Kind->Seek(request.kind);
			request.lawKind = *Kind->lawKinds;

			if (request.Insert())
			{
				// that's enough to create lawsuit
				isValid = true;

				// create side(s)
				TSideWind sideWind;
				msql m;

				sideWind << inReg;
				sideWind.FloatCriteria(m);
				TQuery q(str(m));
				while (q.Read())
				{
					sideWind << q;
					if (sideWind.state == STATE_APPEALER)
					{
						sideWind << request;
						sideWind.involvement = INVOLVEMENT_INAPPEALER;
						sideWind.state = STATE_NONE;
						sideWind.Insert();
					}
				}

				// create connect
				TConnect connect;

				connect << request;
				sprintf(connect.connectNo, "%ld", inReg.lawsuit.no);
				connect.connectYear = inReg.lawsuit.year;
				connect.connectType = CONNECT_TYPE_LOWER_INSTANCE;
				connect.connectKind = KIND_ADMINISTRATIVE_LAWSUIT;
				connect.ucnType = UCN_INSTANCE_SELF;
				strcpy(connect.ucn, INSTANCE_SELF);
				connect.Insert();
			}
		}
	}

	return isValid;
}
