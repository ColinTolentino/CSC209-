#!/bin/bash


declare -i lab_marks=0;
declare -i lab_total=0;
declare -i assignment_marks=0;
declare -i assignment_total=0;

function marks {
	grade=`tail -1 ./$1/feedback.txt | tr -d [:blank:]`
	IFS='/' read -ra grArray <<< "$grade"
	score=${grArray[0]}
	max=${grArray[1]}
	if [ $2 = labs ]; then
		lab_marks=$lab_marks+score;
		lab_total=$lab_total+max;
	fi
	if [ $2 = assignments ]; then
		assignment_marks=$lab_marks+score;
		assignment_total=$lab_total+max;
	fi
	echo "$1: $score / $max"
}

for assignment in a1 a2 a3 a4 ; do
	if [ -f "./$labs/feedback.txt" ]; then
		marks $assignment assignments
	else
		echo "$assignment: - / -"
	fi

done
echo ""

for labs in w1 w2 w3 w4 w5 w6 w7 w8 w9 w10 w11; do
	if [ -f "./$labs/feedback.txt" ]; then
		marks $labs labs
	else
		echo "$labs: - / -"
	fi
done

echo ""
echo "Assignment Total: $assignment_marks / $assignment_total"
echo "Lab Total: $lab_marks / $lab_total"



