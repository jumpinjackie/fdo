/************************************************************************
 * Messages handlers.
 *
 * Modification history
 *   Mnemonic    Release Date        Who     Review
 *
 *   NLSTOOLS    victor  92jan08     Mike Tardif
 *       Created.
 *
 *	ID: $RCSfile: msg.c $ $Revision: 4.3 $ $Date: 1997/02/11 18:52:53 $ $Author: greg $
 ************************************************************************/
/*LINTLIBRARY*/
/*NOTUSED*/

#include <stdlib.h>			/* for def'n of EXIT_FAILURE */
#ifdef	__STDC__
#include <stdarg.h>			/* for def'n of va_start(), va_end(), ... */
#else
#include <varargs.h>		/* for def'n of va_alist, va_dcl, va_start(), ... */
#endif
#include <stdio.h>			/* for def'n of fprintf(), vsprintf(), ... */
#include <mkcatdefs.h>		/* for dcl'n of msg*[], progname, ... */


/*
 *	Generic error and informational messages.
 */
char_t msg00[] = "fatal internal error in `%s' on line %d";
char_t msg01[] = "path `%s' is too long";
char_t msg02[] = "no more memory";
char_t msg03[] = "name `%s' is too long";
char_t msg04[] = "no more hash table space";
char_t msg05[] = "errors found: no %s created";
char_t msg06[] = "%s created";

/*
 *	Message source file error messages.
 */
char_t msg10[] = "cannot write message header file `%s'";
char_t msg11[] = "invalid directive `%s' found on line %d of file `%s'";
char_t msg12[] = "premature eof on line %d of file `%s'";
char_t msg13[] = "cannot read message source file `%s'";
char_t msg14[] = "set or message identifier `%s' used more than once";
char_t msg15[] = "too long set or message identifier on line %d of file `%s'";
char_t msg16[] = "missing quote character on line %d of file `%s'";
char_t msg17[] = "missing separator character on line %d of file `%s'";
char_t msg18[] = "non-increasing set or message number (%d) on line %d of file `%s'";
char_t msg19[] = "set or message identifier number `%d' exceeds maximal value";
char_t msg20[] = "cannot write to stdout";


static int msg_errors = 0;


/*
 * msg_errors_found - errors found.
 *
 * Returns boolean of errors found.
 */
int
#ifdef	__STDC__
msg_errors_found(void)
#else
msg_errors_found()
#endif
{
	return (msg_errors > 0);
}


/*
 * msg_issue_info - issue informational message.
 *
 * For each informational (warning) message
 * issued, count an error.
 * Returns void upon success, and panics on error.
 */
void
#ifdef	__STDC__
/*VARARGS1*/
msg_issue_info(const char_t *format, ...)
#else
/*VARARGS1*/
msg_issue_info(format, va_alist)
	char_t *format;
	va_dcl
#endif
{
	va_list argp;
	msg_errors++;
	/*
	 *	Print informational message.
	 */
	(void) fprintf(stderr, "%s: ", progname);
#ifdef	__STDC__
	va_start(argp, format);
#else
	va_start(argp);
#endif
	(void) vfprintf(stderr, format, argp);
	va_end(argp);
	putc('\n', stderr);
	return;
}


/*
 * msg_issue_error - issue fatal error message.
 *
 * Print the error message and exit.
 * Never returns.
 */
void
#ifdef	__STDC__
/*VARARGS1*/
msg_issue_error(const char_t *format, ...)
#else
/*VARARGS1*/
msg_issue_error(format, va_alist)
	char_t *format;
	va_dcl
#endif
{
	va_list argp;
	/*
	 *	Print error message.
	 */
	(void) fprintf(stderr, "%s: ", progname);
#ifdef	__STDC__
	va_start(argp, format);
#else
	va_start(argp);
#endif
	(void) vfprintf(stderr, format, argp);
	va_end(argp);
	putc('\n', stderr);
	/*
	 *	Quit.
	 */
	exit(EXIT_FAILURE);
}



#ifdef	STANDALONE
/*
 * $Compile: mk -m$ARCH %f
 * $Compile(AIX): xlc -g -DSTANDALONE -D_ALL_SOURCE -I. %f -o %F $mylib/mkcatdefs.a
 * $Compile(Sun4): /usr/5bin/cc -g -DSTANDALONE -I. %f -o %F $mylib/mkcatdefs.a
 */

char_t *progname;

main(argc, argv)
char_t *argv[];
{
	progname = argv[0];
}
#endif
