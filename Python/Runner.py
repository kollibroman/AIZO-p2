#!/usr/bin/env python3
"""
Main runner script for generating all performance charts
"""
import os
import sys
from pathlib import Path

# Add current directory to Python path
sys.path.append(str(Path(__file__).parent))

from Type1ChartGenerator import Type1ChartGenerator
from Type2ChartGenerator import Type2ChartGenerator

def main():
    """Main function to generate all charts"""
    print("=== Graph Algorithms Performance Chart Generator ===")
    print()
    
    # Check if Results directory exists
    results_path = Path("../Results")
    if not results_path.exists():
        print("Error: Results directory not found!")
        print("Please run the C++ performance tests first.")
        print("Expected path:", results_path.absolute())
        return 1
    
    # Check for CSV files
    csv_files = list(results_path.glob("*.csv"))
    if not csv_files:
        print("Error: No CSV files found in Results directory!")
        print("Please run the C++ performance tests first.")
        return 1
    
    print(f"Found {len(csv_files)} CSV files in Results directory")
    print()
    
    # Generate Type 1 charts
    print("=== Generating Type 1 Charts ===")
    print("(Separate charts for each graph representation)")
    try:
        type1_generator = Type1ChartGenerator()
        type1_generator.create_all_type1_charts()
        print("✓ Type 1 charts generated successfully!")
    except Exception as e:
        print(f"✗ Error generating Type 1 charts: {e}")
        return 1
    
    print()
    
    # Generate Type 2 charts
    print("=== Generating Type 2 Charts ===")
    print("(Separate charts for each graph density)")
    try:
        type2_generator = Type2ChartGenerator()
        type2_generator.create_all_type2_charts()
        print("✓ Type 2 charts generated successfully!")
    except Exception as e:
        print(f"✗ Error generating Type 2 charts: {e}")
        return 1
    
    print()
    print("=== All Charts Generated Successfully! ===")
    print("Check the following directories for generated charts:")
    print(f"- Type 1 charts: {Path('charts_type1').absolute()}")
    print(f"- Type 2 charts: {Path('charts_type2').absolute()}")
    
    return 0

if __name__ == "__main__":
    exit_code = main()
    sys.exit(exit_code)