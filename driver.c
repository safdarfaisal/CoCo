#include <stdio.h>
#include "astgen.h"
#include "symboltable.h"
#include "codegen.h"

/*

    The main driver function - use this to run the program.
        1. Provides the tokenized file as <filename>.tkn
        else just exits. Writes errors as TK_ERROR (No error handling yet)
*/
int main(){
    AstTreeNode *node = astgen_run();
    SymbolTable *symTable = createSymbolTable(10);
    RecordTable *recTable = createRecordTable(10);
    run(node, symTable, recTable);
    codegen_run("test.nasm", symTable, node);
    return 0;
}
