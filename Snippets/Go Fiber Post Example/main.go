package main

import (
	"encoding/json"
	"net/http"
	"net/url"
	"sort"
	"strconv"

	"github.com/gofiber/fiber/v2"
	"github.com/sirupsen/logrus"
)

type (
	BinarySearchRequest struct {
		Numbers []int `json:"numbers"`
		Target  int   `json:"target"`
	}

	BinarySearchResponse struct {
		TargetIndex int    `json:"target_index"`
		Error       string `json:"error,omitempty"`
	}
)

type (
	CreateLinkRequest struct {
		External string `json:"external"`
		Internal string `json:"internal"`
	}

	GetLinkResponse struct {
		Internal string `json:"internal"`
	}
)

type (
	GetTaskResponse struct {
		ID       int64  `json:"id"`
		Desc     string `json:"description"`
		Deadline int64  `json:"deadline"`
	}

	CreateTaskRequest struct {
		Desc     string `json:"description"`
		Deadline int64  `json:"deadline"`
	}

	CreateTaskResponse struct {
		ID int64 `json:"id"`
	}

	UpdateTaskRequest struct {
		Desc     string `json:"description"`
		Deadline int64  `json:"deadline"`
	}

	Task struct {
		ID       int64
		Desc     string
		Deadline int64
	}
)

var links = make(map[string]string)

const targetNotFound = -1

var (
	taskIDCounter int64 = 1
	tasks               = make(map[int64]Task)
)

func main() {
	webApp := fiber.New()
	webApp.Get("/", func(c *fiber.Ctx) error {
		return c.SendStatus(200)
	})

	// BEGIN (write your solution here)
	webApp.Post("/search", func(c *fiber.Ctx) error {
		bodyval := BinarySearchRequest{}

		response := BinarySearchResponse{}
		response.TargetIndex = targetNotFound

		if err := c.BodyParser(&bodyval); err != nil {
			response.Error = "Invalid JSON"
			bytes, _ := json.Marshal(&response)
			return c.Status(http.StatusBadRequest).SendString(string(bytes))
		}

		target := sort.SearchInts(bodyval.Numbers, bodyval.Target)
		if target == len(bodyval.Numbers) {
			response.Error = "Target was not found"
			bytes, _ := json.Marshal(&response)
			return c.Status(http.StatusNotFound).SendString(string(bytes))
		}
		response.TargetIndex = target
		bytes, _ := json.Marshal(&response)
		return c.Status(http.StatusOK).SendString(string(bytes))
	})
	////////////////////////////////////////////////////////////////
	webApp.Post("/links", func(c *fiber.Ctx) error {
		req := CreateLinkRequest{}
		if err := c.BodyParser(&req); err != nil {
			return c.Status(400).SendString("Invalid JSON")
		} else {
			link, _ := url.QueryUnescape(req.External)
			links[link] = req.Internal
			return c.SendStatus(200)
		}
	})
	webApp.Get("/links/:external", func(c *fiber.Ctx) error {
		req := GetLinkResponse{}
		search, _ := url.QueryUnescape(c.Params("external"))
		ok := false
		if req.Internal, ok = links[search]; !ok {
			return c.Status(404).SendString("Link not found")
		} else {
			bytes, _ := json.Marshal(&req)
			return c.Status(200).SendString(string(bytes))
		}
	})
	////////////////////////////////////////////////////////////////
	webApp.Post("/tasks", func(c *fiber.Ctx) error {
		body := CreateTaskRequest{}
		if err := c.BodyParser(&body); err != nil {
			task := Task{}
			task.ID = taskIDCounter
			task.Desc = body.Desc
			task.Deadline = body.Deadline
			tasks[taskIDCounter] = task
			taskIDCounter++

			t := struct {
				Id int64 `json:"id"`
			}{}
			t.Id = task.ID
			bytes, _ := json.Marshal(t)
			return c.Status(200).SendString(string(bytes))
		}
		return c.SendStatus(400)
	})

	webApp.Patch("/tasks/:id", func(c *fiber.Ctx) error {
		body := CreateTaskRequest{}
		id, _ := strconv.Atoi(c.Params("id"))

		if err := c.BodyParser(&body); err != nil {
			if task, err := tasks[int64(id)]; !err {
				return c.SendStatus(404)
			} else {
				task.ID = int64(id)
				if body.Deadline != 0 {
					task.Deadline = body.Deadline
				}
				if body.Desc != "" {
					task.Desc = body.Desc
				}
				tasks[int64(id)] = task
				return c.SendStatus(200)
			}
		}
		return c.SendStatus(400)
	})

	webApp.Get("/tasks/:id", func(c *fiber.Ctx) error {
		id, _ := strconv.Atoi(c.Params("id"))
		if task, err := tasks[int64(id)]; !err {
			return c.SendStatus(404)
		} else {
			t := GetTaskResponse{}
			t.ID = task.ID
			t.Deadline = task.Deadline
			t.Desc = task.Desc
			bytes, _ := json.Marshal(t)
			return c.Status(200).SendString(string(bytes))
		}
	})

	webApp.Delete("/tasks/:id", func(c *fiber.Ctx) error {
		id, _ := strconv.Atoi(c.Params("id"))
		if _, err := tasks[int64(id)]; !err {
			return c.SendStatus(404)
		} else {
			delete(tasks, int64(id))
			return c.SendStatus(200)
		}
	})
	// END

	logrus.Fatal(webApp.Listen(":8080"))
}
