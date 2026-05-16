#ifndef UI_H
#define UI_H

#include "database.h"
#include <mysql.h>

/* Funções de Interface de Usuário */

void ui_clear_screen(void);
void ui_print_header(const char *title);
void ui_print_footer(void);
void ui_print_separator(void);
int ui_main_menu(void);
void ui_list_alunos(MYSQL *conn);
void ui_list_turmas(MYSQL *conn);
void ui_list_notas(MYSQL *conn);
void ui_analyze_student(MYSQL *conn);
void ui_generate_report(MYSQL *conn);
void ui_custom_ai_prompt(void);
char* ui_read_line(void);
void ui_press_enter_to_continue(void);
void ui_show_error(const char *message);
void ui_show_success(const char *message);

#endif // UI_H
