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

    def find_type2_csv_files(self, pattern="*_type2_*.csv"):
        """Find all Type 2 CSV files matching the pattern"""
        return list(self.results_path.glob(pattern))

    def create_type2_charts(self, problem_type="MST"):
        """
        Create Type 2 charts (separate line charts for each graph density)
        Line charts with vertex count on X-axis and algorithm-representation combinations as lines

        Args:
            problem_type: Type of problem (MST, SHORTEST_PATH, MAX_FLOW)
        """
        # Find CSV files for the specific problem type
        csv_files = self.find_type2_csv_files(f"*{problem_type}_type2_*.csv")

        if not csv_files:
            print(f"Nie znaleziono plików CSV typu 2 dla problemu: {problem_type}")
            return

        for csv_file in csv_files:
            self._create_single_density_chart(csv_file, problem_type)

    def _create_single_density_chart(self, csv_file, problem_type):
        """Create a line chart for a single graph density"""
        try:
            # Read CSV data
            df = pd.read_csv(csv_file)

            # Extract density from filename
            density = self._extract_density_from_filename(csv_file.stem)

            # Create the chart
            fig, ax = plt.subplots(figsize=(12, 8))

            # Get vertex count values (X-axis)
            vertex_counts = df['VertexCount'].values

            # Plot each algorithm-representation combination
            algorithm_rep_columns = [col for col in df.columns if col != 'VertexCount']

            for algo_rep_col in algorithm_rep_columns:
                # Get execution times for this algorithm-representation combination
                times = df[algo_rep_col].values

                # Convert from microseconds to milliseconds for better readability
                times_ms = times / 1000.0

                # Remove NaN values and corresponding vertex counts
                valid_mask = ~pd.isna(times_ms) & (times_ms != '') & (times_ms > 0)
                valid_vertex_counts = vertex_counts[valid_mask]
                valid_times_ms = times_ms[valid_mask]

                if len(valid_times_ms) > 0:
                    # Format the legend label
                    legend_label = self._format_algorithm_representation_name(algo_rep_col)
                    
                    ax.plot(valid_vertex_counts, valid_times_ms,
                            marker='o',
                            label=legend_label,
                            linestyle='-',
                            markersize=6,
                            linewidth=2)

            # Customize the chart
            self._customize_type2_chart(ax, density, problem_type)

            # Save the chart
            output_filename = f"Typ2_{problem_type}_gestosc{density}_wykres.png"
            output_path = self.output_dir / output_filename

            plt.savefig(output_path, dpi=300, bbox_inches='tight')
            plt.close()

            print(f"Wykres typu 2 zapisany: {output_path}")

        except Exception as e:
            print(f"Błąd podczas tworzenia wykresu typu 2 dla {csv_file}: {e}")

    def _extract_density_from_filename(self, filename):
        """Extract density value from filename"""
        import re
        match = re.search(r'density(\d+)', filename)
        if match:
            return match.group(1)
        return "Nieznana"

    def _format_algorithm_representation_name(self, algo_rep_col):
        """Format algorithm-representation combination name for display"""
        # Extract algorithm name and representation from column name like "Kruskal_AdjacencyMatrix"
        if "_" in algo_rep_col:
            parts = algo_rep_col.split("_")
            if len(parts) >= 2:
                algorithm = parts[0]
                representation = parts[1]
                
                # Handle special cases with multiple underscores
                if len(parts) > 2:
                    # For cases like "Ford-Fulkerson-DFS_AdjacencyMatrix"
                    algorithm = "_".join(parts[:-1])
                    representation = parts[-1]
                
                # Format algorithm name
                algorithm_map = {
                    "Kruskal": "Kruskal",
                    "Prim": "Prim",
                    "Dijkstra": "Dijkstra",
                    "Bellman-Ford": "Bellman-Ford",
                    "Ford-Fulkerson-DFS": "Ford-Fulkerson (DFS)",
                    "Ford-Fulkerson-BFS": "Ford-Fulkerson (BFS)"
                }
                
                # Format representation name
                representation_map = {
                    "AdjacencyMatrix": "Macierz",
                    "AdjacencyList": "Lista"
                }
                
                formatted_algorithm = algorithm_map.get(algorithm, algorithm)
                formatted_representation = representation_map.get(representation, representation)
                
                return f"{formatted_algorithm} ({formatted_representation})"
        
        return algo_rep_col

    def _get_algorithms_for_problem_type(self, problem_type):
        """Get relevant algorithms for the problem type"""
        if problem_type.upper() == "MST":
            return ["Kruskal", "Prim"]
        elif problem_type.upper() in ["SSP", "SHORTEST_PATH"]:
            return ["Dijkstra", "Bellman-Ford"]
        elif problem_type.upper() == "MAX_FLOW":
            return ["Ford-Fulkerson-DFS", "Ford-Fulkerson-BFS"]
        return []

    def _format_problem_type(self, problem_type):
        """Format problem type for display in Polish"""
        problem_map = {
            "MST": "MST",
            "SHORTEST_PATH": "najkrótsza ścieżka",
            "MAX_FLOW": "maksymalny przepływ"
        }
        return problem_map.get(problem_type, problem_type)

    def _customize_type2_chart(self, ax, density, problem_type):
        """Customize Type 2 line chart appearance with Polish labels"""
        # Set labels and title
        ax.set_xlabel('Liczba wierzchołków', fontsize=12)
        ax.set_ylabel('Średni czas (ms)', fontsize=12)

        title = f'Wydajność algorytmów {self._format_problem_type(problem_type)} - gęstość {density}%'
        ax.set_title(title, fontsize=16)

        # Add legend
        ax.legend(title="Algorytmy i reprezentacje", bbox_to_anchor=(1.05, 1), loc='upper left')

        # Grid
        ax.grid(True, alpha=0.3)
        
        # Always use linear scale
        ax.set_yscale('linear')

    def create_comparison_chart(self, problem_type="MST"):
        """
        Create a comparison chart showing all densities on the same plot
        """
        csv_files = self.find_type2_csv_files(f"*{problem_type}_type2_*.csv")

        if len(csv_files) < 2:
            print(f"Potrzeba przynajmniej 2 plików CSV do wykresu porównawczego typu 2")
            return

        fig, ax = plt.subplots(figsize=(14, 10))

        colors = ['blue', 'red', 'green', 'orange', 'purple', 'brown', 'pink', 'gray']
        line_styles = ['-', '--', '-.', ':']
        color_idx = 0

        for csv_file in csv_files:
            try:
                df = pd.read_csv(csv_file)
                density = self._extract_density_from_filename(csv_file.stem)

                vertex_counts = df['VertexCount'].values
                algorithm_rep_columns = [col for col in df.columns if col != 'VertexCount']

                for i, algo_rep_col in enumerate(algorithm_rep_columns):
                    times = df[algo_rep_col].values
                    times_ms = times / 1000.0  # Convert to milliseconds
                    
                    valid_mask = ~pd.isna(times_ms) & (times_ms != '') & (times_ms > 0)
                    valid_vertex_counts = vertex_counts[valid_mask]
                    valid_times_ms = times_ms[valid_mask]

                    if len(valid_times_ms) > 0:
                        legend_label = f"{self._format_algorithm_representation_name(algo_rep_col)} (gęstość {density}%)"
                        
                        ax.plot(valid_vertex_counts, valid_times_ms,
                                marker='o',
                                label=legend_label,
                                linestyle=line_styles[i % len(line_styles)],
                                color=colors[color_idx % len(colors)],
                                markersize=6,
                                linewidth=2)
                        
                        color_idx += 1

            except Exception as e:
                print(f"Błąd podczas przetwarzania {csv_file} do porównania typu 2: {e}")

        # Customize comparison chart
        ax.set_xlabel('Liczba wierzchołków', fontsize=12)
        ax.set_ylabel('Średni czas (ms)', fontsize=12)
        ax.set_title(f'Porównanie algorytmów {self._format_problem_type(problem_type)} - wszystkie gęstości', fontsize=16)

        ax.legend(title="Algorytmy, reprezentacje i gęstość", bbox_to_anchor=(1.05, 1), loc='upper left')
        ax.grid(True, alpha=0.3)
        
        # Always use linear scale
        ax.set_yscale('linear')

        # Save comparison chart
        output_filename = f"Typ2_{problem_type}_porownanie_wykres.png"
        output_path = self.output_dir / output_filename

        plt.savefig(output_path, dpi=300, bbox_inches='tight')
        plt.close()

        print(f"Wykres porównawczy typu 2 zapisany: {output_path}")

    def create_all_type2_charts(self):
        """Create all Type 2 charts for all problem types"""
        problem_types = ["MST", "SHORTEST_PATH", "MAX_FLOW"]

        print("Generowanie wykresów liniowych typu 2 (osobne dla każdej gęstości)...")
        for problem_type in problem_types:
            print(f"\nPrzetwarzanie wykresów typu 2 dla {problem_type}...")
            self.create_type2_charts(problem_type)

            print(f"Tworzenie wykresu porównawczego typu 2 dla {problem_type}...")
            self.create_comparison_chart(problem_type)

        print("\nWszystkie wykresy liniowe typu 2 zostały wygenerowane pomyślnie!")

def main():
    """Main function to generate Type 2 charts"""
    generator = Type2ChartGenerator()
    generator.create_all_type2_charts()

if __name__ == "__main__":
    main()