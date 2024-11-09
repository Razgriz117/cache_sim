import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import pandas as pd

df = pd.read_excel("/home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/assignment/cacti_table.xls")

do_aat = False
exp = 1
# get values
values = []
if exp == 1:
    for i in range(10, 21):
        L1size = 2**i
        sizewise = []
        for L1assoc in [1, 2, 4, 8, int(L1size/32)]:
            # size 1028, assoc 8 isn't in the table
            if do_aat and L1size == 1024 and L1assoc == 8:
                sizewise.append(np.nan)
                continue
            filename = "exp1/ASSOC_"+str(L1assoc)+"_L1Size_"+str(L1size)+".txt"
            with open(filename, "r") as file:
                L1MR = float(file.readlines()[0])
                if do_aat:
                    # calculate AAT = L1 hit time + L1 MR . Miss penalty(100 ns)
                    if L1assoc == int(L1size/32): assoc = "FA"
                    else: assoc = L1assoc
                    try:
                        L1hitTime = df[(df["Cache Size(bytes)"] == L1size) & 
                                    (df[" Block Size(bytes)"] == 32) &
                                    (df[" Associativity"] == assoc)][" Access Time(ns)"].item()
                    except:
                        if L1assoc == int(L1size/32): assoc = " FA"
                        L1hitTime = df[(df["Cache Size(bytes)"] == L1size) & 
                                    (df[" Block Size(bytes)"] == 32) &
                                    (df[" Associativity"] == assoc)][" Access Time(ns)"].item()

                    assert type(L1hitTime) == float
                    AAT = L1hitTime + L1MR * 100
                    sizewise.append(AAT)
                else:
                    sizewise.append(L1MR)
        values.append(sizewise)
    x = np.arange(10, 21)
elif exp == 2:
    for i in range (10, 19):
        L1size = 2**i
        sizewise = []
        for repl in [0, 1, 2]:
            filename = "exp2/REPL_"+str(repl)+"_L1Size_"+str(L1size)+".txt" 
            with open(filename, "r") as file:
                L1MR = float(file.readlines()[0])
                # calculate AAT = L1 hit time + L1 MR . Miss penalty(100 ns)
                try:
                    L1hitTime = df[(df["Cache Size(bytes)"] == L1size) & 
                                (df[" Block Size(bytes)"] == 32) &
                                (df[" Associativity"] == 4)][" Access Time(ns)"].item()
                except: breakpoint()
                assert type(L1hitTime) == float
                AAT = L1hitTime + L1MR * 100
                sizewise.append(AAT)
        values.append(sizewise)
    x = np.arange(10, 19)

elif exp == 3:
    L1hitTime = df[(df["Cache Size(bytes)"] == 1024) & 
                    (df[" Block Size(bytes)"] == 32) &
                    (df[" Associativity"] == 4)][" Access Time(ns)"].item()
    for i in range (11, 17):
        L2size = 2**i
        sizewise = []
        for incl in [0, 1]:
            filename = "exp3/INCL_"+str(incl)+"_L2Size_"+str(L2size)+".txt" 
            with open(filename, "r") as file:
                missRates = file.readlines()[0].split(" ")
                L1MR = float(missRates[0])
                L2MR = float(missRates[1].strip())
                # calculate AAT = L1 hit time + L1 MR (L2HT + L2MR . Miss penalty(100 ns))
                try:
                    L2hitTime = df[(df["Cache Size(bytes)"] == L2size) & 
                                (df[" Block Size(bytes)"] == 32) &
                                (df[" Associativity"] == 8)][" Access Time(ns)"].item()
                except: breakpoint()
                assert type(L1hitTime) == float
                AAT = L1hitTime + L1MR * (L2hitTime + L2MR * 100)
                sizewise.append(AAT)
        values.append(sizewise)
    x = np.arange(11, 17)

# transform
values = np.array([list(item) for item in zip(*values)])

# plot
plt.rcParams['mathtext.fontset'] = 'stix'
plt.rcParams['font.family'] = 'STIXGeneral'

for i, row in enumerate(values):
    color = cm.coolwarm(i / len(values))
    plt.plot(x, row, color=color)

# formatting
plt.grid(True, color='lightgray', alpha=0.5)
if exp == 3: plt.xlabel(r"$\log_2(\text{L2 Size})$", fontsize=14, labelpad=10)
else: plt.xlabel(r"$\log_2(\text{L1 Size})$", fontsize=14, labelpad=10)

if exp == 1:
    plt.legend(("Direct-mapped", "2-way Set Associative", "4-way Set Associative", "8-way Set Associative", "Fully Associative"),
           title="ASSOCIATIVITIES", loc='upper right', shadow=True)
    if do_aat: 
        plt.ylim(2, 20)
        plt.ylabel("Average Access Time (ns)", fontsize=14, labelpad=10)
        plt.title("Average Access Time (ns) vs. L1 Size for 5 Associativities", fontsize=16, pad=20)
        plt.savefig("exp1/exp1b.png", bbox_inches="tight", dpi=300)
    elif not do_aat: 
        plt.ylim(0.023, 0.2)
        plt.ylabel("L1 Miss Rate", fontsize=14, labelpad=10)
        plt.title("L1 Miss Rate vs. L1 Size for 5 Associativities", fontsize=16, pad=20)
        plt.savefig("exp1/exp1a.png", bbox_inches="tight", dpi=300)
elif exp == 2:
    plt.ylim(2, 16)
    plt.legend(("LRU", "FIFO", "Optimal"),
           title="REPLACEMENT POLICIES", loc='upper right', shadow=True)
    plt.ylabel("Average Access Time (ns)", fontsize=14, labelpad=10) 
    plt.title("Average Access Time (ns) vs. L1 Size for 3 Replacement Policies", fontsize=16, pad=20)
    plt.savefig("exp2/exp2.png", bbox_inches="tight", dpi=300)
elif exp == 3:
    plt.ylim(2, 10)
    plt.legend(("Non-inclusive", "Inclusive"),
           title="INCLUSION PROPERTIES", loc='upper right', shadow=True)
    plt.ylabel("Average Access Time (ns)", fontsize=14, labelpad=10) 
    plt.title("Average Access Time (ns) vs. L2 Size for 2 Inclusion Policies", fontsize=16, pad=20)
    plt.savefig("exp3/exp3.png", bbox_inches="tight", dpi=300) 

plt.show()
