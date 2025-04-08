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

void afisareMasina(Masina masina) {
	//afiseaza toate atributele unei masini, toate informatiile din struct;
	//tinem cont de parametrul functiei, care este struct Masina !masina!; 
	printf("ID:%d\n", masina.id);
	printf("Nr usi:%d\n", masina.nrUsi);
	printf("Pret:%.2f\n", masina.pret);
	printf("Model:%s\n", masina.model);
	printf("Nume sofer:%s\n", masina.numeSofer);
	printf("Serie:%c\n\n", masina.serie);
}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	//afiseaza toate elemente de tip masina din vector prin apelarea functiei afisareMasina()
	//functia are ca parametru vectorul masini, il parcurgem si pt fiecare masina apelam functia afisareMasina
	for (int i = 0; i < nrMasini; i++) {
		afisareMasina(masini[i]);
	}
}

void adaugaMasinaInVector(Masina** masini, int * nrMasini, Masina masinaNoua) {
	//aceasta functie o vom apela in citireVectorMasiniFisier; vom citi o masina din fisier si cand o citim,
	// o punem in vector. cand o vom citi din fisier, ii vom aloca spatiu pt model, numele soferului..
	// daca i am alocat spatiu deja in heap in citireMasinaFisier, nu mai are rost sa facem deep copy aici
	//adauga in vectorul primit o noua masina pe care o primim ca parametru
	//ATENTIE - se modifica numarul de masini din vector; se adauga la finalul vectorului
	//alocam spatiu pt mai multe elemente (1 spatiu  in plus) si in noul spatiu adaugam masina noua
	//creem un vector nou si facem shallow copy; la final stergem vectorul initial, dar nu stergem zona de mem
	//zona de mem ramane pt noul vector;
	//redirectionam pointerul care pointa spre vectorul vechi catre adresa de memorie a noului vector
	
	// Masina* este un pointer către primul element din vector.Dacă vrei să schimbi acel
	// pointer (să realoci memoria, să-l redimensionezi), trebuie să transmiți adresa lui.
	// Adresa unui Masina* este Masina**. pointerul masina* va pointa catre o alta adresa de memorie, adresa
	// vectorului nou creat.

	Masina* aux = (Masina*)malloc(sizeof(Masina) * ((*nrMasini) + 1));
	// *nrMasini dereferențiază pointerul pentru a obține valoarea reală a numărului de mașini.
	for (int i = 0; i < *nrMasini; i++) { //deref nrMasini pt ca e transmis prin pointer
		aux[i] = (*masini)[i]; //prioritizam dereferentierea
	}
	// sizeof(Masina): dimens unei STRUCTURI de tip masina
	// (Masina*) converteste void* in Masina*

	aux[(*nrMasini)] = masinaNoua; //shallow copy pt ca vom folosi in continuare ~ zona de memorie
	// ca la vectorul initial
	// Când aveți *nrMasini mașini, ultima locație validă este *nrMasini - 1 
	// (pentru că indicii încep de la 0). Dacă vreți să adăugați o mașină, aceasta va merge\
	// în poziția următoare, adică la indexul* nrMasini (unde nu este nicio mașină în prezent).

	free(*masini); 
	(*masini) = aux; //adresa pe care o tin in aux este transmisa si poinerului masini. am legat primul vector
	// de vectorul nou dpdv al adreselor
	(*nrMasini)++;

	
}

Masina citireMasinaFisier(FILE* file) {
	//functia citeste o masina dintr-un stream deja deschis
	//masina citita este returnata;

	Masina m; //declaram o masina
	//filegets ca sa citeasca o linie intreaga
	//folosim strtok, care primeste separatorii ca parametru; returneaza un sir de caractere (=token=ce
	// gaseste pana la separator)
	//ca sa citim toata linia avem nevoie de buffer in care citim, putem sa l alocam si static
	char buffer[100]; // il alocam pe stiva de memorie a acestei functii; cand functia se termina,
	// si el va si sters, se va sterge zona de memorie respectiva; 100 de octeti
	char sep[4] = ",;\n"; //lista de 3 separatori + inca un octet pt sfarsit de sir de caractere
	fgets(buffer, 100, file);
	//acum luam din buffer fiecare element in parte(pret, sofer, serie...)
	m.id = atoi(strtok(buffer, sep)); //transformam din ascii in integer. m.id e tip int, 
	// strtok returneaza char*; citeste 1,
	m.nrUsi = atoi(strtok(NULL, sep)); //nu ii mai dam buffer ca sa plece de la 0, ii dam null ca sa
	// plece de unde a ramas 
	m.pret = atof(strtok(NULL, sep));
	// m.model este de tip char pointer si nu stim cat spatiu ii alocam, nu stim cat e pana la urmatorul sep
	// luam aux in care punem sirul Astra ca sa det ce lungime are; aux=in stiva de memorie a fct de fata
	char* aux;
	aux = strtok(NULL, sep);
	m.model = (char*)malloc(sizeof(char) * strlen(aux) + 1);
	strcpy(m.model, aux); //am copiat modelul
	aux = strtok(NULL, sep);
	m.numeSofer = (char*)malloc(sizeof(char) * strlen(aux) + 1);
	strcpy(m.model, aux);
	m.serie = strtok(NULL, sep)[0]; //din serie, care e un char, luam doar primul caracter, de pe poz 0
	return m;

		// strtok() returnează un pointer la primul caracter al fiecărui token extras
		//Variabila aux stocheaza un pointer către următorul token extras din buffer si
		// returnat de strtok. Deoarece strtok returnează un char*, 
		// trebuie să declarăm aux ca fiind char* pentru a putea gestiona aceste valori corect
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier 
	//prin apelul repetat al functiei citireMasinaFisier()
	//numarul de masini este determinat prin numarul de citiri din fisier
	//ATENTIE - la final inchidem fisierul/stream-ul

	// avem numele fisierului, pe baza lui deschidem un stream. dam streamul ca parametru functiei
	// citireMasinaFisier si ne va citi masina pe care o adaugam in vector cu ajutorul functiei adauga
	// adaugaMasinaInVector. astfel citim mai multe masini. fiecare masina este adaugata intr un vector
	// la final se returneaza vectorul Masina*, adica adresa unde avem acel vector si nr de masini citite

	FILE* file = fopen(numeFisier, "r");
	Masina* masini = NULL; //declaram un vector
	(*nrMasiniCitite) = 0;
	// acum apelam adaugaMasinaInVector pana ajungem la sf fisierului (endoffile e flag care ne zice ca am
	//ajuns la sf fisierului
	while (!feof(file))
	{
		adaugaMasinaInVector(&masini, nrMasiniCitite, citireMasinaFisier(file)); //adresa pointerului masini; 
			// nrMasiniCitite e deja adresa
	}
	fclose(file);
	return masini; //returnam vectorul
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	//pt fiecare elem din vector avem 2 pointeri, model si numeSofer pe care trb sa ii dezalocam
	for (int i = 0; i < (*nrMasini); i++) { //parcurgem tot vectorul
		if ((*vector)[i].model != NULL) { //vedem daca avem ce sa stergem

			free((*vector)[i].model); //am deref, indexat si accesat modelul
		}
		if ((*vector)[i].numeSofer!=NULL) {
			free((*vector)[i].numeSofer); //stergem pointerii coresp fiecarui element din vector
		}
		}
	free(*vector); //stergem vectorul
	(*vector) = NULL; 
	(*nrMasini) = 0;
}

int main() {
	int nrMasini = 0; //va fi in stiva lui main
	Masina* masini = citireVectorMasiniFisier("masini.txt", &nrMasini); //ii dam adresa din stiva lui main
	afisareVectorMasini(masini, nrMasini);
	dezalocareVectorMasini(&masini, &nrMasini);
	return 0;
}