// patch from http://groups.google.ca/groups?hl=en&lr=&ie=UTF-8&oe=UTF-8&threadm=1Qvpb.3763%24mB5.288520%40news20.bellglobal.com&rnum=1&prev=/groups%3Fq%3D_ctype_b%26hl%3Den%26lr%3D%26ie%3DUTF-8%26oe%3DUTF-8%26selm%3D1Qvpb.3763%2524mB5.288520%2540news20.bellglobal.com%26rnum%3D1
// to cure undefined reference to _ctype_b, _ctype_toupper, _ctype_tolower

#include <ctype.h>
                                                                               
                                                                               
__const unsigned short int *__ctype_b;
__const __int32_t *__ctype_tolower;
__const __int32_t *__ctype_toupper;
                                                                               
                                                                               
void ctSetup()
{
__ctype_b = *(__ctype_b_loc());
__ctype_toupper = *(__ctype_toupper_loc());
__ctype_tolower = *(__ctype_tolower_loc());
}

/*
#include <pthread.h>
int  pthread_atfork(void  (*prepare)(void),  void (*parent)(void), void (*child)(void))
{
   return (1); // fail

}
*/
