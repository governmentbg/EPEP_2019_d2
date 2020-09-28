//{{TXferDocumentListBox Implementation}}

TXferDocumentListBox::TXferDocumentListBox(TWindow* parent, int resourceId, const TRequest *tRequest)
:
	TCloneListBox(parent, resourceId, NULL, TXferDocument::Clone), request(tRequest)
{
}

void TXferDocumentListBox::Criteria(msql &m, TGroup *)
{
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	long types = 0;

	if (request->college == TYPE_PUNISHMENT_LAW)
		types |= XFER_TYPES_PUNISHMENT;
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

#if COURT_TYPE == COURT_ADMIN
	long types = XFER_TYPES_ALL;
#endif  // ADMIN

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	if (strchr(TYPE_CITIZALIKE_LAWS, request->college))
		types |= XFER_TYPES_CITIZEN;
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	if (strchr(TYPE_CITIZALIKE_LAWS, request->college))
		types |= XFER_TYPES_FIRM;
#endif  // APPEAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_AREAL
	if (request->college == TYPE_FIRM_LAW)
		types |= XFER_TYPES_FIRM;
#endif  // AREAL

	// 2014:210 FB2: FloatCriteria() + nested select -> join
	m.Begin("T_XFER_DOCUMENT X JOIN T_XFER_KIND K ON K.F_XFER_KIND = X.F_XFER_KIND", "X.*");
	m.AddLong("X.F_IO", XFER_RECEIVE);
	m.AddChars("X.F_XFER_TYPE", XFER_TYPE_INCOMINGS);
	m.AddFlags("K.F_TYPES", types, -1, XFER_TYPES_ALL);
	m.AddOrder("X.F_COURT_SENDER, X.F_XFER_TYPE, X.F_YEAR, X.F_NO");
}

void TXferDocumentListBox::SetupWindow()
{
	TCloneListBox::SetupWindow();

	if (request->xfer)
	{
		int index;

		for (index = 0; index < array->Count(); index++)
		{
			const TXferDocument *document = (const TXferDocument *) (*array)[index];

			if (document->key.xferKind == request->xfer->key.xferKind && document->key.no == request->xfer->key.no &&
				document->key.year == request->xfer->key.year &&
				document->key.courtSender == request->xfer->key.courtSender)
			{
				break;
			}
		}

		if (index < array->Count())
			SetVisualIndex(index);
	}
}
