package main

import "fmt"

//	Runtime		1ms		Beats	70.70%	of users with Go
//	Memory		2.19MB		Beats	73.46%	of users with Go

func removeElement(nums []int, val int) int {
	j := 0
	for i := 0; i < len(nums); i++ {
		if nums[i] != val {
			nums[j], nums[i] = nums[i], nums[j]
			j++
		}
	}
	return j
}

func main() {
	arr1 := []int{3, 2, 2, 3}
	c1 := removeElement(arr1, 3)
	arr2 := []int{0, 1, 2, 2, 3, 0, 4, 2}
	c2 := removeElement(arr2, 2)
	fmt.Println(c1, arr1)
	fmt.Println(c2, arr2)
}
