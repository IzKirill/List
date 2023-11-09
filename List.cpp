#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "DotList.h"
#include "List.h"

#define REALLOC_LIST(List, Size) ReallocList((List), (Size), __LINE__,   \
						__FILE__,__PRETTY_FUNCTION__)

FILE* LogFile = 0;

static void EndProgramm(void);

static ListErrors ReallocList(List* List, const size_t Size, 
				const size_t Line, const char* File, const char* Function);
static ListErrors ListResizeDown (List* List, const size_t Size, const Elemt Value);

static size_t Counter = 1;

static void EndProgramm(void) 
{
	fprintf(LogFile,  "<font color=\"green\">List log file successfully closed</font>\n");
	fclose(LogFile);	
}

ListErrors ListInsert(List* List, const Elemt Value, const size_t Position)
{
	ListErrors Error = CHECK_LIST_AND_POSITION(List, Position);
	if (Error) return Error;

	if ((List->Size + 2) == List->Capacity) {
		ListResize(List, List->Size * 2);
	}

	size_t NextFree = List->Next[List->Free];

	List->Data[List->Free] = List->Data[Position];
	List->Next[List->Free] = List->Next[Position];
	List->Prev[List->Free] = Position;

	List->Prev[List->Next[Position]] = List->Free;

	List->Next[Position] = List->Free;
	List->Data[Position] = Value;
		
	List->Free = NextFree;  
	
	List->Size += 1;

	Error = LIST_OK(List);
	if (Error) return Error;

	fprintf(LogFile, "Add Value %d on position %ld\n", Value, Position);
	LIST_DUMP(List);

	return LIST_OK;
}

ListErrors ListInsert(List* List, const size_t Position, 
					  const size_t CountElements, const size_t Value)
{
	ListErrors Error = CHECK_LIST_AND_POSITION(List, Position);
	if (Error) return Error;

	for (size_t NumberElements = CountElements; NumberElements > 0; NumberElements--) {
		ListInsert(List, Value, Position);
	}

	Error = LIST_OK(List);
	if (Error) return Error;

	fprintf(LogFile, "Add %ld elements with value %ld from position %ld\n", CountElements, Value, Position);
	LIST_DUMP(List);

	return LIST_OK;
}

size_t ListPushFront (List* List, const Elemt Value) 
{
	ListErrors Error = LIST_OK(List);
	if (Error) return Error;
		
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

	Error = LIST_OK(List);
	if (Error) return Error;

	fprintf(LogFile, "Push %d at the front\n", Value);
	LIST_DUMP(List);

	return Position;
}

size_t ListPushBack (List* List, const Elemt Value)
{
	ListErrors Error = LIST_OK(List);
	if (Error) return Error;

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

	Error = LIST_OK(List);
	if (Error) return Error;

	fprintf(LogFile, "Push %d at the back\n", Value);
	LIST_DUMP(List);

	return Position;
}

size_t ListPopFront (List* List)
{
	ListErrors Error = LIST_OK(List);
	if (Error) return Error;

	Elemt Value = List->Data[List->Next[List->Head]];

	fprintf(LogFile, "Pop %d at the front\n", Value);
	ListErase(List, List->Next[List->Head]);

	Error = LIST_OK(List);
	if (Error) return Error;

	return Value;
}

size_t ListPopBack (List* List)
{
	ListErrors Error = LIST_OK(List);
	if (Error) return Error;

	Elemt Value = List->Data[List->Prev[List->Tail]];

	fprintf(LogFile, "Pop %d at the back\n", Value);	
	ListErase(List, List->Prev[List->Tail]);

	Error = LIST_OK(List);
	if (Error) return Error;

	return Value;
}

size_t ListErase(List* List, const size_t Position)  
{
	ListErrors Error = CHECK_LIST_AND_POSITION(List, Position);
	if(Error) return Error;

	size_t NextElement = List->Next[Position];

	List->Next[List->Prev[Position]] = List->Next[Position];
	List->Prev[List->Next[Position]] = List->Prev[Position];

	List->Data[Position] = PoisonValue;
	List->Prev[Position] = PoisonValue;

	List->Next[Position] = List->Free;
	List->Free = Position;
	 
	List->Size -= 1;

	Error = LIST_OK(List);
	if (Error) return Error;

	fprintf(LogFile,"Delete element on position %ld\n", Position);
	LIST_DUMP(List);

	return NextElement;
}

ListErrors ListErase(List* List, const size_t Begin, const size_t End)
{
	ListErrors Error1 = CHECK_LIST_AND_POSITION(List, Begin);
	ListErrors Error2 = CHECK_LIST_AND_POSITION(List, End);
	if(Error1 || Error2) return (Error1 != LIST_OK) ? Error1 : Error2;

	size_t Index = Begin;
	while (Index < End) {
		if (List->Prev[Index] != PoisonValue) {
			ListErase(List, Index);
		}
		Index++;
	}	

	Error1 = LIST_OK(List);
	if (Error1) return Error1;

	fprintf(LogFile,"Delete element from position %ld to position %ld\n", Begin, End);
	LIST_DUMP(List);

	return LIST_OK;
}

static ListErrors ReallocList (List* List, const size_t Size, 
				const size_t Line, const char* File, const char* Function)
{
	Elemt* TempPtr = (Elemt*) calloc((Size + 2) * (2 * sizeof(int) + sizeof(Elemt)), sizeof(char));  

	if (TempPtr == nullptr) {
		PRINT_ERROR("Not enough memory on your pc")
		return NOT_ENOUGH_MEMORY;
	}

	int* PastNext = List->Next;
	int* PastPrev = List->Prev;

	List->Next = (int*) ((char*) TempPtr + sizeof(Elemt) * (Size + 2));
	List->Prev = (int*) ((char*) List->Next + sizeof(int) * (Size + 2));;

	memmove(TempPtr, List->Data, List->Capacity * sizeof(Elemt));
	memmove(List->Next, PastNext, List->Capacity * sizeof(int));
	memmove(List->Prev, PastPrev, List->Capacity * sizeof(int));

	free(List->Data);

	List->Data = TempPtr;

	return LIST_OK;
}

ListErrors ListResize(List* List, const size_t Size)
{
	ListErrors Error = LIST_OK(List);
	if (Error) return Error;

	if (Size > (List->Capacity - 2)) {

		if (REALLOC_LIST(List, Size) != LIST_OK) {
			return NOT_ENOUGH_MEMORY;
		}

		for (size_t NumberElement = List->Capacity; NumberElement < (Size + 2); NumberElement++) {
			List->Prev[NumberElement] = PoisonValue;
			List->Data[NumberElement] = PoisonValue;

			List->Next[NumberElement] = List->Free;
			List->Free = NumberElement;	
		}

		List->Capacity = (Size + 2);

	} else if (Size < (List->Capacity - 2)) {
		if (ListResizeDown(List, Size, PoisonValue) == NOT_ENOUGH_MEMORY) {
			return NOT_ENOUGH_MEMORY;
		}
	} 
		
	Error = LIST_OK(List);
	if (Error) return Error;

	fprintf(LogFile, "Resize list to size %ld\n", Size);
	LIST_DUMP(List);

	return LIST_OK;
}

ListErrors ListResize(List* List, const size_t Size, const Elemt Value)
{
	ListErrors Error = LIST_OK(List);
	if (Error) return Error;

	if (Size > (List->Capacity - 2)) {

		if (REALLOC_LIST(List, Size) != LIST_OK) {
			return NOT_ENOUGH_MEMORY;
		}

		for (size_t NumberElement = List->Size + 2; NumberElement < (Size + 2); NumberElement++) {
			if (NumberElement == List->Capacity) {
				List->Capacity++;
				List->Next[NumberElement] = List->Free;
				List->Free = NumberElement;
			}

			ListPushBack(List, Value);	
		}

		List->Size = Size;

	} else if (Size < (List->Capacity - 2)) {
		if (ListResizeDown(List, Size, Value) == NOT_ENOUGH_MEMORY) {
			return NOT_ENOUGH_MEMORY;
		}
	} 

	Error = LIST_OK(List);
	if (Error) return Error;

	fprintf(LogFile, "Resize list to size %ld and fill free cell with value %d\n", Size, Value);
	LIST_DUMP(List);

	return LIST_OK;
}

ListErrors ListResizeDown (List* List, const size_t Size, const Elemt Value)
{
	Elemt* TempMassiveData = (Elemt*) calloc((List->Size + 2) * (sizeof(Elemt) + sizeof(int) * 2), sizeof(char));
	CHECKCONDITION(TempMassiveData == nullptr, NOT_ENOUGH_MEMORY, "Not enough memory on your pc");

	int* TempMassiveNext = (int*) ((char*) TempMassiveData + sizeof(Elemt) * (List->Size + 2));
	int* TempMassivePrev = (TempMassiveNext + List->Size + 2);

	while (List->Size > Size) {
		ListPopBack(List);
	}

	TempMassiveData[HeadPosition] = PoisonValue;
	TempMassivePrev[HeadPosition] = PrevHeadIndicator;
	TempMassiveData[TailPosition] = PoisonValue;
	TempMassiveNext[TailPosition] = NextTailIndicator;
	if (Size > 0) {
		TempMassiveNext[HeadPosition] = 2;
		TempMassivePrev[TailPosition] = List->Size + 1;
	} else {
		TempMassiveNext[HeadPosition] = 1;
		TempMassivePrev[TailPosition] = 0;
	}
	
	size_t TotalPosition = 2;
	size_t PrevIndex = 0;
	size_t ListIndex = List->Next[List->Head];
		
	for (; TotalPosition < (List->Size + 2); TotalPosition++) {
		TempMassiveData[TotalPosition] = List->Data[ListIndex];
		ListIndex = List->Next[ListIndex];
	
		if (TotalPosition == (List->Size + 1)) {
			TempMassiveNext[TotalPosition] = TailPosition;
		} else {
			TempMassiveNext[TotalPosition] = TotalPosition + 1;
		}
		TempMassivePrev[TotalPosition] = PrevIndex;

		PrevIndex = TotalPosition;
	}

	Elemt* TempPtr = (Elemt*) calloc((Size + 2) * (2 * sizeof(int) + sizeof(Elemt)), sizeof(char));  
	CHECKCONDITION(TempPtr == nullptr, NOT_ENOUGH_MEMORY, "Not enough memory on your pc");

	List->Next = (int*) ((char*) TempPtr + sizeof(Elemt) * (Size + 2));
	List->Prev = (int*) ((char*) List->Next + sizeof(int) * (Size + 2));

	memmove(TempPtr, TempMassiveData, TotalPosition * sizeof(Elemt));
	memmove(List->Next, TempMassiveNext, TotalPosition * sizeof(int));
	memmove(List->Prev, TempMassivePrev, TotalPosition * sizeof(int));
	
	free(TempMassiveData);
	free(List->Data);
	List->Data = TempPtr;

	List->Free = TailPosition;

	if (List->Size > Size) {
		List->Size = Size;
	}

	List->Capacity = Size + 2;

	for (; TotalPosition < (Size + 2); TotalPosition++) {
		List->Prev[TotalPosition] = PoisonValue;
		List->Data[TotalPosition] = PoisonValue;
		
		List->Next[TotalPosition] = List->Free;
		List->Free = TotalPosition;
		if (Value != PoisonValue) ListPushBack(List, Value);
	}

	return LIST_OK;
}

size_t ListBegin(List* List)
{
	ListErrors Error = LIST_OK(List);
	if (Error) return Error;

	return List->Next[List->Head];
}

size_t ListEnd(List* List)
{
	ListErrors Error = LIST_OK(List);
	if (Error) return Error;

	return List->Prev[List->Tail];
}

bool ListEmpty(List* List) 
{
	ListErrors Error = LIST_OK(List);
	if (Error) return Error;

	return List->Size;
}

size_t ListSize(List* List)
{
	ListErrors Error = LIST_OK(List);
	if (Error) return Error;

	return List->Size;
}

size_t ListCapacity(List* List)
{
	ListErrors Error = LIST_OK(List);
	if (Error) return Error;

	return List->Capacity;
}

Elemt ListFront(List* List)
{
	ListErrors Error = LIST_OK(List);
	if (Error) return Error;

	return List->Data[List->Next[List->Head]];
}

Elemt ListBack(List* List)
{
	ListErrors Error = LIST_OK(List);
	if (Error) return Error;

	return List->Data[List->Prev[List->Tail]];
}

ListErrors ListClear(List* List)
{
	ListErrors Error = LIST_OK(List);
	if (Error) return Error;

	ListResize(List, 0);

	return LIST_OK;
}

ListErrors ListCtor(List* List, size_t Capacity, const char* list_name,
				    const size_t birth_line, const char* birth_file, const char* birth_function) 
{    
	CHECKCONDITION(List == nullptr, LIST_NULLPTR, "List is null ptr");
 
	LogFile = fopen(NameLogFile, "w");
	CHECKCONDITION(LogFile == nullptr, CANT_CREATE_LOGS, "Can not create log file");
	atexit(EndProgramm);
	
	List->IsCtor = true;
	List->IsDtor = false;
	
	List->Info.list_name = list_name;
	List->Info.birth_function = birth_function;
	List->Info.birth_line = birth_line;
	List->Info.birth_file = birth_file;

	List->Capacity = Capacity + 2;

	List->Data = (Elemt*) calloc(List->Capacity * (2 * sizeof(int) + sizeof(Elemt)), sizeof(char));
	CHECKCONDITION(List->Data == nullptr, NOT_ENOUGH_MEMORY, "Not enough memory on your pc");
	
	List->Next = (int*) ((char*) List->Data + sizeof(Elemt) * List->Capacity); 
	List->Prev = (int*) ((char*) List->Next + sizeof(int) * List->Capacity);

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

	ListErrors Error = LIST_OK(List);
	if (Error) return Error;

	fprintf(LogFile, "Fill list primary values\n");
	LIST_DUMP(List);

	return LIST_OK;
}

ListErrors ListDtor(List* List)
{
	ListErrors Error = LIST_OK(List);
	if (Error) return Error;

	List->IsDtor = true;

	for (size_t NumberElement = 0; NumberElement < List->Capacity; NumberElement++) {
		List->Data[NumberElement] = PoisonValue;
		List->Next[NumberElement] = PoisonValue;
		List->Prev[NumberElement] = PoisonValue;
	}

	free(List->Data);
	
	List->Capacity = PoisonValue;
	List->Free = PoisonValue;
	List->Head = PoisonValue;
	List->Tail = PoisonValue;
	List->Size = PoisonValue;
	
	return LIST_OK;
}

ListErrors ListOK(List* List, const size_t Line, 
				  const char* File, const char* Function)
{
	if (LogFile == nullptr) {
		LogFile = fopen(NameLogFile, "w");		
	}
	if (List == nullptr) {
		PRINT_ERROR("List is null ptr")
		return LIST_NULLPTR;
	}
	if (List->IsCtor == false) {
		PRINT_ERROR("List have not created yet")
		return LIST_NOT_CTOR;
	}
	if (List->IsDtor == true) {
		PRINT_ERROR("This list have already deleted")
		return LIST_IS_DTOR;
	}
	if (List->Capacity == 0) {
		PRINT_ERROR("List capacity equal zero")
		List->ErrorsInfo |= NULL_CAPACITY;
	}
	if (List->Size > List->Capacity) {
		PRINT_ERROR("List size > list capacity")
		List->ErrorsInfo |= SIZE_GREATER_CAPACITY;
	}
	if (List->Data == nullptr) {
		PRINT_ERROR("List data is null ptr")
		List->ErrorsInfo |= NULL_DATA;
	}
	if (List->Next == nullptr) {
		PRINT_ERROR("List next is null ptr")
		List->ErrorsInfo |= NULL_NEXT;
	}
	if (List->Prev == nullptr) {
		PRINT_ERROR("List prev is null ptr")
		List->ErrorsInfo |= NULL_PREV;
	}
	if (List->Head != HeadPosition) {
		PRINT_ERROR("Position of list head changed")
		List->ErrorsInfo |= INCOR_HEAD_POS;
	}
	if (List->Tail != TailPosition) {
		PRINT_ERROR("Position of list tail changed")
		List->ErrorsInfo |= INCOR_TAIL_POS;
	}
	if (List->Free >= List->Capacity) {
		PRINT_ERROR("Incorrect position of list free")
		List->ErrorsInfo |= INCOR_FREE_POS;
	}								

	if (List->ErrorsInfo != 0) {
		return LIST_ERROR;
	} else {
		return LIST_OK;	
	}
}

ListErrors CheckListAndPosition(List* List, size_t Position, const size_t Line, 
				  			 const char* File, const char* Function)
{
	ListErrors ListErr = ListOK(List, Line, File, Function);
	if (ListErr != LIST_ERROR && ListErr != LIST_OK) {
		return ListErr;
	} 

	if (Position == 1) {
		PRINT_ERROR("You can not do something with the list tail")
		List->ErrorsInfo |= POS_EQUALS_TAIL;
	}
	if (Position == 0) {
		PRINT_ERROR("You can not do something with the list head")
		List->ErrorsInfo |= POS_EQUALS_HEAD;
	}
	if (Position >= List->Capacity) {
		PRINT_ERROR("Position is greater than the list capacity")
		List->ErrorsInfo |= POS_GREATER_CAPACITY;
	}
	if (List->Prev != nullptr && List->Prev[Position] == PoisonValue) {
		PRINT_ERROR("Element on this position is not a list element")
		List->ErrorsInfo |= ELEMENT_ON_POS_NOT_EXIST;
	}

	if (List->ErrorsInfo != 0) {
		return LIST_ERROR;
	} else {
		return LIST_OK;	
	}
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

	fprintf(LogFile, "<font color=\"844EFF\">");
	fprintf(LogFile,  "List[%p] %s from %s(%ld) %s ", List, List->Info.list_name,
	List->Info.birth_file, List->Info.birth_line, List->Info.birth_function);

	fprintf(LogFile,  "called from %s(%ld) %s.\n", NameFile, NLine, Function);

	fprintf(LogFile,  "{\n    Number Elements = %ld\n", List->Size);
	fprintf(LogFile,  "    List Capacity = %ld\n", List->Capacity);
	fprintf(LogFile,  "    List Head = %ld\n", List->Head);
	fprintf(LogFile,  "    List Tail = %ld\n", List->Tail);
	fprintf(LogFile,  "    List Free Element = %ld\n", List->Free);

	fprintf(LogFile,  "    List Data[%p]\n    {\n        ", List->Data);
	DUMP_PRINT_MASSIVE(List->Data);

	fprintf(LogFile,  "    List Next Element[%p]\n    {\n        ", List->Next);
	DUMP_PRINT_MASSIVE(List->Next);

	fprintf(LogFile,  "    List Previous Element[%p]\n    {\n        ", List->Prev);
	DUMP_PRINT_MASSIVE(List->Prev);

	fprintf(LogFile, "}\n </font>"); 

#ifndef NO_PNG
	fprintf(LogFile, "\n <img src = DotPng/List%ld.png width 25%%>\n", 100 + Counter);
	
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
		GeneratePng[38] = '0' + Counter / 10;
	}
	if (Counter > 99) {
		GeneratePng[37] = '0' + Counter / 100;
	}

	system(GeneratePng);
	Counter++;
#endif

	return LIST_OK;
}



