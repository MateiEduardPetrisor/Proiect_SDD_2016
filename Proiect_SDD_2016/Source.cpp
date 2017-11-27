#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

struct Data
{
	int zi;
	int luna;
	int an;
};

struct Fisier
{
	char* numeFisier;
	char* numeComplet;
	char* extensieFisier;
	long long dimensiuneFisier;
	Data dataCreareFisiser;
};

struct NodLista
{
	Fisier info;
	NodLista* next;
};

struct HashTable
{
	int dimensiuneHashTable;
	NodLista** vectorListe;
};

struct NodArbore
{
	long long id;
	Fisier info;
	NodArbore* stanga;
	NodArbore* dreapta;
};

struct NodCoada
{
	NodArbore* info;
	NodCoada* next;
};

struct NodStiva
{
	NodArbore* info;
	NodStiva* next;
};

void deazalocaLista(NodLista* &lista);

void dezalocaHashTable(HashTable* &hashTable);

void dezalocaArboreNonRecursiv(NodArbore* &radacina);

HashTable* inserareHashTableChaining(HashTable* &hashTable, NodLista* nod);

NodLista* creareNodLista(Fisier fisier)
{
	NodLista* nod = (NodLista*)malloc(sizeof(NodLista));
	nod->info = fisier;
	nod->next = NULL;
	return nod;
}

NodLista* inserareInceput(NodLista* lista, NodLista* nod)
{
	if (lista == NULL)
	{
		lista = nod;
	}
	else
	{
		nod->next = lista;
		lista = nod;
	}
	return lista;
}

NodLista* inserareSfarsit(NodLista* lista, NodLista* nod)
{
	if (lista == NULL)
	{
		lista = nod;
	}
	else
	{
		NodLista* tmp = lista;
		while (tmp->next)
		{
			tmp = tmp->next;
		}
		tmp->next = nod;
	}
	return lista;
}

void afisareNodLista(NodLista* nod)
{
	if (nod != NULL)
	{
		printf("Nume Fisier: %s\n", nod->info.numeFisier);
		printf("Nume Complet: %s\n", nod->info.numeComplet);
		printf("Extensie Fisier: %s\n", nod->info.extensieFisier);
		printf("Dimensiune Fisier: %lld KB\n", nod->info.dimensiuneFisier / 1024);
		printf("Ziua: %d\n", nod->info.dataCreareFisiser.zi);
		printf("Luna: %d\n", nod->info.dataCreareFisiser.luna);
		printf("Anul: %d\n", nod->info.dataCreareFisiser.an);
	}
}

void afisareLista(NodLista* lista)
{
	if (lista != NULL)
	{
		NodLista* tmp = lista;
		while (tmp)
		{
			afisareNodLista(tmp);
			tmp = tmp->next;
		}
	}
	else
	{
		printf("Lista Goala\n");
	}
}

HashTable* creareHashTable(int dimensiuneHashTable)
{
	HashTable* hashTable = (HashTable*)malloc(sizeof(HashTable));
	hashTable->dimensiuneHashTable = dimensiuneHashTable;
	hashTable->vectorListe = (NodLista**)malloc(sizeof(NodLista*)*hashTable->dimensiuneHashTable);
	for (int indexHashTable = 0; indexHashTable < hashTable->dimensiuneHashTable; indexHashTable++)
	{
		hashTable->vectorListe[indexHashTable] = NULL;
	}
	return hashTable;
}

int char2int(char* numeFisier)
{
	int suma = 0;
	int lungimeString = strlen(numeFisier);
	for (int i = 0; i < lungimeString; i++)
	{
		suma += numeFisier[i];
	}
	return suma;
}

int functiaHash(HashTable* hashTable, char* numeFisier)
{
	return char2int(numeFisier) % hashTable->dimensiuneHashTable;
}

float calculeazaGradulDeIncarcare(HashTable* hashTable)
{
	int pozitiiOcupate = 0;
	if (hashTable != NULL)
	{
		if (hashTable->vectorListe != NULL)
		{
			for (int indexHashTable = 0; indexHashTable < hashTable->dimensiuneHashTable; indexHashTable++)
			{
				NodLista* tmp = hashTable->vectorListe[indexHashTable];
				if (tmp != NULL)
				{
					pozitiiOcupate++;
				}
			}
			return (float)pozitiiOcupate / (float)hashTable->dimensiuneHashTable;
		}
	}
}

void copiereHashTableinHashTable(HashTable* &rezultat, HashTable* sursa)
{
	if (sursa != NULL)
	{
		if (sursa->vectorListe != NULL)
		{
			for (int indexHashTable = 0; indexHashTable < sursa->dimensiuneHashTable; indexHashTable++)
			{
				if (sursa->vectorListe[indexHashTable] != NULL)
				{
					while (sursa->vectorListe[indexHashTable] != NULL)
					{
						Fisier fisier;
						fisier.numeFisier = (char*)malloc(sizeof(char)*strlen(sursa->vectorListe[indexHashTable]->info.numeFisier) + 1);
						strcpy(fisier.numeFisier, sursa->vectorListe[indexHashTable]->info.numeFisier);
						fisier.numeComplet = (char*)malloc(sizeof(char)*strlen(sursa->vectorListe[indexHashTable]->info.numeComplet) + 1);
						strcpy(fisier.numeComplet, sursa->vectorListe[indexHashTable]->info.numeComplet);
						fisier.extensieFisier = (char*)malloc(sizeof(char)*strlen(sursa->vectorListe[indexHashTable]->info.extensieFisier) + 1);
						strcpy(fisier.extensieFisier, sursa->vectorListe[indexHashTable]->info.extensieFisier);
						fisier.dimensiuneFisier = sursa->vectorListe[indexHashTable]->info.dimensiuneFisier;
						Data dataCreare;
						dataCreare.zi = sursa->vectorListe[indexHashTable]->info.dataCreareFisiser.zi;
						dataCreare.luna = sursa->vectorListe[indexHashTable]->info.dataCreareFisiser.luna;
						dataCreare.an = sursa->vectorListe[indexHashTable]->info.dataCreareFisiser.an;
						fisier.dataCreareFisiser = dataCreare;
						inserareHashTableChaining(rezultat, creareNodLista(fisier));
						sursa->vectorListe[indexHashTable] = sursa->vectorListe[indexHashTable]->next;
					}
				}
			}
		}
	}
}

HashTable* inserareHashTableChaining(HashTable* &hashTable, NodLista* nod)
{
	if (hashTable == NULL)
	{
		hashTable = creareHashTable(1);
	}
	if (calculeazaGradulDeIncarcare(hashTable) >= 0.75)
	{
		printf("Realocare\n");
		printf("Gradul De Ocupare %4.2f \n", calculeazaGradulDeIncarcare(hashTable));
		HashTable* rezultat = creareHashTable(hashTable->dimensiuneHashTable * 2);
		copiereHashTableinHashTable(rezultat, hashTable);
		dezalocaHashTable(hashTable);
		hashTable = rezultat;
	}
	int index = functiaHash(hashTable, nod->info.numeFisier);
	hashTable->vectorListe[index] = inserareInceput(hashTable->vectorListe[index], nod);
	return hashTable;
}

void afisareHashTable(HashTable* hashTable)
{
	if (hashTable != NULL)
	{
		if (hashTable->vectorListe != NULL)
		{
			for (int indexHashTable = 0; indexHashTable < hashTable->dimensiuneHashTable; indexHashTable++)
			{
				NodLista* tmp = hashTable->vectorListe[indexHashTable];
				while (tmp)
				{
					afisareNodLista(tmp);
					tmp = tmp->next;
				}
			}
		}
	}
	else
	{
		printf("Tabela Hash Goala\n");
	}
}

NodArbore* creareNodArbore(Fisier fisier, int &idFisier)
{
	NodArbore* nod = (NodArbore*)malloc(sizeof(NodArbore));
	nod->info = fisier;
	nod->id = idFisier;
	nod->stanga = NULL;
	nod->dreapta = NULL;
	idFisier = idFisier + 1;
	return nod;
}

NodArbore* adaugaStangaDreaptaNonRecursiv(NodArbore* radacina, NodArbore* nod)
{
	NodArbore* aux = NULL;
	if (radacina == NULL)
	{
		return nod;
	}
	else
	{
		aux = radacina;
		while (1)
		{
			if (nod->id < radacina->id)
			{
				if (radacina->stanga)
				{
					radacina = radacina->stanga;
				}
				else
				{
					radacina->stanga = nod;
					return aux;
				}
			}
			else
			{
				if (nod->id > radacina->id)
				{
					if (radacina->dreapta)
					{
						radacina = radacina->dreapta;
					}
					else
					{
						radacina->dreapta = nod;
						return aux;
					}
				}
				else
				{
					return aux;
				}
			}
		}
	}
}

void afisareNodArbore(NodArbore* nod)
{
	if (nod != NULL)
	{
		printf("Nume Fisier: %s\n", nod->info.numeFisier);
		printf("Nume Complet: %s\n", nod->info.numeComplet);
		printf("Extensie Fisier: %s\n", nod->info.extensieFisier);
		printf("Dimensiune Fisier: %lld KB\n", nod->info.dimensiuneFisier / 1024);
		printf("Ziua: %d\n", nod->info.dataCreareFisiser.zi);
		printf("Luna: %d\n", nod->info.dataCreareFisiser.luna);
		printf("Anul: %d\n", nod->info.dataCreareFisiser.an);
	}
}

NodCoada* creareNodCoada(NodArbore* nodArbore)
{
	NodCoada* nod = (NodCoada*)malloc(sizeof(NodCoada));
	nod->info = nodArbore;
	nod->next = NULL;
	return nod;
}

NodCoada* inserareSfarsit(NodCoada* coada, NodCoada* nod)
{
	if (coada == NULL)
	{
		coada = nod;
	}
	else
	{
		NodCoada* tmp = coada;
		while (tmp->next)
		{
			tmp = tmp->next;
		}
		tmp->next = nod;
	}
	return coada;
}

NodArbore* extragereInceput(NodCoada* &coada)
{
	if (coada != NULL)
	{
		NodArbore* nodExtras = NULL;
		while (coada != NULL)
		{
			NodCoada* extrage = coada;
			coada = coada->next;
			extrage->next = NULL;
			nodExtras = extrage->info;
			free(extrage);
			extrage = NULL;
			return nodExtras;
		}
	}
	else
	{
		printf("Coada Goala\n");
	}
}

NodStiva* creareNodStiva(NodArbore* nodArbore)
{
	NodStiva* nod = (NodStiva*)malloc(sizeof(NodArbore));
	nod->info = nodArbore;
	nod->next = NULL;
	return nod;
}

NodStiva* inserareInceput(NodStiva* stiva, NodStiva* nod)
{
	if (stiva == NULL)
	{
		stiva = nod;
	}
	else
	{
		nod->next = stiva;
		stiva = nod;
	}
	return stiva;
}

NodArbore* extragereInceput(NodStiva* &stiva)
{
	if (stiva != NULL)
	{
		NodArbore* nodExtras = NULL;
		while (stiva != NULL)
		{
			NodStiva* extrage = stiva;
			stiva = stiva->next;
			extrage->next = NULL;
			nodExtras = extrage->info;
			free(extrage);
			extrage = NULL;
			return nodExtras;
		}
	}
	else
	{
		printf("Stiva Goala\n");
	}
}

void parcurgeInordineNonRecursiv(NodArbore* radacina)
{
	if (radacina != NULL)
	{
		NodArbore* nodCurent = radacina;
		NodStiva* stiva = NULL;
		int gata = 0;
		while (gata == 0)
		{
			if (nodCurent != NULL)
			{
				NodStiva* nod = creareNodStiva(nodCurent);
				stiva = inserareInceput(stiva, nod);
				nodCurent = nodCurent->stanga;
			}
			else
			{
				if (stiva != NULL)
				{
					nodCurent = extragereInceput(stiva);
					afisareNodArbore(nodCurent);

					nodCurent = nodCurent->dreapta;
				}
				else
				{
					gata = 1;
				}
			}
		}
	}
}

Fisier creareFisier(char* directorStart, WIN32_FIND_DATAA cautaFisiere)
{
	LARGE_INTEGER marimeFisier;
	SYSTEMTIME timpSistem;
	LPSYSTEMTIME pointerCatreTimpSistem = NULL;
	char* pointerCatreExtensiaFisierului = NULL;
	Fisier fisier;
	Data dataCreare;
	fisier.numeFisier = (char*)malloc(sizeof(char) * strlen(cautaFisiere.cFileName) + 1);
	strcpy(fisier.numeFisier, cautaFisiere.cFileName);
	marimeFisier.HighPart = cautaFisiere.nFileSizeHigh;
	marimeFisier.LowPart = cautaFisiere.nFileSizeLow;
	fisier.dimensiuneFisier = marimeFisier.QuadPart;
	if (pointerCatreTimpSistem == NULL)
	{
		pointerCatreTimpSistem = &timpSistem;
	}
	bool test = FileTimeToSystemTime(&cautaFisiere.ftCreationTime, pointerCatreTimpSistem);
	if (test)
	{
		dataCreare.zi = pointerCatreTimpSistem->wDay;
		dataCreare.luna = pointerCatreTimpSistem->wMonth;
		dataCreare.an = pointerCatreTimpSistem->wYear;
		fisier.dataCreareFisiser = dataCreare;
	}
	int lungimeNumeComplet = (strlen(directorStart) + strlen(fisier.numeFisier) + strlen("\\")) + 1;
	fisier.numeComplet = (char*)malloc(sizeof(char)*lungimeNumeComplet);
	strcpy(fisier.numeComplet, directorStart);
	strcat(fisier.numeComplet, "\\");
	strcat(fisier.numeComplet, fisier.numeFisier);
	pointerCatreExtensiaFisierului = PathFindExtensionA(fisier.numeComplet);
	fisier.extensieFisier = (char*)malloc(sizeof(char)*strlen(pointerCatreExtensiaFisierului) + 1);
	strcpy(fisier.extensieFisier, pointerCatreExtensiaFisierului);

	return fisier;
}

void comandaLIST(char* directorStart, HashTable* &hashTable, short int &eroareLIST)
{
	if (eroareLIST == 0)
	{
		if (directorStart != NULL)
		{
			HANDLE handlerCautare;
			WIN32_FIND_DATAA cautaFisiere;
			LARGE_INTEGER marimeFisier;
			SYSTEMTIME timpSistem;
			LPSYSTEMTIME pointerCatreTimpSistem = NULL;
			char caleDirector[MAX_PATH];
			char* pointerCatreExtensiaFisierului = NULL;
			sprintf(caleDirector, "%s\\*.*", directorStart);
			if ((handlerCautare = FindFirstFileA(caleDirector, &cautaFisiere)) == INVALID_HANDLE_VALUE)
			{
				//fprintf(stderr, "FindFirstFIle failed on path = \"%s\"\n", caleDirector);
				eroareLIST = 1;
			}
			BOOL cont = TRUE;
			while (cont == TRUE)
			{
				if ((strncmp(".", cautaFisiere.cFileName, 1) != 0) && (strncmp("..", cautaFisiere.cFileName, 2) != 0))
				{
					if (cautaFisiere.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						sprintf(caleDirector, "%s\\%s", directorStart, cautaFisiere.cFileName);
						Fisier fisier;
						Data dataCreare;
						fisier.numeFisier = (char*)malloc(sizeof(char) * strlen(cautaFisiere.cFileName) + 1);
						strcpy(fisier.numeFisier, cautaFisiere.cFileName);
						marimeFisier.HighPart = cautaFisiere.nFileSizeHigh;
						marimeFisier.LowPart = cautaFisiere.nFileSizeLow;
						fisier.dimensiuneFisier = marimeFisier.QuadPart;
						if (pointerCatreTimpSistem == NULL)
						{
							pointerCatreTimpSistem = &timpSistem;
						}
						bool test = FileTimeToSystemTime(&cautaFisiere.ftCreationTime, pointerCatreTimpSistem);
						if (test)
						{
							dataCreare.zi = pointerCatreTimpSistem->wDay;
							dataCreare.luna = pointerCatreTimpSistem->wMonth;
							dataCreare.an = pointerCatreTimpSistem->wYear;

						}
						fisier.dataCreareFisiser = dataCreare;
						int lungimeNumeComplet = (strlen(directorStart) + strlen(fisier.numeFisier) + strlen("\\")) + 1;
						fisier.numeComplet = (char*)malloc(sizeof(char)*lungimeNumeComplet);
						strcpy(fisier.numeComplet, directorStart);
						strcat(fisier.numeComplet, "\\");
						strcat(fisier.numeComplet, fisier.numeFisier);
						pointerCatreExtensiaFisierului = PathFindExtensionA(fisier.numeComplet);
						fisier.extensieFisier = (char*)malloc(sizeof(char)*strlen(pointerCatreExtensiaFisierului) + 1);
						strcpy(fisier.extensieFisier, pointerCatreExtensiaFisierului);
						hashTable = inserareHashTableChaining(hashTable, creareNodLista(fisier));

						comandaLIST(caleDirector, hashTable, eroareLIST);
					}
					else
					{
						Fisier fisier;
						Data dataCreare;
						fisier.numeFisier = (char*)malloc(sizeof(char) * strlen(cautaFisiere.cFileName) + 1);
						strcpy(fisier.numeFisier, cautaFisiere.cFileName);
						marimeFisier.HighPart = cautaFisiere.nFileSizeHigh;
						marimeFisier.LowPart = cautaFisiere.nFileSizeLow;
						fisier.dimensiuneFisier = marimeFisier.QuadPart;
						if (pointerCatreTimpSistem == NULL)
						{
							pointerCatreTimpSistem = &timpSistem;
						}
						bool test = FileTimeToSystemTime(&cautaFisiere.ftCreationTime, pointerCatreTimpSistem);
						if (test)
						{
							dataCreare.zi = pointerCatreTimpSistem->wDay;
							dataCreare.luna = pointerCatreTimpSistem->wMonth;
							dataCreare.an = pointerCatreTimpSistem->wYear;
						}
						fisier.dataCreareFisiser = dataCreare;
						int lungimeNumeComplet = ((strlen(directorStart)) + (strlen(fisier.numeFisier)) + (strlen("\\"))) + 1;
						fisier.numeComplet = (char*)malloc(sizeof(char)*lungimeNumeComplet);
						strcpy(fisier.numeComplet, directorStart);
						strcat(fisier.numeComplet, "\\");
						strcat(fisier.numeComplet, fisier.numeFisier);
						pointerCatreExtensiaFisierului = PathFindExtensionA(fisier.numeComplet);
						fisier.extensieFisier = (char*)malloc(sizeof(char)*strlen(pointerCatreExtensiaFisierului) + 1);
						strcpy(fisier.extensieFisier, pointerCatreExtensiaFisierului);
						hashTable = inserareHashTableChaining(hashTable, creareNodLista(fisier));
					}
				}
				cont = FindNextFileA(handlerCautare, &cautaFisiere);
			}
			if (GetLastError() != ERROR_NO_MORE_FILES)
			{
				eroareLIST = 1;
			}
			if (FindClose(handlerCautare) == FALSE)
			{
				eroareLIST = 1;
			}
		}
		else
		{
			char* caleaAplicatiei = (char*)malloc(sizeof(char)*MAX_PATH + 1);
			GetCurrentDirectoryA(MAX_PATH, caleaAplicatiei);
			comandaLIST(caleaAplicatiei, hashTable, eroareLIST);
			free(caleaAplicatiei);
			caleaAplicatiei = NULL;
		}
	}
	else
	{
		dezalocaHashTable(hashTable);
	}
}

void comandaFILTER_FILES_AND_FOLDERS(char* directorStart, NodArbore* &arbore,
	short int &eroareFILTER, int &idFisier)
{
	if (eroareFILTER == 0)
	{
		if (directorStart != NULL)
		{
			HANDLE handlerCautare;
			WIN32_FIND_DATAA cautaFisiere;
			LPSYSTEMTIME pointerCatreTimpSistem = NULL;
			char caleDirector[MAX_PATH];
			char* pointerCatreExtensiaFisierului = NULL;

			sprintf(caleDirector, "%s\\*.*", directorStart);
			if ((handlerCautare = FindFirstFileA(caleDirector, &cautaFisiere)) == INVALID_HANDLE_VALUE)
			{
				eroareFILTER = 1;
			}
			BOOL cont = TRUE;
			while (cont == TRUE)
			{
				if ((strncmp(".", cautaFisiere.cFileName, 1) != 0) && (strncmp("..", cautaFisiere.cFileName, 2) != 0))
				{
					if (cautaFisiere.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						sprintf(caleDirector, "%s\\%s", directorStart, cautaFisiere.cFileName);
						Fisier fisier = creareFisier(directorStart, cautaFisiere);
						NodArbore* nod = creareNodArbore(fisier, idFisier);
						arbore = adaugaStangaDreaptaNonRecursiv(arbore, nod);
						//adaugaStangaDreapta(arbore, creareNodArbore(fisier, idFisier));
						comandaFILTER_FILES_AND_FOLDERS(caleDirector, arbore, eroareFILTER, idFisier);
					}
					else
					{
						Fisier fisier = creareFisier(directorStart, cautaFisiere);
						NodArbore* nod = creareNodArbore(fisier, idFisier);
						arbore = adaugaStangaDreaptaNonRecursiv(arbore, nod);
						//adaugaStangaDreapta(arbore, creareNodArbore(fisier, idFisier));
					}
				}
				cont = FindNextFileA(handlerCautare, &cautaFisiere);
			}
			if (GetLastError() != ERROR_NO_MORE_FILES)
			{
				eroareFILTER = 1;
			}
			if (FindClose(handlerCautare) == FALSE)
			{
				eroareFILTER = 1;
			}
		}
		else
		{
			char* caleaAplicatiei = (char*)malloc(sizeof(char)*MAX_PATH + 1);
			GetCurrentDirectoryA(MAX_PATH, caleaAplicatiei);
			comandaFILTER_FILES_AND_FOLDERS(caleaAplicatiei, arbore, eroareFILTER, idFisier);
			free(caleaAplicatiei);
			caleaAplicatiei = NULL;
		}
	}
	else
	{
		dezalocaArboreNonRecursiv(arbore);
	}
}

void comandaFILTER_ONLY_FILES(char* directorStart, NodArbore* &arbore,
	short int &eroareFILTER, int &idFisier)
{
	if (eroareFILTER == 0)
	{
		if (directorStart != NULL)
		{
			HANDLE handlerCautare;
			WIN32_FIND_DATAA cautaFisiere;
			LPSYSTEMTIME pointerCatreTimpSistem = NULL;
			char caleDirector[MAX_PATH];
			char* pointerCatreExtensiaFisierului = NULL;
			sprintf(caleDirector, "%s\\*.*", directorStart);
			if ((handlerCautare = FindFirstFileA(caleDirector, &cautaFisiere)) == INVALID_HANDLE_VALUE)
			{
				eroareFILTER = 1;
			}
			BOOL cont = TRUE;
			while (cont == TRUE)
			{
				if ((strncmp(".", cautaFisiere.cFileName, 1) != 0) && (strncmp("..", cautaFisiere.cFileName, 2) != 0))
				{
					if (cautaFisiere.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						sprintf(caleDirector, "%s\\%s", directorStart, cautaFisiere.cFileName);
						comandaFILTER_ONLY_FILES(caleDirector, arbore, eroareFILTER, idFisier);
					}
					else
					{
						Fisier fisier = creareFisier(directorStart, cautaFisiere);
						NodArbore* nod = creareNodArbore(fisier, idFisier);
						arbore = adaugaStangaDreaptaNonRecursiv(arbore, nod);
						//adaugaStangaDreapta(arbore, creareNodArbore(fisier, idFisier));
					}
				}
				cont = FindNextFileA(handlerCautare, &cautaFisiere);
			}
			if (GetLastError() != ERROR_NO_MORE_FILES)
			{
				eroareFILTER = 1;
			}
			if (FindClose(handlerCautare) == FALSE)
			{
				eroareFILTER = 1;
			}
		}
		else
		{
			char* caleaAplicatiei = (char*)malloc(sizeof(char)*MAX_PATH + 1);
			GetCurrentDirectoryA(MAX_PATH, caleaAplicatiei);
			comandaFILTER_ONLY_FILES(caleaAplicatiei, arbore, eroareFILTER, idFisier);
			free(caleaAplicatiei);
			caleaAplicatiei = NULL;
		}
	}
	else
	{
		dezalocaArboreNonRecursiv(arbore);
	}
}

void comandaFILTER_ONLY_DIRECTORIES(char* directorStart, NodArbore* &arbore,
	short int &eroareFILTER, int &idFisier)
{
	if (eroareFILTER == 0)
	{
		if (directorStart != NULL)
		{
			HANDLE handlerCautare;
			WIN32_FIND_DATAA cautaFisiere;
			LPSYSTEMTIME pointerCatreTimpSistem = NULL;
			char caleDirector[MAX_PATH];
			char* pointerCatreExtensiaFisierului = NULL;
			sprintf(caleDirector, "%s\\*.*", directorStart);
			if ((handlerCautare = FindFirstFileA(caleDirector, &cautaFisiere)) == INVALID_HANDLE_VALUE)
			{
				eroareFILTER = 1;
			}
			BOOL cont = TRUE;
			while (cont == TRUE)
			{
				if ((strncmp(".", cautaFisiere.cFileName, 1) != 0) && (strncmp("..", cautaFisiere.cFileName, 2) != 0))
				{
					if (cautaFisiere.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						sprintf(caleDirector, "%s\\%s", directorStart, cautaFisiere.cFileName);
						Fisier fisier = creareFisier(directorStart, cautaFisiere);
						//adaugaStangaDreapta(arbore, creareNodArbore(fisier, idFisier));
						NodArbore* nod = creareNodArbore(fisier, idFisier);
						arbore = adaugaStangaDreaptaNonRecursiv(arbore, nod);
						comandaFILTER_ONLY_DIRECTORIES(caleDirector, arbore, eroareFILTER, idFisier);
					}
				}
				cont = FindNextFileA(handlerCautare, &cautaFisiere);
			}
			if (GetLastError() != ERROR_NO_MORE_FILES)
			{
				eroareFILTER = 1;
			}
			if (FindClose(handlerCautare) == FALSE)
			{
				eroareFILTER = 1;
			}
		}
		else
		{
			char* caleaAplicatiei = (char*)malloc(sizeof(char)*MAX_PATH + 1);
			GetCurrentDirectoryA(MAX_PATH, caleaAplicatiei);
			comandaFILTER_ONLY_DIRECTORIES(caleaAplicatiei, arbore, eroareFILTER, idFisier);
			free(caleaAplicatiei);
			caleaAplicatiei = NULL;
		}
	}
	else
	{
		dezalocaArboreNonRecursiv(arbore);
	}
}

void comandaFILTER_FILES_AND_DIRECTORIES_SIZE(char* directorStart, NodArbore* &arbore,
	short int &eroareFILTER, char* fisierSauDirector, long long dimensiune, int &idFisier)
{
	if (eroareFILTER == 0)
	{
		if (directorStart != NULL)
		{
			HANDLE handlerCautare;
			WIN32_FIND_DATAA cautaFisiere;
			char caleDirector[MAX_PATH];
			char* pointerCatreExtensiaFisierului = NULL;
			sprintf(caleDirector, "%s\\*.*", directorStart);
			if ((handlerCautare = FindFirstFileA(caleDirector, &cautaFisiere)) == INVALID_HANDLE_VALUE)
			{
				eroareFILTER = 1;
			}
			BOOL cont = TRUE;
			while (cont == TRUE)
			{
				if ((strncmp(".", cautaFisiere.cFileName, 1) != 0) && (strncmp("..", cautaFisiere.cFileName, 2) != 0))
				{
					if (cautaFisiere.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						if (stricmp(fisierSauDirector, "-d") == 0)
						{
							Fisier fisier = creareFisier(directorStart, cautaFisiere);
							if (fisier.dimensiuneFisier <= dimensiune * 1024)
							{
								free(fisier.numeComplet);
								fisier.numeComplet = NULL;
								free(fisier.numeFisier);
								fisier.numeFisier = NULL;
								free(fisier.extensieFisier);
								fisier.extensieFisier = NULL;
							}
							else
							{
								adaugaStangaDreaptaNonRecursiv(arbore, creareNodArbore(fisier, idFisier));
							}
						}
						sprintf(caleDirector, "%s\\%s", directorStart, cautaFisiere.cFileName);
						comandaFILTER_FILES_AND_DIRECTORIES_SIZE(caleDirector, arbore, eroareFILTER, fisierSauDirector, dimensiune, idFisier);
					}
					else
					{
						if (stricmp(fisierSauDirector, "-f") == 0)
						{
							Fisier fisier = creareFisier(directorStart, cautaFisiere);
							if (fisier.dimensiuneFisier <= dimensiune * 1024)
							{
								free(fisier.numeComplet);
								fisier.numeComplet = NULL;
								free(fisier.numeFisier);
								fisier.numeFisier = NULL;
								free(fisier.extensieFisier);
								fisier.extensieFisier = NULL;
							}
							else
							{
								//adaugaStangaDreapta(arbore, creareNodArbore(fisier, idFisier));
								NodArbore* nod = creareNodArbore(fisier, idFisier);
								arbore = adaugaStangaDreaptaNonRecursiv(arbore, nod);
							}
						}
					}
				}
				cont = FindNextFileA(handlerCautare, &cautaFisiere);
			}
			if (GetLastError() != ERROR_NO_MORE_FILES)
			{
				eroareFILTER = 1;
			}
			if (FindClose(handlerCautare) == FALSE)
			{
				eroareFILTER = 1;
			}
		}
		else
		{
			char* caleaAplicatiei = (char*)malloc(sizeof(char)*MAX_PATH + 1);
			GetCurrentDirectoryA(MAX_PATH, caleaAplicatiei);
			comandaFILTER_FILES_AND_DIRECTORIES_SIZE(caleaAplicatiei, arbore, eroareFILTER, fisierSauDirector, dimensiune, idFisier);
			free(caleaAplicatiei);
			caleaAplicatiei = NULL;
		}
	}
	else
	{
		dezalocaArboreNonRecursiv(arbore);
	}
}

void comandaGET(HashTable* hashTable, NodLista* &lista, short int &eroareGET)
{
	if (hashTable != NULL)
	{
		if (hashTable->vectorListe != NULL)
		{
			for (int indexHashTable = 0; indexHashTable < hashTable->dimensiuneHashTable; indexHashTable++)
			{
				NodLista* tmp = hashTable->vectorListe[indexHashTable];
				while (tmp != NULL)
				{
					Fisier fisier;
					Data dataCrearii;
					fisier.numeFisier = (char*)malloc(sizeof(char) * strlen(tmp->info.numeFisier) + 1);
					strcpy(fisier.numeFisier, tmp->info.numeFisier);
					fisier.numeComplet = (char*)malloc(sizeof(char) * strlen(tmp->info.numeComplet) + 1);
					strcpy(fisier.numeComplet, tmp->info.numeComplet);
					fisier.extensieFisier = (char*)malloc(sizeof(char) * strlen(tmp->info.extensieFisier) + 1);
					strcpy(fisier.extensieFisier, tmp->info.extensieFisier);
					dataCrearii.an = tmp->info.dataCreareFisiser.an;
					dataCrearii.zi = tmp->info.dataCreareFisiser.zi;
					dataCrearii.luna = tmp->info.dataCreareFisiser.luna;
					fisier.dataCreareFisiser = dataCrearii;
					fisier.dimensiuneFisier = tmp->info.dimensiuneFisier;
					lista = inserareInceput(lista, creareNodLista(fisier));
					tmp = tmp->next;
				}
			}
		}
	}
	else
	{
		eroareGET = 1;
	}
}

void sortareAlfabeticaLista(NodLista* &lista)
{
	if (lista != NULL)
	{
		for (NodLista *nodCurent = lista; nodCurent->next != NULL; nodCurent = nodCurent->next)
		{
			for (NodLista *nodUrmator = nodCurent->next; nodUrmator != NULL; nodUrmator = nodUrmator->next)
			{
				if (strcmp(nodCurent->info.numeFisier, nodUrmator->info.numeFisier) > 0)
				{
					Fisier aux = nodCurent->info;
					nodCurent->info = nodUrmator->info;
					nodUrmator->info = aux;
				}
			}
		}
	}
}

void comandaGET_SORTARE_ALFABETICA(HashTable* hashTable, NodLista* &lista, short int &eroareGET)
{
	if (hashTable != NULL)
	{
		if (hashTable->vectorListe != NULL)
		{
			for (int indexHashTable = 0; indexHashTable < hashTable->dimensiuneHashTable; indexHashTable++)
			{
				NodLista* tmp = hashTable->vectorListe[indexHashTable];
				while (tmp != NULL)
				{
					Fisier fisier;
					Data dataCrearii;
					fisier.numeFisier = (char*)malloc(sizeof(char) * strlen(tmp->info.numeFisier) + 1);
					strcpy(fisier.numeFisier, tmp->info.numeFisier);
					fisier.numeComplet = (char*)malloc(sizeof(char) * strlen(tmp->info.numeComplet) + 1);
					strcpy(fisier.numeComplet, tmp->info.numeComplet);
					fisier.extensieFisier = (char*)malloc(sizeof(char) * strlen(tmp->info.extensieFisier) + 1);
					strcpy(fisier.extensieFisier, tmp->info.extensieFisier);
					dataCrearii.an = tmp->info.dataCreareFisiser.an;
					dataCrearii.zi = tmp->info.dataCreareFisiser.zi;
					dataCrearii.luna = tmp->info.dataCreareFisiser.luna;
					fisier.dataCreareFisiser = dataCrearii;
					fisier.dimensiuneFisier = tmp->info.dimensiuneFisier;
					lista = inserareInceput(lista, creareNodLista(fisier));
					tmp = tmp->next;
				}
			}
		}
		sortareAlfabeticaLista(lista);
	}
	else
	{
		eroareGET = 1;
	}
}

void comandaGET_SORTARE_ALFABETICA_SIZE(HashTable* hashTable, NodLista* &lista,
	short int &eroareGET, long long dimensiune)
{
	if (hashTable != NULL)
	{
		if (hashTable->vectorListe != NULL)
		{
			for (int indexHashTable = 0; indexHashTable < hashTable->dimensiuneHashTable; indexHashTable++)
			{
				NodLista* tmp = hashTable->vectorListe[indexHashTable];
				while (tmp != NULL)
				{
					if (tmp->info.dimensiuneFisier >= dimensiune * 1024)
					{
						Fisier fisier;
						Data dataCrearii;
						fisier.numeFisier = (char*)malloc(sizeof(char) * strlen(tmp->info.numeFisier) + 1);
						strcpy(fisier.numeFisier, tmp->info.numeFisier);
						fisier.numeComplet = (char*)malloc(sizeof(char) * strlen(tmp->info.numeComplet) + 1);
						strcpy(fisier.numeComplet, tmp->info.numeComplet);
						fisier.extensieFisier = (char*)malloc(sizeof(char) * strlen(tmp->info.extensieFisier) + 1);
						strcpy(fisier.extensieFisier, tmp->info.extensieFisier);
						dataCrearii.an = tmp->info.dataCreareFisiser.an;
						dataCrearii.zi = tmp->info.dataCreareFisiser.zi;
						dataCrearii.luna = tmp->info.dataCreareFisiser.luna;
						fisier.dataCreareFisiser = dataCrearii;
						fisier.dimensiuneFisier = tmp->info.dimensiuneFisier;
						lista = inserareInceput(lista, creareNodLista(fisier));
					}
					tmp = tmp->next;
				}
			}
		}
		sortareAlfabeticaLista(lista);
	}
	else
	{
		eroareGET = 1;
	}
}

int comparaDouaDateCalendaristice(Data dataInceput, Data dataSfarsit)
{
	if (dataInceput.an < dataSfarsit.an)
		return -1;
	else if (dataInceput.an > dataSfarsit.an)
		return 1;
	if (dataInceput.an == dataSfarsit.an)
	{
		if (dataInceput.luna < dataSfarsit.luna)
			return -1;
		else if (dataInceput.luna > dataSfarsit.luna)
			return 1;
		else if (dataInceput.zi < dataSfarsit.zi)
			return -1;
		else if (dataInceput.zi > dataSfarsit.zi)
			return 1;
		else
			return 0;
	}
}

void comandaGET_SORTARE_ALFABETICA_SIZE_DATE(HashTable* hashTable, NodLista* &lista,
	short int &eroareGET, long long dimensiune, Data data)
{
	if (hashTable != NULL)
	{
		if (hashTable->vectorListe != NULL)
		{
			for (int indexHashTable = 0; indexHashTable < hashTable->dimensiuneHashTable; indexHashTable++)
			{
				NodLista* tmp = hashTable->vectorListe[indexHashTable];
				while (tmp != NULL)
				{
					if ((tmp->info.dimensiuneFisier >= dimensiune * 1024))
					{
						int rezultat = comparaDouaDateCalendaristice(tmp->info.dataCreareFisiser, data);
						switch (rezultat)
						{
							Fisier fisier;
							fisier.numeComplet = NULL;
							fisier.numeFisier = NULL;
							fisier.extensieFisier = NULL;
							Data dataCrearii;
						case -1:
							//printf("Data %d/%d/%d Este Inainte De Data %d/%d %d", tmp->info.dataCreareFisiser.zi, tmp->info.dataCreareFisiser.luna, tmp->info.dataCreareFisiser.an, data.zi, data.luna, data.an);
							break;
						case 1:
							//printf("Data %d/%d/%d Este Dupa Data %d/%d/%d", tmp->info.dataCreareFisiser.zi, tmp->info.dataCreareFisiser.luna, tmp->info.dataCreareFisiser.an, data.zi, data.luna, data.an);
							fisier.numeFisier = (char*)malloc(sizeof(char) * strlen(tmp->info.numeFisier) + 1);
							strcpy(fisier.numeFisier, tmp->info.numeFisier);
							fisier.numeComplet = (char*)malloc(sizeof(char) * strlen(tmp->info.numeComplet) + 1);
							strcpy(fisier.numeComplet, tmp->info.numeComplet);
							fisier.extensieFisier = (char*)malloc(sizeof(char) * strlen(tmp->info.extensieFisier) + 1);
							strcpy(fisier.extensieFisier, tmp->info.extensieFisier);
							dataCrearii.an = tmp->info.dataCreareFisiser.an;
							dataCrearii.zi = tmp->info.dataCreareFisiser.zi;
							dataCrearii.luna = tmp->info.dataCreareFisiser.luna;
							fisier.dataCreareFisiser = dataCrearii;
							fisier.dimensiuneFisier = tmp->info.dimensiuneFisier;
							lista = inserareInceput(lista, creareNodLista(fisier));
							break;
						case 0:
							//printf("Data %d/%d/%d Este Egala Cu Data %d/%d/%d", tmp->info.dataCreareFisiser.zi, tmp->info.dataCreareFisiser.luna, tmp->info.dataCreareFisiser.an, data.zi, data.luna, data.an);
							fisier.numeFisier = (char*)malloc(sizeof(char) * strlen(tmp->info.numeFisier) + 1);
							strcpy(fisier.numeFisier, tmp->info.numeFisier);
							fisier.numeComplet = (char*)malloc(sizeof(char) * strlen(tmp->info.numeComplet) + 1);
							strcpy(fisier.numeComplet, tmp->info.numeComplet);
							fisier.extensieFisier = (char*)malloc(sizeof(char) * strlen(tmp->info.extensieFisier) + 1);
							strcpy(fisier.extensieFisier, tmp->info.extensieFisier);
							dataCrearii.an = tmp->info.dataCreareFisiser.an;
							dataCrearii.zi = tmp->info.dataCreareFisiser.zi;
							dataCrearii.luna = tmp->info.dataCreareFisiser.luna;
							fisier.dataCreareFisiser = dataCrearii;
							fisier.dimensiuneFisier = tmp->info.dimensiuneFisier;
							lista = inserareInceput(lista, creareNodLista(fisier));
							break;
						}
					}
					tmp = tmp->next;
				}
			}
		}
		sortareAlfabeticaLista(lista);
	}
	else
	{
		eroareGET = 1;
	}
}

void comandaGET_SORTARE_ALFABETICA_SIZE_DATE_EXTENSION(HashTable* hashTable,
	NodLista* &lista, short int &eroareGET, long long dimensiune, Data data, char* extensie)
{
	if (hashTable != NULL)
	{
		if (hashTable->vectorListe != NULL)
		{
			for (int indexHashTable = 0; indexHashTable < hashTable->dimensiuneHashTable; indexHashTable++)
			{
				NodLista* tmp = hashTable->vectorListe[indexHashTable];
				while (tmp != NULL)
				{
					if ((tmp->info.dimensiuneFisier >= dimensiune * 1024) && (stricmp(tmp->info.extensieFisier, extensie) == 0))
					{
						int rezultat = comparaDouaDateCalendaristice(tmp->info.dataCreareFisiser, data);
						switch (rezultat)
						{
							Fisier fisier;
							fisier.numeComplet = NULL;
							fisier.numeFisier = NULL;
							fisier.extensieFisier = NULL;
							Data dataCrearii;
						case -1:
							//printf("Data %d/%d/%d Este Inainte De Data %d/%d %d", tmp->info.dataCreareFisiser.zi, tmp->info.dataCreareFisiser.luna, tmp->info.dataCreareFisiser.an, data.zi, data.luna, data.an);
							break;
						case 1:
							//printf("Data %d/%d/%d Este Dupa Data %d/%d/%d", tmp->info.dataCreareFisiser.zi, tmp->info.dataCreareFisiser.luna, tmp->info.dataCreareFisiser.an, data.zi, data.luna, data.an);
							fisier.numeFisier = (char*)malloc(sizeof(char) * strlen(tmp->info.numeFisier) + 1);
							strcpy(fisier.numeFisier, tmp->info.numeFisier);
							fisier.numeComplet = (char*)malloc(sizeof(char) * strlen(tmp->info.numeComplet) + 1);
							strcpy(fisier.numeComplet, tmp->info.numeComplet);
							fisier.extensieFisier = (char*)malloc(sizeof(char) * strlen(tmp->info.extensieFisier) + 1);
							strcpy(fisier.extensieFisier, tmp->info.extensieFisier);
							dataCrearii.an = tmp->info.dataCreareFisiser.an;
							dataCrearii.zi = tmp->info.dataCreareFisiser.zi;
							dataCrearii.luna = tmp->info.dataCreareFisiser.luna;
							fisier.dataCreareFisiser = dataCrearii;
							fisier.dimensiuneFisier = tmp->info.dimensiuneFisier;
							lista = inserareInceput(lista, creareNodLista(fisier));
							break;
						case 0:
							//printf("Data %d/%d/%d Este Egala Cu Data %d/%d/%d", tmp->info.dataCreareFisiser.zi, tmp->info.dataCreareFisiser.luna, tmp->info.dataCreareFisiser.an, data.zi, data.luna, data.an);
							fisier.numeFisier = (char*)malloc(sizeof(char) * strlen(tmp->info.numeFisier) + 1);
							strcpy(fisier.numeFisier, tmp->info.numeFisier);
							fisier.numeComplet = (char*)malloc(sizeof(char) * strlen(tmp->info.numeComplet) + 1);
							strcpy(fisier.numeComplet, tmp->info.numeComplet);
							fisier.extensieFisier = (char*)malloc(sizeof(char) * strlen(tmp->info.extensieFisier) + 1);
							strcpy(fisier.extensieFisier, tmp->info.extensieFisier);
							dataCrearii.an = tmp->info.dataCreareFisiser.an;
							dataCrearii.zi = tmp->info.dataCreareFisiser.zi;
							dataCrearii.luna = tmp->info.dataCreareFisiser.luna;
							fisier.dataCreareFisiser = dataCrearii;
							fisier.dimensiuneFisier = tmp->info.dimensiuneFisier;
							lista = inserareInceput(lista, creareNodLista(fisier));
							break;
						}
					}
					tmp = tmp->next;
				}
			}
		}
		sortareAlfabeticaLista(lista);
	}
	else
	{
		eroareGET = 1;
	}
}

void afisareListaSortata(NodLista* lista)
{
	if (lista != NULL)
	{
		NodLista* tmp = lista;
		while (tmp)
		{
			printf("Nume Fisier: %s\n", tmp->info.numeFisier);
			printf("Extensie Fisier: %s\n", tmp->info.extensieFisier);
			printf("Dimensiune Fisier: %lld KB\n", tmp->info.dimensiuneFisier / 1024);
			tmp = tmp->next;
		}
	}
}

void afisareNumeComplet(NodLista* lista)
{
	if (lista != NULL)
	{
		NodLista* tmp = lista;
		while (tmp)
		{
			printf("Nume Complet: %s\n", tmp->info.numeComplet);
			tmp = tmp->next;
		}
	}
}

void comandaSAVEGET(NodLista* lista, char* numeFisier, short int &eroareSAVEGET)
{
	FILE* fisier = fopen(numeFisier, "w");
	if (fisier != NULL)
	{
		if (lista != NULL)
		{
			NodLista* tmp = lista;
			while (tmp)
			{
				fprintf(fisier, "%s ", tmp->info.numeFisier);
				fprintf(fisier, "%s ", tmp->info.numeComplet);
				fprintf(fisier, "%s ", tmp->info.extensieFisier);
				fprintf(fisier, "%lld KB ", tmp->info.dimensiuneFisier / 1024);
				fprintf(fisier, "%d ", tmp->info.dataCreareFisiser.zi);
				fprintf(fisier, "%d ", tmp->info.dataCreareFisiser.luna);
				fprintf(fisier, "%d\n", tmp->info.dataCreareFisiser.an);
				tmp = tmp->next;
			}
			fclose(fisier);
			fisier = NULL;
		}
		else
		{
			eroareSAVEGET = 1;
		}
	}
	else
	{
		eroareSAVEGET = 1;
	}
}

void salvareFisierLoguri(char* numeFisier, char** parametriiComenzii,
	int numarParametrii, short int codEroareComanda)
{
	FILE* fisierLoguri = fopen(numeFisier, "a+");
	if (fisierLoguri != NULL)
	{
		SYSTEMTIME timpSitem;
		GetLocalTime(&timpSitem);
		LPSYSTEMTIME pointerCatreTimpSistem = &timpSitem;
		int an = pointerCatreTimpSistem->wYear;
		int zi = pointerCatreTimpSistem->wDay;
		int luna = pointerCatreTimpSistem->wMonth;
		int ora = pointerCatreTimpSistem->wHour;
		int minut = pointerCatreTimpSistem->wMinute;
		int secunda = pointerCatreTimpSistem->wSecond;
		char* statusComanda = NULL;
		if (codEroareComanda == 0)
		{
			statusComanda = (char*)malloc(sizeof(char)* strlen("SUCCESS") + 1);
			strcpy(statusComanda, "SUCCESS");
		}
		else
		{
			statusComanda = (char*)malloc(sizeof(char)* strlen("ESEC") + 1);
			strcpy(statusComanda, "ESEC");
		}
		fprintf(fisierLoguri, "%d ", zi);
		fprintf(fisierLoguri, "%d ", luna);
		fprintf(fisierLoguri, "%d ", an);
		fprintf(fisierLoguri, "%d ", ora);
		fprintf(fisierLoguri, "%d ", minut);
		fprintf(fisierLoguri, "%d ", secunda);
		for (int i = 0; i < numarParametrii; i++)
		{
			fprintf(fisierLoguri, "%s ", parametriiComenzii[i]);
		}
		fprintf(fisierLoguri, "%s\n", statusComanda);
		free(statusComanda);
		statusComanda = NULL;
		fclose(fisierLoguri);
		fisierLoguri = NULL;
	}
	else
	{
		printf("~~~ Nu S-a Salvat Log-ul Comenzii ~~~\n");
	}
}

void stergeListaDeParametrii(int numarParametrii, char** &listaParametrii)
{
	if (listaParametrii != NULL)
	{
		for (int i = 0; i < numarParametrii; i++)
		{
			free(listaParametrii[i]);
			listaParametrii[i] = NULL;
		}
		free(listaParametrii);
		listaParametrii = NULL;
	}
}

void afiseaListaParametrii(int numarParametrii, char** listaParametrii)
{
	if (listaParametrii != NULL)
	{
		for (int i = 0; i < numarParametrii; i++)
		{
			if (listaParametrii[i] != NULL)
			{
				printf_s("param[%d] %s ", i, listaParametrii[i]);
			}
		}
		printf_s("\n");
	}
}

void creareListaParametrii(int numarParametrii, char** &listaParametrii)
{
	listaParametrii = (char**)malloc(sizeof(char*) * numarParametrii + 1);
	for (int i = 0; i < numarParametrii; i++)
	{
		listaParametrii[i] = NULL;
	}
}

void dezalocaArboreNonRecursiv(NodArbore* &radacina)
{
	if (radacina != NULL)
	{
		NodCoada* coada = NULL;
		coada = inserareSfarsit(coada, creareNodCoada(radacina));
		while (coada != NULL)
		{
			NodArbore* nod = extragereInceput(coada);
			if (nod->stanga != NULL)
			{
				coada = inserareSfarsit(coada, creareNodCoada(nod->stanga));
			}
			if (nod->dreapta != NULL)
			{
				coada = inserareSfarsit(coada, creareNodCoada(nod->dreapta));
			}
			//printf("Deleteting Nod @:%p\n",nod);
			free(nod->info.numeFisier);
			nod->info.numeFisier = NULL;
			free(nod->info.numeComplet);
			nod->info.numeComplet = NULL;
			free(nod->info.extensieFisier);
			nod->info.extensieFisier = NULL;
			free(nod);
			nod = NULL;
		}
		radacina = NULL;
	}
}

void deazalocaLista(NodLista* &lista)
{
	if (lista != NULL)
	{
		while (lista != NULL)
		{
			NodLista* tmp = lista;
			lista = lista->next;
			if (tmp != NULL)
			{
				tmp->next = NULL;
				free(tmp->info.numeFisier);
				tmp->info.numeFisier = NULL;
				free(tmp->info.numeComplet);
				tmp->info.numeComplet = NULL;
				free(tmp->info.extensieFisier);
				tmp->info.extensieFisier = NULL;
				free(tmp);
				tmp = NULL;
			}
		}
		free(lista);
		lista = NULL;
	}
}

void dezalocaHashTable(HashTable* &hashTable)
{
	if (hashTable != NULL)
	{
		if (hashTable->vectorListe != NULL)
		{
			for (int indexHashTable = 0; indexHashTable < hashTable->dimensiuneHashTable; indexHashTable++)
			{
				NodLista* tmp = hashTable->vectorListe[indexHashTable];
				while (tmp != NULL)
				{
					deazalocaLista(tmp);
				}
			}
			free(hashTable->vectorListe);
			hashTable->vectorListe = NULL;
			hashTable->dimensiuneHashTable = 0;
			free(hashTable);
			hashTable = NULL;
		}
	}
}

void exit(HashTable* &hashTable, NodArbore* &arbore, NodLista* &lista)
{
	dezalocaHashTable(hashTable);
	dezalocaArboreNonRecursiv(arbore);
	deazalocaLista(lista);
}

void afisareExempleComenzi()
{
	printf("\n~~~ Exemple De Comenzi Acceptate ~~~\n");
	printf("list -> considera fisiere + directoare din folderul aplicatiei\n");
	printf("list'D:\\BCK' -> considera fisiere + directoare din folderul BCK aflat in D:\n");
	printf("filter -> considera fisiere + directoare din folderul aplicatiei\n");
	printf("filter'D:\\BCK' -> considera fisiere + directoare din folderul BCK aflat D:\n");
	printf("filter'D:\\BCK''-d | -f' -> aceeasi ca filter'D:\\BCK' doar ca se iau in considerare fisierele sau directorele:\n");
	printf("filter'D:\\BCK''-d | -f''10' -> aceeasi ca filter'D:\\BCK''-f | -d' considerandu-se fisierele mai mari de 10 KB\n");
	printf("get -> considera fisiere + directoare din Hastable creat de comanda list\n");
	printf("get'-s'-> considera fisiere + directoare din Hastable creat de comanda list sortate alfabetic\n");
	printf("get'-s''-p' -> considera fisiere + directoare din Hastable creat de comanda list sortate alfabetic numele fiind precedat de locatia de pe disc\n");
	printf("get'-s''-p''10'-> aceeasi ca get'-s''-p' doar ca se iau in considerare fisierele mai mari de 10 KB\n");
	printf("get'-s''-p''10''1''2''2016' -> aceeasi ca get'-s''-p''10' doar ca se iau in considerare fisierele create inainte de data 01/02/2016\n");
	printf("get'-s''-p''10''1''2''2016''.exe'-> aceeasi ca get'-s''-p''10''1''2''2017' doar ca se iau in considerare fisierele cu extensia .exe\n");
	printf("saveget'fisier.txt' -> salveaza in fisier.txt continutul structurii create prin comanda get\n");
	printf("saveget'D:\\BCK\\fisier.txt' -> salvea fisier.txt la locatia precizata\n");
	printf("exit -> dezaloca structurile de date folosite.\n\n");
}

void main()
{
	int idFisier = 0;
	short int eroareLIST = 0;
	short int eroareFILTER = 0;
	short int eroareGET = 0;
	short int eroareSAVEGET = 0;
	HashTable* hashTable = creareHashTable(50);
	NodArbore* arbore = NULL;
	NodLista* lista = NULL;
	char bufferTastatura[200];
	char bufferFaraTerminatorLinieNoua[200];
	char bufferInserareInListaDeParametrii[200];
	int numarParametriiComanda = 0;
	int indexParametriiComanda = 0;
	char** parametriiComenzi = NULL;

	afisareExempleComenzi();

	while ((fgets(bufferTastatura, 200, stdin)) != NULL)
	{
		strncpy_s(bufferFaraTerminatorLinieNoua, bufferTastatura, strlen(bufferTastatura) - 1);
		strcpy(bufferInserareInListaDeParametrii, bufferFaraTerminatorLinieNoua);
		char delimitator[2] = "'";
		char* parti = strtok(bufferFaraTerminatorLinieNoua, delimitator);
		while (parti != NULL)
		{
			numarParametriiComanda = numarParametriiComanda + 1;
			parti = strtok(NULL, delimitator);
		}
		free(parti);
		parti = NULL;
		creareListaParametrii(numarParametriiComanda, parametriiComenzi);
		parti = strtok(bufferInserareInListaDeParametrii, delimitator);
		while (parti != NULL)
		{
			parametriiComenzi[indexParametriiComanda] = (char*)malloc(sizeof(char) * strlen(parti) + 1);
			strcpy(parametriiComenzi[indexParametriiComanda], parti);
			indexParametriiComanda = indexParametriiComanda + 1;
			parti = strtok(NULL, delimitator);
		}
		free(parti);
		parti = NULL;

		if (numarParametriiComanda == 0)
		{
			printf("~~~ Comanda Invalida ~~~\n");
			salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, 1);
			stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
			indexParametriiComanda = 0;
			numarParametriiComanda = 0;
		}
		else if ((stricmp(parametriiComenzi[0], "list") == 0))
		{
			if (numarParametriiComanda == 1)
			{
				comandaLIST(NULL, hashTable, eroareLIST);
				if (eroareLIST == 0)
				{
					afisareHashTable(hashTable);
					printf("~~~ Comanda LIST S-a Efectuat Cu Success ~~~\n");
					salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, eroareLIST);
					stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
					numarParametriiComanda = 0;
					indexParametriiComanda = 0;
					eroareLIST = 0;
					afisareExempleComenzi();
				}
				else if (eroareLIST == 1)
				{
					printf("~~~ Comanda LIST a ESUAT ~~~\n");
					salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, eroareLIST);
					stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
					numarParametriiComanda = 0;
					indexParametriiComanda = 0;
					eroareLIST = 0;
					afisareExempleComenzi();
				}
			}
			else if (numarParametriiComanda == 2)
			{
				comandaLIST(parametriiComenzi[1], hashTable, eroareLIST);
				if (eroareLIST == 0)
				{
					afisareHashTable(hashTable);
					printf("~~~ Comanda LIST %s S-a Efectuat Cu Success ~~~\n", parametriiComenzi[1]);
					salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, eroareLIST);
					stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
					numarParametriiComanda = 0;
					indexParametriiComanda = 0;
					eroareLIST = 0;
					afisareExempleComenzi();
				}
				else if (eroareLIST == 1)
				{
					printf("Comanda LIST %s a ESUAT\n", parametriiComenzi[1]);
					salvareFisierLoguri("logs.txt", parametriiComenzi, numarParametriiComanda, eroareLIST);
					stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
					numarParametriiComanda = 0;
					indexParametriiComanda = 0;
					eroareLIST = 0;
					afisareExempleComenzi();
				}
			}
			else
			{
				printf("Comanda LIST a ESUAT\n");
				salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, 1);
				stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
				numarParametriiComanda = 0;
				indexParametriiComanda = 0;
				eroareLIST = 0;
				afisareExempleComenzi();
			}
		}
		else if (stricmp(parametriiComenzi[0], "filter") == 0)
		{
			if (numarParametriiComanda == 1)	//filter
			{
				if (arbore == NULL)
				{
					comandaFILTER_FILES_AND_FOLDERS(NULL, arbore, eroareFILTER, idFisier);
				}
				else
				{
					dezalocaArboreNonRecursiv(arbore);
					idFisier = 0;
					comandaFILTER_FILES_AND_FOLDERS(NULL, arbore, eroareFILTER, idFisier);
				}
				if (eroareFILTER == 0)
				{
					parcurgeInordineNonRecursiv(arbore);
					printf("~~~ Comanda FILTER S-a Efectuat Cu SUCCESS ~~~\n");
					salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, eroareFILTER);
					stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
					numarParametriiComanda = 0;
					indexParametriiComanda = 0;
					eroareFILTER = 0;
					afisareExempleComenzi();
				}
				else if (eroareFILTER == 1)
				{
					salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, eroareFILTER);
					printf("~~~ Comanda FILTER a ESUAT ~~~\n");
					stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
					numarParametriiComanda = 0;
					indexParametriiComanda = 0;
					eroareFILTER = 0;
					afisareExempleComenzi();
				}
			}
			else if (numarParametriiComanda == 2)
			{
				if ((parametriiComenzi[0] != NULL) && (parametriiComenzi[1] != NULL))
				{
					if (parametriiComenzi[1] != NULL)
					{
						if (arbore == NULL)
						{
							comandaFILTER_FILES_AND_FOLDERS(parametriiComenzi[1], arbore, eroareFILTER, idFisier);	//filter 'D:\\Prj'
						}
						else
						{
							dezalocaArboreNonRecursiv(arbore);
							idFisier = 0;
							comandaFILTER_FILES_AND_FOLDERS(parametriiComenzi[1], arbore, eroareFILTER, idFisier);	//filter 'D:\\Prj'
						}
						if (eroareFILTER == 0)
						{
							parcurgeInordineNonRecursiv(arbore);
							printf("~~~ Comanda FILTER %s S-a Efectuat Cu SUCCESS ~~~\n", parametriiComenzi[1]);
							salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, eroareFILTER);
							stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
							numarParametriiComanda = 0;
							indexParametriiComanda = 0;
							eroareFILTER = 0;
							afisareExempleComenzi();
						}
						else if (eroareFILTER == 1)
						{
							printf("Comanda FILTER %s a ESUAT\n", parametriiComenzi[1]);
							salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, eroareFILTER);
							stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
							numarParametriiComanda = 0;
							indexParametriiComanda = 0;
							eroareFILTER = 0;
							afisareExempleComenzi();
						}
					}
				}
				else
				{
					printf("~~~ Comanda Invalida ~~~\n");
					salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, 1);
					stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
					numarParametriiComanda = 0;
					indexParametriiComanda = 0;
					eroareFILTER = 0;
					afisareExempleComenzi();
				}
			}
			else if (numarParametriiComanda == 3)
			{
				if ((parametriiComenzi[0] != NULL) && (parametriiComenzi[1] != NULL) && (stricmp(parametriiComenzi[2], "-f") == 0)) //FILTER 'D:\\Prj' -f
				{
					if (arbore == NULL)
					{
						comandaFILTER_ONLY_FILES(parametriiComenzi[1], arbore, eroareFILTER, idFisier);
					}
					else
					{
						dezalocaArboreNonRecursiv(arbore);
						idFisier = 0;
						comandaFILTER_ONLY_FILES(parametriiComenzi[1], arbore, eroareFILTER, idFisier);
					}
					if (eroareFILTER == 0)
					{
						parcurgeInordineNonRecursiv(arbore);
						printf("~~~ Comadna FILTER %s %s S-a Efectuat Cu Succes ~~~\n", parametriiComenzi[1], parametriiComenzi[2]);
						salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, eroareFILTER);
						stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
						numarParametriiComanda = 0;
						indexParametriiComanda = 0;
						eroareFILTER = 0;
						afisareExempleComenzi();
					}
					else if (eroareFILTER == 1)
					{
						printf("Comanda FILTER %s %s a ESUAT\n", parametriiComenzi[1], parametriiComenzi[2]);
						salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, eroareFILTER);
						stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
						numarParametriiComanda = 0;
						indexParametriiComanda = 0;
						eroareFILTER = 0;
						afisareExempleComenzi();
					}
				}
				else if ((parametriiComenzi[0] != NULL) && (parametriiComenzi[1] != NULL) && (stricmp(parametriiComenzi[2], "-d") == 0)) //filter 'D:\\Prj' -d
				{
					if (arbore == NULL)
					{
						comandaFILTER_ONLY_DIRECTORIES(parametriiComenzi[1], arbore, eroareFILTER, idFisier);
					}
					else
					{
						dezalocaArboreNonRecursiv(arbore);
						idFisier = 0;
						comandaFILTER_ONLY_DIRECTORIES(parametriiComenzi[1], arbore, eroareFILTER, idFisier);
					}
					if (eroareFILTER == 0)
					{
						parcurgeInordineNonRecursiv(arbore);
						salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, eroareFILTER);
						stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
						numarParametriiComanda = 0;
						indexParametriiComanda = 0;
						eroareFILTER = 0;
						afisareExempleComenzi();
					}
					else if (eroareFILTER == 1)
					{
						printf("Comanda FILTER 'path' -d a ESUAT\n");
						salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, eroareFILTER);
						stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
						numarParametriiComanda = 0;
						indexParametriiComanda = 0;
						eroareFILTER = 0;
						afisareExempleComenzi();
					}
				}
				else
				{
					printf("~~~ Comanda Invalida ~~~\n");
					salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, 1);
					stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
					numarParametriiComanda = 0;
					indexParametriiComanda = 0;
					eroareFILTER = 0;
					afisareExempleComenzi();
				}
			}
			else if (numarParametriiComanda == 4)
			{
				if ((parametriiComenzi[0] != NULL) && (parametriiComenzi[1] != NULL) && (parametriiComenzi[2] != NULL) && (parametriiComenzi[3] != NULL))
				{
					if (atoll(parametriiComenzi[3]) != 0)
					{
						if (arbore == NULL)
						{
							comandaFILTER_FILES_AND_DIRECTORIES_SIZE(parametriiComenzi[1], arbore, eroareFILTER, parametriiComenzi[2], atoll(parametriiComenzi[3]), idFisier); //filter 'D:\\Prj' -d | -f 10
						}
						else
						{
							dezalocaArboreNonRecursiv(arbore);
							idFisier = 0;
							comandaFILTER_FILES_AND_DIRECTORIES_SIZE(parametriiComenzi[1], arbore, eroareFILTER, parametriiComenzi[2], atoll(parametriiComenzi[3]), idFisier); //filter 'D:\\Prj' -d | -f 10
						}
						if (eroareFILTER == 0)
						{
							parcurgeInordineNonRecursiv(arbore);
							printf("~~~ Comanda FILTER %s %s %s S-a Efecuat Cu SUCCESS ~~~\n", parametriiComenzi[1], parametriiComenzi[2], parametriiComenzi[3]);
							salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, eroareFILTER);
							stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
							numarParametriiComanda = 0;
							indexParametriiComanda = 0;
							eroareFILTER = 0;
							afisareExempleComenzi();
						}
						else if (eroareFILTER == 1)
						{
							printf("~~~ Comanda FILTER %s %s %s a ESUAT ~~~\n", parametriiComenzi[1], parametriiComenzi[2], parametriiComenzi[3]);
							salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, eroareFILTER);
							stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
							numarParametriiComanda = 0;
							indexParametriiComanda = 0;
							eroareFILTER = 0;
							afisareExempleComenzi();
						}
					}
					else
					{
						printf("~~~ Comanda FILTER %s %s %s a ESUAT ~~~\n", parametriiComenzi[1], parametriiComenzi[2], parametriiComenzi[3]);
						salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, eroareFILTER);
						stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
						numarParametriiComanda = 0;
						indexParametriiComanda = 0;
						eroareFILTER = 0;
						afisareExempleComenzi();
					}
				}
				else
				{
					printf("~~~ Comanda Invalida ~~~\n");
					salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, 1);
					stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
					numarParametriiComanda = 0;
					indexParametriiComanda = 0;
					eroareFILTER = 0;
					afisareExempleComenzi();
				}
			}
			else
			{
				printf("~~~ Comanda Invalida ~~~\n");
				salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, 1);
				stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
				numarParametriiComanda = 0;
				indexParametriiComanda = 0;
				eroareFILTER = 0;
				afisareExempleComenzi();
			}
		}
		else if ((stricmp(parametriiComenzi[0], "get") == 0))
		{
			if (numarParametriiComanda == 1)
			{
				comandaGET(hashTable, lista, eroareGET);
				if (eroareGET == 0)
				{
					afisareLista(lista);
					printf("~~~ Comanda GET S-a Efectuat Cu SUCCESS ~~~\n");
					salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, eroareGET);
					stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
					numarParametriiComanda = 0;
					indexParametriiComanda = 0;
					eroareGET = 0;
					afisareExempleComenzi();
				}
				else if (eroareGET == 1)
				{
					printf("~~~ Comanda GET a ESUAT ~~~\n");
					salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, eroareGET);
					stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
					numarParametriiComanda = 0;
					indexParametriiComanda = 0;
					eroareGET = 0;
					afisareExempleComenzi();
				}
			}
			else if (numarParametriiComanda == 2)
			{
				if ((parametriiComenzi[0] != NULL) && (parametriiComenzi[1] != NULL))
				{
					if (stricmp(parametriiComenzi[1], "-s") == 0)
					{
						if (lista == NULL)
						{
							comandaGET_SORTARE_ALFABETICA(hashTable, lista, eroareGET);	//get -s
						}
						else
						{
							deazalocaLista(lista);
							comandaGET_SORTARE_ALFABETICA(hashTable, lista, eroareGET);
						}
						if (eroareGET == 0)
						{
							afisareListaSortata(lista);
							printf("~~~ Comanda GET %s S-a Efectuat Cu SUCCESS ~~~\n", parametriiComenzi[1]);
							salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, eroareGET);
							stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
							numarParametriiComanda = 0;
							indexParametriiComanda = 0;
							eroareGET = 0;
							afisareExempleComenzi();
						}
						else if (eroareGET == 1)
						{
							printf("~~~ Comanda GET %s a ESUAT ~~~\n", parametriiComenzi[1]);
							salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, eroareGET);
							stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
							numarParametriiComanda = 0;
							indexParametriiComanda = 0;
							eroareGET = 0;
							afisareExempleComenzi();
						}
					}
					else
					{
						printf("~~~ Comanda Invalida ~~~\n");
						salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, 1);
						stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
						numarParametriiComanda = 0;
						indexParametriiComanda = 0;
						eroareGET = 0;
						afisareExempleComenzi();
					}
				}
			}
			else if (numarParametriiComanda == 3)
			{
				if ((parametriiComenzi[1] != NULL) && (parametriiComenzi[2] != NULL))
				{
					if (stricmp(parametriiComenzi[1], "-s") == 0 && (stricmp(parametriiComenzi[2], "-p") == 0))
					{
						if (lista == NULL)
						{
							comandaGET_SORTARE_ALFABETICA(hashTable, lista, eroareGET);	//get -s -p
						}
						else
						{
							deazalocaLista(lista);
							comandaGET_SORTARE_ALFABETICA(hashTable, lista, eroareGET);	//get -s -p
						}
						if (eroareGET == 0)
						{
							afisareNumeComplet(lista);
							printf("~~~ Comanda GET %s %s S-a Efectuat Cu SUCCESS ~~~\n", parametriiComenzi[1], parametriiComenzi[2]);
							salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, eroareGET);
							stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
							numarParametriiComanda = 0;
							indexParametriiComanda = 0;
							eroareGET = 0;
							afisareExempleComenzi();
						}
						else if (eroareGET == 1)
						{
							printf("~~~ Comanda GET %s %s a ESUAT ~~~\n", parametriiComenzi[1], parametriiComenzi[2]);
							salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, eroareGET);
							stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
							numarParametriiComanda = 0;
							indexParametriiComanda = 0;
							eroareGET = 0;
							afisareExempleComenzi();
						}
					}
					else
					{
						printf("~~~ Comanda invalida ~~~\n");
						salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, 1);
						stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
						numarParametriiComanda = 0;
						indexParametriiComanda = 0;
						eroareGET = 0;
						afisareExempleComenzi();
					}
				}
			}
			else if (numarParametriiComanda == 4)
			{
				if ((parametriiComenzi[1] != NULL) && (parametriiComenzi[2] != NULL) && (parametriiComenzi[3] != NULL))
				{
					if (stricmp(parametriiComenzi[1], "-s") == 0 &&
						(stricmp(parametriiComenzi[2], "-p") == 0) && (atoll(parametriiComenzi[3]) != 0))
					{
						if (lista == NULL)
						{
							comandaGET_SORTARE_ALFABETICA_SIZE(hashTable, lista, eroareGET, (atoll(parametriiComenzi[3])));
						}
						else
						{
							deazalocaLista(lista);
							comandaGET_SORTARE_ALFABETICA_SIZE(hashTable, lista, eroareGET, (atoll(parametriiComenzi[3])));
						}
						if (eroareGET == 0)
						{
							afisareLista(lista);
							printf("~~~ Comanda GET %s %s %s S-a Efectuat Cu SUCCESS ~~~\n", parametriiComenzi[1], parametriiComenzi[2], parametriiComenzi[3]);
							salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, eroareGET);
							stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
							numarParametriiComanda = 0;
							indexParametriiComanda = 0;
							eroareGET = 0;
							afisareExempleComenzi();
						}
						else if (eroareGET == 1)
						{
							printf("~~~ Comanda GET %s %s %s a ESUAT ~~~\n", parametriiComenzi[1], parametriiComenzi[2], parametriiComenzi[3]);
							salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, eroareGET);
							stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
							numarParametriiComanda = 0;
							indexParametriiComanda = 0;
							eroareGET = 0;
							afisareExempleComenzi();
						}
					}
					else
					{
						printf("~~~ Comanda Invalida ~~~\n");
						salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, 1);
						stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
						numarParametriiComanda = 0;
						indexParametriiComanda = 0;
						eroareGET = 0;
						afisareExempleComenzi();
					}
				}
			}
			else if (numarParametriiComanda == 7)
			{
				if (stricmp(parametriiComenzi[1], "-s") == 0 && (stricmp(parametriiComenzi[2], "-p") == 0) &&
					(atoll(parametriiComenzi[3]) != 0) && (atoi(parametriiComenzi[4]) != 0) &&
					(atoi(parametriiComenzi[5]) != 0) && (atoi(parametriiComenzi[6]) != 0))
				{
					if (lista == NULL)
					{
						Data data;
						data.zi = atoi(parametriiComenzi[4]);
						data.luna = atoi(parametriiComenzi[5]);
						data.an = atoi(parametriiComenzi[6]);
						comandaGET_SORTARE_ALFABETICA_SIZE_DATE(hashTable, lista, eroareGET, (atoll(parametriiComenzi[3])), data);
					}
					else
					{
						deazalocaLista(lista);
						Data data;
						data.zi = atoi(parametriiComenzi[4]);
						data.luna = atoi(parametriiComenzi[5]);
						data.an = atoi(parametriiComenzi[6]);
						comandaGET_SORTARE_ALFABETICA_SIZE_DATE(hashTable, lista, eroareGET, (atoll(parametriiComenzi[3])), data);
					}

					if (eroareGET == 0)
					{
						afisareLista(lista);
						printf("~~~ Comanda GET %s %s %s %s %s %s S-a Efectuat Cu SUCCESS ~~~\n", parametriiComenzi[1], parametriiComenzi[2],
							parametriiComenzi[3], parametriiComenzi[4], parametriiComenzi[5], parametriiComenzi[6]);
						salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, eroareGET);
						stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
						numarParametriiComanda = 0;
						indexParametriiComanda = 0;
						eroareGET = 0;
						afisareExempleComenzi();
					}
					else if (eroareGET == 1)
					{
						printf("~~~ Comanda GET %s %s %s %s %s %s a ESUAT ~~~\n", parametriiComenzi[1], parametriiComenzi[2],
							parametriiComenzi[3], parametriiComenzi[4], parametriiComenzi[5], parametriiComenzi[6]);
						salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, eroareGET);
						stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
						numarParametriiComanda = 0;
						indexParametriiComanda = 0;
						eroareGET = 0;
						afisareExempleComenzi();
					}
				}
				else
				{
					printf("~~~ Comanda Invdalida ~~~\n");
					salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, 1);
					stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
					numarParametriiComanda = 0;
					indexParametriiComanda = 0;
					eroareGET = 0;
					afisareExempleComenzi();
				}
			}
			else if (numarParametriiComanda == 8)
			{
				if (stricmp(parametriiComenzi[1], "-s") == 0 && (stricmp(parametriiComenzi[2], "-p") == 0) &&
					(atoll(parametriiComenzi[3]) != 0) && (atoi(parametriiComenzi[4]) != 0) &&
					(atoi(parametriiComenzi[5]) != 0) && (atoi(parametriiComenzi[6]) != 0) &&
					(parametriiComenzi[6] != NULL))
				{
					if (lista == NULL)
					{
						Data data;
						data.zi = atoi(parametriiComenzi[4]);
						data.luna = atoi(parametriiComenzi[5]);
						data.an = atoi(parametriiComenzi[6]);
						comandaGET_SORTARE_ALFABETICA_SIZE_DATE_EXTENSION(hashTable, lista, eroareGET, (atoll(parametriiComenzi[3])), data, parametriiComenzi[7]);
					}
					else
					{
						deazalocaLista(lista);
						Data data;
						data.zi = atoi(parametriiComenzi[4]);
						data.luna = atoi(parametriiComenzi[5]);
						data.an = atoi(parametriiComenzi[6]);
						comandaGET_SORTARE_ALFABETICA_SIZE_DATE_EXTENSION(hashTable, lista, eroareGET, (atoll(parametriiComenzi[3])), data, parametriiComenzi[7]);
					}
					if (eroareGET == 0)
					{
						afisareLista(lista);
						printf("~~~ Comanda GET %s %s %s %s %s %s %s S-a Efecuat Cu SUCCESS ~~~\n", parametriiComenzi[1], parametriiComenzi[2],
							parametriiComenzi[3], parametriiComenzi[4], parametriiComenzi[5], parametriiComenzi[6], parametriiComenzi[7]);
						salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, eroareGET);
						stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
						numarParametriiComanda = 0;
						indexParametriiComanda = 0;
						eroareGET = 0;
						afisareExempleComenzi();
					}
					else if (eroareGET == 1)
					{
						printf("~~~ Comanda GET %s %s %s %s %s %s %s a ESUAT ~~~\n", parametriiComenzi[1], parametriiComenzi[2],
							parametriiComenzi[3], parametriiComenzi[4], parametriiComenzi[5], parametriiComenzi[6], parametriiComenzi[7]);
						salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, eroareGET);
						stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
						numarParametriiComanda = 0;
						indexParametriiComanda = 0;
						eroareGET = 0;
						afisareExempleComenzi();
					}
				}
				else
				{
					printf("~~~ Comanda Invalida ~~~\n");
					salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, 1);
					stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
					numarParametriiComanda = 0;
					indexParametriiComanda = 0;
					eroareGET = 0;
					afisareExempleComenzi();
				}
			}
			else
			{
				printf("~~~ Comanda Invalida ~~~\n");
				salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, 1);
				stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
				numarParametriiComanda = 0;
				indexParametriiComanda = 0;
				eroareGET = 0;
				afisareExempleComenzi();
			}
		}
		else if ((stricmp(parametriiComenzi[0], "saveget")) == 0)
		{
			if (numarParametriiComanda == 2)
			{
				if ((parametriiComenzi[0] != NULL) && (parametriiComenzi[1] != NULL))
				{
					comandaSAVEGET(lista, parametriiComenzi[1], eroareSAVEGET);
					if (eroareSAVEGET == 0)
					{
						printf("~~~ Comanda SAVEGET %s S-a Efecuat Cu SUCCESS ~~~\n", parametriiComenzi[1]);
						salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, eroareSAVEGET);
						stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
						numarParametriiComanda = 0;
						indexParametriiComanda = 0;
						eroareSAVEGET = 0;
						afisareExempleComenzi();
					}
					else if (eroareSAVEGET == 1)
					{
						printf("~~~ Comanda SAVEGET %s a ESUAT~~~\n", parametriiComenzi[1]);
						salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, eroareSAVEGET);
						stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
						numarParametriiComanda = 0;
						indexParametriiComanda = 0;
						eroareSAVEGET = 0;
						afisareExempleComenzi();
					}
				}
			}
			else
			{
				printf("~~~ Comanda Invalida ~~~\n");
				salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, 1);
				stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
				numarParametriiComanda = 0;
				indexParametriiComanda = 0;
				eroareSAVEGET = 0;
				afisareExempleComenzi();
			}
		}
		else if (stricmp(parametriiComenzi[0], "exit") == 0)
		{
			if (numarParametriiComanda == 1)
			{
				exit(hashTable, arbore, lista);
				salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, 0);
				stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
				numarParametriiComanda = 0;
				indexParametriiComanda = 0;
				eroareFILTER = 0;
				eroareLIST = 0;
				eroareGET = 0;
				eroareSAVEGET = 0;
				printf("~~~ Comanda EXIT S-a Efecuat Cu SUCCESS ~~~\n");
				_getch();
				break;
			}
			else
			{
				printf("~~~ Comanda Invalida ~~~\n");
				salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, 1);
				stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
				numarParametriiComanda = 0;
				indexParametriiComanda = 0;
				eroareFILTER = 0;
				eroareLIST = 0;
				eroareGET = 0;
				eroareSAVEGET = 0;
				afisareExempleComenzi();
			}
		}
		else
		{
			printf("~~~ Comanda invalida ~~~\n");
			salvareFisierLoguri("Logs.txt", parametriiComenzi, numarParametriiComanda, 1);
			stergeListaDeParametrii(numarParametriiComanda, parametriiComenzi);
			numarParametriiComanda = 0;
			indexParametriiComanda = 0;
			eroareFILTER = 0;
			eroareLIST = 0;
			eroareGET = 0;
			eroareSAVEGET = 0;
			afisareExempleComenzi();
		}
	}
}