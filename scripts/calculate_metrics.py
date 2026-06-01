import argparse


def read_float(label):
    while True:
        value = input(f"{label}: ").strip().replace(",", ".")
        try:
            number = float(value)
            if number <= 0:
                print("Informe um valor maior que zero.")
                continue
            return number
        except ValueError:
            print("Valor invalido. Exemplo: 0.000080000")


def read_int(label):
    while True:
        value = input(f"{label}: ").strip()
        try:
            number = int(value)
            if number <= 0:
                print("Informe um valor inteiro maior que zero.")
                continue
            return number
        except ValueError:
            print("Valor invalido. Exemplo: 333")


def calculate_metrics(baseline, measured_time, units, iterations, points):
    speedup = baseline / measured_time
    efficiency = speedup / units
    time_per_iteration = measured_time / iterations
    time_per_point = measured_time / points

    return {
        "Tempo sequencial": f"{baseline:.9f} s",
        "Tempo da versao testada": f"{measured_time:.9f} s",
        "Processos/threads usados": str(units),
        "Iteracoes": str(iterations),
        "Pontos": str(points),
        "Speedup": f"{speedup:.6f}",
        "Eficiencia": f"{efficiency:.6f}",
        "Tempo por iteracao": f"{time_per_iteration:.9f} s",
        "Tempo por ponto": f"{time_per_point:.9f} s",
    }


def print_metrics(metrics):
    print("\nMetricas calculadas")
    print("-" * 20)
    for name, value in metrics.items():
        print(f"{name}: {value}")


def parse_args():
    parser = argparse.ArgumentParser(
        description="Calcula speedup, eficiencia e tempos derivados para os experimentos."
    )
    parser.add_argument("--baseline", type=float, help="tempo da versao sequencial em segundos")
    parser.add_argument("--time", type=float, help="tempo da versao testada em segundos")
    parser.add_argument("--units", type=int, help="total de processos/threads usados")
    parser.add_argument("--iterations", type=int, help="quantidade de iteracoes do K-means")
    parser.add_argument("--points", type=int, help="quantidade de pontos processados")
    return parser.parse_args()


def main():
    args = parse_args()

    baseline = args.baseline if args.baseline else read_float("Tempo do sequencial em segundos")
    measured_time = args.time if args.time else read_float("Tempo dessa versao em segundos")
    units = args.units if args.units else read_int("Total de processos/threads usados")
    iterations = args.iterations if args.iterations else read_int("Iteracoes")
    points = args.points if args.points else read_int("Pontos processados")

    metrics = calculate_metrics(baseline, measured_time, units, iterations, points)
    print_metrics(metrics)


if __name__ == "__main__":
    main()
