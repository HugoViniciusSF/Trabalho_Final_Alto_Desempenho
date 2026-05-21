# Penguins Dataset

O arquivo `penguins.csv` contém medições de pinguins de diferentes espécies. Ele será usado como base para preparar os dados de entrada do K-means.

## Colunas

| Coluna | Descrição | Uso no projeto |
|---|---|---|
| `species` | Espécie do pinguim: Adelie, Gentoo ou Chinstrap. | Referência para interpretar os agrupamentos. |
| `island` | Ilha onde o pinguim foi observado. | Informação auxiliar. |
| `bill_length_mm` | Comprimento do bico em milímetros. | Atributo numérico para análise e K-means. |
| `bill_depth_mm` | Profundidade do bico em milímetros. | Atributo numérico para análise e K-means. |
| `flipper_length_mm` | Comprimento da nadadeira em milímetros. | Atributo numérico para análise e K-means. |
| `body_mass_g` | Massa corporal em gramas. | Atributo numérico para análise e K-means. |
| `sex` | Sexo registrado do pinguim. | Informação auxiliar. |
| `year` | Ano da observação. | Informação auxiliar. |

## Exploração inicial

Para visualizar um resumo do CSV:

```bash
python3 scripts/explore_penguins.py
```
