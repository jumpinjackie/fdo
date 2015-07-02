// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

/************************************************************************
*                                                                       *
* Name                                                                  *
*   ut.h - header defining common macros and functions                  *
*                                                                       *
* Synopsis                                                              *
*   #include    <Inc/ut.h>                                              *
*                                                                       *
* Description                                                           *
*       This  header  defines  the macros and functions which are       *
*       required by many applications,  and  are  not application       *
*       specific.                                                       *
*                                                                       *
* Parameters                                                            *
*   N/A                                                                 *
*                                                                       *
* Function value                                                        *
*   N/A                                                                 *
*                                                                       *
* Remarks                                                               *
*                                                                       *
*                                                                       *
************************************************************************/

#ifndef UT_H
#define UT_H    1

#include    <Inc/os.h> 
#include    <Inc/limits.h>
#include    <Inc/datum.h>
#include    <Inc/types.h>
#include    <Inc/stdlib.h>
#include    <Inc/time.h>
#include    <Inc/tfn.h>
#include    <Inc/ctype.h>
#include    <Inc/mio.h>

#include <time.h>

#ifndef FILE
#include    <stdio.h>
#endif

#ifndef HUGE_VAL
#include    <math.h>                        /* needed for atan2, M_PI */
#endif  /* HUGE_VAL */

#ifdef C_PLUSPLUS
extern "C" 
{
#endif

        /***    constants   ***/

#define UT_LINE_SIZE        1024

#define WHITE_SPACE         " \t\n\f"
#define SINGLE_QUOTE        '\047'
#define DOUBLE_QUOTE        '\042'
#define HUGEINT             0x7fffffff      /* largest positive int */
#ifndef MS_WINDOWS
#define TINYINT             0x80000000      /* largest negative int */
#else
#define TINYINT             INT_MIN
#endif

#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif

        /***    keyword matching routine special return codes       ***/

#define NO_MATCH            -1              /* keyword has not been matched */
#define AMBIGUOUS           -2              /* keyword is ambiguous */

        /***    ut_whatis arbitrary conversion routine return codes     ***/

#define UT_TYPE_OTHER           000000  /* no match         */
#define UT_TYPE_INT             000001  /* an integer       */
#define UT_TYPE_DOUBLE          000002  /* a float number   */
#define UT_TYPE_OPER            000004  /* an operator / + - / etc          */
#define UT_TYPE_LITERAL         000010  /* quoted ascii     */
#define UT_TYPE_ASCII           000020  /* unquoted ascii   */

#define UT_TYPE_DOLLAR          000100
#define UT_TYPE_AT              000200
#define UT_TYPE_DOT             000400

#define UT_TYPE_DOLLAR_ASCII    000120  /* unquoted ascii starting with '$' */
#define UT_TYPE_AT_ASCII        000220  /* unquoted ascii starting with '@' */
#define UT_TYPE_DOT_ASCII       000420  /* unquoted ascii containing one '.' */

#define UT_TYPE_AT_DOT_ASCII    000620  /* unquoted ascii starting with an '@' and containing a dot */

        /***    edit file status constants  ***/

#define UT_EDIT_CLOSE           1       /* if edit file is closed.          */
#define UT_EDIT_READ            2       /* if edit file is opened for read  */
#define UT_EDIT_WRITE           3       /* if edit file is opened for write */

        /***    ut_pmpt_reply() constants   ***/

#define UT_NOECHO               FALSE
#define UT_ECHO                 TRUE
#define UT_ECHO_IF_IACTIVE      FALSE+TRUE+1

        /***    our macros  ***/

#define UT_STRSET(ptr) ((ptr != NULL && ptr[0] != '\0')?TRUE:FALSE)
#define UT_PTRSET(ptr) ((ptr != NULL)?TRUE:FALSE)

        /***    macros      ***/

/************************************************************************
*       Obsolete - have to be removed over time                         *
************************************************************************/
#ifdef abs
#undef abs
#endif
#define abs(x)                  ((x)<0 ? -(x) : (x))    /*  obsolete    */
#define between(v,l,h)          (((v) >= ut_min((l), (h))) && ((v) <= ut_max((l), (h))))
#define rem(a,b)                ((a)>=0 ? (a)%(b) : ((a)%(b)==0 ? 0 : (b)+(a)%(b)))

#define ut_abs(x)               ((x)<0 ? -(x) : (x))
#define ut_between(v,l,h)       (((v) >= ut_min((l), (h))) && ((v) <= ut_max((l), (h))))
#define ut_div(a,b)             ((a)>=0 ? (a)/(b) : ((a)+1)/(b)-1)
#define ut_min(a,b)             ((a) < (b) ? (a) : (b))
#define ut_max(a,b)             ((a) > (b) ? (a) : (b))
#define ut_rem(a,b)             ((a)>=0 ? (a)%(b) : ((a)%(b)==0 ? 0 : (b)+(a)%(b)))
#define strtcpy(dst,src,n)      strncpy(dst,src,n);dst[n]='\0'
#define try(func)               if (!func) return(FALSE)
#define ut_square(x)            ((x) * (x))
#define ut_atan2(y,x)           (((x)==0 && (y)==0) ? 0 : atan2(y,x))
#define ut_copy(dst,src,type)   ut_memcpy((char *)(dst), (char *)(src), sizeof(type))
#define ut_deg_to_rad(d)        ((d)*M_PI/180)
#define ut_rad_to_deg(r)        ((r)*180/M_PI)
#define ut_free(ptr)            if (ptr!=0) free(ptr)
#define ut_memchr(s,c,n)        memchr(s,c,n)
#define ut_memcpy(d,s,n)        memcpy(d,s,n)
#define ut_memset(s,c,n)        memset(s,c,n)
#define ut_round(x)             ((Int32) (((x) >= 0) ? (x+0.5) : (x-0.5)))
#define ut_tolower(c)           (tolower((unsigned char) (c)))
#define ut_toupper(c)           (toupper((unsigned char) (c)))
#define ut_toascii(c)           (toascii((unsigned char) (c)))
#define ut_isalnum(c)           (isalnum((unsigned char) (c)))
#define ut_isalpha(c)           (isalpha((unsigned char) (c)))
#define ut_iscntrl(c)           (iscntrl((unsigned char) (c)))
#define ut_isprint(c)           (isprint((unsigned char) (c)))
#define ut_isspace(c)           (isspace((unsigned char) (c)))
#define ut_isdigit(c)           (isdigit((unsigned char) (c)))
#define ut_isxdigit(c)          (isxdigit((unsigned char) (c)))
#define ut_isupper(c)           (isupper((unsigned char) (c)))
#define ut_islower(c)           (islower((unsigned char) (c)))
#define ut_isascii(c)           (isascii((unsigned char) (c)))
#define ut_ispunct(c)           (ispunct((unsigned char) (c)))
#if defined(unix) || defined(HPUX)
#define ut_unlink(f)            unlink(f)
#define vmalloc(a)              ut_vm_malloc(__FILE__, (unsigned) (a))  /* Obsolete */
#define vmfree(p)               ut_vm_free(__FILE__, p)                 /* Obsolete */
#endif
#ifdef  VMS
#define ut_unlink(f)            delete(f)
#define vmalloc(a)              ut_vm_malloc("anonymous", (unsigned)(a))
#define vmfree(p)               ut_vm_free("anonymous", p)
#endif

#ifdef MS_WINDOWS
#define ut_unlink(f)            _unlink(f)
#define ut_vfscanf              vfscanf
#define ut_vmscanf              vmscanf
#endif


#ifndef MS_WINDOWS
#define ut_puts                 puts
#define ut_vprintf              vprintf
#define ut_sscanf               sscanf
#define ut_fscanf               fscanf
#define ut_flush()              fflush(stdout)
#define ut_printf               printf
#endif

#define ut_compare_pointers(p1, p2) (((long)(p1)) - ((long)(p2)))

typedef enum {
    UT_FOPEN_CLOSE_ON_EXEC,
    UT_FOPEN_NO_CLOSE_ON_EXEC
    } ut_fopen_close_on_exec_type;

typedef enum {
    UT_FOPEN_EOL_TEXT,
    UT_FOPEN_EOL_BINARY,
    UT_FOPEN_EOL_DONT_CARE
    } ut_fopen_eol_type;


        /***    external routine declarations   ***/

#if defined(FULLPROTO) || defined(c_plusplus) || defined(__cplusplus) 

typedef int (*ut_fnCompare)(const void * elem1, const void * elem2);
typedef void (*ut_fnAssign)(void * element1, const void * element2);
typedef void (*ut_fnSwap)(void * element1, void * element2);
typedef int (*ut_fnApply)(void * element1, void * element2);


long ut_abs_long(
        long i
        );
int ut_acute_ang(
        double x1,
        double y1,
        double x2,
        double y2,
        double x3,
        double y3,
        int side
        );
int ut_ad_fil_ext(
        char * filename,
        char * ext
        );
int ut_alarm_off(
        int pid
        );
int ut_alarm_on(
        unsigned int seconds
        );
int ut_architecture(
        char * sysname,
        char * machine
        );

char * ut_asctoint(
        char * string,
        int * number
        );
char * ut_atod(
        char * string,
        double * number
        );
int ut_atodeg(
        char * string,
        double * degrees
        );
int ut_atodegrees(
        char * string,
        double * degrees
        );
char * ut_atoi(
        char * string,
        int * number
        );
char * ut_atol(
        char * string,
        long * number
        );
double ut_azi_2_bear(
        double azimuth,
        char * quad_start,
        char * quad_end
        );
int ut_bit_get(
        unsigned char * array,
        int bit_num
        );
void ut_bit_clr(
        unsigned char * array,
        int bit_num
        );
void ut_bit_set(
        unsigned char * array,
        int bit_num
        );
char * ut_bsearch(
        char * key,
        char * base,
        unsigned int nel,
        unsigned int elm_size,
        ut_fnCompare compare
        );
void ut_base_ext(
        char * filespec,
        char * base,
        char * ext
        );
int ut_blk_a_str(
        char * s,
        char subs[]
        );
int ut_buf_offset(
        double offset,
        double px,
        double py,
        double cx,
        double cy,
        double nx,
        double ny,
        int * arc,
        double * x1,
        double * y1,
        double * x2,
        double * y2,
        double * arc_angle_1,
        double * arc_angle_2
        );
int ut_buf_offset_z(
        double offset,
        double px,
        double py,
        double pz,
        double cx,
        double cy,
        double cz,
        double nx,
        double ny,
        double nz,
        int * arc,
        double * x1,
        double * y1,
        double * z1,
        double * x2,
        double * y2,
        double * z2,
        double * arc_angle_1,
        double * arc_angle_2
        );
int ut_cat_string(
        char * * dest_ptr,
        char * cat_ptr
        );
int ut_ch_fil_ext(
        char * filename,
        char * ext
        );
int ut_chkname(
        char * name,
        int maxlen,
        int path_allow
        );
int ut_ci_blk_str(
        char * string,
        char sub[]
        );
int ut_ci_sub_loc(
        char s[],
        char subs[]
        );
int ut_cls_edit(
        void
        );

int ut_cmp_dbl(
        double number_1,
        double number_2
        );
int ut_cmp_dblres(
        double number_1,
        double number_2,
        double res
        );
int ut_cmp_double(
        double number_1,
        double number_2
        );
int ut_cmpdbl_tol(
        double number_1,
        double number_2,
        int tol
        );
int ut_comp_str(
        char * s
        );
int ut_comp_type(
        long type,
        char * num1,
        char * num2,
        int * result
        );

void ut_copy_trunc(
        char * dest,
        char * source
        );
void ut_crypt_encode(
        char * src,
        char * encoded
        );
void ut_crypt_decode(
        char * encoded,
        char * decoded
        );
int ut_ctrlc_ast(
        void
        );
int ut_date_time(
        char * cur_date,
        char * cur_time
        );
int ut_decode(
        char * src,
        char * dst
        );
int ut_decode_string(
        char * src,
        char * dst
        );
int ut_deg_to_dms(
        double degrees,
        double * degs,
        double * mins,
        double * secs
        );

void
ut_diagResourceSnapshot(
    double              *realtime_O,
    double              *cputime_O,
    unsigned long       *memory_O );

double
ut_diagCpuTickToSec(
    double               cpuTicks_I );

double ut_delta_angl(
        double angle_1,
        double angle_2
        );
int ut_dms_to_deg(
        double degs,
        double mins,
        double secs,
        double * degrees
        );
int ut_dst2ptline(
        double x_0,
        double y_0,
        double x_1,
        double y_1,
        double x_2,
        double y_2,
        double * x_c,
        double * y_c,
        double * distsqrd
        );
int ut_distptlinz(
        double x_0,
        double y_0,
        double z_0,
        double x_1,
        double y_1,
        double z_1,
        double x_2,
        double y_2,
        double z_2,
        double * x_c,
        double * y_c,
        double * z_c,
        double * distance
        );
int ut_dst2ptlinz(
        double x_0,
        double y_0,
        double z_0,
        double x_1,
        double y_1,
        double z_1,
        double x_2,
        double y_2,
        double z_2,
        double * x_c,
        double * y_c,
        double * z_c,
        double * distsqrd
        );
char * ut_dtoa(
        double n
        );
char * ut_ftoa(     /* takes a double because of the K&R promotion rules */
        double n
        );
int ut_encode(
        char * src,
        char * dst
        );
int ut_encode_string(
        char * src,
        char * dst
        );

long ut_dbl_to_lng(
        double x
        );
double ut_dist(
        double x_1,
        double y_1,
        double z_1,
        double x_2,
        double y_2,
        double z_2
        );
double ut_dround(
        double number,
        double prec
        );
char * ut_dtoa_p(
        double d,
        int p
        );
int ut_edit_file(
        char * file_name
        );
int ut_edit_stat(
        int status
        );
int ut_edit_str(
        char * dir,
        char * prefix,
        char * before,
        char * * after
        );
int ut_entrlookup(
        char * word,
        char * table[]
        );
int ut_equal_type(
        int type,
        char * val1,
        char * val2,
        int * equal
        );
void ut_esc_char(
        char * in_str,
        char * out_str
        );
int ut_extent_int(
        double dmin_x,
        double dmin_y,
        double dmax_x,
        double dmax_y,
        double resolution,
        long * imin_x,
        long * imin_y,
        long * imax_x,
        long * imax_y,
        double * centre_x,
        double * centre_y,
        double * best_resolution
        );
int ut_fexist(
        char * filename
        );
int ut_fgets(
        FILE * file,
        int maxchar,
        char * buffer
        );
int ut_filetomap(
        char * file_name,
        char * map_name
        );
int ut_find_file(
        char * infilename,
        char dirflag,
        char * outfilename,
        int * new_file
        );
int ut_find_macro(
        char * macro_name,
        char path[],
        time_t * mod_time
        );
FILE * ut_fopenp(
        char * name,
        char * access,
        ut_fopen_eol_type   eol_type,
        ut_fopen_close_on_exec_type close_on_exec_type,
        char * string,
        char * path_list,
        char * full_name
        );
int ut_get_2pt_arc(
        double x1,
        double y1,
        double x2,
        double y2,
        double radius,
        int direction,
        double * xc,
        double * yc,
        double * angle_s,
        double * angle_e
        );
int ut_get_arc(
        double x1,
        double y1,
        double x2,
        double y2,
        double x3,
        double y3,
        double * xc,
        double * yc,
        double * radius,
        double * angle_s,
        double * angle_e,
        int * direction
        );
int ut_get_char(
        char * key
        );
int ut_get_mode(
        int * mode
        );
int ut_get_p_name(
        char * process_name
        );
int ut_get_reply(
        int echo,
        char * real_dest,
        int * re_try
        );
int ut_getopt(
        int argc,
        char * * argv,
        char * opts
        );
int ut_getrec(
        FILE * file,
        int recsize,
        int number,
        int nbytes,
        types_addr_t record
        );


char * ut_find_stem(
        char * filespec
        );
int ut_find_subs(
        char s[],
        char subs[]
        );
char * ut_first_char(
        char * string
        );
FILE * ut_fopen(
        char * name,
        char * access,
        ut_fopen_eol_type   eol_type,
        ut_fopen_close_on_exec_type close_on_exec_type,
        char * string
        );
#ifdef MS_WINDOWS
int ut_vfscanf(
        FILE * fp,
        const char * format,
        va_list argptr
        );
int ut_vmscanf(
        MIO_Methods *mio,
        const char * format,
        va_list argptr
        );
#endif /* MS_WINDOWS */

char *ut_get_vision_env(
        char *string
        );
int ut_grp_rec_op(
        int record_length
        );
int ut_grp_rec_rd(
        int group_no,
        int record_no,
        char * record
        );
int ut_grp_rec_tm(
        int group_no
        );
int ut_grp_rec_wr(
        int group_no,
        int record_no,
        char * record
        );
Int32 ut_hashpjw(
        char * s
        );

char * ut_index(
        char * string1,
        char * string2
        );
int ut_int_valdt(
        char * s,
        int len
        );
void ut_ip_calc_pt(
        double ax,
        double bx,
        double cx,
        double dx,
        double ay,
        double by,
        double cy,
        double dy,
        double t,
        double * x,
        double * y
        );
int ut_ip_calcptz(
        double ax,
        double bx,
        double cx,
        double dx,
        double ay,
        double by,
        double cy,
        double dy,
        double az,
        double bz,
        double cz,
        double dz,
        double t,
        double * x,
        double * y,
        double * z
        );
int ut_ip_init(
        double x[],
        double y[],
        double * start,
        double * end,
        double * ax,
        double * bx,
        double * cx,
        double * dx,
        double * ay,
        double * by,
        double * cy,
        double * dy
        );
int ut_ip_init_z(
        double x[],
        double y[],
        double z[],
        double * start,
        double * end,
        double * ax,
        double * bx,
        double * cx,
        double * dx,
        double * ay,
        double * by,
        double * cy,
        double * dy,
        double * az,
        double * bz,
        double * cz,
        double * dz
        );
char * ut_last_char(
        char * string
        );
int ut_linetoline(
        double x11,
        double y11,
        double x12,
        double y12,
        double x21,
        double y21,
        double x22,
        double y22,
        double * xi,
        double * yi,
        double * dist1
        );
int ut_linztolinz(
        double x11,
        double y11,
        double z11,
        double x12,
        double y12,
        double z12,
        double x21,
        double y21,
        double z21,
        double x22,
        double y22,
        double z22,
        double * x1,
        double * y1,
        double * z1,
        double * x2,
        double * y2,
        double * z2,
        double * dist
        );
int ut_list_alloc(
        char * * * string_list,
        int size_incr,
        int elmnt_size,
        int * new_size
        );
int ut_list_free(
        char * * * string_list
        );
int ut_ln2ln_all(
        double x11,
        double y11,
        double x12,
        double y12,
        double x21,
        double y21,
        double x22,
        double y22,
        double * xi1,
        double * yi1,
        double * xi2,
        double * yi2
        );
int ut_ln_to_ln(
        double x11,
        double y11,
        double x12,
        double y12,
        double x21,
        double y21,
        double x22,
        double y22,
        double * xi,
        double * yi
        );

int ut_istrcmp(
        char * a,
        char * b
        );
char * ut_itoa(
        int i,
        char * dst
        );
char * ut_ltoa(
        long l,
        char * dst
        );
char * ut_uitoa(
        unsigned int ui,
        char * dst
        );
char * ut_ultoa(
        unsigned long ul,
        char * dst
        );
int ut_kill(
        int pid
        );

int ut_abr_st_cmp(
        char * str1,
        char * str2
        );
int ut_letter_cmp(
        char * str1,
        char * str2
        );

char * ut_lfind(
        char * key,
        char * base,
        unsigned int * nelp,
        unsigned int width,
        ut_fnCompare compare
        );
int ut_ludecomp(
        int n,
        double a[],
        int eindex[]
        );
int ut_make_desc(
        void
        );
char * ut_match_char(
        char * string
        );
int ut_match_pat(
        char * pattern,
        char * string,
        int case_absolute
        );
int ut_move_type(
        int type,
        char * move_to,
        char * move_from
        );
int ut_nb_of_dec(
        double number
        );
int ut_num_check(
        char * string,
        char type,
        int disp_msg,
        char * number,
        int * valid_num
        );
int ut_num_occur(
        char * string,
        char character,
        int * count
        );
int ut_num_of_dig(
        char * s
        );
int ut_numeric(
        char * string
        );
int ut_opn_edit(
        char * fname
        );
int ut_oracle_str(
        char * string,
        int * valid_string
        );
int ut_orient_lin(
        double x0,
        double y0,
        double x1,
        double y1,
        double x2,
        double y2
        );
int ut_orient_pt(
        double startx,
        double starty,
        double endx,
        double endy,
        double x,
        double y,
        int * orientx,
        int * orienty
        );
int ut_outfil_msg(
        char * msg_buf,
        char severity
        );
int ut_pmpt_parse(
        char * prompt,
        char * buffer,
        long max_parms,
        char * parm_pos[],
        long parm_len[],
        long echo
        );
int ut_prime(
        int forward,
        int ceiling
        );
int ut_prompt(
        char * prompt,
        char * buffer,
        long max_parms,
        char * del_lis,
        char * parm_pos[],
        long parm_len[],
        long echo,
        int * no_parms
        );
int ut_putenv(
        char * name,
        char * value
        );
int ut_putrec(
        FILE * file,
        int recsize,
        int number,
        int nbytes,
        types_addr_t record
        );
int ut_rd_timeout(
        int fd,
        char buf[],
        int len,
        int secs
        );
int ut_readnoecho(
        char * answer,
        int maxlen
        );
int ut_repeat(
        double seconds,
        int times,
        int (* func)(void)
        );
int ut_res_word(
        char * reserved_str,
        int * valid
        );
void ut_revers_cpy(
        char * s1,
        char * s2
        );
void ut_revers_str(
        char s[]
        );
int ut_ring_bell(
        int ntimes,
        int gap
        );
int ut_setup_resp(  
        int argc,
        char * argv[]
        );
int ut_sig_msg(
        int signo
        );


void ut_maptofile(
        char * map_name,
        char * file_name
        );
char * ut_match_quot(
        char * string
        );
double ut_offset(
        double offset,
        double px,
        double py,
        double cx,
        double cy,
        double nx,
        double ny,
        double * x,
        double * y
        );
int ut_parser(
        char * string,
        long max_parms,
        char * parm_pos[],
        long parm_len[]
        );
int ut_pclose(
        FILE * pipe
        );
int ut_pmpt_reply(
        char * prompt,
        char * dest,
        long echo
        );
FILE * ut_popen(
        char * pipename,
        char * access
        );
typedef int (*ut_reduce_in_rtn_t) (long n, double * x, double * y);
typedef int (* ut_reduce_out_rtn_t) (long n, double x, double y);
int ut_reduce(
        ut_reduce_in_rtn_t in_rtn,
        long num_pts,
        double tolerance,
        ut_reduce_out_rtn_t out_rtn
        );
typedef int (* ut_reducez_in_rtn_t) (long n, double * x, double * y, double * z);
typedef int (* ut_reducez_out_rtn_t) (long n, double x, double y, double z);
int ut_reducez(
        ut_reducez_in_rtn_t in_rtn,
        long num_pts,
        double tolerance,
        ut_reducez_out_rtn_t out_rtn
        );
double ut_rint(
        double number
        );
char * ut_set_cap(
        char * cap,
        char * buffer
        );
void ut_sleep(
        unsigned int seconds
        );
int ut_spawn(
        char * file_name,
        char * parameters[]
        );
char * ut_spawner(
        char * requester
        );
void * ut_stk_init(
        Int32 size
        );
int ut_stk_clear(
        void * id
        );
int ut_stk_dstry(
        void * id
        );
int ut_stk_pop(
        void * id,
        char * data
        );
int ut_stk_push(
        void * id,
        char * data,
        Int32 nbytes
        );

int ut_str_parse(
        char * string,
        long max_parms,
        char * del_lis,
        char * parm_pos[],
        long parm_len[],
        int * no_parms
        );
int ut_strIndex(
        char c,
        char * s
        );
size_t ut_strftime(
        char * buf,
        size_t buflen,
        char * locale,
        char * format,
        struct tm * tms
        );
int ut_stripnl(
        char * string
        );
int ut_strlookup(
        char * word,
        char * table[]
        );
int ut_strmat(
        char * string1,
        char * string2
        );
int ut_strp_bl_tb(
        char * string
        );

char * ut_strptime(
        char * buf,
        char * locale,
        char * format,
        struct tm * tms
        );
int ut_strsize(
        char * str,
        int    max_size
        );
int ut_strstrip(
        char * source,
        char * dest
        );
int ut_strtrunc(
        char * dst,
        char * src,
        int n,
        int * n_copied
        );
void ut_strup(
        char * string
        );
int ut_strxcpy(
        char * dest,
        char * source,
        int count
        );
int ut_sub_a_str(
        char * source_str,
        char * from_str,
        char * to_str
        );
int ut_tab_expand(
        int tab,
        char * source,
        char * dest,
        int dest_size
        );
int ut_system(
        char * command
        );
int ut_timecmp(
        struct timeval * time_1,
        char * Operator,
        struct timeval * time_2
        );

char * ut_tmp_dirnam(
        void
        );

int ut_to_lower(
        char * string
        );
int ut_to_upper(
        char * string
        );
int ut_tol_intsec(
        double x11,
        double y11,
        double x12,
        double y12,
        double x21,
        double y21,
        double x22,
        double y22,
        double tol,
        double * xi,
        double * yi,
        double * dist1
        );
int ut_trim_comnt(
        char * string,
        char delimiter
        );
int ut_trn_log(
        char * logical_name,
        char * dest_name
        );
int ut_v_spawn(
        char * file_name,
        char * parameters[],
        int wait_flag,
        long * cstat
        );
int ut_valid_date(
        char * date_string,
        char * locale,
        char * date_format,
        char * new_date_string
        );

types_addr_t ut_vm_calloc(
        char * name,
        unsigned int nelem,
        unsigned int elsize
        );
int ut_vm_free(
        char * name,
        types_addr_t sptr
        );
types_addr_t ut_vm_malloc(
        char * name,
        unsigned int mlen
        );
types_addr_t ut_vm_realloc(
        char * name,
        types_addr_t iptr,
        unsigned int mlen
        );
int ut_wait(
        int * status
        );

int ut_word_cmp(
        char * str1,
        char * str2
        );
int ut_write_edit(
        char * format,
        char * str
        );

typedef int (* ut_x_turns_get_pt_rtn_t)(
        long i,
        double * x,
        double *y
        );
typedef int (* ut_x_turns_out_turn_rtn_t)(
        long pt_no,
        int dirn,
        double min_x,
        double min_y,
        double max_x,
        double max_y
        );
 
int ut_x_turns(
        ut_x_turns_get_pt_rtn_t get_pt_rtn,
        long n,
        ut_x_turns_out_turn_rtn_t out_turn_rtn
        );
int ut_x_turns_z(
        ut_x_turns_get_pt_rtn_t get_pt_rtn,
        long n,
        ut_x_turns_out_turn_rtn_t out_turn_rtn
        );

int     ut_utf8_from_unicode(
        const wchar_t *               Wtext,
        char    *               str_out,
        int                     out_max_size);

int     ut_utf8_charlen(const char *str_in);

int     ut_utf8_to_unicode(
                const char    *       str_in,
                wchar_t *       Wtext,
                int                     out_size);

#if defined(MS_WINDOWS)
int ut_printf( 
        const char *format, 
        /* va_list argptr */
        ...
        );
int ut_puts(
        const char *string
        );
int ut_flush(
        );
int ut_vprintf( 
        const char *format, 
        /* va_list argptr */
        ...
        ); 
int ut_sscanf( 
        const char *buffer,
        const char *format,
        ...
        );
int ut_fscanf( 
        FILE *,
        const char *format,
        ...
        );
#endif

int ut_signal(
        long pid, 
        unsigned long signal_no
        );
        
int ut_string_need_conversion();

int ut_string_to_utf8(
        char    *from_str,
        char    **to_str
        );

int ut_string_from_utf8(
        char    *from_str,
        char    **to_str
        );

#else /* defined(FULLPROTO) || defined(C_PLUSPLUS) */    

typedef int (*ut_fnCompare)();
typedef void (*ut_fnAssign)();
typedef void (*ut_fnSwap)();
typedef int (*ut_fnApply)();


extern long ut_abs_long();  
extern int ut_acute_ang();
extern int ut_ad_fil_ext();
extern int ut_alarm_off(); 
extern int ut_alarm_on();
extern int ut_architecture();

extern char * ut_asctoint();
extern int ut_atodeg();
extern int ut_atodegrees();
extern char * ut_atod();
extern char * ut_atoi();
extern char * ut_atol();
extern double ut_azi_2_bear();
extern int ut_bit_get();
extern void ut_bit_clr();
extern void ut_bit_set();
extern char * ut_bsearch();
extern void ut_base_ext();
extern int ut_blk_a_str();
extern int ut_buf_offset();

extern int ut_cat_string();
extern int ut_ch_fil_ext();
extern int ut_chkname();
extern int ut_ci_blk_str();
extern int ut_ci_sub_loc();
extern int ut_cls_edit();

extern int ut_cmp_dbl();
extern int ut_cmp_dblres();
extern int ut_cmp_double();
extern int ut_cmpdbl_tol();
extern int ut_comp_str();
extern int ut_comp_type();

extern void ut_copy_trunc();
extern void ut_crypt_encode();
extern void ut_crypt_decode();
extern int ut_ctrlc_ast();
extern int ut_date_time();
extern int ut_decode();
extern double ut_delta_angl();
extern int ut_deg_to_dms();
extern int ut_dms_to_deg();
extern int ut_distptlinz();
extern int ut_dst2ptline();
extern int ut_dst2ptlinz();


extern char * ut_dtoa();
extern char * ut_ftoa();/* takes a double because of the K&R promotion rules */

extern int ut_encode();

extern long ut_dbl_to_lng();
extern double ut_dist();
extern double ut_dround();
extern char * ut_dtoa_p();
extern int ut_edit_file();
extern int ut_edit_stat();
extern int ut_edit_str();
extern int ut_entrlookup(); 
extern int ut_equal_type(); 
extern void ut_esc_char();
extern int ut_extent_int();
extern int ut_fexist();
extern int ut_fgets();
extern int ut_filetomap();
extern int ut_find_file();
extern int ut_find_macro();
extern FILE * ut_fopenp();
extern int ut_get_2pt_arc();
extern int ut_get_arc();
extern int ut_get_char();
extern int ut_get_mode();
extern int ut_get_p_name();
extern int ut_get_reply();
extern int ut_getopt();
extern int ut_getrec();

extern char * ut_find_stem();
extern int ut_find_subs();
extern char * ut_first_char();
extern FILE * ut_fopen();
extern int ut_vfscanf();
extern int ut_vmscanf();
extern char *ut_get_vision_env();
extern int ut_grp_rec_op(); 
extern int ut_grp_rec_rd();
extern int ut_grp_rec_tm();
extern int ut_grp_rec_wr();
extern Int32 ut_hashpjw();

extern char * ut_index();

extern int ut_int_valdt();
extern int ut_ip_calc_pt();
extern int ut_ip_calcptz();
extern int ut_ip_init();
extern int ut_ip_init_z();
extern char * ut_last_char();
extern int ut_linetoline();
extern int ut_linztolinz();
extern int ut_list_alloc();
extern int ut_list_free();
extern int ut_ln2ln_all();
extern int ut_ln_to_ln();

extern int ut_istrcmp();
extern char * ut_itoa();
extern char * ut_ltoa();
extern char * ut_uitoa();
extern char * ut_ultoa();
extern int ut_kill();
extern int ut_abr_st_cmp();
extern int ut_letter_cmp();
extern char * ut_lfind();
extern int ut_ludecomp();

extern int ut_make_desc();
extern char* ut_match_char();
extern int ut_match_pat();
extern int ut_move_type();
extern int ut_nb_of_dec();
extern int ut_num_check();
extern int ut_num_occur();
extern int ut_num_of_dig();
extern int ut_numeric();
extern int ut_opn_edit();
extern int ut_oracle_str();
extern int ut_orient_lin();
extern int ut_orient_pt();
extern int ut_outfil_msg();
extern int ut_pmpt_parse();
extern int ut_prime();
extern int ut_prompt();
extern int ut_putenv();
extern int ut_putrec();
extern int ut_rd_timeout();
extern int ut_readnoecho();
extern int ut_repeat();
extern int ut_res_word();
extern int ut_revers_cpy();
extern int ut_revers_str();
extern int ut_ring_bell();
extern int ut_setup_resp();
extern int ut_sig_msg();

extern void ut_maptofile();
extern char * ut_match_quot();
extern double ut_offset();
extern int ut_parser();
extern int ut_pclose();
extern int ut_pmpt_reply();
extern FILE * ut_popen();

typedef int (* ut_reduce_in_rtn_t) ();
typedef int (* ut_reduce_out_rtn_t) ();
extern int ut_reduce();

typedef int (* ut_reducez_in_rtn_t) ();
typedef int (* ut_reducez_out_rtn_t) ();
extern int ut_reducez();

extern double ut_rint();
extern char * ut_set_cap();
extern void ut_sleep();
extern int ut_spawn();
extern char * ut_spawner();
extern void * ut_stk_init();
extern int ut_stk_clear();
extern int ut_stk_dstry();
extern int ut_stk_pop();
extern int ut_stk_push();
extern int ut_str_parse();
extern char * ut_strptime();
extern int ut_strstrip();
extern int ut_strtrunc();
extern int ut_strup();
extern int ut_strxcpy();
extern int ut_system();
extern int ut_timecmp();
extern char * ut_tmp_dirnam();
extern int ut_to_lower();
extern int ut_to_upper();

types_addr_t ut_vm_calloc();
extern int ut_vm_free();
extern types_addr_t ut_vm_malloc();
extern types_addr_t ut_vm_realloc();

extern int ut_wait();
typedef int (* ut_x_turns_get_pt_rtn_t)();
typedef int (* ut_x_turns_out_turn_rtn_t)();
extern int ut_x_turns();
extern int ut_x_turns_z();
extern int ut_sub_a_str();
extern int ut_strsize();

#endif /* defined(FULLPROTO) || defined(C_PLUSPLUS) */

#ifdef C_PLUSPLUS
}
#endif

#endif

