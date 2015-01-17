#!/bin/bash

#(-b|) (--grasp -a 0.3|--grasp -a 0.6|--grasp -a 1|)

mkdir -p Results/test_ants/
make

i=1
while [ $i -le 15 ] do
	for p in 10 30 60 90 120 180 300 400 500; do
		
		f=Instances/tcbvrp_$p.prob
		mkdir -p Results/test_ants/$p
		mkdir -p Results/test_ants/times/time_$p
		
		stime=$(date +"%T")
		echo "Run $i with pheromone = 1 started at $stime"
		before=$(date +%s)
		./tcbvrp --ant1 $f > Results/test_ants/$p/try$i.out 2> Results/test_ants/$p/try$i.err
		after=$(date +%s)
		echo "Instance $f with pheromone = 1 .. elapsed time:" $((after - $before)) "seconds"	> Results/test_ants/times/time_$p/try_$i_ph_1.time
		echo "Saved: Instance $f with pheromone = 1 .. elapsed time:" $((after - $before)) "seconds"

		stime=$(date +"%T")
		echo "Run $i with pheromone = 2 started at $stime"
		before=$(date +%s)
		./tcbvrp --ant2 $f > Results/test_ants/$p/try$i.out 2> Results/test_ants/$p/try$i.err
		after=$(date +%s)
		echo "Instance $f with pheromone = 2 .. elapsed time:" $((after - $before)) "seconds"	> Results/test_ants/times/time_$p/try_$i_ph_2.time
		echo "Saved: Instance $f with pheromoe = 2 .. elapsed time:" $((after - $before)) "seconds"
	done
	((i++))
done
