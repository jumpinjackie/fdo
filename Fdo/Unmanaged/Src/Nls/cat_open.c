/*
 * 
* Copyright (C) 2004-2006  Autodesk, Inc.
* 
* This library is free software; you can redistribute it and/or
* modify it under the terms of version 2.1 of the GNU Lesser
* General Public License as published by the Free Software Foundation.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
* 
 */
/*NOTUSED*/

#include <errno.h>
#include <limits.h>
#include <Inc/Sys/stat.h>

#include <Inc/debugext.h>
#include <Inc/tfn.h>
#include <Inc/nls.h>
#include "func.h"


typedef struct Entry_s {            /* catalog cache entry */
    char            *catalog_name;  /*  catalog name */
    nl_catd         catalog_desc;   /*  catalog descriptor */
} Entry_t;

typedef struct Node_s {             /* linked list node */
    struct Node_s   *llink;         /*  left node of linked list */
    Entry_t         *entry;         /*  catalog cache entry */
    struct Node_s   *rlink;         /*  right node of linked list */
} Node_t;

typedef struct Cache_s {            /* catalog cache */
    unsigned int    limit;          /*  max number of cache entries */
    unsigned int    size;           /*  current number of cache entries */
    Node_t          *head;          /*  node of MRU cache entry */
} Cache_t;


/*
 *  The catalog cache: implemented as a finite doubly linked list of nodes
 *  of pointers to opened catalog entries.  The cache.head points to the
 *  node of the Most Recently Used catalog descriptor.  The head's left
 *  successor node points to the Least Recently Used catalog descriptor.
 *  The maximal cache size is stored in cache.limit, while the current
 *  cache size is stored in cache.size.
 *
 */
static Cache_t cat_cache = {
    NLS_CACHE_LIMIT, 0, NULL,
};


#if defined(mips)
/*
 *  Strdup returns a pointer to a new string, which is a duplicate of the
 *  string pointed by s1.  Space for the new string is obtained by using
 *  malloc().  A NULL pointer is returned if the new strings cannot be
 *  created.
 *
 *  Obviously Ultrix is non-yet SVID compliant!
 */
char *
strdup(s1)
register char *s1;
{
    register char *p;
    if ((p = (char *) malloc(strlen(s1) + 1)) == (char *) NULL)
        return(p);
    return(strcpy(p, s1));
}
#endif


/*
 * cat_alloc - Allocate a catalog cache entry, given by catalog name.
 *
 * Returns (Entry_t *) pointer to allocated cache entry upon success,
 * and NULL otherwise.
 */
static Entry_t * cat_alloc(char *cat_name)
{
    Entry_t *entry = NULL;

    entry = (Entry_t *)malloc(sizeof(Entry_t));

    if (entry != NULL) {
        char    *cpath;
        char    cname[PATH_MAX];
        char    *s;             /* duplicate catalog name */

        s = strdup(cat_name);               /* duplicate catalog name */
        if (s != NULL) {
            nl_catd c;

#if defined(SOLARIS)
            
            int                 exists = FALSE;
            struct stat         file_info;

            /* try to find catalog through env variable */
            if (amsut_nlspath_act())    {
                while (!exists) {
                    cpath = amsut_nlspath_get();
                    if (cpath == NULL) break;

                    if ((strlen(cpath)+strlen(cat_name)+2) >= PATH_MAX)
                        break;  
                    sprintf(cname, "%s/%s", cpath, cat_name);
                    /* Must check if file exists. catopen() does not return
                       any errors if file does not exists.
                    */
                    if (stat(cname,&file_info) != -1 )  
                        exists = TRUE;
                }
                amsut_nlspath_deac();
            }
            errno = 0;
            if (exists) {
                free(s);            
                s = strdup(cname);
                errno = 0;
                c = catopen(cname, 0);  /* open catalog */
            }
            else    { /* just try the catalog name */
                c = catopen(cat_name, 0);   /* open catalog */
            }   
#else
            c = catopen(cat_name, 0);   /* open catalog */    
#endif

            if (c == (nl_catd) -1) {
                char fullfilename[512];
                struct stat my_stat;
                const char *install = "/usr/local/fdo-3.6.0/nls/";

                // Determine the user-specified FDO install location
                char *fdo_home = getenv( "FDOHOME" );
                if ( NULL != fdo_home ) { 
                    install = fdo_home;
                }
            
                sprintf (fullfilename, "%s%s", "./", cat_name);
                if ((0 != stat (fullfilename, &my_stat)) || !S_ISREG(my_stat.st_mode)) {
                    // not found or not a file, try the install location
                    sprintf (fullfilename, "%s%s", install, cat_name);
                    if ((0 == stat (fullfilename, &my_stat)) && S_ISREG(my_stat.st_mode))
                        c = catopen(fullfilename, 0);   /* open catalog */
                }
            }

#if defined(OSF1)
            if (catgets(c, 1, 1, NULL) != NULL) {
#else
            if (c != (nl_catd) -1) {
#endif
                entry->catalog_name = s;        /* initialize cache entry */
                entry->catalog_desc = c;
            }
            else {
                free(s);
                free(entry);
                entry = NULL;
            }
        }
        else {
            free(entry);
            entry = NULL;
        }
    }
	return entry;	
}


/*
 * cat_free - Free the catalog cache entry, given by cache entry.
 *
 * Returns boolean TRUE upon success, and FALSE otherwise.
 */
static int cat_free(register Entry_t *entry)
{
    int status = 0;
    if (entry != NULL) {
        if (entry->catalog_name != NULL)
            free(entry->catalog_name);
        if (entry->catalog_desc != (nl_catd) -1)
            status = catclose(entry->catalog_desc);
        free(entry);
    }
    return(status == 0);
}


/*
 * dll_search - Search the circular doubly linked list
 * pointed to by *root for the node of catalog cache entry
 * given by catalog name.
 *
 * Returns (Note_t *) pointer to the node of the cache entry upon success,
 * and NULL otherwise.
 */
static Node_t * dll_search(register Node_t **root, register char *cat_name)
{
    register Node_t *p, *q;
    for (p = *root; p; p = p->rlink) {
        if (strcmp(cat_name, p->entry->catalog_name) == 0)
            return(p);                          /* entry found */
        if (p->rlink == *root)
            break;                              /* entry not found */
    }
    return(NULL);
}


/*
 * dll_insert - Insert into circular doubly linked list
 * (in front of node) pointed to by *root, by given cache entry.
 *
 * Returns (Note_t *) pointer to the node of the cache entry upon success,
 * and NULL otherwise.
 */
static Node_t * dll_insert(register Node_t **root, register Entry_t *entry)
{
    register Node_t *p, *q;
    if ((q = (Node_t *) malloc(sizeof(Node_t))) != NULL) {
        q->entry = entry;                   /* initialize new node */
        if (*root == NULL) {                /* empty list */
            p = *root = q;
            q->rlink = p;
        }
        else {
            p = (*root)->llink;
            q->rlink = p->rlink;            /* link new node */
        }
        q->llink = p;
        p->rlink = p->rlink->llink = q;     /* update new root */
    }
    return(q);
}


/*
 * dll_delete - Delete from circular doubly linked list
 * the node pointed to by *root and advance **root to the
 * rightmost successor, the next most recently used catalog
 * descriptor.
 *
 * Returns (Entry_t *) pointer to the cache entry of the
 * deleted node upon success, and NULL otherwise.
 */
static Entry_t * dll_delete(register Node_t **root)
{
    register Node_t *p, *q;
    register Entry_t *entry = NULL;
    if ((p = *root) != NULL) {              /* non-empty list */
        p->llink->rlink = p->rlink;         /* unlink node */
        p->rlink->llink = p->llink;
        q = p->rlink;
        if (p == q)                         /* list now empty */
            q = NULL;
        *root = q;                          /* new root */
        entry = p->entry;
        free(p);                            /* node deleted */
    }
    return(entry);                          /* return entry */
}


/*
 * dll_walk - Walk the circular doubly linked list
 * pointed to by *root performing by given action on
 * each node.
 *
 * Returns void.
 */
static void dll_walk(register Node_t **root, void (*action)(Node_t *))
{
    register Node_t *p;
    if (action) {
        for (p = *root; p; p = p->rlink) {
            (*action)(p);                       /* do action */
            if (*root == NULL || p->rlink == *root)
                break;                          /* end of list */
        }
    }
    return;
}



/*
 *  nls_cat_open - Open catalog given by catalog name.
 *
 *  If not already in the cache and the cache is not full, nls_cat_open()
 *  opens the given catalog if it exists and adds a new cache entry at
 *  the head of the cache.
 *  If not already in the cache and the cache is full, nls_cat_open()
 *  opens the given catalog if it exists, closes the LRU opened catalog
 *  making room for the new cache entry, and places the given catalog at
 *  the head of the cache.
 *  If already in the cache but not at the head of the cache, nls_cat_open()
 *  moves the given catalog to head of the cache.
 *  Upon success, return the catalog descriptor at the head of the cache,
 *  otherwise, return -1.
 *
 */
nl_catd nls_cat_open(char *cat_name)
{
    Cache_t *cache = &cat_cache;
    Node_t  *node;
    /*
     *  Debug
     */
    /*
     *  Search cache for catalog.
     */
    node = dll_search(&cache->head, cat_name);
    /*
     *  If catalog is not in the cache...
     */
    if (node == NULL) {
        /*
         *  If the cache is not full...
         */
        if (cache->size < cache->limit) {
            Entry_t *new_entry;
            /*
             *  Allocate a new cache entry.
             */
            new_entry = cat_alloc(cat_name);
            if (new_entry == NULL)
                goto the_exit;
            /*
             *  Add it to the head of the cache.
             */
            node = dll_insert(&cache->head, new_entry);
            if (node == NULL) {
                (void) cat_free(new_entry);
                goto the_exit;
            }
            cache->head = node;
            cache->size++;                  
        }
        /*
         *  Else the cache is full
         */
        else {
            Entry_t *new_entry, *lru_entry;
            /*
             *  Allocate a new cache entry.
             */
            new_entry = cat_alloc(cat_name);
            if (new_entry == NULL)
                goto the_exit;
            node = cache->head->llink;
            /*
             *  Remove and free the lru cache entry.
             */
            lru_entry = dll_delete(&node);
            if (lru_entry == NULL || !cat_free(lru_entry)) {
                (void) cat_free(new_entry);
                goto the_exit;
            }
            if (node == NULL)
                cache->head = NULL;
            /*
             *  Add the new cache entry to the head of the cache.
             */
            node = dll_insert(&cache->head, new_entry);
            if (node == NULL) {
                (void) cat_free(new_entry);
                goto the_exit;
            }
            cache->head = node;
        }
    }
    /*
     *  Else catalog is already in the cache, but not at the head of the cache.
     */
    else if (node != cache->head) {
        Entry_t *cur_entry;
        /*
         *  Remove the current cache entry from the cache.
         */
        cur_entry = dll_delete(&node);
        if (cur_entry == NULL)
            goto the_exit;
        /*
         *  Insert it at the head of the cache.
         */
        node = dll_insert(&cache->head, cur_entry);
        if (node == NULL)
            goto the_exit;
        cache->head = node;
    }
    /*
     *  Success, so return the catalog descriptor at the head of the cache.
     */
    return(cache->head->entry->catalog_desc);
    /*
     *  Failure, so return -1.
     */
the_exit:
    return((nl_catd) -1);
}


#ifdef STANDALONE
/*
 * $Compile: lint -uwP -I/victor -I. %f
 */

#include <stdio.h>

void
ident(n)
    Node_t *n;
{
    Entry_t *e = (Entry_t *) n->entry;
    (void) printf("%s ", e->catalog_name);
}


main(argc, argv)
    int argc;
    char *argv[];
{
    char catalog[BUFSIZ];
    nls_locale_set();
    while (scanf("%s", catalog) != EOF) {
        (void) printf("%s:\t", catalog);
        (void) nls_cat_open(catalog);
        dll_walk(&cat_cache.head, ident);
        (void) printf("\n");
    }
    exit(0);
}

#endif /* STANDALONE */

