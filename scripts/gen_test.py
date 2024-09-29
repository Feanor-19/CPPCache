import sys
import random

def main():
    if len(sys.argv) != 5:
        print("Usage: python script.py <cache_size> <queries_count> <queries_range> <filename>")
        sys.exit(1)

    cache_size = int(sys.argv[1])
    queries_count = int(sys.argv[2])
    queries_range = int(sys.argv[3])
    filename = sys.argv[4]

    print(f"Cache Size: {cache_size}")
    print(f"Queries Count: {queries_count}")
    print(f"Queries Range: {queries_range}")
    print(f"Output filename: {filename}")

    if queries_range < cache_size:
        print("WARNING: queries_range < cache_size! The cache will never be fully filled!")

    if queries_count < cache_size:
        print("WARNING: queries_count < cache_size! The cache will never be fully filled!")

    # Generate a random sequence of integers
    generated_queries = [random.randint(1, queries_range) for _ in range(queries_count)]

    # Write the generated queries to a file
    with open(filename, 'w') as file:
        file.write(f"{cache_size} {queries_count}\n")
        file.write(' '.join(map(str, generated_queries)))
        file.write('\n')

if __name__ == "__main__":
    main()
