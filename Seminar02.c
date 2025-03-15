#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Masina {
    int id;
    int nrKm;
    char* model;
    float pret;
    char tipMotor;
};

struct Masina initializare(int id, int nrKm, const char* model, float pret, char tipMotor) {
    struct Masina m;
    m.id = id;
    m.nrKm = nrKm;
    m.pret = pret;
    m.tipMotor = tipMotor;
    m.model = (char*)malloc((strlen(model) + 1) * sizeof(char));
    if (m.model) {
        strcpy_s(m.model, strlen(model) + 1, model);
    }
    return m;
}

void afisare(struct Masina m) {
    printf("ID: %d\nNrKm: %d\nModel: %s\nPret: %.2f\nTipMotor: %c\n\n", m.id, m.nrKm, m.model, m.pret, m.tipMotor);
}

void afisareVector(struct Masina* vector, int nrElemente) {
    for (int i = 0; i < nrElemente; i++) {
        afisare(vector[i]);
    }
}

struct Masina* copiazaPrimeleNElemente(struct Masina* vector, int nrElemente, int* nrElementeCopiate) {
    if (vector == NULL || *nrElementeCopiate <= 0) {
        return NULL;
    }

    if (*nrElementeCopiate > nrElemente) {
        *nrElementeCopiate = nrElemente;
    }

    struct Masina* vectorNou = (struct Masina*)malloc(sizeof(struct Masina) * (*nrElementeCopiate));
    if (!vectorNou) {
        return NULL;
    }

    for (int i = 0; i < *nrElementeCopiate; i++) {
        vectorNou[i] = initializare(vector[i].id, vector[i].nrKm, vector[i].model, vector[i].pret, vector[i].tipMotor);
    }
    return vectorNou;
}

void dezalocare(struct Masina* m) {
    if (m->model) {
        free(m->model);
        m->model = NULL;
    }
}

void dezalocareVector(struct Masina** vector, int* nrElemente) {
    if (vector && *vector) {
        for (int i = 0; i < *nrElemente; i++) {
            dezalocare(&(*vector)[i]);
        }
        free(*vector);
        *vector = NULL;
        *nrElemente = 0;
    }
}

void copiazaAnumiteElemente(struct Masina* vector, int nrElemente, float prag, struct Masina** vectorNou, int* dimensiune) {
    *dimensiune = 0;

    for (int i = 0; i < nrElemente; i++) {
        if (vector[i].pret < prag) {
            (*dimensiune)++;
        }
    }

    if (*dimensiune == 0) {
        *vectorNou = NULL;
        return;
    }

    *vectorNou = (struct Masina*)malloc(sizeof(struct Masina) * (*dimensiune));
    if (!*vectorNou) {
        return;
    }

    int index = 0;
    for (int i = 0; i < nrElemente; i++) {
        if (vector[i].pret < prag) {
            (*vectorNou)[index] = initializare(vector[i].id, vector[i].nrKm, vector[i].model, vector[i].pret, vector[i].tipMotor);
            index++;
        }
    }
}

struct Masina getPrimulElementConditionat(struct Masina* vector, int nrElemente, const char* conditie) {
    for (int i = 0; i < nrElemente; i++) {
        if (strstr(vector[i].model, conditie)) {
            return initializare(vector[i].id, vector[i].nrKm, vector[i].model, vector[i].pret, vector[i].tipMotor);
        }
    }
    return initializare(0, 0, "None", 0.0, 'N');
}

int main() {
    int nrElemente = 3;
    struct Masina* vec = (struct Masina*)malloc(sizeof(struct Masina) * nrElemente);

    if (!vec) {
        printf("Eroare la alocare\n");
        return 1;
    }

    vec[0] = initializare(5, 200000, "Logan", 3000.5, 'B');
    vec[1] = initializare(3, 80000, "Porche", 300100.5, 'D');
    vec[2] = initializare(8, 100, "Cyber Truck", 30500, 'E');

    int nrElementeCopiate = 5;
    struct Masina* vec2 = copiazaPrimeleNElemente(vec, nrElemente, &nrElementeCopiate);

    struct Masina* vec3;
    int dimensiuneVec3;
    copiazaAnumiteElemente(vec, nrElemente, 100000, &vec3, &dimensiuneVec3);

    if (vec3) {
        afisareVector(vec3, dimensiuneVec3);
        dezalocareVector(&vec3, &dimensiuneVec3);
    }

    dezalocareVector(&vec, &nrElemente);
    dezalocareVector(&vec2, &nrElementeCopiate);

    return 0;
}
