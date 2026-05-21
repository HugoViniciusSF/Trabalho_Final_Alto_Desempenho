# Resultados iniciais da exploração dos dados

## Visão geral

O arquivo analisado foi `data/raw/penguins.csv`, correspondente ao Penguins Dataset.

| Informação | Valor |
|---|---:|
| Linhas | 344 |
| Colunas | 8 |
| Linhas duplicadas | 0 |

## Colunas e valores ausentes

| Coluna | Tipo | Valores ausentes |
|---|---|---:|
| `species` | Categórica | 0 |
| `island` | Categórica | 0 |
| `bill_length_mm` | Numérica | 2 |
| `bill_depth_mm` | Numérica | 2 |
| `flipper_length_mm` | Numérica | 2 |
| `body_mass_g` | Numérica | 2 |
| `sex` | Categórica | 11 |
| `year` | Categórica | 0 |

## Distribuição das colunas categóricas

### Espécies

| Espécie | Frequência |
|---|---:|
| Adelie | 152 |
| Gentoo | 124 |
| Chinstrap | 68 |

### Ilhas

| Ilha | Frequência |
|---|---:|
| Biscoe | 168 |
| Dream | 124 |
| Torgersen | 52 |

### Sexo

| Sexo | Frequência |
|---|---:|
| Male | 168 |
| Female | 165 |

### Ano

| Ano | Frequência |
|---|---:|
| 2009 | 120 |
| 2008 | 114 |
| 2007 | 110 |

## Estatísticas das colunas numéricas

| Coluna | Contagem | Média | Desvio padrão | Mínimo | Q1 | Mediana | Q3 | Máximo |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| `bill_length_mm` | 342 | 43.922 | 5.460 | 32.100 | 39.225 | 44.450 | 48.500 | 59.600 |
| `bill_depth_mm` | 342 | 17.151 | 1.975 | 13.100 | 15.600 | 17.300 | 18.700 | 21.500 |
| `flipper_length_mm` | 342 | 200.915 | 14.062 | 172.000 | 190.000 | 197.000 | 213.000 | 231.000 |
| `body_mass_g` | 342 | 4201.754 | 801.955 | 2700.000 | 3550.000 | 4050.000 | 4750.000 | 6300.000 |

## Amostra dos dados

| species | island | bill_length_mm | bill_depth_mm | flipper_length_mm | body_mass_g | sex | year |
|---|---|---:|---:|---:|---:|---|---:|
| Adelie | Torgersen | 39.1 | 18.7 | 181 | 3750 | male | 2007 |
| Adelie | Torgersen | 39.5 | 17.4 | 186 | 3800 | female | 2007 |
| Adelie | Torgersen | 40.3 | 18.0 | 195 | 3250 | female | 2007 |
| Adelie | Torgersen | NA | NA | NA | NA | NA | 2007 |
| Adelie | Torgersen | 36.7 | 19.3 | 193 | 3450 | female | 2007 |

## Observações iniciais

O dataset possui três espécies e registros distribuídos entre três ilhas. Não foram encontradas linhas duplicadas.

Para o K-means, os principais atributos de entrada são `bill_length_mm`, `bill_depth_mm`, `flipper_length_mm` e `body_mass_g`. Antes da implementação do algoritmo, será necessário definir o tratamento dos valores ausentes e a padronização dos atributos numéricos.
