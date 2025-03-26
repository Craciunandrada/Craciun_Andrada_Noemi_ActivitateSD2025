#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;


typedef struct Nod {
	Masina info;
	struct Nod* next;
}Nod;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(Nod* cap) {
	

	while (cap) {
		afisareMasina(cap->info);
		cap = cap->next;
	}

}

void adaugaMasinaInLista(Nod** cap, Masina masinaNoua) {
	
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;

	if (*cap != NULL) {
		Nod* temp = (*cap);

		while (temp->next) {
			temp = temp->next;
		}

		temp->next = nou;
	}
	else {
		*cap = nou;
	}
}

void adaugaLaInceputInLista(Nod** cap, Masina masinaNoua) {
	
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = *cap;

	*cap = nou;
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {
	

	FILE* file = fopen(numeFisier, "r");
	Nod* cap = NULL;

	while (!feof(file)) {
		Masina m = citireMasinaDinFisier(file);
		adaugaLaInceputInLista(&cap, m);
	}

	return cap;

}

void dezalocareListaMasini(Nod** cap) {
	
	while (*cap) {
		Nod* temp = *cap;
		*cap = (*cap)->next;

		free(temp->info.model);
		free(temp->info.numeSofer);
		free(temp);
	}

	free(*cap);
}

float calculeazaPretMediu(Nod* cap) {

	float suma = 0;
	int nrMasini = 0;

	while (cap) {
		suma += cap->info.pret;
		nrMasini++;
		cap = cap->next;
	}

	if (!nrMasini)return 0;

	return (float)suma / nrMasini;
}

void stergeMasiniDinSeria(/*lista masini*/ char serieCautata) {
	
}

float calculeazaPretulMasinilorUnuiSofer(/*lista masini*/ const char* numeSofer) {
	return 0;
}

int main() {

	Nod* cap = citireListaMasiniDinFisier("masini.txt");

	float pretMediu = calculeazaPretMediu(cap);

	printf("Pretul mediu al masinilor este: %.02f\n", pretMediu);

	return 0;
}