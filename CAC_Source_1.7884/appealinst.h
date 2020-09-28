struct TAppealInstance
{
	const char *types;
	const char *kinds;
	const char *vz0;
	const char *vz_res;
	const char *ks0;
	const char *ks1;
	const char *ks_res;
	long flags;

	bool KSRecv(const char *receiver) const;	// 2012:079 LPR: method
};

const TAppealInstance *obtain_appeal_instance(char type, char kind, char decisionKind);
const TAppealInstance *obtain_appeal_instance(const TInReg *inReg);
bool fill_appeal_fixeivers(const TInReg *inReg, TFixeiver &fixeiver, char *receiver);

#if TESTVER
void dump_appeal_instances();
#endif  // TESTVER
