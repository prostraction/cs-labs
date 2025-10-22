package main

import (
	"auth-jwt/internal/db"
	"log"
	"net/http"
)

type Application struct {
	mux       *http.ServeMux
	db        db.Database
	log       *log.Logger
	accessMap map[int]string
}

func main() {
	a := Application{}
	a.db.URI = "mongodb://localhost:27017"
	a.mux = http.NewServeMux()
	a.mux.HandleFunc("/getAuth", a.getAuth)
	a.mux.HandleFunc("/refreshAuth", a.refreshAuth)
	a.log = log.Default()
	err := http.ListenAndServe(":4000", a.mux)
	log.Fatal(err)
}
