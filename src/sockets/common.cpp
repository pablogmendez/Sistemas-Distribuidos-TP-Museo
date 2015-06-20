/*
Autor: Marelli Ricardo Guido
Padron: 79134

Descripcion: Implementa las funciones definidas en common.h
*/

#include "common.h"
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int printmsg(const char* format,...)
{
   char s[__PRINT_MSG_LEN__];
   int size=0;
   va_list ap;
   va_start(ap,format);
   sprintf(s,"[%d] ",getpid());
   size=strlen(s);
   if(vsnprintf(&s[size],__PRINT_MSG_LEN__-size,format,ap)<0) return -1;
   va_end(ap);
   size=strlen(s);
   if(size < __PRINT_MSG_LEN__-1) {
       s[size]='\n';
       s[size+1]='\0';
   }
   return write(fileno(stdout),s,strlen(s));
}

