# Projeto 3ª Unidade

Disciplina: **IMD1116 - Computação de Alto Desempenho**

Tema: **Implementação e análise de desempenho do algoritmo K-means paralelo**

Dataset escolhido: **Penguins Dataset**

## Integrantes

- ARTHUR FERREIRA DE HOLANDA
- GABRIEL COSTA LIMA DANTAS
- HUGO VINICIUS DA SILVA FIGUEIREDO
- RAFAEL GOMES DANTAS GURGEL SIQUEIRA
- TALES VINICIUS DE MEDEIROS ALVES

## Objetivo do Projeto

O objetivo do projeto é implementar e comparar diferentes versões do algoritmo de agrupamento **K-means**, avaliando o desempenho de abordagens sequencial e paralelas em CPU e GPU.

As versões planejadas são:

| Versão | Modelo de execução | Objetivo |
|--------|--------------------|----------|
| Sequencial | CPU | Servir como baseline para comparação |
| MPI + OpenMP | CPU com processos e threads | Explorar paralelismo híbrido em memória distribuída/compartilhada |
| OpenMP GPU | GPU via diretivas OpenMP | Avaliar offload para GPU com uma abordagem baseada em diretivas |
| CUDA | GPU com CUDA | Comparar uma implementação mais explícita em GPU |

## Dataset: Penguins

O **Penguins Dataset** contém medições de pinguins de diferentes espécies. Ele é frequentemente usado em tarefas de análise exploratória, classificação e agrupamento por possuir variáveis numéricas úteis para separar grupos naturalmente.

Algumas variáveis típicas do dataset são:

- comprimento do bico;
- profundidade do bico;
- comprimento da nadadeira;
- massa corporal;
- espécie;
- ilha;
- sexo.

Para o K-means, o foco será utilizar principalmente os atributos numéricos. As colunas categóricas podem ser usadas apenas para análise posterior dos grupos ou removidas durante o pré-processamento.

## O que será feito

1. Preparar o dataset, tratando valores ausentes e selecionando atributos numéricos.
2. Implementar uma versão sequencial do K-means.
3. Implementar uma versão paralela com MPI + OpenMP.
4. Implementar uma versão com OpenMP GPU.
5. Implementar uma versão com CUDA.
6. Medir tempo de execução, speedup, eficiência e escalabilidade.
7. Comparar os resultados entre CPU e GPU.
8. Discutir custos de comunicação, balanceamento de carga e transferência de dados entre CPU e GPU.

## Métricas de Avaliação

As implementações serão comparadas usando:

- tempo de execução;
- speedup;
- eficiência;
- escalabilidade forte;
- escalabilidade fraca;
- impacto da comunicação no MPI;
- impacto da transferência de dados entre CPU e GPU;
- qualidade/estabilidade dos agrupamentos.

## Cronograma

O cronograma abaixo foi retirado do guia do projeto e serve como referência para organizar as entregas do grupo.

| Data | Atividade | Organização sugerida |
|------|-----------|----------------------|
| 19/05 | Definição das equipes e sorteio dos datasets | Confirmar integrantes e dataset Penguins |
| 23/05 | Primeira reunião do grupo | Registrar ata, decisões iniciais e divisão preliminar de tarefas |
| 30/05 | Segunda reunião do grupo | Validar pré-processamento do dataset e iniciar baseline sequencial |
| 13/06 | Terceira reunião do grupo | Revisar implementações paralelas e planejar experimentos |
| 19/06 | Último dia para enviar todas as atividades | Fechar código, resultados, README e materiais de entrega |
| 22/06 e 23/06 | Arguição da segunda lista | Preparar respostas e revisar conceitos relacionados |
| 30/06 e 02/07 | Apresentação do projeto | Apresentação oral, demonstração do código e discussão dos resultados |
| 07/07 | Dúvidas sobre a quarta prova | Revisão final da disciplina |
| 09/07 | Quarta prova | Avaliação final |

Para facilitar o andamento, o grupo pode usar as reuniões para atualizar três pontos: o que foi concluído, o que está bloqueado e o que cada integrante fará até o próximo encontro.

## Estrutura do Projeto

```text
PROJETO_FINAL_ALTO_DESEMPENHO/
├── README.md
├── docs/
│   └── Projeto3Unidade.pdf
├── data/
│   ├── raw/
│   └── processed/
├── src/
│   ├── sequencial/
│   ├── mpi_openmp/
│   ├── openmp_gpu/
│   └── cuda/
├── scripts/
├── resultados/
└── apresentacao/
```

## Organização das Pastas

| Pasta | Conteúdo |
|-------|----------|
| `docs/` | Guia do projeto e documentos auxiliares |
| `data/raw/` | Dataset original baixado do Kaggle |
| `data/processed/` | Dados tratados para execução do K-means |
| `src/sequencial/` | Implementação sequencial |
| `src/mpi_openmp/` | Implementação híbrida com MPI + OpenMP |
| `src/openmp_gpu/` | Implementação com OpenMP offload para GPU |
| `src/cuda/` | Implementação CUDA |
| `scripts/` | Scripts de preparação, execução e coleta de resultados |
| `resultados/` | Tabelas, logs e gráficos de desempenho |
| `apresentacao/` | Materiais usados na apresentação final |

## Guia do Projeto

O enunciado fornecido pelos professores está disponível em:

[docs/Projeto3Unidade.pdf](docs/Projeto3Unidade.pdf)
