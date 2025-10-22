package main

import (
	"camera-dct-median-test/embedding"
	"errors"
	"fmt"
	"image"
	"image/color"
	"image/draw"
	"image/jpeg"
	"image/png"
	"io/ioutil"
	"log"
	"os"
	"path/filepath"
	"runtime"
	"strings"
	"sync"

	"github.com/edwvee/exiffix"
)

// Opens file and return image from it
func openImage(path string) (img image.Image, err error) {
	file, err := os.Open(path)
	if err != nil {
		return nil, err
	}
	defer file.Close()
	switch filepath.Ext(strings.ToLower(path)) {
	case ".jpeg":
		fallthrough
	case ".jpg":
		img, _, err = exiffix.Decode(file)
	case ".JPG":
		img, _, err = exiffix.Decode(file)
	case ".png":
		img, err = png.Decode(file)
	default:
		return nil, errors.New("wrong type of file: " + path)
	}
	return
}

func writeImage(path string, img *image.RGBA) error {
	f_out, err := os.Create(path)
	if err != nil {
		return err
	}
	defer f_out.Close()
	switch filepath.Ext(strings.ToLower(path)) {
	case ".jpeg":
		fallthrough
	case ".jpg":
		return jpeg.Encode(f_out, img, nil)
	case ".png":
		return png.Encode(f_out, img)
	default:
		return errors.New("unknown type of file" + path)
	}
}

func EncodeFile(pathIn string, pathOut string) error {
	fmt.Println(pathOut)

	img, err := openImage(pathIn)
	if err != nil {
		return err
	}
	b := img.Bounds()
	imgRGBA := image.NewRGBA(image.Rect(0, 0, b.Max.X, b.Max.Y))
	draw.Draw(imgRGBA, imgRGBA.Bounds(), img, b.Min, draw.Src)

	work := make(chan int)
	wg := sync.WaitGroup{}
	stack := make([]int, 3)
	for i := 0; i < 3; i++ {
		stack[i] = i
	}
	for i := 0; i < 3; i++ {
		wg.Add(1)
		go func(c int) {
			defer wg.Done()
			err = embedding.Encode(imgRGBA, c)
		}(i)
	}
	go func() {
		for _, s := range stack {
			work <- s
		}
		close(work)
	}()
	wg.Wait()
	if err != nil {
		return err
	}
	writeImage(pathOut, imgRGBA) // change to file
	return nil
}

func DecodeFile(path string) error {
	fmt.Println(path)
	img, err := openImage(path)
	if err != nil {
		return err
	}
	b := img.Bounds()
	imgRGBA := image.NewRGBA(image.Rect(0, 0, b.Dx(), b.Dy()))
	draw.Draw(imgRGBA, imgRGBA.Bounds(), img, b.Min, draw.Src)
	err = embedding.Decode(imgRGBA, 0)
	err = embedding.Decode(imgRGBA, 1)
	err = embedding.Decode(imgRGBA, 2)

	writeImage(path, imgRGBA)
	return err
}

func AddToImage(img1 *image.RGBA, path string) error {
	img2, err := openImage(path)
	if err != nil {
		return err
	}

	b := img2.Bounds()
	imgRGBA := image.NewRGBA(b)
	draw.Draw(imgRGBA, imgRGBA.Bounds(), img2, b.Min, draw.Over)

	for y := 0; y < b.Max.Y; y++ {
		for x := 0; x < b.Max.X; x++ {
			srcColor := imgRGBA.At(x, y)
			dstColor := img1.At(x, y)

			// Average the colors
			r1, g1, b1, a1 := dstColor.RGBA()
			r2, g2, b2, a2 := srcColor.RGBA()

			// Compute the averaged color
			avgColor := color.RGBA{
				R: uint8((r1 + r2) / 2 >> 8),
				G: uint8((g1 + g2) / 2 >> 8),
				B: uint8((b1 + b2) / 2 >> 8),
				A: uint8((a1 + a2) / 2 >> 8),
			}

			img1.Set(x, y, avgColor)
		}
	}
	return nil
}

func concRun(procAction int, dirIn string, dirOut string, img_test image.Image) ([]string, []error, error) {
	runtime.GOMAXPROCS(runtime.NumCPU())
	fList, err := ioutil.ReadDir(dirIn)
	if err != nil {
		return nil, nil, err
	}
	work := make(chan int)
	wait := sync.WaitGroup{}
	stackValue := make([]string, len(fList))
	stackNames := make([]int, len(fList))
	stackError := make([]error, len(fList))
	for i := 0; i < len(fList); i++ {
		stackNames[i] = i
	}
	for cpu := 0; cpu < runtime.NumCPU(); cpu++ {
		wait.Add(1)
		go func() {
			defer wait.Done()
			for i := range work {
				switch procAction {
				case encodeAction:
					stackError[i] = EncodeFile(dirIn+"//"+fList[i].Name(), dirOut+"//"+fList[i].Name())
				case testAction:
					// Type assert img_test to *image.RGBA
					if imgRGBA, ok := img_test.(*image.RGBA); ok {
						stackError[i] = AddToImage(imgRGBA, dirIn+"//"+fList[i].Name())
					} else {
						stackError[i] = fmt.Errorf("img_test is not of type *image.RGBA")
					}
				case decodeAction:
					stackError[i] = DecodeFile(dirIn + "//" + fList[i].Name())
				}
			}
		}()
	}
	go func() {
		for _, s := range stackNames {
			work <- s
		}
		close(work)
	}()
	wait.Wait()
	return stackValue, stackError, nil
}

const (
	encodeAction = iota
	testAction
	decodeAction
	benchAction
)

func convertToRGBA(img image.Image) *image.RGBA {
	bounds := img.Bounds()
	rgba := image.NewRGBA(bounds)
	draw.Draw(rgba, bounds, img, bounds.Min, draw.Src)
	return rgba
}

func main() {
	// Open the test image
	img, err := openImage("data-in/002.jpeg")
	if err != nil {
		log.Fatal(err)
	}

	// Convert the image to *image.RGBA
	test_img := convertToRGBA(img)

	// Run concurrent processing
	//concRun(encodeAction, "data-in", "data-out", nil)
	concRun(testAction, "data-out", "", test_img)
	//concRun(decodeAction, "data-out", "", nil)
}
