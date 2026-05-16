#include "database.h"
#include "../config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Conecta ao banco de dados MySQL */
MYSQL* db_connect(const char *host, const char *user, const char *pass, const char *database) {
    MYSQL *conn = mysql_init(NULL);
    
    if (conn == NULL) {
        fprintf(stderr, "Erro: mysql_init() falhou\n");
        return NULL;
    }
    
    if (mysql_real_connect(conn, host, user, pass, database, 0, NULL, 0) == NULL) {
        fprintf(stderr, "Erro ao conectar: %s\n", mysql_error(conn));
        mysql_close(conn);
        return NULL;
    }
    
    #if DEBUG_MODE
    printf("Conexão ao banco de dados estabelecida com sucesso!\n");
    #endif
    
    return conn;
}

/* Desconecta do banco de dados */
void db_disconnect(MYSQL *conn) {
    if (conn != NULL) {
        mysql_close(conn);
    }
}

/* Obtém todos os alunos do banco de dados */
int db_get_alunos(MYSQL *conn, Aluno *alunos, int max_count) {
    MYSQL_RES *result;
    MYSQL_ROW row;
    int count = 0;
    
    const char *query = "SELECT id, nome, email, matricula, turma_id FROM alunos ORDER BY nome";
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Erro na query: %s\n", mysql_error(conn));
        return 0;
    }
    
    result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "Erro ao armazenar resultado: %s\n", mysql_error(conn));
        return 0;
    }
    
    while ((row = mysql_fetch_row(result)) && count < max_count) {
        alunos[count].id = atoi(row[0]);
        strncpy(alunos[count].nome, row[1], 255);
        strncpy(alunos[count].email, row[2] ? row[2] : "", 255);
        strncpy(alunos[count].matricula, row[3] ? row[3] : "", 49);
        alunos[count].turma_id = atoi(row[4]);
        count++;
    }
    
    mysql_free_result(result);
    return count;
}

/* Obtém todas as turmas do banco de dados */
int db_get_turmas(MYSQL *conn, Turma *turmas, int max_count) {
    MYSQL_RES *result;
    MYSQL_ROW row;
    int count = 0;
    
    const char *query = "SELECT id, nome, ano_letivo, professor_id FROM turmas ORDER BY nome";
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Erro na query: %s\n", mysql_error(conn));
        return 0;
    }
    
    result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "Erro ao armazenar resultado: %s\n", mysql_error(conn));
        return 0;
    }
    
    while ((row = mysql_fetch_row(result)) && count < max_count) {
        turmas[count].id = atoi(row[0]);
        strncpy(turmas[count].nome, row[1], 99);
        turmas[count].ano_letivo = atoi(row[2]);
        turmas[count].professor_id = atoi(row[3]);
        count++;
    }
    
    mysql_free_result(result);
    return count;
}

/* Obtém notas de um aluno específico */
int db_get_notas_by_aluno(MYSQL *conn, int aluno_id, Nota *notas, int max_count) {
    MYSQL_RES *result;
    MYSQL_ROW row;
    int count = 0;
    char query[MAX_QUERY_SIZE];
    
    snprintf(query, MAX_QUERY_SIZE, 
        "SELECT id, aluno_id, disciplina, nota, data_avaliacao FROM notas WHERE aluno_id = %d ORDER BY data_avaliacao DESC",
        aluno_id);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Erro na query: %s\n", mysql_error(conn));
        return 0;
    }
    
    result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "Erro ao armazenar resultado: %s\n", mysql_error(conn));
        return 0;
    }
    
    while ((row = mysql_fetch_row(result)) && count < max_count) {
        notas[count].id = atoi(row[0]);
        notas[count].aluno_id = atoi(row[1]);
        strncpy(notas[count].disciplina, row[2], 99);
        notas[count].nota = atof(row[3]);
        strncpy(notas[count].data_avaliacao, row[4], 10);
        count++;
    }
    
    mysql_free_result(result);
    return count;
}

/* Obtém notas por disciplina */
int db_get_notas_by_disciplina(MYSQL *conn, const char *disciplina, Nota *notas, int max_count) {
    MYSQL_RES *result;
    MYSQL_ROW row;
    int count = 0;
    char query[MAX_QUERY_SIZE];
    
    snprintf(query, MAX_QUERY_SIZE,
        "SELECT id, aluno_id, disciplina, nota, data_avaliacao FROM notas WHERE disciplina = '%s' ORDER BY nota DESC",
        disciplina);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Erro na query: %s\n", mysql_error(conn));
        return 0;
    }
    
    result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "Erro ao armazenar resultado: %s\n", mysql_error(conn));
        return 0;
    }
    
    while ((row = mysql_fetch_row(result)) && count < max_count) {
        notas[count].id = atoi(row[0]);
        notas[count].aluno_id = atoi(row[1]);
        strncpy(notas[count].disciplina, row[2], 99);
        notas[count].nota = atof(row[3]);
        strncpy(notas[count].data_avaliacao, row[4], 10);
        count++;
    }
    
    mysql_free_result(result);
    return count;
}

/* Obtém um aluno específico por ID */
int db_get_aluno_by_id(MYSQL *conn, int id, Aluno *aluno) {
    MYSQL_RES *result;
    MYSQL_ROW row;
    char query[MAX_QUERY_SIZE];
    
    snprintf(query, MAX_QUERY_SIZE, "SELECT id, nome, email, matricula, turma_id FROM alunos WHERE id = %d", id);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Erro na query: %s\n", mysql_error(conn));
        return 0;
    }
    
    result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "Erro ao armazenar resultado: %s\n", mysql_error(conn));
        return 0;
    }
    
    if ((row = mysql_fetch_row(result))) {
        aluno->id = atoi(row[0]);
        strncpy(aluno->nome, row[1], 255);
        strncpy(aluno->email, row[2] ? row[2] : "", 255);
        strncpy(aluno->matricula, row[3] ? row[3] : "", 49);
        aluno->turma_id = atoi(row[4]);
        mysql_free_result(result);
        return 1;
    }
    
    mysql_free_result(result);
    return 0;
}

/* Obtém uma turma específica por ID */
int db_get_turma_by_id(MYSQL *conn, int id, Turma *turma) {
    MYSQL_RES *result;
    MYSQL_ROW row;
    char query[MAX_QUERY_SIZE];
    
    snprintf(query, MAX_QUERY_SIZE, "SELECT id, nome, ano_letivo, professor_id FROM turmas WHERE id = %d", id);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Erro na query: %s\n", mysql_error(conn));
        return 0;
    }
    
    result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "Erro ao armazenar resultado: %s\n", mysql_error(conn));
        return 0;
    }
    
    if ((row = mysql_fetch_row(result))) {
        turma->id = atoi(row[0]);
        strncpy(turma->nome, row[1], 99);
        turma->ano_letivo = atoi(row[2]);
        turma->professor_id = atoi(row[3]);
        mysql_free_result(result);
        return 1;
    }
    
    mysql_free_result(result);
    return 0;
}

/* Imprime erro do MySQL */
void db_print_error(MYSQL *conn) {
    fprintf(stderr, "Erro MySQL: %s\n", mysql_error(conn));
}
