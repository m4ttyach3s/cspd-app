#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<unistd.h>

/* AGGIORNAMENTO
il fattore base passa da 0.055 a 0.065
il fattore due passa da 0.050 a 0.060
il fattore tre passa da 0.045 a 0.055
*/

#define const 50
#define fattorebase 0.065
#define fattoredue 0.060
#define fattoretre 0.055
#define prezzofisso 6.50

//----------- FUNCTION AREA -----------//

/**
 * @brief crea il file csv per poter salvare i dati
 * 
 */

void filemaker(){
    FILE *ptr = fopen("calcolo_fattura.csv", "w");
    //sanity check
    if (ptr == NULL){
        printf("Errore nell'apertura del file. Esco dal programma.");
        exit(-1);
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(ptr, "\t\tANNO E MESE DEL CALCOLO: %d-%02d\nPESO LORDO, METRI CUBI, FACCHINAGGIO, ZTL, PREZZO CALCOLATO\n", tm.tm_year + 1900, tm.tm_mon +1);

    fclose(ptr);
}

/**
 * @brief compara il valore iniziale di j e f, assegna ad essi il valore calcolato a seconda della loro dimensione (peso lordo PL o metri cubi MC)
 * e decide quale dei due ha il valore più alto, ovvero il valore da inserire in output
 * 
 * @param j peso lordo
 * @param f metri cubi
 * @return float guadagno più alto ottenuto dalla comparazione
 */

float compare(float j, float f) {

	float result;

	if (j<=500 && j>=100) {
		j = j * fattorebase; //se il peso è tra 100 e 500 -> moltiplicalo per 0.055 (5.50€)
	} else if(j>500 && j<=1000) {
		j = j * fattoredue; //se il peso è tra 501 e 1000 -> moltiplicalo per 0.050 (5.00€)
	} else if(j>1000) {
		j = j * fattoretre; //se il peso è superiore a 1000 -> moltiplicalo per 0.045 (4.50€)
	} else if(j<100){
		j = prezzofisso;
	}

	if (f<=500 && f>=100) {
		f = f * fattorebase; //se il peso è tra 100 e 500 -> moltiplicalo per 0.055 (5.50€)
	} else if(f>500 && f<=1000) {
		f = f * fattoredue; //se il peso è tra 501 e 1000 -> moltiplicalo per 0.050 (5.00€)
	} else if(f>1000) {
		f = f * fattoretre; //se il peso è superiore a 1000 -> moltiplicalo per 0.045 (4.50€)
	} else if (f<100){
		f = prezzofisso;
	}

	printf("\n\n");
	printf("\n\tGuadagno considerando il Peso Lordo: %.2f", j);
	printf("\n\tGuadagno considerando i Metri Cubi: %.2f", f);
	printf("\n");

	if ( j > f) {
		result = j;
		return result;
	} else if (f > j) {
		result = f;
		return f;
	}
	if (j == f) {
		return j;
	}
	return result;
}

/**
 * @brief questa funzione mi calcola il valore assoluto del numero in input
 * e l'intero superiore per poter arrotondare o per eccesso o per difetto
 * 
 * @param pl valore da arrotondare
 * @return float del valore calcolato per il suo intero superiore o inferiore
 */

float fxroundpl (float pl) {

	float x, elle, newpl;
	x = fmod(pl, const);

	if (x!=0) {
		newpl = fmod(pl, 100); //trovo da 134 -> 34
		if (newpl < 50) {
			elle = const - newpl; //50 - modulo pl; 50-34
			elle = fabsf(elle); // valore assoulto di elle
			pl = pl + elle; //134 + (50-34) -> sarà sempre multiplo di 50 così
			return pl;
		} else if (newpl > 50) { //trovo 160 -> 60
			elle = const - newpl; //50 -60
			pl = (pl + elle) + const; //160 - 10 = 150 +50 = 200 arrotondato
			return pl;
		}
	}
	return pl;
}

/**
 * @brief appende i valori calcolati mano a mano
 * 
 * @param pl peso lordo inserito
 * @param mc metri cubi inseriti
 * @param ztl presenza o meno della ztl
 * @param fack presenza o meno del facchinaggio
 * @param result valore calcolato della singola riga
 */

void addvalues(float pl, float mc, float ztl, float fack, float result){
    FILE *ptr = fopen("calcolo_fattura.csv", "a+");
    fprintf(ptr, "%.2f, %.2f, %.2f, %.2f, %.2f€\n", pl, mc, fack, ztl, result+ztl+fack);
    fclose(ptr);
}

/**
 * @brief appende il valore della sponda, quando presente, e il totale.
 * 
 * @param sponda numero di viaggi con sponda
 * @param sum valore totale
 */

void addsponda(int sponda, float sum){
    FILE *ptr = fopen("calcolo_fattura.csv", "a+");

    if (sponda > 0){
        fprintf(ptr, "\n\nSponda,%d,,Aggiunto,%.2d€\nTOTALE CALCOLATO,-,-,-,%.2f€", sponda/10, sponda, sum);
    } else {
        fprintf(ptr, "\n\tTOTALE CALCOLATO,-,-,-,%.2f€", sum);
    }

    fclose(ptr);
}

/**
 * @brief calcola il valore di ogni viaggio, considerando l'user input di peso lordo pl, metri cubi mc, facchinaggio fack e ztl.
 * 
 * @param viaggi numero di viaggi da calcolare
 * @return float somma totale della fattura
 */

float computation(int viaggi){
    float pl, mc, j=0, f=0, result, mc_new, sum = 0, ztl = 0, fack = 0;
    int choice, addme;

    for(int i = 1; i<(viaggi+1); i++) {
		printf("\n\tInserisci il peso lordo del viaggio %d: ", i);
		scanf("%f", &pl);
		printf("\n\tInserisci i metri cubi del viaggio %d: ", i);
		scanf("%f", &mc);

		if (pl <100 && mc <0.50) {
			result = prezzofisso;
			int ztl_choice, fack_choice;
           
            ztl = 0; fack = 0;

			printf("\n\tIl viaggio aveva ZTL?\n\t\t1. Si\n\t\t2. No.\n\tScegli: ");
			scanf("%i", &ztl_choice);
			if (ztl_choice == 1){
				ztl = 4;
			}
			printf("\n\tIl viaggio prevedeva facchinaggio?\n\t\t1. Si\n\t\t2. No.\n\tScegli: ");
			scanf("%i", &fack_choice);
			if (fack_choice==1){
				fack = 5;
			}

			printf("\n\tRisultato[%i]: %.2f\n", i, result);
			sum = sum + result + ztl + fack;
			printf("\n\tSomma parziale: %.2f\n", sum);
			sleep(1);
		} else {
            ztl = 0; fack = 0;
			j = fxroundpl(pl);
			mc_new = mc * 200; //2.66 * 200 = 532
			f = fxroundpl(mc_new);

			printf("\n\tIl viaggio aveva ZTL?\n\t\t1. Si\n\t\t2. No.\n\tScegli: ");
			int ztl_choice;
			scanf("%i",&ztl_choice);

			printf("\n\tIl viaggio prevedeva facchinaggio?\n\t\t1. Si\n\t\t2. No.\n\tScegli: ");
			int fack_choice;
			scanf("%i", &fack_choice);

			if (ztl_choice == 1) {
				ztl = (j/const)*2;
				printf("\n\tValore di ZTL da aggiungere: %.2f\n", ztl);
			}
			if(fack_choice == 1) {
				fack = (j/const)*(2.5);
				printf("\n\tValore di facchinaggio da aggiungere: %.2f\n", fack);
			}

			printf("\n\tPeso Lordo arrotondato: %.2f", j);
			printf("\n\tMetri Cubi arrotondati: %.2f", f); //deve darmi 550

			result = compare(j, f);
			sum = sum + result + ztl + fack;

			printf("\033[0;31m");
			printf("\n\n\tRisultato[%i]: %.2f", i, result);
			printf("\n\n\tSomma parziale (si sono aggiunti eventuali ZTL e/o facchinaggi): %.2f", sum);
			printf("\033[0m");
			printf("\n");
			sleep(1);
		}
        addvalues(pl, mc, ztl, fack, result);
	}

	printf("\n\tSe i viaggi avevano sponda idraulica, specifica quanti.\n\tNel caso non ne avessero, scrivi il numero 0: ");
	int sponda;
	scanf("%i", &sponda);
	if (sponda > 0) {
		sponda = sponda * 10;
		printf("\n\tAggiungo %i al totale\n", sponda);
		sum = sum + sponda;
	}

    addsponda(sponda, sum);

    return sum;
}

//----------- END OF FUNCTION AREA -----------//