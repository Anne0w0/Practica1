#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char *archivos[10]={"archivo1.txt","archivo2.txt","archivo3.txt","archivo4.txt","archivo5.txt","archivo6.txt","archivo7.txt","archivo8.txt","archivo9.txt","archivo10.txt"};//archivos qque vamos a registrar
char *palabra[10] = {"río ", "no ", "ya ", "soy ", "cielo ", "fuego ", "soledad ", "tiempo ", "olvido ", "ni "}; 
void* funcion(void* param);


typedef struct {
    char caracter[30];
    double porcA[10];//porcentaje por archivo
    int contIT;// contador insidencias totales
    int contIA[10];//incidencias por archivo
} hilo_p;

int main(int argc, char const *argv[]){

    /**** Se corroboran el numero de argumentos recibidos 
    if( argc < 2){
        printf("Favor de ingresar las palabras a buscar\n");
        exit(1);
    }*/

    /**** Paso 1: Creacion de los hilos ****/

    //mem_global = "archivo.txt";
    int n = 5 ; // n sera el numero de hilos (empezando por 0)
    void *variable; // variable para recibir datos del hilo
    pthread_t *id = (pthread_t *)malloc(sizeof(pthread_t)*n); // id's de los hilos;
    hilo_p * hilo_D = (hilo_p*) malloc(sizeof(hilo_p)*n); // estructuras que se le pasara a cada uno de los hijos


    for(int i = 0; i != n ; i++){
        strcpy(hilo_D[i].caracter, palabra[i]);
            hilo_D[i].contIT = i;
            //hilo_D[i].indice = i+1;
            if (pthread_create(&id[i], NULL, (void*) funcion, &hilo_D[i]) == -1){ // Esta funcion nos regresa 1 si se creo y -1 en caso contrario
                printf("Error al crear el hilo\n");
                exit(1);
            }
        
    }

    /**** Paso 2: Hacer que el padre espere a los hijos ****/
    float p=0;
    for (int j = 0; j != n; j++){
        pthread_join(id[j], &variable);
        printf("La palabrar \"%s\" se encontro %d veces en los 10 archivos\n",((hilo_p *) variable)->caracter, ((hilo_p *) variable)->contIT);
        for(int k=0;k!=10;k++){
            printf("Veces que aparece \"%s\" en el archivo %d = %d\n",((hilo_p *) variable)->caracter,k+1,((hilo_p*)variable)->contIA[k]);
            printf("porcentaje de aparicion\"%s\" en el archivo %d = %f\n",((hilo_p *) variable)->caracter,k+1,((hilo_p*)variable)->porcA[k]);
        }
        printf("\n");
    }
    
    free(id);
    free(hilo_D);

    return 0;
}

void* funcion(void* param){

    int contIT=0;//contador de insidencias totales(10 archivos)
    int contIA=0;//contador de insidencias por archivo

    /**** Paso 3: Crear la funcion que se convertira en el hilo ****/
    hilo_p *h = (hilo_p *) param;

    for(int k=0; k != 10; k++){ 
        /* APERTURA DEL ARCHIVO */
        FILE *archivo_hijo = fopen(archivos[k], "r");
        if(archivo_hijo == NULL){
            printf("Error al abrir el archivo");
            exit(1);
        }
         contIA = 0;
        /* Algoritmo para contar numero de coicidencias de la palabra indicada */
        char texto[1000];
        int tmp1, tmp2, cont = 0;

        while (feof(archivo_hijo) == 0){
            fgets(texto, 1000, archivo_hijo);
            for (int i = 0; i < strlen(texto); i++) {
                if(h->caracter[0] == texto[i] ){
                    tmp1 = 0;
                    tmp2 = i;
                    while ((h->caracter[tmp1] == texto[tmp2]) && (tmp2 < strlen(texto)) && (tmp1 != strlen(h->caracter))){
                        tmp1++;
                        tmp2++;
                        if(tmp1 == strlen(h->caracter))
                            cont++;
                    }
                }
            }
            contIA =  cont;
        }
        fclose(archivo_hijo);
        contIT=contIT+cont;
        h->contIA[k]=contIA;
    }

    h->contIT = contIT; //VALOR FINAL

    for(int i=0;i<10;i++)
        h->porcA[i]= ((h->contIA[i]*100)/contIT);
    
    pthread_exit((void*) &h[0]);
}



