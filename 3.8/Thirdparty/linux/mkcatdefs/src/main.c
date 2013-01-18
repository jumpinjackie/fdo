/************************************************************************
 * mkcatdefs - catalog message source file tool
 *
 * Modification history
 *   Mnemonic    Release Date        Who     Review
 *
 *   NLSTOOLS    victor  92jan09     Mike Tardif
 *       Created.
 *
 * SccsId = "@(#)main.c	9301.1 93/01/15"
 ************************************************************************/
/*LINTLIBRARY*/
/*NOTUSED*/

#include <stdio.h>			/* for def'n of BUFSIZ, ... */
#include <stdlib.h>			/* for def'n of EXIT_SUCCESS */
#include <unistd.h>			/* for def'n of R_OK */
#include <mkcatdefs.h>		/* for def'n of msg_issue_error(), ... */


extern int optind;			/* for use of getopt() */
char_t *progname;				/* the program name */

#ifndef lint
static char_t *Sccsid = "@(#)main.c	9301.1 93/01/15";
#endif


#ifdef	__STDC__
main(int argc, char_t *argv[])
#else
main(argc, argv)
	int argc;
	char_t *argv[];
#endif
{
	char_t title[BUFSIZ];
	/*
	 *	Record program name.
	 */
	progname = argv[0];
	/*
	 *	Parse program options.
	 */
	args_parse_opts(argc, argv);
	/*
	 *	Foreach each remaining program argument do ...
	 */
	for ( ; optind < argc; optind++) {
		char_t *file = argv[optind];
		/*
		 *	If file argument starts with a tilde then expand to home directory.
		 */
		if (*file == '~')
			file = args_tilde_home(file);
		/*
		 *	If cannot access file then issue info.
		 */
		if (file == NULL || access(file, R_OK))
			msg_issue_info(msg13, argv[optind]);
		/*
		 *	Process messages source file.
		 */
		msf_mk_cat_defs(file);
		/*
		 *	Flush message source file.
		 */
		msf_flush_header(DONT_CLOSE);
	}
	/*
	 *	Write message source file.
	 */
	msf_flush_header(DO_CLOSE);
	exit(EXIT_SUCCESS);
	/*NOTREACHED*/
	return (0);
}
