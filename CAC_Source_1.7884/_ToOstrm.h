# if ! defined (_TOOSTRM_H)
# define _TOOSTRM_H

void container_sides_to_ostream (ostream & os,
					   const TRCDKeyContainer & container,
					   const TSideWind & pattern,
					   const int suitors = -1,  // < 0 --> all
					   const int defendants = -1,  // < 0 --> all
					   const int others = 0) ;

void preliminary_actions_to_ostream (ostream & os,
                                     const TLawsuit & law,
                                 const char * const senderInstances = NULL,
                                     const bool withSender = true,
                                 const char * const senderPrefix = NULL,
                                     const bool labelOnly = false 
                                    ) ;
# endif  // _TOOSTRM_H
//

