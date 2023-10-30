#ifndef __LIST_H__
#define __LIST_H__

#include <cstddef>

#define LIST_CTOR(List, Capacity) ListCtor((List), (Capacity), #List, \
                                  __LINE__,__FILE__,__PRETTY_FUNCTION__)

#define LIST_DUMP(List) ListDump(List,__LINE__,__FILE__,__PRETTY_FUNCTION__)

const char NameLogFile[] = "ListLogs.txt";

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
	int* Prev;    // make size_t ?
	size_t Head;
	size_t Tail;                       
	size_t Size;     // нужно ли выравнивать?
	size_t Capacity;
	size_t Free;
	char ErrorsInfo;
	bool IsCtor;
	bool IsDtor;	
	BirthInfo Info;	
};

enum ListErrors {
	LIST_OK = 0,
	LIST_ERROR = -1
};

ListErrors ListCtor(List* List, size_t Capacity, const char* list_name,
               	    const size_t birth_line, const char* birth_file,
				    const char* birth_function);
ListErrors ListDtor(List* List);
ListErrors ListDump(List* List,const size_t NLine, 
					const char* NameFile, const char* Function);

#endif