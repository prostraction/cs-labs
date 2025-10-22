package main

import "fmt"

//	Runtime		9ms		Beats	93.63%	of users with Go
//	Memory		6.19MB		Beats	77.94%	of users with Go

// Boyer–Moore majority vote algorithm
func majorityElement(nums []int) int {
	m := 0
	c := 0
	for i := 0; i < len(nums); i++ {
		if c == 0 {
			m = nums[i]
			c = 1
		} else if m == nums[i] {
			c++
		} else {
			c--
		}
	}
	return m
}

func main() {
	arr1 := []int{6, 5, 5}
	arr2 := []int{3, 3, 4}
	fmt.Println(majorityElement(arr1))
	fmt.Println(majorityElement(arr2))
}
