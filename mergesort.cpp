//arquivo para teste do mergesort multiway, no qual dividimos os arquivos em k arquivos com aproximadamente o
// mesmo numero de entradas e depois os juntamos ao final, ja ordenados

#include "teste.h"


// tentar dividir o arquivo:

void intercala(int v[], int p, int q, int r){
    int i = p, j = q;
    int tamanho = r - p + 1;
    int w[tamanho]; // vetor auxiliar
    int k = 0;
    while ((i < q) and (j <= r)) {                
       if (v[i] <= v[j])  {
           w[k++] = v[i++]; /* w[k] = v[i]; k++; i++; */
		} else  {
			w[k++] = v[j++]; /* w[k] = v[j]; k++; j++; */
		}
	} 
    while (i < q) {
        w[k++] = v[i++];
    }   
    while (j <= r) {
        w[k++] = v[j++]; 
    }
    // agora copiamos do vetor auxiliar aux[] em v[p:r]
    for (int m = 0; m < tamanho; m++){ 
        v[p + m] = w[m];
    }
}
















