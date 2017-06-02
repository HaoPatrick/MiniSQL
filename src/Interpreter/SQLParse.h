#pragma once
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

void emit(char *s, ...)
{
  extern yylineno;

  va_list ap;
  va_start(ap, s);

  printf("rpn: ");
  vfprintf(stdout, s, ap);
  printf("\n");
}

void yyerror(char *s, ...)
{
  extern yylineno;

  va_list ap;
  va_start(ap, s);

  fprintf(stderr, "%d: error: ", yylineno);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
}

main(int ac, char **av)
{
  extern FILE *yyin;
  extern yydebug;
  printf("> ");
  if (ac > 1 && !strcmp(av[1], "-d"))
  {
    yydebug = 1;
    ac--;
    av++;
  }

  if (ac > 1 && (yyin = fopen(av[1], "r")) == NULL)
  {
    perror(av[1]);
    exit(1);
  }

  if (!yyparse())
    printf("SQL parse worked\n");
  else
    printf("SQL parse failed\n");
} /* main */
