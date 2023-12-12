#ifndef FEAT_H_
#define FEAT_H_

/* features.h but not really */

#define _OS_Linux     0
#define _OS_Windows   1
#define _OS_FreeBSD   2
#define _OS_MacOS     3

#if defined(_WIN32)
# define _OS _OS_Windows
#elif defined(__linux__)
# define _OS _OS_Linux
#elif defined(__FreeBSD__)
# define _OS _OS_FreeBSD
#elif defined(__MACH__)
# define _OS _OS_MacOS
#else
# error Unsupported operating system
#endif

#if __STDC_VERSION__ >= 201112L
#elif defined(__GNUC__)
# define _Noreturn __attribute__((__noreturn__))
#else
# define _Noreturn
#endif

#define __weak __attribute__((__weak__))
#define __hidden __attribute__((__visibility__("hidden")))
#define weak_alias(old, new) \
    extern __typeof(old) new __attribute__((__weak__, __alias__(#old)))

#ifdef __GNUC__
# define __unused __attribute__((unused))
#else
# define __unused
#endif

#ifdef __GNUC__
# define __fallthrough __attribute__((fallthrough))
#else
# define __fallthrough
#endif

#endif /* FEAT_H_ */
