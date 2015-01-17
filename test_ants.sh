#!/bin/bash

#(-b|) (--grasp -a 0.3|--grasp -a 0.6|--grasp -a 1|)

mkdir -p Results/test_ants/
make

for p in 10 30 60 90 120 180 300 400 500; do
	f=Instances/tcbvrp_$p.prob
	mkdir -p Results/test_ants/$p
	mkdir -p Results/test_ants/times/time_$p
	echo "Processing ants $f..";
	i=1
	
		stime=$(date +"%T")
		echo "Run $i started at $stime"
		before=$(date +%s)
		./tcbvrp $f > Results/ants_3/$p/try$i.out 2> Results/ants_3/$p/try$i.err
		after=$(date +%s)
		echo "Instance $f.. elapsed time:" $((after - $before)) "seconds"	> Results/ants_3/times/time_$p/try_$i.time
		echo "Saved: Instance $f.. elapsed time:" $((after - $before)) "seconds"
		#let i=$i+1	
done

for p in 10 30 60 90 120 180 300 400 500; do
	
	f=Instances/tcbvrp_$p.prob
	mkdir -p Results/test_ants/$p
	mkdir -p Results/test_ants/times/time_$p
	echo "Processing ants $f..";
	i=1
	while [ $i -le 29 ]
	do
		stime=$(date +"%T")
		echo "Run $i started at $stime"
		before=$(date +%s)
		./tcbvrp $f > Results/test_ants/$p/try$i.out 2> Results/test_ants/$p/try$i.err
		after=$(date +%s)
		echo "Instance $f.. elapsed time:" $((after - $before)) "seconds"	> Results/test_ants/times/time_$p/try_$i.time
		echo "Saved: Instance $f.. elapsed time:" $((after - $before)) "seconds"
		let i=$i+1	
	done
done

