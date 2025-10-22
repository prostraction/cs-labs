package main

import (
	"backend/models"
	"encoding/json"
	"errors"
	"net/http"
	"strconv"

	"github.com/julienschmidt/httprouter"
)

func (app *application) getOrder(w http.ResponseWriter, r *http.Request) {

}

func (app *application) getOrdersBuyByShop(w http.ResponseWriter, r *http.Request) {
	params := httprouter.ParamsFromContext(r.Context())
	id, err := strconv.Atoi(params.ByName("id"))
	if err != nil {
		app.logger.Println(errors.New("invalid id parameter"))
		app.errorJson(w, err)
		return
	}
	orders, err := app.db.GetOrdersBuyByShop(id)
	if err != nil {
		app.errorJson(w, err)
		return
	}
	err = app.writeJson(w, http.StatusOK, orders, "orders")
	if err != nil {
		app.logger.Println(err)
	}
}

func (app *application) getOrdersSoldByShop(w http.ResponseWriter, r *http.Request) {
	params := httprouter.ParamsFromContext(r.Context())
	id, err := strconv.Atoi(params.ByName("id"))
	if err != nil {
		app.logger.Println(errors.New("invalid id parameter"))
		app.errorJson(w, err)
		return
	}
	orders, err := app.db.GetOrdersSoldByShop(id)
	if err != nil {
		app.errorJson(w, err)
		return
	}
	err = app.writeJson(w, http.StatusOK, orders, "orders")
	if err != nil {
		app.logger.Println(err)
	}
}

// //////////////////////////////////////////////////////////////////////////
func (app *application) getProduct(w http.ResponseWriter, r *http.Request) {
	params := httprouter.ParamsFromContext(r.Context())
	id, err := strconv.Atoi(params.ByName("id"))
	if err != nil {
		app.logger.Println(errors.New("invalid id parameter"))
		app.errorJson(w, err)
		return
	}
	product, err := app.db.GetProduct(id)
	if err != nil {
		app.errorJson(w, err)
		return
	}
	err = app.writeJson(w, http.StatusOK, product, "product")
	if err != nil {
		app.logger.Println(err)
	}
}

func (app *application) postProduct(w http.ResponseWriter, r *http.Request) {
	var product models.Product
	decoder := json.NewDecoder(r.Body)
	decoder.UseNumber()
	err := decoder.Decode(&product)
	if err != nil {
		app.logger.Println(err)
	}
	err = app.db.PostProduct(product)
	if err != nil {
		app.logger.Println(err)
	}
}

func (app *application) getProducts(w http.ResponseWriter, r *http.Request) {
	products, err := app.db.GetProducts()
	if err != nil {
		app.errorJson(w, err)
		return
	}
	err = app.writeJson(w, http.StatusOK, products, "products")
	if err != nil {
		app.logger.Println(err)
	}
}

// //////////////////////////////////////////////////////////////////////////
func (app *application) getShop(w http.ResponseWriter, r *http.Request) {
	params := httprouter.ParamsFromContext(r.Context())
	id, err := strconv.Atoi(params.ByName("id"))
	if err != nil {
		app.logger.Println(errors.New("invalid id parameter"))
		app.errorJson(w, err)
		return
	}
	shop, err := app.db.GetShop(id)
	if err != nil {
		app.errorJson(w, err)
		return
	}
	err = app.writeJson(w, http.StatusOK, shop, "shop")
	if err != nil {
		app.logger.Println(err)
	}
}

func (app *application) postShop(w http.ResponseWriter, r *http.Request) {
	var shop models.Shop
	decoder := json.NewDecoder(r.Body)
	decoder.UseNumber()
	err := decoder.Decode(&shop)
	if err != nil {
		app.logger.Println(err)
	}
	err = app.db.PostShop(shop)
	if err != nil {
		app.logger.Println(err)
	}
}

func (app *application) getShops(w http.ResponseWriter, r *http.Request) {
	shops, err := app.db.GetShops()
	if err != nil {
		app.errorJson(w, err)
		return
	}
	err = app.writeJson(w, http.StatusOK, shops, "shops")
	if err != nil {
		app.logger.Println(err)
	}
}
