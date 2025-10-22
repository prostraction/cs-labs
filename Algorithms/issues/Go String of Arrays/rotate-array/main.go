package main

import "fmt"

// 	Runtime	25ms	Beats	72.30%	of users with Go
//	Memory	6.92MB		Beats	92.78%	of users with Go

func reverse(nums []int, from int, to int) {
	for i, j := from, to; i < j; i, j = i+1, j-1 {
		nums[i], nums[j] = nums[j], nums[i]
	}
}

func rotate(nums []int, k int) {	
	if len(nums) > 1 {
		reverse(nums, 0, len(nums) - k % len(nums) - 1)
		reverse(nums, len(nums) - k % len(nums) , len(nums) - 1)
		reverse(nums, 0, len(nums) - 1)
	}
}

func main() {
	arr1 := []int{1, 2, 3, 4, 5, 6, 7}
	arr2 := []int{1, 2}
	arr3 := []int{1, 2, 3}
	rotate(arr1, 3)
	rotate(arr2, 2)
	rotate(arr3, 4)
	fmt.Println(arr1, 1)
	fmt.Println(arr2, 2)
	fmt.Println(arr3, 2)
}
