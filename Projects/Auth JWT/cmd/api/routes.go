package main

import (
	"encoding/json"
	"net/http"
	"strconv"
)

type Response struct {
	Access  string `json:"access"`
	Refresh string `json:"refresh"`
}

// Первый маршрут выдает пару Access, Refresh токенов для пользователя сидентификатором (GUID) указанным в параметре запроса
func (app *Application) getAuth(w http.ResponseWriter, r *http.Request) {
	guid := r.FormValue("guid")
	id, _ := strconv.Atoi(guid)

	if res, err := app.genTokenPair(id); err != nil {
		w.WriteHeader(502)
		w.Header().Add("Content-Type", "application/json")
		errJSON, _ := json.Marshal(err.Error())
		w.Write(errJSON)
		app.log.Print("getAuth error: ", err, " with id ", id)
	} else {
		w.WriteHeader(200)
		w.Header().Add("Content-Type", "application/json")
		resJSON, _ := json.Marshal(res)
		w.Write(resJSON)
	}
}

// Второй маршрут выполняет Refresh операцию на пару Access, Refresh токенов
func (app *Application) refreshAuth(w http.ResponseWriter, r *http.Request) {
	guid := r.FormValue("guid")
	rt := r.FormValue("refresh")
	id, _ := strconv.Atoi(guid)

	if res, err := app.refTokenPair(id, rt); err != nil {
		w.WriteHeader(502)
		w.Header().Add("Content-Type", "application/json")
		errJSON, _ := json.Marshal(err.Error())
		w.Write(errJSON)
		app.log.Print("refreshAuth error: ", err, " with id ", id)
	} else {
		w.WriteHeader(200)
		w.Header().Add("Content-Type", "application/json")
		resJSON, _ := json.Marshal(res)
		w.Write(resJSON)
	}
}
