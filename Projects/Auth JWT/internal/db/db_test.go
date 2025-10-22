package db

import (
	"context"
	"log"
	"testing"
	"time"

	"go.mongodb.org/mongo-driver/mongo"
	"go.mongodb.org/mongo-driver/mongo/options"
	"golang.org/x/crypto/bcrypt"
)

func (db *Database) NewClient() error {
	var err error
	ctx, cancel := context.WithTimeout(context.Background(), 10*time.Second)
	defer cancel()
	serverAPI := options.ServerAPI(options.ServerAPIVersion1)
	db.opts = options.Client().ApplyURI(db.URI).SetServerAPIOptions(serverAPI)
	client, err := mongo.Connect(context.Background(), db.opts)
	if err != nil {
		return err
	}
	defer client.Disconnect(ctx)
	return nil
}

func TestAll(t *testing.T) {
	db := Database{}
	db.URI = "mongodb://localhost:27017"
	var err error
	err = db.NewClient()
	if err != nil {
		log.Fatal(err)
	}
	pass, err := bcrypt.GenerateFromPassword([]byte("secret"), 14)
	if err != nil {
		log.Fatal(err)
	}

	u := User{UserID: 999, Refresh: string(pass)}
	err = db.CreateRecord(u.UserID, u.Refresh)
	if err != nil {
		log.Fatal(err)
	}
	val, err := db.GetRecord(999, u.Refresh)
	if err != nil {
		log.Fatal(err)
	}
	err = bcrypt.CompareHashAndPassword([]byte(val), []byte("secret"))
	if err != nil {
		log.Fatal("hash missmatch!: ", "VAL: ", val, " PASS: ", u.Refresh)
	}

	err = db.DeleteRecord(999)
	if err != nil {
		log.Fatal(err)
	}

	val2, err := db.GetRecord(999, u.Refresh)
	if err == nil {
		log.Fatal(val2, err)
	}
}
