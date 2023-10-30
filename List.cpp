#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>
#include "List.h"

FILE* LogFile = 0;
static ListErrors ListOK(List* List);
static void EndProgramm(void);

static void EndProgramm(void) 
{
	printf("List log file successfully closed\n");
	fclose(LogFile);	
}

ListErrors ListInsert(List* List, const Elemt Value, const size_t Position)
{
	assert(List != nullptr);
	assert(Position < List->Capacity);
	assert(Position != 0);
	assert(Position != 1);

	if(ListOK(List)) return LIST_ERROR;
															// Recalloc add
	if (List->Prev[Position] != PoisonValue) {
		List->Data[List->Free] = List->Data[Position];
		List->Next[List->Free] = List->Next[Position];
		List->Prev[List->Free] = Position;

		List->Next[Position] = List->Free;
		List->Data[Position] = Value;
		
		List->Free = List->Next[List->Free];
	} else { // adding to the end of list or what?
		List->Data[Position] = Value;
		List->Next[Position] = List->Tail;
		List->Prev[Position] = List->Prev[List->Tail];

		List->Next[List->Prev[List->Tail]] = Position;

		List->Prev[List->Tail] = Position;
	}
	
	List->Size += 1;

	if(ListOK(List)) return LIST_ERROR;

	return LIST_OK;
}

ListErrors ListErase(List* List, const size_t Position)  // return pos next element ?
{
	assert(List != nullptr);
	assert(Position < List->Capacity);
	assert(Position != 0);   // copypaste
	assert(Position != 1);

	if(ListOK(List)) return LIST_ERROR;

	if (List->Prev[Position] == PoisonValue) {
		// Error
	} else {
		List->Next[List->Prev[Position]] = List->Next[Position];
		List->Prev[List->Next[Position]] = List->Prev[Position];

		List->Data[Position] = PoisonValue;
		List->Prev[Position] = PoisonValue;

		List->Next[Position] = List->Free;
		List->Free = Position;
	} 

	List->Size -= 1;

	if(ListOK(List)) return LIST_ERROR;
}

ListErrors ListResize(List* List, const size_t Size, const Elemt Value)
{
	assert(List != nullptr);
	
	if(ListOK(List)) return LIST_ERROR;

	if (Size > (List->Capacity - 2)) {

		Elemt* TemporPtr = realloc(List->Data, sizeof(Elemt) * (Size + 2));  // auto free memory?
		assert(TemporPtr != nullptr);

		List->Data = TemporPtr;

		int* TempPtr = realloc(List->Next, sizeof(int) * (Size + 2));
		assert(TempPtr != nullptr);
		
		List->Next = TempPtr;

		TempPtr = realloc(List->Prev, sizeof(int) * (Size + 2));
		assert(TempPtr != nullptr);

		List->Prev = TempPtr;
		
		for (size_t NumberElement = List->Capacity; NumberElement < (Size + 2) NumberElement++) {
			List->Prev[NumberElement] = PoisonValue;
			ListInsert(List, Value, NumberElement);
		}

		List->Size = Size;
		List->Capacity = (Size + 2);

	} else if (Size < (List->Capacity - 2)) {
		Elemt* TempPtrData = realloc(List->Data, sizeof(Elemt) * (Size + 2));
		assert(TempPtrData != nullptr);

		int* TempPtrNext = realloc(List->Next, sizeof(int) * (Size + 2));
		assert(TempPtrNext != nullptr);

		int* TempPtrPrev = realloc(List->Prev, sizeof(int) * (Size + 2));
		assert(TempPtrPrev != nullptr);

		size_t TotalPosition = 0;

		for(size_t NumberElement = 0; NumberElemnt < List->Capacity && TotalPosition < (Size + 2); NumberElemnt++) {
			if (List->Prev[NumberElement] != PoisonValue) {
				TempPtrData[TotalPosition] = List->Data[NumberElement];
				TempPtrNext[TotalPosition] = List->Next[NumberElement];
				TempPtrPrev[TotalPosition] = List->Prev[NumberElement];

				TotalPosition++;
			}
		}

		List->Data = TempPtrData;
		List->Next = TempPtrNext;
		List->Prev = TempPtrPrev;

		for (; TotalPosition < (Size + 2) TotalPosition++) {
			List->Prev[TotalPosition] = PoisonValue;
			ListInsert(List, Value, TotalPosition);
		}

		List->Size = Size
		List->Capacity = Size + 2;

	} else {
		// strange request
	}

	if(ListOK(List)) return LIST_ERROR;

}

ListErrors ListResize(List* List, const size_t Size)
{
	assert(List != nullptr);

	if(ListOK(List)) return LIST_ERROR;

	if (Size > (List->Capacity - 2)) {

		Elemt* TemporPtr = realloc(List->Data, sizeof(Elemt) * (Size + 2));  // auto free memory?
		assert(TemporPtr != nullptr);

		List->Data = TemporPtr;

		int* TempPtr = realloc(List->Next, sizeof(int) * (Size + 2));
		assert(TempPtr != nullptr);
		
		List->Next = TempPtr;

		TempPtr = realloc(List->Prev, sizeof(int) * (Size + 2));
		assert(TempPtr != nullptr);

		List->Prev = TempPtr;
		
		for (size_t NumberElement = List->Capacity; NumberElement < (Size + 2) NumberElement++) {
			List->Prev[NumberElement] = PoisonValue;
			List->Data[NumberElement] = PoisonValue;

			List->Next[NumberElement] = List->Free;
			List->Free = Position;	
		}

		List->Capacity = (Size + 2);

	} else if (Size < (List->Capacity - 2)) {
		Elemt* TempPtrData = realloc(List->Data, sizeof(Elemt) * (Size + 2));
		assert(TempPtrData != nullptr);

		int* TempPtrNext = realloc(List->Next, sizeof(int) * (Size + 2));
		assert(TempPtrNext != nullptr);

		int* TempPtrPrev = realloc(List->Prev, sizeof(int) * (Size + 2));
		assert(TempPtrPrev != nullptr);

		size_t TotalPosition = 0;

		for(size_t NumberElement = 0; NumberElemnt < List->Capacity && TotalPosition < (Size + 2); NumberElemnt++) {
			if (List->Prev[NumberElement] != PoisonValue) {
				TempPtrData[TotalPosition] = List->Data[NumberElement];
				TempPtrNext[TotalPosition] = List->Next[NumberElement];
				TempPtrPrev[TotalPosition] = List->Prev[NumberElement];

				TotalPosition++;
			}
		}

		List->Data = TempPtrData;
		List->Next = TempPtrNext;
		List->Prev = TempPtrPrev;

		for (; TotalPosition < (Size + 2) TotalPosition++) {
			List->Prev[TotalPosition] = PoisonValue;
			List->Data[TotalPosition] = PoisonValue;

			List->Next[NumberElement] = List->Free;
			List->Free = Position;
		}

		List->Size = Size
		List->Capacity = Size + 2;

	} else {
		// strange request
	}


	if(ListOK(List)) return LIST_ERROR;

}

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

	List->Free = 2;
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
	List->Free = PoisonValue;
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
	printf("    List Free Element = %ld\n", List->Free);

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






