#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>
#include "DotList.h"
#include "List.h"

FILE* LogFile = 0;

static ListErrors ListOK(List* List);
static void EndProgramm(void);

static size_t Counter = 1;

static void EndProgramm(void) 
{
	fprintf(LogFile,  "List log file successfully closed\n");
	fclose(LogFile);	
}

ListErrors ListInsert(List* List, const Elemt Value, const size_t Position)
{
	assert(List != nullptr);
	assert(Position < List->Capacity);
	assert(Position != 0);
	assert(Position != 1);

	if(ListOK(List)) return LIST_ERROR;

	if ((List->Size + 2) >= List->Capacity)
		ListResize(List, List->Size * 2);

	if (List->Prev[Position] != PoisonValue) {
		size_t NextFree = List->Next[List->Free];

		List->Data[List->Free] = List->Data[Position];
		List->Next[List->Free] = List->Next[Position];
		List->Prev[List->Free] = Position;

		List->Prev[List->Next[Position]] = List->Free;

		List->Next[Position] = List->Free;
		List->Data[Position] = Value;
		
		List->Free = NextFree;  
	} else {
		return LIST_ERROR;
	}	/*else { // adding to the end of list or what?
		
		if (Position == List->Free) {
			List->Free = List->Next[List->Free];
		}

		List->Data[Position] = Value;
		List->Next[Position] = List->Tail;
		List->Prev[Position] = List->Prev[List->Tail];

		List->Next[List->Prev[List->Tail]] = Position;

		List->Prev[List->Tail] = Position;
	} */ // запретить так делать
	
	List->Size += 1;

	if(ListOK(List)) return LIST_ERROR;


	fprintf(LogFile, "Add Value %d on position %ld\n", Value, Position);
	LIST_DUMP(List);

	return LIST_OK;
}

size_t ListPushFront (List* List, const Elemt Value) 
{
	assert(List != nullptr);

	if(ListOK(List)) return LIST_ERROR;
	
	if ((List->Size + 2) >= List->Capacity)
		ListResize(List, List->Size * 2);

	size_t NextFree = List->Next[List->Free];
	size_t Position = List->Free;

	List->Data[List->Free] = Value;
	List->Next[List->Free] = List->Next[List->Head];
	List->Prev[List->Free] = List->Head;

	List->Next[List->Head] = List->Free;

	List->Prev[List->Next[List->Free]] = List->Free; 

	List->Free = NextFree;

	List->Size += 1;

	if(ListOK(List)) return LIST_ERROR;

	fprintf(LogFile, "Push %d at the front\n", Value);
	LIST_DUMP(List);

	return Position;
}

size_t ListPushBack (List* List, const Elemt Value)
{
	assert(List != nullptr);

	if ((List->Size + 2) >= List->Capacity)
		ListResize(List, List->Size * 2);

	size_t Position = List->Free;
	size_t NextFree = List->Next[List->Free];

	List->Data[List->Free] = Value;
	List->Next[List->Free] = List->Tail;
	List->Prev[List->Free] = List->Prev[List->Tail];

	List->Prev[List->Tail] = List->Free;

	List->Next[List->Prev[List->Free]] = List->Free;

	List->Free = NextFree;

	List->Size += 1; 

	if(ListOK(List)) return LIST_ERROR;

	fprintf(LogFile, "Push %d at the back\n", Value);
	LIST_DUMP(List);

	return Position;
}

size_t ListPopFront (List* List)
{
	assert(List != nullptr);

	if ((List->Size + 2) >= List->Capacity)
		ListResize(List, List->Size * 2);

	size_t Position = List->Next[List->Head];
	size_t Value = List->Data[Position];

	List->Data[Position] = PoisonValue;
	List->Prev[Position] = PoisonValue;

	List->Next[List->Head] = List->Next[Position];

	List->Prev[List->Next[List->Head]] = List->Head;

	List->Next[Position] = List->Free;
	List->Free = Position;

	List->Size--;

	fprintf(LogFile, "Pop %ld at the front\n", Value);
	LIST_DUMP(List);

	return Value;
}

size_t ListPopBack (List* List)
{
	assert(List != nullptr);

	if ((List->Size + 2) >= List->Capacity)
		ListResize(List, List->Size * 2);

	size_t Position = List->Prev[List->Tail];
	size_t Value = List->Data[Position];

	List->Prev[List->Tail] = List->Prev[Position];

	List->Data[Position] = PoisonValue;
	List->Prev[Position] = PoisonValue;

	List->Next[List->Prev[List->Tail]] = List->Tail;

	List->Next[Position] = List->Free;
	List->Free = Position;

	List->Size--;

	fprintf(LogFile, "Pop %ld at the back\n", Value);
	LIST_DUMP(List);

	return Value;
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

	fprintf(LogFile,"Delete element on position %ld\n", Position);
	LIST_DUMP(List);

	return LIST_OK;

}

ListErrors ListResize(List* List, const size_t Size)
{
	assert(List != nullptr);

	if(ListOK(List)) return LIST_ERROR;

	if (Size > (List->Capacity - 2)) {

		Elemt* TemporPtr = (Elemt*) realloc(List->Data, sizeof(Elemt) * (Size + 2));  // auto free memory?
		assert(TemporPtr != nullptr);

		List->Data = TemporPtr;

		int* TempPtr = (int*) realloc(List->Next, sizeof(int) * (Size + 2));
		assert(TempPtr != nullptr);
		
		List->Next = TempPtr;

		TempPtr = (int*) realloc(List->Prev, sizeof(int) * (Size + 2));
		assert(TempPtr != nullptr);

		List->Prev = TempPtr;
		
		for (size_t NumberElement = List->Capacity; NumberElement < (Size + 2); NumberElement++) {
			List->Prev[NumberElement] = PoisonValue;
			List->Data[NumberElement] = PoisonValue;

			List->Next[NumberElement] = List->Free;
			List->Free = NumberElement;	
		}

		List->Capacity = (Size + 2);

	} else if (Size < (List->Capacity - 2)) {
		Elemt TempMassiveData[List->Size + 2] = { };
		int TempMassiveNext[List->Size + 2] = { };
		int TempMassivePrev[List->Size + 2] = { };  // make dynamic

		size_t TotalPosition = 0;

		for (size_t NumberElement = 0; NumberElement < List->Capacity; NumberElement++) {
			if (List->Prev[NumberElement] != PoisonValue) {
				TempMassiveData[TotalPosition] = List->Data[NumberElement];
				TempMassiveNext[TotalPosition] = List->Next[NumberElement];
				TempMassivePrev[TotalPosition] = List->Prev[NumberElement];

				if (NumberElement == List->Next[List->Head]) {
					TempMassiveNext[List->Head] = TotalPosition;
				}
				if (NumberElement == List->Prev[List->Tail]) {
					TempMassivePrev[List->Tail] = TotalPosition;
				}

				TotalPosition++;
			}
		}

		Elemt* TempPtrData = (Elemt*) realloc(List->Data, sizeof(Elemt) * (Size + 2));
		assert(TempPtrData != nullptr);

		int* TempPtrNext = (int*) realloc(List->Next, sizeof(int) * (Size + 2));
		assert(TempPtrNext != nullptr);

		int* TempPtrPrev = (int*) realloc(List->Prev, sizeof(int) * (Size + 2));
		assert(TempPtrPrev != nullptr);

		for (size_t NumberElement = Size + 2; NumberElement < TotalPosition; TotalPosition--) {
			TempMassivePrev[TempMassiveNext[NumberElement]] = TempMassivePrev[NumberElement];
			TempMassiveNext[TempMassivePrev[NumberElement]] = TempMassiveNext[NumberElement];
		}

		for (size_t NumberElement = 0; NumberElement < TotalPosition; NumberElement++) {
			TempPtrData[NumberElement] = TempMassiveData[NumberElement];
			TempPtrNext[NumberElement] = TempMassiveNext[NumberElement];
			TempPtrPrev[NumberElement] = TempMassivePrev[NumberElement];
		}

		List->Data = TempPtrData;
		List->Next = TempPtrNext;
		List->Prev = TempPtrPrev;

		List->Free = TailPosition;
		for (; TotalPosition < (Size + 2); TotalPosition++) {
			List->Prev[TotalPosition] = PoisonValue;
			List->Data[TotalPosition] = PoisonValue;

			List->Next[TotalPosition] = List->Free;
			List->Free = TotalPosition;
		}

		if (List->Size > Size) {
			List->Size = Size;
		}
		List->Capacity = Size + 2;

	} else {
		// strange request
	}

	if(ListOK(List)) return LIST_ERROR;

	fprintf(LogFile, "Resize list to size %ld\n", Size);
	LIST_DUMP(List);

	return LIST_OK;
}

ListErrors ListResize(List* List, const size_t Size, const Elemt Value)
{
	assert(List != nullptr);

	if(ListOK(List)) return LIST_ERROR;

	if (Size > (List->Capacity - 2)) {

		Elemt* TemporPtr = (Elemt*) realloc(List->Data, sizeof(Elemt) * (Size + 2));  // auto free memory?
		assert(TemporPtr != nullptr);

		List->Data = TemporPtr;

		int* TempPtr = (int*) realloc(List->Next, sizeof(int) * (Size + 2));
		assert(TempPtr != nullptr);
		
		List->Next = TempPtr;

		TempPtr = (int*) realloc(List->Prev, sizeof(int) * (Size + 2));
		assert(TempPtr != nullptr);

		List->Prev = TempPtr;
		
		size_t NumberElement = List->Size + 2;
		List->Capacity = (Size + 2); 

		for (; NumberElement < (Size + 2); NumberElement++) {
			
			List->Next[NumberElement] = List->Free;
			List->Free = NumberElement;

			ListPushBack(List, Value);	
		}

		List->Size = Size;

	} else if (Size < (List->Capacity - 2)) {
		Elemt TempMassiveData[List->Size + 2] = { };
		int TempMassiveNext[List->Size + 2] = { };
		int TempMassivePrev[List->Size + 2] = { };  // make dynamic

		size_t TotalPosition = 0;

		for (size_t NumberElement = 0; NumberElement < List->Capacity; NumberElement++) {
			if (List->Prev[NumberElement] != PoisonValue) {
				TempMassiveData[TotalPosition] = List->Data[NumberElement];
				TempMassiveNext[TotalPosition] = List->Next[NumberElement];
				TempMassivePrev[TotalPosition] = List->Prev[NumberElement];

				if (NumberElement == List->Next[List->Head]) {
					TempMassiveNext[List->Head] = TotalPosition;
				}
				if (NumberElement == List->Prev[List->Tail]) {
					TempMassivePrev[List->Tail] = TotalPosition;
				}

				fprintf(LogFile,  "%lu \n", TotalPosition);
				TotalPosition++;
				}
		}

		Elemt* TempPtrData = (Elemt*) realloc(List->Data, sizeof(Elemt) * (Size + 2));
		assert(TempPtrData != nullptr);

		int* TempPtrNext = (int*) realloc(List->Next, sizeof(int) * (Size + 2));
		assert(TempPtrNext != nullptr);

		int* TempPtrPrev = (int*) realloc(List->Prev, sizeof(int) * (Size + 2));
		assert(TempPtrPrev != nullptr);

		for(size_t NumberElement = 0; NumberElement < TotalPosition; NumberElement++) {
			TempPtrData[NumberElement] = TempMassiveData[NumberElement];
			TempPtrNext[NumberElement] = TempMassiveNext[NumberElement];
			TempPtrPrev[NumberElement] = TempMassivePrev[NumberElement];
		}

		List->Data = TempPtrData;
		List->Next = TempPtrNext;
		List->Prev = TempPtrPrev;

		List->Free = TailPosition;
		List->Capacity = Size + 2;
		
		for (; TotalPosition < (Size + 2); TotalPosition++) {
						
			List->Next[TotalPosition] = List->Free;
			List->Free = TotalPosition;

			ListPushBack(List, Value);
		}

		List->Size = Size;

	} else {
		// strange request
	}

	if(ListOK(List)) return LIST_ERROR;

	fprintf(LogFile, "Resize list to size %ld and fill free cell with value %d\n", Size, Value);
	LIST_DUMP(List);

	return LIST_OK;
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

	List->Data = (Elemt*) (Elemt*) calloc(List->Capacity, sizeof(Elemt));
	assert(List->Data != nullptr);

	List->Next = (int*) (int*) calloc(List->Capacity, sizeof(int));
	assert(List->Next != nullptr);

	List->Prev = (int*) (int*) calloc(List->Capacity, sizeof(int));
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

	fprintf(LogFile, "Fill list primary values\n");
	LIST_DUMP(List);

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
	return LIST_OK;

	assert(List != nullptr);
	assert(List->IsCtor == true);  // copypaste in all functions
	assert(List->IsDtor == false);

	if (List->Capacity == 0) ;
	if (List->Size < List->Capacity) ;
	if (List->Data == nullptr);
	if (List->Next == nullptr);
	if (List->Prev == nullptr);

	for (size_t NumberElement = 0; NumberElement < List->Size; NumberElement) {
		List->Prev[List->Next[List->Head]] = List->Head;

	} //
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
	
	fprintf(LogFile, "<pre>\n");

	fprintf(LogFile,  "List[%p] %s from %s(%ld) %s ", List, List->Info.list_name,
	List->Info.birth_file, List->Info.birth_line, List->Info.birth_function);

	fprintf(LogFile,  "called from %s(%ld) %s.\n", NameFile, NLine, Function);

	fprintf(LogFile,  "{\n    Number Elements = %ld\n", List->Size);
	fprintf(LogFile,  "    List Capacity = %ld\n", List->Capacity);
	fprintf(LogFile,  "    List Head = %ld\n", List->Head);
	fprintf(LogFile,  "    List Tail = %ld\n", List->Tail);
	fprintf(LogFile,  "    List Free Element = %ld\n", List->Free);

	fprintf(LogFile,  "    List Data[%p]\n    {\n        ", List->Data);
	for (size_t NumberElement = 0; NumberElement < List->Capacity; NumberElement++) {
		fprintf(LogFile,  "%6ld ", NumberElement);
	}
	fprintf(LogFile,  "\n        ");
	for (size_t NumberElement = 0; NumberElement < List->Capacity; NumberElement++) {
		if (List->Data[NumberElement] != PoisonValue) {
			fprintf(LogFile,  "%6d ", List->Data[NumberElement]);
		} else {
			fprintf(LogFile,  "Poison ");
		}
	}
	fprintf(LogFile,  "\n    }\n");

	fprintf(LogFile,  "    List Next Element[%p]\n    {\n        ", List->Next);
	for (size_t NumberElement = 0; NumberElement < List->Capacity; NumberElement++) {
		fprintf(LogFile,  "%6ld ", NumberElement);
	}
	fprintf(LogFile,  "\n        ");
	for (size_t NumberElement = 0; NumberElement < List->Capacity; NumberElement++) {
		if (List->Next[NumberElement] != PoisonValue) {
			fprintf(LogFile,  "%6d ", List->Next[NumberElement]);
		} else {
			fprintf(LogFile,  "Poison ");
		}
	}  // copypaste
	fprintf(LogFile,  "\n    }\n");

	fprintf(LogFile,  "    List Previous Element[%p]\n    {\n        ", List->Prev);
	for (size_t NumberElement = 0; NumberElement < List->Capacity; NumberElement++) {
		fprintf(LogFile,  "%6ld ", NumberElement);
	}
	fprintf(LogFile,  "\n        ");
	for (size_t NumberElement = 0; NumberElement < List->Capacity; NumberElement++) {
		if (List->Prev[NumberElement] != PoisonValue) {
			fprintf(LogFile,  "%6d ", List->Prev[NumberElement]);
		} else {
			fprintf(LogFile,  "Poison ");
		}
	}
	fprintf(LogFile, "\n    }\n");
	fprintf(LogFile, "}\n "); 

#ifndef NOPNG
	fprintf(LogFile, "\n <img src = List%ld.png width 25%%>\n", 100 + Counter);
	
	FILE* DotFile = fopen(NameDotFile,"w");
	fprintf(DotFile, "digraph G {\n");
	DRAW_LIST_STRUCT();
	DRAW_HEAD();
	DRAW_TAIL();

	for (size_t NumberElement = 2; NumberElement < List->Capacity; NumberElement++) {
		if (List->Prev[NumberElement] == PoisonValue) {
			DRAW_FREE_ELEMENT(NumberElement);
		}
		else {
			DRAW_LIST_ELEMENT(NumberElement);
		}
	}
	fprintf(DotFile, "}");

	fclose(DotFile);
	
	char GeneratePng[49] = "dot -T png ListDot.dot -o DotPng/List100.png";

	GeneratePng[39] = '0' + Counter % 10;
	if (Counter > 9) {
		GeneratePng[38] = '0' + Counter / 10;}

	system(GeneratePng);
	Counter++;
#endif

	return LIST_OK;
}
