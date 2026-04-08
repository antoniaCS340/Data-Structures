#include <stdio.h>

void Heapify(int A[], int i);
int main(void){
 int A[12];
    A[0] = 7;
    A[1]  = 4;
    A[2] = 3;
    A[3] = 11;
    A[4] = 21;
    A[5] = 15;
    A[6] = 14;
    A[7] = 26;
    A[8] = 23;
    A[9] = 30;
    A[10] = 27;
    A[11] = 19;
    
    for(int i = 1; i < 12; i++){
		Heapify(A, i);
        for (int i = 0; i < 12; i++){
            printf("%d,",A[i]);
        }
        printf("\n");
    }

}

void Heapify(int A[],int i){

	int m = 11;
	int lc = 2*m-12;
	int	rc = lc-1;
	int p;
	
	printf(" %d >= %d && A[%d] < A[%d] A[lc] = %d A[m] = %d \n",lc,i,lc,m,A[lc],A[m]);
	while(( lc >= i && A[lc] < A[m]) || (rc >= i && A[rc] < A[m])){
		printf("hello");
		lc = 2*m-11;
		rc = lc-1;
		if (rc >= i){
			if (A[lc] < A[rc]){
				p = lc;
			} else {
				p = rc;
			}
		} else {
			p = i;
		}

		//swap
		int temp = A[m];
		A[m] = A[p];
		A[p] = temp;
		m = p;

	}
}