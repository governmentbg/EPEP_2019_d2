# if ! defined (_DEFINES_H)
# define _DEFINES_H

# define FOLLOW_LAWSUIT_TYPES 1

# if INSTANCE
#   if APPEAL
# define AREA_CANCEL_ENACTMENT_ON_FACE 0  // 25.jul.2001
# define FIRM_REPORT_USE_ARRAY 0  // 12.oct.2000
# define FIRM_REPORT_FAST 0  // 19.oct.2000
# define TL_634_MANY_SURR_ONE_SESS 0  // 09.jan.2001
# define UNWRITTEN_ACTS_CALL_JIMMY 0  // 08.feb.2001
# define JUDGE_CHKUP_EXCLUDE_RES_FIXED 0  // 25.jan.2001
# define PUNISHM_2ND_REPORT_12_SEP_2001 1  // 12.sep.2001
#   else  // of APPEAL
# define AREA_CANCEL_ENACTMENT_ON_FACE 1  // 25.jul.2001
# define FIRM_REPORT_USE_ARRAY 0  // 12.oct.2000
# define FIRM_REPORT_FAST 1  // 19.oct.2000
# define TL_634_MANY_SURR_ONE_SESS 1  // 09.jan.2001
# define UNWRITTEN_ACTS_CALL_JIMMY 1  // 08.feb.2001; 2010:284 LPR/LRQ: 1 -> 0
# define JUDGE_CHKUP_EXCLUDE_RES_FIXED 1  // 25.jan.2001
# define PUNISHM_2ND_REPORT_12_SEP_2001 1  // 12.sep.2001
#   endif  // of APPEAL
# else  // of INSTANCE
# define AREA_CANCEL_ENACTMENT_ON_FACE 0  // 25.jul.2001
# define FIRM_REPORT_USE_ARRAY 0  // 12.oct.2000
# define FIRM_REPORT_FAST 0  // 19.oct.2000
# define TL_634_MANY_SURR_ONE_SESS 0  // 09.jan.2001
# define UNWRITTEN_ACTS_CALL_JIMMY 0  // 08.feb.2001
# define JUDGE_CHKUP_EXCLUDE_RES_FIXED 1  // 25.jan.2001
# define PUNISHM_2ND_REPORT_12_SEP_2001 0  // 12.sep.2001
# endif  // of INSTANCE

# if INSTANCE
#   if APPEAL
#     if AREA_CANCEL_ENACTMENT_ON_FACE
#       error AREA_CANCEL_ENACTMENT_ON_FACE must be 0 in the appeal version
#     endif  // of AREA_CANCEL_ENACTMENT_ON_FACE
#     if FIRM_REPORT_USE_ARRAY
#       error FIRM_REPORT_USE_ARRAY must be 0 in the appeal version
#     endif  // of FIRM_REPORT_USE_ARRAY
#     if FIRM_REPORT_FAST
#       error FIRM_REPORT_FAST must be 0 in the appeal version
#     endif  // of FIRM_REPORT_FAST
#     if TL_634_MANY_SURR_ONE_SESS
#       error TL_634_MANY_SURR_ONE_SESS must be 0 in the appeal version
#     endif  // of TL_634_MANY_SURR_ONE_SESS
#   else  // of APPEAL
#   endif  // of APPEAL
# else  // of INSTANCE
#   if AREA_CANCEL_ENACTMENT_ON_FACE
#     error AREA_CANCEL_ENACTMENT_ON_FACE must be 0 in the regional version
#   endif  // of AREA_CANCEL_ENACTMENT_ON_FACE
#   if FIRM_REPORT_USE_ARRAY
#     error FIRM_REPORT_USE_ARRAY must be 0 in the regional version
#   endif  // of FIRM_REPORT_USE_ARRAY
#   if FIRM_REPORT_FAST
#     error FIRM_REPORT_FAST must be 0 in the regional version
#   endif  // of FIRM_REPORT_FAST
#   if TL_634_MANY_SURR_ONE_SESS
#     error TL_634_MANY_SURR_ONE_SESS must be 0 in the regional version
#   endif  // of TL_634_MANY_SURR_ONE_SESS
#   if PUNISHM_2ND_REPORT_12_SEP_2001
#     error PUNISHM_2ND_REPORT_12_SEP_2001 must be 0 in the regional\
 version
#   endif
# endif  // of INSTANCE

# endif  // _DEFINES_H
