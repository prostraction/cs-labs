package main

import (
	"fmt"
	"regexp"
	"strings"
)

func splitSentence(s string) []string {
	r := strings.Split(s, " ")

	var m map[string]int
	var mVisited map[string]bool
	m = make(map[string]int)
	mVisited = make(map[string]bool)

	for i := range r {
		r[i] = regexp.MustCompile(`[^a-zA-Z ]+`).ReplaceAllString(r[i], "")
		m[r[i]]++
	}

	fmt.Print("{")
	for k := range r {
		if k+1 != len(r) {
			if !mVisited[r[k]] {
				fmt.Print("\"", r[k], "\":", m[r[k]], ",")
				mVisited[r[k]] = true
			}
		} else {
			if !mVisited[r[k]] {
				fmt.Print("\"", r[k], "\": ", m[r[k]], "};")
				mVisited[r[k]] = true
			}
		}
	}

	return r
}

func main() {
	sentence := "John likes to watch movies. Mary likes movies too."
	sentenceAdd := "Mary also likes to watch football games."

	splitSentence(sentence)
	fmt.Print("\n")
	splitSentence(sentenceAdd)

	/*
		for i := range s {
			print("\"" + s[i] + "\"")
			if i+1 != len(s) {
				print(", ")
			}
		}
		print("\n")
		for i := range s1 {
			print("\"" + s1[i] + "\"")
			if i+1 != len(s1) {
				print(", ")
			}
		}
		print("\n")
	*/

}
