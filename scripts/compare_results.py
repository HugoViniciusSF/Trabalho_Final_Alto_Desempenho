import argparse
import csv
import sys


REQUIRED_COLUMNS = {"point_id", "cluster_id"}


def read_clusters(path):
    clusters = {}

    with open(path, newline="", encoding="utf-8") as csv_file:
        reader = csv.DictReader(csv_file)

        if reader.fieldnames is None:
            raise ValueError(f"O arquivo {path} está vazio.")

        columns = set(reader.fieldnames)
        missing_columns = REQUIRED_COLUMNS - columns
        if missing_columns:
            missing = ", ".join(sorted(missing_columns))
            raise ValueError(f"O arquivo {path} não possui as colunas: {missing}.")

        for row_number, row in enumerate(reader, start=2):
            point_id = row["point_id"].strip()
            cluster_id = row["cluster_id"].strip()

            if not point_id:
                raise ValueError(f"Linha {row_number} de {path} sem point_id.")

            if point_id in clusters:
                raise ValueError(f"point_id duplicado em {path}: {point_id}.")

            clusters[point_id] = cluster_id

    return clusters


def sort_point_ids(point_ids):
    def sort_key(point_id):
        try:
            return (0, int(point_id))
        except ValueError:
            return (1, point_id)

    return sorted(point_ids, key=sort_key)


def compare_clusters(base_clusters, test_clusters):
    base_ids = set(base_clusters)
    test_ids = set(test_clusters)
    common_ids = sort_point_ids(base_ids & test_ids)

    differences = []
    equal_count = 0

    for point_id in common_ids:
        base_cluster = base_clusters[point_id]
        test_cluster = test_clusters[point_id]

        if base_cluster == test_cluster:
            equal_count += 1
        else:
            differences.append((point_id, base_cluster, test_cluster))

    compared = len(common_ids)
    agreement = (equal_count / compared * 100) if compared else 0.0

    return {
        "base_total": len(base_clusters),
        "test_total": len(test_clusters),
        "compared": compared,
        "equal_count": equal_count,
        "different_count": len(differences),
        "agreement": agreement,
        "missing_in_test": sort_point_ids(base_ids - test_ids),
        "extra_in_test": sort_point_ids(test_ids - base_ids),
        "differences": differences,
    }


def print_metric_table(result):
    rows = [
        ("Pontos na base", str(result["base_total"])),
        ("Pontos no teste", str(result["test_total"])),
        ("Pontos comparados", str(result["compared"])),
        ("Pontos iguais", str(result["equal_count"])),
        ("Pontos diferentes", str(result["different_count"])),
        ("Concordância", f"{result['agreement']:.2f}%"),
        ("Pontos ausentes no teste", str(len(result["missing_in_test"]))),
        ("Pontos extras no teste", str(len(result["extra_in_test"]))),
    ]

    print("| Métrica | Valor |")
    print("|---|---:|")
    for metric, value in rows:
        print(f"| {metric} | {value} |")


def print_differences(differences, limit):
    if not differences:
        print("\nNenhuma diferença de cluster encontrada.")
        return

    print("\n| point_id | cluster_base | cluster_teste |")
    print("|---:|---:|---:|")
    for point_id, base_cluster, test_cluster in differences[:limit]:
        print(f"| {point_id} | {base_cluster} | {test_cluster} |")

    remaining = len(differences) - limit
    if remaining > 0:
        print(f"\nOutras diferenças não exibidas: {remaining}")


def parse_args():
    parser = argparse.ArgumentParser(
        description="Compara os clusters gerados por duas versões do K-means."
    )
    parser.add_argument("base_csv", help="arquivo CSV de referência")
    parser.add_argument("test_csv", help="arquivo CSV da versão comparada")
    parser.add_argument(
        "--limit",
        type=int,
        default=20,
        help="quantidade máxima de diferenças exibidas",
    )
    return parser.parse_args()


def main():
    args = parse_args()

    try:
        base_clusters = read_clusters(args.base_csv)
        test_clusters = read_clusters(args.test_csv)
        result = compare_clusters(base_clusters, test_clusters)
    except (OSError, ValueError) as error:
        print(f"Erro: {error}", file=sys.stderr)
        return 1

    print("# Comparação de Clusters")
    print()
    print(f"Base: `{args.base_csv}`")
    print(f"Teste: `{args.test_csv}`")
    print()
    print_metric_table(result)
    print_differences(result["differences"], args.limit)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
