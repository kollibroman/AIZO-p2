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

        # Algorithm categories
        self.problem_types = {
            'MST': ['Kruskal', 'Prim'],
            'SHORTEST_PATH': ['Dijkstra', 'Bellman-Ford'],
            'MAX_FLOW': ['Ford-Fulkerson-DFS', 'Ford-Fulkerson-BFS']
        }

    def find_type1_csv_files(self, pattern="*_type1_*.csv"):
        """Find all Type 1 CSV files matching the pattern"""
        return list(self.results_path.glob(pattern))

    def create_type1_charts(self, problem_type="MST"):
        """
        Create Type 1 charts (separate line charts for each graph representation)
        Line charts with vertex count on X-axis and algorithm-density combinations as lines

        Args:
            problem_type: Type of problem (MST, SHORTEST_PATH, MAX_FLOW)
        """
        # Find CSV files for the specific problem type
        csv_files = self.find_type1_csv_files(f"*{problem_type}_type1_*.csv")

        if not csv_files:
            print(f"Nie znaleziono plików CSV typu 1 dla problemu: {problem_type}")
            return

        for csv_file in csv_files:
            self._create_single_representation_chart(csv_file, problem_type)

    def _create_single_representation_chart(self, csv_file, problem_type):
        """Create a line chart for a single graph representation"""
        try:
            # Read CSV data
            df = pd.read_csv(csv_file)

            # Extract representation from filename
            representation = self._extract_representation_from_filename(csv_file.stem)

            # Create the chart
            fig, ax = plt.subplots(figsize=(12, 8))

            # Get vertex count values (X-axis)
            vertex_counts = df['VertexCount'].values

            # Plot each algorithm-density combination
            algorithm_density_columns = [col for col in df.columns if col != 'VertexCount']

            for algo_density_col in algorithm_density_columns:
                # Get execution times for this algorithm-density combination
                times = df[algo_density_col].values

                # Convert from microseconds to milliseconds for better readability
                times_ms = times / 1000.0

                # Remove NaN values and corresponding vertex counts
                valid_mask = ~pd.isna(times_ms) & (times_ms != '') & (times_ms > 0)
                valid_vertex_counts = vertex_counts[valid_mask]
                valid_times_ms = times_ms[valid_mask]

                if len(valid_times_ms) > 0:
                    # Format the legend label
                    legend_label = self._format_algorithm_density_name(algo_density_col)
                    
                    ax.plot(valid_vertex_counts, valid_times_ms,
                            marker='o',
                            label=legend_label,
                            linestyle='-',
                            markersize=6,
                            linewidth=2)

            # Customize the chart
            self._customize_type1_chart(ax, representation, problem_type)

            # Save the chart
            output_filename = f"Typ1_{problem_type}_{representation}_wykres.png"
            output_path = self.output_dir / output_filename

            plt.savefig(output_path, dpi=300, bbox_inches='tight')
            plt.close()

            print(f"Wykres typu 1 zapisany: {output_path}")

        except Exception as e:
            print(f"Błąd podczas tworzenia wykresu typu 1 dla {csv_file}: {e}")

    def _extract_representation_from_filename(self, filename):
        """Extract representation value from filename"""
        if "AdjacencyMatrix" in filename:
            return "AdjacencyMatrix"
        elif "AdjacencyList" in filename:
            return "AdjacencyList"
        return "Unknown"

    def _format_algorithm_density_name(self, algo_density_col):
        """Format algorithm-density combination name for display"""
        # Extract algorithm name and density from column name like "Kruskal_density20"
        if "_density" in algo_density_col:
            parts = algo_density_col.split("_density")
            if len(parts) == 2:
                algorithm = parts[0]
                density = parts[1]
                
                # Format algorithm name
                algorithm_map = {
                    "Kruskal": "Kruskal",
                    "Prim": "Prim",
                    "Dijkstra": "Dijkstra",
                    "Bellman-Ford": "Bellman-Ford",
                    "Ford-Fulkerson-DFS": "Ford-Fulkerson (DFS)",
                    "Ford-Fulkerson-BFS": "Ford-Fulkerson (BFS)"
                }
                
                formatted_algorithm = algorithm_map.get(algorithm, algorithm)
                return f"{formatted_algorithm} (gęstość {density}%)"
        
        return algo_density_col

    def _get_algorithms_for_problem_type(self, problem_type):
        """Get relevant algorithms for the problem type"""
        return self.problem_types.get(problem_type.upper(), [])

    def _format_problem_type(self, problem_type):
        """Format problem type for display in Polish"""
        problem_map = {
            "MST": "MST",
            "SHORTEST_PATH": "najkrótsza ścieżka",
            "MAX_FLOW": "maksymalny przepływ"
        }
        return problem_map.get(problem_type, problem_type)

    def _format_representation_name(self, representation):
        """Format representation name for display in Polish"""
        representation_map = {
            "AdjacencyMatrix": "macierz sąsiedztwa",
            "AdjacencyList": "lista sąsiedztwa"
        }
        return representation_map.get(representation, representation)

    def _customize_type1_chart(self, ax, representation, problem_type):
        """Customize Type 1 line chart appearance with Polish labels"""
        # Set labels and title
        ax.set_xlabel('Liczba wierzchołków', fontsize=12)
        ax.set_ylabel('Średni czas (ms)', fontsize=12)

        formatted_representation = self._format_representation_name(representation)
        title = f'Wydajność algorytmów {self._format_problem_type(problem_type)} - {formatted_representation}'
        ax.set_title(title, fontsize=16)

        # Add legend
        ax.legend(title="Algorytmy i gęstość", bbox_to_anchor=(1.05, 1), loc='upper left')

        # Grid
        ax.grid(True, alpha=0.3)
        
        # Always use linear scale
        ax.set_yscale('linear')

    def create_comparison_chart(self, problem_type="MST"):
        """
        Create a comparison chart showing all representations on the same plot
        """
        csv_files = self.find_type1_csv_files(f"*{problem_type}_type1_*.csv")

        if len(csv_files) < 2:
            print(f"Potrzeba przynajmniej 2 plików CSV do wykresu porównawczego typu 1")
            return

        fig, ax = plt.subplots(figsize=(14, 10))

        colors = ['blue', 'red', 'green', 'orange', 'purple', 'brown', 'pink', 'gray']
        line_styles = ['-', '--', '-.', ':']
        color_idx = 0

        for csv_file in csv_files:
            try:
                df = pd.read_csv(csv_file)
                representation = self._extract_representation_from_filename(csv_file.stem)

                vertex_counts = df['VertexCount'].values
                algorithm_density_columns = [col for col in df.columns if col != 'VertexCount']

                for i, algo_density_col in enumerate(algorithm_density_columns):
                    times = df[algo_density_col].values
                    times_ms = times / 1000.0  # Convert to milliseconds
                    
                    valid_mask = ~pd.isna(times_ms) & (times_ms != '') & (times_ms > 0)
                    valid_vertex_counts = vertex_counts[valid_mask]
                    valid_times_ms = times_ms[valid_mask]

                    if len(valid_times_ms) > 0:
                        algo_density_name = self._format_algorithm_density_name(algo_density_col)
                        repr_name = self._format_representation_name(representation)
                        legend_label = f"{algo_density_name} ({repr_name})"
                        
                        ax.plot(valid_vertex_counts, valid_times_ms,
                                marker='o',
                                label=legend_label,
                                linestyle=line_styles[i % len(line_styles)],
                                color=colors[color_idx % len(colors)],
                                markersize=6,
                                linewidth=2)
                        
                        color_idx += 1

            except Exception as e:
                print(f"Błąd podczas przetwarzania {csv_file} do porównania typu 1: {e}")

        # Customize comparison chart
        ax.set_xlabel('Liczba wierzchołków', fontsize=12)
        ax.set_ylabel('Średni czas (ms)', fontsize=12)
        ax.set_title(f'Porównanie algorytmów {self._format_problem_type(problem_type)} - wszystkie reprezentacje', fontsize=16)

        ax.legend(title="Algorytmy, gęstość i reprezentacja", bbox_to_anchor=(1.05, 1), loc='upper left')
        ax.grid(True, alpha=0.3)
        
        # Always use linear scale
        ax.set_yscale('linear')

        # Save comparison chart
        output_filename = f"Typ1_{problem_type}_porownanie_wykres.png"
        output_path = self.output_dir / output_filename

        plt.savefig(output_path, dpi=300, bbox_inches='tight')
        plt.close()

        print(f"Wykres porównawczy typu 1 zapisany: {output_path}")

    def create_all_type1_charts(self):
        """Create all Type 1 charts for all problem types"""
        problem_types = ["MST", "SHORTEST_PATH", "MAX_FLOW"]

        print("Generowanie wykresów liniowych typu 1 (osobne dla każdej reprezentacji)...")
        for problem_type in problem_types:
            print(f"\nPrzetwarzanie wykresów typu 1 dla {problem_type}...")
            self.create_type1_charts(problem_type)

            print(f"Tworzenie wykresu porównawczego typu 1 dla {problem_type}...")
            self.create_comparison_chart(problem_type)

        print("\nWszystkie wykresy liniowe typu 1 zostały wygenerowane pomyślnie!")

def main():
    """Main function to generate Type 1 charts"""
    generator = Type1ChartGenerator()
    generator.create_all_type1_charts()

if __name__ == "__main__":
    main()