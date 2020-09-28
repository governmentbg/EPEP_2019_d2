enum
{
	DBIMAXNAMELEN = 31,
	DBIMAXSCFLDLEN = 128
};

typedef CHAR DBINAME [DBIMAXNAMELEN + 1];
typedef BYTE *pBYTE;
typedef UINT16 DBIResult;

typedef struct
{
	UINT16	iFldNum;
	DBINAME	szName;
	UINT16	iFldType;
	UINT16	iOffset;
	UINT16	iLen;
	UINT16	iUnUsed[2];
} FLDDesc, *pFLDDesc;
