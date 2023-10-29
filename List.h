#ifndef __LIST_H__
#define __LIST_H__

#define LIST_CTOR(List, Capacity) ListCtor((List), (Capacity), #List, \
                                  __LINE__,__FILE__,__PRETTY_FUNCTION__)

const char NameLogFile = "ListLogs.txt";

const int PoisonValue = 2399229923;

typedef int Elemt;

struct BirthInfo {
    const char* list_name;  
    const char* birth_function;
    size_t birth_line;
    const char* birth_file;
}; 

struct List {
	Elemt* Data;
	int* NextElement;
	int* PrevElement;
	size_t Head;
	size_t Tail;
	size_t NumberElements;
	size_t Capacite;
	size_t Tail;                       
	size_t Capacity;
	size_t FreeElement;
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
               	    const size_t birth_line, const char* birth_file, const char* birth_function);
ListErrors ListDtor(List* List);
ListErrors ListOK(List* List);
ListErrors ListDump(List* List);

#endif