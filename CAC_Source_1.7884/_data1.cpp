class TAbolishedLoaderThread : public TCheckupThread
{
	public:
		TAbolishedLoaderThread(const collCompBegDEndDBegN & cCBDEDBN,
			TGSArray<BriefSessionData>& arr);
	protected:
		const collCompBegDEndDBegN& rCCBDEDBN;
		TGSArray<BriefSessionData>& rArr;
		virtual void Execute();
};

TAbolishedLoaderThread::
TAbolishedLoaderThread(const collCompBegDEndDBegN& cCBDEDBN,
	TGSArray<BriefSessionData>& arr) :
		TCheckupThread(0, 0), rCCBDEDBN(cCBDEDBN), rArr(arr)
{
}

void TAbolishedLoaderThread::Execute()
{
	try
	{  // -- 1 --
		static const char* const loadingStr =
			"Зареждане на отмени на хода по същество....";

		{  // -- 2 --
			CharPtr oSRCriteria(256 + 128 + 64);
			CharPtr aCriteria(256 + 64);

			if(composeSessionsByResultsCriteria(oSRCriteria, oSRCriteria.Size(),
					rCCBDEDBN, RESULT_FINALIZEDS) &&
				composeAbolishedProgressCriteria(aCriteria, aCriteria.Size(),
					rCCBDEDBN))
			{
				totalPartSteps = 3;
				reportFinishedItems = false;

				{
					wtdl->SetHeader(loadingStr);
					startingVal = 0;
					endingVal = barHiVal;
					passeds = "Стъпка";
					Tick(false);
				}

				TCountRecsQuery oSRQ(oSRCriteria);

				Tick(false);

				TCountRecsQuery aQ(aCriteria);

				Tick(false);

				SessPtr pSess;

				while(oSRQ.Read())
				{
					pSess.Ref() << oSRQ;
					if(sessResOverridden_AbolishProgr(pSess.Ref(), &aQ))
					{
						BriefSessionData* pNewT = new BriefSessionData();

						(*pNewT) << pSess.Ref();
						rArr.Add(pNewT);
					}
				}

				Tick(true);
				wtdl->AlmostDone();
			}
			else
				error("Неуспешно \"%s\"", loadingStr);
		}  // -- 2 --
	}  // -- 1 --
	CATCH_ANYTHING
}

overriddenResultsSessionsBS::overriddenResultsSessionsBS(TWindow *, const collCompBegDEndDBegN& cCBDEDBN)
{
	TAbolishedLoaderThread(cCBDEDBN, *this).Generate();
}
