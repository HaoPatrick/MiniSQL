%skeleton "lalr1.cc"
%require  "3.0"
%debug 
%error-verbose
%defines 
%define api.namespace {MC}
%define parser_class_name {MC_Parser}

%code requires{
   namespace MC {
      class MC_Driver;
      class MC_Scanner;
   }

// The following definitions is missing when %locations isn't used
# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

}

%parse-param { MC_Scanner  &scanner  }
%parse-param { MC_Driver  &driver  }

%code{
   #include <iostream>
   #include <cstdlib>
   #include <fstream>
   
   /* include for all driver functions */
   #include "mc_driver.hpp"

#undef yylex
#define yylex scanner.yylex
}

%define api.value.type variant
%define parse.assert

%right ASSIGN
%left OR
%left XOR
%left ANDOP
%nonassoc IN IS LIKE REGEXP
%left NOT '!'
%left BETWEEN
%left COMPARISON /* = <> < > <= >= <=> */
%left '|'
%left '&'
// %left <subtok> SHIFT /* << >> */
%left '+' '-'
%left '*' '/' '%' MOD
%left '^'
%nonassoc UMINUS

%token               END    0     "end of file"
%token               UPPER
%token               LOWER
%token <std::string> WORD
%token               NEWLINE
%token               CHAR
%token <int>         COMPARISON
%token <std::string> HLH

%token <std::string> NAME
%token <std::string> STRING
%token <int> INTNUM
%token <bool> BOOL
%token <float> APPROXNUM
%token <std::string> USERVAR

%token ADD
%token ALL
%token ALTER
%token ANALYZE
%token AND
%token ANY
%token AS
%token ASC
%token AUTO_INCREMENT
%token BEFORE
%token BETWEEN
%token BIGINT
%token BINARY
%token BIT
%token BLOB
%token BOTH
%token BY
%token CALL
%token CASCADE
%token CASE
%token CHANGE
%token CHAR
%token CHECK
%token COLLATE
%token COLUMN
%token COMMENT
%token CONDITION
%token CONSTRAINT
%token CONTINUE
%token CONVERT
%token CREATE
%token CROSS
%token CURRENT_DATE
%token CURRENT_TIME
%token CURRENT_TIMESTAMP
%token CURRENT_USER
%token CURSOR
%token DATABASE
%token DATABASES
%token DATE
%token DATETIME
%token DAY_HOUR
%token DAY_MICROSECOND
%token DAY_MINUTE
%token DAY_SECOND
%token DECIMAL
%token DECLARE
%token DEFAULT
%token DELAYED
%token DELETE
%token DESC
%token DESCRIBE
%token DETERMINISTIC
%token DISTINCT
%token DISTINCTROW
%token DIV
%token DOUBLE
%token DROP
%token DUAL
%token EACH
%token ELSE
%token ELSEIF
%token ENCLOSED
%token END
%token ENUM
%token ESCAPED
%token EXIT
%token EXPLAIN
%token FETCH
%token FLOAT
%token FOR
%token FORCE
%token FOREIGN
%token FROM
%token FULLTEXT
%token GRANT
%token GROUP
%token HAVING
%token HIGH_PRIORITY
%token HOUR_MICROSECOND
%token HOUR_MINUTE
%token HOUR_SECOND
%token IF
%token IGNORE
%token IN
%token INDEX
%token INFILE
%token INNER
%token INOUT
%token INSENSITIVE
%token INSERT
%token INT
%token INTEGER
%token INTERVAL
%token INTO
%token ITERATE
%token JOIN
%token KEY
%token KEYS
%token KILL
%token LEADING
%token LEAVE
%token LEFT
%token LIKE
%token LIMIT
%token LINES
%token LOAD
%token LOCALTIME
%token LOCALTIMESTAMP
%token LOCK
%token LONG
%token LONGBLOB
%token LONGTEXT
%token LOOP
%token LOW_PRIORITY
%token MATCH
%token MEDIUMBLOB
%token MEDIUMINT
%token MEDIUMTEXT
%token MINUTE_MICROSECOND
%token MINUTE_SECOND
%token MOD
%token MODIFIES
%token NATURAL
%token NOT
%token NO_WRITE_TO_BINLOG
%token NULLX
%token NUMBER
%token ON
%token ONDUPLICATE
%token OPTIMIZE
%token OPTION
%token OPTIONALLY
%token OR
%token ORDER
%token OUT
%token OUTER
%token OUTFILE
%token PRECISION
%token PRIMARY
%token PROCEDURE
%token PURGE
%token QUICK
%token READ
%token READS
%token REAL
%token REFERENCES
%token REGEXP
%token RELEASE
%token RENAME
%token REPEAT
%token REPLACE
%token REQUIRE
%token RESTRICT
%token RETURN
%token REVOKE
%token RIGHT
%token ROLLUP
%token SCHEMA
%token SCHEMAS
%token SECOND_MICROSECOND
%token SELECT
%token SENSITIVE
%token SEPARATOR
%token SET
%token SHOW
%token SMALLINT
%token SOME
%token SONAME
%token SPATIAL
%token SPECIFIC
%token SQL
%token SQLEXCEPTION
%token SQLSTATE
%token SQLWARNING
%token SQL_BIG_RESULT
%token SQL_CALC_FOUND_ROWS
%token SQL_SMALL_RESULT
%token SSL
%token STARTING
%token STRAIGHT_JOIN
%token TABLE
%token TEMPORARY
%token TEXT
%token TERMINATED
%token THEN
%token TIME
%token TIMESTAMP
%token TINYBLOB
%token TINYINT
%token TINYTEXT
%token TO
%token TRAILING
%token TRIGGER
%token UNDO
%token UNION
%token UNIQUE
%token UNLOCK
%token UNSIGNED
%token UPDATE
%token USAGE
%token USE
%token USING
%token UTC_DATE
%token UTC_TIME
%token UTC_TIMESTAMP
%token VALUES
%token VARBINARY
%token VARCHAR
%token VARYING
%token WHEN
%token WHERE
%token WHILE
%token WITH
%token WRITE
%token XOR
%token YEAR
%token YEAR_MONTH
%token ZEROFILL

 /* functions with special syntax */
%token FSUBSTRING
%token FTRIM
%token FDATE_ADD FDATE_SUB
%token FCOUNT

%type <int>  select_expr_list 
%type <int> table_references
%type <int> column_list
%type <int> delete_opts delete_list
%type <int> insert_opts insert_vals insert_vals_list
%type <int>  opt_length opt_binary opt_uz 
%type <int> column_atts data_type create_col_list


%start stmt_list

%locations

%%

// list_option : END | list END;

// list
//   : item
//   | list item
//   ;

// item
//   : UPPER   { driver.add_upper(); }
//   | LOWER   { driver.add_lower(); }
//   | WORD    { driver.add_word( $1 ); }
//   | NEWLINE { driver.add_newline(); }
//   | CHAR    { driver.add_char(); }
//   | HLH     {driver.test_display($1); }
//   ;

stmt_list: stmt ';' {driver.debug_info();driver.clear_all(); std::cout<<"> ";}
  | stmt_list stmt ';' {driver.debug_info();driver.clear_all(); std::cout<<"> ";}
  ;

stmt: select_stmt { driver.emit("STMT");
                    }
   ;

select_stmt: SELECT  select_expr_list
                        { driver.emit("SELECTNODATA"); } ;
    | SELECT  select_expr_list
     FROM table_references
     opt_where
     { driver.emit("SELECT"); } ;
;


opt_where: /* nil */ 
   | WHERE expr { driver.emit("WHERE"); };

column_list: NAME { driver.emit("COLUMN");driver.add_column($1); $$ = 1; }
  | column_list ',' NAME  { driver.emit("COLUMN");driver.add_column($3);  $$ = $1 + 1; }
  ;

select_expr_list: select_expr { $$ = 1; }
    | select_expr_list ',' select_expr {$$ = $1 + 1; }
    | '*' { driver.emit("SELECTALL"); $$ = 1; }
    ;

select_expr: expr ;

table_references:    table_reference { $$ = 1; }
    | table_references ',' table_reference { $$ = $1 + 1; }
    ;

table_reference:  table_factor
;

table_factor:
    NAME {driver.emit("TABLE");}


   /* statements: delete statement */

stmt: delete_stmt { driver.emit("STMT"); }
   ;

delete_stmt: DELETE delete_opts FROM NAME
    opt_where 
                  { driver.emit("DELETEONE"); }
;

delete_opts: /* nil */ { $$ = 0; }
   ;

delete_stmt: DELETE delete_opts
    delete_list
    FROM table_references opt_where
            { driver.emit("DELETEMULTI"); }

delete_list: NAME opt_dot_star { driver.emit("TABLE %s"); $$ = 1; }
   | delete_list ',' NAME opt_dot_star
            { driver.emit("TABLE"); $$ = $1 + 1; }
   ;

opt_dot_star: /* nil */ | '.' '*' ;

delete_stmt: DELETE delete_opts
    FROM delete_list
    USING table_references opt_where
            { driver.emit("DELETEMULTI"); }
;


   /* statements: insert statement */

stmt: insert_stmt { driver.emit("STMT");driver.insert_value(); }
   ;

insert_stmt: INSERT insert_opts opt_into NAME
     opt_col_names
     VALUES insert_vals_list
      { driver.emit("INSERTVALS %d %d %s");driver.set_table_name($4); }
   ;


insert_opts: /* nil */ { $$ = 0; }
   ;

opt_into: INTO | /* nil */
   ;

opt_col_names: /* nil */
   | '(' column_list ')' { driver.emit("INSERTCOLS %d"); }
   ;

insert_vals_list: '(' insert_vals ')' { driver.emit("VALUES %d"); $$ = 1; }
   | insert_vals_list ',' '(' insert_vals ')' { driver.emit("VALUES %d"); $$ = $1 + 1; }

insert_vals:
     expr { $$ = 1; }
   | DEFAULT { driver.emit("DEFAULT"); $$ = 1; }
   | insert_vals ',' expr { $$ = $1 + 1; }
   | insert_vals ',' DEFAULT { driver.emit("DEFAULT"); $$ = $1 + 1; }
   ;



   /** create index **/
stmt: create_index_stmt {}
   ;

create_index_stmt: CREATE KEY NAME ON NAME
   '(' NAME ')' {driver.create_index($3,$5,$7); }
   ;

   /** drop index **/
stmt: drop_index_stmt {}
    ;

drop_index_stmt: DROP KEY NAME {driver.drop_index($3);}
    ;

   /** drop table **/
stmt: drop_table_stmt {}
    ;
drop_table_stmt: DROP TABLE NAME {driver.drop_table($3);}
    ;


   /** create table **/
stmt: create_table_stmt { driver.emit("STMT"); driver.create_table();}
   ;

create_table_stmt: CREATE TABLE NAME
   '(' create_col_list ')' { driver.emit("CREATETABLE");
                            driver.set_table_name($3); }
   ;

create_col_list: create_definition { $$ = 1; }
    | create_col_list ',' create_definition { $$ = $1 + 1; }
    ;

create_definition: { driver.emit("STARTCOL"); } NAME data_type column_atts
                   { driver.emit("COLUMNDEF");driver.add_column_type($3);driver.add_column($2);}

    | PRIMARY KEY '(' column_list ')'    { driver.emit("PRIKEY"); }
    | KEY '(' column_list ')'            { driver.emit("KEY"); }
    | INDEX '(' column_list ')'          { driver.emit("KEY"); }
    | FULLTEXT INDEX '(' column_list ')' { driver.emit("TEXTINDEX"); }
    | FULLTEXT KEY '(' column_list ')'   { driver.emit("TEXTINDEX"); }
    ;

column_atts: /* nil */ { $$ = 0; }
    | column_atts NOT NULLX             { driver.emit("ATTR NOTNULL"); $$ = $1 + 1; }
    | column_atts NULLX
    | column_atts AUTO_INCREMENT        { driver.emit("ATTR AUTOINC"); $$ = $1 + 1; }
    | column_atts UNIQUE '(' column_list ')' { driver.emit("ATTR UNIQUEKEY"); $$ = $1 + 1; }
    | column_atts UNIQUE KEY { driver.emit("ATTR UNIQUEKEY"); $$ = $1 + 1; }
    | column_atts PRIMARY KEY { driver.emit("ATTR PRIKEY"); $$ = $1 + 1; }
    | column_atts KEY { driver.emit("ATTR PRIKEY"); $$ = $1 + 1; }
    ;

opt_length: /* nil */ { $$ = 0; }
   | '(' INTNUM ')' { $$ = $2; }
   | '(' INTNUM ',' INTNUM ')' { $$ = $2 + 1000*$4; }
   ;

opt_binary: /* nil */ { $$ = 0; }
   | BINARY { $$ = 4000; }
   ;

opt_uz: /* nil */ { $$ = 0; }
   | opt_uz UNSIGNED { $$ = $1 | 1000; }
   | opt_uz ZEROFILL { $$ = $1 | 2000; }
   ;

opt_csc: /* nil */
   | opt_csc CHAR SET STRING { driver.emit("COLCHARSET %s"); }
   | opt_csc COLLATE STRING { driver.emit("COLCOLLATE %s");  }
   ;

data_type:
    INT opt_length opt_uz { $$ = 40000 + $2 + $3; }
   | INTEGER opt_length opt_uz { $$ = 50000 + $2 + $3; }
   | DOUBLE opt_length opt_uz { $$ = 80000 + $2 + $3; }
   | FLOAT opt_length opt_uz { $$ = 90000 + $2 + $3; }
   | TIMESTAMP { $$ = 100003; }
   | CHAR opt_length opt_csc { $$ = 120000 + $2; }
   | VARCHAR '(' INTNUM ')' opt_csc { $$ = 130000 + $3; }
   | TEXT opt_binary opt_csc { $$ = 171000 + $2; }
   ;



   /**** expressions ****/

expr: NAME          { driver.emit("NAME"); }
   | USERVAR         { driver.emit("USERVAR");  }
   | NAME '.' NAME { driver.emit("FIELDNAME "); }
   | STRING        { driver.emit("STRING");driver.add_value($1); }
   | INTNUM        { driver.emit("NUMBER");driver.add_value($1); }
   | APPROXNUM     { driver.emit("FLOAT"); driver.add_value($1);}
   | BOOL          { driver.emit("BOOL"); }
   ;

expr: expr '+' expr { driver.emit("ADD"); }
   | expr '-' expr { driver.emit("SUB"); }
   | expr '*' expr { driver.emit("MUL"); }
   | expr '/' expr { driver.emit("DIV"); }
   | expr '%' expr { driver.emit("MOD"); }
   | expr MOD expr { driver.emit("MOD"); }
   | '-' expr %prec UMINUS { driver.emit("NEG"); }
   | expr ANDOP expr { driver.emit("AND"); }
   | expr OR expr { driver.emit("OR"); }
   | expr XOR expr { driver.emit("XOR"); }
   | expr COMPARISON expr { driver.emit("CMP"); }
   | expr COMPARISON '(' select_stmt ')' {driver. emit("CMPSELECT"); }
   | NOT expr { driver.emit("NOT"); }
   | '!' expr { driver.emit("NOT"); }
   | USERVAR ASSIGN expr { driver.emit("ASSIGN @"); }
   ;    

expr:  expr IS NULLX     { driver.emit("ISNULL"); }
   |   expr IS NOT NULLX { driver.emit("ISNULL"); driver.emit("NOT"); }
   |   expr IS BOOL      { driver.emit("ISBOOL"); }
   |   expr IS NOT BOOL  { driver.emit("ISBOOL"); driver.emit("NOT"); }
   ;
%%


void 
MC::MC_Parser::error( const location_type &l, const std::string &err_message )
{
   std::cerr << "Error: " << err_message << " at " << l << "\n";
}
