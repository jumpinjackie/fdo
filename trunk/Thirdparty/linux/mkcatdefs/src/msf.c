/************************************************************************
 * Message source file (msf) handlers.
 *
 * Modification history
 *   Mnemonic    Release Date        Who     Review
 *
 *   NLSTOOLS    victor  93jan09     Mike Tardif
 *		Created.
 *
 *   NLSTOOLS    victor  93feb15     Mike Tardif
 *		Corrected "if (msf.m_msgno > NL_SETMAX)" statement.
 *
 *   NLSTOOLS    victor  93may17     Mike Tardif
 *		Added msf_strrchr() to ensure message strings are quote terminated.
 *
 *   OSFPORT    	tools  	94jun30     Mike Tardif
 *       Tweaked for OSF1 and Solaris ports.
 *
 *   HPUXPORT   	tools  	95mar17     Weibo Wang
 *       For HP-UX ports.
 *
 *   HPUXPORT   	tools  	95mar21     Weibo Wang
 *       For HP-UX ports ($delset, $set, and $quote).
 *
*   NUTC        cyclops     97Apr23     Doug Carr                       *
*       Nutcracker port
*
 *  ID: $RCSfile: msf.c $ $Revision: 4.4 $ $Date: 1997/04/29 17:53:37 $ $Author: doug $
 ************************************************************************/
/*LINTLIBRARY*/
/*NOTUSED*/

#include <stdio.h>			/* for def'n of fopen(), fgets(), ... */
#include <stdlib.h>			/* for def'n of strtol(), malloc() */
#include <ctype.h>			/* for def'n of toupper(), isdigit(), ... */
#include <string.h>			/* for def'n of strcpy(), strlen(), ... */
#include <search.h>			/* for def'n of hcreate(), hsearch() */
#include <sys/param.h>		/* for def'n of MAXPATHLEN */
#include <mkcatdefs.h>		/* for def'n of msg_issue_error(), ... */
#if defined(_AIX)
#include <nl_types.h>		/* for def'n of NL_SETMAX, ... */
#endif
#if defined(ultrix) || defined(SVR4) || defined(HPUX) 
#include <limits.h>			/* for def'n of NL_SETMAX, ... */
#endif
#if defined(sun) && !defined(SVR4)		/* avoids using -L/usr/xpg2include */
#include "/usr/xpg2include/limits.h"	/* for def'n of NL_SETMAX, ... */
#endif


#define	MAX_ENTRY	   2048	/* estimate maxno of set and message identifiers */
#define NL_SETD	  	      1 /* default set identifier number */
#define NL_IDMAX         64 /* suprema of max length of msf identifiers */

#if defined(VISION_NUTC) || defined(LINUX)
#define NL_SETMAX       255 /* suprema of max set identfier number */
#define NL_MSGMAX     32767 /* suprema of max message identfier number */
#endif

#if		NL_SETMAX > 255
#undef	NL_SETMAX	
#define NL_SETMAX	  	255	/* suprema of max set identfier number */
#endif
#if		NL_MSGMAX > 32767
#undef	NL_MSGMAX
#define NL_MSGMAX	  32767 /* suprema of max message identfier number */
#endif
#if		NL_TEXTMAX > 255
#undef	NL_TEXTMAX
#define NL_TEXTMAX	  	255	/* suprema of max message length */
#endif


enum Directive_e {
	D_TEXT, D_COMMENT, D_SEPARATOR, D_DELSET, D_SET, D_QUOTE 
};
#define	N_DIRECTIVES	5			/* = D_QUOTE */

struct Directive_s {
	enum Directive_e  d_id;			/* associative directive array index */
	char_t *d_name;					/* directive name */
};


static struct Directive_s directives[N_DIRECTIVES + 1] = {
	{ D_TEXT, 		NULL },			/* sentinel must be NULL */
	{ D_COMMENT,	"$ " },
	{ D_SEPARATOR, 	"$separator " },
	{ D_DELSET,		"$delset " },
	{ D_SET,		"$set " },
	{ D_QUOTE,		"$quote " },
};

static struct Msf_s {
	char_t m_fname[MAXPATHLEN + 1];	/* message source header file name */
	char_t m_catname[NL_IDMAX + 1];	/* upcased message source catalog name */
	FILE *m_fp;						/* message source header file pointer */
	int	m_setno;					/* next available set number */
	int m_msgno;					/* next available message number */
	char_t m_separator;				/* message separator character */
	char_t m_quote;					/* message quote character */
	char_t *m_stab;					/* string table base pointer */
	char_t *m_stabptr;				/* current string table pointer */
	int m_stabsz;					/* current string table size */
} msf;


#if	defined(sun)
extern int hcreate PROTO((unsigned nel));
extern ENTRY *hsearch PROTO((ENTRY item, ACTION action));
extern void hdestroy PROTO((void));
#endif


/*
 * msf_strrchr - return last occurence.
 *
 * Return pointer to last occurence of `c', ensuring
 * that last occurence of `c' is not escaped
 * in string `cs' or NULL if not present or unescaped.
 */
char *
#ifdef	__STDC__
msf_strrchr(char_t *cs, int c)
#else
msf_strrchr(cs, c)
			register char_t *cs;
			register int c;
#endif
{
	register char_t *p;
	if ((p = strrchr(cs, c)) == NULL)
		return (NULL);
	if (p[-1] == '\\')
		return (NULL);
	return (p);
}


/*
 * msf_mk_cat_defs - read msf file
 *
 * Parse contents of msf file `path',
 * translating set and message identifers
 * to set and message numbers, and generating
 * corresponding #define's in msf header file.
 * Returns void upon success, and panics on error.
 */
void
#ifdef	__STDC__
msf_mk_cat_defs(char_t *path)
#else
msf_mk_cat_defs(path)
				register char_t *path;
#endif
{
	char_t *tp;
	int lineno = 0;
	int continuation = 0;
	FILE *fp;
	/*
	 *	If cannot read msf file then issue warning.
	 */
	if ((fp = fopen(path, "r")) == NULL)
		msg_issue_info(msg13, path);
	/*
	 *	If can't write preamble to header file then issue warning.
	 */
	if (fprintf(msf.m_fp,
				"\n\n/* The following was generated from %s. */\n\n", path) < 0)
		msg_issue_info(msg10, msf.m_fname);
	/*
	 *	For each msf file input line do ...
	 */
	while (1) {
		char_t line[BUFSIZ];
		register char_t *lp;
		register int n;
		/*
		 *	If at eof then ...
		 */
		if ((lp = fgets(line, BUFSIZ, fp)) == NULL) {
			/*
			 *	If were in a continuation line then issue warning.
			 */
			if (continuation)
				msg_issue_info(msg12, lineno, path);
			/*
			 *	We're done.
			 */
			break;
		}
		lineno++;
		/*
		 *	If we're in a continuation line then ...
		 */
		if (continuation) {
			/*
			 *	If can't print continuation line then issue warning.
			 */
			if (printf("%s", lp) < 0)
				msg_issue_info(msg20);
			/*
			 *	If still in a continuation line then flag it.
			 */
			n = strlen(lp);
			continuation = (n > 2 && lp[n - 2] == '\\');
			/*
			 *	If missing terminating quote then issue warning.
			 */
			if (!continuation && msf_strrchr(lp, msf.m_quote) == NULL) 
				msg_issue_info(msg16, lineno, path);
			continue;
		}
		/*
		 *	Discart blank lines.
		 */
		while (isspace(*lp) && *lp != '\n')
			lp++;
		if (*lp == '\n')
			continue;
		/*
		 *	If a directive is encountered then ...
		 */
		if (*lp == '$') {
			register struct Directive_s *d = directives;
			/*
			 *	For each type of directive do ...
			 */
			for (d = &directives[N_DIRECTIVES]; d->d_name; d--) {
				/*
				 *	If directive found then ...
				 */
				if (strncmp(lp, d->d_name, strlen(d->d_name)) == 0) {
					/*
					 *	Advance to character beyond directive name and break.
					 */
					lp += strlen(d->d_name);
					break;
				}
			}
			switch (d->d_id) {
			case D_COMMENT:
				/*
				 *	$ [comment]
				 */
				/*
				 *	If can't print comment lines then issue warning.
				 */
				if (printf("%s%s", d->d_name, lp) < 0)
					msg_issue_info(msg20);
				break;
			case D_SEPARATOR:
				/*
				 *	$separator char_t [comment]
				 */
				/*
				 *	If no char_t or no trailing space then issue warning.
				 */
				if (*lp == '\0' || *lp == '\n' || !isspace(*(lp + 1)))
					msg_issue_info(msg11, lp, lineno, path);
				msf.m_separator = *lp;
				/*
				 *	If can't print separator directives then issue warning.
				 */
				if (printf("%s%s", d->d_name, lp) < 0)
					msg_issue_info(msg20);
				break;
			case D_QUOTE:
				/*
				 *	$quote char_t [comment]
				 */
				/*
				 *	If no char_t or no trailing space then issue warning.
				 */
				if (*lp == '\0' || *lp == '\n' || !isspace(*(lp + 1)))
					msg_issue_info(msg11, lp, lineno, path);
				msf.m_quote = *lp;
				/*
				 *	If can't print quote directives then issue warning.
				 */
#if !defined(HPUX)
				if (printf("\n%s%s\n", d->d_name, lp) < 0)
					msg_issue_info(msg20);
#endif
				break;
			case D_DELSET:
				/*
				 *	$delset n [comment]
				 */
				/*
				 *	If no number or no trailing space then issue warning.
				 */
				n = strtol(lp, &tp, 10);
				if (n <= 0 || !isspace(*tp))
					msg_issue_info(msg11, lp, lineno, path);
				/*
				 *	If can't print delset directives then issue warning.
				 */
#if !defined(HPUX) && ! defined(VISION_NUTC)
				if (printf("%s%s", d->d_name, lp) < 0)
					msg_issue_info(msg20);
#endif
				break;
			case D_SET:
				/*
				 *	$set n [comment]
				 */
				if (isdigit(*lp)) {
					/*
					 *	Extract set number.
					 */
					n = strtol(lp, &tp, 10);
					/*
					 *	If invalid format encountered then issue warning.
					 */
					if (!isspace(*tp))
						msg_issue_info(msg11, lp, lineno, path);
					/*
					 *	If set number hasn't increased then issue warning.
					 */
					if (n <= msf.m_setno)
						msg_issue_info(msg18, n, lineno, path);
					/*
					 *	If set number exceeds maximum then issue warning.
					 */
					msf.m_setno = n;
					msf.m_msgno = 0;
					if (msf.m_setno > NL_SETMAX)
						msg_issue_info(msg19, msf.m_setno);
					/*
					 *	If can't print set directives then issue warning.
					 */
#if defined(HPUX) || defined(VISION_NUTC)
					if (printf("%s%u\n", d->d_name,  msf.m_setno)< 0)
						msg_issue_info(msg20);
#else
					if (printf("$delset %u\n%s%u\n",
								msf.m_setno, d->d_name, msf.m_setno) < 0)
						msg_issue_info(msg20);
#endif
					/*
					 *	If can't write to header file then issue warning.
					 */
					if (fprintf(msf.m_fp, "\n/* definitions for set %u */\n",
													msf.m_setno) < 0)
						msg_issue_info(msg10, msf.m_fname);
				}
				/*
				 *	$set identifier [comment]
				 */
				else if (isalpha(*lp) || *lp == '_') {
					char_t ident[NL_IDMAX + 1];
					/*
					 *	Scan an identfier.
					 */
					tp = lp;
					while (isalnum(*lp) || *lp == '_')
						lp++;
					/*
					 *	If invalid format encountered then issue warning.
					 */
					if (!isspace(*lp))
						msg_issue_info(msg11, lp, lineno, path);
					/*
					 *	If set identifier is too long then issue warning.
					 */
					if (lp - tp + 1 > NL_IDMAX)
						msg_issue_info(msg15, lineno, tp);
					/*
					 *	Generate local copy of identifier.
					 */
					strncpy(ident, tp, lp - tp), ident[lp - tp] = '\0';
					/*
					 *	If set number exceeds maximum then issue warning.
					 */
					msf.m_setno += 1;
					msf.m_msgno = 0;
					if (msf.m_setno > NL_SETMAX)
						msg_issue_info(msg19, msf.m_setno);
					/*
					 *	Add set identifier to hash table.
					 */
					msf_add_identifier(ident);
					/*
					 *	If can't print set directives then issue warning.
					 */
#if defined(HPUX) || defined(VISION_NUTC)
					if (printf("%s%u\n", d->d_name,  msf.m_setno)< 0)
						msg_issue_info(msg20);
#else
					if (printf("$delset %u\n%s%u\n",
								msf.m_setno, d->d_name, msf.m_setno) < 0)
						msg_issue_info(msg20);
#endif
					/*
					 *	If can't print to header file then issue warning.
					 */
					if (fprintf(msf.m_fp,
						"\n/* definitions for set %s */\n#define %s %u\n\n",
												ident, ident, msf.m_setno) < 0)
						msg_issue_info(msg10, msf.m_fname);
				}
				/*
				 *	Else unknown directive encountered so issue warning.
				 */
				else {
					msg_issue_info(msg11, lp, lineno, path);
				}
				break;
			default:
				/*
				 *	Unknown directive encountered, issue warning.
				 */
				msg_issue_info(msg11, lp, lineno, path);
				break;
			}
		}
		/*
		 *	Else if number encounter then scan a line in the form:
		 *		<message number><separator><quote><message string><quote>
		 */
		else if (isdigit(*lp)) {
			/*
			 *	Extract message number.
			 */
			n = strtol(lp, &tp, 10);
			/*
			 *	If following character is not a separator then issue warning.
			 */
			if (*tp++ != msf.m_separator)
				msg_issue_info(msg17, lineno, path);
			/*
			 *	If following character is not a quote then issue warning.
			 */
			if (*tp++ != msf.m_quote)
				msg_issue_info(msg16, lineno, path);
			/*
			 *	If its a continuation line then flag it.
			 */
			continuation = (strlen(tp) > 2 && tp[strlen(tp) - 2] == '\\');
			/*
			 *	If missing terminating quote then issue warning.
			 */
			if (!continuation && msf_strrchr(tp, msf.m_quote) == NULL) 
				msg_issue_info(msg16, lineno, path);
			/*
			 *	If message number isn't strictly increasing then issue warning.
			 */
			if (n <= msf.m_msgno)
				msg_issue_info(msg18, n, lineno, path);
			/*
			 *	If message number exceeds maximum then issue warning.
			 */
			msf.m_msgno = n;
			if (msf.m_msgno > NL_MSGMAX)
				msg_issue_info(msg19, msf.m_msgno);
			/*
			 *	If can't print message number lines then issue warning.
			 */
			if (printf("%s", lp) < 0)
				msg_issue_info(msg20);
		}
		/*
		 *	Else if alphabetic then scan a line in the form:
		 *		<message identifier><separator><quote><message string><quote>
		 */
		else if (isalpha(*lp) || *lp == '_') {
			char_t ident[NL_IDMAX + 1];
			/*
			 *	Scan an identfier.
			 */
			tp = lp;
			while (isalnum(*lp) || *lp == '_')
				lp++;
			/*
			 *	If message identifier is too long then issue warning.
			 */
			if (lp - tp > NL_IDMAX)
				msg_issue_info(msg15, lineno, path);
			/*
			 *	Generate local copy of identifier.
			 */
			strncpy(ident, tp, lp - tp), ident[lp - tp] = '\0';
			/*
			 *	If following character is not a separator then issue warning.
			 */
			tp = lp;
			if (*lp++ != msf.m_separator)
				msg_issue_info(msg17, lineno, path);
			/*
			 *	If following character is not a quote then issue warning.
			 */
			if (*lp++ != msf.m_quote)
				msg_issue_info(msg16, lineno, path);
			/*
			 *	If its a continuation line then flag it.
			 */
			n = strlen(lp);
			continuation = (n > 2 && lp[n - 2] == '\\');
			/*
			 *	If missing terminating quote then issue warning.
			 */
			if (!continuation && msf_strrchr(lp, msf.m_quote) == NULL) 
				msg_issue_info(msg16, lineno, path);
			/*
			 *	If message number exceeds maximum then issue warning.
			 */
			msf.m_msgno += 1;
			if (msf.m_msgno > NL_MSGMAX)
				msg_issue_info(msg19, msf.m_msgno);
			/*
			 *	Add message identifier to hash table.
			 */
			msf_add_identifier(ident);
			/*
			 *	If can't print message directives then issue warning.
			 */
			if (printf("%u%s", msf.m_msgno, tp) < 0)
				msg_issue_info(msg20);
			/*
			 *	If can't print #define to header file then issue warning.
			 */
			if (fprintf(msf.m_fp, "#define %s %u\n", ident, msf.m_msgno) < 0)
				msg_issue_info(msg10, msf.m_fname);
		}
		/*
		 *	Else unknown directive encountered so issue warning.
		 */
		else {
			msg_issue_info(msg11, lp, lineno, path);
		}
	}
	return;
}


/*
 * msf_create_header - create msf header file
 *
 * Create msf header file from `catname'.
 * initializing set and message number,
 * quote and separator characters and
 * string and hash tables.
 * Returns void upon success, and panics on error.
 */
void
#ifdef	__STDC__
msf_create_header(char_t *catname)
#else
msf_create_header(catname)
				  register char_t *catname;
#endif
{
	register char_t *tp;
	char_t *suffix = "_msg.h";
	/*
	 *  If catalog name is too long then issue error.
	 */
	if (strlen(catname) > NL_IDMAX)
		msg_issue_error(msg03, catname);
	/*
	 *	Upcase catalog name.
	 */
	(void) strcpy(msf.m_catname, catname);
	for (tp = msf.m_catname; *tp; tp++)
		*tp = toupper(*tp);
	/*
	 *  If path name catalog[_msg.h] is too long then issue error.
	 */
	if (strlen(catname) + strlen(suffix) > MAXPATHLEN)
		msg_issue_error(msg01, catname);
	/*
	 *	Use catname.msf as filename.
	 */
	(void) sprintf(msf.m_fname, "%s%s", catname, suffix);
	/*
	 *	If cannot create msf header file then issue warning.
	 */
	if ((msf.m_fp = fopen(msf.m_fname, "w")) == NULL)
		msg_issue_info(msg10, msf.m_fname);
	/*
	 *  Pre-initialize msf struct.
	 */
	msf.m_setno = NL_SETD -1;
	msf.m_msgno = 0;
	msf.m_separator = ' ';
	msf.m_quote = '"';
	/*
	 *	Initialize symbol table space to an eight of the maximum possible.
	 */
	msf.m_stabsz = (MAX_ENTRY * (NL_IDMAX / 8));
	/*
	 *	If can't allocate string table space then issue error.
	 */
	if ((msf.m_stab = malloc(msf.m_stabsz)) == NULL)
		msg_issue_error(msg02);
	msf.m_stabptr = msf.m_stab;
	/*
	 *	If can't create hash table then issue error.
	 */
	if (hcreate(MAX_ENTRY) == 0)
		msg_issue_error(msg02);
	/*
	 *	If can't write header file preamble then issue warning.
	 */
	if (fprintf(msf.m_fp, "#ifndef _H_%s_MSG \n", msf.m_catname) < 0)
		msg_issue_info(msg10, msf.m_fname);
	if (fprintf(msf.m_fp, "#define _H_%s_MSG \n", msf.m_catname) < 0)
		msg_issue_info(msg10, msf.m_fname);
	if (fprintf(msf.m_fp, "#include <limits.h>\n") < 0)
		msg_issue_info(msg10, msf.m_fname);
	if (fprintf(msf.m_fp, "#include <nl_types.h>\n") < 0)
		msg_issue_info(msg10, msf.m_fname);
	if (fprintf(msf.m_fp, "#define MF_%s \"%s.cat\"\n\n",
								msf.m_catname, catname) < 0)
		msg_issue_info(msg10, msf.m_fname);
	return;
}


/*
 * msf_flush_header - flush msf header file
 *
 * Write postamble to msf header file and optionally close
 * header file, or flush msf header file and leave the file
 * open, or remove msf header file.  If errors were found,
 * then on close, unlink msf header file instead.
 * Returns TRUE upon success, FALSE on error.
 */
void
#ifdef	__STDC__
msf_flush_header(int op)
#else
msf_flush_header(op)
				 int op;
#endif
{
	if (op == DO_CLOSE && msg_errors_found())
		op = DO_UNLINK;
	/*
	 *	If want to close file then ...
	 */
	if (op == DO_CLOSE) {
		/*
		 *	If can't write postamble then issue warning.
		 */
		if (fprintf(msf.m_fp, "#endif \n") < 0)
			msg_issue_info(msg10, msf.m_fname);
		/*
		 *	If can't close msf file then issue warning.
		 */
		if (fclose(msf.m_fp) == EOF)
			msg_issue_info(msg10, msf.m_fname);
		msg_issue_info(msg06, msf.m_fname);
	}
	/*
	 *	Else if want to flush file then ...
	 */
	else if (op == DONT_CLOSE) {
		/*
		 *	If can't flush msf file then issue warning.
		 */
		if (fflush(msf.m_fp) == EOF)
			msg_issue_info(msg10, msf.m_fname);
	}
	/*
	 *	Else if want to unlink then ...
	 */
	else if (op == DO_UNLINK) {
		/*
		 *	Remove it.
		 */
		(void) remove(msf.m_fname);
		msg_issue_info(msg05, msf.m_fname);
	}
	/*
	 *	Else unknown operator so issue error.
	 */
	else {
		msg_issue_error(msg00, __FILE__, __LINE__);
	}
	return;
}


/*
 * msf_add_identifier - add identifier to hash table.
 *
 * Add identifier `ident' to both string and
 * hash table.  If `ident' is already in hash
 * table, then a duplicate is found.  Otherwise
 * add `ident' to hash table.  Keep doubling
 * size of string table as needed until hash
 * table overflows.
 * Returns void upon success, and panics on error.
 */
void
#ifdef	__STDC__
msf_add_identifier(char_t *ident)
#else
msf_add_identifier(ident)
				   register char_t *ident;
#endif
{
	ENTRY item;
	ENTRY *found_item;
	/*
	 *	If out of string table space then ...
	 */
	if (msf.m_stabptr + strlen(ident) + 1 >= msf.m_stab + msf.m_stabsz) {
		char_t *tp;
		/*
		 *	Double string table space size.
		 */
		msf.m_stabsz *= 2;
		/*
		 *	If can't double string table space then issue error.
		 */
		if ((tp = realloc(msf.m_stab, msf.m_stabsz)) == NULL)
			msg_issue_error(msg02);
		/*
		 *	Re-initialize string table pointers.
		 */
		msf.m_stabptr = tp + (msf.m_stabptr - msf.m_stab);
		msf.m_stab = tp; 
	}
	/*
	 *	Initialize hash table entry.
	 */
	item.key = strcpy(msf.m_stabptr, ident);
	item.data = NULL;
	msf.m_stabptr += strlen(ident) + 1;
	/*
	 *	If item is already present in hash table then issue warning.
	 */
	if (hsearch(item, FIND) != NULL)
		msg_issue_info(msg14, item.key);
	/*
	 *	If hash table is full then issue error.
	 */
	if (hsearch(item, ENTER) == NULL)
		msg_issue_error(msg04);
	return;
}



#ifdef	STANDALONE
/*
 * $Compile: mk -m$ARCH %f
 * $Compile(AIX): xlc -g -DSTANDALONE -D_ALL_SOURCE -I. %f -o %F $mylib/mkcatdefs.a
 * $Compile(Sun4): /usr/5bin/cc -g -DSTANDALONE -I. %f -o %F $mylib/mkcatdefs.a
 * $Compile(LINT): lint -ua -D_ALL_SOURCE -I. %f
 */

char_t *progname;

main(argc, argv)
char_t *argv[];
{
	progname = argv[0];
	msf_create_header("junk");
	msf_add_identifier("fish");
	msf_add_identifier("fred");
	msf_add_identifier("fish");
	msf_flush_header(DO_CLOSE);
}
#endif
