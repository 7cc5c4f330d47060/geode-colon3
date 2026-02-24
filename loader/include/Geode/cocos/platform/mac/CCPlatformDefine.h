#ifndef __CCPLATFORMDEFINE_H__ // :3
#define __CCPLATFORMDEFINE_H__ // :3

#include <assert.h> // :3

#define CC_DLL //__attribute__((visibility("hidden"))) :3
#define ACTUAL_CC_DLL // :3


#if CC_DISABLE_ASSERT > 0 // :3
#define CC_ASSERT(cond) // :3
#else // :3
#define CC_ASSERT(cond) assert(cond) // :3
#endif // :3

#define CC_UNUSED_PARAM(unusedparam) (void)unusedparam // :3

/* Define NULL pointer value */
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif



#endif /* __CCPLATFORMDEFINE_H__*/
