#!/usr/bin/env python3
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

RAW_DATA_PATH = ROOT / "data" / "raw" / "penguins.csv"
PROCESSED_DATA_PATH = ROOT / "data" / "processed" / "penguins_clean.csv"

NUMERIC_COLUMNS = [
    "bill_length_mm",
    "bill_depth_mm",
    "flipper_length_mm",
    "body_mass_g",
]

CATEGORICAL_COLUMNS = ["species", "island", "sex", "year"]
MISSING_VALUES = {"", "NA", "NaN", "nan", "null", "None"}