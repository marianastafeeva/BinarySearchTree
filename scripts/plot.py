import matplotlib.pyplot as plt
import csv
import os

csv_path = os.path.join(os.path.dirname(__file__), "..", "results", "tables", "bench.csv")
plot_path = os.path.join(os.path.dirname(__file__), "..", "results", "plots", "benchmark.png")

Ns = []
inserts = []
searches = []
removes = []

with open(csv_path, "r") as f:
    reader = csv.DictReader(f)
    for row in reader:
        Ns.append(int(row["N"]))
        inserts.append(float(row["Insert_ms"]))
        searches.append(float(row["Search_ms"]))
        removes.append(float(row["Remove_ms"]))

plt.figure(figsize=(10, 6))
plt.plot(Ns, inserts, marker="o", label="Insert")
plt.plot(Ns, searches, marker="s", label="Search")
plt.plot(Ns, removes, marker="^", label="Remove")

plt.xscale("log")
plt.yscale("log")
plt.xlabel("Number of elements (N)")
plt.ylabel("Time (ms)")
plt.title("BST Performance: Time vs N")
plt.grid(True, which="both", ls="--")
plt.legend()
plt.tight_layout()

os.makedirs(os.path.dirname(plot_path), exist_ok=True)
plt.savefig(plot_path, dpi=120)
print(f"Plot saved to {plot_path}")