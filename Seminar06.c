#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

typedef struct Nod nod;
struct Nod {
	Masina info;
	nod* next;
};

struct HashTable {
	int dim;
	nod** vector;
};
typedef struct HashTable HashTable;

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

void afisareListaMasini(nod* cap) {
	while (cap) {
		afisareMasina(cap->info);
		cap = cap->next;
	}

}

void adaugaMasinaInLista(nod** cap, Masina masinaNoua) {
	nod* temp = (nod*)malloc(sizeof(nod));
	temp->info = masinaNoua; //shallow copy
	temp->next = NULL;

	if ((*cap) == NULL) {
		*cap = temp;
	}
	else {
		nod* aux = (*cap);
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = temp;
	}
}

HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	ht.vector = (nod**)malloc(sizeof(nod*) * dimensiune);
	for (int i = 0; i < dimensiune; i++) {
		ht.vector[i] = NULL;
	}
	return ht;
}

int calculeazaHash(const char* cheieNumeSofer, int dimensiune) {
	if (dimensiune < 0) {
		return -1;
	}

	unsigned int sum = 0;

	for (int i = 0; i < strlen(cheieNumeSofer); i++) {
		sum += cheieNumeSofer[i];
	}

	sum += 777; // pentru noroc (e unic acum :) ) 
	sum %= dimensiune;

	return sum;
}

void inserareMasinaInTabela(HashTable hash, Masina masina) {
	
	int pozitie = calculeazaHash(masina.numeSofer, hash.dim);
	if (hash.vector[pozitie] == NULL) {
		adaugaMasinaInLista(&(hash.vector[pozitie]), masina); // nu avem coliziune, adaugam direct
	}
	else {
		adaugaMasinaInLista(&(hash.vector[pozitie]), masina); // avem coliziune
	}
}

HashTable citireMasiniDinFisier(const char* numeFisier, int dim) {
	FILE* f = fopen(numeFisier, "r");
	HashTable hash = initializareHashTable(dim);
	while (!feof(f)) {
		Masina masina = citireMasinaDinFisier(f);
		inserareMasinaInTabela(hash, masina);
	}
	fclose(f);
	return hash;
}

void afisareTabelaDeMasini(HashTable ht) {

	for (int i = 0; i < ht.dim; i++) {
		if (ht.vector[i] != NULL) {
			printf("Clusterul numarul %d\n", i);
			afisareListaMasini(ht.vector[i]);
			printf("\n\n");
		}
	}
}

void dezalocareTabelaDeMasini(HashTable* ht) {
	//sunt dezalocate toate masinile din tabela de dispersie
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	//calculeaza pretul mediu al masinilor din fiecare cluster.
	//trebuie sa returnam un vector cu valorile medii per cluster.
	//lungimea vectorului este data de numarul de clustere care contin masini
	return NULL;
}

Masina getMasinaDupaCheie(HashTable ht, const char* numeSofer) {
	Masina m;
	m.id = -1;

	int pozitie = calculeazaHash(numeSofer, ht.dim);

	if (pozitie < 0 || pozitie >= ht.dim || ht.vector == NULL)
		return m;

	nod* temp = ht.vector[pozitie];
	while (temp && strcmp(temp->info.numeSofer, numeSofer) != 0)
		temp = temp->next;
	if (temp != NULL) {
		m.numeSofer = (char*)malloc(strlen(numeSofer) + 1);
		strcpy(m.numeSofer, numeSofer);

		m.model = (char*)malloc(strlen(temp->info.model) + 1);
		strcpy(m.model, temp->info.model);

		m.id = temp->info.id;
		m.nrUsi = temp->info.nrUsi;
		m.pret = temp->info.pret;
		m.serie = temp->info.serie;
	}

	return m;
}

int main() {
	HashTable hash = citireMasiniDinFisier("Masini.txt", 10);
	afisareTabelaDeMasini(hash);
	printf("Masina lui Ionescu este: ");
	Masina IonescuMasina = getMasinaDupaCheie(hash, "Ionescu");
	if (IonescuMasina.id != -1) {
		afisareMasina(IonescuMasina);
		free(IonescuMasina.model);
		free(IonescuMasina.numeSofer);
	}
	else {
		printf("Nu am gasit masina!");
	}

	return 0;
}