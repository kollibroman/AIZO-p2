#!/usr/bin/env python3
"""
Main Chart Generator Runner
This script automatically generates all charts from the Results folder
using both Type1 and Type2 chart generators.
"""

import sys
from pathlib import Path

# Add the current directory to Python path
current_dir = Path(__file__).parent
sys.path.append(str(current_dir))

from Type1ChartGenerator import Type1ChartGenerator
from Type2ChartGenerator import Type2ChartGenerator

def main():
    """
    Main function to run both chart generators
    """
    print("=" * 60)
    print("AUTOMATED CHART GENERATION")
    print("=" * 60)
    print("This script will automatically generate charts from CSV files in the Results folder.")
    print()

    # Initialize both generators
    print("Initializing chart generators...")
    type1_generator = Type1ChartGenerator(results_path="../Results", output_dir="charts_type1")
    type2_generator = Type2ChartGenerator(results_path="../Results", output_dir="charts_type2")

    print("✓ Type 1 Chart Generator initialized")
    print("✓ Type 2 Chart Generator initialized")
    print()

    # Check if Results folder exists and has CSV files
    results_path = Path("../Results")
    if not results_path.exists():
        print("❌ Error: Results folder not found!")
        print("   Make sure the Results folder exists in the parent directory.")
        return

    csv_files = list(results_path.glob("*.csv"))
    if not csv_files:
        print("❌ Error: No CSV files found in Results folder!")
        print("   Make sure your C++ program has generated CSV files.")
        return

    print(f"✓ Found {len(csv_files)} CSV files in Results folder")
    print()

    # Generate Type 1 charts
    print("🔄 GENERATING TYPE 1 CHARTS")
    print("-" * 40)
    try:
        type1_generator.create_all_type1_charts()
        print("✅ Type 1 charts generation completed successfully!")
    except Exception as e:
        print(f"❌ Error generating Type 1 charts: {e}")
    print()

    # Generate Type 2 charts
    print("🔄 GENERATING TYPE 2 CHARTS")
    print("-" * 40)
    try:
        type2_generator.create_all_type2_charts()
        print("✅ Type 2 charts generation completed successfully!")
    except Exception as e:
        print(f"❌ Error generating Type 2 charts: {e}")
    print()

    # Summary
    print("=" * 60)
    print("GENERATION SUMMARY")
    print("=" * 60)

    # Count generated charts
    type1_charts = list(Path("charts_type1").glob("*.png")) if Path("charts_type1").exists() else []
    type2_charts = list(Path("charts_type2").glob("*.png")) if Path("charts_type2").exists() else []

    print(f"📊 Type 1 charts generated: {len(type1_charts)}")
    print(f"📊 Type 2 charts generated: {len(type2_charts)}")
    print(f"📊 Total charts generated: {len(type1_charts) + len(type2_charts)}")
    print()

    if type1_charts:
        print("Type 1 charts saved in: charts_type1/")
        for chart in sorted(type1_charts):
            print(f"  - {chart.name}")

    if type2_charts:
        print("Type 2 charts saved in: charts_type2/")
        for chart in sorted(type2_charts):
            print(f"  - {chart.name}")

    print()
    print("🎉 Chart generation process completed!")

if __name__ == "__main__":
    main()