#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType{
	struct RecordType record;
	struct HashType *next;
};

// Compute the hash function
int hash(int x, int hash_size){
	return x % hash_size;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData){
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;

	for (i=0;i<hashSz;++i){
		printf("Index %d ->", i);
		struct HashType *current = &pHashArray[i];
		while(current != NULL){
			printf("%d %c %d -> ", current->record.id, current->record.name, current->record.order);
			current = current->next;
		}		
		printf("\n");
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input_lab_9.txt", &pRecords);
	printRecords(pRecords, recordSz);
	int hash_size = 23;
	struct HashType *hashtable = (struct HashType *)malloc(sizeof(struct HashType)*hash_size);
	if(hashtable == NULL){
		printf("memory allocation failed");
		exit(-1);
	}
	for(int i = 0; i < hash_size; i++){
		hashtable[i].next = NULL;
	}
	for(int i = 0; i < recordSz; i++){
		int index = hash(pRecords[i].id, hash_size);
		struct HashType *newnode = (struct HashType*)malloc(sizeof(struct HashType));
		if(newnode == NULL){
			printf("memory allocation failed");
			exit(-1);
		}
		newnode->record = pRecords[i];
		newnode->next = NULL;
		if(hashtable[index].next == NULL){
			hashtable[index].next = newnode;
		}else{
			struct HashType *current = hashtable[index].next;
			while(current->next != NULL){
				current = current->next;
			}
			current->next = newnode;
		}		
    }
	displayRecordsInHash(hashtable, hash_size);
	free(pRecords);
    for (int i = 0; i < hash_size; ++i) {
        struct HashType *current = hashtable[i].next;
        while (current != NULL) {
            struct HashType *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(hashtable);

    return 0;
}
