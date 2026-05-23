#!/usr/bin/env python3
from __future__ import annotations

import argparse
import csv
import math
from collections import Counter
from pathlib import Path
from dataset_config import RAW_DATA_PATH, NUMERIC_COLUMNS, CATEGORICAL_COLUMNS, MISSING_VALUES

def is_missing(value: str | None) -> bool:
    return value is None or value.strip() in MISSING_VALUES


def to_float(value: str | None) -> float | None:
    if is_missing(value):
        return None
    return float(value)


def mean(values: list[float]) -> float:
    return sum(values) / len(values)


def sample_std(values: list[float]) -> float:
    if len(values) < 2:
        return 0.0
    avg = mean(values)
    variance = sum((value - avg) ** 2 for value in values) / (len(values) - 1)
    return math.sqrt(variance)


def median(values: list[float]) -> float:
    ordered = sorted(values)
    middle = len(ordered) // 2
    if len(ordered) % 2:
        return ordered[middle]
    return (ordered[middle - 1] + ordered[middle]) / 2


def quantile(values: list[float], q: float) -> float:
    ordered = sorted(values)
    position = (len(ordered) - 1) * q
    lower = math.floor(position)
    upper = math.ceil(position)
    if lower == upper:
        return ordered[int(position)]
    weight = position - lower
    return ordered[lower] * (1 - weight) + ordered[upper] * weight


def read_csv(path: Path) -> list[dict[str, str]]:
    with path.open("r", newline="", encoding="utf-8") as file:
        return list(csv.DictReader(file))


def numeric_values(rows: list[dict[str, str]], column: str) -> list[float]:
    values = []
    for row in rows:
        value = to_float(row.get(column))
        if value is not None:
            values.append(value)
    return values


def print_table(headers: list[str], rows: list[list[object]]) -> None:
    text_rows = [[str(value) for value in row] for row in rows]
    widths = [
        max(len(headers[index]), *(len(row[index]) for row in text_rows))
        for index in range(len(headers))
    ]

    header_line = " | ".join(
        headers[index].ljust(widths[index]) for index in range(len(headers))
    )
    divider = "-+-".join("-" * width for width in widths)

    print(header_line)
    print(divider)
    for row in text_rows:
        print(" | ".join(row[index].ljust(widths[index]) for index in range(len(headers))))


def explore(path: Path) -> None:
    rows = read_csv(path)
    if not rows:
        raise ValueError(f"O arquivo {path} esta vazio.")

    columns = list(rows[0].keys())
    duplicated_rows = len(rows) - len(
        {tuple(row.get(column, "") for column in columns) for row in rows}
    )

    print("=== Penguins Dataset ===")
    print(f"Arquivo: {path}")
    print(f"Linhas: {len(rows)}")
    print(f"Colunas: {len(columns)}")
    print(f"Linhas duplicadas: {duplicated_rows}")
    print()

    print("=== Colunas e valores ausentes ===")
    missing_rows = []
    for column in columns:
        expected_type = "numerica" if column in NUMERIC_COLUMNS else "categorica"
        missing_count = sum(1 for row in rows if is_missing(row.get(column)))
        missing_rows.append([column, expected_type, missing_count])
    print_table(["coluna", "tipo", "ausentes"], missing_rows)
    print()

    print("=== Distribuicao das colunas categoricas ===")
    for column in CATEGORICAL_COLUMNS:
        counts = Counter(
            row.get(column, "")
            for row in rows
            if not is_missing(row.get(column))
        )
        print(f"\n{column}")
        print_table(["valor", "frequencia"], [[value, count] for value, count in counts.most_common()])
    print()

    print("=== Estatisticas das colunas numericas ===")
    numeric_rows = []
    for column in NUMERIC_COLUMNS:
        values = numeric_values(rows, column)
        numeric_rows.append(
            [
                column,
                len(values),
                f"{mean(values):.3f}",
                f"{sample_std(values):.3f}",
                f"{min(values):.3f}",
                f"{quantile(values, 0.25):.3f}",
                f"{median(values):.3f}",
                f"{quantile(values, 0.75):.3f}",
                f"{max(values):.3f}",
            ]
        )
    print_table(
        ["coluna", "contagem", "media", "desvio", "min", "q1", "mediana", "q3", "max"],
        numeric_rows,
    )
    print()

    print("=== Primeiras 5 linhas ===")
    preview_rows = [[row[column] for column in columns] for row in rows[:5]]
    print_table(columns, preview_rows)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Le e explora o arquivo data/raw/penguins.csv."
    )
    parser.add_argument("--input", type=Path, default=RAW_DATA_PATH)
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    explore(args.input)


if __name__ == "__main__":
    main()
