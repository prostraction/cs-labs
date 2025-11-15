package main

import (
  "bufio"
  "fmt"
  "os"
  "strconv"
  "strings"
  "time"
)

func readInt(in *bufio.Reader) int {
  nStr, _ := in.ReadString('\n')
  nStr = strings.ReplaceAll(nStr, "\r", "")
  nStr = strings.ReplaceAll(nStr, "\n", "")
  n, _ := strconv.Atoi(nStr)
  return n
}

func readLineNumbs(in *bufio.Reader) []string {
  line, _ := in.ReadString('\n')
  line = strings.ReplaceAll(line, "\r", "")
  line = strings.ReplaceAll(line, "\n", "")
  numbs := strings.Split(line, " ")
  return numbs
}

func readLineStr(in *bufio.Reader) string {
  line, _ := in.ReadString('\n')
  line = strings.ReplaceAll(line, "\r", "")
  line = strings.ReplaceAll(line, "\n", "")
  return line
}

func readArrInt(in *bufio.Reader) []int {
  numbs := readLineNumbs(in)
  arr := make([]int, len(numbs))
  for i, n := range numbs {
    val, _ := strconv.Atoi(n)
    arr[i] = val
  }
  return arr
}

type pair struct {
  a, b int
}

type table struct {
  rows [][]int
  n    int
  m    int
}

type interval struct {
  a, b time.Time
}

type cpu struct {
  power         int
  timeTask      int
  timeSpent     int
  busy          bool
  pointerToFree int
}

func remove(slice []pair, s int) []pair {
  return append(slice[:s], slice[s+1:]...)
}

func findLargestNum(array []int) int {
  largestNum := 0

  for i := 0; i < len(array); i++ {
    if array[i] > largestNum {
      largestNum = array[i]
    }
  }
  return largestNum
}

func radixSort(array []int) []int {

  //fmt.Println("Running Radix Sort on Unsorted List\n")

  // Base 10 is used
  largestNum := findLargestNum(array)
  size := len(array)
  significantDigit := 1
  semiSorted := make([]int, size, size)

  // Loop until we reach the largest significant digit
  for largestNum/significantDigit > 0 {

    //fmt.Println("\tSorting: "+strconv.Itoa(significantDigit)+"'s place", array)

    bucket := [10]int{0}

    // Counts the number of "keys" or digits that will go into each bucket
    for i := 0; i < size; i++ {
      bucket[(array[i]/significantDigit)%10]++
    }

    // Add the count of the previous buckets
    // Acquires the indexes after the end of each bucket location in the array
    // Works similar to the count sort algorithm
    for i := 1; i < 10; i++ {
      bucket[i] += bucket[i-1]
    }

    // Use the bucket to fill a "semiSorted" array
    for i := size - 1; i >= 0; i-- {
      bucket[(array[i]/significantDigit)%10]--
      semiSorted[bucket[(array[i]/significantDigit)%10]] = array[i]
    }

    // Replace the current array with the semisorted array
    for i := 0; i < size; i++ {
      array[i] = semiSorted[i]
    }

    //fmt.Println("\n\tBucket: ", bucket)

    // Move to next significant digit
    significantDigit *= 10
  }

  return array
}

func main() {
  in := bufio.NewReader(os.Stdin)
  counts := readArrInt(in)

  cpuCount := counts[0]
  taskCount := counts[1]

  //var CPUs []cpu
  CPUs := make([]cpu, cpuCount, cpuCount)
  CPUsBusy := make([]cpu, cpuCount, cpuCount)
  cpuPower := readArrInt(in)

  /* sort */
  radixSort(cpuPower)
  //sort.Slice(CPUs, func(i, j int) bool {
  //  return CPUs[i].power < CPUs[j].power
  //})

  for i := 0; i < cpuCount; i++ {
    var cpuT cpu
    cpuT.power = cpuPower[i]
    cpuT.timeSpent = 0
    cpuT.timeTask = 0
    cpuT.pointerToFree = 0
    cpuT.busy = false
    CPUs[i] = cpuT
    //CPUs = append(CPUs, cpuT)
  }

  busyCPUCount := 0
  taskPassed := 0
  cpuPowerUsage := 0
  currentTime := 0
  for i := 0; i < taskCount; i++ {
    task := readArrInt(in)
    deltaTime := task[0] - currentTime
    for j := 0; j < busyCPUCount; j++ {
      CPUsBusy[j].timeSpent += deltaTime
      if CPUsBusy[j].timeSpent >= CPUsBusy[j].timeTask {
        for k := 0; k < CPUs[j].power; k++ {
          cpuPowerUsage += CPUsBusy[j].timeTask
        }

        CPUsBusy[j].timeTask = 0
        CPUsBusy[j].timeSpent = 0
        CPUsBusy[j].busy = false
        taskPassed++
        busyCPUCount--
      }
    }

    for j := 0; j < cpuCount; j++ {
      fmt.Println("=== ", j, ": ", CPUs[j].busy)
      // fill cpu
      if !CPUs[j].busy {
        CPUs[j].busy = true
        CPUs[j].timeTask = task[1]
        CPUs[j].timeSpent = 0

        busyCPUCount++
        break
      }
    }
    fmt.Println(CPUsBusy)
    currentTime = task[0]
  }
  for j := 0; j < cpuCount; j++ {
    for k := 0; k < CPUs[j].power; k++ {
      cpuPowerUsage += CPUsBusy[j].timeTask
    }

    CPUsBusy[j].timeTask = 0
    CPUsBusy[j].timeSpent = 0
    CPUsBusy[j].busy = false
    taskPassed++
  }

  fmt.Println(cpuPowerUsage)

}