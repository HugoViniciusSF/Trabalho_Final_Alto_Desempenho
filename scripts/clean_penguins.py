#!/usr/bin/env python3
from __future__ import annotations

import argparse
import csv
import random
from pathlib import Path
from dataset_config import NUMERIC_COLUMNS

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

def quantile(values: list[float], q: float) -> float:
    ordered = sorted(values)
    position = (len(ordered) - 1) * q
    lower = int(position)
    upper = lower + 1 if position > lower else lower
        
    if lower == upper:
        return ordered[lower]
        
    weight = position - lower
    return ordered[lower] * (1 - weight) + ordered[upper] * weight

def row_has_missing_value(row: dict[str, str], columns: list[str]) -> bool:
    return any(is_missing(row.get(column)) for column in columns)

def count_duplicate_rows(rows: list[dict[str, str]]) -> int:
    columns = list(rows[0].keys())
    unique_rows = {tuple(row.get(column, "") for column in columns) for row in rows} ## Set de tuplas (evita adicionar repetidos)
    ## A diferença entre o total de linhas e o tamanho do set é o número de duplicatas
    return len(rows) - len(unique_rows)

def remove_duplicate_rows(rows: list[dict[str, str]]) -> list[dict[str, str]]:
    seen = set()
    unique_rows = []
        
    columns = list(rows[0].keys())
    for row in rows:
        row_tuple = tuple(row.get(column, "") for column in columns)
        if row_tuple not in seen:
            seen.add(row_tuple)
            unique_rows.append(row)
    return unique_rows

def remove_outliers_iqr(rows: list[dict[str, str]], column: str, multiplier: float = 1.5) -> list[dict[str, str]]:
    values = []
    for row in rows:
        val_str = row.get(column)
        if val_str and val_str.replace('.', '', 1).isdigit():
            values.append(float(val_str))
            
    if not values:
        return rows

    q1 = quantile(values, 0.25)
    q3 = quantile(values, 0.75)
    iqr = q3 - q1
    
    lower_bound = q1 - multiplier * iqr
    upper_bound = q3 + multiplier * iqr
    clean_rows = []
    for row in rows:
        val_str = row.get(column)
        if not val_str or not val_str.replace('.', '', 1).isdigit():
            clean_rows.append(row)
            continue
        val = float(val_str)
        
        if lower_bound <= val <= upper_bound:
            clean_rows.append(row)
            
    return clean_rows

def normalize_min_max(rows: list[dict[str, str]], columns_to_normalize: list[str]) -> list[dict[str, str]]:
    min_max_vals = {}
    for col in columns_to_normalize:
        vals = [float(row[col]) for row in rows if row.get(col) and row[col].replace('.', '', 1).isdigit()]
        if vals:
            min_max_vals[col] = {"min": min(vals), "max": max(vals)}
            
    normalized_rows = []
    for row in rows:
        new_row = row.copy()
        for col in columns_to_normalize:
            if col in min_max_vals and new_row.get(col) and new_row[col].replace('.', '', 1).isdigit():
                x = float(new_row[col])
                min_val = min_max_vals[col]["min"]
                max_val = min_max_vals[col]["max"]
                
                if max_val - min_val == 0:
                    new_row[col] = "0.0"
                else:
                    x_norm = (x - min_val) / (max_val - min_val)
                    new_row[col] = str(round(x_norm, 6))
        normalized_rows.append(new_row)
    return normalized_rows

def shuffle_dataset(rows: list[dict[str, str]], seed: int = 99) -> list[dict[str, str]]:
    random.seed(seed)
    random.shuffle(rows)
    return rows

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

    duplicates_count = count_duplicate_rows(clean_rows)
    if duplicates_count > 0:
        clean_rows = remove_duplicate_rows(clean_rows)
    
    for col in NUMERIC_COLUMNS:
        clean_rows = remove_outliers_iqr(clean_rows, col)

    clean_rows = normalize_min_max(clean_rows, NUMERIC_COLUMNS)
    clean_rows = shuffle_dataset(clean_rows)
    
    write_csv(output_path, columns, clean_rows)

    removed_rows = len(rows) - len(clean_rows)
    print(f"Arquivo original: {input_path}")
    print(f"Arquivo limpo: {output_path}")
    print(f"Linhas originais: {len(rows)}")
    print(f"Linhas duplicadas removidas: {duplicates_count}")
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
