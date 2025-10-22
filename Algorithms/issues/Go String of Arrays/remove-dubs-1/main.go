package main

import "fmt"

// 	Runtime		9ms		Beats	59.66%	of users with Go
//	Memory		4.46MB		Beats	78.06%	of users with Go

func removeDuplicates(nums []int) int {
	prevVal := -101
	k := 0
	for i := 0; i < len(nums); i++ {
		if nums[i] != prevVal {
			nums[k] = nums[i]
			k++
		}
		prevVal = nums[i]
	}
	return k
}

func main() {
	arr1 := []int{1, 1, 2}
	arr2 := []int{0, 0, 1, 1, 1, 2, 2, 3, 3, 4}
	a1 := removeDuplicates(arr1)
	a2 := removeDuplicates(arr2)
	fmt.Println(a1, arr1)
	fmt.Println(a2, arr2)
}
