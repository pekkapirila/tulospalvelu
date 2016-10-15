void tagetpage(indexfile *idxf, int r, tapageptr *pgptr);
void tanewpage(indexfile *idxf, int *r, tapageptr *pgptr);
void tareturnpage(indexfile *idxf, int pageref);
int tacompkeys(char *k1, char far *kf2, int dr1, int dr2, int dup,
   int klen);
void copypgtoitem(taitem *item, tapageptr pgptr, int i, int keyl);
void copyitemtopg(tapageptr pgptr, taitem *item, int i, int keyl);
void copypgtopg(tapageptr pgptr2, tapageptr pgptr1, int i2, int i1,
   int keyl);

