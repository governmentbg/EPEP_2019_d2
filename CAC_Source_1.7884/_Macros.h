#if !defined(_APPNAME_MACROS_H)
#define _APPNAME_MACROS_H

#define ENDING_ZERO(a) ((a)[sizeof(a) - 1] = '\0')
#define ZERO_LEN(a) ((a)[0] = '\0')

#if 20150274
#define CATCH_ANYTHING \
	catch(TXBase& e) { error("%s", e.why().c_str()); }\
	catch(...) { error("%s", UNSPECIFIED_EXCEPTION_CAUGHT); }
#else  // 20150274
#define CATCH_ANYTHING \
catch(TXBase& e) { error("%s\nÔאיכ %s, נוה %d", e.why().c_str(), \
__FILE__, __LINE__); }\
catch(...) { error("%s\nÔאיכ %s, נוה %d", UNSPECIFIED_EXCEPTION_CAUGHT, \
__FILE__, __LINE__); }
#endif  // 20150274

#define METHOD_NAME(a) const char* const methodName = (a)
#define FUNC_NAME(a) const char* const funcName = (a)

#define REPORT_M_INVALID_ARGS error("%s\n%s::%s", \
WRONG_ARGS_PASSED_TO, typeid(*this).name(), methodName)

#define REPORT_F_INVALID_ARGS error("%s\n%s", WRONG_ARGS_PASSED_TO, funcName)

#define REPORT_M_UNSUCCESSFUL error("%s::%s\n%s", \
typeid(*this).name(), methodName, UNSUCCESSFUL_OPERATION)

#define REPORT_F_UNSUCCESSFUL error("%s\n%s", funcName, UNSUCCESSFUL_OPERATION)

#define REPORT_M_NOT_IMPLEMENTED info("%s::%s\n%s.", \
typeid(*this).name(), methodName, IS_NOT_IMPLEMENTED)

#define SUPPRESS_UNUSED(a) if(a) {}

#endif  // of _APPNAME_MACROS_H
