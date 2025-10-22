package token

import (
	"log"
	"testing"

	"github.com/google/uuid"
)

func Test1(t *testing.T) {
	id := uuid.New()
	t1, err := GenerateRefresh(1, id)
	if err != nil {
		log.Fatal(err)
	}
	sub1, uuid1, exp1, err1 := ReadToken(t1, false)
	t2, err := GenerateAccess(1, id)
	if err != nil {
		log.Fatal(err)
	}
	sub2, uuid2, exp2, err2 := ReadToken(t2, true)
	if sub1 != sub2 || uuid1 != uuid2 || exp1 == exp2 || err1 != nil || err2 != nil {
		log.Fatal("sub1 = ", sub1, " ",
			"sub2 = ", sub2, " ",
			"uuid1 = ", uuid1, " ",
			"uuid2 = ", uuid2, " ",
			"exp1 = ", exp1, " ",
			"exp2 = ", exp2, " ",
			"err1 = ", err1, " ",
			"err2 = ", err2, " ")
	}

}

func RunAll(t *testing.T) {
	Test1(t)
}
