# Pré-processamento
Etapas do pré-prcessamento aplicadas ao dataset bruto de peguins do Kaggle.
1. Remoção de linhas ausentes;
2. Verificação e remoção de linhas duplicadas;
3. Verificação e remoção de linhas com outliers, utilizando iqr primeiro e terceiro quartis;
4. Normalização das colunas numéricas, utilizando Min-Max 0 a 1, Essa etapa de normalização é essencial para o algoritmo do K-means, considerando que a diferença absoluta entre os valores presentes na coluna bill_length e flipper_length estão em escalas muito menores do que os valores de body_mass_g, que estão na casa dos milhares.
5. Shuffle das linhas, já que estavam em ordem crescente de ano seguido de espécie.
6. O fato de estarmos utilizando um K-means acabou sendo um impeditivo na seleção de features, uma feature que seria muito relevante seria a informação de sexo, o comum para uma feature binária seria utilizar o One-Hot Enconding, porém se fizermos isso a distância euclidiana entre um fêmea e um macho seria exatamente 1, ofuscando as outras features e dando importância máxima ao sexo do animal, o que prejudicaria na nossa tarefa de clusterização.
