import matplotlib.pyplot as plt
import re
import subprocess

with open('results/text.txt', 'r') as result:
    lines = result.readlines()

# Extract error values using regex
error_values = [float(match) for line in lines for match in re.findall(r"Error: (\d+\.\d+) %", line)]

# Divide errors into intervals by 1 percent
intervals = range(0, 101)  # 0% to 100%
interval_counts = {i: 0 for i in intervals}
for error in error_values:
    interval = min(int(error), 100)
    interval_counts[interval] += 1

# Prepare data for plotting
interval_labels = list(interval_counts.keys())
counts = list(interval_counts.values())

# Plot the interval distribution
plt.figure(figsize=(12, 6))
plt.bar(interval_labels, counts, width=0.9, color='b', edgecolor='black')
plt.title("Distribution of Errors into Intervals", fontsize=14)
plt.xlabel("Error Intervals (%)", fontsize=12)
plt.ylabel("Number of Errors", fontsize=12)
plt.xticks(range(0, 101, 5))  # Show every 5% for clarity
plt.grid(axis='y', linestyle='--', alpha=0.7)
plt.savefig('plot.png')
