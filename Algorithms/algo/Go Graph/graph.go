package main

import "fmt"

func runAlgo(graph map[string][]string, start, destination string) (result int) {
 processed := map[string]bool{}
 processVertexRecursive(start, graph[start], destination, &graph, &result, &processed)
 return
}

func processVertexRecursive(
 start string,
 vertices []string,
 destination string,
 graph *map[string][]string,
 result *int,
 processed *map[string]bool,
) {
 *result++                              // счетчик итераций
 nextLevelVertices := make([]string, 0) // очередь для соседей
 for _, v := range vertices {
  if v == start {
   continue // не движемся в обратном направлении
  }
  if (*processed)[v] {
   continue // не перебираем одну вершину два раза
  }
  (*processed)[v] = true
  if v == destination {
   return // мы дошли до целевой вершины
  } else {
   nextLevelVertices = append(nextLevelVertices, (*graph)[v]...) // добавляем соседей в очередь
  }
 }

 // рекурсивно обрабатываем очередь соседей
 processVertexRecursive(start, nextLevelVertices, destination, graph, result, processed)
}

func main() {
 graph := map[string][]string{
  "a": {"b", "d"},
  "b": {"a", "c"},
  "c": {"b", "f", "j"},
  "d": {"a", "i"},
  "f": {"c"},
  "i": {"d", "j"},
  "j": {"c", "i"},
 }
 graph["a"] = append(graph["a"], "y")
 fmt.Println(graph)

 r := runAlgo(graph, "a", "j")
 fmt.Println(r)

}