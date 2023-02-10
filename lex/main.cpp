#include <iostream>
#include <vector>
#include <stack>
#include <cstring>
#include <string>
#include <ctype.h>
#include <algorithm>

using namespace std;


enum TypeOfLex
{
  LEX_NULL,                                                               /*0*/
  LEX_AND,   LEX_BREAK,  LEX_BOOL,     LEX_DO,    LEX_ELSE,    LEX_IF,    /*6*/
  LEX_FALSE, LEX_INT,    LEX_NOT,      LEX_OR,    LEX_PROGRAM, LEX_READ,  /*12*/
  LEX_THEN,  LEX_TRUE,   LEX_WHILE,    LEX_WRITE, LEX_STRING,  LEX_FOR,   /*18*/
  LEX_GOTO,  LEX_STRUCT,                                                  /*20*/
  LEX_FIN,                                                                /*21*/
  LEX_SEMICOLON, LEX_COMMA,   LEX_COLON,  LEX_ASSIGN,   LEX_LPAREN,       /*26*/
  LEX_RPAREN,    LEX_EQ,      LEX_LSS,    LEX_GTR,      LEX_PLUS,         /*31*/
  LEX_MINUS,     LEX_LBRACE,  LEX_RBRACE, LEX_TIMES,    LEX_SLASH,        /*36*/
  LEX_LEQ,       LEX_NEQ,     LEX_GEQ,    LEX_POINT,                      /*40*/
  LEX_NUM,                                                                /*41*/
  LEX_ID,                                                                 /*42*/
  LEX_STR,                                                                /*43*/
  POLIZ_LABEL,                                                            /*44*/ 
  POLIZ_ADDRESS,                                                          /*45*/
  POLIZ_GO,                                                               /*46*/
  POLIZ_FGO,                                                              /*47*/
  POLIZ_MINUS,                                                            /*48*/
  LEX_OBJECT                                                              /*49*/
};


class Lex
{
  public:
    Lex (TypeOfLex t = LEX_NULL, int v = -1, int v2 = -1, string s = ""):
        typeOfLex_(t), value_(v), value2_(v2), str_(s) {}
    TypeOfLex getType () const
    {
      return typeOfLex_;
    }
    int getValue () const
    {
      return value_;
    }
    int getValue2() const
    {
      return value2_;
    }
    string getStr () const
    {
      return str_;
    }
    Lex (const Lex& lex) 
    {
      typeOfLex_ = lex.typeOfLex_;
      value_ = lex.value_;
      value2_ = lex.value2_;
      str_ = lex.str_;
    }
    Lex& operator= (const Lex& lex) 
    {
      if (this == &lex) {
        return *this;
      }
      typeOfLex_ = lex.typeOfLex_;
      value_ = lex.value_;
      value2_ = lex.value2_;
      str_ = lex.str_;
      return *this;
    }
    friend ostream& operator<< (ostream& out, Lex lex);
  private:
    TypeOfLex  typeOfLex_;
    int        value_;
    int        value2_;
    string     str_;
};




class Ident
{
  public:
    Ident ()
    {
      valueTypeStruct_ = -1;
      value_ = 0;
      str_ = "";
      declare_ = false;
      assign_  = false;
      isStruct_ = false;
      isLabel_  = false;
    }
    Ident (const string n)
    {
      name_ = n;
      valueTypeStruct_ = -1;
      value_ = 0;
      str_ = "";
      declare_ = false;
      assign_ = false;
      isStruct_ = false;
      isLabel_  = false; // метка
    }
    bool operator==(const string& s) const {
      return name_ == s;
    }
    string getName () const
    {
      return name_;
    }
    bool getDeclare () const
    {
      return declare_;
    }
    void putDeclare ()
    {
      declare_ = true;
    }
    TypeOfLex getType () const
    {
     return typeOfLex_;
    }
    void putType (TypeOfLex type)
    {
      typeOfLex_ = type;
    }
    bool getAssign () const
    {
      return assign_;
    }
    void putAssign ()
    {
      assign_ = true;
    }
    int getValue () const
    {
      return value_;
    }
    string getStr() const
    {
      return str_;
    }
    void putValue (int v)
    {
      value_ = v;
    }
    void putValue (string s)
    {
      str_ = s;
    }
    bool isStruct() const
    {
      return isStruct_;
    }
    void putIsStruct()
    {
      isStruct_ = true;
    }
    void putIsLabel()
    {
      isLabel_ = true;
    }
    bool isLabel() const
    {
      return isLabel_;
    }
    void putValueStruct(int i)
    {
      valueTypeStruct_ = i;
    }
    int getValueStruct() const
    {
      return valueTypeStruct_;
    }
    Ident& operator= (Ident& ident);
    void outInfo ();
    vector<Ident> tid; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!
    

  private:
    string     name_;
    bool       declare_;
    TypeOfLex  typeOfLex_;
    bool       assign_;
    int        value_;
    string     str_;
    bool       isStruct_;
    bool       isObject_;
    bool       isLabel_;
    int        valueTypeStruct_;
};

void Ident::outInfo ()
{
  cout << "name: "                 << name_              << endl;
  cout << "declare: "              << declare_           << endl;
  cout << "typeOfLex: "            << typeOfLex_         << endl;
  cout << "assign: "               << assign_            << endl;
  cout << "value: "                << value_             << endl;
  cout << "isStruct: "             << isStruct_          << endl;
  cout << "isObject: "             << isObject_          << endl;
  cout << "isLabel: "              << isLabel_           << endl;
  cout << "value type of struct: " << valueTypeStruct_   << endl;
  if (isObject_ || isStruct_) {
    vector<Ident>::iterator p = tid.begin();
    while ( p != tid.end() ) {
      cout << "\ttype: "     << (*p).getType()    << endl;
      cout << "\tname: "     << (*p).getName()    << endl;
      cout << "\tdeclared: " << (*p).getDeclare() << endl;
      cout << endl;
      ++p;
    }
  }
  cout << endl << endl;
}

Ident& Ident::operator= (Ident& ident) 
{
  if (this == &ident) {
    return *this;
  }
  if (tid.empty() == 0) {
    tid.clear();
  }
  vector<Ident>::iterator p = ident.tid.begin();
  while (p != ident.tid.end()) {
    tid.push_back(*p);
    ++p;
  }
  return *this;
}




class Scanner
{
  public:
    Lex getLex (vector<Ident>& tid);
    Scanner (const char* program);
    void stepBack (int i) 
    {
      fseek (file_, -i, SEEK_CUR);
    }
    static const char*  TW[];
    static const char*  TD[];
    ~Scanner () 
    { 
      fclose(file_); 
    }
  private:
    FILE* file_;
    char  currSym_;
    int look (const string buf, const char** list);
    void gc ()
    {
      currSym_ = fgetc(file_);
    }
};


int Scanner::look (const string buf, const char** list)
{
  int i = 0;
  while (list[i]) {
    if (buf == list[i])
      return i;
    ++i;
  }
  return 0;
}


Scanner::Scanner (const char* program)
{
  file_ = fopen (program, "r");
  if (file_ == NULL) 
    throw "Wrong name of file!";
}


const char* Scanner::TW[] =  // Word service table
{
  "",         // 0
  "and",      // 1
  "break",    // 2
  "bool",     // 3
  "do",       // 4
  "else",     // 5
  "if",       // 6
  "false",    // 7
  "int",      // 8
  "not",      // 9
  "or",       // 10
  "program",  // 11
  "read",     // 12
  "then",     // 13
  "true",     // 14
  "while",    // 15
  "write",    // 16
  "string",   // 17
  "for",      // 18
  "goto",     // 19
  "struct",   // 20
  NULL
};


const char* Scanner::TD[] =  // Delimiter table
{
  "@",  // 0
  ";",  // 1
  ",",  // 2
  ":",  // 3
  "=",  // 4
  "(",  // 5
  ")",  // 6
  "==", // 7
  "<",  // 8
  ">",  // 9
  "+",  // 10
  "-",  // 11
  "{",  // 12
  "}",  // 13
  "*",  // 14
  "/",  // 15
  "<=", // 16
  "!=", // 17
  ">=", // 18
  ".",  // 19
  NULL
};


vector<Ident> TID;   // вектор идентификаторов


int putIdent (const string& buf, vector<Ident>& tid)
{
  vector<Ident>::iterator k;
  if ( (k = find(tid.begin(), tid.end(), buf)) != tid.end() )
    return k - tid.begin();
  tid.push_back(Ident(buf));
  return tid.size() - 1;
}


ostream& operator<< (ostream& out, Lex lex)
{
  string s;
  if ( lex.typeOfLex_ <= 20 )
    s = Scanner::TW[lex.typeOfLex_];

  else if ((lex.typeOfLex_ >= 21) && (lex.typeOfLex_ <= 40))
    s = Scanner::TD[lex.typeOfLex_ - 21];

  else if (lex.typeOfLex_ == 41)
    s = "NUM";

  else if (lex.typeOfLex_ == 42)
    s = TID [lex.value_].getName();

  else if (lex.typeOfLex_ == 43)
    s = "STR";

  else if (lex.typeOfLex_ == 44)
    s = "Label";
  
  else if (lex.typeOfLex_ == 45)
    s = "Addr";
  
  else if (lex.typeOfLex_ == 46)
    s = "!";

  else if (lex.typeOfLex_ == 47)
    s = "!F";

  else if (lex.typeOfLex_ == 48)
    s = "minus";
  
  else if (lex.typeOfLex_ == 49)
    s = "OBJECT";

  else
    throw lex;
  if (lex.value_ != -1){
    if (lex.str_ != "") {
      out << '(' << s << ',' << lex.str_ << ") ";
    }
    else if (lex.value2_ != -1) {
      out << '(' << s << ',' << lex.value_ << ',' << lex.value2_ << ") ";
    }
    else
      out << '(' << s << ',' << lex.value_ << ") ";
  } else {
    if (lex.str_ != "") {
      out << '(' << s << ',' << lex.str_ << ") ";
    }
    else if (lex.value2_ != -1) {
      out << '(' << s << ',' << lex.value_ << ',' << lex.value2_ << ") ";
    }
    else
      out << '(' << s << ',' << "-" << ") ";
  }
  return out;
}


Lex Scanner::getLex (vector<Ident>& tid) 
{
  enum state { H, IDENT, NUMB, COM, ALE, NEQ, COM1, COM2, STR };
  int d, j;
  string buf;
  state CS = H;
  do {
    gc();
    if ( currSym_ == EOF && CS != H)
      throw "Unclosed comment or string!";
    else if (currSym_ == EOF)
      return (Lex(LEX_FIN));
    switch(CS) {

        case H:   if ( currSym_ == ' '  || currSym_ == '\n' || 
                       currSym_ == '\r' || currSym_ == '\t' );
                  else if ( isalpha(currSym_) ) { // a or A (check on letter)
                    buf.push_back(currSym_);
                    CS = IDENT;
                  }
                  else if ( isdigit(currSym_) ) { // check on number
                    d = currSym_ - '0';
                    CS = NUMB;
                  }
                  else if ( currSym_ == '/' ) {
                    CS = COM1;
                  }
                  else if ( currSym_ == '=' || currSym_ == '<' || currSym_ == '>' ) {
                    buf.push_back(currSym_);
                    CS = ALE;
                  }
                  else if (currSym_ == '!') {
                    buf.push_back(currSym_);
                    CS = NEQ;
                  }
                  else if (currSym_ == '\"') {
                    CS = STR;
                  }
                  else {
                    buf.push_back(currSym_);
                    if ( (j = look ( buf, TD)) ) {
                      return Lex ( (TypeOfLex)(j+(int)LEX_FIN), j );
                    }
                    else
                      throw currSym_;
                  }
                  break;

      case STR:   if ( currSym_ != '\"') {
                    buf.push_back(currSym_);
                  }
                  else {
                    return Lex (LEX_STR, 0,-1, buf);
                  }
                  break;

      case IDENT: if ( isalpha(currSym_) || isdigit(currSym_) ) {
                    buf.push_back(currSym_);
                  }
                  else {
                    ungetc (currSym_, file_);
                    if ( (j = look (buf, TW)) ){
                      return Lex ((TypeOfLex)j, j);
                    }
                    else {
                      j = putIdent(buf, tid);
                      return Lex (LEX_ID, j);
                    }
                  }
                  break;

      case NUMB:  if ( isdigit(currSym_) ) {
                   d = d * 10 + (currSym_ - '0');
                  }
                  else {
                    ungetc (currSym_, file_);
                    return Lex ( LEX_NUM, d);
                  }
                  break;

      case COM1:  if ( currSym_ == '*' )
                    CS = COM;
                  else {
                    ungetc (currSym_, file_);
                    return Lex (LEX_SLASH, 15);
                  }
                  break;

      case COM2:  if ( currSym_ == '/')
                    CS = H;
                  else if ( currSym_ != '*')
                    CS = COM;

                  break;

      case COM:   if ( currSym_ == '*' ) {
                    CS = COM2;
                  }
                  break;

      case ALE:   if ( currSym_ == '=') {
                    buf.push_back(currSym_);
                    j = look ( buf, TD );
                    return Lex ( (TypeOfLex)(j+(int)LEX_FIN), j);
                  }
                  else {
                    ungetc (currSym_, file_);
                    j = look ( buf, TD );
                    return Lex ( (TypeOfLex)(j+(int)LEX_FIN), j );
                  }
                  break;

      case NEQ:   if (currSym_ == '=') {
                    buf.push_back(currSym_);
                    j = look ( buf, TD );
                    return Lex ( LEX_NEQ, j );
                  }
                  else
                    throw '!';
                  break;
    }
  } while (true);
}

class Parser 
{ 
  public:
    vector<Lex> poliz;
    Parser (const char* program): scanner_ (program) {} 
    void analyze();
  private:
    struct Goto_ {  // структура для гоу ту, адрес и значение
      int valueLabel_;
      int adressGoto_;
    };
    Lex                  currLex_;
    TypeOfLex            currType_;
    int                  currValue_;
    Scanner              scanner_;
    stack <int>          stInt_;
    stack <TypeOfLex>    stLex_;
    static vector<int>   break_;
    static vector<Goto_> goto_;  // вектор из гоу ту
    
    void  Prog();  //P
    //void  Struct_checker(); // D2
    void  Variable_or_struct (vector<Ident>& tid = TID, bool flagIsStruct = false); //D1
    void  Variable_work  (TypeOfLex type, vector<Ident>& tid = TID, bool flagIsStruct = false);//D
    void  Struct_work (int value = 0); //D3
    void  Work_in_brace (bool isCycle = false);//B1
    void  Main_functions (bool isCycle = false);//B
    void  Ident_work (Lex currLex, bool isCycle = false);//G
    void  Logical_signs_work(Lex currLex, bool notObject = false, bool isWrite = false);//E
    void  Struct_eq (Lex currLex);//S
    void  Ident_first();//E1
    void  Ident_second();//E2
    void  Ident_three();//E3
    void  Ident_four();//E4
    void  Ident_five();//T
    void  Put_in_polis (bool isMinus = false, bool isNot = false);//F
    void  dec (TypeOfLex type, vector<Ident>& tid = TID);
    void  decStruct (int value = 0);
    void  checkId (int currValue);
    void  checkLabel (int value);
    void  checkObject();
    void  eqTypeStruct();
    void  checkOp();
    void  checkNot();
    void  checkMinus();
    void  eqType();
    void  eqBool();
    void  checkIdInRead();
    void  gl (vector<Ident>& tid = TID) 
    {
      currLex_ = scanner_.getLex(tid);  // сама лексема
      currType_ = currLex_.getType(); // тип лексемы
      currValue_ = currLex_.getValue(); // номер в таблице
    }
};


vector<int>   Parser::break_; // объявляю потому что статитк
vector<Parser::Goto_> Parser::goto_;

void Parser::analyze ()  // самое начало
{
  gl (); // читаю первую лексему
  Prog(); // началао синтаксического анализатора
  if (currType_ != LEX_FIN) { 
    throw currLex_;
  }
  vector <Lex>::iterator q = poliz.begin(); // получаем итератор для полиза
  while (q != poliz.end()) { // Вывод полиза
    cout << *q;
    ++q;
  }
  cout << endl;
}

void Parser::Prog () 
{
  if (currType_ == LEX_PROGRAM) { // проверка на слово програм
    gl();
  }
  else {
    throw currLex_;
  }
  if (currType_ == LEX_LBRACE) { // проверка на левую скобку
    gl();
  }
  else {
    throw currLex_;
  }
  //Struct_checker();
  Variable_or_struct();
  Work_in_brace();
  if (currType_ == LEX_RBRACE) { // проверка на правую фигурную скобку, закрывающую программу
    gl();
  }
  else {
    throw currLex_;
  }
  vector<Goto_>::iterator p = goto_.begin(); // итератор для хождения по вектору гоу ту
  int i;                                   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  while (p != goto_.end()) {
    i = p->valueLabel_;                  // номер в таблице тид
    if (TID[i].getDeclare() == 0) {		// если не объявлено то ошибка
      throw "not declared label";
    }
    poliz[p->adressGoto_] = Lex(POLIZ_LABEL, TID[i].getValue());
    ++p;
  }
}

/*void Parser::Struct_checker () 
{
  if (currType_ == LEX_STRUCT) {  // если не струкутура, то пропускается
    gl();
    if (TID[currValue_].getDeclare()) // не было ли двойной инициализации
      throw "twice initialization of struct";
    if (currType_ != LEX_ID) {
      throw currLex_;
    }
    TID[currValue_].putDeclare();  // объявляем занятым
    TID[currValue_].putType(LEX_STRUCT); //кладем тип структуры
    int index = currValue_;
    TID[index].putIsStruct(); // кладем метку что струкутура
    gl();
    if (currType_ != LEX_LBRACE) {  
      throw currLex_;
    }
    gl();
    Variable_or_struct(TID[index].tid, true); // начинаем ситывать переменные в структуре !!!!!!!!!!!!!!!!!!!!!!!!!!
    if (currType_ != LEX_RBRACE) {
      throw currLex_;
    }
    gl();
    if (currType_ != LEX_SEMICOLON) { // если не точка с запятой то ошибка
      throw currLex_;
    }
    gl();
    Struct_checker(); // возмодны ещё структуры
  }
}
*/

void Parser::Variable_or_struct (vector<Ident>& tid, bool flagIsStruct) 
{ 
  if (currType_ == LEX_INT || currType_ == LEX_STRING || currType_ == LEX_BOOL) { // тип
    TypeOfLex oldType = currType_;
    gl(tid); // читаем лексему 
    Variable_work (oldType, tid, flagIsStruct); // !!!!!!!!!!!!!!!!
    if (currType_ == LEX_SEMICOLON) { // если точка с запятой
      gl();
    }
    else {
      throw currLex_;
    }
    Variable_or_struct(tid, flagIsStruct); // могут быть ещё переменные
  }
}



void Parser::Variable_work (TypeOfLex type, vector<Ident>& tid, bool flagIsStruct) { // c идентификатором
  if (currType_ != LEX_ID) {
    throw currLex_;
  }
  else {
    stInt_.push (currValue_); // записываем в вектор номер идентифифкатора в таблице тид
    int oldValue = currValue_;
    gl(tid); // считываем = или ; и т.д
    if ((currType_ == LEX_ASSIGN) && (flagIsStruct == true)) { // в структуре нет равенств
      throw "initialization";
    }
    if (currType_ == LEX_ASSIGN) { // если =
      poliz.push_back (Lex ( POLIZ_ADDRESS, oldValue) ); // записываем в вектор полиз адрес переменной
      gl(tid); // считываем новую лексему
      if (type == LEX_BOOL) {       //обработка бул
        if  (currType_ == LEX_TRUE) {
          poliz.push_back (Lex (LEX_TRUE, 1));
        }
        else if (currType_ == LEX_FALSE) {
          poliz.push_back (Lex (LEX_FALSE, 0));
        }
        else { 
          throw currLex_;
        }
      }
      else if (type == LEX_STRING) { // обработка строки
        if (currType_ == LEX_STR) {
          poliz.push_back (currLex_);
        }
        else {
          throw currLex_;
        }
      }
      else if (type == LEX_INT) { //обработка инт 
        if (currType_ == LEX_NUM) {
          poliz.push_back (currLex_);
        }
        else { 
          throw currLex_;
        }
      } 
      poliz.push_back (Lex (LEX_ASSIGN) ); // добавляет =
      poliz.push_back (Lex(LEX_SEMICOLON,1)); // добавляет точку с запятой
      gl(tid);
    }
    while (currType_ == LEX_COMMA) { // пока запятые, аналогично
      gl(tid);
      if (currType_ != LEX_ID) {
        throw currLex_;
      }
      else {
        stInt_.push (currValue_);
        oldValue = currValue_;
        gl(tid);
        if ((currType_ == LEX_ASSIGN) && (flagIsStruct == true)) {
          throw "initialization";
        }
        if (currType_ == LEX_ASSIGN) {
          poliz.push_back (Lex ( POLIZ_ADDRESS, oldValue) );
          gl(tid);
          if (type == LEX_BOOL) {
            if  (currType_ == LEX_TRUE) {
              poliz.push_back (Lex (LEX_TRUE, 1));
            }
            else if (currType_ == LEX_FALSE) {
              poliz.push_back (Lex (LEX_FALSE, 0));
            }
            else { 
              throw currLex_;
            }
          }
          else if (type == LEX_STRING) {
            if (currType_ == LEX_STR) {
              poliz.push_back (currLex_);
            }
            else {
              throw currLex_;
            }
          }
          else if (type == LEX_INT) {
            if (currType_ == LEX_NUM) {
              poliz.push_back ( currLex_ );
            }
            else { 
              throw currLex_;
            }
          } 
          poliz.push_back (Lex (LEX_ASSIGN) );
          poliz.push_back (Lex(LEX_SEMICOLON,1));
          gl(tid);
        }
      }
    }
    dec (type, tid); // объявление идентификаторов
  }
}


void Parser::Work_in_brace (bool isCycle)
{
  while (currType_ != LEX_RBRACE) { // пока не правая скобка
    Main_functions (isCycle);
  }
}


void Parser::Main_functions (bool isCycle)
{
  int pl0, pl1, pl2, pl3;
  int l1, l2, l3, l4;

  if (currType_ == LEX_IF) { // работа с иф
    gl();
    if (currType_ != LEX_LPAREN) { // проверка на скобки
      throw currLex_;
    }
    gl(); // считывается лексема
    Logical_signs_work (currLex_); // !!!!!!!
    eqBool(); // если тип последней лексему не бул, то ошибка
    pl2 = poliz.size(); // запоминаем место, которое освободим
    poliz.push_back (Lex()); // занимаем место
    poliz.push_back (Lex(POLIZ_FGO)); //переход по лжи
    if (currType_ != LEX_RPAREN) { //если не конец условия, то ошибка
      throw currLex_;
    }
    gl();
    Main_functions (isCycle); // обработка того, что в ифе
    pl3 = poliz.size();
    poliz.push_back (Lex()); // по правилу составления полиза для if
    poliz.push_back (Lex(POLIZ_GO));
    poliz[pl2] = Lex (POLIZ_LABEL, poliz.size()); // метка
    if (currType_ == LEX_ELSE) {
      gl();
      Main_functions (isCycle); //для else
      poliz[pl3] = Lex(POLIZ_LABEL, poliz.size());
    }
    else {
      throw currLex_;
    }
  } 
  else if (currType_ == LEX_WHILE) { // работа с вайл
    pl0 = poliz.size();
    gl();
    if (currType_ != LEX_LPAREN) { // нужна левая скобка
      throw currLex_;
    }
    gl();
    Logical_signs_work (currLex_); // обрабатывается выражение в скобках
    eqBool(); // должен быть тип бул
    pl1 = poliz.size(); //построение по шаблону
    poliz.push_back (Lex());
    poliz.push_back (Lex(POLIZ_FGO));
    if (currType_ != LEX_RPAREN) { // если не )
      throw currLex_;
    }
    gl();
    Main_functions (true); // выполняем условие в теле вайла
    poliz.push_back (Lex(POLIZ_LABEL, pl0));//по шаблону
    poliz.push_back (Lex(POLIZ_GO));
    poliz[pl1] = Lex (POLIZ_LABEL, poliz.size());//ссылка
    vector<int>::iterator p = break_.begin();// !!!!!!!
    while (p != break_.end()) { 
      poliz[*p] = Lex (POLIZ_LABEL, poliz.size());
      ++p;
    }
    break_.clear();
  } 
  else if (currType_ == LEX_FOR) { // работа с фор
    gl();
    if (currType_ != LEX_LPAREN) {
      throw currLex_;
    }
    gl();
    Logical_signs_work (currLex_);  // первоу условие в фор
    if (currType_ != LEX_SEMICOLON)
      throw currLex_;
    poliz.push_back(currLex_); // второе условие в фор
    gl();
    l1 = poliz.size();
    Logical_signs_work (currLex_); // выполненеие 2 условия
    eqBool();  //должен быть булеан
    if (currType_ != LEX_SEMICOLON)
      throw currLex_;
    gl();   
    l2 = poliz.size(); //третье условие в фор
    poliz.push_back (Lex()); // по шаблону
    poliz.push_back (Lex(POLIZ_FGO));
    l3 = poliz.size();
    poliz.size();
    poliz.push_back (Lex());
    poliz.push_back (Lex(POLIZ_GO));
    l4 = poliz.size();
    Logical_signs_work (currLex_); // выполнение 3 условия
    if (currType_ != LEX_RPAREN)
      throw currLex_;
    poliz.push_back(Lex(LEX_SEMICOLON,1));
    gl();
    poliz.push_back (Lex(POLIZ_LABEL, l1)); //по шаблону
    poliz.push_back (Lex(POLIZ_GO));
    poliz[l3] = Lex(POLIZ_LABEL, poliz.size());
    Main_functions (true); // true -> цикл
    poliz.push_back (Lex(POLIZ_LABEL, l4));
    poliz.push_back (Lex(POLIZ_GO));
    poliz[l2] = Lex (POLIZ_LABEL, poliz.size());
    vector<int>::iterator p = break_.begin(); // итератор для вектора break
    while (p != break_.end()) { 
      poliz[*p] = Lex (POLIZ_LABEL, poliz.size()); // на местах брейка ссылка выходит за цикл
      ++p;
    }
    break_.clear();
  } 
  else if (currType_ == LEX_BREAK) { // работа с брейк
    if (!isCycle) { // вне цикла нельзя ставить брейк
      throw currLex_;
    }
    break_.push_back(poliz.size()); // в вектор break добавили значение
    poliz.push_back (Lex()); // занимаем место
    poliz.push_back (Lex(POLIZ_GO));
    gl();
    if (currType_ == LEX_SEMICOLON) { // после break обязательно ;
      gl();
    }
    else {
      throw currLex_;
    }
  } 
  else if (currType_ == LEX_LBRACE) { // {
    gl();
    Work_in_brace (isCycle); // вызываем функцию пока не будет }
    if (currType_ == LEX_RBRACE) {
      gl();
    }
    else {
      throw currLex_;
    }
  }
  else if (currType_ == LEX_ID) { // идентифифкатор/переменная !!!!!!!!!!!!!!
    Ident_work (currLex_, isCycle); //!!!!!!!!!!!!!!!!!!!!!
  }
  else if (currType_ == LEX_SEMICOLON) {                      //;;;;;
    gl();
  }
  else if (currType_ == LEX_NUM    || currType_ == LEX_STR   || // если константа или левая скобка
           currType_ == LEX_TRUE   || currType_ == LEX_FALSE ||
           currType_ == LEX_LPAREN || currType_ == LEX_MINUS ||
           currType_ == LEX_NOT ) {
    Ident_first (); //обработка выражения
    if (currType_ == LEX_SEMICOLON) { // если ; то добавляем в полиз
      poliz.push_back(currLex_);
      gl();
    }
    else {
      throw currLex_;
    }
    
  } 
  else if (currType_ == LEX_READ) { // read
    gl();
    if (currType_ == LEX_LPAREN) { // должна быть (
      gl();
      if (currType_ == LEX_ID) { //переменная
        checkIdInRead(); //объявлен ли
          poliz.push_back(Lex(POLIZ_ADDRESS, currValue_)); //кладем адрес
          gl();
      }
      else {
        throw currLex_;
      }
      if (currType_ == LEX_RPAREN) { // если правая скобка, то норм
        gl();
        poliz.push_back (Lex(LEX_READ));
      }
      else {
        throw currLex_;
      }
      if (currType_ == LEX_SEMICOLON) { // должны быть ;
        gl();
      }
      else {
        throw currLex_;
      }
    }
    else {
      throw currLex_;
    }
  } 
  else if (currType_ ==  LEX_WRITE) {  // write
    gl();
    if (currType_ == LEX_LPAREN) {  // прававя скобка
      gl();
      Logical_signs_work (currLex_,false,true); // выполнить условие в write()
      poliz.push_back (Lex(LEX_WRITE));
      while (currType_ == LEX_COMMA) { // для работы с запятыми
        gl();
        Logical_signs_work (currLex_,false,true);
        poliz.push_back (Lex(LEX_WRITE));
      }
      if (currType_ != LEX_RPAREN) {
        throw currLex_;
      }
      gl();
      if (currType_ != LEX_SEMICOLON) {
        throw currLex_;
      }
      gl();
    }
    else {
      throw currLex_;
    }
  }
  else if (currType_ == LEX_GOTO) {    // обработка go to
    gl();
    if (currType_ != LEX_ID) { // обязателен идентифифкатор
      throw currLex_;
    }
    if (TID[currValue_].getDeclare() && TID[currValue_].isLabel()) { // имя "stop" это метка
      poliz.push_back (Lex(POLIZ_LABEL, TID[currValue_].getValue()));
      poliz.push_back (Lex(POLIZ_GO));
    }
    else if (TID[currValue_].getDeclare() && !TID[currValue_].isLabel()) {
      throw "Not label in operator goto";
    }
    else {
      Goto_ g;
      g.adressGoto_ = poliz.size();
      g.valueLabel_ = currValue_;
      goto_.push_back(g);  // добавляаем метку в стек
      poliz.push_back (Lex()); // выделяем место куда будем переходить
      poliz.push_back (Lex(POLIZ_GO));
    }
    gl();
    if (currType_ != LEX_SEMICOLON) {
      throw currLex_;
    }
    gl();
  } 
  else {
    throw currLex_;
  }
}


void Parser::Ident_work (Lex currLex, bool isCycle)
{
  int value;
  if (currType_ == LEX_ID) {
    gl();
    if (currType_ == LEX_COLON) { // двоеточие, обработка метки
      value = currLex.getValue();
      checkLabel(value);
      TID[value].putIsLabel();
      TID[value].putDeclare();
      TID[value].putValue (poliz.size());
      gl();
      Ident_work (currLex_, isCycle);
    }
    else {
      if (currType_ <= 20) { // если таблица TW
        scanner_.stepBack (strlen(Scanner::TW[currValue_])); // чтобы заново обработать
      }
      else if ((currType_ >= 22) && (currType_ <= 40)) { // если таблица TD
        scanner_.stepBack (strlen(Scanner::TD[currValue_])); // еще раз обработать
      }
      else {
        throw currLex_;
      }
      currLex_ = currLex;
      currType_ = currLex_.getType();
      currValue_ = currLex_.getValue();
      Logical_signs_work (currLex_);
      if (currType_ == LEX_SEMICOLON) {
      	poliz.push_back (Lex(LEX_SEMICOLON,1));
      	gl();
      }
      else {
      	throw currLex_;
      }
    }
  }
  else {
    Main_functions (isCycle);
  }
}


void Parser::Logical_signs_work (Lex currLex, bool notObject, bool isWrite)
{ 
  int i,j;
  if (currType_ == LEX_ID) { // если пременная
    gl();
    if (currType_ == LEX_ASSIGN) { // если равенство
      if (currLex.getType() == LEX_OBJECT) { // если в структуре
        i = currLex.getValue(); // получаем адреса для переменной
        j = currLex.getValue2();
        stLex_.push (TID[i].tid[j].getType()); // кладем в стек лексем
        poliz.push_back (Lex(POLIZ_ADDRESS, i, j));
        gl();
        Logical_signs_work (currLex_, notObject, isWrite); // !!!!!!!!!!!!!!!!!!!!!
        eqType();   // !!!!!!!!!!!!!!!!!
        poliz.push_back (Lex(LEX_ASSIGN));
      }
      else {  // равнесто не структур
        checkId (currLex.getValue()); //!!!!!!!
        poliz.push_back (Lex(POLIZ_ADDRESS, currLex.getValue())); // кладем адрес в полиз
        gl();
        Logical_signs_work (currLex_, true, isWrite);   // если есть ещё равенства или просто запишем в полиз
        eqType();
        poliz.push_back (LEX_ASSIGN); //= в полиз
      }
    }
    else if (currType_ == LEX_PLUS  || currType_ == LEX_MINUS || // операция умножения, сравнения
             currType_ == LEX_TIMES || currType_ == LEX_SLASH ||
             currType_ == LEX_LSS   || currType_ == LEX_GTR   ) { // > < * + - ...
      scanner_.stepBack(1); // шаг назад из считывания
      currLex_ = currLex;
      currType_ = currLex_.getType();
      currValue_ = currLex_.getValue();
      Ident_first ();
    }
    else if (currType_ == LEX_OR    || currType_ == LEX_NEQ   || //операция сравнения
             currType_ == LEX_LEQ   || currType_ == LEX_GEQ   ||
             currType_ == LEX_EQ    ) {
      scanner_.stepBack(2); // два шага назад, т.к. два знака в сравнении
      currLex_ = currLex;
      currType_ = currLex_.getType();
      currValue_ = currLex_.getValue();
      Ident_first ();
    }
    else if (currType_ == LEX_AND) { // и
      scanner_.stepBack(3); // назад на 3 шага
      currLex_ = currLex;
      currType_ = currLex_.getType();
      currValue_ = currLex_.getValue();
      Ident_first ();
    }
    else if (currType_ == LEX_RPAREN || (currType_ == LEX_SEMICOLON && !isWrite)) {
      if (currLex.getType() == LEX_OBJECT) { //если объект
        i = currLex.getValue();
        j = currLex.getValue2();
        stLex_.push (TID[i].tid[j].getType()); //в стэк лексем
        poliz.push_back (currLex); //записываем объект в полиз
      }
      else {
        checkId (currLex.getValue());
        poliz.push_back (currLex);
      }
    }
    else if (currType_ == LEX_COMMA && isWrite) {
      if (currLex.getType() == LEX_OBJECT) { //есил объет (st.a)
        i = currLex.getValue();
        j = currLex.getValue2();
        stLex_.push (TID[i].tid[j].getType()); // добавит в стэк лексем
        poliz.push_back (currLex);
      }
      else {
        checkId (currLex.getValue()); // !!!!!!!!!!!!!!!
        poliz.push_back (currLex);
      }
    }
    else if (currType_ == LEX_POINT) { // если точка
      i = currLex.getValue();
      gl(TID[i].tid); // получем лекему ИЗ структуры
      if (currType_ != LEX_ID) {
        throw currLex_;
      }
      if (!TID[i].tid[currValue_].getDeclare()) {
        throw "wrong field of object";
      }
      currLex_ = Lex(LEX_OBJECT, i, currValue_); //отправляем переменную с типом объект
      Logical_signs_work (currLex_, true, isWrite);
    }
    else {
      throw currLex_;
    }
  }
  else {
    Ident_first ();
  }
}


void Parser::checkObject () // проверка на объект
{
    stInt_.push (TID[currValue_].getValueStruct());
}


void Parser::eqTypeStruct () //равенство структур
{
  int t = stInt_.top(); // самый верхний
  stInt_.pop(); //1 выкидываем
  if (t !=stInt_.top()) { //сравнение последнего и предпоследнего
    throw "wrong types are in =";
  }
}

void Parser::Ident_first ()
{
  Ident_second ();
  while (currType_ == LEX_OR) { // если or
    stLex_.push (currType_);
    gl();
    Ident_second ();
    checkOp();
  }
}


void Parser::Ident_second ()
{
  Ident_three ();
  while (currType_ == LEX_AND) {// если and
    stLex_.push (currType_);
    gl();
    Ident_three();
    checkOp();
  }
}


void Parser::Ident_three ()
{
  Ident_four ();
  if ( currType_ == LEX_LSS || currType_ == LEX_GTR || currType_ == LEX_LEQ ||  // если знаки сравнения
       currType_ == LEX_GEQ || currType_ == LEX_EQ  || currType_ == LEX_NEQ  )  { 
    stLex_.push (currType_);
    gl();
    Ident_four ();
    checkOp();
  }
}


void Parser::Ident_four () 
{
  Ident_five();
  while (currType_ == LEX_PLUS || currType_ == LEX_MINUS) { // если + или -
    stLex_.push (currType_);
    gl();
    Ident_five ();
    checkOp();
  }
}


void Parser::Ident_five ()
{
  Put_in_polis ();
  while (currType_ == LEX_TIMES || currType_ == LEX_SLASH) { // если * или /
    stLex_.push (currType_);
    gl();
    Put_in_polis();
    checkOp();
  }
}


void Parser::Put_in_polis (bool isMinus, bool isNot)
{ 
  int i,j;
  if (currType_ == LEX_OBJECT) { // если объект
    i = currLex_.getValue();
    j = currLex_.getValue2();
    stLex_.push (TID[i].tid[j].getType());
    poliz.push_back (Lex(LEX_OBJECT, i, j)); //добавление объекта в полиз
    gl();
  }
  else if (currType_ == LEX_ID) { // если переменная
      checkId (currValue_); //!!!!!!
      poliz.push_back (Lex (LEX_ID, currValue_));
      gl();
  }
  else if (currType_ == LEX_NUM) {
    stLex_.push (LEX_INT); //записываем в стек
    poliz.push_back (currLex_);
    gl();
  }
  else if (currType_ == LEX_TRUE) { // анлаогично
    stLex_.push (LEX_BOOL);
    poliz.push_back (Lex(LEX_TRUE, 1));
    gl();
  }
  else if (currType_ == LEX_FALSE) {// анлаогично
    stLex_.push (LEX_BOOL);
    poliz.push_back (Lex (LEX_FALSE, 0));
    gl();
  }
  else if (currType_ == LEX_NOT) {// анлаогично
    if (isMinus) {
      throw currLex_;
    }
    gl();
    Put_in_polis (false, true); //isNot = true
    checkNot(); //!!!!!!
  }
  else if (currType_ == LEX_LPAREN) { //если левая скобка
    gl();
    Logical_signs_work (currLex_);
    if ((currType_ == LEX_RPAREN)) { //если правая скобка
      gl();
    }
    else {
      throw currLex_;
    }
  }
  else if (currType_ == LEX_STR) { //если строка
    stLex_.push (LEX_STRING);
    poliz.push_back (currLex_);
    gl();
  }
  else if (currType_ == LEX_MINUS) { //если минус
    if (isNot) {
      throw currLex_;
    }
    gl();
    Put_in_polis (true, false);
    checkMinus(); // !!!!!
  }
  else {
    throw currLex_;
  }
}


void Parser::checkNot () // проперка на not
{
  if (stLex_.top() != LEX_BOOL) {
    throw "wrong type is in not";
  }
  else {
    poliz.push_back (Lex (LEX_NOT));
  }
}


void Parser::checkMinus ()  // проверка на -
{
  if (stLex_.top() != LEX_INT) {
    throw "wrong type is in MINUS";
  }
  else {
    poliz.push_back (Lex (POLIZ_MINUS));
  }
}


void Parser::eqType () // проверка на равнество типов
{
  TypeOfLex t = stLex_.top();
  stLex_.pop();
  if (stLex_.empty()) {
    throw "wrong types are in =";
  }
  if (t != stLex_.top()) {
    throw "wrong types are in =";
  }
}


void Parser::eqBool () //проверка на bool
{
  if (stLex_.top() != LEX_BOOL) {
    throw "expression is not boolean";
  }
  stLex_.pop();
}


void Parser::checkId (int currValue) // проверка 
{
  if (TID[currValue].getDeclare()) {
    if (TID[currValue].isLabel()) { //метка
      throw "wrong using label";
    }
    stLex_.push (TID[currValue].getType()); // кладем тип в стек
  }
  else {
    throw "not declared";
  }
}


void Parser::checkLabel (int value)
{
  if (TID[value].getDeclare()) { //нельзя чтобы было объявлено
    throw "label matches variable or redescribed";
  }
}


void Parser::checkOp () // проверка операции
{
  TypeOfLex t1, t2, op;

  t2 = stLex_.top();
  stLex_.pop();
  op = stLex_.top();
  stLex_.pop();
  t1 = stLex_.top();
  stLex_.pop();
  
  if (t1 == t2 && t1 == LEX_STRING) { // проверка строк
    if (op == LEX_PLUS) {
      stLex_.push (LEX_STRING);
    }
    else if (op == LEX_GTR || op == LEX_LSS || op == LEX_EQ || op == LEX_NEQ) {
      stLex_.push (LEX_BOOL);
    }
    else {
      throw "wrong operation for type string";
    }
  }
  else if (t1 == t2 && t1 == LEX_INT) { // проверка инт
    if (op == LEX_PLUS || op == LEX_MINUS || op == LEX_TIMES || op == LEX_SLASH) {
      stLex_.push (LEX_INT);
    }
    else {
      stLex_.push (LEX_BOOL);
    }
  }
  else if (t1 == t2 && t1 == LEX_BOOL) { // проверка бул
    if (op == LEX_OR || op == LEX_AND) {
      stLex_.push (LEX_BOOL);
    }
    else {
      throw "wrong operation for type bool";
    }
  }
  else {
    throw "wrong types are in operation";
  }
  poliz.push_back (Lex(op));
}


void Parser::dec (TypeOfLex type, vector<Ident>& tid) // обявление идентификаторов
{
  int i;
  while ( !stInt_.empty()) {
    i = stInt_.top();
    stInt_.pop();
    if ( tid[i].getDeclare() ) {
      throw "twice";
    }
    else {
      tid[i].putDeclare();
      tid[i].putType(type);
    }
  }
}


void Parser::checkIdInRead () 
{
  if ( !TID [currValue_].getDeclare() )
    throw "not declared";
}





class Executer 
{
  public:
    void execute (vector <Lex>& poliz);
  private:
    Lex pcEl_;
    struct Element {
      int val_;
      int val2_;
      string str_;
      bool isString_;
      bool isObject_;
      int getValue () const 
      {
        return val_;
      }
      int getValue2 () const 
      {
        return val2_;
      }
      void putIsObject()
      {
        isObject_ = true;
      }
      Element (int val, int val2 = -1): val_(val), val2_(val2), isString_(false) 
      {
        isObject_ = (val2_== -1) ? false : true;
      }
      Element (string s): str_(s), isString_(true), isObject_(false) {}
      bool isString () { return isString_; }
      operator int () { return val_; }
      operator string () { return str_; }
    };
};


void Executer::execute (vector <Lex>& poliz) 
{
  stack <Element> args;  // стэк элементов
  int num, i, j, index = 0, size = poliz.size();
  string s1, s2;
  Ident* id;
  while (index < size) { // сайз = размер полиза
  	pcEl_ = poliz[index]; // лексема
  	switch (pcEl_.getType()) {

      case LEX_TRUE:   case LEX_FALSE:    case LEX_NUM:  case POLIZ_LABEL:
        args.push (pcEl_.getValue());  //кладем число
        break;

      case POLIZ_ADDRESS:
        if (pcEl_.getValue2() != -1) {   //структуры
          i = pcEl_.getValue();
          j = pcEl_.getValue2();
          args.push (Element(i,j));
        }
        else {
          args.push (pcEl_.getValue()); //кладем число
        }
        break;

      case LEX_STR:
        args.push (pcEl_.getStr()); //кладем строку
        break;

      case LEX_ID:  //обработка идентифифкатора
        i = pcEl_.getValue(); //кладем число
        if (TID[i].getAssign()) {  //было ли использовано
          if (TID[i].getType() == LEX_STRING) {    // строка или переменная
            args.push (TID[i].getStr());
          }
          else {
            args.push (TID[i].getValue());
          }
        }
        else {
          throw "POLIZ: indefinite identifier";
        }
        break;
      
      case LEX_OBJECT:
        i = pcEl_.getValue();
        j = pcEl_.getValue2();
        id = &(TID[i].tid[j]);
        if ((*id).getAssign()) {
          if ((*id).getType() == LEX_STRING) {
            args.push ((*id).getStr());
          }
          else {
            args.push ((*id).getValue());
          }
        }
        else {
          throw "POLIZ: indefinite identifier";
        }
        break;

      case LEX_SEMICOLON: //очистка
        args.pop();
        break;


      case LEX_NOT:  //изменение типа бул
        i = (int) args.top();
        args.pop();
        args.push (!i);
        break;

      case POLIZ_MINUS:  //изменение знака
        i = (int) args.top();
        args.pop();
        args.push (-i);
        break;

      case LEX_OR:  //логическое или
        i = (int) args.top();
        args.pop();
        j = (int) args.top();
        args.pop();
        args.push (j || i);
        break;

      case LEX_AND:  // логическое и
        i = (int) args.top();
        args.pop();
        j = (int) args.top();
        args.pop();
        args.push ( j && i );
        break;

      case POLIZ_GO:   // безусловный переход
        i = (int) args.top();
        args.pop();
        index = i-1; //перместиоись на новый элмент полиза
        break;

      case POLIZ_FGO:  //переход по лжи
        i = (int) args.top();
        args.pop();
        j = (int) args.top();
        args.pop();
        if ( !j ) {   //проврка на правда или нет
          index = i-1;
        }
        break;

      case LEX_WRITE: 
        if (args.top().isString()) { //если строка
          s2 = (string) args.top();
          args.pop();
          cout << s2 << endl;
        }
        else {
          j = (int) args.top(); //если число
          args.pop();
          cout << j << endl;
        }
        break;

      case LEX_READ:
        int k;
	      i = (int) args.top(); 
	      args.pop();
	      if (TID[i].getType() == LEX_INT) { //считываем число
	        cout << "Waiting for int " << TID[i].getName() << endl;
	        cin >> k;
	        TID[i].putValue(k);   //кладем значение
	        TID[i].putAssign();
	      }
	      else if (TID[i].getType() == LEX_STRING) {  //считываем строку
	        cout << "Waiting for string " << TID[i].getName() << endl;
	        cin >> s1;
	        TID[i].putValue(s1); //кладем строку
	        TID[i].putAssign();
	      }
	      else {
	        string j;
	        while (1) {
	          cout << "Waiting for true or false " << TID[i].getName() << endl;
	          cin >> j;
	          if ( j != "true" && j != "false" ) {
	            cout << "Error " << endl;
	            continue;
	          }
	          k = (j == "true") ? 1 : 0;
	          break;
	        }
	        TID[i].putValue(k); //кладем значение в идентификатор
	        TID[i].putAssign();
	      }
        break;

      case LEX_PLUS:  //плюс
        if (args.top().isString()) {
          s2 = (string) args.top();
          args.pop();
          s1 = (string) args.top();
          args.pop();
          args.push (s1 + s2); 
        }
        else {
          i = (int) args.top();
          args.pop();
          j = (int) args.top();
          args.pop();
          args.push ( i + j );
        }
        break;

      case LEX_TIMES: //умножение
        i = (int) args.top();
        args.pop();
        j = (int) args.top();
        args.pop();
        args.push ( i * j );
        break;

      case LEX_MINUS: //минус
        i = (int) args.top();
        args.pop();
        j = (int) args.top();
        args.pop();
        args.push ( j - i );
        break;

      case LEX_SLASH: //деление
        i = (int) args.top();
        args.pop();
        j = (int) args.top();
        args.pop();
        if (i != 0) {
          args.push(j / i);
          break;
        }
        else {
          throw "ERROR ZERO";
        }

      case LEX_EQ: // =
        if (args.top().isString()) {
          s2 = (string) args.top();
          args.pop();
          s1 = (string) args.top();
          args.pop();
          args.push (s1 == s2); 
        }
        else {
          i = (int) args.top();
          args.pop();
          j = (int) args.top();
          args.pop();
          args.push ( j == i );
        }
        break;

      case LEX_LSS: // <
        if (args.top().isString()) {
          s2 = (string) args.top();
          args.pop();
          s1 = (string) args.top();
          args.pop();
          args.push (s1 < s2); 
        }
        else {
          i = (int) args.top();
          args.pop();
          j = (int) args.top();
          args.pop();
          args.push ( j < i );
        }
        break;

      case LEX_GTR: // >
        if (args.top().isString()) {
          s2 = (string) args.top();
          args.pop();
          s1 = (string) args.top();
          args.pop();
          args.push (s1 > s2); 
        }
        else {
          i = (int) args.top();
          args.pop();
          j = (int) args.top();
          args.pop();
          args.push ( j > i );
        }
        break;

      case LEX_LEQ: // <=
        i = (int) args.top();
        args.pop();
        j = (int) args.top();
        args.pop();
        args.push ( j <= i );
        break;

      case LEX_GEQ: // >=
        i = (int) args.top();
        args.pop();
        j = (int) args.top();
        args.pop();
        args.push ( j >= i );
        break;

      case LEX_NEQ:// !=
        if (args.top().isString()) {
          s2 = (string) args.top();
          args.pop();
          s1 = (string) args.top();
          args.pop();
          args.push (s1 != s2); 
        }
        else {
          i = (int) args.top();
          args.pop();
          j = (int) args.top();
          args.pop();
          args.push ( j != i );
        }
        break;

      case LEX_ASSIGN: // =
        if (args.top().isString()) {     
          s2 = (string) args.top();
          args.pop();
            j = (int) args.top();
            args.pop();
            TID[j].putValue(s2);
            TID[j].putAssign();
          args.push (s2);
        }
        else {
          num = (int) args.top();
          args.pop();
            j = (int) args.top();
            args.pop();
            TID[j].putValue(num);
            TID[j].putAssign();
          args.push (num);
        }
        break;

      default:
        throw "unexpected elem";
  	}  // end of switch
  	++ index;
  }  // end of while
  cout << "END" << endl;
}


class Interpretator 
{
  public:
    Interpretator (const char* program): pars(program) {}
    void interpretation ();	
  private:  
  	Parser pars;
    Executer E;
};
 

void Interpretator::interpretation () 
{
  pars.analyze ();
  E.execute ( pars.poliz );
}

int main(int argc, char** argv) {
	try {
    Interpretator I (argv[1]);
    I.interpretation();
    return 0;
  }
  catch (char c) {
    cout << "unexpected symbol " << c << endl;
    return 1;
  }
  catch (Lex l) {
    cout << "unexpected lexeme" << l << endl;
    return 1;
  }
  catch (const char* c){
    cout << c << endl;
    return 1;
  }
}
