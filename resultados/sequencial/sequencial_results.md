# Resultados da Execucao Sequencial

## Saida do Terminal

```text
Carregou 333 pontos com 4 features.
Lendo as seguintes colunas: bill_length_mm, bill_depth_mm, flipper_length_mm, body_mass_g
Iniciando K-Means
K-means concluido com sucesso em 9 interacoes
Wall Clock Time: 0.000080000 segundos
Clusters salvo: ../../data/processed/results_sequencial.csv
```

## Observacao

A execucao utilizou o dataset tratado `data/processed/penguins_clean.csv`, com 333 pontos e 4 atributos numericos. O arquivo gerado com os clusters foi salvo em `data/processed/results_sequencial.csv`.

## Metricas Calculadas

O tempo sequencial e a base da comparacao. Como esta versao nao usa paralelismo, o speedup e a eficiencia ficam em 1.00.

| Metrica | Valor |
|---|---:|
| Tempo sequencial | 0.000080000 s |
| Iteracoes | 9 |
| Processos/threads usados | 1 |
| Speedup | 1.00 |
| Eficiencia | 1.00 |
| Tempo por iteracao | 0.000008889 s |
| Tempo por ponto | 0.000000240 s |
