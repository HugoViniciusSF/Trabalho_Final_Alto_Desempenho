#!/usr/bin/env python3
from __future__ import annotations

import argparse
import csv
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_INPUT = ROOT / "data" / "raw" / "penguins.csv"
DEFAULT_OUTPUT = ROOT / "data" / "processed" / "penguins_clean.csv"
MISSING_VALUES = {"", "NA", "NaN", "nan", "null", "None"}


def is_missing(value: str | None) -> bool:
    return value is None or value.strip() in MISSING_VALUES


def read_csv(path: Path) -> tuple[list[str], list[dict[str, str]]]:
    with path.open("r", newline="", encoding="utf-8") as file:
        reader = csv.DictReader(file)
        return list(reader.fieldnames or []), list(reader)


def row_has_missing_value(row: dict[str, str], columns: list[str]) -> bool:
    return any(is_missing(row.get(column)) for column in columns)


def write_csv(path: Path, columns: list[str], rows: list[dict[str, str]]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", newline="", encoding="utf-8") as file:
        writer = csv.DictWriter(file, fieldnames=columns)
        writer.writeheader()
        writer.writerows(rows)


def clean_csv(input_path: Path, output_path: Path) -> None:
    columns, rows = read_csv(input_path)
    if not columns:
        raise ValueError(f"O arquivo {input_path} nao possui cabecalho.")

    clean_rows = [
        row for row in rows
        if not row_has_missing_value(row, columns)
    ]

    write_csv(output_path, columns, clean_rows)

    removed_rows = len(rows) - len(clean_rows)
    print(f"Arquivo original: {input_path}")
    print(f"Arquivo limpo: {output_path}")
    print(f"Linhas originais: {len(rows)}")
    print(f"Linhas removidas: {removed_rows}")
    print(f"Linhas salvas: {len(clean_rows)}")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Remove registros com valores vazios do penguins.csv."
    )
    parser.add_argument("--input", type=Path, default=DEFAULT_INPUT)
    parser.add_argument("--output", type=Path, default=DEFAULT_OUTPUT)
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    clean_csv(args.input, args.output)


if __name__ == "__main__":
    main()
