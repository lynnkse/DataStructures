#define RETURN_IF_EQUAL(check, checkAgainst, returnVal) if(checkAgainst == check) \
															{ \
															return returnVal; \
															} 
#define RETURN_VOID_IF_EQUAL(check, checkAgainst) if(checkAgainst == check) \
															{ \
															return; \
															} 

#define RELEASE_MEMORY_AND_RETURN_IF_EQUAL(check, checkAgainst, toFree, returnVal) if(checkAgainst == check) \
																		{\
																			free(toFree); \
																			return returnVal; \
																		}
