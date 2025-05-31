import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os
import glob
from pathlib import Path

class Type1ChartGenerator:
    def __init__(self, results_path="../Results", output_dir="charts_type1"):
        """
        Initialize the Type 1 chart generator

        Args:
            results_path: Directory containing CSV files
            output_dir: Directory to save generated charts
        """
        self.results_path = Path(results_path)
        self.output_dir = Path(output_dir)
        self.output_dir.mkdir(exist_ok=True)

    def find_type1_csv_files(self, pattern="Type1_*.csv"):
        """Find all Type 1 CSV files matching the pattern"""
        return list(self.results_path.glob(pattern))

    def create_type1_charts(self, problem_type="MST"):
        """
        Create Type 1 charts (separate charts for each graph representation)

        Args:
            problem_type: Type of problem (MST, SHORTEST_PATH, MAX_FLOW)
        """
        # Find CSV files for the specific problem type
        csv_files = self.find_type1_csv_files(f"Type1_{problem_type}_*.csv")

        if not csv_files:
            print(f"Nie znaleziono plików CSV typu 1 dla problemu: {problem_type}")
            return

        for csv_file in csv_files:
            self._create_single_representation_chart(csv_file, problem_type)

    def _create_single_representation_chart(self, csv_file, problem_type):
        """Create a chart for a single graph representation"""
        try:
            # Read CSV data
            df = pd.read_csv(csv_file)

            # Extract representation name from filename
            filename = csv_file.stem
            representation = self._extract_representation_name(filename)

            # Create the chart
            fig, ax = plt.subplots(figsize=(12, 8))

            # Get density values (X-axis)
            densities = df['Density'].values

            # Plot each algorithm
            algorithm_columns = [col for col in df.columns if col != 'Density']

            for algorithm in algorithm_columns:
                # Get execution times for this algorithm
                times = df[algorithm].values

                # Remove NaN values and corresponding densities
                valid_mask = ~pd.isna(times)
                valid_densities = densities[valid_mask]
                valid_times = times[valid_mask]

                if len(valid_times) > 0:
                    ax.plot(valid_densities, valid_times,
                            marker='o',
                            label=self._format_algorithm_name(algorithm),
                            linestyle='-',
                            markersize=8)

            # Customize the chart
            self._customize_chart(ax, representation, problem_type)

            # Save the chart
            output_filename = f"Typ1_{problem_type}_{representation.replace(' ', '_')}_wykres.png"
            output_path = self.output_dir / output_filename

            plt.savefig(output_path)
            plt.close()

            print(f"Wykres typu 1 zapisany: {output_path}")

        except Exception as e:
            print(f"Błąd podczas tworzenia wykresu typu 1 dla {csv_file}: {e}")

    def _extract_representation_name(self, filename):
        """Extract representation name from filename"""
        if "AdjacencyMatrix" in filename:
            return "Macierz_sąsiedztwa"
        elif "AdjacencyList" in filename:
            return "Lista_sąsiedztwa"
        else:
            return "Nieznana_reprezentacja"

    def _format_algorithm_name(self, algorithm):
        """Format algorithm name for display in Polish"""
        algorithm_map = {
            "Kruskal": "Kruskal",
            "Prim": "Prim",
            "Dijkstra": "Dijkstra",
            "BellmanFord": "Bellman-Ford",
            "FordFulkersonDFS": "Ford-Fulkerson (DFS)",
            "FordFulkersonBFS": "Ford-Fulkerson (BFS)"
        }
        return algorithm_map.get(algorithm, algorithm)

    def _customize_chart(self, ax, representation, problem_type):
        """Customize chart appearance with Polish labels"""
        # Set labels and title
        ax.set_xlabel('Gęstość grafu (%)', fontsize=12)
        ax.set_ylabel('Średni czas (mikrosekundy)', fontsize=12)

        title = f'Wydajność algorytmów {self._format_problem_type(problem_type)} - {representation.replace("_", " ")}'
        ax.set_title(title, fontsize=16)

        # Add legend
        ax.legend(title="algorytmy")

        # Grid
        ax.grid(True)

    def _format_problem_type(self, problem_type):
        """Format problem type for display in Polish"""
        problem_map = {
            "MST": "MST",
            "SHORTEST_PATH": "najkrótsza ścieżka",
            "MAX_FLOW": "maksymalny przepływ"
        }
        return problem_map.get(problem_type, problem_type)

    def create_comparison_chart(self, problem_type="MST"):
        """
        Create a comparison chart showing both representations on the same plot
        """
        csv_files = self.find_type1_csv_files(f"Type1_{problem_type}_*.csv")

        if len(csv_files) < 2:
            print(f"Potrzeba przynajmniej 2 plików CSV do wykresu porównawczego typu 1")
            return

        fig, ax = plt.subplots(figsize=(12, 8))

        for csv_file in csv_files:
            try:
                df = pd.read_csv(csv_file)
                representation = self._extract_representation_name(csv_file.stem)

                densities = df['Density'].values
                algorithm_columns = [col for col in df.columns if col != 'Density']

                for algorithm in algorithm_columns:
                    times = df[algorithm].values
                    valid_mask = ~pd.isna(times)
                    valid_densities = densities[valid_mask]
                    valid_times = times[valid_mask]

                    if len(valid_times) > 0:
                        label = f"{self._format_algorithm_name(algorithm)} ({representation.replace('_', ' ')})"
                        ax.plot(valid_densities, valid_times,
                                marker='o',
                                label=label,
                                linestyle='-',
                                markersize=8)

            except Exception as e:
                print(f"Błąd podczas przetwarzania {csv_file} do porównania typu 1: {e}")

        # Customize comparison chart
        ax.set_xlabel('Gęstość grafu (%)', fontsize=12)
        ax.set_ylabel('Średni czas (ms)', fontsize=12)
        ax.set_title(f'Porównanie algorytmów {self._format_problem_type(problem_type)} - wszystkie reprezentacje', fontsize=16)

        ax.legend(title="algorytmy i reprezentacje")
        ax.grid(True)

        # Save comparison chart
        output_filename = f"Typ1_{problem_type}_porownanie_wykres.png"
        output_path = self.output_dir / output_filename

        plt.savefig(output_path)
        plt.close()

        print(f"Wykres porównawczy typu 1 zapisany: {output_path}")

    def create_all_type1_charts(self):
        """Create all Type 1 charts for all problem types"""
        problem_types = ["MST", "SHORTEST_PATH", "MAX_FLOW"]

        print("Generowanie wykresów typu 1 (osobne dla każdej reprezentacji)...")
        for problem_type in problem_types:
            print(f"\nPrzetwarzanie wykresów typu 1 dla {problem_type}...")
            self.create_type1_charts(problem_type)

            print(f"Tworzenie wykresu porównawczego typu 1 dla {problem_type}...")
            self.create_comparison_chart(problem_type)

        print("\nWszystkie wykresy typu 1 zostały wygenerowane pomyślnie!")

def main():
    """Main function to generate Type 1 charts"""
    generator = Type1ChartGenerator()
    generator.create_all_type1_charts()

if __name__ == "__main__":
    main()