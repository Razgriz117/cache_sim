#!/bin/bash
# make clean
# make

# experiment one
if [ "$1" == "exp1" ]; then
    for (( outer=10; outer<=20; outer++ )); do
        L1size=$((2**outer))
        echo "L1 size: $L1size"
        for L1Assoc in 1 2 4 8 "$L1size"; do
            if [ "$L1Assoc" -eq "$L1size" ]; then
                L1Assoc=$((L1Assoc / 32))
            fi
            echo "  L1 associativity: $L1Assoc"
            ./sim_cache 32 $L1size $L1Assoc 0 0 0 0 /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/assignment/traces/gcc_trace.txt
        done
    done
# experiment two
elif [ "$1" == "exp2" ]; then
    for (( outer=10; outer<=18; outer++ )); do
        L1size=$((2**outer))
        echo "L1 size: $L1size"
        # LRU
        ./sim_cache 32 $L1size 4 0 0 0 0 /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/assignment/traces/gcc_trace.txt
        # FIFO
        ./sim_cache 32 $L1size 4 0 0 1 0 /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/assignment/traces/gcc_trace.txt
        # OPTIMAL
        ./sim_cache 32 $L1size 4 0 0 2 0 /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/assignment/traces/gcc_trace.txt
    done
# experiment 3
elif [ "$1" == "exp3" ]; then
    for (( outer=11; outer<=16; outer++ )); do
        L2size=$((2**outer))
        # non-inclusive
        ./sim_cache 32 1024 4 $L2size 8 0 0 /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/assignment/traces/gcc_trace.txt
        # inclusive   
        ./sim_cache 32 1024 4 $L2size 8 0 1 /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/assignment/traces/gcc_trace.txt
    done
fi