# Database Join Cardinality Estimator

## Quick Start

### Prerequisites

- C++17 compatible compiler
- Python 3.x with matplotlib for visualization
- Make build system

### Building and Running

```bash
# Build the project
make clean
make

# Run tests
./join_estimator > results/text/text.txt
python plotter.py
```

## Project Structure

### Files and Directories

```
.
├── cardinality.cpp    # CRC32 hash generation and test data creation
├── estimation.cpp     # Core estimation algorithm implementation
├── main.cpp          # Program entry point and test orchestration
├── main.hpp          # Constants and function declarations
├── Makefile          # Build configuration
├── plotter.py        # Results visualization
└── results/          # Test results directory
    ├── plot/         # Generated plots
    └── text/         # Raw test results
```

## Data Formats

### Input Data

The program works with two types of input:

1. Raw strings that get converted to CRC32 hashes
2. Pre-computed CRC32 hash values

Example:

```
"hello_world"  -> 0x4A17B156 (CRC32 hash)
"test_string"  -> 0x0E2C157F (CRC32 hash)
```

### Dataset Naming Convention

Files follow the pattern: `[buckets]-[total_size]-[set_size]-[dist1]-[dist2]`

Example: `256-50k-30k-binom-normal` means:

- `256`: Number of histogram buckets
- `50k`: Total size of unique strings pool (50,000)
- `30k`: Size of each test set (30,000)
- `binom`: Distribution for first set (Binomial)
- `normal`: Distribution for second set (Normal)

Available distributions:

- `binom`: Binomial distribution
- `normal`: Normal distribution
- `poisson`: Poisson distribution

## Output Format

### Console Output

Each test iteration outputs:

```
Actual intersection cardinality: 141364
Estimated intersection cardinality: 116597
Error: 17.52 %
Performance: 3.4088e+08 bytes/second
```

### Results Files

- `results/text/`: Contains raw test results
- `results/plot/`: Contains generated error distribution plots

### Plot Interpretation

- X-axis: Error percentage intervals
- Y-axis: Number of estimates in each interval
- Lower and narrower distribution indicates better accuracy

## Configuration

Key parameters in `main.hpp`:

- `BUCKETS`: Number of histogram buckets (default: 256)
- `STR_LEN`: Length of test strings (default: 20)
- `SET_SIZE`: Size of test sets (30k or 80k)
- `NUM_ESTIMS`: Number of estimation iterations (default: 100)
