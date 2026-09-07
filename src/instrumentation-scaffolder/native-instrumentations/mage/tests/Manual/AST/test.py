#!/usr/bin/env python3
"""
A small, self-contained Python script demonstrating a main() function.
"""

import sys


def main() -> int:
    """Main entry point for the script."""
    print("Hello from the main function!")
    
    if len(sys.argv) > 1:
        print(f"You passed {len(sys.argv) - 1} argument(s):")
        for i, arg in enumerate(sys.argv[1:], start=1):
            print(f"  {i}: {arg}")
    else:
        print("No command-line arguments provided.")
    
    print("Goodbye!")
    return 0


if __name__ == "__main__":
    sys.exit(main())