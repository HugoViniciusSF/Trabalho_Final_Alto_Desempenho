#ifndef CONFIG_H
#define CONFIG_H

#define CLEAN_DATA_PATH "../../data/processed/penguins_clean.csv"
// #define CLEAN_DATA_PATH "../../data/processed/penguins_expanded.csv"

#define NUM_CLUSTERS 3
#define NUM_FEATURES 4
static const char *FEATURE_NAMES[] = {
    "bill_length_mm",
    "bill_depth_mm",
    "flipper_length_mm",
    "body_mass_g"
};

#endif