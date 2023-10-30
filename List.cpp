#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>
#include "List.h"

FILE* LogFile = 0;

ListErrors ListCtor(List* List, size_t Capacity, const char* list_name,
				    const size_t birth_line, const char* birth_file, const char* birth_function) 
{    
	assert(List != nullptr);
	assert(Capacity > 0);    // think about Ctor after Dtor and about Ctor twice
 
	LogFile = fopen(NameLogFile, "w");
	atexit(EndProgramm);
	
	List->IsCtor = true;
	List->IsDtor = false;
	
	List->Info.list_name = list_name;
	List->Info.birth_function = birth_function;
	List->Info.birth_line = birth_line;
	List->Info.birth_file = birth_file;

	List->Capacity = Capacity + 2;

	List->Data = (Elemt*) calloc(List->Capacity, sizeof(Elemt));
	assert(List->Data != nullptr);

	List->Next = (int*) calloc(List->Capacity, sizeof(int));
	assert(List->Next != nullptr);

	List->Prev = (int*) calloc(List->Capacity, sizeof(int));
	assert(List->Prev != nullptr);

	for (size_t NumberElement = 0; NumberElement < List->Capacity; NumberElement++) {
		List->Data[NumberElement] = PoisonValue;
		List->Next[NumberElement] = NumberElement + 1;
		List->Prev[NumberElement] = PoisonValue;
	}

	List->Next[List->Capacity - 1] = 1;

	List->Head = HeadPosition;	
	List->Tail = TailPosition;
	List->Next[List->Head] = List->Tail;
	List->Prev[List->Head] = PrevHeadIndicator;
	List->Next[List->Tail] = NextTailIndicator;
	List->Prev[List->Tail] = List->Head;

	List->FreeElement = 2;
	List->Size = 0;

	List->ErrorsInfo = 0;

	if (ListOK(List)) return LIST_ERROR;

	return LIST_OK;
}

ListErrors ListDtor(List* List)
{
	assert(List != nullptr);
	assert(List->IsCtor == true);
	assert(List->IsDtor == false);

	if (ListOK(List)) return LIST_ERROR;

	List->IsDtor = true;

	for (size_t NumberElement = 0; NumberElement < List->Capacity; NumberElement++) {
		List->Data[NumberElement] = PoisonValue;
		List->Next[NumberElement] = PoisonValue;
		List->Prev[NumberElement] = PoisonValue;
	}
	free(List->Data);
	free(List->Next);
	free(List->Prev);
	List->Capacity = PoisonValue;
	List->FreeElement = PoisonValue;
	List->Head = PoisonValue;
	List->Tail = PoisonValue;
	List->Size = PoisonValue;
	
	return LIST_OK;
}

ListErrors ListOK(List* List)
{
	assert(List != nullptr);
	assert(List->IsCtor == true);  // copypaste in all functions
	assert(List->IsDtor == false);

	if (List->Capacity == nullptr) ;
	if (List->Size < List->Capacity) ;
	if (List->Data == nullptr);
	if (List->Next == nullptr);
	if (List->Prev == nullptr);

	for (size_t NumberElement = 0; NumberElement < List->Size; NumberElement) {
		List->Prev[List->Next[List->Head]] = List->Head;

	}

	return LIST_OK;
}

ListErrors ListDump(List* List, const size_t NLine, 
				    const char* NameFile, const char* Function)
{
	assert(List != nullptr);
	assert(List->Data != nullptr);
	assert(List->Next != nullptr);
	assert(List->Prev != nullptr);
	assert(List->IsCtor == true);
	assert(List->IsDtor == false);
	
	printf("List[%p] %s from %s(%ld) %s ", List, List->Info.list_name,
	List->Info.birth_file, List->Info.birth_line, List->Info.birth_function);

	printf("called from %s(%ld) %s.\n", NameFile, NLine, Function);

	printf("{\n    Number Elements = %ld\n", List->Size);
	printf("    List Capacity = %ld\n", List->Capacity);
	printf("    List Head = %ld\n", List->Head);
	printf("    List Tail = %ld\n", List->Tail);
	printf("    List Free Element = %ld\n", List->FreeElement);

	printf(" List Data[%p]\n    {\n        ", List->Data);
	for (size_t NumberElement = 0; NumberElement < List->Capacity; NumberElement++) {
		printf("%6ld ", NumberElement);
	}
	printf("\n");
	for (size_t NumberElement = 0; NumberElement < List->Capacity; NumberElement++) {
		if (List->Data[NumberElement] != PoisonValue) {
			printf("%6d ", List->Data[NumberElement]);
		} else {
			printf("Poison ");
		}
	}
	printf("    }\n");

	printf(" List Next Element[%p]\n    {\n        ", List->Next);
	for (size_t NumberElement = 0; NumberElement < List->Capacity; NumberElement++) {
		printf("%6ld ", NumberElement);
	}
	printf("\n");
	for (size_t NumberElement = 0; NumberElement < List->Capacity; NumberElement++) {
		if (List->Next[NumberElement] != PoisonValue) {
			printf("%6d ", List->Next[NumberElement]);
		} else {
			printf("Poison ");
		}
	}  // copypaste
	printf("    }\n");

	printf(" List Previous Element[%p]\n    {\n        ", List->Prev);
	for (size_t NumberElement = 0; NumberElement < List->Capacity; NumberElement++) {
		printf("%6ld ", NumberElement);
	}
	printf("\n");
	for (size_t NumberElement = 0; NumberElement < List->Capacity; NumberElement++) {
		if (List->Prev[NumberElement] != PoisonValue) {
			printf("%6d ", List->Prev[NumberElement]);
		} else {
			printf("Poison ");
		}
	}
	printf("    }\n");
	printf("}\n");

	return LIST_OK;
}






