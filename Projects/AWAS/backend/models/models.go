package models

import (
	"database/sql"
	"encoding/json"
)

// Product struct
type Product struct {
	ID          int             `json:"id"`
	Name        string          `json:"name"`
	Article     string          `json:"article"`
	Description json.RawMessage `json:"description"` // jsonb can be translated as json.RawMessage in Go
}

// Shop struct
type Shop struct {
	ID      int    `json:"id"`
	Name    string `json:"name"`
	Address string `json:"address"`
}

// ShopProduct struct
type ShopProduct struct {
	ID        int             `json:"id"`
	ProductID sql.NullInt64   `json:"product_id"` // using sql.NullInt64 to handle potential NULLs
	Amount    sql.NullInt64   `json:"amount"`     // using sql.NullInt64 to handle potential NULLs
	Price     sql.NullFloat64 `json:"price"`      // using sql.NullFloat64 for decimal types
}

// ShopsProxy struct
type ShopsProxy struct {
	ShopID         sql.NullInt64 `json:"shop_id"`
	ShopProductsID sql.NullInt64 `json:"shop_products_id"`
}

// Order struct
type Order struct {
	ID        int             `json:"id"`
	Time      string          `json:"time"` // Timestamp in PostgreSQL translates to time.Time in Go
	OrderType int             `json:"order_type"`
	ShopID    int             `json:"shop_id"`
	Distrib   sql.NullString  `json:"distrib"`
	Status    sql.NullString  `json:"status"`
	Count     sql.NullInt64   `json:"count"`
	Price     sql.NullFloat64 `json:"price"`
}

// OrderDetail struct
type OrderDetail struct {
	OrderDetailsID int             `json:"order_details_id"`
	ProductID      sql.NullInt64   `json:"product_id"`
	Amount         sql.NullInt64   `json:"amount"`
	Price          sql.NullFloat64 `json:"price"`
}

// OrderProxy struct
type OrderProxy struct {
	OrderID        sql.NullInt64 `json:"order_id"`
	OrderDetailsID sql.NullInt64 `json:"order_details_id"`
}
