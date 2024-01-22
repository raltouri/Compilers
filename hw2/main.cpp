/**************************************
 *
 * University of Haifa
 *
 * Theory of compilation
 *
 * P machine compiler - code generation project
 *
 * Yosi Ben Asher
 * Email: yosibenasher@gmail.com
 *
 * Mariah Akree
 * Email: mariah.uni.haifa@gmail.com
 *
 **************************************/

#include "main.h"

extern int yydebug;

int startAddress=5;

int codelr=0;

int rightSon=0;

int rightSonPremitive=0;

int lastOp=0;

int iflag=0;

int loopflag=0;

int switchflag=0;

int caseflag=0;

int arrayRefFlag=0;

int currentDim=0;

int varDecFlag=0;

string lastVar="";

string lastType="";

/// for record
int recRefFlag=0;
int recTypeFlag=0;
int recRefLeftFlag=0;
int recRefRightFlag=0;
map <string, recInfo> recsInfo;

map <string, Variable> recFatherST;
map <string, arrInfo> recFatherarrsInfo;
///

/// for adressType
int addressTypeFlag=0;
int addressRefFlag=0;
int DecaddressTypeFlag=0;
map <string, Variable> addressST;
///

stack<int> currentSwitch;

map<string,Variable> ST;

map<string,arrInfo> arrsInfo;


void writeAST(Object* r, fstream& file)
{
	//Perform recursive tree print
	r->print(file);
}
void Pcode(Object* r, fstream& file)
{
	//Perform recursive tree print
	r->pcodegen(file);
}

Object* getTree(char* progFile)
{

	yydebug = 1;
	assert(progFile);

	// yyin is an external variable that been used in yyparse as pointer to the source file.
	yyin = fopen(progFile, "r");
	if (!yyin) {
		cerr << "Error: file " << progFile << " does not exst. Aborting ..." << endl;;
		exit(1);
	}
	assert(yyin);

	// yyparse is yacc function that parse the program, checks syntax and builds the program AST.
	yyparse();

	fclose(yyin);

	// root was initialized in yyparse while it was building the tree.
	// root is the pointer of the returning tree.
	assert(root);
	return(root);
}



int main(int argc, char* argv[])
{
	CodeGenerator codeGen;

	//check input arguments.
	if (argc < 2) {
		cerr << endl << "Input file is missing. Aborting ..." << endl;
		exit(1);
	}

	Object* theProgram = getTree(argv[1]);
	assert(theProgram == root);

	fstream treeFile(TREE_OUTPUT_TEXT_FILE, ios::out);
	fstream pcodeFile(OUTPUT_CODE_TEXT, ios::out);
	treeFile << AST_FILE_HEADER << endl;
	///pcodeFile << AST_FILE_HEADER << endl;
	writeAST(theProgram, treeFile);
	Pcode(theProgram, pcodeFile);
	treeFile.close();
	return (0);
}


