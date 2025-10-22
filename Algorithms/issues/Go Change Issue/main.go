package main

import (
	"fmt"
	"math"
)

/*
	You find yourself in a bookstore buying a book for $4.23 which you pay for with a $5 bill.
	You would be due 77 cents in change, and the cashier now makes a decision as to exactly how you get it.
	You would be annoyed at a fistful of 77 pennies or 15 nickels and 2 pennies, which raises the question of how to make change in the least annoying way.
	Most cashiers try to minimize the number of coins returned for a particular quantity of change.
	What is the minimum number of coins of denominations (25,10,5,1) needed to change 77 cents? (5)
 */
func ChangeDumb(money *int, nominal[]int, exchangedMoney *[]int) {
	for *money > 0 {
		for i := range nominal {
			if *money - nominal[i] >= 0 {
				*money = *money - nominal[i]
				*exchangedMoney = append(*exchangedMoney, nominal[i])
				break
			}
		}
	}
}
/*
	Change is an incorrect algorithm! Suppose you were changing 40 cents into coins with denominations of c1=25, c2=20, c3=10, c4=5, and c5=1.
	Change would incorrectly return 1 quarter, 1 dime, and 1 nickel, instead of 2 twenty-cent pieces.
	To correct the Chang algorithm, we could consider every possible combination of coins with denominations c1,c2,…,cd​ that adds to money,
	and return the combination with the fewest. We only need to consider combinations with i_1 ≤ money/c_1 and i_2 ≤ money/c_2
	(in general, i_k should not exceed money/c_k​), because we would otherwise be returning an amount of money larger than money.
*/
func ChangeCorrect(money *int, nominal []int, exchangedMoney *[]int) {
	smallestNumberOfCoins := math.MaxInt64
	var stackState = make([]int, len(nominal))
	var toExchangeLocal []int

	/* Heap's algorithm generates all possible permutations of n objects.
	For a collection C C containing n different elements, Heap found a systematic method for choosing at each step
	a pair of elements to switch in order to produce every possible permutation of these elements exactly once.
	Described recursively as a decrease and conquer method, Heap's algorithm operates at each step on the k k initial elements of the collection.
	Initially k = n and thereafter k < n. Each step generates the k ! permutations that end with the same n − k final elements.
	It does this by calling itself once with the k_th element unaltered and then k − 1 times with the k_th element exchanged for each of the initial k − 1 elements.
	The recursive calls modify the initial k − 1 elements and a rule is needed at each iteration to select which will be exchanged with the last.
	Heap's method says that this choice can be made by the parity of the number of elements operated on at this step.
	If k k is even, then the final element is iteratively exchanged with each element index.
	If k k is odd, the final element is always exchanged with the first.
	*/
	i := 0
	for i < len(nominal) {
		if stackState[i] < i {
			if i % 2 == 0 {
				nominal[0], nominal[i] = nominal[i], nominal[0]
			} else {
				nominal[stackState[i]], nominal[i] = nominal[i], nominal[stackState[i]]
			}
			m := *money
			*exchangedMoney = nil
			ChangeDumb(&m, nominal, exchangedMoney)
			if len(*exchangedMoney) < smallestNumberOfCoins {
				smallestNumberOfCoins = len(*exchangedMoney)
				toExchangeLocal = nil
				for j := range *exchangedMoney {
					toExchangeLocal = append(toExchangeLocal, (*exchangedMoney)[j])
				}
				toExchangeLocal = *exchangedMoney
			}
			stackState[i] += 1
			i = 1
		} else {
			stackState[i] = 0
			i++
		}
	}
	*exchangedMoney = toExchangeLocal
}
// 3.2 Greedy (incomplete)
func MaxNonOverlappedIntervals(interval *[][]int) int {
	if len(*interval) < 1 {
		return 0
	}
	if len(*interval) == 1 {
		return 1
	}
	var discardedIndexes []int
	var selectedIndexes []int
	var selectedIndex int

	for i := range (*interval)[1] {
		var minRightBound = (*interval)[1][i]
		for j := i; j < len((*interval)[1]) - i; j++ {
			if (*interval)[1][j] < minRightBound {
				isDiscarded := false
				for k := range discardedIndexes {
					if j == discardedIndexes[k] {
						isDiscarded = true
					}
				}
				if !isDiscarded {
					minRightBound = (*interval)[1][j]
					selectedIndex = j
				}
			}
		}

		//fmt.Println("min bound = ", minRightBound)

		for j := range (*interval)[0] {
			if (*interval)[0][j] < minRightBound && (*interval)[1][j] > minRightBound && j != selectedIndex {
				discardedIndexes = append(discardedIndexes, j)
			}
		}

		//fmt.Println("discarded: ", discardedIndexes)

		if selectedIndex != -1 {
			selectedIndexes = append(selectedIndexes, selectedIndex)
		}
		selectedIndex = -1
	}
	//fmt.Println(selectedIndexes)
	return len(selectedIndexes)
}
//


func CountMinimumDotsOnIntervals() (int, int) {
	return 0,0
}


func main() {
	fmt.Println("====================")
	{
		var money, m = 40, 40
		var toExchange []int
		ChangeDumb(&money, []int{25, 20, 10, 5, 1}, &toExchange)
		fmt.Println("Dumb Exchange:", m, "is", toExchange)
		money = m
		toExchange = nil
		ChangeCorrect(&money, []int{25, 20, 10, 5, 1}, &toExchange)
		fmt.Println("Brute force Exchange:", money, "is", toExchange)
	}
	fmt.Println("====================")
	{
		var t [][]int

		//row1 := []int{0, 4, 9}
		//row2 := []int{13, 6, 16}

		row1 := []int{0, 4, 0, 16, 4, 10, 8, 8, 3, 6, 12}
		row2 := []int{13, 6, 4, 18, 14, 12, 11, 10, 10, 4, 6}

		t = append(t, row1)
		t = append(t, row2)

		fmt.Println(t)

		fmt.Println(MaxNonOverlappedIntervals(&t))
	}
	fmt.Println("====================")
	{
		var countLines int
		fmt.Scan(&countLines)
		for i := 0; i < countLines; i++ {
			var n, m int
			fmt.Scan(&n, &m)
			fmt.Println(n, m)
		}
	}
}
