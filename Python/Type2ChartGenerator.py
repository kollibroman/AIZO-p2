import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os
import glob
from pathlib import Path

class Type2ChartGenerator:
    def __init__(self, results_path="../Results", output_dir="charts_type2"):
        """
        Initialize the Type 2 chart generator

        Args:
            results_path: Directory containing CSV files
            output_dir: Directory to save generated charts
        """
        self.results_path = Path(results_path)
        self.output_dir = Path(output_dir)
        self.output_dir.mkdir(exist_ok=True)

        # Representation types
        self.representations = ['AdjacencyMatrix', 'AdjacencyList']

    def find_type2_csv_files(self, pattern="Type2_*.csv"):
        """Find all Type 2 CSV files matching the pattern"""
        return list(self.results_path.glob(pattern))

    def create_type2_charts(self, problem_type="MST"):
        """
        Create Type 2 charts (separate column charts for each graph density)

        Args:
            problem_type: Type of problem (MST, SHORTEST_PATH, MAX_FLOW)
        """
        # Find CSV files for the specific problem type
        csv_files = self.find_type2_csv_files(f"Type2_{problem_type}_*.csv")

        if not csv_files:
            print(f"Nie znaleziono plików CSV typu 2 dla problemu: {problem_type}")
            return

        for csv_file in csv_files:
            self._create_single_density_chart(csv_file, problem_type)

    def _create_single_density_chart(self, csv_file, problem_type):
        """Create a column chart for a single graph density"""
        try:
            # Read CSV data
            df = pd.read_csv(csv_file)

            # Extract density from filename
            density = self._extract_density_from_filename(csv_file.stem)

            # Create the chart
            fig, ax = plt.subplots(figsize=(12, 8))

            # Parse algorithm-representation combinations
            algorithm_rep_combinations = []
            execution_times = []

            for _, row in df.iterrows():
                combination = row['AlgorithmRepresentation']
                time = row['ExecutionTime']

                if pd.notna(time) and time != '':
                    algorithm_rep_combinations.append(combination)
                    execution_times.append(float(time))

            if not algorithm_rep_combinations:
                print(f"Nie znaleziono prawidłowych danych w {csv_file}")
                return

            # Debug: print combinations found
            print(f"Znaleziono kombinacje w {csv_file}: {algorithm_rep_combinations}")

            # Group data by algorithm and representation
            grouped_data = self._group_algorithm_representation_data(
                algorithm_rep_combinations, execution_times
            )

            # Create column chart
            self._create_column_chart_for_density(ax, grouped_data, problem_type)

            # Customize the chart
            self._customize_type2_chart(ax, density, problem_type)

            # Save the chart
            output_filename = f"Typ2_{problem_type}_gestosc{density}_wykres.png"
            output_path = self.output_dir / output_filename

            plt.savefig(output_path)
            plt.close()

            print(f"Wykres typu 2 zapisany: {output_path}")

        except Exception as e:
            print(f"Błąd podczas tworzenia wykresu typu 2 dla {csv_file}: {e}")

    def _create_column_chart_for_density(self, ax, grouped_data, problem_type):
        """Create column chart for algorithm comparison across representations"""
        algorithms = self._get_algorithms_for_problem_type(problem_type)
        
        # Debug: print algorithms and grouped data
        print(f"Algorytmy dla {problem_type}: {algorithms}")
        print(f"Zgrupowane dane: {grouped_data}")
        
        # Prepare data for column chart
        rep_names = [self._format_representation_name(rep) for rep in self.representations]
        x_pos = np.arange(len(rep_names))
        
        # Width of bars
        bar_width = 0.35
        
        # Create bars for each algorithm
        for i, algorithm in enumerate(algorithms):
            values = []
            
            for representation in self.representations:
                # For MAX_FLOW, use the actual format from CSV data
                if problem_type.upper() == "MAX_FLOW":
                    key = f"{algorithm}_{representation}"
                else:
                    # For other problem types, use standard format
                    key = f"{algorithm}_{representation}"
                
                found_value = 0
                if key in grouped_data:
                    found_value = grouped_data[key]
                
                values.append(found_value)
            
            # Only plot if we have some data for this algorithm
            if any(v > 0 for v in values):
                label = self._format_algorithm_name(algorithm)
                ax.bar(x_pos + i * bar_width, values, bar_width, label=label)

        # Set x-axis labels and positions
        ax.set_xticks(x_pos + bar_width / 2)
        ax.set_xticklabels(rep_names)

    def _group_algorithm_representation_data(self, combinations, times):
        """Group data by algorithm-representation combinations"""
        grouped = {}
        for combination, time in zip(combinations, times):
            grouped[combination] = time
        return grouped

    def _extract_density_from_filename(self, filename):
        """Extract density value from filename"""
        import re
        match = re.search(r'density(\d+)', filename)
        if match:
            return match.group(1)
        return "Nieznana"

    def _get_algorithms_for_problem_type(self, problem_type):
        """Get relevant algorithms for the problem type"""
        if problem_type.upper() == "MST":
            return ["Kruskal", "Prim"]
        elif problem_type.upper() in ["SSP", "SHORTEST_PATH"]:
            return ["Dijkstra", "Bellman-Ford"]
        elif problem_type.upper() == "MAX_FLOW":
            # Use the actual algorithm names from CSV data
            return ["Ford-Fulkerson-DFS", "Ford-Fulkerson-BFS"]
        return []

    def _format_algorithm_name(self, algorithm):
        """Format algorithm name for display in Polish"""
        algorithm_map = {
            "Kruskal": "Kruskal",
            "Prim": "Prim",
            "Dijkstra": "Dijkstra",
            "Bellman-Ford": "Bellman-Ford",
            "FordFulkersonDFS": "Ford-Fulkerson (DFS)",
            "FordFulkersonBFS": "Ford-Fulkerson (BFS)",
            "FordFulkersonList": "Ford-Fulkerson (Lista)",
            "FordFulkersonMatrix": "Ford-Fulkerson (Macierz)",
            "Ford-Fulkerson-DFS": "Ford-Fulkerson (DFS)",
            "Ford-Fulkerson-BFS": "Ford-Fulkerson (BFS)"
        }
        return algorithm_map.get(algorithm, algorithm)

    def _format_representation_name(self, representation):
        """Format representation name for display in Polish"""
        if representation == "AdjacencyMatrix":
            return "Macierz sąsiedztwa"
        elif representation == "AdjacencyList":
            return "Lista sąsiedztwa"
        return representation

    def _format_problem_type(self, problem_type):
        """Format problem type for display in Polish"""
        problem_map = {
            "MST": "MST",
            "SHORTEST_PATH": "najkrótsza ścieżka",
            "MAX_FLOW": "maksymalny przepływ"
        }
        return problem_map.get(problem_type, problem_type)

    def _customize_type2_chart(self, ax, density, problem_type):
        """Customize Type 2 column chart appearance with Polish labels"""
        # Set labels and title
        ax.set_xlabel('Reprezentacja grafu', fontsize=12)
        ax.set_ylabel('Średni czas (ms)', fontsize=12)

        title = f'Wydajność algorytmów {self._format_problem_type(problem_type)} - gęstość {density}%'
        ax.set_title(title, fontsize=16)

        # Add legend
        ax.legend(title="algorytmy")

        # Grid
        ax.grid(True)

    def create_all_type2_charts(self):
        """Create all Type 2 charts for all problem types"""
        problem_types = ["MST", "SHORTEST_PATH", "MAX_FLOW"]

        print("Generowanie wykresów kolumnowych typu 2 (osobne dla każdej gęstości)...")
        for problem_type in problem_types:
            print(f"\nPrzetwarzanie wykresów typu 2 dla {problem_type}...")
            self.create_type2_charts(problem_type)

        print("\nWszystkie wykresy kolumnowe typu 2 zostały wygenerowane pomyślnie!")

def main():
    """Main function to generate Type 2 charts"""
    generator = Type2ChartGenerator()
    generator.create_all_type2_charts()

if __name__ == "__main__":
    main()