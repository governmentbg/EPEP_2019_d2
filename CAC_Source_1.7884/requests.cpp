#include "cac.h"

#include "RequestSideListDialog.h"
#include "RequestSideListDialog.cpp"

#include "EditRequestDialog.h"
#include "EditRequestDialog.cpp"

// 2003:295 BRW: call LawsuitDialog or FirmLawsuitDialog - public/remove this?
// 2004:056 BUGFIX: bool -> void + related, chain_action() does the exist test
bool edit_request(TWindow *parent, TRequest *request)
{
	char status = request->Status();
	bool result = false;

	if (strchr(RESULT_CLEANS, status) || ask("Документа е със статус %s. Ще го редактирате ли?", Resolution->Seek(status)))
	{
		// 2005:116 LRQ: BUGFIX: handle kind change [new request only]
		TRequest tRequest;
		tRequest << request;

		result = TEditRequestDialog(parent, request, status).Execute() != IDCANCEL;

		if (!result)
			xfer_reject(request);	// 2016:194 FIX: was missing
		else
		{
			if (tRequest.kind != request->kind)
			{
				TSideWind sideWind;
				TLinkWind linkWind;

				sideWind.Transfer(&tRequest, request, NULL);
				linkWind.Transfer(&tRequest, request, NULL);

				sideWind << tRequest;
				sideWind.Delete(false);
				linkWind << tRequest;
				linkWind.Delete(false);
			}

			log(LOG_EDIT, CM_REG_EDIT, request);

			if (!eispp_stsakt(parent, request))
			{
				*request->eispp_nmr = '\0';
				request->Update("F_EISPP_NMR");
			}

			TRequestSideListDialog(parent, request, SIDE_MODE_EDIT).Execute();

			// 2006:081 LPR: xfer any new sides to prove
			TProve prove;
			TSideWind sideWind;

			if (prove.TryBy(request))
				copy_winds(request, &prove, sideWind);
		}
	}

	return result;
}

#include "ResolutionSideListDialog.h"
#include "ResolutionSideListDialog.cpp"

#include "ResoluteRequestDialog.h"
#include "ResoluteRequestDialog.cpp"

bool edit_request_resolution(TWindow *parent, TRequestResolution *resolution)
{
	if (TResoluteRequestDialog(parent, resolution, PrimaryGroup(TRequest), true).Execute() == IDOK)
	{
		// 2004:112 LPR: log resolution data
		log(LOG_EDIT, CM_CLERK_EDIT_REQUEST_RESOLUTION, resolution);
		TResolutionSideListDialog(parent, resolution, PrimaryGroup(TRequest), SIDE_MODE_EDIT).Execute();
		return true;
	}

	return false;
}

#include "NewRequestDialog.h"
#include "NewRequestDialog.cpp"

void new_request(TWindow *parent, const char *kinds, char type)
{
	TDummyContainer container;
	container.key.type = TYPE_REQUEST;

	do
	{
		TRequest *const request = new TRequest;
		TVirtPtr VP(request);

		*request << container;

		if (TNewRequestDialog(parent, request, kinds, type).Execute() == IDCANCEL)
		{
			xfer_reject(request);
			break;
		}

		log(LOG_NEW_DOC, CM_REG_NEW, request);

		if (!eispp_stsakt(parent, request))
		{
			*request->eispp_nmr = '\0';
			request->Update("F_EISPP_NMR");
		}

		TRequestSideListDialog(parent, request, SIDE_MODE_ADD).Execute();
		chain_action(parent, request, next_new_lawsuit);

		container << request;
		container.key.no = 0;

	} while (REPEAT_REQUESTS);
}

void new_request_resolution(TWindow *parent, TRequest *request)
{
	TRequestResolution resolution;

	resolution.key = request->key;
	if (TResoluteRequestDialog(parent, &resolution, request, false).Execute() == IDOK)
	{
		// 2004:112 VRQ: log request data
		log_event(LOG_ENTER, CM_CLERK_NEW_REQUEST_RESOLUTION, &resolution);
		// 2003:290 BRW: switched to side winds
		TResolutionSideListDialog(parent, &resolution, request, SIDE_MODE_ADD).Execute();
	}
}

void revert_source(TLawsuit *lawsuit)
{
	TRequestResolution resolution;

	resolution.key = lawsuit->source;
	resolution.date = lawsuit->date;

	if (resolution.Try() && resolution.Status() == RESOLUTION_LAWED)
	{
		// 2007:185 LPR: lawed + assigned support
		if (atob(resolution.assign))
		{
			// 2007:257 FIX: was missing
			if (resolution.eventNo)
			{
				DecisionIndex->type = resolution.IndexType();
				DecisionIndex->kind = resolution.IndexKind();
				DecisionIndex->year = resolution.date.year;
				DecisionIndex->eventNo = resolution.eventNo;
				DecisionIndex->Delete(true);
				resolution.eventNo = 0;
			}

			const char *s;

			if ((s = resolution.text.rchr('\n')) != NULL)
				resolution.text.cut(s);
			resolution.resolution = RESOLUTION_ASSIGNED;
			if (!resolution.Update("F_RESOLUTION, F_TEXT, F_EVENT_NO"))
				error("Разпореждането за образуване на делото не е изтрито!");
		}
		else if (!delete_resolution(&resolution))
			error("Разпореждането за образуване на делото не е изтрито!");
	}
	else
		error("Няма достъп до разпореждането за образуване на делото.");
}

void subvert_source(const TRequest *request, TLawsuit *lawsuit)
{
	// BUGFIX: delete the resolutions after LAWED date
	TRequestResolution resolution;
	// 2007:180 +related: ASSIGNED support
	TRequestResolution tResolution;
	msql m(resolution.Name);

	m.Add(request, "F_NO, F_YEAR, F_TYPE");
	m.Add("F_DATE");
	m.printf(">= %ld", lawsuit->date.Value());
	TQuery q(str(m));

	while (q.Read())
	{
		tResolution << q;
		// 2009:289 LPR: Today -> lawsuit->date
		if (tResolution.date == lawsuit->date && tResolution.resolution == RESOLUTION_ASSIGNED)
			resolution << tResolution;
		else
		{
			info("Разпореждане \"%s\" от %D по входящ документ %ld/%ldг. ще бъде изтрито.",
				Resolution->Seek(tResolution.resolution), &tResolution.date, tResolution.key.no,
				tResolution.key.year);
			tResolution.Delete(true);
		}
	}

	resolution.key = request->key;

	if (atob(resolution.assign))
		resolution.text.printf("\n%s %ld/%ld", Kind->Seek(lawsuit->kind), lawsuit->key.no, lawsuit->key.year);
	else
	{
		resolution.text.clear();
		resolution.text.printf("%s %ld/%ld", Kind->Seek(lawsuit->kind), lawsuit->key.no, lawsuit->key.year);
	}

	resolution.resolution = RESOLUTION_LAWED;
	resolution.date = lawsuit->date;
	if (atob(resolution.assign))
	{
		resolution.IndexEvent();	// 2007:257 FIX: was missing
		resolution.Update();
	}
	else
		resolution.Insert();
}

bool renumber_request(TWindow *parent, TRequest *source)
{
	TRequest target;
	target << source;

	if (check_subpoenas(source) && TRenumberDocDialog(parent, &target).Execute() == IDOK)
	{
		// 2016:259 LPR: added e-portal support
		if (source->key.no != target.key.no || source->key.year != target.key.year)
		{
			renumber_doc(source, &target);
			eportal_send(source, LOG_DELETE);
		}
		else if (source->date != target.date)
		{
			target.Update("F_DATE");
			move_winds(source, &target);
		}

		eportal_send(&target, LOG_WRITE);
		return true;
	}

	return false;
}
