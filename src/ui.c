#include "ui.h"
#include "gemini_api.h"
#include "../config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
    #define CLEAR_CMD "cls"
#else
    #define CLEAR_CMD "clear"
#endif

/* Limpa a tela */
void ui_clear_screen(void) {
    system(CLEAR_CMD);
}

/* Imprime cabeçalho */
void ui_print_header(const char *title) {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║  SGE Evolution Integration - %s\n", title);
    printf("╚════════════════════════════════════════════════════════════╝\n");
    printf("\n");
}

/* Imprime rodapé */
void ui_print_footer(void) {
    printf("\n────────────────────────────────────────────────────────────\n");
}

/* Imprime separador */
void ui_print_separator(void) {
    printf("────────────────────────────────────────────────────────────\n");
}

/* Menu principal */
int ui_main_menu(void) {
    int choice;
    
    ui_clear_screen();
    ui_print_header("MENU PRINCIPAL");
    
    printf("1. Listar Alunos\n");
    printf("2. Listar Turmas\n");
    printf("3. Listar Notas de Aluno\n");
    printf("4. Analisar Desempenho (IA)\n");
    printf("5. Gerar Relatório (IA)\n");
    printf("6. Prompt Customizado (IA)\n");
    printf("0. Sair\n");
    ui_print_footer();
    printf("Escolha uma opção: ");
    scanf("%d", &choice);
    getchar();
    
    return choice;
}

/* Lista alunos */
void ui_list_alunos(MYSQL *conn) {
    Aluno *alunos = malloc(sizeof(Aluno) * MAX_STUDENTS);
    if (!alunos) {
        ui_show_error("Erro ao alocar memória");
        return;
    }
    
    int count = db_get_alunos(conn, alunos, MAX_STUDENTS);
    
    ui_clear_screen();
    ui_print_header("LISTA DE ALUNOS");
    
    if (count == 0) {
        ui_show_error("Nenhum aluno encontrado");
        free(alunos);
        ui_press_enter_to_continue();
        return;
    }
    
    printf("%-4s | %-30s | %-25s | %-15s\n", "ID", "Nome", "Email", "Matrícula");
    ui_print_separator();
    
    for (int i = 0; i < count; i++) {
        printf("%-4d | %-30s | %-25s | %-15s\n",
            alunos[i].id,
            alunos[i].nome,
            alunos[i].email,
            alunos[i].matricula);
    }
    
    printf("\nTotal de alunos: %d\n", count);
    free(alunos);
    ui_press_enter_to_continue();
}

/* Lista turmas */
void ui_list_turmas(MYSQL *conn) {
    Turma *turmas = malloc(sizeof(Turma) * MAX_CLASSES);
    if (!turmas) {
        ui_show_error("Erro ao alocar memória");
        return;
    }
    
    int count = db_get_turmas(conn, turmas, MAX_CLASSES);
    
    ui_clear_screen();
    ui_print_header("LISTA DE TURMAS");
    
    if (count == 0) {
        ui_show_error("Nenhuma turma encontrada");
        free(turmas);
        ui_press_enter_to_continue();
        return;
    }
    
    printf("%-4s | %-30s | %-12s | %-12s\n", "ID", "Nome", "Ano Letivo", "Professor ID");
    ui_print_separator();
    
    for (int i = 0; i < count; i++) {
        printf("%-4d | %-30s | %-12d | %-12d\n",
            turmas[i].id,
            turmas[i].nome,
            turmas[i].ano_letivo,
            turmas[i].professor_id);
    }
    
    printf("\nTotal de turmas: %d\n", count);
    free(turmas);
    ui_press_enter_to_continue();
}

/* Lista notas de um aluno */
void ui_list_notas(MYSQL *conn) {
    int aluno_id;
    
    ui_clear_screen();
    ui_print_header("LISTAR NOTAS POR ALUNO");
    
    printf("Digite o ID do aluno: ");
    scanf("%d", &aluno_id);
    getchar();
    
    Aluno aluno;
    if (!db_get_aluno_by_id(conn, aluno_id, &aluno)) {
        ui_show_error("Aluno não encontrado");
        ui_press_enter_to_continue();
        return;
    }
    
    Nota *notas = malloc(sizeof(Nota) * MAX_GRADES);
    if (!notas) {
        ui_show_error("Erro ao alocar memória");
        return;
    }
    
    int count = db_get_notas_by_aluno(conn, aluno_id, notas, MAX_GRADES);
    
    ui_clear_screen();
    printf("\nNotas de: %s\n", aluno.nome);
    ui_print_separator();
    
    if (count == 0) {
        ui_show_error("Nenhuma nota registrada");
        free(notas);
        ui_press_enter_to_continue();
        return;
    }
    
    float soma = 0;
    printf("%-30s | %-8s | %-12s\n", "Disciplina", "Nota", "Data");
    ui_print_separator();
    
    for (int i = 0; i < count; i++) {
        printf("%-30s | %-8.2f | %-12s\n",
            notas[i].disciplina,
            notas[i].nota,
            notas[i].data_avaliacao);
        soma += notas[i].nota;
    }
    
    float media = soma / count;
    printf("\n" );
    ui_print_separator();
    printf("Média Geral: %.2f\n", media);
    
    free(notas);
    ui_press_enter_to_continue();
}

/* Analisa desempenho do aluno com IA */
void ui_analyze_student(MYSQL *conn) {
    int aluno_id;
    
    ui_clear_screen();
    ui_print_header("ANÁLISE DE DESEMPENHO (IA)");
    
    printf("Digite o ID do aluno: ");
    scanf("%d", &aluno_id);
    getchar();
    
    Aluno aluno;
    if (!db_get_aluno_by_id(conn, aluno_id, &aluno)) {
        ui_show_error("Aluno não encontrado");
        ui_press_enter_to_continue();
        return;
    }
    
    Nota *notas = malloc(sizeof(Nota) * MAX_GRADES);
    if (!notas) {
        ui_show_error("Erro ao alocar memória");
        return;
    }
    
    int count = db_get_notas_by_aluno(conn, aluno_id, notas, MAX_GRADES);
    
    if (count == 0) {
        ui_show_error("Nenhuma nota registrada para este aluno");
        free(notas);
        ui_press_enter_to_continue();
        return;
    }
    
    float soma = 0;
    for (int i = 0; i < count; i++) {
        soma += notas[i].nota;
    }
    float media = soma / count;
    
    printf("\nAnalisando desempenho de %s...\n", aluno.nome);
    printf("Aguarde enquanto a IA processa os dados...\n\n");
    
    GeminiResponse response = gemini_analyze_grades(aluno.nome, media, count);
    
    if (response.success) {
        ui_print_separator();
        printf("ANÁLISE DA IA:\n");
        ui_print_separator();
        printf("%s\n", response.content);
    } else {
        ui_show_error(response.error_message);
    }
    
    gemini_response_free(&response);
    free(notas);
    ui_press_enter_to_continue();
}

/* Gera relatório com IA */
void ui_generate_report(MYSQL *conn) {
    int turma_id;
    
    ui_clear_screen();
    ui_print_header("GERAR RELATÓRIO (IA)");
    
    printf("Digite o ID da turma: ");
    scanf("%d", &turma_id);
    getchar();
    
    Turma turma;
    if (!db_get_turma_by_id(conn, turma_id, &turma)) {
        ui_show_error("Turma não encontrada");
        ui_press_enter_to_continue();
        return;
    }
    
    printf("\nGerando relatório para %s...\n", turma.nome);
    printf("Aguarde enquanto a IA processa os dados...\n\n");
    
    char summary[512];
    snprintf(summary, sizeof(summary), "Turma: %s, Ano Letivo: %d", turma.nome, turma.ano_letivo);
    
    GeminiResponse response = gemini_generate_report(turma.nome, summary);
    
    if (response.success) {
        ui_print_separator();
        printf("RELATÓRIO GERADO PELA IA:\n");
        ui_print_separator();
        printf("%s\n", response.content);
    } else {
        ui_show_error(response.error_message);
    }
    
    gemini_response_free(&response);
    ui_press_enter_to_continue();
}

/* Prompt customizado */
void ui_custom_ai_prompt(void) {
    ui_clear_screen();
    ui_print_header("PROMPT CUSTOMIZADO");
    
    printf("Digite sua pergunta/prompt para a IA:\n");
    printf("(Máximo 2048 caracteres)\n\n");
    
    char prompt[2048];
    fgets(prompt, sizeof(prompt), stdin);
    
    /* Remove newline */
    size_t len = strlen(prompt);
    if (len > 0 && prompt[len-1] == '\n') {
        prompt[len-1] = '\0';
    }
    
    if (strlen(prompt) == 0) {
        ui_show_error("Prompt vazio");
        ui_press_enter_to_continue();
        return;
    }
    
    printf("\nEnviando para a IA...\n\n");
    
    GeminiResponse response = gemini_custom_prompt(prompt);
    
    if (response.success) {
        ui_print_separator();
        printf("RESPOSTA DA IA:\n");
        ui_print_separator();
        printf("%s\n", response.content);
    } else {
        ui_show_error(response.error_message);
    }
    
    gemini_response_free(&response);
    ui_press_enter_to_continue();
}

/* Lê uma linha do input */
char* ui_read_line(void) {
    char *buffer = malloc(MAX_BUFFER_SIZE);
    if (!buffer) return NULL;
    fgets(buffer, MAX_BUFFER_SIZE, stdin);
    return buffer;
}

/* Aguarda enter para continuar */
void ui_press_enter_to_continue(void) {
    printf("\nPressione ENTER para continuar...");
    getchar();
}

/* Mostra erro */
void ui_show_error(const char *message) {
    printf("\n❌ ERRO: %s\n", message);
}

/* Mostra sucesso */
void ui_show_success(const char *message) {
    printf("\n✅ SUCESSO: %s\n", message);
}
