#include <stdlib.h>
#include <string.h>

#include <sys/types.h>  //OPEN READ
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/types.h> // STAT 
#include <sys/stat.h> 
#include <unistd.h>

#include <stdio.h> //POPEN

#include <float.h>

#include <stdarg.h>//for ARRAY_SZ


#define BUFSIZE 256
#define MAX_WIDTH 16

#define ARR_SIZE(X)  sizeof(X)/sizeof(X[0])

#define getName(var)  #var


int isIn( int *arr, int s){
  int *temp = arr;
  while (temp++){
    if (temp[0] == s)
      return 1;
  }
  return 0;
}

int isfloat (const char *s)
{
  char *ep = NULL;
  double f = strtod (s, &ep);

  if (!ep  ||  *ep)
    return 0;  // has non-floating digits after number, if any

  return 1;
}

int strsize( char *src){
  int i,res = 0;
  for (i = 0; src[i] != '\0'; i++){
    res++;
  }
  return (res);
}

void disp( int *src, int size){
  int i = 0, n = size;
  for (i = 0; i < n ; i++){
    printf("%u\t",src[i]);
  }
  putchar('\n');
}

void disp2( char **src, int size){
  int i = 0, n = size;
  for (i = 0; i < n ; i++){
    printf("%s\n",src[i]);
  }
}

int strfreq( char *src, char s){
  int nb = 0, i = 0;
  while (src[i++] != '\0'){
    if (src[i]==s) nb++;
  }
  return nb;
}

char *rmchar(char *str, char s){
  char *strStripped = malloc(sizeof(str)-1);
  int i = 0, c = 0;
  for(; i < strlen(str); i++){
      if (str[i] != '"'){
	  strStripped[c] = str[i];
	  c++;
	}
    }
  strStripped[c] = '\0';
  return strStripped;
  
}





      








typedef struct datum{
  double *dble_props;
  char **str_props;
} datum;

datum *init( int nbdble, int nbstr){
  datum *dat;
  dat->dble_props = malloc( nbdble*sizeof(double) );
  dat->str_props = malloc( nbstr*BUFSIZE );
  return dat;
}

void freeing( datum *dat, int nbdble, int nbstr){
  int i;
  free(dat->dble_props);
  for (i=0;i<nbstr;i++){
    free(dat->str_props[i]);
  }
  free(dat->str_props);
}

datum *datcpy( datum *src, int nbdble, int nbstr){
  int i = 0;
  datum *dest;
  
  dest = init(nbdble,nbstr);
  dest->dble_props = src->dble_props;
  
  for (i;i<nbstr;i++){
    dest->str_props[i] = src->str_props[i];
  }
}



int disp3( datum *dat, char **dble_names, char **str_names, int nbdble, int nbstr){
  int i;
  for (i = 0; i<nbstr; i++){
    printf("%s : %s", str_names[i], dat->str_props[i] );
    putchar('\n');
  }  
  for (i = 0; i<nbdble; i++){
    printf("%s : %*f", dble_names[i], MAX_WIDTH, dat->dble_props[i] );
    putchar('\n');
  }
  putchar('\n');
  return 0;
}


int *getinfo( datum *dat, int prtf){
  int dble_size = 0, str_size = 0, bitsize = 0;
  while ( dat->dble_props[dble_size]  ){
    //printf("%f\n",dat->dble_props[dble_size++]);
    dble_size++;
  }
  while ( dat->str_props[str_size] != NULL ){
    //printf("%s\n",dat->str_props[str_size++]);
    str_size++;
    }
  bitsize = dble_size*sizeof(  typeof(dat->dble_props[0])   ) \
    + str_size*sizeof(   typeof(dat->str_props[0])   );
  if (prtf){
    printf("name :%s\n", getName(dat) );
    printf("%u Categories:\n", dble_size+str_size);
    printf("\t %u numeric\n", dble_size);
    printf("\t %u names\n", str_size);
    printf("bitsize: %u\n", bitsize);
  }
  int *size = malloc(2*sizeof(int));
  size[0] = dble_size;
  size[1] = str_size;
  return size;
}

void dollar( datum *dat, char *cat, char **dble_names, char **str_names){
  int i, *size = getinfo(dat,0);
  for (i=0;i<size[0];i++){    
    if ( strcmp(dble_names[i],cat) ==0){
      printf("%s.%s: %f", getName(dat), cat, dat->dble_props[i]);
    }
  }
  for (i=0;i<size[1];i++){
    if ( strcmp(str_names[i],cat) ==0){
      printf("%s.%s: %s", getName(dat), cat, dat->str_props[i]);
    }
  }
  putchar('\n');
}






int parser(char *filename){
  int fd, nbline, i, iter, m, n, iterbis, obs;
  struct stat f_st;
  FILE *com;
  char buffer[BUFSIZE], buffer2[BUFSIZE], \
    *temp,					\
    *buffertemp = malloc(BUFSIZE*sizeof(char)), \
    *buffer2temp = malloc(BUFSIZE*sizeof(char)), \
    *token = malloc(BUFSIZE/2*sizeof(char));
  char dest[BUFSIZE];
  char delim = '\t';
  double dtemp;
  int *corresptemp;
  char c;
  int ci;


  //PARAMETERS of FILE
  int nbcateg = 0;
  int nbstr = 0;
  int nbdble = 0;
  char **dble_names;
  char **str_names;
  int *corresp = (int *)malloc(sizeof(int));
  int *corresp2 = (int *)malloc(sizeof(int));

  /*
    CODE:
    XX : Needed before fueling, \
        for instance to get data structure\
	typically, when BUFFER changes

    OO : fueling part    
  */

  
  //STAT
  /*
  if ( stat(filename, &f_st) == -1 ){
    exit(EXIT_FAILURE);
    }*/

  //////////////////////////////////////////////////////////////////////////
  //GET NBLINE                                                            XX
  strcpy( dest , "wc -l ");
  strcat( dest, filename);
  com = popen( dest, "r");
  if (!com) exit(EXIT_FAILURE);
  fgets( buffer, sizeof(buffer), com);
  pclose(com);
  nbline = atoi(strtok(buffer, " ")) - 1;
  printf("Number of observations : %u\n", nbline);
  //////////////////////////////////////////////////////////////////////////

  
  //OPEN
  /*
  fd = open(filename, O_RDONLY, \
           S_IRUSR | S_IRGRP | S_IROTH);
  if (fd<0){
    exit(EXIT_FAILURE);
    }*/

  //////////////////////////////////////////////////////////////////////////  
  //GET NBCATEG, NBDBLE & CORRESP TAB                                     XX
  com = fopen( filename, "r");
  if (  fgets(buffer, sizeof(buffer), com) == NULL  )
    exit(EXIT_FAILURE);
  
  nbcateg = strfreq(buffer,delim) + 1;
  printf("Number of Categories: %u \n", nbcateg);
  fgets(buffer2, sizeof(buffer2), com);    
  strcpy( buffertemp, buffer);
  strcpy( buffer2temp, buffer2);
  iter = -1;
  while ( (token=strsep(&buffer2temp, "\t") ) != NULL ){
    iter++;
    if ( isfloat(token) ){
      ++nbdble;
      corresp = realloc(corresp,nbdble*sizeof(int) );
      corresp[nbdble-1] = iter;
    }
    else{
      ++nbstr;
      corresp2 = realloc(corresp2,nbstr*sizeof(int) );
      corresp2[nbstr-1] = iter;
    }
  }
  corresptemp = malloc(nbdble*sizeof(int));
  memcpy(corresptemp, corresp, nbdble*sizeof(int) );
  //////////////////////////////////////////////////////////////////////////
  
  /*
    disp( corresp, nbdble);
    disp( corresptemp, nbdble);
  */
  
  if (nbstr != (nbcateg-nbdble) ) {
    printf("error in corresp process\n");
    exit(EXIT_FAILURE);
  }

  //////////////////////////////////////////////////////////////////////////
  // GET NBSTR, DBLE_NAMES, STR_NAMES                                     XX
  //
  printf("Number of Double Categories : %u\n",nbdble);
  printf("Number of String Categories : %u\n",nbstr);
  dble_names = malloc(nbdble*BUFSIZE );
  str_names = malloc(nbstr*BUFSIZE );
  iter = 0;
  m = 0; n = 0;
  while ( (token=strsep(&buffertemp, "\t") ) != NULL ){

    token = strsep(&token, "\n");
    token = rmchar(token,  34);
      
    if ( iter == corresp[0]){
      dble_names[m] = token;
      corresp = corresp + 1;
      m++;
    }
    else{
      str_names[n] = token;
      n++;
    }
    iter++;
  }
  //////////////////////////////////////////////////////////////////////////

  //            PRINT  VERIF
  //disp2(dble_names, m);
  //disp2(str_names, n);
  //disp( corresptemp, nbdble);

  //PROblem
  //memcpy(corresp, corresptemp, nbdble*sizeof(int) );
  //   free(corresptemp);
  //           OK
    

  //////////////////////////////////////////////////////////////////////////
  // DEALING WITH BUFFERTEMPS                                            OO
  // GE FILLER                                                           OO
  buffertemp = malloc(BUFSIZE*sizeof(char));
  buffer2temp = malloc(BUFSIZE*sizeof(char));
  strcpy( buffertemp, buffer );
  strcpy( buffer2temp, buffer2 );

  char **filler = malloc(nbcateg*sizeof(char) );
  for (i = 0; i < nbcateg; i++){
    filler[i] = malloc(BUFSIZE);
    filler[i] = strsep(&buffer2temp, "\t");
  }
  //////////////////////////////////////////////////////////////////////////
  
  //         PRINTF VERIF
  /*
    printf("%s\n",buffer);
    printf("%s\n",buffer2);
    printf("filler:\n");
    disp2( filler, nbcateg );
    printf("corresptemp:\n");
    disp(corresptemp,nbdble);
    printf("corresp2:\n");
    disp(corresp2,nbstr);
  */
  //       OK

  //////////////////////////////////////////////////////////////////////////
  // GET LOCAL IPSUM /  FUELING                                           OO
  printf("\n\n");
  datum *ipsum = init(nbdble,nbstr);

  // fueling with double data
  iter = 0;
  for ( i = 0 ; i<nbdble ; i++){
    ipsum->dble_props[iter++] = strtof( filler[corresptemp[i]] , NULL );
  }
  //fueling with string data
  //disp2( filler, nbcateg );
  iterbis = 0;
  for ( i = 0 ; i<nbstr; i++){
    ipsum->str_props[iterbis++] = rmchar( strsep(&filler[corresp2[i]], "\n") , 34 );
  }
  //////////////////////////////////////////////////////////////////////////




  //////////////////////////////////////////////////////////////////////////
  // CREATE & INIT WHOLE DATA SET                                      00-XX
  obs = 0;
  datum **data = malloc( nbline*sizeof(datum) );
  data[obs] = ipsum;
  disp3(data[obs],dble_names,str_names,nbdble,nbstr);
  //////////////////////////////////////////////////////////////////////////

    
  //printf("%s\n",buffer2);
  //disp3(ipsum,dble_names,str_names,nbdble,nbstr);
  //putchar('\n');
  //putchar('\n');
  //getinfo(ipsum);
  //putchar('\n');
  //getinfo(data[obs]);

  //disp3(data[obs],dble_names,str_names,nbdble,nbstr);
  freeing(ipsum, nbdble, nbstr);

  //dollar( ipsum, "Javeline", dble_names, str_names);

  
  //////////////////////////////////////////////////////////////////////////
  //READ AND FILL                                                         OO
  while (fgets(buffer, sizeof(buffer), com) != NULL){

    //COPY
    buffer2temp = realloc(buffer2temp,BUFSIZE);
    strcpy(buffer2temp, buffer);

    //FILLER
    //filler = malloc(nbcateg*BUFSIZE);
    /*
    for (i = 0; i < nbcateg; i++){
      free(filler[i]);
    }
    free(filler);
    filler = malloc(nbcateg*sizeof(char) );
    */
    for (i = 0; i < nbcateg; i++){
      //filler[i] = malloc(BUFSIZE);
      filler[i] = strsep(&buffer2temp, "\t");
    }

    
    //LOCAL IPSUM
    iter=0;    
    for ( i = 0 ; i<nbdble ; i++){
      ipsum->dble_props[iter] = strtof( filler[corresptemp[i]] , NULL );
      //printf("%f\n", ipsum->dble_props[iter]);
      iter+=1;
    }
    
    iterbis = 0;
   
    //printf("\n\nfiller:\n");
    //disp2( filler, nbcateg );
      
    for ( i = 0 ; i<nbstr; i++){
      //printf("debutloop\n");
      //memcpy( ipsum->str_props[iterbis], rmchar( strsep(&filler[corresp2[i]], "\n") , 34 ), BUFSIZE );
      ipsum->str_props[iterbis] = rmchar( strsep(&filler[corresp2[i]], "\n") , 34 );
      //strncpy(     ipsum->str_props[iterbis] , rmchar( strsep(&filler[corresp2[i]], "\n") , 34 )   ,    BUFSIZE);
      //printf("then\n");
      //printf("import(%lu) : %s\n", sizeof( strsep(&filler[corresp2[i]], "\n") ),  strsep(&filler[corresp2[i]], "\n") );
      //printf("ipsum(%lu) : %s\n\n", sizeof(ipsum->str_props[iterbis])  , ipsum->str_props[iterbis]);
      iterbis+=1;
      //printf("endlopp\n\n");
    }
    //disp3(ipsum,dble_names,str_names,nbdble,nbstr);

    

    
    //FILL DATA
    obs+=1;
    
    //data = realloc( data, (obs+1)*sizeof(ipsum) );
    data[obs] = ipsum;
    /*
    //FREEING IPSUM
    freeing(ipsum, nbdble, nbstr);
    */

    disp3(data[obs],dble_names,str_names,nbdble,nbstr);
    
    continue;
  }





  

  
  fclose(com);
  
  exit(EXIT_SUCCESS);
}


int main(int argc, char **argv){
  char *filename = "decathlon.txt";
  return parser(filename);
}
