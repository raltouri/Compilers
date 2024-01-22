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
#include <stack>
#include <map>
#include <vector>
#include <typeinfo>
#include <iomanip>


using std::map;
using namespace std;
using std::string;
using std::make_pair;

extern int startAddress;
extern int codelr; /// 0 if codel 1 if coder
extern int rightSon;/// 0 if right son is constant number 1 else
extern int rightSonPremitive;
extern int lastOp;
extern int iflag;
extern int loopflag;
extern int switchflag;
extern int caseflag;

extern int arrayRefFlag;
extern int varDecFlag;
extern int currentDim;

extern string lastVar;
extern string lastType;

/// for record
extern int recRefFlag;
extern int recRefLeftFlag;
extern int recRefRightFlag;
extern int recTypeFlag;
///

/// for adressType
extern int addressTypeFlag;
extern int addressRefFlag;
///
extern stack<int> currentSwitch;

/**
 * classes 
 */

// base class for tree nodes
class Object {
public:
    virtual void print(ostream &os) = 0;

    virtual void pcodegen(ostream &os) = 0;

    virtual Object *clone() const { return NULL; }

    virtual ~Object() {}
};

class Variable {
    string id;
    string type;
    int size;    /// how many memory a variable holds
    int address;    /// address of variable
public:
    Variable(string &identifier, string &type, int size, int address) {
        this->id = identifier;
        this->type = type; //////////
        this->size = size;
        this->address = address;

        ///this->print();
    }

    int getAdress() {
        return this->address;
    }

    int getSize() {
        return this->size;
    }

    void updateSize(int size) {
        this->size = size;
    }

    void updateAddress(int adress) {
        this->address = adress;
    }

    void updateType(string type) {
        this->type = type;
    }

    string &getType() {
        return this->type;
    }

    void print() {
        cout << "Var Identifier's Name:" << id
             << "\nType:" << type
             << "\nSize: " << size
             << "\nAddress: " << address << endl;
    }

};

class arrInfo {
public:
    int cellSize;
    int subpart;
    vector<int> lowerBounds;
    vector<int> upperBounds;
    vector<int> dims;
    vector<int> dimsMul;

    string cellType;

    arrInfo() {
        cellSize = 1;
        subpart = 1;
        cellType = "";
    }

    arrInfo(const arrInfo &target) {
        this->cellSize = target.cellSize;
        this->cellType = target.cellType;
        this->subpart = target.subpart;
        this->dims = target.dims;
        this->lowerBounds = target.lowerBounds;
        this->upperBounds = target.upperBounds;
        this->dimsMul = target.dimsMul;
    }

    void calcSubpart() {
        int subp = 0;
        this->dimsMul = this->dims;
        for (int i = this->dimsMul.size() - 2; i >= 0; --i) {
            this->dimsMul[i] = this->dimsMul[i] * this->dimsMul[i + 1];
        }
        this->dimsMul.push_back(1);
        for (int i = 0; i < this->lowerBounds.size(); ++i) {
            subp += this->lowerBounds[i] * this->dimsMul[i + 1];
        }
        this->subpart = subp * this->cellSize;
    }
};

class recInfo {
public:
    map<string, int> offset;
    int currentSize;
    map <string, Variable> recST;
    map <string, arrInfo> recarrsInfo;

    recInfo() {
        currentSize = 0;
    }

    recInfo(const recInfo &target) {
        this->offset = target.offset;
        this->currentSize = target.currentSize;
        this->recST = target.recST;
        this->recarrsInfo = target.recarrsInfo;
    }
};

extern map <string, recInfo> recsInfo;
extern map <string, Variable> recFatherST; /// symbol table holds the original
/// ST when we switch STs in RecordType declaration
extern map <string, arrInfo> recFatherarrsInfo; /// same mission like last one

extern map <string, Variable> addressST;

extern map <string, Variable> ST; /// empty symbol table

extern map <string, arrInfo> arrsInfo;

class Expr : public Object {
public :
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

            switch (op_) {
                case 286:
                    os << "inc" << endl;
                    break;
                case 287:
                    os << "neg" << endl;
                    break;
            }
        } else {
            assert(left_ && right_);

            /**string rightype = typeid(right_).name();
            os<<"right son type is "<<right_<<endl;*/
            /// DEAL WITH CASE RIGHT SON IS CONSTANT NUMBER AND PRINT INC/DEC "NUM"

            rightSon = 0;
            right_->pcodegen(os);

            lastOp = op_;/// maybe wrong place


            rightSon = 1;
            left_->pcodegen(os);


            switch (op_) {
                case 286:
                    if (rightSonPremitive != 1) {
                        /**os << "inc" << endl;
                    }else {*/
                        os << "add" << endl;
                    }
                    break;
                case 287:
                    if (rightSonPremitive != 1) {
                        os << "sub" << endl;
                    }
                    break;
                case 288:
                    os << "mul" << endl;
                    break;
                case 289:
                    os << "div" << endl;
                    break;
                case 292:
                    os << "les" << endl;
                    break;
                case 293:
                    os << "leq" << endl;
                    break;
                case 294:
                    os << "equ" << endl;
                    break;
                case 295:
                    os << "neq" << endl;
                    break;
                case 296:
                    os << "grt" << endl;
                    break;
                case 297:
                    os << "geq" << endl;
                    break;
                case 298:
                    os << "and" << endl;
                    break;
                case 299:
                    os << "or" << endl;
                    break;
            }
            rightSon = 0;
            rightSonPremitive = 0;
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
public :
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
        exp_->pcodegen(os);

        auto itr = arrsInfo.find(lastVar);
        string tmplastVar = lastVar;
        string tmpType = itr->second.cellType;
        int tmpCurrentDim = currentDim;
        int access_shift = 0;

        auto tmpcell = ST.find(tmpType);
        if (currentDim > itr->second.dims.size() - 1) {
            ///os<<"dec "<<itr->second.subpart<<endl;

            ///os<<"current var name: "<<lastVar<<", current dim is :"<<currentDim<<endl;
            if (tmpcell != ST.end()) {
                if (tmpcell->second.getType() == "Array") {/// add support for record and pointer
                    lastVar = tmpType;
                    currentDim = 0;
                    itr = arrsInfo.find(lastVar);
                }
            }
            ///os<<"next var name: "<<lastVar<<", updated dim is :"<<currentDim<<endl;
        }


        access_shift = itr->second.dimsMul[currentDim + 1] * itr->second.cellSize;
        os << "ixa " << access_shift << endl;
        currentDim++;

        if (currentDim > itr->second.dims.size() - 1) {
            os << "dec " << itr->second.subpart << endl;
        }
        if (dim_) {
            dim_->pcodegen(os);
        }

        lastVar = tmplastVar;
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
        ///os<<"lastOp is:"<<lastOp<<endl;
        if (rightSon == 1 && lastOp == 286) {
            rightSonPremitive = 1;
            os << "inc " << i_ << endl;
            return;
        }
        if (rightSon == 1 && lastOp == 287) {
            rightSonPremitive = 1;
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
        ///os << std::fixed << std::setprecision(1) << "ldc " << r_ << endl;

        if (rightSon == 1 && lastOp == 286) {
            rightSonPremitive = 1;
            os << std::fixed << std::setprecision(1) << "inc " << r_ << endl;
            return;
        }
        if (rightSon == 1 && lastOp == 287) {
            rightSonPremitive = 1;
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
public :
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
public :
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

        int tmparrayref = arrayRefFlag;
        int tmpDim = currentDim;
        string tmpLastType = lastType;
        string tmpLastVar = lastVar;

        arrayRefFlag = 1;
        lastType = "ArrayRef";

        var_->pcodegen(os);
        dim_->pcodegen(os);

        if (codelr == 1) {
            os << "ind" << endl;
        }

        lastType = tmpLastType;
        currentDim = tmpDim;
        arrayRefFlag = tmparrayref;
        lastVar = tmpLastVar;
    }

    virtual Object *clone() const { return new ArrayRef(*this); }

private:
    Object *var_;
    Object *dim_;
};

class RecordRef : public Var {
public :
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

        int tmprecFlag = recRefFlag;
        recRefFlag = 1;

        if (recRefLeftFlag == 0) {
            recRefLeftFlag = 1;
        }
        varExt_->pcodegen(os);
        recRefLeftFlag = 0;

        auto recItr = recsInfo.find(lastVar);
        map <string, Variable> tmpST = ST;
        ST = recItr->second.recST;
        map <string, arrInfo> tmparrsInfo = arrsInfo;
        arrsInfo = recItr->second.recarrsInfo;

        recRefRightFlag = 1;
        varIn_->pcodegen(os);
        recRefRightFlag = 0;

        ST = tmpST;
        arrsInfo = tmparrsInfo;
        recRefFlag = tmprecFlag;
    }

    virtual Object *clone() const { return new RecordRef(*this); }

private:
    Object *varExt_;
    Object *varIn_;
};

class AddressRef : public Var {
public :
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
        int tmpaddressRefFlag = addressRefFlag;
        addressRefFlag = 1;

        var_->pcodegen(os);

        addressRefFlag = tmpaddressRefFlag;
    }

    virtual Object *clone() { return new AddressRef(*this); }

private:
    Object *var_;
};

class Statement : public Object {
};

class NewStatement : public Statement {
public :
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
        var_->pcodegen(os);

        os << "new" << endl;
    }

    virtual Object *clone() { return new NewStatement(*this); }

private:
    Object *var_;
};

class WriteStrStatement : public Statement {
public :
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
public :
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
public :
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
        if (expr_list_) {
            expr_list_->pcodegen(os);
        }
    }

    virtual Object *clone() const { return new ProcedureStatement(*this); }

private:
    Object *expr_list_;
    string *str_;
};

class Case : public Object {
public :
    Case(Object *stat_list, int val) : leafChild_(NULL), stat_list_(stat_list) {
        // note the special treatment in miny.y for this case (makenode)
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

        caseflag++;
        ///int sflag = switchflag;
        int cflag = caseflag;
        os << "case_" << cflag << "_" << currentSwitch.top() << ":" << endl;

        stat_list_->pcodegen(os);

        os << "ujp end_switch_" << currentSwitch.top() << endl;

        ///switchflag=sflag;
    }

    virtual Object *clone() const { return new Case(*this); }

private:
    Object *stat_list_;
    Object *leafChild_;
};


class CaseList : public Object {
public :
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
        case_->pcodegen(os);
        if (case_list_) {
            case_list_->pcodegen(os);
        }
    }

    virtual Object *clone() const { return new CaseList(*this); }

private:
    Object *case_;
    Object *case_list_;
};

class CaseStatement : public Statement {
public :
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
        exp_->pcodegen(os);

        int cflag = caseflag;
        switchflag++;
        ///int sflag = switchflag;

        currentSwitch.push(switchflag);

        os << "neg" << endl;
        os << "ixj end_switch_" << currentSwitch.top() << endl;

        caseflag = 0;
        case_list_->pcodegen(os);

        for (int i = caseflag; i > 0; --i) {
            os << "ujp case_" << i << "_" << currentSwitch.top() << endl;
        }
        os << "end_switch_" << currentSwitch.top() << ":" << endl;
        caseflag = cflag;
        currentSwitch.pop();
    }

    virtual Object *clone() const { return new CaseStatement(*this); }

private:
    Object *exp_;
    Object *case_list_;
};

class LoopStatement : public Statement {
public :
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
        assert(exp_ && stat_list_);

        loopflag++;
        int c = loopflag;
        os << "loop_" << c << ":" << endl;

        exp_->pcodegen(os);

        os << "fjp end_loop_" << c << endl;

        stat_list_->pcodegen(os);

        os << "ujp loop_" << c << endl;
        os << "end_loop_" << c << ":" << endl;
    }

    virtual Object *clone() const { return new LoopStatement(*this); }

private:
    Object *exp_;
    Object *stat_list_;
};


class ConditionalStatement : public Statement {
public :
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
        assert(exp_ && stat_list_if_);

        exp_->pcodegen(os);

        iflag++;
        int c = iflag;

        if (stat_list_else_) {
            os << "fjp else_if_" << c << endl;

            stat_list_if_->pcodegen(os);

            os << "ujp end_if_" << c << endl;

            os << "else_if_" << c << ":" << endl;

            stat_list_else_->pcodegen(os);

            os << "end_if_" << c << ":" << endl;
        } else {
            os << "fjp end_if_" << c << endl;

            stat_list_if_->pcodegen(os);

            os << "end_if_" << c << ":" << endl;
        }

    }

    virtual Object *clone() const { return new ConditionalStatement(*this); }

private:
    Object *exp_;
    Object *stat_list_if_;
    Object *stat_list_else_;
};


class Assign : public Statement {
public :
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
        os << "Node name : Assign" << endl;
        assert(var_ && exp_);
        exp_->print(os); /// switch order with var
        var_->print(os);


    }

    void pcodegen(ostream &os) {
        assert(var_ && exp_);

        codelr = 0; /// Review this
        exp_->pcodegen(os);

        codelr = 1; /// Review this
        var_->pcodegen(os);


        os << "sto" << endl;
    }

    virtual Object *clone() const { return new Assign(*this); }

private:
    Object *var_;
    Object *exp_;
};

class StatementList : public Object {
public :
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
public :
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
            record_list_->pcodegen(os);
        }
        assert(var_decl_);
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
        os << "Node name : SimpleType" << endl;;
        os << "Type is : " << (*name_) << endl;
    }

    void pcodegen(ostream &os) {
        /**int cellSize;
            int subpart;
            vector<int> lowerBounds;
            vector<int> upperBounds;
            vector<int> dims;*/
        if (lastType == "Array") {
            auto itr = arrsInfo.find(lastVar);
            itr->second.cellSize = 1;
            itr->second.cellType = *name_;
            itr->second.calcSubpart();
            int total_arr_size = itr->second.dimsMul[0] * itr->second.cellSize;
            auto arrSymbolTableIter = ST.find(lastVar);
            arrSymbolTableIter->second.updateType(lastType);
            arrSymbolTableIter->second.updateSize(total_arr_size);
            startAddress = arrSymbolTableIter->second.getAdress() +
                           arrSymbolTableIter->second.getSize();
            ///os <<"next free adress :"<<startAddress<<endl;

            cout << "AAAAAAAAAAAAAAAAAarray : " << lastVar << ".subpart is: " << itr->second.subpart << endl;
            cout << "array name: " << lastVar << " total size :" << arrSymbolTableIter->second.getSize() << endl;
        } else {
            lastType = *name_;
            cout << "SimpleTypellllllllllllllast var name " << lastVar << endl;
            cout << "SimpleTypellllllllllllllast var type " << lastType << endl;
        }
        lastType = *name_;
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
        if (varDecFlag == 0) {
            string *tmpname = name_;
            auto printItr = ST.find(*name_);
            auto printTargetItr = ST.find(*name_);
            int adrs = ST.find(*tmpname)->second.getAdress();
            if (addressRefFlag == 1) {
                auto nameItr = addressST.find(*name_);
                /// os<<"Poniter name: "<<*name_<<endl;
                name_ = &(nameItr->second.getType());
                ///  os<<"Points to : "<<*name_<<endl;
                ///os<<"Poniter name: "<<*tmpname<<" local Adress "<<ST.find(*tmpname)->second.getAdress()<<endl;
                printItr = ST.find(*name_);
                if (printItr == ST.end()) {
                    printItr = recFatherST.find(*name_);

                    /// os<<"Poniter name: "<<*tmpname<<" Points to "<<*name_<<endl;

                    //// danger change
                    ST = recFatherST;
                    arrsInfo = recFatherarrsInfo;
                    ///
                } else {
                    ///os<<*name_<<" and "<<*tmpname<<" are in same symboltable"<<endl;
                }
                ///danger change
                if (printItr->second.getType() == "Array") {
                    lastVar = *name_;
                }
                if (printItr->second.getType() == "RecordType") {
/*                    map<string, int> offset;
                    int currentSize;
                    map <string, Variable> recST;
                    map <string, arrInfo> recarrsInfo; */

                    ST = recsInfo.find(*name_)->second.recST;
                    ///os<<"switch ST to "<<*name_<<" recST"<<endl;
                    arrsInfo = recsInfo.find(*name_)->second.recarrsInfo;
                }
                ///
                ///os << "pointer name: "<<*tmpname<<" address in symbols tamble is: "<< ST.find(*tmpname)->second.getAdress()<<endl;
                ///os << "ide name: " << *name_ << " address is: "<< printItr->second.getAdress() << endl;
            }
            if (addressRefFlag == 1) {
                if (recRefLeftFlag == 1 && recRefRightFlag == 0) {
                    os << "ldc " << adrs << endl;
                } else {
                    os << "inc " << adrs << endl;
                }
                os << "ind" << endl;
            } else {
                if (recRefFlag == 0) {
                    if (arrayRefFlag == 0) {
                        if (codelr == 0) {
                            os << "ldc " << adrs << endl;
                        }
                        if (codelr == 1) {
                            os << "ldc " << adrs << endl;
                            os << "ind" << endl;
                        }

                    } else {
                        os << "ldc " << adrs << endl;
                    }
                } else {
                    if (recRefLeftFlag == 1 && recRefRightFlag == 0) {
                        if (codelr == 0) {
                            os << "ldc " << adrs << endl;
                        }
                        if (codelr == 1) {
                            os << "ldc " << adrs << endl;
                            os << "ind" << endl;
                        }
                    } else {
                        ///  os << "we are printing : "<<*tmpname<<" address in symbols table is: "
                        ///      << adrs<<" last Variable name:  "<<lastVar <<endl;
                        if (codelr == 0) {
                            os << "inc " << adrs << endl;
                        }
                        if (codelr == 1) {
                            os << "inc " << adrs << endl;
                            os << "ind" << endl;
                        }
                    }
                }
            }
            /*if (addressRefFlag == 1) {
                name_ = tmpname;
            }*/
        } else {
            auto itrST = ST.find(lastVar);
            auto itrArrInfo = arrsInfo.find(lastVar);
            auto itradressST = addressST.find(lastVar);

            if (varDecFlag == 1 && addressTypeFlag == 1) {
                ///auto itradressST = addressST.find(*toDelete);
                itrST = ST.find(*name_);
                if (itrST == ST.end()) {
                    ///os << "didn't found name_: " << *name_ << " in ST" << endl;
                    itrST = recFatherST.find(*name_);
                    if (itrST == recFatherST.end()) {
                        os << "didn't found name_: " << *name_ << " in fatherST" << endl;
                    }
                }
                itradressST->second.updateSize(itrST->second.getSize());
                itradressST->second.updateAddress(itrST->second.getAdress());
                itradressST->second.updateType(*name_);
                /// delete toDelete; used like this in var declare for pointer

            } else {
                if (varDecFlag == 1 && recTypeFlag == 0 && lastType == "Array") {
                    itrST->second.updateType(lastType);
                    itrArrInfo->second.cellSize = ST.find(*name_)->second.getSize();
                    itrArrInfo->second.cellType = *name_;
                    itrArrInfo->second.calcSubpart();
                    int total_arr_size = itrArrInfo->second.dimsMul[0] * itrArrInfo->second.cellSize;
                    itrST->second.updateSize(total_arr_size);
                    startAddress = itrST->second.getAdress() + itrST->second.getSize();

                } else {
                    if (varDecFlag == 1 && recTypeFlag == 0) { /// simple type or record
                        auto complexTypeItrST = ST.find(*name_);
                        auto complexitrArrInfo = arrsInfo.find(*name_);

                        string type = complexTypeItrST->second.getType();
                        itrST->second.updateType(type);
                        int sizze = complexTypeItrST->second.getSize();
                        itrST->second.updateSize(sizze);

                        if (type == "RecordType") {
                            recInfo localRecInfo = recsInfo.find(*name_)->second;
                            recsInfo.insert(make_pair(lastVar, localRecInfo));
                        }
                        if (type == "Array") {
                            arrInfo arrCopy = complexitrArrInfo->second;
                            arrsInfo.insert(make_pair(lastVar, arrCopy));
                        }
                    }
                    if (varDecFlag == 1 && recTypeFlag == 1) {
                        /// map <string, Variable> recFatherST;
                        /// map <string, arrInfo> recFatherarrsInfo;

                        auto complexTypeItrST = ST.find(*name_);
                        auto complexitrArrInfo = arrsInfo.find(*name_);

                        if (complexTypeItrST == ST.end()) {
                            complexTypeItrST = recFatherST.find(*name_);
                            complexitrArrInfo = recFatherarrsInfo.find(*name_);
/*
                    os<<"ComplexType name is :"<<*name_<<". Type is :"<<complexTypeItrST->second.getType()
                                            <<". Size is :"<<complexTypeItrST->second.getSize()<<endl;
*/
                        }
                        string type = complexTypeItrST->second.getType();
                        ///os<<"Declaring name is :"<<lastVar<<". Type is :"<<type<<endl;
                        itrST->second.updateType(type);
                        ///os<<"Declaring name is :"<<lastVar<<". Type is :"<<type<<endl;
                        int sizze = complexTypeItrST->second.getSize();
                        if (lastType != "Array") {
                            itrST->second.updateSize(sizze);
                            ///os << "Declaring name is :" << lastVar << ". Type is :" << type << ". Size is :" << sizze << endl;
                        }
                        if (lastType == "Array") {
                            /*auto itrST = ST.find(lastVar);
                            auto itrArrInfo = arrsInfo.find(lastVar);
                            */
                            itrST->second.updateType(lastType);
                            itrArrInfo->second.cellSize = complexTypeItrST->second.getSize();
                            itrArrInfo->second.cellType = *name_;
                            itrArrInfo->second.calcSubpart();
                            int total_arr_size = itrArrInfo->second.dimsMul[0] * itrArrInfo->second.cellSize;
                            /*os << "Declaring name is :" << lastVar << ". Type is :" <<
                                        type << ". total Size is :" << total_arr_size <<
                                        ". DimsMul[0]="<<itrArrInfo->second.dimsMul[0]
                                        <<"upperBound[0]="<<itrArrInfo->second.upperBounds[0]
                                    <<"lowerBound[0]="<<itrArrInfo->second.lowerBounds[0]<< endl;*/
                            itrST->second.updateSize(total_arr_size);

                        } else {
                            string type = complexTypeItrST->second.getType();
                            itrST->second.updateType(type);
                            int sizze = complexTypeItrST->second.getSize();
                            itrST->second.updateSize(sizze);

                            if (type == "RecordType") {
                                recInfo localRecInfo = recsInfo.find(*name_)->second;
                                recsInfo.insert(make_pair(lastVar, localRecInfo));
                            }
                            if (type == "Array") {
                                arrInfo arrCopy = complexitrArrInfo->second;
                                arrsInfo.insert(make_pair(lastVar, arrCopy));
                            }
                        }

                        startAddress = itrST->second.getAdress() + itrST->second.getSize();
                    }
                }
            }
        }
        lastVar = *name_;
    }

    virtual Object *clone() const { return new IdeType(*this); }

private:
    string *name_;
};

class ArrayType : public Type {
public :
    ArrayType(int l, int u, Object *type) : low_(l), up_(u), type_(type) {
        assert(type_);
    }

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


        string tmp = lastType;

        lastType = "Array";
        auto itr = arrsInfo.find(lastVar);
        cout << "llllllllllllllllllllllast var is " << lastVar << endl;
        cout << "llllllllllllllllllllllast var type " << lastType << endl;
        if (itr == arrsInfo.end()) {
            /**int cellSize;
            int subpart;
            vector<int> lowerBounds;
            vector<int> upperBounds;
            vector<int> dims;*/
            ///arrInfo tmp();
            arrsInfo.insert(std::pair<string, arrInfo>(lastVar, arrInfo()));
            itr = arrsInfo.find(lastVar);
        }
        itr->second.lowerBounds.push_back(low_);
        itr->second.upperBounds.push_back(up_);
        itr->second.dims.push_back(up_ - low_ + 1);


        type_->pcodegen(os);

        ///os<<"current variable name :"<<lastVar<<" .Adress :"<<ST.find(lastVar)->second.getAdress()<<endl;
        lastType = tmp;
    }

    virtual Object *clone() const { return new ArrayType(*this); }

private:
    Object *type_;
    int low_, up_;
};

class RecordType : public Type {
public :
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

        int size = 0;//// calc this for current record
        int tmpflag = recTypeFlag;
        recTypeFlag = 1;
        auto itr = ST.find(lastVar);
        ///if(varDecFlag==1){

        ///ST.insert(make_pair(*name_, v));
        itr->second.updateType("RecordType");
        recsInfo.insert(std::pair<string, recInfo>(lastVar, recInfo()));

        ///}
        /// backup block switching symbol table and arrays tables
        int tmpstartAdress = startAddress;
        startAddress = 0;

        string tmplastVar = lastVar;

        recFatherST = ST;///new
        recFatherarrsInfo = arrsInfo;///neew

        map <string, Variable> tmpST = ST;
        ST = map<string, Variable>();
        map <string, arrInfo> tmparrsInfo = arrsInfo;
        arrsInfo = map<string, arrInfo>();
        ///

        record_list_->pcodegen(os);

        /// restore Symbols table and update record
        lastVar = tmplastVar;

        auto recInfoItr = recsInfo.find(lastVar);
        recInfoItr->second.recST = ST;
        recInfoItr->second.recarrsInfo = arrsInfo;
        recInfoItr->second.currentSize = startAddress;

        ST = tmpST;
        arrsInfo = tmparrsInfo;
        ///startAddress = tmpstartAdress;

        itr = ST.find(lastVar);

        size = recsInfo.find(lastVar)->second.currentSize;
        ///os<<"current record name: "<<lastVar<<". size: "<<size<<endl;
        itr->second.updateSize(size);
        startAddress = itr->second.getAdress() + itr->second.getSize();

        recFatherST = ST;///new
        recFatherarrsInfo = arrsInfo;///neew

        recTypeFlag = tmpflag;
        /// check if we need to use recsInfo.ofssets map cause recST already saves every var adrress(local offset)
    }

    virtual Object *clone() const { return new RecordType(*this); }

private:
    Object *record_list_;
};


class AddressType : public Type {
public :
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
        /// this pointer name is last variable
        int tmpaddressTypeFlag = addressTypeFlag;
        addressTypeFlag = 1;
        auto itrST = ST.find(lastVar);
        itrST->second.updateType("AddressType");
        string tmplastVar = lastVar;
        string tp = "AdressType";
        Variable pointr(lastVar, tp, 1, 0); ///check adress and other parameteres
        addressST.insert(std::pair<string, Variable>(lastVar, pointr));

        type_->pcodegen(os); /// will alloccate new memory if the type isn't ide name

        addressTypeFlag = tmpaddressTypeFlag;
        lastVar = tmplastVar;
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

        /*string tp = typeid(type_).name();
        ///string *tp = (string*) type;

        int size = sizeof(type);
        Variable v(*name_, tp, 1, startAddress);
        ////v.print(); /// new
        ++startAddress;
        ST.insert(make_pair(*name_, v));

        lastVar = *name_; /// Catching the latest variable name*/

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

        string tp = typeid(type_).name();
        ///string *tp = (string*) type;
        string *toDelete;
        if (addressTypeFlag == 1) {
            toDelete = name_;
            name_ = new string(*name_ + "^");
            ///delete toDelete;
        }
        Variable v(*name_, tp, 1, startAddress);
        ////v.print(); /// new
        ++startAddress;
        ST.insert(make_pair(*name_, v));

        lastVar = *name_; /// Catching the latest variable name

        int tmp = varDecFlag;
        varDecFlag = 1;
        type_->pcodegen(os);
        varDecFlag = tmp;

        if (addressTypeFlag == 1) {
            auto itradressST = addressST.find(*toDelete);
            auto itrST = ST.find(*name_);
            itradressST->second.updateSize(itrST->second.getSize());
            itradressST->second.updateAddress(itrST->second.getAdress());
            itradressST->second.updateType(*name_);
            delete toDelete;
        }
    }

    virtual Object *clone() const { return new VariableDeclaration(*this); }

private:
    Object *type_;
    string *name_;
};

class Parameter : public Object {
public :
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
        assert(type_);
        type_->pcodegen(os);
    }

protected:
    virtual void printWayOfPassing(ostream &os) const = 0;

private:
    Object *type_;
    string *name_;
};

class ByReferenceParameter : public Parameter {
public :
    ByReferenceParameter(Object *type, const char *name) : Parameter(type, name) {}

    virtual Object *clone() const { return new ByReferenceParameter(*this); }

protected:
    void printWayOfPassing(ostream &os) const {
        os << "Node name : ByReferenceParameter ";
    }
};

class ByValueParameter : public Parameter {
public :
    ByValueParameter(Object *type, const char *name) : Parameter(type, name) {}

    virtual Object *clone() const { return new ByValueParameter(*this); }

protected:
    void printWayOfPassing(ostream &os) const {
        os << "Node name : ByValueParameter ";
    }
};

class ParameterList : public Object {
public :
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
        if (formal_list_) {
            formal_list_->print(os);
        }
        assert(formal_);
        formal_->print(os);
    }

    void pcodegen(ostream &os) {
        if (formal_list_) {
            formal_list_->pcodegen(os);
        }
        assert(formal_);
        formal_->pcodegen(os);
    }

    virtual Object *clone() const { return new ParameterList(*this); }

private:
    Object *formal_;
    Object *formal_list_;
};

class FunctionDeclaration : public Declaration {
public :
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
public :
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
public :
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
public :
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
        if (decl_list_) {
            decl_list_->pcodegen(os);
        }
        assert(stat_seq_);
        stat_seq_->pcodegen(os);
    }

    virtual Object *clone() const { return new Block(*this); }

private:
    Object *decl_list_;
    Object *stat_seq_;
};

class Program : public Object {
public :
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
        block_->pcodegen(os);
    }

    virtual Object *clone() const { return new Program(*this); }

private:
    Block *block_;
    string *name_;
};


#endif //AST_H