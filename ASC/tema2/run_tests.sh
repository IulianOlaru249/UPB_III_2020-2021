#!/bin/bash

echo "Queueing tasks..."
qsub -cwd -q ibm-nehalem.q queue_task.sh
qstat

echo "Waiting for result..."
while true
do
	if [ $(qstat | wc -l) -eq 0 ]
	then
		break
	fi
	sleep 1
done

echo ""
echo "Time reult..."
cat queue_task.sh.o*

# rm queue_task.sh.o*
rm queue_task.sh.e*

echo ""
echo "Diff reult..."
# ./compare out1 /export/asc/tema2/out1 0.0000000001
# ./compare out2 /export/asc/tema2/out2 0.0000000001
# ./compare out3 /export/asc/tema2/out3 0.0000000001
