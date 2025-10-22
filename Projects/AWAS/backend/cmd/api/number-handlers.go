package main

/*
import (
	"errors"
	"net/http"
	"strconv"

	"github.com/julienschmidt/httprouter"
)

func (app *application) getNumber(w http.ResponseWriter, r *http.Request) {
	params := httprouter.ParamsFromContext(r.Context())
	id, err := strconv.Atoi(params.ByName("id"))
	if err != nil {
		app.logger.Println(errors.New("invalid id parameter"))
		app.errorJson(w, err)
		return
	}
	number, err := app.db.GetNumberInfo(id)
	if err != nil {
		app.errorJson(w, err)
		return
	}
	err = app.writeJson(w, http.StatusOK, number, "number")
	if err != nil {
		app.logger.Println(err)
	}
}

func (app *application) getNumbersAll(w http.ResponseWriter, r *http.Request) {

}
*/
