package token

import (
	"fmt"
	"os"
	"time"

	"github.com/golang-jwt/jwt/v4"
	"github.com/google/uuid"
)

var accessSecretKey = []byte("secret-key-at")
var refreshSecretKey = []byte("secret-key-rt")

func Init() {
	if at := os.Getenv("at"); len(at) > 0 {
		accessSecretKey = []byte(at)
	}
	if rt := os.Getenv("rt"); len(rt) > 0 {
		accessSecretKey = []byte(rt)
	}
}

func GenerateAccess(id int, uuid uuid.UUID) (string, error) {

	payload := jwt.MapClaims{
		"sub":  id,
		"uuid": uuid,
		"exp":  time.Now().Add(time.Minute * 10).Unix(),
	}
	token := jwt.NewWithClaims(jwt.SigningMethodHS512, payload)
	t, err := token.SignedString(accessSecretKey)
	return t, err
}

func GenerateRefresh(id int, uuid uuid.UUID) (string, error) {
	payload := jwt.MapClaims{
		"sub":  id,
		"uuid": uuid,
		"exp":  time.Now().Add(time.Hour * 72).Unix(),
	}
	token := jwt.NewWithClaims(jwt.SigningMethodHS256, payload)
	t, err := token.SignedString(refreshSecretKey)
	return t, err
}

func ReadToken(token string, isAccess bool) (sub int, uuid string, exp time.Time, err error) {
	payload := jwt.MapClaims{
		"sub":  0,
		"uuid": 0,
		"exp":  0,
	}
	t, err := jwt.ParseWithClaims(token, payload, func(token *jwt.Token) (interface{}, error) {
		if isAccess {
			return accessSecretKey, nil
		}
		return refreshSecretKey, nil
	})
	claims, _ := t.Claims.(jwt.MapClaims)

	var ok bool
	if subFloat, ok := claims["sub"].(float64); !ok {
		return 0, "", time.Now(), fmt.Errorf("sub is invalid")
	} else {
		sub = int(subFloat)
	}
	if uuid, ok = claims["uuid"].(string); !ok {
		return 0, "", time.Now(), fmt.Errorf("uuid is invalid")
	}
	if expFloat, ok := claims["exp"].(float64); !ok {
		return 0, "", time.Now(), fmt.Errorf("exp is invalid")
	} else {
		exp = time.Unix(int64(expFloat), 0)
	}
	return sub, uuid, exp, err
}
