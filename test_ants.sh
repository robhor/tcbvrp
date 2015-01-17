#!/bin/bash

#(-b|) (--grasp -a 0.3|--grasp -a 0.6|--grasp -a 1|)

mkdir -p Results/ants_3/
mkdir -p Results/ants_3/times
make

for p in 60; do
	f=Instances/tcbvrp_$p.prob
	mkdir -p Results/ants_3/$p
	mkdir -p Results/ants_3/times/time_$p
	echo "Processing ants $f..";
	i=1
	while [ $i -le 10 ]
	do
		stime=$(date +"%T")
		echo "Run $i started at $stime"
		before=$(date +%s)
		./tcbvrp $f > Results/ants_3/$p/try$i.out 2> Results/ants_3/$p/try$i.err
		after=$(date +%s)
		echo "Instance $f.. elapsed time:" $((after - $before)) "seconds"	> Results/ants_3/times/time_$p/try_$i.time
		echo "Saved: Instance $f.. elapsed time:" $((after - $before)) "seconds"
		let i=$i+1	
	done
done

