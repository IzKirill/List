#ifndef __LIST_H__
#define __LIST_H__

const char LogFileName = "ListLogs.txt";

typedef int Elemt

struct BirthInfo {
    const char* ListName;  
    const char* BirthFunction;
    size_t BirthLine;
    const char* BirthFile;
}; 

struct List {
	Elemt* Data;
	Elemt* NextElement;
	Elemt* PrevElement;
	size_t Head;
	size_t Tail;
	size_t NumberElements;
	size_t Capacite;
	size_t FreeElement;
	bool IsCtor;
	bool IsDtor;
	BirthInfo ListInfo;	
};

enum ListErrors {
	LIST_OK = 0
};

ListErrors ListCtor();
ListErrors ListDtor();
ListErrors ListOK();
ListErrors ListDump();




#endif