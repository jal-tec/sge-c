#include "gemini_api.h"
#include "../config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <ctype.h>

/* Estrutura para armazenar dados da resposta HTTP */
typedef struct {
    char *data;
    size_t size;
} HttpResponse;

/* Callback para armazenar resposta HTTP */
static size_t curl_write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    HttpResponse *mem = (HttpResponse *)userp;
    
    char *ptr = realloc(mem->data, mem->size + realsize + 1);
    if (!ptr) {
        fprintf(stderr, "Erro: não há memória suficiente para curl_callback\n");
        return 0;
    }
    
    mem->data = ptr;
    memcpy(&(mem->data[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->data[mem->size] = 0;
    
    return realsize;
}

/* Escapa caracteres especiais JSON */
char* gemini_escape_json_string(const char *input) {
    if (input == NULL) return NULL;
    
    char *output = malloc(strlen(input) * 2 + 1);
    if (!output) return NULL;
    
    int j = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        switch(input[i]) {
            case '"':
                output[j++] = '\\';
                output[j++] = '"';
                break;
            case '\\':
                output[j++] = '\\';
                output[j++] = '\\';
                break;
            case '\n':
                output[j++] = '\\';
                output[j++] = 'n';
                break;
            case '\r':
                output[j++] = '\\';
                output[j++] = 'r';
                break;
            case '\t':
                output[j++] = '\\';
                output[j++] = 't';
                break;
            default:
                output[j++] = input[i];
        }
    }
    output[j] = '\0';
    return output;
}

/* Função genérica para fazer requisição à Gemini API */
static GeminiResponse gemini_request(const char *prompt) {
    GeminiResponse response = {0};
    CURL *curl;
    CURLcode res;
    HttpResponse http_response = {0};
    char url[1024];
    
    if (!GEMINI_API_KEY || strlen(GEMINI_API_KEY) == 0) {
        response.success = 0;
        strcpy(response.error_message, "Chave da API Gemini não configurada");
        response.content = malloc(1);
        response.content[0] = '\0';
        return response;
    }
    
    http_response.data = malloc(1);
    http_response.size = 0;
    
    /* Monta URL com chave de API */
    snprintf(url, sizeof(url), "%s?key=%s", GEMINI_API_URL, GEMINI_API_KEY);
    
    /* Escapa o prompt */
    char *escaped_prompt = gemini_escape_json_string(prompt);
    if (!escaped_prompt) {
        response.success = 0;
        strcpy(response.error_message, "Erro ao processar prompt");
        response.content = malloc(1);
        response.content[0] = '\0';
        free(http_response.data);
        return response;
    }
    
    /* Monta payload JSON */
    char payload[4096];
    snprintf(payload, sizeof(payload),
        "{\"contents\":[{\"parts\":[{\"text\":\"%s\"}]}]}",
        escaped_prompt);
    
    curl = curl_easy_init();
    if (!curl) {
        response.success = 0;
        strcpy(response.error_message, "Erro ao inicializar CURL");
        response.content = malloc(1);
        response.content[0] = '\0';
        free(escaped_prompt);
        free(http_response.data);
        return response;
    }
    
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&http_response);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, HTTP_TIMEOUT);
    
    res = curl_easy_perform(curl);
    
    if (res != CURLE_OK) {
        response.success = 0;
        snprintf(response.error_message, sizeof(response.error_message),
            "Erro CURL: %s", curl_easy_strerror(res));
        response.content = malloc(1);
        response.content[0] = '\0';
    } else {
        response.success = 1;
        response.content = http_response.data;
    }
    
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    free(escaped_prompt);
    
    return response;
}

/* Analisa notas de um aluno */
GeminiResponse gemini_analyze_grades(const char *student_name, float average_grade, int total_grades) {
    char prompt[1024];
    
    snprintf(prompt, sizeof(prompt),
        "Analise o desempenho escolar do aluno %s com média %.2f em %d avaliações. Forneça feedback construtivo, identifique pontos fortes, áreas de melhoria e recomendações específicas. Responda em português.",
        student_name, average_grade, total_grades);
    
    return gemini_request(prompt);
}

/* Gera relatório de turma */
GeminiResponse gemini_generate_report(const char *class_name, const char *data_summary) {
    char prompt[2048];
    
    snprintf(prompt, sizeof(prompt),
        "Gere um relatório analítico para a turma %s com base nestes dados: %s. Inclua resumo de desempenho, estatísticas importantes e recomendações para melhorias. Responda em português.",
        class_name, data_summary);
    
    return gemini_request(prompt);
}

/* Prompt customizado */
GeminiResponse gemini_custom_prompt(const char *prompt) {
    if (!prompt) {
        GeminiResponse response = {0};
        response.success = 0;
        strcpy(response.error_message, "Prompt vazio");
        response.content = malloc(1);
        response.content[0] = '\0';
        return response;
    }
    
    return gemini_request(prompt);
}

/* Libera memória da resposta */
void gemini_response_free(GeminiResponse *response) {
    if (response && response->content) {
        free(response->content);
        response->content = NULL;
    }
}
