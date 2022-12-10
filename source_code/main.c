#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<unistd.h>
#include"cspdapp.h"

int main(void) {

	float sum;
	int viaggi;

	// in *NIX environment, replace "cls" with "clear"
	system("cls");

    filemaker();

	printf("\n\n\t---------------------------------------\n\n");
	printf("\n\tScrivi quanti viaggi vuoi inserire: ");
	scanf("%d", &viaggi);
	printf("\n\n\t---------------- INIZIO ----------------\n\n");
	printf("\n\t--- NON PUOI USARE LA VIRGOLA PER RAPPRESENTARE I NUMERI DECIMALI (TIPO 2,34)\n\t--- MA DEVI USARE IL PUNTO COME AD ESEMPIO: 2.34\n\t");

    sum = computation(viaggi);

	printf("\033[0;33m");
	printf("\n\n\tSOMMA TOTALE: %.2f", sum);
	printf("\033[0m");
	printf("\n\n\tCalcolo finito. Ricopia il valore e poi premi 0 per uscire...\n");
	printf("\n\t");

	int uscita;
	scanf("%i", &uscita);
	printf("\n\n\t----------------- FINE -----------------\n\n");

	return 0;
}