package models

import (
	"context"
	"log"
	"strings"

	"github.com/jackc/pgx/v4/pgxpool"
)

type Database struct {
	con *pgxpool.Pool
}

func (db *Database) Init() (err error) {
	DB_URL := `postgres://postgres:postgres@localhost:5432/postgres`
	db.con, err = pgxpool.Connect(context.Background(), DB_URL)
	if err != nil {
		log.Fatal(err)
	}
	err = db.createTables()
	if err != nil {
		log.Fatal(err)
	}
	return
}

func (db *Database) noEmptyError(err error) error {
	if strings.Contains(err.Error(), "no rows in result set") {
		return nil
	}
	return err
}

func (db *Database) createTable(query string) (err error) {
	err = db.con.QueryRow(context.Background(), query).Scan()
	if db.noEmptyError(err) != nil {
		return err
	}
	return nil
}

func (db *Database) createTables() (err error) {
	if err = db.createTable(`CREATE TABLE IF NOT EXISTS products (id SERIAL PRIMARY KEY,name TEXT NOT NULL,article VARCHAR(30) NOT NULL,description jsonb NOT NULL);`); err != nil {
		return err
	}
	if err = db.createTable(`CREATE TABLE IF NOT EXISTS shops (id SERIAL PRIMARY KEY,name TEXT NOT NULL,address TEXT NOT NULL);`); err != nil {
		return err
	}
	if err = db.createTable(`CREATE TABLE IF NOT EXISTS shop_products (id SERIAL PRIMARY KEY,product_id INTEGER,amount INTEGER,price DECIMAL(10, 2),FOREIGN KEY (product_id) REFERENCES products (id) ON DELETE CASCADE);`); err != nil {
		return err
	}
	if err = db.createTable(`CREATE TABLE IF NOT EXISTS shops_proxy (shop_id INTEGER,shop_products_id INTEGER,FOREIGN KEY (shop_id) REFERENCES shops (id) ON DELETE CASCADE,FOREIGN KEY (shop_products_id) REFERENCES shop_products (id) ON DELETE CASCADE);`); err != nil {
		return err
	}
	if err = db.createTable(`CREATE TABLE IF NOT EXISTS orders (id SERIAL PRIMARY KEY,time TIMESTAMP NOT NULL,order_type INT NOT NULL,shop_id INTEGER NOT NULL,distrub TEXT,status TEXT,FOREIGN KEY (shop_id) REFERENCES shops (id) ON DELETE SET NULL);`); err != nil {
		return err
	}
	if err = db.createTable(`CREATE TABLE IF NOT EXISTS order_details (order_details_id SERIAL PRIMARY KEY,product_id INTEGER,amount INTEGER,price DECIMAL(10,3),FOREIGN KEY (product_id) REFERENCES products (id) ON DELETE SET NULL);`); err != nil {
		return err
	}
	if err = db.createTable(`CREATE TABLE IF NOT EXISTS order_proxy (order_id INTEGER,order_details_id INTEGER,FOREIGN KEY (order_id) REFERENCES orders (id) ON DELETE CASCADE,FOREIGN KEY (order_details_id) REFERENCES order_details (order_details_id) ON DELETE CASCADE);`); err != nil {
		return err
	}
	return nil
}

//	Products
//
// ////////////////////////////////////////////////////////////////////////////
func (db *Database) GetProduct(id int) (product Product, err error) {
	err = db.con.QueryRow(context.Background(), "SELECT id, name, article, description FROM products WHERE id = $1;", id).Scan(
		&product.ID,
		&product.Name,
		&product.Article,
		&product.Description)
	return
}

func (db *Database) PostProduct(product Product) (err error) {
	err = db.con.QueryRow(context.Background(), "INSERT INTO products (name, article, description) VALUES ($1, $2, $3) RETURNING id", product.Name, product.Article, product.Description).Scan(&product.ID)
	return
}

func (db *Database) GetProducts() (products []*Product, err error) {
	rows, err := db.con.Query(context.Background(), "SELECT id, name, article, description FROM products ORDER BY id DESC;")
	for rows.Next() {
		var product Product
		err := rows.Scan(
			&product.ID,
			&product.Name,
			&product.Article,
			&product.Description)
		if err != nil {
			return nil, err
		}
		products = append(products, &product)
	}
	return
}

//	Shops
//
// ////////////////////////////////////////////////////////////////////////////
func (db *Database) GetShop(id int) (shop Shop, err error) {
	err = db.con.QueryRow(context.Background(), "SELECT id, name, address FROM shops WHERE id = $1;", id).Scan(
		&shop.ID,
		&shop.Name,
		&shop.Address)
	return
}

func (db *Database) PostShop(shop Shop) (err error) {
	err = db.con.QueryRow(context.Background(), "INSERT INTO shops (name, address) VALUES ($1, $2) RETURNING id", shop.Name, shop.Address).Scan(&shop.ID)
	return
}

func (db *Database) GetShops() (shops []*Shop, err error) {
	rows, err := db.con.Query(context.Background(), "SELECT id, name, address FROM shops ORDER BY id DESC;")
	for rows.Next() {
		var shop Shop
		err := rows.Scan(
			&shop.ID,
			&shop.Name,
			&shop.Address)
		if err != nil {
			return nil, err
		}
		shops = append(shops, &shop)
	}
	return
}

//	Orders
//
// ////////////////////////////////////////////////////////////////////////////
/*
func (db *Database) GetOrder(id int) (shop Shop, err error) {
	err = db.con.QueryRow(context.Background(), "SELECT id, name, address FROM shops WHERE id = $1;", id).Scan(
		&shop.ID,
		&shop.Name,
		&shop.Address)
	return
}

func (db *Database) PostBuyOrder(order Order, orders []*Order) (err error) {
	err = db.con.QueryRow(context.Background(), "INSERT INTO orders (name, address) VALUES ($1, $2) RETURNING id", shop.Name, shop.Address).Scan(&shop.ID)
	return
}*/

func (db *Database) GetOrdersBuyByShop(shop_id int) (orders []*Order, err error) {
	rows, err := db.con.Query(context.Background(), "SELECT orders.id, to_char(orders.time, 'dd/mm/yyyy HH24:MI:SS'), orders.distrub, orders.status, COUNT(*), SUM(order_details.price) FROM orders JOIN order_proxy ON orders.id = order_proxy.order_id JOIN order_details ON order_proxy.order_details_id = order_details.order_details_id WHERE shop_id = $1 AND order_type = 1 GROUP BY orders.id;", shop_id)
	for rows.Next() {
		var order Order
		err := rows.Scan(
			&order.ID,
			&order.Time,
			&order.Distrib,
			&order.Status,
			&order.Count,
			&order.Price)
		if err != nil {
			return nil, err
		}
		orders = append(orders, &order)
	}
	return
}
func (db *Database) GetOrdersSoldByShop(shop_id int) (orders []*Order, err error) {
	rows, err := db.con.Query(context.Background(), "SELECT orders.id, orders.time, orders.status, COUNT(*), SUM(order_details.price) FROM orders JOIN order_proxy ON orders.id = order_proxy.order_id JOIN order_details ON order_proxy.order_details_id = order_details.order_details_id WHERE shop_id = $1 AND order_type = 2 GROUP BY orders.id;", shop_id)
	for rows.Next() {
		var order Order
		err := rows.Scan(
			&order.ID,
			&order.Time,
			&order.Status,
			&order.Count,
			&order.Price)
		if err != nil {
			return nil, err
		}
		orders = append(orders, &order)
	}
	return
}
