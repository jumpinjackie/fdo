/************************************************************************
 * Global manifest constants and declarations.
 *
 * Modification history
 *   Mnemonic    Release Date        Who     Review
 *
 *   NLSTOOLS    victor  92jan09     Mike Tardif
 *       Created.
 *
 *   HPUXPORT   	tools  	95mar17     Weibo Wang
 *       For HP-UX ports.
 *
*   NUTC        cyclops     97Apr23     Doug Carr                       *
*       Nutcracker port
*
 *  ID: $RCSfile: mkcatdefs.h $ $Revision: 4.4 $ $Date: 1997/04/29 17:56:58 $ $Author: doug $
 ************************************************************************/

#if defined(__STDC__) && ! defined(VISION_NUTC_STDC) /* macros for function prototypes */
#define PROTO(x)        x
#else
#define PROTO(x)        ()
#endif

#if !defined(TRUE) || ((TRUE) != 1)
#define TRUE    		(1)
#endif
#if !defined(FALSE) || ((FALSE) != 0)
#define FALSE   		(0)
#endif

#define	DO_CLOSE		1		/* do close file */
#define	DONT_CLOSE		0		/* don't bother to close file */
#define	DO_UNLINK		-1		/* do unlink file if empty */

#if defined(VISION_NUTC)
typedef char char_t;
#endif
#if defined(HPUX)
typedef char char_t;
#endif
#if	defined(ultrix)
typedef char char_t;
#endif
#if	defined(sun)
typedef char char_t;
#endif
#if	defined(_AIX)
typedef char char_t;
#endif
#if	defined(LINUX)
typedef char char_t;
#endif


/*
 *	defined in msf.c
 */
extern void msf_mk_cat_defs PROTO((char_t *path));
extern void msf_create_header PROTO((char_t *catname));
extern void msf_flush_header PROTO((int op));
extern void msf_add_identifier PROTO((char_t *id));

/*
 *	defined in args.c
 */
extern void args_parse_opts PROTO((int argc, char_t *argv[]));
extern char_t *args_tilde_home PROTO((char_t *path));

/*
 *	defined in msg.c
 */
extern void msg_issue_error PROTO((const char_t *format, ...));
extern void msg_issue_info PROTO((const char_t *format, ...));
extern int msg_errors_found PROTO((void));


extern char_t *progname;

/*
 *	generic messages
 */
extern char_t msg00[];
extern char_t msg01[];
extern char_t msg02[];
extern char_t msg03[];
extern char_t msg04[];
extern char_t msg05[];
extern char_t msg06[];

/*
 *	messages specific to msf.c
 */
extern char_t msg10[];
extern char_t msg11[];
extern char_t msg12[];
extern char_t msg13[];
extern char_t msg14[];
extern char_t msg15[];
extern char_t msg16[];
extern char_t msg17[];
extern char_t msg18[];
extern char_t msg19[];
extern char_t msg20[];
