package main

import (
	getToken "auth-jwt/internal/token"
	"crypto/sha1"
	"encoding/base64"
	"encoding/hex"
	"fmt"
	"time"

	"github.com/google/uuid"
	"golang.org/x/crypto/bcrypt"
)

func (app *Application) genTokenPair(id int) (res Response, err error) {
	if _, ok := app.accessMap[id]; ok {
		app.accessMap[id] = ""
		app.db.DeleteRecord(id)
	}

	uuid := uuid.New()
	access, err := getToken.GenerateAccess(id, uuid)
	if err != nil {
		return Response{}, err
	}
	refresh, err := getToken.GenerateRefresh(id, uuid)
	if err != nil {
		return Response{}, err
	}
	if app.accessMap == nil {
		app.accessMap = make(map[int]string)
	}
	app.accessMap[id] = access

	// bcrypt не позволяет зашифровать весь токен целиком, шифруем sha1-хеш
	h := sha1.New()
	h.Write([]byte(refresh))
	sha1_Hash := hex.EncodeToString(h.Sum(nil))

	// Запись refresh -> sha1 -> bcrypt в бд
	refHash, err := bcrypt.GenerateFromPassword([]byte(sha1_Hash), 14)
	if err != nil {
		return Response{}, err
	}
	if err = app.db.CreateRecord(id, string(refHash)); err != nil {
		return Response{}, err
	}
	return Response{Access: access, Refresh: base64.StdEncoding.EncodeToString([]byte(refresh))}, nil
}

func (app *Application) refTokenPair(id int, refresh string) (res Response, err error) {
	/* base64 -> sha1 == бд -> bcrypt (внутри тот же sha1) */

	// base64 -> sha1
	rfEnc, err := base64.StdEncoding.DecodeString(refresh)
	if err != nil {
		return Response{}, err
	}
	h := sha1.New()
	h.Write([]byte(rfEnc))
	sha1_Hash := hex.EncodeToString(h.Sum(nil))
	rfEncCrypt, err := bcrypt.GenerateFromPassword([]byte(sha1_Hash), 14)
	if err != nil {
		return Response{}, err
	}

	// бд -> bcrypt
	refDb, err := app.db.GetRecord(id, string(rfEncCrypt))
	if err != nil {
		return Response{}, err
	}
	err = bcrypt.CompareHashAndPassword([]byte(refDb), []byte(sha1_Hash))
	if err != nil {
		return Response{}, err
	}

	/* Access Token == Refresh Token */
	if atEnc, ok := app.accessMap[id]; !ok {
		return Response{}, fmt.Errorf("no Access Token on App memory")
	} else {
		atSub, atUUID, atTime, atErr := getToken.ReadToken(atEnc, true)
		if atErr != nil {
			return Response{}, atErr
		}
		rtSub, rtUUID, rtTime, rtErr := getToken.ReadToken(string(rfEnc), false)
		if rtErr != nil {
			return Response{}, atErr
		}
		if atSub != rtSub {
			return Response{}, fmt.Errorf("sub mismatch")
		}
		if atUUID != rtUUID {
			return Response{}, fmt.Errorf("uuid mismatch")
		}
		if atTime.Add(time.Hour*71+time.Minute*50) != rtTime {
			return Response{}, fmt.Errorf("time mismatch")
		}
		if rtTime.Unix() < time.Now().Unix() {
			app.accessMap[id] = ""
			app.db.DeleteRecord(id)
			return Response{}, fmt.Errorf("exceeded life time of refresh token: generate access/refresh new pair")
		}
	}

	/* После валидации удаляем старую пару access/refresh, даем новую */
	app.accessMap[id] = ""
	app.db.DeleteRecord(id)
	res, err = app.genTokenPair(id)
	return res, err
}
