package main

import "fmt"

//	Runtime	94ms	Beats	91.45%	of users with Go
//	Memory	7.55MB		Beats	95.80%	of users with Go

func maxProfit(prices []int) int {
	// var is faster then :=? Plus, less memory storage? Compare it with benchmark later...
	var ret = 0
	var minVal = prices[0]
	for i := 1; i < len(prices); i++ {
		if prices[i] < minVal {
			minVal = prices[i]
		} else if prices[i]-minVal > ret {
			ret = prices[i] - minVal
		}
	}
	return ret
}

func main() {
	fmt.Println(maxProfit([]int{7, 1, 5, 3, 6, 4}))
	fmt.Println(maxProfit([]int{7, 6, 4, 3, 1}))
}
