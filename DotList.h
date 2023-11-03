#ifndef _DOTLIST_H__
#define _DOTLIST_H__

#define DRAW_LIST_STRUCT() do { \
fprintf(DotFile,"STRUCT[shape = \"rectangle\"style = \"filled\", fillcolor = \"lightgrey\",];\n"); \
fprintf(DotFile, "STRUCTLIST[shape = \"record\", style=\"filled\", fillcolor=\"#e5ff15\",  \
label=\"{ name: %s | size: %ld | <f0>capacity: %ld | <H>head: %ld | <T>tail: %ld | <F>free: %ld}\"]\n", \
List->Info.list_name, List->Size, List->Capacity, List->Head, List->Tail, List->Free); \
fprintf(DotFile, "STRUCT->STRUCTLIST[color = white];"); \
fprintf(DotFile, "STRUCTLIST:<T> -> ELEMENT1[color = \"#e06ffb\"];\n"); \
fprintf(DotFile, "STRUCTLIST:<H> -> ELEMENT0[color = \"#62c3ff\"];\n");  \
fprintf(DotFile, "STRUCTLIST:<F> -> ELEMENT%ld[color = black];\n", List->Free); \
} while(0);  

#define DRAW_HEAD() do { \
fprintf(DotFile, "ELEMENT0[shape = \"record\", style=\"filled\", fillcolor=\"#62c3ff\", label = \"{ HEAD | <N0>nx: %d| <P0>pr: %d}\"]			\
\nSTRUCTLIST->ELEMENT0[color = white];\n", List->Next[List->Head], List->Prev[List->Head]);									\
fprintf(DotFile, "ELEMENT0:<N0> -> ELEMENT%d:<N%d>[color = blue];\n", List->Next[List->Head], List->Next[List->Head]);				\
} while(0);

#define DRAW_TAIL() do { \
fprintf(DotFile, "    ELEMENT1[shape = \"record\", style=\"filled\", fillcolor=\"#e06ffb\", label = \"{ TAIL | <N1>nx: %d | <P1>pr: %d}\"]   \
\nELEMENT0 -> ELEMENT1[color = white];\n", List->Next[List->Tail], List->Prev[List->Tail]);															\
fprintf(DotFile, " ELEMENT1:<P1> -> ELEMENT%d:<P%d>[color=red];\n", List->Prev[List->Tail], List->Prev[List->Tail]); \
} while(0);

#define DRAW_FREE_ELEMENT(Position) do { \
fprintf(DotFile, "ELEMENT%ld[shape = \"record\", style=\"filled\", fillcolor=\"#ff1b1b\", label = \"{ FREE | <N%ld>nx: %d | pr: PSN}\"]   \
\nELEMENT%ld -> ELEMENT%ld[color=white];\n", Position, Position, List->Next[Position], Position - 1, Position);				\
fprintf(DotFile, "ELEMENT%ld:<N%ld> -> ELEMENT%d:<N%d>[color=lightgreen, style=\"dashed\"];\n",  \
Position, Position, List->Next[Position], List->Next[Position]); 		\
} while (0); 

#define DRAW_LIST_ELEMENT(Position) do { \
fprintf(DotFile, "ELEMENT%ld[shape = \"record\", style=\"filled\", fillcolor=\"#8ef482\", label = \"{ ID: %ld | vl: %d |<N%ld>nx: %d | <P%ld>pr: %d}\"]    \
\nELEMENT%ld -> ELEMENT%ld[color=white];\n", Position, Position, List->Data[Position], Position, List->Next[Position], Position, List->Prev[Position], Position - 1, Position);  				\
fprintf(DotFile, "ELEMENT%ld:<N%ld> -> ELEMENT%d:<N%d>[color = blue];\n", Position, Position, List->Next[Position], List->Next[Position]); \
fprintf(DotFile, "ELEMENT%ld:<P%ld> -> ELEMENT%d:<P%d>[color=red];\n", Position, Position, List->Prev[Position], List->Prev[Position]); \
} while(0);

#endif