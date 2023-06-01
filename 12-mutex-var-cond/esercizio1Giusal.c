#include "../apue/apue.h"
#include <pthread.h>
int coord[2]={-1,-1};
int key,righe,colonne;
pthread_mutex_t coord_mutex;
int **matrice;
pthread_t *threads;
void crea_matrice();
void stampa_matrice();

void *ricerca(void *tid)
{
    int k,j;
pthread_t *mytid;
mytid = (pthread_t *) tid;
printf ("Thread %d doing search %d \n",*mytid, *mytid);
    
   for (j=0; j < colonne; j++) {
if (matrice[*mytid][j] == key){
pthread_mutex_lock(&coord_mutex);
coord[0]=*mytid;
coord[1]=j;
for (k=0;k<righe;k++){
if(k!=*mytid){
printf("%d %d\n",k,*mytid);
pthread_cancel(threads[k]);
}
}
pthread_mutex_unlock(&coord_mutex);
pthread_exit(NULL);
}
pthread_exit(NULL);
}
}

int main(int argc, char**argv){
pthread_t *tids, i;
righe = atoi(argv[1]);
colonne = atoi(argv[2]);
crea_matrice(righe,colonne);
stampa_matrice();
printf("Elemento da ricercare: ");
scanf("%d",&key);
threads = calloc(righe,sizeof(pthread_t));
tids = calloc(righe, sizeof(pthread_t));
for (i=0; i<righe; i++) {
tids[i] = i;
pthread_create(&threads[i], NULL, ricerca, (void *) &tids[i]);
}
for (i=0; i<righe; i++) {
pthread_join(threads[i], NULL);
}
printf ("Fatto. Coordinate = %d %d\n", coord[0],coord[1]);
exit(0);
}

void crea_matrice(){
int i,j;
matrice = calloc(righe,sizeof(int *));
for(i=0;i<righe;i++)
matrice[i] = calloc(righe,sizeof(int));
for(i=0;i<righe;i++)
for(j=0;j<colonne;j++)
matrice[i][j] = rand() % 20;
}

void stampa_matrice(){
    int i,j;
    for(i=0;i<righe;i++){
        for(j=0;j<colonne;j++)
            printf("%d\t",matrice[i][j]);
        printf("\n");
    }
}