#!/bin/bash

#(-b|) (--grasp -a 0.3|--grasp -a 0.6|--grasp -a 1|)

mkdir -p Results/grasp/0.3
mkdir -p Results/grasp/0.6
mkdir -p Results/grasp/1
mkdir -p Results/default/best
mkdir -p Results/default/first

for p in 10 30 60 90 120 180 300 400 500 700; do
	f=Instances/tcbvrp_$p.prob
	
	echo "Processing $f..";
	
	# GRASP
	alpha=0.6
	echo "GRASP $alpha..";
	./tcbvrp -b --grasp -a $alpha $f > Results/grasp/$alpha/$p.out 2> Results/grasp/$alpha/$p.err

	# Default
	echo "Best-improvement..";
	./tcbvrp -b $f > Results/default/best/$p.out 2> Results/default/best/$p.err
	echo "First-improvement..";
	./tcbvrp $f > Results/default/first/$p.out 2> Results/default/first/$p.err
done

for p in 10 30 60 90 120 180 300 400 500 700; do
	f=Instances/tcbvrp_$p.prob
	
	echo "Processing $f..";
	
	# GRASP
	alpha=0.3
	echo "GRASP $alpha..";
	./tcbvrp -b --grasp -a $alpha $f > Results/grasp/$alpha/$p.out 2> Results/grasp/$alpha/$p.err

	alpha=1
	echo "GRASP $alpha..";
	./tcbvrp -b --grasp -a $alpha $f > Results/grasp/$alpha/$p.out 2> Results/grasp/$alpha/$p.err
done