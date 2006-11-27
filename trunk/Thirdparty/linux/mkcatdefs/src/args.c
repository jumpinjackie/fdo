/************************************************************************
 * (Command line) Argument and usage handlers.
 *
 * Modification history
 *   Mnemonic    Release Date        Who     Review
 *
 *   NLSTOOLS    victor  92jan09     Mike Tardif
 *       Created.
 *
*   NUTC        cyclops     97Apr23     Doug Carr                       *
*       Nutcracker port
*
 *      ID: $RCSfile: args.c $ $Revision: 4.4 $ $Date: 1997/04/29 17:58:51 $ $Author: doug $
 ************************************************************************/
/*LINTLIBRARY*/
/*NOTUSED*/

#include <stdio.h>                      /* for def'n of fprintf(), ... */
#include <string.h>                     /* for def'n of strcpy(), strcat()  */
#include <stdlib.h>                     /* for def'n of getopt(), getenv()  */
#include <unistd.h>                     /* for def'n of R_OK */
#include <pwd.h>                        /* for def'n of getpwnam() */
#include <sys/param.h>          /* for def'n of MAXPATHLEN */
#include <mkcatdefs.h>          /* for def'n of char_t, ... */


/*
 *  usage - print usage and quit
 *
 *      Usage: <prog> <catname> <msffile> ...
 */
static void
#ifdef  __STDC__
usage(char_t *prog)
#else
usage(prog)
	char_t *prog;
#endif
{
	/*
	 *      Print usage message.
	 */
	(void) fprintf(stderr, "Usage: %s <catname> <msffile> ...\n", prog);
	exit(EXIT_FAILURE);
}


#if ! defined(VISION_NUTC)
int optind = 1;             /* as though it were used by getopt() */
#endif


/*
 *      args_parse_opts - parse program options
 *
 *      Returns void on success, and prints usage and quits otherwise.
 */
void
#ifdef  __STDC__
args_parse_opts(int argc, char_t *argv[])
#else
args_parse_opts(argc, argv)
	int argc;
	char_t *argv[];
#endif
{
    register int eflag = 0;

    /*
	 *      If not enough arguments then print usage and quit.
	 */
	if (argc <= 1 || argv[1] == NULL)
		usage(progname);
	/*
	 *      Create msf header file.
	 */
	msf_create_header(argv[1]);
	/*
	 *      Simulate getopt() behaviour.
	 */
	optind += 1;
	return;
}


/*
 *      args_tilde_home - expand leading '~' in a path to a user's home directory
 *
 *      Returns char_t * pointer to
 *              1. the original path if path has no leading '~'.
 *              2. NULL if "~user" is not a valid username.
 *              3. a static buffer of the expanded path.
 */
char_t *
#ifdef  __STDC__
args_tilde_home(char_t *path)
#else
args_tilde_home(path)
	char_t  *path;
#endif
{
	static char_t homepath[MAXPATHLEN+1];
	if (*path == '~') {
		register char_t *tp = path + 1;
		/*
		 *      If path == "~/" or path == "~" then use user's home directory.
		 */
		if (*tp == '/' || *tp == '\0') {
			char_t *homedir;
			/*
			 *      Get user's home directory from environment variable.
			 */
			if ((homedir = getenv("HOME")) == NULL)
				homedir = "/";
			/*
			 *      Copy user's home directory.
			 */
			(void) strcpy(homepath, homedir);
		}
		/*
		 *      Else use user's home directory from passwd file.
		 */
		else {
			char_t username[L_cuserid], *np = username;
			struct passwd *ent;
			/*
			 *      Copy username part of "~username/" or "~username"
			 */
			while (*tp != '/' && *tp != '\0') {
				*np++ = *tp++;
				/*
				 *      If about to overflow username buffer then stop copying. 
				 */
				if (np == &username[L_cuserid - 1])
					break;
			}
			*np = '\0';
			/*
			 *      Get user's passwd entry.
			 */
			if ((ent = getpwnam(username)) == NULL)
				return (NULL);
			/*
			 *      Copy user's home directory.
			 */
			(void) strcpy(homepath, ent->pw_dir);
		}
		/*
		 *      Append rest of path.
		 */
		(void) strcat(homepath, tp);
		return (homepath);
	}
	return (path);
}


#ifdef STANDALONE
/*
 * $Compile: mk -m$ARCH %f
 * $Compile(AIX): xlc -DSTANDALONE -D_ALL_SOURCE -I. -g %f -o %F $mylib/mkcatdefs.a
 * $Compile(Sun4): /usr/5bin/cc -g -DSTANDALONE -I. %f -o %F $mylib/mkcatdefs.a
 */

char_t *progname;

main(int argc, char_t **argv)
{

#if defined(VISION_NUTC)
optind = 1;             /* as though it were used by getopt() */
#endif

	progname = argv[0];
	(void) fprintf(stderr, "%s\n", args_tilde_home("~mpt"));
	args_parse_opts(argc, argv);
	for ( /*void*/; optind < argc; optind++) {
		char_t *path = argv[optind];
		if (*path == '~')
			path = args_tilde_home(path);
		if (path == NULL || access(path, R_OK))
			(void) fprintf(stderr, "Can't open %s\n", argv[optind]);
		else
			(void) fprintf(stderr, "%s\n", path);
	}
	exit(0);
}
#endif
