import os
import shutil
from data_generator import generate

if os.path.exists('data'):
    shutil.rmtree('data')
os.mkdir('data')
os.chdir('data')

count = 0

for i in range(100):
    count += 1
    with open(f"data_no_cycle_{count}.txt", "w") as f:
        f.write(generate(length=1, no_same=False, self_cycle_max=1, no_cycle=True, confuse=False))

for i in range(100):
    count += 1
    with open(f"data_no_cycle_{count}.txt", "w") as f:
        f.write(generate(length=i // 5 + 1, no_same=False, self_cycle_max=1, no_cycle=True, confuse=False))

for i in range(100):
    count += 1
    with open(f"data_no_cycle_{count}.txt", "w") as f:
        f.write(generate(length=100, no_same=False, self_cycle_max=1, no_cycle=True, confuse=False))

for i in range(100):
    count += 1
    with open(f"data_no_cycle_{count}.txt", "w") as f:
        f.write(generate(length=1000, no_same=False, self_cycle_max=1, no_cycle=True, confuse=False))

count = 0
for i in range(100):
    count += 1
    with open(f"data_cycle_{count}.txt", "w") as f:
        f.write(generate(length=1, no_same=False, self_cycle_max=2, no_cycle=False, confuse=False))

for i in range(100):
    count += 1
    with open(f"data_cycle_{count}.txt", "w") as f:
        f.write(generate(length=i // 5 + 1, no_same=False, self_cycle_max=4, no_cycle=False, confuse=False))

for i in range(100):
    count += 1
    with open(f"data_cycle_{count}.txt", "w") as f:
        f.write(generate(length=100, no_same=False, self_cycle_max=5, no_cycle=False, confuse=False))
