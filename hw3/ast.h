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
/*   PROGRAM = 258,
     BBEGIN = 259,
     END = 260,
     DECLARE = 261,
     PROCEDURE = 262,
     FUNCTION = 263,
     LABEL = 264,
     INTEGER = 265,
     REAL = 266,
     RECORD = 267,
     BOOLEAN = 268,
     ARRAY = 269,
     OF = 270,
     ASSIGN = 271,
     LC = 272,
     RC = 273,
     IF = 274,
     THEN = 275,
     ELSE = 276,
     WHILE = 277,
     REPEAT = 278,
     FI = 279,
     DO = 280,
     OD = 281,
     READ = 282,
     WRITE = 283,
     TRUE = 284,
     FALSE = 285,
     ADD = 286,
     MIN = 287,
     MUL = 288,
     DIV = 289,
     GOTO = 290,
     MOD = 291,
     LES = 292,
     LEQ = 293,
     EQU = 294,
     NEQ = 295,
     GRE = 296,
     GEQ = 297,
     AND = 298,
     OR = 299,
     NOT = 300,
     CASE = 301,
     FOR = 302,
     FIN = 303,
     IDENTICAL = 304,
     FROM = 305,
     BY = 306,
     TO = 307,
     NEW = 308,
     INTCONST = 309,
     IDE = 310,
     REALCONST = 311,
     STRING = 312,
     DUMMY = 313*/

#ifndef AST_H
#define AST_H

#include <iostream>
#include <assert.h>
#include <string>
#include <iomanip>
#include <stack>
#include <typeinfo>

using namespace std;

/**
 * classes
 */

extern int currentIf;
extern int currentVarAddress;
extern string codelrFlag;

extern int incDecFlag;
extern int loopCount;
extern int switchCount;
extern int currentCase;

extern stack<int> currentSwitch;

extern int rightSon;

extern int lastOp;

extern string currentVarName;

extern int newFlag;

/// for array
extern int flagArray;
extern int arrayPrintNow;
extern int countDim;
extern string arrName;
extern string arrCellTypePlace;
extern string arrFirstType;
extern int currentDim;
///

/// for record
extern string currentRec;
extern int flagRecord;
extern int recordRefRight;
extern int recordRefLeft;
extern string recName;
extern int recSize;
///

/// for pointer
extern string ptrName;
extern int ptrCount;
extern int addressTypeFlag;
extern int arrInRec;
///

/// for porcedure functions
static string byRefVal = "";
static string nameParam = "";
static string progFlag = "";
static string procFuncName = "";
static string paramType = "";
static int currentMst = 0;
static int paramAddress = 5;
static int pIdx = 0;
static int varFlag = 0;
static string procFuncEnd = "";
static string procFuncCall = "";
static int blockFlag = -1;
static int finFlag = 0;
static int ordFlag = 0;
static int funcProcedFlag = 0;
static int procFuncSt = 0;
static int paramCount = -1;
static string progName = "";
static int ordSecFlag = 0;
static int varX = 0;
static int RefTchk = 0;
static int prmFlag = 0;
static int chg = 0;
///


struct dims {
    int down_;
    int up_;
    struct dims *next = NULL;
};

class dims_List {
private:
    dims *head;
    dims *tail;
public:
    dims_List() {
        head = NULL;
        tail = NULL;
    }

    void insertDim(int down, int up) {
        dims *new_dim = new dims;
        new_dim->down_ = down;
        new_dim->up_ = up;

        if (head == NULL) {
            head = new_dim;
            tail = new_dim;
        } else {
            tail->next = new_dim;
            tail = tail->next;
        }
    }

    int mull_dims() {
        dims *temp = NULL;
        int counter = 1;
        temp = head;
        while (temp != NULL) {
            counter = counter * (temp->up_ - temp->down_ + 1);
            temp = temp->next;
        }
        return counter;
    }

    int mull_dims_(int from) {
        dims *temp = NULL;
        temp = head;
        int counter = 1;
        int start = 1;
        while (temp != NULL) {
            if (start > from)
                counter = counter * (temp->up_ - temp->down_ + 1);
            start = start + 1;
            temp = temp->next;
        }
        return counter;
    }

    int for_subpart() {
        int Counter = 0;
        int i = 1;
        dims *temp = head;
        while (temp != NULL) {
            Counter = Counter + temp->down_ * mull_dims_(i);
            i = i + 1;
            temp = temp->next;
        }
        return Counter;
    }

    int count_dims() {
        int i = 0;
        dims *temp = head;
        while (temp != NULL) {
            i = i + 1;
            temp = temp->next;
        }
        return i;
    }
};

struct reclist {
    string identifier;
    string typed;
    int size;
    int address;
    string pointer = "";
    string recname = "";

    int pointed_at_size;

    dims_List dimLis;
    int arrCellSize;
    string arrCellsType = "";
    int flag_that_cell_array = 0;

    struct reclist *next = NULL;
};

class Record_List {
private:

    reclist *head;
    reclist *tail;
public:
    Record_List() {

        head = NULL;
        tail = NULL;
    }

    void insertRL(string _id, int _address, string _typed, int _size) {
        reclist *temp = new reclist;
        temp->address = _address;
        temp->identifier = _id;
        temp->size = _size;
        temp->typed = _typed;
        if (head == NULL) {
            head = temp;
            tail = temp;
        } else {
            tail->next = temp;
            tail = tail->next;
        }

    }

    void insertRLp(string _id, int _address, string _typed, int _size, string pointer, int sizeofcells) {
        reclist *temp = new reclist;
        temp->address = _address;
        temp->identifier = _id;
        temp->size = _size;
        temp->typed = _typed;
        temp->pointer = pointer;
        temp->pointed_at_size = sizeofcells;

        if (head == NULL) {
            head = temp;
            tail = temp;
        } else {
            tail->next = temp;
            tail = tail->next;
        }

    }

    void insertRLn(string _id, int _address, string _typed, int _size, string name) {
        reclist *temp = new reclist;
        temp->address = _address;
        temp->identifier = _id;
        temp->size = _size;
        temp->typed = _typed;
        temp->recname = name;
        if (head == NULL) {
            head = temp;
            tail = temp;
        } else {
            tail->next = temp;
            tail = tail->next;
        }

    }

    reclist *findList_rec(string name) {

        reclist *temp = NULL;
        temp = head;
        while (temp != NULL) {
            if (temp->identifier == name)

                return temp;
            temp = temp->next;
        }
        return NULL;
    }

    void insertArr_rec(string _id, int _address, string _typed) {
        if (findList_rec(_id) == NULL) {
            reclist *temp = new reclist;
            temp->address = _address;
            temp->identifier = _id;
            temp->typed = _typed;

            if (head == NULL) {
                head = temp;
                tail = temp;
            } else {
                tail->next = temp;
                tail = tail->next;
            }
        }

    }

    int calcArrSize_rec(string _id, int cell_size) {
        reclist *temp = NULL;
        temp = head;
        int conter = 1;
        while (temp != NULL) {
            if (temp->identifier == _id) {
                conter = temp->dimLis.mull_dims();
                break;
            }
            temp = temp->next;
        }
        conter = conter * cell_size;
        temp->size = conter;
        return conter;
    }

    void add_cell_info_rec(string _id, int cell_size, string cell_type) {
        reclist *temp = NULL;
        temp = head;
        while (temp != NULL) {
            if (temp->identifier == _id) {
                temp->arrCellSize = cell_size;
                temp->arrCellsType = cell_type;
                break;
            }
            temp = temp->next;
        }
    }

    int access_shift(string _id, int order) {
        reclist *temp = NULL;
        temp = head;
        int return_val;
        while (temp != NULL) {
            if (temp->identifier == _id) {
                return_val = (temp->dimLis.mull_dims_(order)) * (temp->arrCellSize);
                break;
            }
            temp = temp->next;
        }

        return return_val;
    }

    int subpart(string _id) {

        reclist *temp = NULL;
        temp = head;
        int return_val;
        while (temp != NULL) {
            if (temp->identifier == _id) {
                return_val = (temp->dimLis.for_subpart()) * (temp->arrCellSize);
                break;
            }
            temp = temp->next;
        }

        return return_val;

    }

    int numbet_of_dims(string _id) {

        reclist *temp = NULL;
        temp = head;
        int return_val;
        while (temp != NULL) {
            if (temp->identifier == _id) {
                return_val = temp->dimLis.count_dims();
                break;
            }
            temp = temp->next;
        }

        return return_val;
    }

    string getcelltype(string _id) {
        reclist *temp = NULL;
        temp = head;
        while (temp != NULL) {
            if (temp->identifier == _id)
                return temp->arrCellsType;
            temp = temp->next;
        }
        //If element not found
        return "null";
    }


    int findorder(string _id) {
        reclist *temp = NULL;
        int counter = 0;
        temp = head;
        if (temp->identifier == _id) {
            return 0;
        } else {
            while (temp != NULL) {

                counter = counter + temp->size;
                temp = temp->next;

                if (temp->identifier == _id) {
                    return counter;
                }

            }
            return -1;
        }
    }


    string recordName4(string _id) {
        reclist *temp = NULL;

        temp = head;
        while (temp != NULL) {
            if (temp->identifier == _id) {
                return temp->recname;
            }

            temp = temp->next;
        }
        return NULL;
    }

    string getType(string _id) {
        reclist *temp = NULL;

        temp = head;
        while (temp != NULL) {
            if (temp->identifier == _id) {
                return temp->typed;
            }

            temp = temp->next;
        }
        string ret = "";
        return ret;
    }

    reclist *findListt(string name) {
        reclist *temp = NULL;
        temp = head;
        while (temp != NULL) {
            if (temp->identifier == name)

                return temp;
            temp = temp->next;
        }
        return NULL;
    }

    void set_size_arr_poi(string _id, int cell_size) {
        reclist *temp = NULL;
        temp = head;
        int conter = 1;
        while (temp != NULL) {
            if (temp->identifier == _id) {
                conter = temp->dimLis.mull_dims();
                break;
            }
            temp = temp->next;
        }
        conter = conter * cell_size;
        temp->size = 1;
        temp->pointed_at_size = conter;
    }

    void set_namepoint(string _id) {
        reclist *temp = NULL;
        temp = head;
        while (temp != NULL) {
            if (temp->identifier == _id) {
                temp->pointer = "is not declared before";
                break;
            }
            temp = temp->next;
        }

    }

    int find_the_field_in_that_record(string _id) {
        reclist *temp = NULL;
        temp = head;
        while (temp != NULL) {
            if (temp->identifier == _id)
                return 1;
            temp = temp->next;
        }
        return 0;
    }

};

struct node {
    string identifier;
    string typed;
    int size;
    int address;
    int pointed_at_size;
    dims_List dimLis;
    int arrCellSize;
    string arrCellsType = "";
    int flag_that_cell_array = 0;

    Record_List recordLis;
    string pointers = "";
    string funcName = "";
    int ref = 0;
    struct node *next = NULL;
};

class Symbol_Table {
private:
    node *head;
    node *tail;
public:
    Symbol_Table() {

        head = NULL;
        tail = NULL;
    }

    void setAddress(string _id, int address) {
        node *temp = NULL;
        temp = head;
        while (temp != NULL) {
            if (temp->identifier == _id) {
                temp->address = address;
            }

            temp = temp->next;
        }

    }
    node *findList(string name) {
        node *temp = NULL;
        temp = head;
        while (temp != NULL) {
            if (temp->identifier == name)

                return temp;
            temp = temp->next;
        }
        return NULL;
    }
    void insert(string _id, int _address, string _typed, int _size) {
        node *temp = new node;
        temp->address = _address;
        temp->identifier = _id;
        temp->size = _size;
        temp->typed = _typed;
        if (head == NULL) {
            head = temp;
            tail = temp;
        } else {
            tail->next = temp;
            tail = tail->next;
        }
    }
    void insertf(string _id, int _address, string _typed, int _size, string func, int ref) {
        node *temp = new node;
        temp->address = _address;
        temp->identifier = _id;
        temp->size = _size;
        temp->typed = _typed;
        temp->funcName = func;
        temp->ref = ref;

        if (head == NULL) {
            head = temp;
            tail = temp;
        } else {
            tail->next = temp;
            tail = tail->next;
        }
    }
    void insert_Ptr(string _id, int _address, string _typed, int _size, string pointer, int pointed_at_size_) {
        node *temp = new node;
        temp->address = _address;
        temp->identifier = _id;
        temp->size = _size;
        temp->typed = _typed;
        temp->pointers = pointer;
        temp->pointed_at_size = pointed_at_size_;

        if (head == NULL) {
            head = temp;
            tail = temp;
        } else {
            tail->next = temp;
            tail = tail->next;
        }
    }

    void insert_Arr(string _id, int _address, string _typed) {
        if (findList(_id) == NULL) {
            node *temp = new node;
            temp->address = _address;
            temp->identifier = _id;
            temp->typed = _typed;

            if (head == NULL) {
                head = temp;
                tail = temp;
            } else {
                tail->next = temp;
                tail = tail->next;
            }
        }
    }
    int find(string _id) {
        node *temp = NULL;
        temp = head;
        while (temp != NULL) {
            if (temp->identifier == _id)
                return temp->address;
            temp = temp->next;
        }
        return -1;
    }

    string funcname(string _id) {
        node *temp = NULL;
        temp = head;
        while (temp != NULL) {
            if (temp->identifier == _id)
                return temp->funcName;
            temp = temp->next;
        }
        return "";
    }

    int calcArrSize(string _id, int cell_size) {
        node *temp = NULL;
        temp = head;
        int conter = 1;
        while (temp != NULL) {
            if (temp->identifier == _id) {
                conter = temp->dimLis.mull_dims();
                break;
            }
            temp = temp->next;
        }
        conter = conter * cell_size;
        temp->size = conter;
        return conter;
    }

    void cellAdd(string _id, int cell_size, string cell_type) {
        node *temp = NULL;
        temp = head;
        while (temp != NULL) {
            if (temp->identifier == _id) {
                temp->arrCellSize = cell_size;
                temp->arrCellsType = cell_type;
                break;
            }
            temp = temp->next;
        }
    }

    int getAccessShift(string _id, int order) {
        node *temp = NULL;
        temp = head;
        int return_val;
        while (temp != NULL) {
            if (temp->identifier == _id) {
                return_val = (temp->dimLis.mull_dims_(order)) * (temp->arrCellSize);
                break;
            }
            temp = temp->next;
        }

        return return_val;
    }

    int calcSubpart(string _id) {

        node *temp = NULL;
        temp = head;
        int return_val;
        while (temp != NULL) {
            if (temp->identifier == _id) {
                return_val = (temp->dimLis.for_subpart()) * (temp->arrCellSize);
                break;
            }
            temp = temp->next;
        }

        return return_val;

    }

    int dimsNum(string _id) {

        node *temp = NULL;
        temp = head;
        int return_val;
        while (temp != NULL) {
            if (temp->identifier == _id) {
                return_val = temp->dimLis.count_dims();
                break;
            }
            temp = temp->next;
        }

        return return_val;
    }

    string getCellType(string _id) {
        node *temp = NULL;
        temp = head;
        while (temp != NULL) {
            if (temp->identifier == _id)
                return temp->arrCellsType;
            temp = temp->next;
        }
        return "null";
    }

    void arrSizePtrSet(string _id, int cell_size) {
        node *temp = NULL;
        temp = head;
        int conter = 1;
        while (temp != NULL) {
            if (temp->identifier == _id) {
                conter = temp->dimLis.mull_dims();
                break;
            }
            temp = temp->next;
        }
        conter = conter * cell_size;
        temp->size = 1;
        temp->pointed_at_size = conter;
    }

    void set_namepoint(string _id) {
        node *temp = NULL;
        temp = head;
        while (temp != NULL) {
            if (temp->identifier == _id) {
                temp->pointers = "is not declared before";
                break;
            }
            temp = temp->next;
        }

    }

    string find_the_record_by_field(string _field) {
        node *temp = NULL;
        temp = head;
        while (temp != NULL) {
            if (temp->typed == "RecordType") {
                if (temp->recordLis.find_the_field_in_that_record(_field) == 1) {
                    return temp->identifier;
                }
            }
            temp = temp->next;
        }
        return "null";
    }

};

static Symbol_Table my_st;

struct functionsNames {
    string FuncName = "";
    string static_link = "";
    string parameter[100];
    string dynamic_list[100];
    string endp = "";
    int pcounter = 5;
    int parameterSize = 0;
    int mst = -1;
    string returnType = "";
    struct functionsNames *next = NULL;
};

class FuncStack {
private:
    functionsNames *head;
    functionsNames *tail;
public:
    FuncStack() {
        head = NULL;
        tail = NULL;
    }

    void Mst_change() {
        int counter = -1;
        functionsNames *temp = NULL;
        temp = head;
        while (temp != NULL) {
            counter = counter + 1;
            temp = temp->next;
        }
        temp = head;
        int i = 0;
        while (temp != NULL) {
            temp->mst = counter - i;
            i++;
            temp = temp->next;
        }
    }

    void insert(string _id) {
        functionsNames *temp = new functionsNames;
        temp->FuncName = _id;
        for (int i = 0; i < 100; i++) {
            temp->parameter[i] = "";
        }
        for (int i = 0; i < 100; i++) {
            temp->dynamic_list[i] = "";
        }
        temp->static_link = "";
        temp->pcounter = 5;
        temp->endp = "";
        temp->mst = -1;
        temp->returnType = "";
        temp->parameterSize = 0;
        if (head == NULL) {
            head = temp;
            tail = temp;
        } else {
            tail->next = temp;
            tail = tail->next;
        }

    }

    functionsNames *find(string _id) {

        functionsNames *temp = NULL;
        temp = head;
        while (temp != NULL) {
            if (temp->FuncName == _id)

                return temp;
            temp = temp->next;
        }
        return NULL;
    }

    int orderFunc(string _id) {
        functionsNames *temp = NULL;
        temp = head;
        int counterrr = -1;
        while (temp != NULL) {
            temp = temp->next;
            counterrr = counterrr + 1;

        }
        temp = head;
        while (temp != NULL) {
            if (temp->FuncName == _id) {
                return counterrr;
            }
            temp = temp->next;
            counterrr = counterrr - 1;
        }
        return -1;
    }

};

static FuncStack myFunc;

class Object {
public:
    virtual void print(ostream &os) = 0;

    virtual void pcodegen(ostream &os) = 0;

    virtual Object *clone() const { return NULL; }

    virtual ~Object() {}
};


class Expr : public Object {
public:
    // Unary operations
    Expr(int op, Object *atom) : op_(op), atom_(atom), left_(NULL), right_(NULL), unary_(true) {}

    // Binary operations
    Expr(int op, Object *left, Object *right) : op_(op), left_(left), right_(right), atom_(NULL), unary_(false) {}

    Expr(const Expr &exp) {
        unary_ = exp.unary_;
        op_ = exp.op_;
        left_ = exp.left_->clone();
        right_ = exp.right_->clone();
        atom_ = exp.atom_->clone();
    }

    virtual ~Expr() {
        if (left_) delete left_;
        if (right_) delete right_;
        if (atom_) delete atom_;
    }

    void print(ostream &os) {
        os << "Node name : Expr" << endl;
        assert(op_);
        if (unary_) {
            os << "Unary op is : " << op_;
            assert(atom_);
            atom_->print(os);
        } else {
            os << "Binary op is : " << op_;
            assert(left_ && right_);


            left_->print(os);
            right_->print(os);
        }
    }

    void pcodegen(ostream &os) {
        assert(op_);
        if (unary_) {
            assert(atom_);
            atom_->pcodegen(os);
            if (op_ == 287)
                os << "neg" << endl;
            if (op_ == 300)
                os << "not" << endl;
        } else {
            assert(left_ && right_);

            rightSon = 0;

            right_->pcodegen(os);

            rightSon = 1;
            lastOp = op_;

            left_->pcodegen(os);

            if (op_ == 286)
                if (incDecFlag == 0)
                    os << "add" << endl;
            if (op_ == 287)
                if (incDecFlag == 0)
                    os << "sub" << endl;

            if (op_ == 296)
                os << "grt" << endl;
            if (op_ == 292)
                os << "les" << endl;
            if (op_ == 298)
                os << "and" << endl;
            if (op_ == 294)
                os << "equ" << endl;
            if (op_ == 299)
                os << "or" << endl;
            if (op_ == 288)
                os << "mul" << endl;
            if (op_ == 289)
                os << "div" << endl;
            if (op_ == 291)
                os << "mod" << endl;
            if (op_ == 293)
                os << "leq" << endl;
            if (op_ == 297)
                os << "geq" << endl;
            if (op_ == 295)
                os << "neq" << endl;
            rightSon = 0;
            incDecFlag = 0;
        }
    }

    virtual Object *clone() const { return new Expr(*this); }

private:
    bool unary_;
    int op_;
    Object *left_;
    Object *right_;
    Object *atom_;
};


class ExprList : public Object {
public:

    ExprList(Object *expr) : expr_(expr), expr_list_(NULL) { assert(expr_); }

    ExprList(Object *expr, Object *expr_list) : expr_(expr), expr_list_(expr_list) { assert(expr_ && expr_list_); }

    ExprList(const ExprList &exp) {
        expr_ = exp.expr_->clone();
        expr_list_ = exp.expr_list_->clone();
    }

    virtual ~ExprList() {
        if (expr_) delete expr_;
        if (expr_list_) delete expr_list_;
    }

    void print(ostream &os) {
        os << "Node name : ExprList";
        assert(expr_);
        expr_->print(os);
        if (expr_list_) {
            expr_list_->print(os);
        }
    }

    void pcodegen(ostream &os) {
        assert(expr_);

        expr_->pcodegen(os);
        if (expr_list_) {
            RefTchk = 1;
            expr_list_->pcodegen(os);
        }

    }

    virtual Object *clone() const { return new ExprList(*this); }

private:
    Object *expr_;
    Object *expr_list_;
};


class Dim : public Object {
public:
    Dim(Object *exp) : exp_(exp), dim_(NULL) { assert(exp_); }

    Dim(Object *exp, Object *dim) : exp_(exp), dim_(dim) { assert(exp_ && dim_); }

    Dim(const Dim &d) {
        exp_ = d.exp_->clone();
        dim_ = d.dim_->clone();
    }

    virtual ~Dim() {
        if (exp_) delete exp_;
        if (dim_) delete dim_;
    }

    void print(ostream &os) {
        os << "Node name : Dim" << endl;
        assert(exp_);
        exp_->print(os);
        if (dim_) {
            dim_->print(os);
        }
    }

    void pcodegen(ostream &os) {
        assert(exp_);
        countDim = countDim + 1;
        currentDim = currentDim + 1;
        exp_->pcodegen(os);

        if (countDim > 0) {

            if (currentDim == 1) {
                arrFirstType = arrName;
            }

            if (my_st.findList(arrName) != NULL)
                arrInRec = 0;
            else if (my_st.findList(recName)->recordLis.findList_rec(arrName) != NULL)
                arrInRec = 1;


            if (arrInRec == 1) {

                if (my_st.findList(my_st.findList(recName)->recordLis.getcelltype(arrName)) != NULL)
                    arrCellTypePlace = "ST";
                else if (my_st.findList(recName)->recordLis.findList_rec(
                        my_st.findList(recName)->recordLis.getcelltype(arrName)) != NULL)
                    arrCellTypePlace = "record list";
                else
                    arrCellTypePlace = "ST";


                if (arrCellTypePlace == "ST") {
                    int acc_shft = -1;
                    acc_shft = my_st.findList(recName)->recordLis.access_shift(arrName, countDim);
                    os << "ixa " << acc_shft << endl;

                    if (my_st.findList(recName)->recordLis.numbet_of_dims(arrName) == countDim) {
                        int subprt = my_st.findList(recName)->recordLis.subpart(arrName);
                        os << "dec " << subprt << endl;
                        if (my_st.findList(my_st.findList(recName)->recordLis.getcelltype(arrName)) == NULL) {
                            if (codelrFlag == "coder") {
                                os << "ind" << endl;
                            }
                            codelrFlag = "coder";
                        } else if ((my_st.findList(my_st.findList(recName)->recordLis.getcelltype(arrName))->typed !=
                                    "arrayType") &&
                                   (my_st.findList(my_st.findList(recName)->recordLis.getcelltype(arrName))->typed !=
                                    "RecordType")) {

                            if (codelrFlag == "coder") {
                                os << "ind" << endl;
                            }
                            codelrFlag = "coder";
                        }
                        if (my_st.findList(my_st.findList(recName)->recordLis.getcelltype(arrName)) != NULL) {
                            if (my_st.findList(my_st.findList(recName)->recordLis.getcelltype(arrName))->typed ==
                                "arrayType") {
                                countDim = 0;
                                arrName = my_st.findList(recName)->recordLis.getcelltype(arrName);
                            } else if (my_st.findList(my_st.findList(recName)->recordLis.getcelltype(arrName))->typed ==
                                       "addressType" && my_st.findList(
                                    my_st.findList(recName)->recordLis.getcelltype(arrName))->pointers ==
                                                        "is not declared before") {
                                countDim = 0;
                                arrName = my_st.findList(recName)->recordLis.getcelltype(arrName);
                            }

                        }
                    }

                }
                if (arrCellTypePlace == "record list") {
                    int acc_shft = -1;
                    acc_shft = my_st.findList(recName)->recordLis.access_shift(arrName, countDim);
                    os << "ixa " << acc_shft << endl;

                    if (my_st.findList(recName)->recordLis.numbet_of_dims(arrName) == countDim) {
                        int subprt = my_st.findList(recName)->recordLis.subpart(arrName);
                        os << "dec " << subprt << endl;
                        if (my_st.findList(recName)->recordLis.findList_rec(
                                my_st.findList(recName)->recordLis.getcelltype(arrName)) == NULL) {
                            if (codelrFlag == "coder") {
                                os << "ind" << endl;
                            }
                            codelrFlag = "coder";
                        } else if ((my_st.findList(recName)->recordLis.findList_rec(
                                my_st.findList(recName)->recordLis.getcelltype(arrName))->typed != "arrayType") &&
                                   (my_st.findList(recName)->recordLis.findList_rec(
                                           my_st.findList(recName)->recordLis.getcelltype(arrName))->typed !=
                                    "RecordType")) {

                            if (codelrFlag == "coder") {
                                os << "ind" << endl;
                            }
                            codelrFlag = "coder";
                        }
                        if (my_st.findList(recName)->recordLis.findList_rec(
                                my_st.findList(recName)->recordLis.getcelltype(arrName)) !=
                            NULL) { //if the type is array
                            if (my_st.findList(recName)->recordLis.findList_rec(
                                    my_st.findList(recName)->recordLis.getcelltype(arrName))->typed == "arrayType") {
                                countDim = 0;
                                arrName = my_st.findList(recName)->recordLis.getcelltype(arrName);
                            } else if (my_st.findList(recName)->recordLis.findList_rec(
                                    my_st.findList(recName)->recordLis.getcelltype(arrName))->typed == "addressType" &&
                                       my_st.findList(recName)->recordLis.findList_rec(
                                               my_st.findList(recName)->recordLis.getcelltype(arrName))->pointer ==
                                       "is not declared before") {
                                countDim = 0;
                                arrName = my_st.findList(recName)->recordLis.getcelltype(arrName);
                            }
                        }

                    }

                }

                arrInRec = 0;
            } else {
                int acc_shft;
                acc_shft = my_st.getAccessShift(arrName, countDim);
                os << "ixa " << acc_shft << endl;

                if (my_st.dimsNum(arrName) == countDim) {
                    int subprt = my_st.calcSubpart(arrName);
                    os << "dec " << subprt << endl;

                    if (my_st.findList(my_st.getCellType(arrName)) == NULL) {
                        if (codelrFlag == "coder") {
                            os << "ind" << endl;
                        }
                        codelrFlag = "coder";
                    } else if ((my_st.findList(my_st.getCellType(arrName))->typed != "arrayType") &&
                               (my_st.findList(my_st.getCellType(arrName))->typed != "RecordType")) {
                        if (codelrFlag == "coder") {
                            os << "ind" << endl;
                        }
                        codelrFlag = "coder";
                    }


                    if (my_st.findList(my_st.getCellType(arrName)) != NULL) {
                        if (my_st.findList(my_st.getCellType(arrName))->typed == "arrayType") {
                            countDim = 0;
                            arrName = my_st.getCellType(arrName);
                        } else if (my_st.findList(my_st.getCellType(arrName))->typed == "addressType") {
                            if (my_st.findList(my_st.getCellType(arrName))->pointers == "is not declared before") {
                                countDim = 0;
                                arrName = my_st.getCellType(arrName);
                            }

                        }
                    }


                }
            }

        }
        if (dim_) {
            dim_->pcodegen(os);
        }

        recordRefLeft = 0;
        arrInRec = 0;
        countDim = 0;
        currentDim = 0;
        arrName = arrFirstType;
    }

    virtual Object *clone() const { return new Dim(*this); }

private:
    Object *exp_;
    Object *dim_;
};


class Atom : public Object {
};


class IntConst : public Atom {
public:
    IntConst(const int i) : i_(i) {}

    IntConst(const IntConst &in) : i_(in.i_) {}

    void print(ostream &os) {
        os << "Node name : IntConst. Value is :" << i_ << endl;
    }

    void pcodegen(ostream &os) {

        if (rightSon == 1 && lastOp == 286) {
            incDecFlag = 1;
            os << "inc " << i_ << endl;
        } else if (rightSon == 1 && lastOp == 287) {
            incDecFlag = 1;
            os << "dec " << i_ << endl;
        } else {
            os << "ldc " << i_ << endl;

        }
    }

    virtual Object *clone() const { return new IntConst(*this); }

private:
    const int i_;
};


class RealConst : public Atom {
public:
    RealConst(const double r) : r_(r) {}

    RealConst(const RealConst &in) : r_(in.r_) {}

    void print(ostream &os) {
        os << "Node name : RealConst. Value is :" << r_ << endl;
    }

    void pcodegen(ostream &os) {
        if (rightSon == 1 && lastOp == 286) {
            incDecFlag = 1;
            os << std::fixed << std::setprecision(1) << "inc " << r_ << endl;
        } else if (rightSon == 1 && lastOp == 287) {
            incDecFlag = 1;
            os << std::fixed << std::setprecision(1) << "dec " << r_ << endl;
        } else {
            os << std::fixed << std::setprecision(1) << "ldc " << r_ << endl;
        }
    }

    virtual Object *clone() const { return new RealConst(*this); }

private:
    const double r_;
};

class True : public Atom {
public:
    void print(ostream &os) {
        os << "Node name : trueConst. Value is true" << endl;
    }

    void pcodegen(ostream &os) {
        os << "ldc 1" << endl;
    }

    virtual Object *clone() const { return new True(); }

};

class False : public Atom {
public:
    void print(ostream &os) {
        os << "Node name : trueConst. Value is false" << endl;
    }

    void pcodegen(ostream &os) {

        os << "ldc 0" << endl;

    }

    virtual Object *clone() const { return new False(); }
};

class Var : public Atom {
};


class ArrayRef : public Var {
public:
    ArrayRef(Object *var, Object *dim) : var_(var), dim_(dim) { assert(var_ && dim_); }

    ArrayRef(const ArrayRef &arr) {
        var_ = arr.var_->clone();
        dim_ = arr.dim_->clone();
    }

    virtual ~ArrayRef() {
        if (var_) delete var_;
        if (dim_) delete dim_;
    }

    void print(ostream &os) {
        os << "Node name : ArrayRef" << endl;
        assert(var_ && dim_);
        var_->print(os);
        dim_->print(os);
    }

    void pcodegen(ostream &os) {
        assert(var_ && dim_);
        arrayPrintNow = 1;
        var_->pcodegen(os);
        dim_->pcodegen(os);


    }

    virtual Object *clone() const { return new ArrayRef(*this); }

private:
    Object *var_;
    Object *dim_;
};


class RecordRef : public Var {
public:
    RecordRef(Object *varExt, Object *varIn) : varExt_(varExt), varIn_(varIn) { assert(varExt_ && varIn_); }

    RecordRef(const RecordRef &rec) {
        varExt_ = rec.varExt_->clone();
        varIn_ = rec.varIn_->clone();
    }

    virtual ~RecordRef() {
        if (varExt_) delete varExt_;
        if (varIn_) delete varIn_;
    }

    void print(ostream &os) {
        os << "Node name : RecordRef" << endl;
        assert(varExt_ && varIn_);
        varExt_->print(os);
        varIn_->print(os);
    }

    void pcodegen(ostream &os) {

        assert(varExt_ && varIn_);
        recordRefRight = 1;
        varExt_->pcodegen(os);
        recordRefLeft = 1;
        varIn_->pcodegen(os);


    }

    virtual Object *clone() const { return new RecordRef(*this); }

private:
    Object *varExt_;
    Object *varIn_;
};


class AddressRef : public Var {
public:
    AddressRef(Object *var) : var_(var) { assert(var_); }

    AddressRef(const AddressRef &addre) {
        var_ = addre.var_->clone();
    }

    virtual ~AddressRef() {
        if (var_) delete var_;
    }

    void print(ostream &os) {
        os << "Node name : AddressRef" << endl;
        assert(var_);
        var_->print(os);
    }

    void pcodegen(ostream &os) {
        assert(var_);
        ptrCount = ptrCount + 1;
        var_->pcodegen(os);
    }

    virtual Object *clone() { return new AddressRef(*this); }

private:
    Object *var_;
};

class Statement : public Object {
};

class NewStatement : public Statement {
public:
    NewStatement(Object *var) : var_(var) { assert(var_); }

    NewStatement(const NewStatement &ns) {
        var_ = ns.var_->clone();
    }

    virtual ~NewStatement() {
        if (var_) delete var_;
    }

    void print(ostream &os) {
        os << "Node name : NewStatement";
        assert(var_);
        var_->print(os);
    }

    void pcodegen(ostream &os) {
        assert(var_);
        newFlag = 1;
        var_->pcodegen(os);
    }

    virtual Object *clone() { return new NewStatement(*this); }

private:
    Object *var_;
};

class WriteStrStatement : public Statement {
public:
    WriteStrStatement(const char *str) {
        str_ = new string(str);
    }

    WriteStrStatement(const WriteStrStatement &ns) {
        str_ = new string(*ns.str_);
    }

    virtual ~WriteStrStatement() {
        if (str_) delete str_;
    }

    void print(ostream &os) {
        os << "Node name : WriteStrStatement";
        assert(str_);
        os << "Str statement is: " << str_ << endl;
    }

    void pcodegen(ostream &os) {
        assert(str_);
    }

    virtual Object *clone() { return new WriteStrStatement(*this); }

private:
    string *str_;
};

class WriteVarStatement : public Statement {
public:
    WriteVarStatement(Object *exp) : exp_(exp) { assert(exp_); }

    WriteVarStatement(const WriteVarStatement &ex) {
        exp_ = ex.clone();
    }

    virtual ~WriteVarStatement() {
        if (exp_) delete exp_;
    }

    void print(ostream &os) {
        os << "Node name : WriteVarStatement";
        assert(exp_);
        exp_->print(os);
    }

    void pcodegen(ostream &os) {
        assert(exp_);
        exp_->pcodegen(os);
        os << "print" << endl;
    }

    virtual Object *clone() const { return new WriteVarStatement(*this); }

private:
    Object *exp_;
};

class ProcedureStatement : public Statement {
public:
    ProcedureStatement(const char *str) {
        str_ = new string(str);
    }

    ProcedureStatement(Object *expr_list, const char *str) : expr_list_(expr_list) {
        assert(expr_list_);
        str_ = new string(str);
    }

    ProcedureStatement(const ProcedureStatement &ps) {
        expr_list_ = ps.expr_list_->clone();
        str_ = new string(*ps.str_);
    }

    virtual ~ProcedureStatement() {
        if (str_) delete str_;
        if (expr_list_) delete expr_list_;
    }

    void print(ostream &os) {
        os << "Node name : ProcedureStatement. Proc name : " << str_ << endl;
        if (expr_list_) {
            expr_list_->print(os);
        }
    }

    void pcodegen(ostream &os) {

        paramCount = 0;


        if (myFunc.find(*str_) == 0) {
            int mstp = 0;

            string fatherf = procFuncCall;
            while (fatherf != progName) {

                if (my_st.findList(*str_)->typed == fatherf) {
                    break;
                }
                fatherf = myFunc.find(fatherf)->static_link;
                mstp++;
            }

            if (fatherf == progName) {
                mstp = 0;
            }
            mstp = mstp - 1;
            procFuncSt = 1;
            os << "mstf " << mstp << " " << my_st.findList(*str_)->address << endl;
        } else {
            procFuncSt = 1;
            int mstp = 1;

            string fatherf = procFuncCall;
            while (fatherf != progName) {

                if (*str_ == fatherf) {
                    break;
                }
                fatherf = myFunc.find(fatherf)->static_link;
                mstp++;
            }

            if (fatherf == progName) {
                mstp = 0;
            }
            os << "mst " << mstp << endl;


        }

        procFuncName = *str_;
        if (expr_list_) {

            expr_list_->pcodegen(os);
            procFuncSt = 0;
            chg = 0;
            if (myFunc.find(*str_) == 0) {
                os << "smp " << myFunc.find(my_st.findList(*str_)->typed)->parameterSize << endl;
                int mstp = 0;

                string fatherf = procFuncCall;
                while (fatherf != progName) {

                    if (my_st.findList(*str_)->typed == fatherf) {
                        break;
                    }
                    fatherf = myFunc.find(fatherf)->static_link;
                    mstp++;
                }

                if (fatherf == progName) {
                    mstp = 0;
                }

                procFuncSt = 1;
                mstp = mstp - 1;
                os << "cupi " << mstp << " " << my_st.findList(*str_)->address << endl;
            } else {
                os << "cup " << myFunc.find(*str_)->parameterSize << " " << *str_ << endl;

            }

        }

    }

    virtual Object *clone() const { return new ProcedureStatement(*this); }

private:
    Object *expr_list_;
    string *str_;
};

class Case : public Object {
public:
    Case(Object *stat_list, int val) : leafChild_(NULL), stat_list_(stat_list) {
        leafChild_ = new IntConst(val);
        assert(stat_list_);
    }

    Case(const Case &c) {
        stat_list_ = c.stat_list_->clone();
        leafChild_ = c.leafChild_->clone();
    }

    virtual ~Case() {
        if (stat_list_) delete stat_list_;
        if (leafChild_) delete leafChild_;
    }

    void print(ostream &os) {
        os << "Node name : Case";
        assert(stat_list_);
        stat_list_->print(os);
    }

    void pcodegen(ostream &os) {
        assert(stat_list_);
        stat_list_->pcodegen(os);
    }

    virtual Object *clone() const { return new Case(*this); }

private:
    Object *stat_list_;
    Object *leafChild_;
};

class CaseList : public Object {
public:
    CaseList(Object *ccase) : case_(ccase), case_list_(NULL) { assert(case_); }

    CaseList(Object *ccase, Object *case_list) : case_(ccase), case_list_(case_list) { assert(case_ && case_list_); }

    CaseList(const CaseList &cl) {
        case_ = cl.case_->clone();
        case_list_ = cl.case_list_->clone();
    }

    virtual ~CaseList() {
        if (case_) delete case_;
        if (case_list_) delete case_list_;
    }

    void print(ostream &os) {
        os << "Node name : CaseList";
        assert(case_);
        case_->print(os);
        if (case_list_) {
            case_list_->print(os);
        }
    }

    void pcodegen(ostream &os) {
        assert(case_);
        int ctr_case = ++currentCase;

        os << "case_" << ctr_case << "_" << currentSwitch.top() << ":" << endl;

        case_->pcodegen(os);

        os << "ujp end_switch_" << currentSwitch.top() << endl;
        if (case_list_) {
            case_list_->pcodegen(os);
        }
        os << "ujp case_" << ctr_case << "_" << currentSwitch.top() << endl;
    }

    virtual Object *clone() const { return new CaseList(*this); }

private:
    Object *case_;
    Object *case_list_;
};

class CaseStatement : public Statement {
public:
    CaseStatement(Object *exp, Object *case_list) : exp_(exp), case_list_(case_list) { assert(exp_ && case_list_); }

    CaseStatement(const CaseStatement &cs) {
        exp_ = cs.exp_->clone();
        case_list_ = cs.case_list_->clone();
    }

    virtual ~CaseStatement() {
        if (exp_) delete exp_;
        if (case_list_) delete case_list_;
    }

    void print(ostream &os) {
        os << "Node name : CaseStatement";
        assert(exp_ && case_list_);
        exp_->print(os);
        case_list_->print(os);
    }

    void pcodegen(ostream &os) {
        assert(exp_ && case_list_);
        int ctr_switch = ++switchCount;
        currentSwitch.push(ctr_switch);
        exp_->pcodegen(os);
        os << "neg" << endl;
        os << "ixj end_switch_" << currentSwitch.top() << endl;
        int revent_case = currentCase;
        currentCase = 0;
        case_list_->pcodegen(os);
        currentCase = revent_case;
        os << "end_switch_" << currentSwitch.top() << ":" << endl;
        currentSwitch.pop();
    }

    virtual Object *clone() const { return new CaseStatement(*this); }

private:
    Object *exp_;
    Object *case_list_;
};

class LoopStatement : public Statement {
public:
    LoopStatement(Object *exp, Object *stat_list) : exp_(exp), stat_list_(stat_list) { assert(exp_ && stat_list_); }

    LoopStatement(const LoopStatement &ls) {
        exp_ = ls.exp_->clone();
        stat_list_ = ls.stat_list_->clone();
    }

    virtual ~LoopStatement() {
        if (exp_) delete exp_;
        if (stat_list_) delete stat_list_;
    }

    void print(ostream &os) {
        os << "Node name : LoopStatement";
        assert(exp_ && stat_list_);
        exp_->print(os);
        stat_list_->print(os);
    }

    void pcodegen(ostream &os) {
        int _counter_loop = ++loopCount;
        assert(exp_ && stat_list_);
        os << "loop_" << _counter_loop << ":" << endl;
        exp_->pcodegen(os);
        os << "fjp end_loop_" << _counter_loop << endl;
        stat_list_->pcodegen(os);
        os << "ujp loop_" << _counter_loop << endl;
        os << "end_loop_" << _counter_loop << ":" << endl;

    }

    virtual Object *clone() const { return new LoopStatement(*this); }

private:
    Object *exp_;
    Object *stat_list_;
};


class ConditionalStatement : public Statement {
public:
    ConditionalStatement(Object *exp, Object *stat_list_if) : exp_(exp), stat_list_if_(stat_list_if),
                                                              stat_list_else_(NULL) { assert(exp_ && stat_list_if_); }

    ConditionalStatement(Object *exp, Object *stat_list_if, Object *stat_list_else) : exp_(exp),
                                                                                      stat_list_if_(stat_list_if),
                                                                                      stat_list_else_(stat_list_else) {
        assert(exp_ && stat_list_if_ && stat_list_else_);
    }

    ConditionalStatement(const ConditionalStatement &cs) {
        exp_ = cs.exp_->clone();
        stat_list_if_ = cs.stat_list_if_->clone();
        stat_list_else_ = cs.stat_list_else_->clone();
    }

    virtual ~ConditionalStatement() {
        if (exp_) delete exp_;
        if (stat_list_if_) delete stat_list_if_;
        if (stat_list_else_) delete stat_list_else_;
    }

    void print(ostream &os) {
        os << "Node name : ConditionalStatement";
        assert(exp_ && stat_list_if_);
        exp_->print(os);
        stat_list_if_->print(os);
        if (stat_list_else_) {
            stat_list_else_->print(os);
        }
    }

    void pcodegen(ostream &os) {
        int num_if = ++currentIf;
        assert(exp_ && stat_list_if_);
        exp_->pcodegen(os);

        if (!stat_list_else_) {
            os << "fjp end_if_" << num_if << endl;
            stat_list_if_->pcodegen(os);
        }

        if (stat_list_else_) {
            os << "fjp else_if_" << num_if << endl;
            stat_list_if_->pcodegen(os);
            os << "ujp end_if_" << num_if << endl;
            os << "else_if_" << num_if << ":" << endl;
            stat_list_else_->pcodegen(os);
        }

        os << "end_if_" << num_if << ":" << endl;
    }

    virtual Object *clone() const { return new ConditionalStatement(*this); }

private:
    Object *exp_;
    Object *stat_list_if_;
    Object *stat_list_else_;
};

class Assign : public Statement {
public:
    Assign(Object *var, Object *exp) : var_(var), exp_(exp) { assert(var_ && exp_); }

    Assign(const Assign &as) {
        var_ = as.var_->clone();
        exp_ = as.exp_->clone();
    }

    virtual ~Assign() {
        if (exp_) delete exp_;
        if (var_) delete var_;
    }

    void print(ostream &os) {
        os << "Node name : Assign";
        assert(var_ && exp_);
        exp_->print(os);
        var_->print(os);
    }

    void pcodegen(ostream &os) {
        assert(var_ && exp_);
        codelrFlag = "codel";
        exp_->pcodegen(os);
        var_->pcodegen(os);
        os << "sto" << endl;


    }

    virtual Object *clone() const { return new Assign(*this); }

private:
    Object *var_;
    Object *exp_;
};

class StatementList : public Object {
public:
    StatementList(Object *stat) : stat_list_(NULL), stat_(stat) { assert(stat_); }

    StatementList(Object *stat_list, Object *stat) : stat_list_(stat_list), stat_(stat) { assert(stat_list_ && stat); }

    StatementList(const StatementList &sl) {
        stat_ = sl.stat_->clone();
        stat_list_ = sl.stat_list_->clone();
    }

    virtual ~StatementList() {
        if (stat_) delete stat_;
        if (stat_list_) delete stat_list_;
    }

    void print(ostream &os) {
        os << "Node name : StatementList" << endl;
        if (stat_list_) {
            stat_list_->print(os);
        }
        assert(stat_);
        stat_->print(os);
    }

    void pcodegen(ostream &os) {
        if (stat_list_) {
            stat_list_->pcodegen(os);

        }
        assert(stat_);

        stat_->pcodegen(os);
    }

    virtual Object *clone() const { return new StatementList(*this); }

private:
    Object *stat_;
    Object *stat_list_;
};


class RecordList : public Object {
public:
    RecordList(Object *var_decl) : record_list_(NULL), var_decl_(var_decl) { assert(var_decl_); }

    RecordList(Object *record_list, Object *var_decl) : record_list_(record_list), var_decl_(var_decl) {
        assert(record_list_ && var_decl);
    }

    RecordList(const RecordList &li) {
        var_decl_ = li.var_decl_->clone();
        record_list_ = li.record_list_->clone();
    }

    virtual ~RecordList() {
        if (var_decl_) delete var_decl_;
        if (record_list_) delete record_list_;
    }

    void print(ostream &os) {
        os << "Node name : RecordList" << endl;
        if (record_list_) {
            record_list_->print(os);
        }
        assert(var_decl_);
        var_decl_->print(os);
    }

    void pcodegen(ostream &os) {
        if (record_list_) {
            flagRecord = 1;
            record_list_->pcodegen(os);
        }
        flagRecord = 1;
        assert(var_decl_);
        flagRecord = 1;
        var_decl_->pcodegen(os);
    }

    virtual Object *clone() const { return new RecordList(*this); }

private:
    Object *var_decl_;
    Object *record_list_;
};


class Type : public Object {
};


class SimpleType : public Type {
public:
    SimpleType(const char *name) {
        name_ = new string(name);
    }

    virtual ~SimpleType() {
        if (name_)delete name_;
    }

    SimpleType(const SimpleType &s) {
        name_ = new string(*s.name_);
    }

    void print(ostream &os) {

        os << "Type is : " << (*name_) << endl;
    }

    void pcodegen(ostream &os) {

        paramType = *name_;


        if (progFlag == "func" && funcProcedFlag == 1) {
            myFunc.find(procFuncName)->returnType = *name_;
            funcProcedFlag = 0;
        } else if ((byRefVal == "Ref" || byRefVal == "Value") && varFlag == 1) {


            int ref = 0;


            if (byRefVal == "Ref") {
                paramAddress = currentVarAddress;
                ref = 1;

                myFunc.find(procFuncName)->pcounter++;
                my_st.insertf(nameParam, currentVarAddress, paramType, 1, procFuncName, ref);
                myFunc.find(procFuncName)->parameterSize++;
                my_st.findList(nameParam)->address = currentVarAddress;
                myFunc.find(procFuncName)->parameter[pIdx] = nameParam;

                pIdx = pIdx + 1;
                int i = 0;
                int counter = 0;


                currentVarAddress = currentVarAddress + 1;

            }

            if (byRefVal == "Value") {

                paramAddress = currentVarAddress;
                myFunc.find(procFuncName)->pcounter++;
                my_st.insertf(nameParam, currentVarAddress, paramType, 1, procFuncName, ref);
                my_st.findList(nameParam)->address = currentVarAddress;
                myFunc.find(procFuncName)->parameterSize++;
                myFunc.find(procFuncName)->parameter[pIdx] = nameParam;
                pIdx = pIdx + 1;
                int i = 0;
                int counter = 0;


                currentVarAddress = currentVarAddress + 1;


            }
            varFlag = 0;
        } else if (flagRecord == 0 && flagArray == 0 && varFlag == 0 && funcProcedFlag == 0) {

            string type = *name_;
            int size = 1;
            my_st.insertf(currentVarName, currentVarAddress, type, size, procFuncName, -1);
            myFunc.find(procFuncName)->pcounter++;
            currentVarAddress = currentVarAddress + 1;
        } else if (flagRecord == 1 && flagArray == 0 && addressTypeFlag == 0) {
            int recordListn = 1;
            string type = *name_;
            my_st.findList(currentRec)->recordLis.insertRL(currentVarName, currentVarAddress, type,
                                                           recordListn);//insert to record fields
            recSize = recSize + 1;
            myFunc.find(procFuncName)->pcounter++;
            currentVarAddress = currentVarAddress + 1;
            my_st.findList(currentRec)->size = recSize;
            flagRecord = 0;
            currentVarName = "";

        } else if (flagArray == 1) {

            if (flagRecord == 0) {
                string type_of_cells = *name_;
                int size_of_type = 1;
                int size = 0;
                size = myFunc.find(procFuncName)->pcounter;
                int totalSize = my_st.calcArrSize(currentVarName, size_of_type);
                my_st.cellAdd(currentVarName, size_of_type, type_of_cells);
                myFunc.find(procFuncName)->pcounter = size + totalSize;
                currentVarAddress = currentVarAddress + totalSize;
                flagArray = 0;
            }
            if (flagRecord == 1) {
                string type_of_cells = *name_;
                int size_of_type = 1;

                int totalSize = my_st.findList(currentRec)->recordLis.calcArrSize_rec(currentVarName, size_of_type);
                my_st.findList(currentRec)->recordLis.add_cell_info_rec(currentVarName, size_of_type, type_of_cells);

                currentVarAddress = currentVarAddress + totalSize;

                recSize = recSize + totalSize;

                my_st.findList(currentRec)->size = recSize;

                flagRecord = 0;
                flagArray = 0;
            }
        }


    }

    virtual Object *clone() const { return new SimpleType(*this); }

private:
    string *name_;
};


class IdeType : public Type {
public:
    IdeType(const char *name) {
        name_ = new string(name);
    }

    virtual ~IdeType() {
        if (name_) delete name_;
    }

    IdeType(const IdeType &s) {
        name_ = new string(*s.name_);
    }

    void print(ostream &os) {
        os << "Node name : IdeType" << endl;
    }

    void pcodegen(ostream &os) {


        if (recordRefRight == 0 && recordRefLeft == 0 && arrayPrintNow == 0 && newFlag == 0 && ptrCount == 0 &&
            varFlag == 0 && addressTypeFlag == 0) {
            string functioname = my_st.funcname(*name_);

            int order = myFunc.orderFunc(functioname);


            if (myFunc.find(*name_) != NULL && codelrFlag == "codel") {

                os << "lda 0 0" << endl;

            } else if (my_st.find(*name_) != -1 && order != -1) {

                os << "lda " << order - ordFlag << " " << my_st.find(*name_) << endl;


                if (my_st.findList(*name_)->ref == 1 && procFuncSt == 0) {

                    os << "ind" << endl;
                }


            }


            if (procFuncSt == 1) {

                int flag6 = 0;

                if (myFunc.find(procFuncName) == 0 && myFunc.find(*name_) == 0) {
                    flag6 = 1;
                    procFuncName = my_st.findList(procFuncName)->typed;

                }


                if (myFunc.find(*name_) == 0 && procFuncName != progName) {


                    if (my_st.findList(myFunc.find(procFuncName)->parameter[paramCount])->ref == 0 && paramCount >= 0 &&
                        my_st.findList(*name_)->ref != 1) {


                        os << "ind" << endl;
                    }


                    if (my_st.findList(myFunc.find(procFuncName)->parameter[paramCount])->ref == 0 && paramCount >= 0 &&
                        my_st.findList(*name_)->ref == 1) {
                        os << "ind" << endl;
                        os << "ind" << endl;
                    }
                    if (my_st.findList(myFunc.find(procFuncName)->parameter[paramCount])->ref != 0 && paramCount >= 0 &&
                        my_st.findList(*name_)->ref == 1) {
                        os << "ind" << endl;
                    }

                    paramCount++;
                }


                if (myFunc.find(*name_) != 0) {
                    os << "ldc " << *name_ << endl;
                    int mstp = 1;

                    string fatherf = procFuncCall;
                    while (fatherf != progName) {

                        if (*name_ == fatherf) {
                            break;
                        }
                        fatherf = myFunc.find(fatherf)->static_link;
                        mstp++;
                    }

                    if (fatherf == progName) {
                        mstp = 0;
                    }

                    os << "lda " << mstp << "  0" << endl;
                }
                flag6 = 0;


            }


            if (codelrFlag == "coder" && procFuncSt == 0) {


                os << "ind" << endl;
            }


            codelrFlag = "coder";

        } else if (varFlag == 1) {
            int size = 0;
            if (flagRecord == 0 && recordRefRight == 0 && recordRefLeft == 0 && myFunc.find(*name_) != NULL) {
                size = myFunc.find(*name_)->parameterSize;
                myFunc.find(procFuncName)->parameterSize = myFunc.find(procFuncName)->parameterSize + size;
                my_st.insertf(nameParam, currentVarAddress, *name_, size, procFuncName, -1);
                myFunc.find(procFuncName)->pcounter = myFunc.find(procFuncName)->pcounter + 2;
                myFunc.find(procFuncName)->parameter[pIdx] = *name_;
                currentVarAddress = currentVarAddress + 1;
            } else if (my_st.findList(*name_) != NULL) {
                size = my_st.findList(*name_)->size;
                myFunc.find(procFuncName)->parameterSize = myFunc.find(procFuncName)->parameterSize + size;
                my_st.insertf(nameParam, currentVarAddress, *name_, size, procFuncName, -1);
                myFunc.find(procFuncName)->pcounter = myFunc.find(procFuncName)->pcounter + size;
                myFunc.find(procFuncName)->parameter[pIdx] = *name_;
                currentVarAddress = currentVarAddress + 1;
            }
            pIdx = pIdx + 1;
            varFlag = 0;
        } else if (addressTypeFlag == 1 && flagRecord == 0 && newFlag == 0 && recordRefRight == 0 &&
                   recordRefLeft == 0) {
            int pointer_at_array_declared = 0;
            if (my_st.findList(*name_) != NULL) {
                if (my_st.findList(*name_)->typed == "arrayType") {
                    pointer_at_array_declared = 1;
                }
            } else if (my_st.findList(currentRec)->recordLis.findListt(*name_) != NULL) {
                if ((my_st.findList(currentRec)->recordLis.findListt(*name_)->typed == "arrayType")) {
                    pointer_at_array_declared = 1;
                }
            }
            if (flagArray == 0 || pointer_at_array_declared == 1) {
                int pointed_at_size = my_st.findList(*name_)->size;
                int size = 1;
                string name = currentVarName;
                string namepoint = *name_;
                my_st.insert_Ptr(currentVarName, currentVarAddress, *name_, size, namepoint, pointed_at_size);
                pointer_at_array_declared = 0;

            } else if (flagArray == 1 && pointer_at_array_declared == 0) {
                //update cells info
                int size_of_cell;
                if (my_st.findList(*name_) != NULL) {
                    size_of_cell = my_st.findList(*name_)->size;
                } else {
                    size_of_cell = my_st.findList(currentRec)->recordLis.findListt(*name_)->size;
                }
                string type_of_cell = *name_;
                my_st.cellAdd(currentVarName, size_of_cell, type_of_cell);

                my_st.set_namepoint(currentVarName);

                my_st.arrSizePtrSet(currentVarName, size_of_cell);
                flagArray = 0;
            }
            currentVarAddress = currentVarAddress + 1;
            addressTypeFlag = 0;
        } else if (addressTypeFlag == 1 && flagRecord == 1 && newFlag == 0 && recordRefRight == 0 &&
                   recordRefLeft == 0)// insert a field of list it is type is pointer
        {
            int pointer_at_array_declared = 0;
            if (my_st.findList(*name_) != NULL) {
                if (my_st.findList(*name_)->typed == "arrayType") {
                    pointer_at_array_declared = 1;
                }
            } else if (my_st.findList(currentRec)->recordLis.findListt(*name_) != NULL) {
                if ((my_st.findList(currentRec)->recordLis.findListt(*name_)->typed == "arrayType")) {
                    pointer_at_array_declared = 1;
                }
            }
            if (flagArray == 0 || pointer_at_array_declared == 1) {

                int pointed_at_size = -1;
                if (my_st.findList(*name_) == NULL) {
                    pointed_at_size = my_st.findList(currentRec)->recordLis.findListt(*name_)->size;
                } else {
                    pointed_at_size = my_st.findList(*name_)->size;
                }
                int size = 1;
                string name = currentVarName;
                string namepoint = *name_;
                my_st.findList(currentRec)->recordLis.insertRLp(currentVarName, currentVarAddress, *name_, size,
                                                                namepoint, pointed_at_size);
                myFunc.find(procFuncName)->pcounter++;
                pointer_at_array_declared = 0;

            } else if (flagArray == 1 && pointer_at_array_declared == 0) {
                int size_of_cell = -1;
                if (my_st.findList(*name_) != NULL) {
                    size_of_cell = my_st.findList(*name_)->size;
                } else {
                    size_of_cell = my_st.findList(currentRec)->recordLis.findListt(*name_)->size;
                }
                string type_of_cell = *name_;
                my_st.findList(currentRec)->recordLis.add_cell_info_rec(currentVarName, size_of_cell, type_of_cell);
                my_st.findList(currentRec)->recordLis.set_namepoint(currentVarName);
                my_st.findList(currentRec)->recordLis.set_size_arr_poi(currentVarName, size_of_cell);
                flagArray = 0;
            }
            recSize = recSize + 1;
            my_st.findList(currentRec)->size = recSize;
            myFunc.find(procFuncName)->pcounter++;
            currentVarAddress = currentVarAddress + 1;
            addressTypeFlag = 0;
            flagRecord = 0;
        } else if (flagArray == 1 && newFlag == 0 && addressTypeFlag == 0 && recordRefRight == 0 &&
                   recordRefLeft == 0) {
            if (flagRecord == 0) {
                int size_of_cell = my_st.findList(*name_)->size;
                string type_of_cell = *name_;
                int totalSize = my_st.calcArrSize(currentVarName, size_of_cell);
                my_st.cellAdd(currentVarName, size_of_cell, type_of_cell);
                currentVarAddress = currentVarAddress + totalSize;
                flagArray = 0;
            }
            if (flagRecord == 1) {
                string type_of_cells = *name_;
                int size_of_cell;
                if (my_st.findList(*name_) != NULL) {
                    size_of_cell = my_st.findList(*name_)->size;
                } else {
                    size_of_cell = my_st.findList(currentRec)->recordLis.findListt(*name_)->size;
                }
                int totalSize = my_st.findList(currentRec)->recordLis.calcArrSize_rec(currentVarName, size_of_cell);
                my_st.findList(currentRec)->recordLis.add_cell_info_rec(currentVarName, size_of_cell, type_of_cells);

                currentVarAddress = currentVarAddress + totalSize;
                recSize = recSize + totalSize;
                my_st.findList(currentRec)->size = recSize;
                myFunc.find(procFuncName)->pcounter = myFunc.find(procFuncName)->pcounter + totalSize;
                flagRecord = 0;
                flagArray = 0;
            }
        } else if (flagRecord == 1 && newFlag == 0 && addressTypeFlag == 0 && recordRefRight == 0 &&
                   recordRefLeft == 0) {
            int size = -1;
            if (my_st.findList(*name_) == NULL) {

                size = my_st.findList(currentRec)->recordLis.findListt(*name_)->size;
                string type = *name_;
                my_st.findList(currentRec)->recordLis.insertRL(currentVarName, currentVarAddress, type, size);
                currentVarAddress = currentVarAddress + size;
                recSize = recSize + size;
                myFunc.find(procFuncName)->pcounter = myFunc.find(procFuncName)->pcounter + size;
                my_st.findList(currentRec)->size = recSize;
                flagRecord = 0;
            } else {

                size = my_st.findList(*name_)->size;
                string type = *name_;
                my_st.findList(currentRec)->recordLis.insertRL(currentVarName, currentVarAddress, type, size);
                currentVarAddress = currentVarAddress + size;
                recSize = recSize + size;
                myFunc.find(procFuncName)->pcounter = myFunc.find(procFuncName)->pcounter + size;
                my_st.findList(currentRec)->size = recSize;
                flagRecord = 0;


            }


        } else if (ptrCount != 0 && recordRefRight == 0 && recordRefLeft == 0 && newFlag == 0) {
            arrName = *name_;
            int ad = my_st.findList(*name_)->address;
            os << "ldc " << ad << endl;
            for (int i = 0; i < ptrCount; i++) {
                os << "ind" << endl;
            }
            if (codelrFlag == "codel" && arrayPrintNow != 1) {
                codelrFlag = "coder";
            } else if (codelrFlag == "coder") {
                //pointerType ind if we have not sto
                os << "ind" << endl;
            }
            ptrCount = 0;

        } else if (newFlag == 1 && recordRefRight == 0 && recordRefLeft == 0) {

            int ad = my_st.findList(*name_)->address;
            int size = my_st.findList(*name_)->pointed_at_size;
            os << "ldc " << ad << endl;
            os << "ldc " << size << endl;
            os << "new " << endl;
            newFlag = 0;


        } else if (recordRefRight == 1 && recordRefLeft == 0 && newFlag == 0 && arrayPrintNow == 0) {

            if (ptrCount == 0) {

                os << "ldc " << my_st.find(*name_) << endl;
                recName = *name_;
                recordRefRight = 0;


            }
            if (ptrCount != 0) {

                os << "ldc " << my_st.find(*name_) << endl;
                for (int i = 0; i < ptrCount; i++) {
                    os << "ind" << endl;
                }
                ptrCount = 0;
                recName = my_st.findList(*name_)->typed;
                recordRefRight = 0;
            }

        } else if (recordRefLeft == 1 && newFlag == 0 && arrayPrintNow == 0) {

            recName = my_st.find_the_record_by_field(*name_);

            int order = -1;
            order = my_st.findList(recName)->recordLis.findorder(*name_);
            os << "inc " << order << endl;


            if (ptrCount != 0) {
                for (int i = 0; i < ptrCount; i++) {
                    os << "ind" << endl;
                }
                ptrCount = 0;

                recName = my_st.findList(recName)->recordLis.getType(*name_);

            } else {
                recName = my_st.findList(recName)->recordLis.getType(*name_);

            }

            if (recordRefRight == 1) {
                recordRefRight = 0;
            }

            if (codelrFlag == "codel" && (recName == "Integer" || recName == "Real" || recName == "Boolean")) {
                codelrFlag = "coder";
            } else if (codelrFlag == "coder" && (recName == "Integer" || recName == "Real" || recName == "Boolean")) {
                os << "ind" << endl;
            }
            recordRefLeft = 0;

        } else if (arrayPrintNow == 1) {

            if (recordRefLeft == 0) {
                arrName = *name_;

                os << "ldc " << my_st.find(*name_) << endl;

                if (ptrCount != 0) {
                    for (int i = 0; i < ptrCount; i++) {
                        os << "ind" << endl;
                    }

                    if (my_st.findList(*name_) != NULL) {
                        if ((my_st.findList(*name_)->pointers) != "is not declared before") {
                            arrName = my_st.findList(*name_)->pointers;
                        }
                    }

                    ptrCount = 0;
                }
                arrayPrintNow = 0;

            }
            if (recordRefLeft == 1) {
                arrName = *name_;
                int order = -1;
                if (recName != "null")
                    order = my_st.findList(recName)->recordLis.findorder(*name_);
                os << "inc " << order << endl;
                if (ptrCount != 0) {
                    for (int i = 0; i < ptrCount; i++) {
                        os << "ind" << endl;
                    }
                    if (my_st.findList(recName)->recordLis.findListt(*name_) != NULL) {
                        if ((my_st.findList(recName)->recordLis.findListt(*name_)->pointer) !=
                            "is not declared before") {
                            arrName = my_st.findList(recName)->recordLis.findListt(*name_)->pointer;
                        }
                    }
                    ptrCount = 0;
                }
                arrayPrintNow = 0;

            }

        } else if (recordRefRight == 1 && newFlag == 1 && recordRefLeft == 0) {

            int ad = my_st.findList(*name_)->address;
            os << "ldc " << ad << endl;
            if (ptrCount != 0) {
                for (int i = 0; i < ptrCount; i++) {

                    os << "ind" << endl;
                }
                ptrCount = 0;
            }

            ptrName = my_st.findList(*name_)->typed;

            recName = my_st.findList(*name_)->typed;

            recordRefRight = 0;
        } else if (recordRefLeft == 1 && newFlag == 1) {

            int order = my_st.findList(recName)->recordLis.findorder(*name_);
            if (order != -1) {
                os << "inc " << order << endl;
            }
            if (ptrCount != 0) {
                for (int i = 0; i < ptrCount; i++) {

                    os << "ind" << endl;
                }
                ptrCount = 0;
            }

            recName = my_st.findList(recName)->recordLis.getType(*name_);


            if (recordRefRight == 0) {
                os << "ldc " << my_st.findList(ptrName)->size << endl;
                os << "new" << endl;
            }
            if (recordRefRight == 1) {
                recordRefRight = 0;
            }
            recordRefLeft = 0;

        }


    }

    virtual Object *clone() const { return new IdeType(*this); }

private:
    string *name_;
};


class ArrayType : public Type {
public:
    ArrayType(int l, int u, Object *type) : low_(l), up_(u), type_(type) { assert(type_); }

    ArrayType(const ArrayType &a) : low_(a.low_), up_(a.up_) {
        type_ = a.type_->clone();
    }

    virtual ~ArrayType() {
        if (type_) delete type_;
    }

    void print(ostream &os) {
        os << "Node name : ArrayType: low bound is: " << low_ << ", up bound is: " << up_ << endl;
        assert(type_);
        type_->print(os);
    }

    void pcodegen(ostream &os) {
        assert(type_);
        flagArray = 1;

        if (flagRecord == 0) {
            string type = "arrayType";
            if (addressTypeFlag == 1) {
                type = "addressType";
            }
            my_st.insert_Arr(currentVarName, currentVarAddress, type);

            my_st.findList(currentVarName)->dimLis.insertDim(low_, up_);
        }
        if (flagRecord == 1) {
            string type = "arrayType";
            if (addressTypeFlag == 1) {
                type = "addressType";
            }
            my_st.findList(currentRec)->recordLis.insertArr_rec(currentVarName, currentVarAddress, type);
            my_st.findList(currentRec)->recordLis.findList_rec(currentVarName)->dimLis.insertDim(low_, up_);
        }

        type_->pcodegen(os);
    }

    virtual Object *clone() const { return new ArrayType(*this); }

private:
    Object *type_;
    int low_, up_;
};


class RecordType : public Type {
public:
    RecordType(Object *record_list) : record_list_(record_list) { assert(record_list_); }

    RecordType(const RecordType &y) {
        record_list_ = y.record_list_->clone();
    }

    virtual ~RecordType() {
        if (record_list_) delete record_list_;
    }

    void print(ostream &os) {
        os << "Node name : RecordType" << endl;
        assert(record_list_);
        record_list_->print(os);
    }

    void pcodegen(ostream &os) {

        assert(record_list_);
        int sizeRecord = 0;
        currentRec = currentVarName;
        recSize = 0;
        string type = "RecordType";
        my_st.insertf(currentRec, currentVarAddress, type, sizeRecord, procFuncName, -1);
        flagRecord = 1;
        record_list_->pcodegen(os);

    }

    virtual Object *clone() const { return new RecordType(*this); }

private:
    Object *record_list_;
};

class AddressType : public Type {
public:
    AddressType(Object *type) : type_(type) { assert(type_); }

    AddressType(const AddressType &t) {
        type_ = t.type_->clone();
    }

    virtual ~AddressType() {
        if (type_) delete type_;
    }

    void print(ostream &os) {
        os << "Node name : AddressType" << endl;
        assert(type_);
        type_->print(os);
    }

    void pcodegen(ostream &os) {
        assert(type_);
        addressTypeFlag = 1;
        type_->pcodegen(os);
    }

    virtual Object *clone() const { return new AddressType(*this); }

private:
    Object *type_;
};

class Declaration : public Object {
};


class VariableDeclaration : public Declaration {
public:


    VariableDeclaration(Object *type, const char *str) : type_(type) {
        assert(type_);
        name_ = new string(str);
    }

    VariableDeclaration(const VariableDeclaration &p) {
        type_ = p.type_->clone();
        name_ = new string(*p.name_);
    }

    virtual ~VariableDeclaration() {
        if (type_) delete type_;
        delete name_;
    }

    void print(ostream &os) {
        os << "Node name : VariableDeclaration. Var name is: " << *name_ << endl;
        assert(type_);
        type_->print(os);

    }

    void pcodegen(ostream &os) {
        assert(type_);
        currentVarName = *name_;
        varX = 1;
        type_->pcodegen(os);
    }


    virtual Object *clone() const { return new VariableDeclaration(*this); }

private:
    Object *type_;
    string *name_;
};


class Parameter : public Object {
public:
    Parameter(Object *type, const char *name) : type_(type) {
        assert(type_);
        name_ = new string(name);
    }

    Parameter(const Parameter &p) {
        type_ = p.type_->clone();
        name_ = new string(*p.name_);
    }

    virtual ~Parameter() {
        if (type_) delete type_;
        delete name_;
    }

    void print(ostream &os) {
        printWayOfPassing(os);
        os << "Parameter name :" << name_ << endl;
        assert(type_);
        type_->print(os);
    }

    void pcodegen(ostream &os) {
        printWayOfPassing(os);
        nameParam = *name_;

        assert(type_);

        varFlag = 1;

        type_->pcodegen(os);
    }

protected:
    virtual void printWayOfPassing(ostream &os) const = 0;

private:
    Object *type_;
    string *name_;
};

class ByReferenceParameter : public Parameter {
public:
    ByReferenceParameter(Object *type, const char *name) : Parameter(type, name) {}

    virtual Object *clone() const { return new ByReferenceParameter(*this); }

protected:
    void printWayOfPassing(ostream &os) const {

        byRefVal = "Ref";

    }
};

class ByValueParameter : public Parameter {
public:
    ByValueParameter(Object *type, const char *name) : Parameter(type, name) {}

    virtual Object *clone() const { return new ByValueParameter(*this); }

protected:
    void printWayOfPassing(ostream &os) const {


        byRefVal = "Value";
    }
};

class ParameterList : public Object {
public:
    ParameterList(Object *formal) : formal_(formal), formal_list_(NULL) { assert(formal_); }

    ParameterList(Object *formal, Object *formal_list) : formal_(formal), formal_list_(formal_list) {
        assert(formal_ && formal_list_);
    }

    ParameterList(const ParameterList &pl) {
        formal_ = pl.formal_->clone();
        formal_list_ = pl.formal_list_->clone();
    }

    virtual ~ParameterList() {
        if (formal_) delete formal_;
        if (formal_list_) delete formal_list_;
    }

    void print(ostream &os) {
        os << "Node name : ParameterList" << endl;
        assert(formal_);
        formal_->print(os);
        if (formal_list_) {
            formal_list_->print(os);
        }

    }

    void pcodegen(ostream &os) {
        prmFlag = 1;

        assert(formal_);

        formal_->pcodegen(os);
        if (formal_list_) {

            formal_list_->pcodegen(os);
        }
        pIdx = 0;


    }

    virtual Object *clone() const { return new ParameterList(*this); }

private:
    Object *formal_;
    Object *formal_list_;
};

class FunctionDeclaration : public Declaration {
public:
    FunctionDeclaration(Object *type, Object *block, const char *name) : type_(type), block_(block),
                                                                         formal_list_(NULL) {
        assert(type_ && block_);
        name_ = new string(name);
    }

    FunctionDeclaration(Object *type, Object *formal_list, Object *block, const char *name) : type_(type),
                                                                                              formal_list_(formal_list),
                                                                                              block_(block) {
        assert(type_ && formal_list_ && block_);
        name_ = new string(name);
    }

    virtual ~FunctionDeclaration() {
        if (type_) delete type_;
        if (block_) delete block_;
        if (formal_list_) delete formal_list_;
        if (name_) delete name_;
    }

    FunctionDeclaration(const FunctionDeclaration &fd) {
        type_ = fd.type_->clone();
        block_ = fd.block_->clone();
        formal_list_ = fd.formal_list_->clone();
        name_ = new string(*fd.name_);
    }

    void print(ostream &os) {
        os << "Node name : FunctionDeclaration. Func name is: " << *name_ << endl;
        assert(type_ && block_);
        type_->print(os);
        if (formal_list_) {
            formal_list_->print(os);
        }
        block_->print(os);
    }

    void pcodegen(ostream &os) {
        assert(type_ && block_);
        currentVarAddress = 5;
        int flag1 = 0;
        if (blockFlag == 0) {
            int num = 50;
            os << procFuncName << ":" << endl;
            os << "ssp " << myFunc.find(procFuncName)->pcounter << endl;
            os << "sep " << num << endl;
            procFuncEnd = "stp";
            os << "ujp " << procFuncName << "_begin" << endl;
            myFunc.find(procFuncName)->mst = currentMst;
            currentMst++;
            currentVarAddress = 5;
            ordSecFlag++;
            flag1 = 1;


        }
        finFlag = 0;
        currentMst = 0;
        blockFlag = 0;
        funcProcedFlag = 1;
        myFunc.insert(*name_);
        myFunc.find(*name_)->static_link = procFuncName;
        //currentMst = currentMst + 1;
        //myFunc.find(*name_)->mst = currentMst;
        procFuncCall = *name_;

        progFlag = "func";
        myFunc.find(*name_)->endp = "retf";
        procFuncName = *name_;
        type_->pcodegen(os);
        if (formal_list_) {
            formal_list_->pcodegen(os);
        }
        block_->pcodegen(os);
    }

    virtual Object *clone() const { return new FunctionDeclaration(*this); }

private:
    Object *type_;
    Object *block_;
    Object *formal_list_;
    string *name_;
};

class ProcedureDeclaration : public Declaration {
public:
    ProcedureDeclaration(Object *block, const char *name) : formal_list_(NULL), block_(block) {
        assert(block_);
        name_ = new string(name);
    }

    ProcedureDeclaration(Object *formal_list, Object *block, const char *name) : formal_list_(formal_list),
                                                                                 block_(block) {
        assert(formal_list_ && block_);
        name_ = new string(name);
    }

    virtual ~ProcedureDeclaration() {
        if (block_) delete block_;
        if (formal_list_) delete formal_list_;
        if (name_) delete name_;
    }

    ProcedureDeclaration(const ProcedureDeclaration &pd) {
        block_ = pd.block_->clone();
        formal_list_ = pd.formal_list_->clone();
        name_ = new string(*pd.name_);
    }

    void print(ostream &os) {
        os << "Node name : ProcedureDeclaration. Proc name is: " << *name_ << endl;
        assert(block_);
        if (formal_list_) {
            formal_list_->print(os);
        }
        block_->print(os);
    }

    void pcodegen(ostream &os) {
        assert(block_);
        currentVarAddress = 5;
        int flag1 = 0;
        finFlag = 0;
        if (blockFlag == 0) {
            int num = 50;
            os << procFuncName << ":" << endl;
            os << "ssp " << myFunc.find(procFuncName)->pcounter << endl;
            os << "sep " << num << endl;
            procFuncEnd = "stp";
            os << "ujp " << procFuncName << "_begin" << endl;
            myFunc.find(procFuncName)->mst = currentMst;
            currentMst++;
            ordSecFlag++;
            currentVarAddress = 5;
            flag1 = 1;


        }

        blockFlag = 0;
        currentMst = 0;

        myFunc.insert(*name_);
        myFunc.find(*name_)->static_link = procFuncName;
        procFuncCall = *name_;

        progFlag = "proc";
        myFunc.find(*name_)->endp = "retp";
        procFuncName = *name_;

        if (formal_list_) {

            formal_list_->pcodegen(os);


        }

        block_->pcodegen(os);


    }

    virtual Object *clone() const { return new ProcedureDeclaration(*this); }

private:
    Object *block_;
    Object *formal_list_;
    string *name_;
};

class DeclarationList : public Object {
public:
    DeclarationList(Object *decl) : decl_(decl), decl_list_(NULL) { assert(decl_); }

    DeclarationList(Object *decl_list, Object *decl) : decl_list_(decl_list), decl_(decl) {
        assert(decl_list_ && decl);
    }

    DeclarationList(const DeclarationList &dl) {
        decl_ = dl.decl_->clone();
        decl_list_ = dl.decl_list_->clone();
    }

    virtual ~DeclarationList() {
        if (decl_) delete decl_;
        if (decl_list_) delete decl_list_;
    }

    void print(ostream &os) {
        os << "Node name : DeclarationList" << endl;
        if (decl_list_) {
            decl_list_->print(os);
        }
        assert(decl_);
        decl_->print(os);
    }

    void pcodegen(ostream &os) {
        if (decl_list_) {
            decl_list_->pcodegen(os);
        }
        assert(decl_);
        decl_->pcodegen(os);
    }

    virtual Object *clone() const { return new DeclarationList(*this); }

private:
    Object *decl_;
    Object *decl_list_;
};

class Block : public Object {
public:
    Block(Object *stat_seq) : stat_seq_(stat_seq), decl_list_(NULL) { assert(stat_seq_); }

    Block(Object *decl_list, Object *stat_seq) : decl_list_(decl_list), stat_seq_(stat_seq) {
        assert(decl_list_ && stat_seq_);
    }

    Block(const Block &b) {
        decl_list_ = b.decl_list_->clone();
        stat_seq_ = b.stat_seq_->clone();
    }

    virtual ~Block() {
        if (stat_seq_) delete stat_seq_;
        if (decl_list_) delete decl_list_;
    }

    void print(ostream &os) {
        os << "Node name : Begin" << endl;
        if (decl_list_) {
            decl_list_->print(os);
        }
        assert(stat_seq_);
        stat_seq_->print(os);
    }

    void pcodegen(ostream &os) {

        int num = 50;

        if (decl_list_) {
            blockFlag = 0;
            decl_list_->pcodegen(os);


        }
        if (finFlag == 0) {

            os << procFuncName << ":" << endl;
            int num = 50;
            os << "ssp " << myFunc.find(procFuncName)->pcounter << endl;
            os << "sep " << num << endl;
            os << "ujp " << procFuncName << "_begin" << endl;
            myFunc.find(procFuncName)->mst = currentMst;
            currentMst++;

            blockFlag = 1;
            finFlag = 1;
            ordSecFlag;


        }


        assert(stat_seq_);
        os << procFuncCall << "_begin:" << endl;

        stat_seq_->pcodegen(os);

        os << myFunc.find(procFuncCall)->endp << endl;
        ordFlag = ordFlag + 1;

        procFuncCall = myFunc.find(procFuncCall)->static_link;


    }

    virtual Object *clone() const { return new Block(*this); }

private:
    Object *decl_list_;
    Object *stat_seq_;
};

class Program : public Object {
public:
    Program(Object *block, const char *str) : block_(NULL) {
        block_ = dynamic_cast<Block *>(block);
        assert(block_);
        name_ = new string(str);
    }

    Program(const Program &prog) {
        block_ = dynamic_cast<Block *>(prog.block_->clone());
        assert(block_);
        name_ = new string(*prog.name_);
    }

    virtual ~Program() {
        if (block_) delete block_;
        delete name_;
    }

    void print(ostream &os) {
        os << "Node name : Root/Program. Program name is: " << *name_ << endl;
        assert(block_);
        block_->print(os);
    }

    void pcodegen(ostream &os) {
        assert(block_);
        progName = *name_;
        myFunc.insert(*name_);
        myFunc.find(*name_)->endp = "stp";
        myFunc.find(*name_)->mst = currentMst;
        myFunc.find(*name_)->static_link = *name_;
        currentVarAddress = 5;
        progFlag = "prog";
        procFuncName = *name_;
        block_->pcodegen(os);


    }

    virtual Object *clone() const { return new Program(*this); }

private:
    Block *block_;
    string *name_;
};


#endif //AST_H