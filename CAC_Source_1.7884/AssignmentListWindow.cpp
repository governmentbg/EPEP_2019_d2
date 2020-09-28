DEFINE_RESPONSE_TABLE1(TAssignmentListWindow, TDoubleListWindow)
//{{TAssignmentListWindowRSP_TBL_BEGIN}}
	EV_WM_LBUTTONDOWN,
	EV_WM_RBUTTONDOWN,
//{{TAssignmentListWindowRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TAssignmentListWindow Implementation}}

TAssignmentListWindow::TAssignmentListWindow(TWindow* parent, int resourceId, TGSArray<TAssignment> *array,
	TDistribution *tDistribution, const char *tKinds, int long_width)
:
	TDoubleListWindow(parent, resourceId, array, 392), distribution(tDistribution), kinds(tKinds), images(NULL),
		resolutions(NULL)
{
	TDoubleListWindow::Add(
		new TGroupListColumn("", generic_null, 12, LVCFMT_RIGHT),
		new TGroupListColumn("Дата", "F_DATE", 32, LVCFMT_RIGHT),
		new TGroupListColumn("Вх.№/г.", assignment_no_year, 36, LVCFMT_RIGHT),
		new TGroupListColumn("Предмет", assignment_subject, long_width),
		new TGroupListColumn("Страни", assignment_left, long_width),
		NULL
	);

	surroundIcon = true;
}

TAssignmentListWindow::~TAssignmentListWindow()
{
	delete images;
}

void TAssignmentListWindow::Add()
{
	TRequest request;
	bool result = get_group(this, &request, TYPE_REQUESTS, NULL, resolutions);

	if (result)
	{
		Kind->Seek(request.kind);
		if (!strmatch(Kind->lawKind, kinds))
			result = error("Документът е за образуване в %s.", Kind->Seek(Kind->lawKind));
	}

	if (result)
		for (int index = 0; index < array->Count(); index++)
			if (AssignmentAt(index)->key == request.key)
				result = error("Документът вече е в списъка.");

	if (result)
	{
		TRequestResolution *resolution = request.LastResolution(NULL);

		if (!resolution)
		{
			TAssignment *assignment = new TAssignment;
			assignment->Init(&request);
			InsertGroup(assignment);
		}
		else if (resolution->date > Today ||
			(resolution->date == Today && !strchr(RESOLUTION_PREPASSIGNS, resolution->resolution)))
		{
			error("Документът е с разпореждане \"%s\" от %Dг.", Resolution->Seek(resolution->resolution),
				&resolution->date);
		}
		else if (ask("Документът е с разпореждане \"%s\" от %Dг. Желаете ли да продължите?",
			Resolution->Seek(resolution->resolution), &resolution->date))
		{
			TAssignment *assignment = new TAssignment;
			assignment->Init(resolution, &request);
			InsertGroup(assignment);
		}

		delete resolution;
	}
}

bool TAssignmentListWindow::IsValid()
{
	bool judgeCh = false, subjectCh = false;
	int discard = 0;

	for (int i = 0; i < array->Count(); i++)
	{
		TAssignment *assignment = AssignmentAt(i);

		if (assignment->JudCmp())
			judgeCh = true;

		if (assignment->subject != assignment->orig.subject)
			subjectCh = true;

		if (strchr(RESOLUTION_PREPASSIGNS, assignment->orig.resolution) && !atob(assignment->Judge()))
			discard++;
	}

	if (discard)
		return AskDiscard(discard);

	if (judgeCh)
		return true;

	return ask("Няма промени в разпределението%s. Желаете ли да продължите?", subjectCh ?
		", ще бъдат записани само новите предмети" : " или предметите");
}

void TAssignmentListWindow::DrawSecond(TListWindItem &lwi, TGroup *group)
{
	TAssignment *assignment = (TAssignment *) group;

	lwi.iSubItem = 0;
	lwi.SetText("");
	lwi.SetImageIndex(1);
	SetItem(lwi);

	lwi.mask = 0;

	lwi.iSubItem = 4;
	lwi.SetText(assignment->indocText);
	SetItem(lwi);

	lwi.iSubItem = 5;
	lwi.SetText(str(assignment->right));
	SetItem(lwi);
}

void TAssignmentListWindow::SetupWindow()
{
	TDoubleListWindow::SetupWindow();

	images = new TImageList(TSize(16, 16), ILC_COLOR4 | ILC_MASK, 2, 1);
	images->Add(TIcon(*GetModule(), TResId(IDI_REQUESTRANS)));
	images->Add(TIcon(*GetModule(), TResId(IDI_TRANSPARENT)));
	SetImageList(*images, Small);
}

void TAssignmentListWindow::EvLButtonDown(uint modKeys, TPoint& point)
{
	TLwHitTestInfo info(point);
	int index = HitTest(info);

	TDoubleListWindow::EvLButtonDown(modKeys, point);

	if (index >= 0)
		Edit();
}

void TAssignmentListWindow::EvRButtonDown(uint modKeys, TPoint& point)
{
	TDoubleListWindow::EvLButtonDown(modKeys, point);
}
