# SGE Evolution Integration

Sistema de integração com SGE (Sistema de Gestão Escolar) desenvolvido em C99 com IA integrada via Gemini API.

## Funcionalidades

✅ Leitura de dados do banco de dados MySQL
✅ Apresentação formatada e intuitiva dos dados
✅ Integração com Gemini AI para análise inteligente
✅ Interface de fácil acesso
✅ C99 com máxima compatibilidade

## Requisitos

- GCC (ou compatível com C99)
- MySQL/MariaDB instalado
- libmysqlclient-dev (MySQL C API)
- curl (para requisições HTTP)
- Chave de API do Google Gemini

## Instalação

### 1. Dependências (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install build-essential libmysqlclient-dev curl libcurl4-openssl-dev
```

### 2. Configuração

Edite o arquivo `config.h` com suas credenciais:

```c
#define DB_HOST "localhost"
#define DB_USER "seu_usuario"
#define DB_PASS "sua_senha"
#define DB_NAME "sge_database"
#define GEMINI_API_KEY "sua_chave_gemini"
```

### 3. Compilação

```bash
make
```

### 4. Execução

```bash
./sge-integration
```

## Uso

O programa oferece um menu interativo:

1. **Listar Alunos** - Exibe todos os alunos cadastrados
2. **Listar Turmas** - Exibe todas as turmas
3. **Listar Notas** - Exibe notas por aluno/disciplina
4. **Análise IA** - Solicita análise inteligente ao Gemini
5. **Relatórios** - Gera relatórios automáticos
6. **Sair** - Encerra o programa

## Estrutura do Projeto

```
sge-c/
├── src/
│   ├── main.c              # Programa principal
│   ├── database.c          # Funções de conexão/leitura do MySQL
│   ├── database.h          # Header do database
│   ├── gemini_api.c        # Integração com Gemini
│   ├── gemini_api.h        # Header do Gemini
│   ├── ui.c                # Interface de usuário
│   └── ui.h                # Header da UI
├── config.h                # Configurações (credenciais)
├── Makefile                # Script de compilação
└── README.md               # Este arquivo
```

## Configuração do Banco de Dados

Crie as tabelas necessárias:

```sql
CREATE DATABASE sge_database;
USE sge_database;

CREATE TABLE alunos (
    id INT PRIMARY KEY AUTO_INCREMENT,
    nome VARCHAR(255) NOT NULL,
    email VARCHAR(255),
    matricula VARCHAR(50) UNIQUE,
    turma_id INT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE turmas (
    id INT PRIMARY KEY AUTO_INCREMENT,
    nome VARCHAR(100) NOT NULL,
    ano_letivo INT,
    professor_id INT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE notas (
    id INT PRIMARY KEY AUTO_INCREMENT,
    aluno_id INT NOT NULL,
    disciplina VARCHAR(100),
    nota DECIMAL(5,2),
    data_avaliacao DATE,
    FOREIGN KEY (aluno_id) REFERENCES alunos(id)
);
```

## Obtendo Chave Gemini API

1. Acesse [Google AI Studio](https://makersuite.google.com/app/apikey)
2. Clique em "Create API Key"
3. Copie a chave
4. Adicione em `config.h`

## Licença

MIT License - Livre para uso e modificação

## Contribuições

Contribuições são bem-vindas! Abra uma issue ou pull request.

## Suporte

Para dúvidas ou problemas, abra uma issue no GitHub.
