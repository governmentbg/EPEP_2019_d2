#include "common.h"

// ----- TVoidPtr ------------------------------------------------------------
TVoidPtr::~TVoidPtr()
{
	delete ptr;
}

// ----- TVirtPtr ------------------------------------------------------------
TVirtPtr::~TVirtPtr()
{
	delete (TVObject *) ptr;
}

// ----- TCharPtr ------------------------------------------------------------
TCharPtr::~TCharPtr()
{
	delete[] (char *) ptr;
}

// ----- TBoolPtr ------------------------------------------------------------
TBoolPtr::~TBoolPtr()
{
	delete[] (bool *) ptr;
}
