// if NULL exit with abort(), not exit()
// use ternary expression
// print file, line and function that program exited at

#include <stdio.h>
#include <stdlib.h>
#define ASSERT(exp) (exp == NULL) ? printf("%s at line %d, %s, %s\n", #exp, __LINE__, __FILE__, __FUNCTION__), abort(), 1 : 1;
								

								
