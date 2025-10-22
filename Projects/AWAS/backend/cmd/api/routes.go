package main

import (
	"net/http"

	"github.com/julienschmidt/httprouter"
)

func (app *application) routes() http.Handler {
	router := httprouter.New()
	router.HandlerFunc(http.MethodGet, "/status", app.statusHandler)
	//////////////////////////////////////////////////////////////////////////
	//router.HandlerFunc(http.MethodGet, "/order/:id", app.getOrder)
	router.HandlerFunc(http.MethodPost, "/shops", app.postShop)

	router.HandlerFunc(http.MethodGet, "/shops", app.getShops)
	router.HandlerFunc(http.MethodGet, "/shops/:id", app.getShop)
	router.HandlerFunc(http.MethodGet, "/shops/:id/orders/buy", app.getOrdersBuyByShop)
	router.HandlerFunc(http.MethodGet, "/shops/:id/orders/sell", app.getOrdersSoldByShop)
	//////////////////////////////////////////////////////////////////////////
	router.HandlerFunc(http.MethodGet, "/product/:id", app.getProduct)
	router.HandlerFunc(http.MethodPost, "/product/", app.postProduct)
	router.HandlerFunc(http.MethodGet, "/products", app.getProducts)
	//////////////////////////////////////////////////////////////////////////
	return app.enableCors(router)
}
