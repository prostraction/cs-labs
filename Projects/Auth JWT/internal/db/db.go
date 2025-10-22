package db

import (
	"context"
	"fmt"
	"time"

	"go.mongodb.org/mongo-driver/bson"
	"go.mongodb.org/mongo-driver/mongo"
	"go.mongodb.org/mongo-driver/mongo/options"
)

type Database struct {
	opts      *options.ClientOptions
	URI       string
	connected bool
}

type User struct {
	UserID  int    `bson:"userid" json:"userid"`
	Refresh string `bson:"refresh" json:"refresh"`
}

func (db *Database) initDatabase() {
	if !db.connected {
		serverAPI := options.ServerAPI(options.ServerAPIVersion1)
		db.opts = options.Client().ApplyURI(db.URI).SetServerAPIOptions(serverAPI)
		db.connected = true
	}
}

func (db *Database) CreateRecord(id int, refresh string) error {
	var err error
	db.initDatabase()
	ctx, cancel := context.WithTimeout(context.Background(), 10*time.Second)
	defer cancel()
	client, err := mongo.Connect(context.Background(), db.opts)
	if err != nil {
		return err
	}
	defer client.Disconnect(ctx)
	userDB := client.Database("users")
	userCol := userDB.Collection("users")
	u := User{UserID: id, Refresh: refresh}
	_, err = userCol.InsertOne(ctx, u)
	return err
}

func (db *Database) GetRecord(id int, refresh string) (string, error) {
	var err error
	db.initDatabase()
	ctx, cancel := context.WithTimeout(context.Background(), 10*time.Second)
	defer cancel()
	client, err := mongo.Connect(context.Background(), db.opts)
	if err != nil {
		return "", err
	}
	defer client.Disconnect(ctx)
	userDB := client.Database("users")
	userCol := userDB.Collection("users")
	filter, err := userCol.Find(ctx, bson.M{"userid": id})
	if err != nil {
		return "", err
	}
	var record []User
	if err = filter.All(ctx, &record); err != nil {
		return "", err
	}
	if len(record) == 0 {
		return "", fmt.Errorf("no value found")
	}
	return record[0].Refresh, err
}

func (db *Database) DeleteRecord(id int) error {
	var err error
	db.initDatabase()
	ctx, cancel := context.WithTimeout(context.Background(), 10*time.Second)
	defer cancel()
	client, err := mongo.Connect(context.Background(), db.opts)
	if err != nil {
		return err
	}
	defer client.Disconnect(ctx)
	userDB := client.Database("users")
	userCol := userDB.Collection("users")
	filter := bson.M{"userid": id}
	opts := options.Delete().SetHint(bson.D{{Key: "_id", Value: 1}})
	_, err = userCol.DeleteMany(ctx, filter, opts)
	return err
}
