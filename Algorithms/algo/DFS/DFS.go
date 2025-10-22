package main

import (
	"bufio"
	"fmt"
	"os"
	"sort"
	"strconv"
	"strings"
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

type AbstractGraph struct {
	Graph map[int]map[int]int
}

func NewAbstractGraph() *AbstractGraph {
	return &AbstractGraph{
		Graph: make(map[int]map[int]int),
	}
}
func (g *AbstractGraph) AddEdge(from, to int, weight int) {
	if _, exists := g.Graph[from]; !exists {
		g.Graph[from] = make(map[int]int)
	}
	if g.Graph[from][to] < weight {
		g.Graph[from][to] = weight
	}
}
func (g *AbstractGraph) Display() {
	for from, edges := range g.Graph {
		for to, weight := range edges {
			fmt.Printf("%d -> %d [weight: %d]\n", from, to, weight)
		}
	}
}
func (g *AbstractGraph) FindPathDFS(start, end int, visited map[int]bool, path []int) []int {
	visited[start] = true
	path = append(path, start)
	if start == end {
		return path
	}
	for neighbor := range g.Graph[start] {
		if !visited[neighbor] {
			newPath := g.FindPathDFS(neighbor, end, visited, path)
			if newPath != nil {
				return newPath
			}
		}
	}

	return nil
}

func (g *AbstractGraph) DeleteNodes(targetWeight int) {
	nodesToDelete := make(map[int]bool)

	for from, edges := range g.Graph {
		for to, weight := range edges {
			reverseWeight, exists := g.Graph[to][from]

			// Check if it's a bidirectional edge
			if exists {
				maxWeight := weight
				if reverseWeight > maxWeight {
					maxWeight = reverseWeight
				}

				if targetWeight >= maxWeight {
					nodesToDelete[from] = true
					nodesToDelete[to] = true
				} else if weight < targetWeight {
					delete(edges, to)
				} else if reverseWeight < targetWeight {
					delete(g.Graph[to], from)
				}
			} else if weight == targetWeight {
				delete(edges, to)
			}
		}
	}

	// Remove nodes that are marked for deletion
	for node := range nodesToDelete {
		delete(g.Graph, node)
	}

	// Remove edges that point to the deleted nodes
	for _, edges := range g.Graph {
		for to := range edges {
			if nodesToDelete[to] {
				delete(edges, to)
			}
		}
	}
}

var index int
var stack []int
var indices map[int]int
var lowlink map[int]int
var onStack map[int]bool
var sccs [][]int

func strongconnect(v int, graph map[int]map[int]int) {
	indices[v] = index
	lowlink[v] = index
	index++
	stack = append(stack, v)
	onStack[v] = true

	for w := range graph[v] {
		if indices[w] == -1 {
			strongconnect(w, graph)
			lowlink[v] = min(lowlink[v], lowlink[w])
		} else if onStack[w] {
			lowlink[v] = min(lowlink[v], indices[w])
		}
	}

	if lowlink[v] == indices[v] {
		var scc []int

		for {
			var w int
			w, stack = stack[len(stack)-1], stack[:len(stack)-1]
			onStack[w] = false
			scc = append(scc, w)
			if w == v {
				break
			}
		}

		sccs = append(sccs, scc)
	}
}

func tarjanSCC(graph map[int]map[int]int) [][]int {
	index = 0
	stack = nil
	indices = make(map[int]int)
	lowlink = make(map[int]int)
	onStack = make(map[int]bool)
	sccs = nil

	for v := range graph {
		indices[v] = -1
		lowlink[v] = 0
		onStack[v] = false
	}

	for v := range graph {
		if indices[v] == -1 {
			strongconnect(v, graph)
		}
	}

	return sccs
}

func min(a, b int) int {
	if a < b {
		return a
	}
	return b
}

func main() {
	ans := 0
	fmt.Println(ans)
	in := bufio.NewReader(os.Stdin)
	data := readArrInt(in)

	graph := NewAbstractGraph()
	weights := make([]int, 0)

	for i := 0; i < data[1]; i++ {
		str2 := readArrInt(in)
		graph.AddEdge(str2[0], str2[1], str2[2])
		weights = append(weights, str2[0], str2[1])
	}
	sort.Slice(weights, func(i, j int) bool {
		return weights[i] < weights[j]
	})

	graph.Display()

	count := len(tarjanSCC(graph.Graph))
	for i := range weights {
		graph.DeleteNodes(i)
		fmt.Println(tarjanSCC(graph.Graph))
		if newCount := len(tarjanSCC(graph.Graph)); newCount != count {
			fmt.Println(i - 1)
			break
		} else {
			count = newCount
		}
	}
}
