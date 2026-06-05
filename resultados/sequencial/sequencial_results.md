# Resultados da Execução Sequencial

## Saída do Terminal

```text
Carregou 333 pontos com 4 features.
Lendo as seguintes colunas: bill_length_mm, bill_depth_mm, flipper_length_mm, body_mass_g
Iniciando K-Means
K-means concluido com sucesso em 9 interacoes
Wall Clock Time: 0.000080000 segundos
Clusters salvo: ../../data/processed/results_sequencial.csv
```

## Observação

A execução utilizou o dataset tratado `data/processed/penguins_clean.csv`, com 333 pontos e 4 atributos numéricos. O arquivo gerado com os clusters foi salvo em `data/processed/results_sequencial.csv`.

## Métricas Calculadas

O tempo sequencial é a base da comparação. Como esta versão não usa paralelismo, o speedup e a eficiência ficam em 1.00.

| Métrica | Valor |
|---|---:|
| Tempo sequencial | 0.000080000 s |
| Iterações | 9 |
| Processos/threads usados | 1 |
| Speedup | 1.00 |
| Eficiência | 1.00 |
| Tempo por iteração | 0.000008889 s |
| Tempo por ponto | 0.000000240 s |
