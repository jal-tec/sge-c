#ifndef CONFIG_H
#define CONFIG_H

/* ============================================
   CONFIGURAÇÕES DO BANCO DE DADOS MySQL
   ============================================ */

#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASS "sua_senha_aqui"
#define DB_NAME "sge_database"
#define DB_PORT 3306

/* ============================================
   CONFIGURAÇÕES DA GEMINI API
   ============================================ */

#define GEMINI_API_KEY "sua_chave_gemini_aqui"
#define GEMINI_API_URL "https://generativelanguage.googleapis.com/v1beta/models/gemini-pro:generateContent"

/* ============================================
   CONFIGURAÇÕES DA APLICAÇÃO
   ============================================ */

#define MAX_BUFFER_SIZE 4096
#define MAX_QUERY_SIZE 2048
#define MAX_STUDENTS 1000
#define MAX_CLASSES 100
#define MAX_GRADES 5000

/* Timeout para requisições HTTP (segundos) */
#define HTTP_TIMEOUT 30

/* Debug mode (1 = ativado, 0 = desativado) */
#define DEBUG_MODE 0

#endif // CONFIG_H
