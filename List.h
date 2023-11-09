#ifndef __LIST_H__
#define __LIST_H__

#include <cstddef>

#define LIST_CTOR(List, Capacity) ListCtor((List), (Capacity), #List, \
                                  __LINE__,__FILE__,__PRETTY_FUNCTION__)

// #define NO_PNG - Dump without png

#define LIST_OK(List) ListOK((List), __LINE__, \
						     __FILE__,__PRETTY_FUNCTION__)

#define CHECK_LIST_AND_POSITION(List, Position) CheckListAndPosition((List),    \
					(Position), __LINE__, __FILE__,__PRETTY_FUNCTION__)					 

#define PRINT_ERROR(ErrorMessage)   \
fprintf(LogFile, "<font color=\"red\">ERROR:</font> " ErrorMessage " in %s(%ld) %s" "\n",   \
		File, Line, Function);

#define DUMP_PRINT_MASSIVE(Massive) do                                  \
{  \
	for (size_t NumberElement = 0; NumberElement < List->Capacity; NumberElement++) {  \
		fprintf(LogFile,  "%6ld ", NumberElement);										\
	} 																				    \
	fprintf(LogFile,  "\n        ");													\
	for (size_t NumberElement = 0; NumberElement < List->Capacity; NumberElement++) {  \
		if (Massive[NumberElement] != PoisonValue) {								   \
			fprintf(LogFile,  "%6d ", Massive[NumberElement]);						\
		} else {																		\
			fprintf(LogFile,  "Poison ");												\
		}																				\
	}																					\
	fprintf(LogFile, "\n    }\n");														\
} while (0);																			

#define CHECKCONDITION(Condition, ErrorCode, ErrorMessage) do {              \
                                                            if (Condition)  { \
                                                                if (LogFile == NULL) \
                                                                    LogFile = fopen(NameLogFile, "w"); \
                                                                fprintf(LogFile,"<font color=\"red\">ERROR:</font> %s in %s(%d) %s" "\n",  \
																	ErrorMessage, __FILE__, __LINE__,__PRETTY_FUNCTION__); \
                                                                return ErrorCode; \
                                                            }    }  while(0) 															

#ifndef RELEASE
	#define LIST_DUMP(List) ListDump(List,__LINE__,__FILE__,__PRETTY_FUNCTION__)
#else
	#define LIST_DUMP(List)
#endif

const char NameLogFile[] = "ListLogs.htm";
const char NameDotFile[] = "ListDot.dot";

const int PoisonValue = 2399229923;

const size_t HeadPosition = 0;
const size_t TailPosition = 1;
const int PrevHeadIndicator = -2;
const int NextTailIndicator = -2;

typedef int Elemt;

struct BirthInfo {
    const char* list_name;  
    const char* birth_function;
    size_t birth_line;
    const char* birth_file;
}; 

struct List {
	Elemt* Data;
	int* Next;
	int* Prev;    
	size_t Head;
	size_t Tail;                       
	size_t Size;     
	size_t Capacity;
	size_t Free;
	short ErrorsInfo;
	bool IsCtor;
	bool IsDtor;	
	BirthInfo Info;	
};

enum ListErrors {
	LIST_OK = 0,
	LIST_ERROR = -1,

	NULL_CAPACITY = (1 << 0),
	SIZE_GREATER_CAPACITY = (1 << 1),
	NULL_DATA = (1 << 2),
	NULL_NEXT = (1 << 3),
	NULL_PREV = (1 << 4),
	INCOR_HEAD_POS = (1 << 5),
	INCOR_TAIL_POS = (1 << 6),
	INCOR_FREE_POS = (1 << 7),
	POS_EQUALS_HEAD = (1 << 8),
	POS_EQUALS_TAIL = (1 << 9),
	POS_GREATER_CAPACITY = (1 << 10),
	ELEMENT_ON_POS_NOT_EXIST = (1 << 11),

	LIST_NULLPTR = 5000,
	LIST_NOT_CTOR = 5001,
	LIST_IS_DTOR = 5002,
	NOT_ENOUGH_MEMORY = 5003,
	CANT_CREATE_LOGS = 5004
};

ListErrors ListCtor(List* List, size_t Capacity, const char* list_name,
               	    const size_t birth_line, const char* birth_file,
				    const char* birth_function);
ListErrors ListDtor(List* List);

ListErrors ListDump(List* List,const size_t NLine, 
					const char* NameFile, const char* Function);

ListErrors ListOK(List* List, const size_t Line, 
				  const char* File, const char* Function);
ListErrors CheckListAndPosition(List* List, size_t Position, const size_t Line, 
				  			 const char* File, const char* Function);

ListErrors ListInsert(List* List, const Elemt Value, const size_t Position);
ListErrors ListInsert(List* List, const size_t Position, 
					  const size_t CountElements, const size_t Value);
//ListErrors ListInsert(List* List1, const size_t Position, List* List2);

size_t ListPushFront(List* List, const Elemt Value); 
size_t ListPushBack(List* List, const Elemt Value); 

size_t ListErase(List* List, const size_t Position); 
ListErrors ListErase(List* List, const size_t Begin, const size_t End);

size_t ListPopFront(List* List); 
size_t ListPopBack(List* List); 

ListErrors ListResize(List* List, const size_t Size, const Elemt Value);
ListErrors ListResize(List* List, const size_t Size); 

size_t ListBegin(List* List);
size_t ListEnd(List* List);

bool ListEmpty(List* List);

size_t ListSize(List* List);
size_t ListCapacity(List* List);

Elemt ListFront(List* List);
Elemt ListBack(List* List);

ListErrors ListClear(List* List); 

#endif



