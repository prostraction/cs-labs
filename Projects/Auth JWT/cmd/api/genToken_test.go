package main

import (
	"log"
	"testing"
)

func TestAll(t *testing.T) {
	a := Application{}
	a.db.URI = "mongodb://localhost:27017"
	res, err := a.genTokenPair(3)
	if err != nil {
		log.Fatal(err)
	}
	res2, err := a.refTokenPair(3, res.Refresh)
	if err != nil {
		log.Fatal(err)
	}
	if res.Access == res2.Access {
		log.Fatal("access token has not been changed")
	}
	_, err = a.refTokenPair(3, res.Refresh)
	if err == nil {
		log.Fatal("id = 3; dublicate refresh token usage")
	}

	res3, err := a.refTokenPair(4, res.Refresh)
	if err == nil {
		log.Fatal("id = 4; token refreshed without access", res3)
	}

	if err := a.db.DeleteRecord(3); err != nil {
		log.Fatal("unable to remove record")
	}

	_, err = a.refTokenPair(3, res.Refresh)
	if err == nil {
		log.Fatal("unable to remove record")
	}
}
