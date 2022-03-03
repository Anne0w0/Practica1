#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

    //memoria global
    int **matrizA,**matrizB,**Respuesta,**matrizIndices,RowA,ColumnB;

int **crearMatriz(int filas,int columnas);
void llenarMatriz(int **matriz,int filas,int columnas);
void calcularIndices(int **matriz,int numHilos);
void *MultiMatrices(void* param);

void main(int argC,int argV[]){

    int RowB,ColumnA,numHilos;
    srand(time(NULL)); //generar valores aleatorios

//-----------Primera matriz------------------
    printf("\n-------Datos de la primer matriz-------");
    printf(" \nIngrese el numero de filas:");
    scanf("%d",&RowA);
    printf(" \nIngrese el numero de columnas:");
    scanf("%d",&ColumnA);
    matrizA=crearMatriz(RowA,ColumnA);
    llenarMatriz(matrizA,RowA,ColumnA);

//-----------Segunda matriz------------------
    printf("\n-------Datos de la segunda matriz-------");
     printf(" \nIngrese el numero de filas:");
    scanf("%d",&RowB); 
    //-----Comprobacion para multiplicar matrces------
    if(ColumnA != RowB){
        printf("\nEl numero de columnas de la primera matriz debe ser igual al numero de filas de la segunda\n");
        exit(0);
    }
    printf(" \nIngrese el numero de columnas:");
    scanf("%d",&ColumnB);
    matrizB=crearMatriz(RowB,ColumnB);
    llenarMatriz(matrizB,RowB,ColumnB);

//----------Proceso para elegir el indice de cada fila------------------
    printf("\nIngrese el numero de procesos: ");
    scanf("%d",&numHilos);
    if(numHilos > RowA){
        printf("\n NO puede haber mas procesos que filas\n");
        exit(0);
    }

    matrizIndices=crearMatriz(numHilos,2);
    calcularIndices(matrizIndices,numHilos);

    Respuesta=crearMatriz(RowA,ColumnB);   

//----------Proceso para crear los hilos------------------

    pthread_t *hilo= (pthread_t*) malloc(numHilos*sizeof(pthread_t)); 
    int *dataIndex=(int*) malloc(numHilos*sizeof(int)); 

    for (int i = 0; i < numHilos; i++){
        dataIndex[i]=i;
        if (pthread_create(&hilo[i], NULL, MultiMatrices, &dataIndex[i]) == -1){
            printf("Error al crear el hilo\n");
            exit(0);
        }
    }

    for (int i = 0; i< numHilos; i++){
        pthread_join(hilo[i],NULL);    
    }
    

//----------Imprimimos valores------------------
    for(int i=0;i<RowA;i++){
        for(int j=0;j<ColumnB;j++){
            printf("%d\t",Respuesta[i][j]);
        }
        printf("\n");
    }

    free(hilo);
    free(dataIndex);
    free(matrizA);
    free(matrizB);
    free(matrizIndices);
    free(Respuesta);

}

int **crearMatriz(int filas,int columnas){
    int **aux;
    aux=(int**)malloc(filas*sizeof(int*));
    if(aux==NULL){
        printf("No se ha podido reservar memoria\n");
        exit(1);
    }    

    for(int i=0;i<filas;i++){
        aux[i]=(int*)malloc(columnas*sizeof(int));
        if(aux==NULL){
        printf("No se ha podido reservar memoria\n");
        exit(1);
        } 
    }
    return aux;
}

void llenarMatriz(int **matriz,int filas,int columnas){
    for(int i=0;i<filas;i++){
        for(int j=0;j<columnas;j++){
            matriz[i][j]=rand()%5;
            printf(" %d",matriz[i][j]);
        }
        printf("\n");
    }
}

void calcularIndices(int **matriz,int numHilos){

    int residuo,FilaxProc,aux=0;

    FilaxProc=RowA/numHilos;
    residuo=RowA%numHilos;

    for(int i=0;i<numHilos;i++){
        for(int j=0;j<2;j++){
            if(j==0){
                matriz[i][j]=aux;
                printf("%d ",matriz[i][j]);
            }else{
                if(i==0)
                    aux=aux+FilaxProc+residuo;
                else
                    aux=aux+FilaxProc;

                matriz[i][j]=(aux-1);
                printf("%d ",matriz[i][j]);
            }
        }
        printf("\n");
    }
}

void *MultiMatrices(void* param){

    int *index = (int*)param;
    for(int h=matrizIndices[*index][0]; h<=matrizIndices[*index][1];h++){
        for(int i=0;i<ColumnB;i++){
            for(int j=0;j<RowA;j++){
                Respuesta[h][i]+=matrizA[h][j]*matrizB[j][i];
            }
        }
    }
}
