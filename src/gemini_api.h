#ifndef GEMINI_API_H
#define GEMINI_API_H

/* Estrutura para armazenar resposta da API */
typedef struct {
    char *content;
    int success;
    char error_message[512];
} GeminiResponse;

/* Funções da Gemini API */

GeminiResponse gemini_analyze_grades(const char *student_name, float average_grade, int total_grades);
GeminiResponse gemini_generate_report(const char *class_name, const char *data_summary);
GeminiResponse gemini_custom_prompt(const char *prompt);
void gemini_response_free(GeminiResponse *response);
char* gemini_escape_json_string(const char *input);

#endif // GEMINI_API_H
