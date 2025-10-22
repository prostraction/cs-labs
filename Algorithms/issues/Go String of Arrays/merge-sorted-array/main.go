package main

import "fmt"

//	Runtime		2ms		Beats	72.01%	of users with Go
//	Memory		2.30MB		Beats	96.01%	of users with Go
func merge(nums1 []int, m int, nums2 []int, n int) {
	initBound := m
	for i := 0; i < n; i++ {
		nums1[initBound] = nums2[i]
		for j := initBound; j >= 1; j-- {
			if nums1[j-1] > nums1[j] {
				t := nums1[j-1]
				nums1[j-1] = nums1[j]
				nums1[j] = t
			} else {
				break
			}
		}
		initBound++
	}
}

func main() {
	arr1 := []int{1, 2, 3, 0, 0, 0}
	merge(arr1, 3, []int{2, 5, 6}, 3)
	arr2 := []int{1}
	merge(arr2, 1, []int{}, 0)
	arr3 := []int{0}
	merge(arr3, 0, []int{1}, 1)
	fmt.Println(arr1)
	fmt.Println(arr2)
	fmt.Println(arr3)
}
