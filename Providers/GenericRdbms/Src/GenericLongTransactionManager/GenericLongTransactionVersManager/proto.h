/******************************************************************************
 * (C) Copyright 2003 by Autodesk, Inc. All Rights Reserved.
 *
 * By using this code, you are agreeing to the terms and conditions of
 * the License Agreement included in the documentation for this code.
 *
 * AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE
 * CORRECTNESS OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE
 * IT. AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY
 * DISCLAIMS ANY LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL
 * DAMAGES FOR ERRORS, OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
 *
 * Use, duplication, or disclosure by the U.S. Government is subject
 * to restrictions set forth in FAR 52.227-19 (Commercial Computer
 * Software Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
 * (Rights in Technical Data and Computer Software), as applicable.
 * 
  *
 ******************************************************************************/

#ifndef VERS_PROTO_H
#define VERS_PROTO_H

#include "vers.h"

#ifdef __cplusplus
extern "C" {
#endif

//int                  vers_act_lk_set (
//                                vers_context            vctx, 
//                                FdoInt64                lock_id);
//int                  vers_act_lk_get (
//                                vers_context            vctx,
//                                FdoInt64                *lock_id);
int                  vers_activate (
                                vers_context            vctx,
                                FdoInt64                version);
int                  vers_add_dep (
                                vers_context            vctx,
                                FdoInt64                parent_version,
                                FdoInt64                version,
                                long                    gen,
                                int                     cycle_check,
                                int                     *establishes_cycle);
//int                  vers_chk_new_vers (
//                                vers_context            vctx, 
//                                FdoInt64                parent_version, 
//                                FdoInt64                next_version, 
//                                FdoInt64                version, 
//                                int                     add_dependency, 
//                                long                    *gen, 
//                                int                     *conflict, 
//                                FdoInt64                *lock_id);
int                  vers_clear_digr (
                                vers_context            vctx);
int                  vers_ctx_chk (
                                vers_context            vctx, 
                                vers_ctx_state          vctx_state, 
                                int                     *change_flags);
int                  vers_ctx_restore (
                                vers_ctx_state          vctx_state, 
                                vers_context            vctx);
int                  vers_ctx_save (
                                vers_context            vctx, 
                                vers_ctx_state          **vctx_state);
//int                  vers_dep_act (
//                                vers_context            vctx, 
//                                int                     flags, 
//                                FdoInt64                version);
//int                  vers_dep_deac (
//                                vers_context            vctx);
//int                  vers_dep_get (
//                                vers_context            vctx, 
//                                FdoInt64                *version, 
//                                int                     *eol);
//int                  vers_depend (
//                                vers_context            vctx, 
//                                FdoInt64                version, 
//                                int                     flags, 
//                                int                     *ndeplist, 
//                                FdoInt64                **deplist);
int                  vers_deplist_upd (
                                vers_context            vctx);
//int                  vers_gen_act (
//                                vers_context            vctx);
//int                  vers_gen_add (
//                                vers_context            vctx, 
//                                long                    generation);
//void                 vers_gen_alloc_reset_cache (
//                                char                    *new_db_name);
//int                  vers_gen_deac (
//                                vers_context            vctx);
//int                  vers_gen_get (
//                                vers_context            vctx, 
//                                long                    *gen, 
//                                int                     *eol);
//int                  vers_gen_get_max (
//                                vers_context            vctx, 
//                                long                    *generation);
int                  vers_gen_qual (
                                int                     opts, 
                                int                     ndep, 
                                FdoInt64                *dep, 
                                char                    *tab_name, 
                                char                    *ver_col,
                                char                    *nxtver_col, 
                                char                    *convert_fn_name, 
                                char                    **qual_buf, 
                                int                     *qbufsize);
int                  vers_gen_qual_mb (
                                int                     opts, 
                                int                     ndep, 
                                FdoInt64                *dep, 
                                int                     nidep, 
                                long                    *idep, 
                                char                    *tab_name, 
                                char                    *ver_col, 
                                char                    *nxtver_col, 
                                char                    *alias, 
                                int                     npkey, 
                                char                    **pkey, 
                                char                    *version_tab, 
                                char                    *version_ver_col, 
                                char                    *convert_fn_name, 
                                char                    **qual_buf, 
                                int                     *qbufsize); 
//int                  vers_get_act (
//                                vers_context            vctx, 
//                                FdoInt64                *version);
//int                  vers_get_dep (
//                                vers_context            vctx,
//                                FdoInt64                parent_version,
//                                FdoInt64                version,
//                                int                     *found,
//                                long                    *gen);
vers_context         vers_init ();
int                  vers_is_dag (
                                vers_context            vctx);
//int                  vers_is_dep (
//                                vers_context            vctx,
//                                FdoInt64                parent_version,
//                                FdoInt64                version);
//int                  vers_is_nvers_qry (
//                                vers_context            vctx);
//int                  vers_is_sel_row (
//                                vers_context            vctx,
//                                FdoInt64                version,
//                                FdoInt64                version_next,
//                                long                    generation,
//                                int                     base_scenario,
//                                int                     *is_sel_row);
void                 **vers_lhash_add (
                                vers_lhash_table_def    *table_p,
                                void                    *key_p);
void                 vers_lhash_clear (
                                vers_lhash_table_def    *table_p,
                                void                    (*free_func)());
vers_lhash_table_def *vers_lhash_create (
                                int                     base_size,
                                int                     upper,
                                int                     lower,
                                int                     key_type,
                                int                     size_of_key,
                                unsigned                (*hash_fn)(),
                                char                    *name_p);
void                 vers_lhash_destroy (
                                vers_lhash_table_def    *table_p,
                                void                    (*free_func)());
void                 **vers_lhash_lookup (
                                vers_lhash_table_def    *table_p,
                                void                    *key_p);
unsigned             vers_lhash_value_fixed (
                                char                    *key_p,
                                int                     size_of_key,
                                int                     size);
unsigned             vers_lhash_value_varying (
                                unsigned char           *key_p,
                                int                     len,
                                int                     size);
//int                  vers_mb_add_ver (
//                                vers_context            vctx,
//                                FdoInt64                version,
//                                char                    type,
//                                long                    scenario);
//int                  vers_mb_calc_next_ver (
//                                vers_context            version_ctx,
//                                FdoInt64                active_vers,
//                                FdoInt64                curr_vers,
//                                FdoInt64                *next_vers);
//int                  vers_mb_chk_valid (
//                                vers_context            vctx,
//                                FdoInt64                par_ver,
//                                FdoInt64                par_next_ver,
//                                FdoInt64                active_ver,
//                                int                     *conflict);
//vers_version_def     *vers_mb_find_ver (
//                                vers_context            vctx,
//                                FdoInt64                version);
//int                  vers_mb_get_info (
//                                vers_context            vctx,
//                                FdoInt64                version,
//                                char                    *type,
//                                long                    *base,
//                                long                    *scenario_version);
//int                  vers_mb_get_max_ver (
//                                vers_context            vctx,
//                                FdoInt64                *version);
//int                  vers_mb_get_scen_lst (
//                                vers_context            vctx,
//                                FdoInt64                *vers_lst,
//                                int                     vlen,
//                                long                    **base_lst,
//                                int                     *blen,
//                                long                    **scen_lst,
//                                int                     *slen);
//int                  vers_nv_override (
//                                vers_context            vctx,
//                                FdoInt64                version,
//                                FdoInt64                *old_version);
//int                  vers_opt_add (
//                                vers_context            vctx, 
//                                int                     opts);
//int                  vers_opt_del (
//                                vers_context            vctx, 
//                                int                     opts);
//int                  vers_opt_get (
//                                vers_context            vctx);
//int                  vers_opt_set (
//                                vers_context            vctx, 
//                                int                     opts);
int                  vers_sel_act (
                                vers_context            vctx, 
                                int                     flags);
//int                  vers_sel_add (
//                                vers_context            vctx, 
//                                FdoInt64                version);
int                  vers_sel_clear (
                                vers_context            vctx);
int                  vers_sel_deac (
                                vers_context            vctx);
int                  vers_sel_get (
                                vers_context            vctx,
                                FdoInt64                *version,
                                int                     *type,
                                int                     *eol);
int                  vers_sel_qual (
                                vers_context            vctx,
                                char                    *tab_name,
                                int                     version_only,
                                char                    *convert_fn_name, 
                                char                    **qual_buf,
                                int                     *qbufsize);
int                  vers_sel_qual_mb (
                                vers_context            vctx,
                                char                    *tab_name,
                                int                     version_only,
                                char                    **pkey,
                                int                     npkey,
                                char                    *convert_fn_name, 
                                char                    **qual_buf,
                                int                     *qbufsize);
int                  vers_term (
                                vers_context            vctx);
int                  vers_term_state (
                                vers_ctx_state          vctx_state);
//int                  vers_upd_mode_get (
//                                vers_context            vctx, 
//                                int                     *upd_mode);
//int                  vers_upd_mode_set (
//                                vers_context            vctx, 
//                                int                     upd_mode);
int                  vers_used_cache_add (
                                vers_context            vctx, 
                                char                    *table);
void                 vers_used_clear (
                                vers_context            vctx);
int                  vers_used_init (
                                vers_context            vctx);
void                 vers_used_term (
                                vers_context            vctx);


vers_context         vers_init ();
int                  vers_term (vers_context            vctx);
int                  vers_ctx_save (
                                vers_context            vctx,
                                vers_ctx_state          **vctx_state);
int                  vers_ctx_restore (
                                vers_ctx_state          vctx_state,
                                vers_context            vctx);
int                  vers_term_state (
                                vers_ctx_state          vctx_state);
int                  vers_ctx_chk (
                                vers_context            vctx,
                                vers_ctx_state          vctx_state,
                                int                     *change_flags);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

