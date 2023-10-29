#include <stdio.h>
#include <malloc.h>
#include <assert.h>

FILE* LogFile = 0;

ListErrors ListCtor(List* List, size_t Capacity, const char* list_name,
				    const size_t birth_line, const char* birth_file, const char* birth_function) 
{    
	assert(List != nullptr);
	assert(Capacity > 0);

	LogFile = fopen(NameLogFile, "w");
	
	List->IsCtor = true;
	List->IsDtor = false;
	
	List->Info.list_name = list_name;
	List->Info.birth_function = birth_function;
	List->Info.birth_line = birth_line;
	List->Info.birth_file = birth_file;

	List->Capacity = Capacity + 2;

	List->Data = (Elemt*) calloc(List->Capacity; sizeof(Elemt));
	assert(List->Data != nullptr);

	List->NextElement = (int*) calloc(List->Capacity; sizeof(int));
	assert(List->NextElement != nullptr);

	List->PrevElement = (int*) calloc(List->Capacity; sizeof(int));
	assert(List->PrevElement != nullptr);

	for (size_t NumberElement = 0; NumberElement < List->Capacity; NumberElement++) {
		List->Data[NumberElement] = PoisonValue;
		List->NextElement[NumberElement] = NumberElement + 1;
		List->PrevElement[NumberElement] = PoisonValue;
	}

	List->Head = 0;	
	List->Tail = 1;
	List->NextElement[List->Head] = List->Tail;
	List->PrevElement[List->Head] = -2;
	List->NextElement[List->Tail] = -2;
	List->PrevElement[List->Tail] = List->Head;

	List->FreeElement = 2;
	List->NumberElements = 0;

	List->ErrorsInfo = 0;

	if (ListOK(List)) return ERROR;
}

ListErrors ListDtor(List* List)
{
	assert(List != nullptr);
	assert(IsCtor == true);
	assert(IsDtor == false);

	if (ListOK(List)) return ERROR;

	List->IsDtor = true;

	for (size_t NumberElement = 0; NumberElement < List->Capacity; NumberElement++) {
		List->Data[NumberElement] = PoisonValue;
		List->NextElement[NumberElement] = PoisonValue;
		List->PrevElement[NumberElement] = PoisonValue;
	}
	free(List->Data);
	free(List->NextElement);
	free(List->PrevElement);
	List->Capacity = PoisonValue;
	List->FreeElement = PoisonValue;
	List->Head = PoisonValue;
	List->Tail = PoisonValue;
	List->NumberElements = PoisonValue;

}







