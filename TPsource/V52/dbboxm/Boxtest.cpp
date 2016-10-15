#include <dos.h>
#include <stdio.h>
#include <errno.h>
#include <conio.h>
#include <string.h>
#include <bvideo.h>
#include <bscreens.h>
#include <bstrings.h>
#include <bkeybrd.h>
#include <butil.h>
#include <tputil.h>
#include <boxm.h>
#define  CRIT_VEC 0x24
#define NUMERSTKS     2
#define ERSTKSIZE  3000
#define FALSE 0
#define TRUE 1

   int listfl, spoolfl, ok, tmaali;
   UCHAR *savebox, *errbox;
   int maaliajat[20], minkilpno = 1,maxkilpno = 9999;

int initcon(wchar_t *title);

int ioerr(UCHAR *fname, int oper, long iores)
{
	return(0);
}

void lopetus(void)
{
   exit(0);
}

/*
void select(UCHAR *prompt, UCHAR *term, UCHAR *tc)
{
   UCHAR ch,ch2;
   int l;

   l = strlen(prompt);
   scclrmsg(22,0,80);
   vidspmsg(22,0,-1,-1,prompt);
   l = min(l,77);
   sccurset(22,l+2);
   do {
      do {
         ch = readkbd(&ch2,FALSE,spoolfl);
      } while (ch2 == 0);
      if (ch == 0) *tc = ch2 + 142;
              else *tc = toupper(ch);
   } while (stschind(*tc, term) == -1);
   listfl = FALSE;
}
*/
void remtimer(void)
{

}

void main()
{
   static UCHAR fname[] = "test.dat";
   static UCHAR indname[] = "test.idx";
   int i;
	int r;
   datafile datf;
   indexfile indf;
   extern int tmaali;
   extern int ok;
   UCHAR rec[53];
   UCHAR *recptr;
   UCHAR key[13];
   UCHAR *keyptr;

   initindex();
   initcon(L"DBboxTest");
   savebox = malloc(400);
   errbox = malloc(400);
   memset(errbox, ' ', 400);
   makefile(&datf, fname, 50);
   makeindex(&indf, indname, 10, 0);
   for (i=1;i<=40;i++){
      puts("tietue :");
	  memset(rec, 0, sizeof(rec));
	  rec[0] = 50;
      recptr=cgets(rec);
      puts(recptr);
	  memset(key, 0, sizeof(key));
      strncpy(key,recptr,10);
      key[10]=0;
      puts(key);
		addrec(&datf, &r, recptr);
      getrec(&datf, r, recptr);
      puts(recptr);
      addkey(&indf, r, key);
      }
   scpclr();
   sccurset(0,0);
   clearkey(&indf);
   for (i=1;i<=42;i++){
      if (i % 20 == 0) getch();
      nextkey(&indf, &r, key);
      getrec(&datf, r, recptr);
      printf("%12s %03d %50s %d\n",key,r,recptr,ok);
      }
   closefile(&datf);
   openfile(&datf, fname, 50);
   puts("Anna avain");
   for (i=1;i<=3;i++){
	  memset(key, 0, sizeof(key));
	  key[0] = 10;
      keyptr=cgets(key);
	  memset(keyptr+strlen(keyptr), 0, 10-strlen(keyptr));
      findkey(&indf, &r, keyptr);
      if(ok) {
         getrec(&datf, r, recptr);
         printf("%12s %03d %50s\n",keyptr,r,recptr);
         deletekey(&indf, &r, keyptr);
         deleterec(&datf, r);
       }
      }
   scpclr();
   sccurset(0,0);
   clearkey(&indf);
   for (i=1;i<=42;i++){
      if (i % 20 == 0) getch();
      nextkey(&indf, &r, key);
      getrec(&datf, r, recptr);
      printf("%12s %03d %50s %d\n",key,r,recptr,ok);
      }
   tmaali = 0xabcd;
   closefile(&datf);
   openfile(&datf, fname, 50);
   puts("Anna avain");
   for (i=1;i<=3;i++){
	  memset(key, 0, sizeof(key));
	  key[0] = 10;
      keyptr=cgets(key);
	  memset(keyptr+strlen(keyptr), 0, 10-strlen(keyptr));
      searchkey(&indf, &r, keyptr);
      if(ok) {
         getrec(&datf, r, recptr);
         printf("%12s %03d %50s\n",keyptr,r,recptr);
         deletekey(&indf, &r, keyptr);
         deleterec(&datf, r);
       }
      }
   scpclr();
   sccurset(0,0);
   clearkey(&indf);
   for (i=1;i<=42;i++){
      if (i % 20 == 0) getch();
      nextkey(&indf, &r, key);
      getrec(&datf, r, recptr);
      printf("%12s %03d %50s %d\n",key,r,recptr,ok);
      }
   tmaali = 0xabcd;
   closeindex(&indf);
   closefile(&datf);
}


