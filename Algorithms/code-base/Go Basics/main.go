package main

import (
	"fmt"
	"sort"
	"strings"
	"sync"
	"unicode/utf8"
)

func SumWorker(numsCh chan []int, sumCh chan int) {
	for {
	  t := <-numsCh
	  fmt.Println(t)
	  sum := 0
	  for _, i := range t {
		sum += i
	  }
	  sumCh <- sum
	}
  }

func MaxSum(nums1, nums2 []int) []int {
	mutex := sync.Mutex{}
	wg := sync.WaitGroup{}
	sums := make([]int, 2)

	wg.Add(1)
	go func(nums []int, sums *[]int) {
		defer wg.Done()
		sum := 0
		for i := range nums1 {
			sum += i
		}
		mutex.Lock()
		(*sums)[0] = sum
		mutex.Unlock()

	}(nums1, &sums)

	wg.Add(1)
	go func(nums []int, sums *[]int) {
		defer wg.Done()
		sum := 0
		for i := range nums2 {
			sum += i
		}
		mutex.Lock()
		(*sums)[1] = sum
		mutex.Unlock()

	}(nums2, &sums)

	wg.Wait()

	if sums[1] > sums[0] {
		return nums2
	} else {
		return nums1
	}
}

// /////////////////////////////////
type Person struct {
	Age uint8
}

type PersonList []Person

func (pl PersonList) GetAgePopularity() map[uint8]int {
	m := make(map[uint8]int)
	for _, i := range pl {
		m[uint8(i.Age)]++
	}
	return m
}

// /////////////////////////////////
type Counter struct {
	Value int
}

func Max(x, y int) int {
	if x < y {
		return y
	}
	return x
}

func (c *Counter) Inc(delta int) {
	if delta == 0 {
		c.Value++
	} else {
		c.Value += delta
	}
}

func (c *Counter) Dec(delta int) {
	if delta == 0 {
		c.Value--
	} else {
		c.Value -= delta
	}
}

// /////////////////////////////////
type Parent struct {
	Name     string
	Children []Child
}
type Child struct {
	Name string
	Age  int
}

func CopyParent(p *Parent) Parent {
	newP := Parent{}
	if p == nil {
		return newP
	}
	newP.Name = (*p).Name
	newP.Children = append(newP.Children, (*p).Children...)
	return newP
}

func MergeNumberLists(numberLists ...[]int) []int {
	m := make([]int, 0)
	for i := range numberLists {
		m = append(m, numberLists[i]...)
	}
	return m
}

func GenerateSelfStory(name string, age int, money float64) string {
	return fmt.Sprintf("Hello! My name is %s. I'm %d y.o. And I also have $%0.2f in my wallet right now.", name, age, money)
}

func LatinLetters(s string) string {
	var sb strings.Builder
	for i, _ := range s {
		if (s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z') {
			sb.WriteByte(s[i])
		}
	}
	return sb.String()
}

func IsASCII(s string) bool {
	return utf8.RuneCountInString(s) == len(s)
}

func ShiftASCII(s string, step int) string {
	newS := make([]byte, len(s))
	for i := 0; i < len(s); i++ {
		newS[i] = s[i] + byte(step)
	}
	return string(newS)
}

func NextASCII(b byte) byte {
	return b + 1
}

func PrevASCII(b byte) byte {
	return b - 1
}

func MostPopularWord(words []string) string {
	mPopular := make(map[string]int)
	for _, val := range words {
		mPopular[val]++
	}
	maxVal := 0
	maxStr := ""
	for i, val := range mPopular {
		if val > maxVal {
			maxVal = val
			maxStr = i
		}
	}
	return maxStr
}

func UniqueUserIDs(userIDs []int64) []int64 {
	sOrder := make([]int64, 0)
	mCount := make(map[int64]int64)
	for _, val := range userIDs {
		if mCount[val] < 1 {
			mCount[val]++
			sOrder = append(sOrder, val)
		}
	}
	return sOrder
}

func UniqueSortedUserIDs(userIDs []int64) []int64 {
	length, newLen := len(userIDs), 1
	if length < 2 {
		return userIDs
	}
	sort.Slice(userIDs, func(i, j int) bool {
		return userIDs[i] < userIDs[j]
	})
	// move dubs
	for i := 1; i < length; i++ {
		if userIDs[i] != userIDs[i-1] {
			userIDs[newLen] = userIDs[i]
			newLen++
		}
	}
	return userIDs[:newLen]
}

func IntsCopy(src []int, maxLen int) []int {
	if maxLen < 1 {
		return []int{}
	}
	newSlice := make([]int, len(src))
	copy(newSlice, src)
	if len(src) < maxLen {
		return newSlice[:len(src)]
	} else {
		return newSlice[:maxLen]
	}
}

func Map(strs []string, mapFunc func(s string) string) []string {
	newStrs := make([]string, 0)
	for i, _ := range strs {
		newStrs = append(newStrs, mapFunc(strs[i]))
	}
	return newStrs
}

func Remove(nums []int, i int) []int {
	if i >= 0 && i < len(nums) {
		return append(nums[:i], nums[i+1:]...)
	}
	return nums
}

func SafeWrite(nums [5]int, i, val int) [5]int {
	if i < 5 && i >= 0 {
		nums[i] = val
	}
	return nums
}

func ErrorMessageToCode(msg string) int {
	switch msg {
	case "OK":
		return 0
	case "CANCELLED":
		return 1
	default:
		return 2
	}
}

type UserCreateRequest struct {
	FirstName string
	Age       int
}

func Validate(req UserCreateRequest) string {
	if strings.ContainsAny(req.FirstName, " ") || len(req.FirstName) == 0 || req.Age < 1 || req.Age > 150 {
		return "invalid request"
	}
	return ""
}

func ModifySpaces(s, mode string) string {
	switch mode {
	case "dash":
		return strings.ReplaceAll(s, " ", "-")
	case "underscore":
		return strings.ReplaceAll(s, " ", "_")
	default:
		return strings.ReplaceAll(s, " ", "*")
	}
}

func DomainForLocale(domain, locale string) string {
	if strings.Count(domain, ".") == 1 {
		if len(locale) == 0 {
			return "en." + domain
		}
		return locale + "." + domain
	}
	return domain
}
