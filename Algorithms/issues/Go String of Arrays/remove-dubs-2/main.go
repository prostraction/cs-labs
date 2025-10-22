package main

import "fmt"

//	Runtime		4ms		Beats	70.05%	of users with Go
//	Memory		2.88MB		Beats	93.92%	of users with Go

func removeDuplicates(nums []int) int {
	if len(nums) < 2 {
		return len(nums)
	}
	prevVal := nums[0]
	c := 0
	k := 0
	for i := 1; i < len(nums); i++ {
		if nums[i] == prevVal {
			c++
		}
		if nums[i] != prevVal {
			nums[k] = prevVal
			k++
			if c >= 1 {
				nums[k] = prevVal
				k++
			}
			c = 0
		}
		if i+1 == len(nums) {
			nums[k] = nums[i]
			k++
			if c >= 1 {
				nums[k] = nums[i]
				k++
			}
		}
		prevVal = nums[i]
	}
	return k
}

func main() {
	arr1 := []int{1, 1, 1, 2, 2, 3}          // 5
	arr2 := []int{0, 0, 1, 1, 1, 1, 2, 3, 3} // 7
	arr3 := []int{1, 1, 1}
	a1 := removeDuplicates(arr1)
	a2 := removeDuplicates(arr2)
	a3 := removeDuplicates(arr3)
	fmt.Println(a1, arr1)
	fmt.Println(a2, arr2)
	fmt.Println(a3, arr3)
}
