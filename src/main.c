#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"
#include "gemini_api.h"
#include "ui.h"
#include "../config.h"

/* Função principal */
int main(int argc, char *argv[]) {
    MYSQL *conn = NULL;
    int choice;
    
    /* Conecta ao banco de dados */
    printf("Conectando ao banco de dados...\n");
    conn = db_connect(DB_HOST, DB_USER, DB_PASS, DB_NAME);
    
    if (conn == NULL) {
        fprintf(stderr, "\n❌ Falha crítica: Não foi possível conectar ao banco de dados.\n");
        fprintf(stderr, "Verifique:\n");
        fprintf(stderr, "  - Se MySQL está rodando\n");
        fprintf(stderr, "  - Host: %s\n", DB_HOST);
        fprintf(stderr, "  - Usuário: %s\n", DB_USER);
        fprintf(stderr, "  - Banco: %s\n", DB_NAME);
        return 1;
    }
    
    printf("✅ Conectado com sucesso!\n\n");
    printf("Aguarde...");
    
    /* Loop principal */
    while (1) {
        choice = ui_main_menu();
        
        switch (choice) {
            case 1:
                ui_list_alunos(conn);
                break;
            case 2:
                ui_list_turmas(conn);
                break;
            case 3:
                ui_list_notas(conn);
                break;
            case 4:
                ui_analyze_student(conn);
                break;
            case 5:
                ui_generate_report(conn);
                break;
            case 6:
                ui_custom_ai_prompt();
                break;
            case 0:
                printf("\n👋 Encerrando aplicação...\n\n");
                db_disconnect(conn);
                return 0;
            default:
                printf("\n❌ Opção inválida! Tente novamente.\n");
                getchar();
        }
    }
    
    db_disconnect(conn);
    return 0;
}
