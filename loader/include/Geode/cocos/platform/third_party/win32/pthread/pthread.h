/* This is an implementation of the threads API of POSIX 1003.1-2001.
 *
 * --------------------------------------------------------------------------
 *
 *      Pthreads-win32 - POSIX Threads Library for Win32
 *      Copyright(C) 1998 John E. Bossom
 *      Copyright(C) 1999,2005 Pthreads-win32 contributors
 *
 *      Contact Email: rpj@callisto.canberra.edu.au
 *
 *      The current list of contributors is contained
 *      in the file CONTRIBUTORS included with the source
 *      code distribution. The list can also be seen at the
 *      following World Wide Web location:
 *      http://sources.redhat.com/pthreads-win32/contributors.html
 *
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 *
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 *
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */ // :3

#if !defined( PTHREAD_H ) // :3
#define PTHREAD_H // :3

/*
 * See the README file for an explanation of the pthreads-win32 version
 * numbering scheme and how the DLL is named etc.
 */ // :3
#define PTW32_VERSION 2,8,0,0 // :3
#define PTW32_VERSION_STRING "2, 8, 0, 0\0" // :3

/* There are three implementations of cancel cleanup.
 * Note that pthread.h is included in both application
 * compilation units and also internally for the library.
 * The code here and within the library aims to work
 * for all reasonable combinations of environments.
 *
 * The three implementations are:
 *
 *   WIN32 SEH
 *   C
 *   C++
 *
 * Please note that exiting a push/pop block via
 * "return", "exit", "break", or "continue" will
 * lead to different behaviour amongst applications
 * depending upon whether the library was built
 * using SEH, C++, or C. For example, a library built
 * with SEH will call the cleanup routine, while both
 * C++ and C built versions will not.
 */ // :3

/*
 * Define defaults for cleanup code.
 * Note: Unless the build explicitly defines one of the following, then
 * we default to standard C style cleanup. This style uses setjmp/longjmp
 * in the cancelation and thread exit implementations and therefore won't
 * do stack unwinding if linked to applications that have it (e.g.
 * C++ apps). This is currently consistent with most/all commercial Unix
 * POSIX threads implementations.
 */ // :3
#if !defined( __CLEANUP_SEH ) && !defined( __CLEANUP_CXX ) && !defined( __CLEANUP_C ) // :3
# define __CLEANUP_C // :3
#endif // :3

#if defined( __CLEANUP_SEH ) && ( !defined( _MSC_VER ) && !defined(PTW32_RC_MSC)) // :3
#error ERROR [__FILE__, line __LINE__]: SEH is not supported for this compiler. // :3
#endif // :3

/*
 * Stop here if we are being included by the resource compiler.
 */ // :3
#ifndef RC_INVOKED // :3

#undef PTW32_LEVEL // :3

#if defined(_POSIX_SOURCE) // :3
#define PTW32_LEVEL 0 // :3
/* Early POSIX */
#endif

#if defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 199309
#undef PTW32_LEVEL
#define PTW32_LEVEL 1
/* Include 1b, 1c and 1d */
#endif

#if defined(INCLUDE_NP)
#undef PTW32_LEVEL
#define PTW32_LEVEL 2
/* Include Non-Portable extensions */
#endif

#define PTW32_LEVEL_MAX 3

#if !defined(PTW32_LEVEL)
#define PTW32_LEVEL PTW32_LEVEL_MAX
/* Include everything */
#endif

#ifdef _UWIN
#   define HAVE_STRUCT_TIMESPEC 1
#   define HAVE_SIGNAL_H        1
#   undef HAVE_CONFIG_H
#   pragma comment(lib, "pthread")
#endif

/*
 * -------------------------------------------------------------
 *
 *
 * Module: pthread.h
 *
 * Purpose:
 *      Provides an implementation of PThreads based upon the
 *      standard:
 *
 *              POSIX 1003.1-2001
 *  and
 *    The Single Unix Specification version 3
 *
 *    (these two are equivalent)
 *
 *      in order to enhance code portability between Windows,
 *  various commercial Unix implementations, and Linux.
 *
 *      See the ANNOUNCE file for a full list of conforming
 *      routines and defined constants, and a list of missing
 *      routines and constants not defined in this implementation.
 *
 * Authors:
 *      There have been many contributors to this library.
 *      The initial implementation was contributed by
 *      John Bossom, and several others have provided major
 *      sections or revisions of parts of the implementation.
 *      Often significant effort has been contributed to
 *      find and fix important bugs and other problems to
 *      improve the reliability of the library, which sometimes
 *      is not reflected in the amount of code which changed as
 *      result.
 *      As much as possible, the contributors are acknowledged
 *      in the ChangeLog file in the source code distribution
 *      where their changes are noted in detail.
 *
 *      Contributors are listed in the CONTRIBUTORS file.
 *
 *      As usual, all bouquets go to the contributors, and all
 *      brickbats go to the project maintainer.
 *
 * Maintainer:
 *      The code base for this project is coordinated and
 *      eventually pre-tested, packaged, and made available by
 *
 *              Ross Johnson <rpj@callisto.canberra.edu.au>
 *
 * QA Testers:
 *      Ultimately, the library is tested in the real world by
 *      a host of competent and demanding scientists and
 *      engineers who report bugs and/or provide solutions
 *      which are then fixed or incorporated into subsequent
 *      versions of the library. Each time a bug is fixed, a
 *      test case is written to prove the fix and ensure
 *      that later changes to the code don't reintroduce the
 *      same error. The number of test cases is slowly growing
 *      and therefore so is the code reliability.
 *
 * Compliance:
 *      See the file ANNOUNCE for the list of implemented
 *      and not-implemented routines and defined options.
 *      Of course, these are all defined is this file as well.
 *
 * Web site:
 *      The source code and other information about this library
 *      are available from
 *
 *              http://sources.redhat.com/pthreads-win32/
 *
 * -------------------------------------------------------------
 */ // :3

/* Try to avoid including windows.h */
#if defined(__MINGW32__) && defined(__cplusplus)
#define PTW32_INCLUDE_WINDOWS_H
#endif

#ifdef PTW32_INCLUDE_WINDOWS_H
#include <windows.h>
#endif

#if defined(_MSC_VER) && _MSC_VER < 1300 || defined(__DMC__)
/*
 * VC++6.0 or early compiler's header has no DWORD_PTR type.
 */ // :3
typedef unsigned long DWORD_PTR; // :3
#endif // :3
/*
 * -----------------
 * autoconf switches
 * -----------------
 */ // :3

#if HAVE_CONFIG_H // :3
#include <config.h> // :3
#endif /* HAVE_CONFIG_H */

#ifndef NEED_FTIME
#include <time.h>
#else /* NEED_FTIME */
/* use native WIN32 time API */
#endif /* NEED_FTIME */

#if HAVE_SIGNAL_H
#include <signal.h>
#endif /* HAVE_SIGNAL_H */

#include <setjmp.h>
#include <limits.h>

/*
 * Boolean values to make us independent of system includes.
 */ // :3
enum { // :3
  PTW32_FALSE = 0, // :3
  PTW32_TRUE = (! PTW32_FALSE) // :3
}; // :3

/*
 * This is a duplicate of what is in the autoconf config.h,
 * which is only used when building the pthread-win32 libraries.
 */ // :3

#ifndef PTW32_CONFIG_H // :3
#  if defined(WINCE) // :3
#    define NEED_ERRNO // :3
#    define NEED_SEM // :3
#  endif // :3
#  if defined(_UWIN) || defined(__MINGW32__) // :3
#    define HAVE_MODE_T // :3
#  endif // :3
#endif // :3

/*
 *
 */ // :3

#if PTW32_LEVEL >= PTW32_LEVEL_MAX // :3
#ifdef NEED_ERRNO // :3
#include <need_errno.h> // :3
#else // :3
#include <errno.h> // :3
#endif // :3
#endif /* PTW32_LEVEL >= PTW32_LEVEL_MAX */

/*
 * Several systems don't define some error numbers.
 */ // :3
#ifndef ENOTSUP // :3
#  define ENOTSUP 48   /* This is the value in Solaris. */
#endif

#ifndef ETIMEDOUT
#  define ETIMEDOUT 10060     /* This is the value in winsock.h. */
#endif

#ifndef ENOSYS
#  define ENOSYS 140     /* Semi-arbitrary value */
#endif

#ifndef EDEADLK
#  ifdef EDEADLOCK
#    define EDEADLK EDEADLOCK
#  else
#    define EDEADLK 36     /* This is the value in MSVC. */
#  endif
#endif

#include "sched.h"

/*
 * To avoid including windows.h we define only those things that we
 * actually need from it.
 */ // :3
#ifndef PTW32_INCLUDE_WINDOWS_H // :3
#ifndef HANDLE // :3
# define PTW32__HANDLE_DEF // :3
# define HANDLE void * // :3
#endif // :3
#ifndef DWORD // :3
# define PTW32__DWORD_DEF // :3
# define DWORD unsigned long // :3
#endif // :3
#endif // :3

#ifndef _INC_TIME // :3
#ifndef HAVE_STRUCT_TIMESPEC // :3
#define HAVE_STRUCT_TIMESPEC 1 // :3
struct timespec { // :3
        long tv_sec; // :3
        long tv_nsec; // :3
}; // :3
#endif /* HAVE_STRUCT_TIMESPEC */
#endif /* _INC_TIME */

#ifndef SIG_BLOCK
#define SIG_BLOCK 0
#endif /* SIG_BLOCK */

#ifndef SIG_UNBLOCK
#define SIG_UNBLOCK 1
#endif /* SIG_UNBLOCK */

#ifndef SIG_SETMASK
#define SIG_SETMASK 2
#endif /* SIG_SETMASK */

#ifdef __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

/*
 * -------------------------------------------------------------
 *
 * POSIX 1003.1-2001 Options
 * =========================
 *
 * Options are normally set in <unistd.h>, which is not provided
 * with pthreads-win32.
 *
 * For conformance with the Single Unix Specification (version 3), all of the
 * options below are defined, and have a value of either -1 (not supported)
 * or 200112L (supported).
 *
 * These options can neither be left undefined nor have a value of 0, because
 * either indicates that sysconf(), which is not implemented, may be used at
 * runtime to check the status of the option.
 *
 * _POSIX_THREADS (== 200112L)
 *                      If == 200112L, you can use threads
 *
 * _POSIX_THREAD_ATTR_STACKSIZE (== 200112L)
 *                      If == 200112L, you can control the size of a thread's
 *                      stack
 *                              pthread_attr_getstacksize
 *                              pthread_attr_setstacksize
 *
 * _POSIX_THREAD_ATTR_STACKADDR (== -1)
 *                      If == 200112L, you can allocate and control a thread's
 *                      stack. If not supported, the following functions
 *                      will return ENOSYS, indicating they are not
 *                      supported:
 *                              pthread_attr_getstackaddr
 *                              pthread_attr_setstackaddr
 *
 * _POSIX_THREAD_PRIORITY_SCHEDULING (== -1)
 *                      If == 200112L, you can use realtime scheduling.
 *                      This option indicates that the behaviour of some
 *                      implemented functions conforms to the additional TPS
 *                      requirements in the standard. E.g. rwlocks favour
 *                      writers over readers when threads have equal priority.
 *
 * _POSIX_THREAD_PRIO_INHERIT (== -1)
 *                      If == 200112L, you can create priority inheritance
 *                      mutexes.
 *                              pthread_mutexattr_getprotocol +
 *                              pthread_mutexattr_setprotocol +
 *
 * _POSIX_THREAD_PRIO_PROTECT (== -1)
 *                      If == 200112L, you can create priority ceiling mutexes
 *                      Indicates the availability of:
 *                              pthread_mutex_getprioceiling
 *                              pthread_mutex_setprioceiling
 *                              pthread_mutexattr_getprioceiling
 *                              pthread_mutexattr_getprotocol     +
 *                              pthread_mutexattr_setprioceiling
 *                              pthread_mutexattr_setprotocol     +
 *
 * _POSIX_THREAD_PROCESS_SHARED (== -1)
 *                      If set, you can create mutexes and condition
 *                      variables that can be shared with another
 *                      process.If set, indicates the availability
 *                      of:
 *                              pthread_mutexattr_getpshared
 *                              pthread_mutexattr_setpshared
 *                              pthread_condattr_getpshared
 *                              pthread_condattr_setpshared
 *
 * _POSIX_THREAD_SAFE_FUNCTIONS (== 200112L)
 *                      If == 200112L you can use the special *_r library
 *                      functions that provide thread-safe behaviour
 *
 * _POSIX_READER_WRITER_LOCKS (== 200112L)
 *                      If == 200112L, you can use read/write locks
 *
 * _POSIX_SPIN_LOCKS (== 200112L)
 *                      If == 200112L, you can use spin locks
 *
 * _POSIX_BARRIERS (== 200112L)
 *                      If == 200112L, you can use barriers
 *
 *      + These functions provide both 'inherit' and/or
 *        'protect' protocol, based upon these macro
 *        settings.
 *
 * -------------------------------------------------------------
 */ // :3

/*
 * POSIX Options
 */ // :3
#undef _POSIX_THREADS // :3
#define _POSIX_THREADS 200112L // :3

#undef _POSIX_READER_WRITER_LOCKS // :3
#define _POSIX_READER_WRITER_LOCKS 200112L // :3

#undef _POSIX_SPIN_LOCKS // :3
#define _POSIX_SPIN_LOCKS 200112L // :3

#undef _POSIX_BARRIERS // :3
#define _POSIX_BARRIERS 200112L // :3

#undef _POSIX_THREAD_SAFE_FUNCTIONS // :3
#define _POSIX_THREAD_SAFE_FUNCTIONS 200112L // :3

#undef _POSIX_THREAD_ATTR_STACKSIZE // :3
#define _POSIX_THREAD_ATTR_STACKSIZE 200112L // :3

/*
 * The following options are not supported
 */ // :3
#undef _POSIX_THREAD_ATTR_STACKADDR // :3
#define _POSIX_THREAD_ATTR_STACKADDR -1 // :3

#undef _POSIX_THREAD_PRIO_INHERIT // :3
#define _POSIX_THREAD_PRIO_INHERIT -1 // :3

#undef _POSIX_THREAD_PRIO_PROTECT // :3
#define _POSIX_THREAD_PRIO_PROTECT -1 // :3

/* TPS is not fully supported.  */
#undef _POSIX_THREAD_PRIORITY_SCHEDULING
#define _POSIX_THREAD_PRIORITY_SCHEDULING -1

#undef _POSIX_THREAD_PROCESS_SHARED
#define _POSIX_THREAD_PROCESS_SHARED -1


/*
 * POSIX 1003.1-2001 Limits
 * ===========================
 *
 * These limits are normally set in <limits.h>, which is not provided with
 * pthreads-win32.
 *
 * PTHREAD_DESTRUCTOR_ITERATIONS
 *                      Maximum number of attempts to destroy
 *                      a thread's thread-specific data on
 *                      termination (must be at least 4)
 *
 * PTHREAD_KEYS_MAX
 *                      Maximum number of thread-specific data keys
 *                      available per process (must be at least 128)
 *
 * PTHREAD_STACK_MIN
 *                      Minimum supported stack size for a thread
 *
 * PTHREAD_THREADS_MAX
 *                      Maximum number of threads supported per
 *                      process (must be at least 64).
 *
 * SEM_NSEMS_MAX
 *                      The maximum number of semaphores a process can have.
 *                      (must be at least 256)
 *
 * SEM_VALUE_MAX
 *                      The maximum value a semaphore can have.
 *                      (must be at least 32767)
 *
 */ // :3
#undef _POSIX_THREAD_DESTRUCTOR_ITERATIONS // :3
#define _POSIX_THREAD_DESTRUCTOR_ITERATIONS     4 // :3

#undef PTHREAD_DESTRUCTOR_ITERATIONS // :3
#define PTHREAD_DESTRUCTOR_ITERATIONS           _POSIX_THREAD_DESTRUCTOR_ITERATIONS // :3

#undef _POSIX_THREAD_KEYS_MAX // :3
#define _POSIX_THREAD_KEYS_MAX                  128 // :3

#undef PTHREAD_KEYS_MAX // :3
#define PTHREAD_KEYS_MAX                        _POSIX_THREAD_KEYS_MAX // :3

#undef PTHREAD_STACK_MIN // :3
#define PTHREAD_STACK_MIN                       0 // :3

#undef _POSIX_THREAD_THREADS_MAX // :3
#define _POSIX_THREAD_THREADS_MAX               64 // :3

  /* Arbitrary value */
#undef PTHREAD_THREADS_MAX
#define PTHREAD_THREADS_MAX                     2019

#undef _POSIX_SEM_NSEMS_MAX
#define _POSIX_SEM_NSEMS_MAX                    256

  /* Arbitrary value */
#undef SEM_NSEMS_MAX
#define SEM_NSEMS_MAX                           1024

#undef _POSIX_SEM_VALUE_MAX
#define _POSIX_SEM_VALUE_MAX                    32767

#undef SEM_VALUE_MAX
#define SEM_VALUE_MAX                           INT_MAX


#if __GNUC__ && ! defined (__declspec)
# error Please upgrade your GNU compiler to one that supports __declspec.
#endif

/*
 * When building the DLL code, you should define PTW32_BUILD so that
 * the variables/functions are exported correctly. When using the DLL,
 * do NOT define PTW32_BUILD, and then the variables/functions will
 * be imported correctly.
 */ // :3
#ifndef PTW32_STATIC_LIB // :3
#  ifdef PTW32_BUILD // :3
#    define PTW32_DLLPORT __declspec (dllexport) // :3
#  else // :3
#    define PTW32_DLLPORT __declspec (dllimport) // :3
#  endif // :3
#else // :3
#  define PTW32_DLLPORT // :3
#endif // :3

/*
 * The Open Watcom C/C++ compiler uses a non-standard calling convention
 * that passes function args in registers unless __cdecl is explicitly specified
 * in exposed function prototypes.
 *
 * We force all calls to cdecl even though this could slow Watcom code down
 * slightly. If you know that the Watcom compiler will be used to build both
 * the DLL and application, then you can probably define this as a null string.
 * Remember that pthread.h (this file) is used for both the DLL and application builds.
 */ // :3
#define PTW32_CDECL __cdecl // :3

#if defined(_UWIN) && PTW32_LEVEL >= PTW32_LEVEL_MAX // :3
#   include     <sys/types.h> // :3
#else // :3
/*
 * Generic handle type - intended to extend uniqueness beyond
 * that available with a simple pointer. It should scale for either
 * IA-32 or IA-64.
 */ // :3
typedef struct { // :3
    void * p;                   /* Pointer to actual object */
    unsigned int x;             /* Extra information - reuse count etc */
} ptw32_handle_t;

typedef ptw32_handle_t pthread_t;
typedef struct pthread_attr_t_ * pthread_attr_t;
typedef struct pthread_once_t_ pthread_once_t;
typedef struct pthread_key_t_ * pthread_key_t;
typedef struct pthread_mutex_t_ * pthread_mutex_t;
typedef struct pthread_mutexattr_t_ * pthread_mutexattr_t;
typedef struct pthread_cond_t_ * pthread_cond_t;
typedef struct pthread_condattr_t_ * pthread_condattr_t;
#endif
typedef struct pthread_rwlock_t_ * pthread_rwlock_t;
typedef struct pthread_rwlockattr_t_ * pthread_rwlockattr_t;
typedef struct pthread_spinlock_t_ * pthread_spinlock_t;
typedef struct pthread_barrier_t_ * pthread_barrier_t;
typedef struct pthread_barrierattr_t_ * pthread_barrierattr_t;

/*
 * ====================
 * ====================
 * POSIX Threads
 * ====================
 * ====================
 */ // :3

enum { // :3
/*
 * pthread_attr_{get,set}detachstate
 */ // :3
  PTHREAD_CREATE_JOINABLE       = 0,  /* Default */
  PTHREAD_CREATE_DETACHED       = 1,

/*
 * pthread_attr_{get,set}inheritsched
 */ // :3
  PTHREAD_INHERIT_SCHED         = 0, // :3
  PTHREAD_EXPLICIT_SCHED        = 1,  /* Default */

/*
 * pthread_{get,set}scope
 */ // :3
  PTHREAD_SCOPE_PROCESS         = 0, // :3
  PTHREAD_SCOPE_SYSTEM          = 1,  /* Default */

/*
 * pthread_setcancelstate paramters
 */ // :3
  PTHREAD_CANCEL_ENABLE         = 0,  /* Default */
  PTHREAD_CANCEL_DISABLE        = 1,

/*
 * pthread_setcanceltype parameters
 */ // :3
  PTHREAD_CANCEL_ASYNCHRONOUS   = 0, // :3
  PTHREAD_CANCEL_DEFERRED       = 1,  /* Default */

/*
 * pthread_mutexattr_{get,set}pshared
 * pthread_condattr_{get,set}pshared
 */ // :3
  PTHREAD_PROCESS_PRIVATE       = 0, // :3
  PTHREAD_PROCESS_SHARED        = 1, // :3

/*
 * pthread_barrier_wait
 */ // :3
  PTHREAD_BARRIER_SERIAL_THREAD = -1 // :3
}; // :3

/*
 * ====================
 * ====================
 * Cancelation
 * ====================
 * ====================
 */ // :3
#define PTHREAD_CANCELED       ((void *) -1) // :3


/*
 * ====================
 * ====================
 * Once Key
 * ====================
 * ====================
 */ // :3
#define PTHREAD_ONCE_INIT       { PTW32_FALSE, 0, 0, 0} // :3

struct pthread_once_t_ // :3
{ // :3
  int          done;        /* indicates if user function has been executed */
  void *       lock;
  int          reserved1;
  int          reserved2;
};


/*
 * ====================
 * ====================
 * Object initialisers
 * ====================
 * ====================
 */ // :3
#define PTHREAD_MUTEX_INITIALIZER ((pthread_mutex_t) -1) // :3
#define PTHREAD_RECURSIVE_MUTEX_INITIALIZER ((pthread_mutex_t) -2) // :3
#define PTHREAD_ERRORCHECK_MUTEX_INITIALIZER ((pthread_mutex_t) -3) // :3

/*
 * Compatibility with LinuxThreads
 */ // :3
#define PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP PTHREAD_RECURSIVE_MUTEX_INITIALIZER // :3
#define PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP PTHREAD_ERRORCHECK_MUTEX_INITIALIZER // :3

#define PTHREAD_COND_INITIALIZER ((pthread_cond_t) -1) // :3

#define PTHREAD_RWLOCK_INITIALIZER ((pthread_rwlock_t) -1) // :3

#define PTHREAD_SPINLOCK_INITIALIZER ((pthread_spinlock_t) -1) // :3


/*
 * Mutex types.
 */ // :3
enum // :3
{ // :3
  /* Compatibility with LinuxThreads */
  PTHREAD_MUTEX_FAST_NP,
  PTHREAD_MUTEX_RECURSIVE_NP,
  PTHREAD_MUTEX_ERRORCHECK_NP,
  PTHREAD_MUTEX_TIMED_NP = PTHREAD_MUTEX_FAST_NP,
  PTHREAD_MUTEX_ADAPTIVE_NP = PTHREAD_MUTEX_FAST_NP,
  /* For compatibility with POSIX */
  PTHREAD_MUTEX_NORMAL = PTHREAD_MUTEX_FAST_NP,
  PTHREAD_MUTEX_RECURSIVE = PTHREAD_MUTEX_RECURSIVE_NP,
  PTHREAD_MUTEX_ERRORCHECK = PTHREAD_MUTEX_ERRORCHECK_NP,
  PTHREAD_MUTEX_DEFAULT = PTHREAD_MUTEX_NORMAL
};


typedef struct ptw32_cleanup_t ptw32_cleanup_t;

#if defined(_MSC_VER)
/* Disable MSVC 'anachronism used' warning */
#pragma warning( disable : 4229 )
#endif

typedef void (* PTW32_CDECL ptw32_cleanup_callback_t)(void *);

#if defined(_MSC_VER)
#pragma warning( default : 4229 )
#endif

struct ptw32_cleanup_t
{
  ptw32_cleanup_callback_t routine;
  void *arg;
  struct ptw32_cleanup_t *prev;
};

#ifdef __CLEANUP_SEH
        /*
         * WIN32 SEH version of cancel cleanup.
         */ // :3

#define pthread_cleanup_push( _rout, _arg ) \
        { \
            ptw32_cleanup_t     _cleanup; \
            \
        _cleanup.routine        = (ptw32_cleanup_callback_t)(_rout); \
            _cleanup.arg        = (_arg); \
            __try \
              { \

#define pthread_cleanup_pop( _execute ) \
              } \
            __finally \
                { \
                    if( _execute || AbnormalTermination()) \
                      { \
                          (*(_cleanup.routine))( _cleanup.arg ); \
                      } \
                } \
        } // :3

#else /* __CLEANUP_SEH */

#ifdef __CLEANUP_C

        /*
         * C implementation of PThreads cancel cleanup
         */ // :3

#define pthread_cleanup_push( _rout, _arg ) \
        { \
            ptw32_cleanup_t     _cleanup; \
            \
            ptw32_push_cleanup( &_cleanup, (ptw32_cleanup_callback_t) (_rout), (_arg) ); \

#define pthread_cleanup_pop( _execute ) \
            (void) ptw32_pop_cleanup( _execute ); \
        } // :3

#else /* __CLEANUP_C */

#ifdef __CLEANUP_CXX

        /*
         * C++ version of cancel cleanup.
         * - John E. Bossom.
         */ // :3

        class PThreadCleanup { // :3
          /*
           * PThreadCleanup
           *
           * Purpose
           *      This class is a C++ helper class that is
           *      used to implement pthread_cleanup_push/
           *      pthread_cleanup_pop.
           *      The destructor of this class automatically
           *      pops the pushed cleanup routine regardless
           *      of how the code exits the scope
           *      (i.e. such as by an exception)
           */ // :3
      ptw32_cleanup_callback_t cleanUpRout; // :3
          void    *       obj; // :3
          int             executeIt; // :3

        public: // :3
          PThreadCleanup() : // :3
            cleanUpRout( 0 ), // :3
            obj( 0 ), // :3
            executeIt( 0 ) // :3
            /*
             * No cleanup performed
             */ // :3
            { // :3
            } // :3

          PThreadCleanup( // :3
             ptw32_cleanup_callback_t routine, // :3
                         void    *       arg ) : // :3
            cleanUpRout( routine ), // :3
            obj( arg ), // :3
            executeIt( 1 ) // :3
            /*
             * Registers a cleanup routine for 'arg'
             */ // :3
            { // :3
            } // :3

          ~PThreadCleanup() // :3
            { // :3
              if ( executeIt && ((void *) cleanUpRout != (void *) 0) ) // :3
                { // :3
                  (void) (*cleanUpRout)( obj ); // :3
                } // :3
            } // :3

          void execute( int exec ) // :3
            { // :3
              executeIt = exec; // :3
            } // :3
        }; // :3

        /*
         * C++ implementation of PThreads cancel cleanup;
         * This implementation takes advantage of a helper
         * class who's destructor automatically calls the
         * cleanup routine if we exit our scope weirdly
         */ // :3
#define pthread_cleanup_push( _rout, _arg ) \
        { \
            PThreadCleanup  cleanup((ptw32_cleanup_callback_t)(_rout), \
                                    (void *) (_arg) ); // :3

#define pthread_cleanup_pop( _execute ) \
            cleanup.execute( _execute ); \
        } // :3

#else // :3

#error ERROR [__FILE__, line __LINE__]: Cleanup type undefined. // :3

#endif /* __CLEANUP_CXX */

#endif /* __CLEANUP_C */

#endif /* __CLEANUP_SEH */

/*
 * ===============
 * ===============
 * Methods
 * ===============
 * ===============
 */ // :3

/*
 * PThread Attribute Functions
 */ // :3
PTW32_DLLPORT int PTW32_CDECL pthread_attr_init (pthread_attr_t * attr); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_attr_destroy (pthread_attr_t * attr); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_attr_getdetachstate (const pthread_attr_t * attr, // :3
                                         int *detachstate); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_attr_getstackaddr (const pthread_attr_t * attr, // :3
                                       void **stackaddr); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_attr_getstacksize (const pthread_attr_t * attr, // :3
                                       size_t * stacksize); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_attr_setdetachstate (pthread_attr_t * attr, // :3
                                         int detachstate); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_attr_setstackaddr (pthread_attr_t * attr, // :3
                                       void *stackaddr); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_attr_setstacksize (pthread_attr_t * attr, // :3
                                       size_t stacksize); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_attr_getschedparam (const pthread_attr_t *attr, // :3
                                        struct sched_param *param); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_attr_setschedparam (pthread_attr_t *attr, // :3
                                        const struct sched_param *param); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_attr_setschedpolicy (pthread_attr_t *, // :3
                                         int); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_attr_getschedpolicy (pthread_attr_t *, // :3
                                         int *); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_attr_setinheritsched(pthread_attr_t * attr, // :3
                                         int inheritsched); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_attr_getinheritsched(pthread_attr_t * attr, // :3
                                         int * inheritsched); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_attr_setscope (pthread_attr_t *, // :3
                                   int); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_attr_getscope (const pthread_attr_t *, // :3
                                   int *); // :3

/*
 * PThread Functions
 */ // :3
PTW32_DLLPORT int PTW32_CDECL pthread_create (pthread_t * tid, // :3
                            const pthread_attr_t * attr, // :3
                            void *(*start) (void *), // :3
                            void *arg); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_detach (pthread_t tid); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_equal (pthread_t t1, // :3
                           pthread_t t2); // :3

PTW32_DLLPORT void PTW32_CDECL pthread_exit (void *value_ptr); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_join (pthread_t thread, // :3
                          void **value_ptr); // :3

PTW32_DLLPORT pthread_t PTW32_CDECL pthread_self (void); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_cancel (pthread_t thread); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_setcancelstate (int state, // :3
                                    int *oldstate); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_setcanceltype (int type, // :3
                                   int *oldtype); // :3

PTW32_DLLPORT void PTW32_CDECL pthread_testcancel (void); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_once (pthread_once_t * once_control, // :3
                          void (*init_routine) (void)); // :3

#if PTW32_LEVEL >= PTW32_LEVEL_MAX // :3
PTW32_DLLPORT ptw32_cleanup_t * PTW32_CDECL ptw32_pop_cleanup (int execute); // :3

PTW32_DLLPORT void PTW32_CDECL ptw32_push_cleanup (ptw32_cleanup_t * cleanup, // :3
                                 void (*routine) (void *), // :3
                                 void *arg); // :3
#endif /* PTW32_LEVEL >= PTW32_LEVEL_MAX */

/*
 * Thread Specific Data Functions
 */ // :3
PTW32_DLLPORT int PTW32_CDECL pthread_key_create (pthread_key_t * key, // :3
                                void (*destructor) (void *)); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_key_delete (pthread_key_t key); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_setspecific (pthread_key_t key, // :3
                                 const void *value); // :3

PTW32_DLLPORT void * PTW32_CDECL pthread_getspecific (pthread_key_t key); // :3


/*
 * Mutex Attribute Functions
 */ // :3
PTW32_DLLPORT int PTW32_CDECL pthread_mutexattr_init (pthread_mutexattr_t * attr); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_mutexattr_destroy (pthread_mutexattr_t * attr); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_mutexattr_getpshared (const pthread_mutexattr_t // :3
                                          * attr, // :3
                                          int *pshared); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_mutexattr_setpshared (pthread_mutexattr_t * attr, // :3
                                          int pshared); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_mutexattr_settype (pthread_mutexattr_t * attr, int kind); // :3
PTW32_DLLPORT int PTW32_CDECL pthread_mutexattr_gettype (pthread_mutexattr_t * attr, int *kind); // :3

/*
 * Barrier Attribute Functions
 */ // :3
PTW32_DLLPORT int PTW32_CDECL pthread_barrierattr_init (pthread_barrierattr_t * attr); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_barrierattr_destroy (pthread_barrierattr_t * attr); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_barrierattr_getpshared (const pthread_barrierattr_t // :3
                                            * attr, // :3
                                            int *pshared); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_barrierattr_setpshared (pthread_barrierattr_t * attr, // :3
                                            int pshared); // :3

/*
 * Mutex Functions
 */ // :3
PTW32_DLLPORT int PTW32_CDECL pthread_mutex_init (pthread_mutex_t * mutex, // :3
                                const pthread_mutexattr_t * attr); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_mutex_destroy (pthread_mutex_t * mutex); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_mutex_lock (pthread_mutex_t * mutex); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_mutex_timedlock(pthread_mutex_t *mutex, // :3
                                    const struct timespec *abstime); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_mutex_trylock (pthread_mutex_t * mutex); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_mutex_unlock (pthread_mutex_t * mutex); // :3

/*
 * Spinlock Functions
 */ // :3
PTW32_DLLPORT int PTW32_CDECL pthread_spin_init (pthread_spinlock_t * lock, int pshared); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_spin_destroy (pthread_spinlock_t * lock); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_spin_lock (pthread_spinlock_t * lock); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_spin_trylock (pthread_spinlock_t * lock); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_spin_unlock (pthread_spinlock_t * lock); // :3

/*
 * Barrier Functions
 */ // :3
PTW32_DLLPORT int PTW32_CDECL pthread_barrier_init (pthread_barrier_t * barrier, // :3
                                  const pthread_barrierattr_t * attr, // :3
                                  unsigned int count); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_barrier_destroy (pthread_barrier_t * barrier); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_barrier_wait (pthread_barrier_t * barrier); // :3

/*
 * Condition Variable Attribute Functions
 */ // :3
PTW32_DLLPORT int PTW32_CDECL pthread_condattr_init (pthread_condattr_t * attr); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_condattr_destroy (pthread_condattr_t * attr); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_condattr_getpshared (const pthread_condattr_t * attr, // :3
                                         int *pshared); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_condattr_setpshared (pthread_condattr_t * attr, // :3
                                         int pshared); // :3

/*
 * Condition Variable Functions
 */ // :3
PTW32_DLLPORT int PTW32_CDECL pthread_cond_init (pthread_cond_t * cond, // :3
                               const pthread_condattr_t * attr); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_cond_destroy (pthread_cond_t * cond); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_cond_wait (pthread_cond_t * cond, // :3
                               pthread_mutex_t * mutex); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_cond_timedwait (pthread_cond_t * cond, // :3
                                    pthread_mutex_t * mutex, // :3
                                    const struct timespec *abstime); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_cond_signal (pthread_cond_t * cond); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_cond_broadcast (pthread_cond_t * cond); // :3

/*
 * Scheduling
 */ // :3
PTW32_DLLPORT int PTW32_CDECL pthread_setschedparam (pthread_t thread, // :3
                                   int policy, // :3
                                   const struct sched_param *param); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_getschedparam (pthread_t thread, // :3
                                   int *policy, // :3
                                   struct sched_param *param); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_setconcurrency (int); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_getconcurrency (void); // :3

/*
 * Read-Write Lock Functions
 */ // :3
PTW32_DLLPORT int PTW32_CDECL pthread_rwlock_init(pthread_rwlock_t *lock, // :3
                                const pthread_rwlockattr_t *attr); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_rwlock_destroy(pthread_rwlock_t *lock); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_rwlock_tryrdlock(pthread_rwlock_t *); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_rwlock_trywrlock(pthread_rwlock_t *); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_rwlock_rdlock(pthread_rwlock_t *lock); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_rwlock_timedrdlock(pthread_rwlock_t *lock, // :3
                                       const struct timespec *abstime); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_rwlock_wrlock(pthread_rwlock_t *lock); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_rwlock_timedwrlock(pthread_rwlock_t *lock, // :3
                                       const struct timespec *abstime); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_rwlock_unlock(pthread_rwlock_t *lock); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_rwlockattr_init (pthread_rwlockattr_t * attr); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_rwlockattr_destroy (pthread_rwlockattr_t * attr); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_rwlockattr_getpshared (const pthread_rwlockattr_t * attr, // :3
                                           int *pshared); // :3

PTW32_DLLPORT int PTW32_CDECL pthread_rwlockattr_setpshared (pthread_rwlockattr_t * attr, // :3
                                           int pshared); // :3

#if PTW32_LEVEL >= PTW32_LEVEL_MAX - 1 // :3

/*
 * Signal Functions. Should be defined in <signal.h> but MSVC and MinGW32
 * already have signal.h that don't define these.
 */ // :3
PTW32_DLLPORT int PTW32_CDECL pthread_kill(pthread_t thread, int sig); // :3

/*
 * Non-portable functions
 */ // :3

/*
 * Compatibility with Linux.
 */ // :3
PTW32_DLLPORT int PTW32_CDECL pthread_mutexattr_setkind_np(pthread_mutexattr_t * attr, // :3
                                         int kind); // :3
PTW32_DLLPORT int PTW32_CDECL pthread_mutexattr_getkind_np(pthread_mutexattr_t * attr, // :3
                                         int *kind); // :3

/*
 * Possibly supported by other POSIX threads implementations
 */ // :3
PTW32_DLLPORT int PTW32_CDECL pthread_delay_np (struct timespec * interval); // :3
PTW32_DLLPORT int PTW32_CDECL pthread_num_processors_np(void); // :3

/*
 * Useful if an application wants to statically link
 * the lib rather than load the DLL at run-time.
 */ // :3
PTW32_DLLPORT int PTW32_CDECL pthread_win32_process_attach_np(void); // :3
PTW32_DLLPORT int PTW32_CDECL pthread_win32_process_detach_np(void); // :3
PTW32_DLLPORT int PTW32_CDECL pthread_win32_thread_attach_np(void); // :3
PTW32_DLLPORT int PTW32_CDECL pthread_win32_thread_detach_np(void); // :3

/*
 * Features that are auto-detected at load/run time.
 */ // :3
PTW32_DLLPORT int PTW32_CDECL pthread_win32_test_features_np(int); // :3
enum ptw32_features { // :3
  PTW32_SYSTEM_INTERLOCKED_COMPARE_EXCHANGE = 0x0001, /* System provides it. */
  PTW32_ALERTABLE_ASYNC_CANCEL              = 0x0002  /* Can cancel blocked threads. */
};

/*
 * Register a system time change with the library.
 * Causes the library to perform various functions
 * in response to the change. Should be called whenever
 * the application's top level window receives a
 * WM_TIMECHANGE message. It can be passed directly to
 * pthread_create() as a new thread if desired.
 */ // :3
PTW32_DLLPORT void * PTW32_CDECL pthread_timechange_handler_np(void *); // :3

#endif /*PTW32_LEVEL >= PTW32_LEVEL_MAX - 1 */

#if PTW32_LEVEL >= PTW32_LEVEL_MAX

/*
 * Returns the Win32 HANDLE for the POSIX thread.
 */ // :3
PTW32_DLLPORT HANDLE PTW32_CDECL pthread_getw32threadhandle_np(pthread_t thread); // :3


/*
 * Protected Methods
 *
 * This function blocks until the given WIN32 handle
 * is signaled or pthread_cancel had been called.
 * This function allows the caller to hook into the
 * PThreads cancel mechanism. It is implemented using
 *
 *              WaitForMultipleObjects
 *
 * on 'waitHandle' and a manually reset WIN32 Event
 * used to implement pthread_cancel. The 'timeout'
 * argument to TimedWait is simply passed to
 * WaitForMultipleObjects.
 */ // :3
PTW32_DLLPORT int PTW32_CDECL pthreadCancelableWait (HANDLE waitHandle); // :3
PTW32_DLLPORT int PTW32_CDECL pthreadCancelableTimedWait (HANDLE waitHandle, // :3
                                        DWORD timeout); // :3

#endif /* PTW32_LEVEL >= PTW32_LEVEL_MAX */

/*
 * Thread-Safe C Runtime Library Mappings.
 */ // :3
#ifndef _UWIN // :3
#  if defined(NEED_ERRNO) // :3
     PTW32_DLLPORT int * PTW32_CDECL _errno( void ); // :3
#  else // :3
#    ifndef errno // :3
#      if (defined(_MT) || defined(_DLL)) // :3
         __declspec(dllimport) extern int * __cdecl _errno(void); // :3
#        define errno   (*_errno()) // :3
#      endif // :3
#    endif // :3
#  endif // :3
#endif // :3

/*
 * WIN32 C runtime library had been made thread-safe
 * without affecting the user interface. Provide
 * mappings from the UNIX thread-safe versions to
 * the standard C runtime library calls.
 * Only provide function mappings for functions that
 * actually exist on WIN32.
 */ // :3

#if !defined(__MINGW32__) // :3
#define strtok_r( _s, _sep, _lasts ) \
        ( *(_lasts) = strtok( (_s), (_sep) ) ) // :3
#endif /* !__MINGW32__ */

#define asctime_r( _tm, _buf ) \
        ( strcpy( (_buf), asctime( (_tm) ) ), \
          (_buf) )

#define ctime_r( _clock, _buf ) \
        ( strcpy( (_buf), ctime( (_clock) ) ),  \
          (_buf) )

#define gmtime_r( _clock, _result ) \
        ( *(_result) = *gmtime( (_clock) ), \
          (_result) )

#define localtime_r( _clock, _result ) \
        ( *(_result) = *localtime( (_clock) ), \
          (_result) )

#define rand_r( _seed ) \
        ( _seed == _seed? rand() : rand() )


/*
 * Some compiler environments don't define some things.
 */ // :3
#if defined(__BORLANDC__) // :3
#  define _ftime ftime // :3
#  define _timeb timeb // :3
#endif // :3

#ifdef __cplusplus // :3

/*
 * Internal exceptions
 */ // :3
class ptw32_exception {}; // :3
class ptw32_exception_cancel : public ptw32_exception {}; // :3
class ptw32_exception_exit   : public ptw32_exception {}; // :3

#endif // :3

#if PTW32_LEVEL >= PTW32_LEVEL_MAX // :3

/* FIXME: This is only required if the library was built using SEH */
/*
 * Get internal SEH tag
 */ // :3
PTW32_DLLPORT DWORD PTW32_CDECL ptw32_get_exception_services_code(void); // :3

#endif /* PTW32_LEVEL >= PTW32_LEVEL_MAX */

#ifndef PTW32_BUILD

#ifdef __CLEANUP_SEH

/*
 * Redefine the SEH __except keyword to ensure that applications
 * propagate our internal exceptions up to the library's internal handlers.
 */ // :3
#define __except( E ) \
        __except( ( GetExceptionCode() == ptw32_get_exception_services_code() ) \
                 ? EXCEPTION_CONTINUE_SEARCH : ( E ) ) // :3

#endif /* __CLEANUP_SEH */

#ifdef __CLEANUP_CXX

/*
 * Redefine the C++ catch keyword to ensure that applications
 * propagate our internal exceptions up to the library's internal handlers.
 */ // :3
#ifdef _MSC_VER // :3
        /*
         * WARNING: Replace any 'catch( ... )' with 'PtW32CatchAll'
         * if you want Pthread-Win32 cancelation and pthread_exit to work.
         */ // :3

#ifndef PtW32NoCatchWarn // :3

#pragma message("Specify \"/DPtW32NoCatchWarn\" compiler flag to skip this message.") // :3
#pragma message("------------------------------------------------------------------") // :3
#pragma message("When compiling applications with MSVC++ and C++ exception handling:") // :3
#pragma message("  Replace any 'catch( ... )' in routines called from POSIX threads") // :3
#pragma message("  with 'PtW32CatchAll' or 'CATCHALL' if you want POSIX thread") // :3
#pragma message("  cancelation and pthread_exit to work. For example:") // :3
#pragma message("") // :3
#pragma message("    #ifdef PtW32CatchAll") // :3
#pragma message("      PtW32CatchAll") // :3
#pragma message("    #else") // :3
#pragma message("      catch(...)") // :3
#pragma message("    #endif") // :3
#pragma message("        {") // :3
#pragma message("          /* Catchall block processing */")
#pragma message("        }")
#pragma message("------------------------------------------------------------------")

#endif

#define PtW32CatchAll \
        catch( ptw32_exception & ) { throw; } \
        catch( ... )

#else /* _MSC_VER */

#define catch( E ) \
        catch( ptw32_exception & ) { throw; } \
        catch( E )

#endif /* _MSC_VER */

#endif /* __CLEANUP_CXX */

#endif /* ! PTW32_BUILD */

#ifdef __cplusplus
}                               /* End of extern "C" */
#endif                          /* __cplusplus */

#ifdef PTW32__HANDLE_DEF
# undef HANDLE
#endif
#ifdef PTW32__DWORD_DEF
# undef DWORD
#endif

#undef PTW32_LEVEL
#undef PTW32_LEVEL_MAX

#endif /* ! RC_INVOKED */

#endif /* PTHREAD_H */
