#ifndef DATABASE_H
#define DATABASE_H

#include <mysql.h>

/* Estruturas de dados */

typedef struct {
    int id;
    char nome[256];
    char email[256];
    char matricula[50];
    int turma_id;
} Aluno;

typedef struct {
    int id;
    char nome[100];
    int ano_letivo;
    int professor_id;
} Turma;

typedef struct {
    int id;
    int aluno_id;
    char disciplina[100];
    float nota;
    char data_avaliacao[11];
} Nota;

/* Funções de banco de dados */

MYSQL* db_connect(const char *host, const char *user, const char *pass, const char *database);
void db_disconnect(MYSQL *conn);
int db_get_alunos(MYSQL *conn, Aluno *alunos, int max_count);
int db_get_turmas(MYSQL *conn, Turma *turmas, int max_count);
int db_get_notas_by_aluno(MYSQL *conn, int aluno_id, Nota *notas, int max_count);
int db_get_notas_by_disciplina(MYSQL *conn, const char *disciplina, Nota *notas, int max_count);
int db_get_aluno_by_id(MYSQL *conn, int id, Aluno *aluno);
int db_get_turma_by_id(MYSQL *conn, int id, Turma *turma);
void db_print_error(MYSQL *conn);

#endif // DATABASE_H
