package embedding

import (
	"camera-dct-median-test/dct"
	"errors"
	"image"
	_ "image"
)

func abs[T int | byte](value T) T {
	if value < 0 {
		return -value
	}
	return value
}
func StringToBoolArray(value string) []bool {
	if len(value) == 0 {
		return nil
	}
	array := make([]bool, 0)
	runeValue := []rune(value)
	for i := 0; i < len(runeValue); i++ {
		symbol := make([]bool, 0, 32)
		ch := runeValue[i]
		for ch != 0 {
			t := ch % 2
			if t == 0 {
				symbol = append(symbol, false)
			} else {
				symbol = append(symbol, true)
			}
			ch /= 2
		}
		for j := 0; j < 32-len(symbol); j++ {
			symbol = append(symbol, false)
		}
		for j := len(symbol) - 1; len(symbol) != 32; j-- {
			symbol = append(symbol, symbol[j])
		}
		array = append(array, symbol...)
		symbol = nil
	}
	return array
}

func BoolArrayToString(value []bool) string {
	if len(value) < 32 {
		return ""
	}
	retString := ""
	symbol := make([]bool, 0, 32)
	count := 1
	for i := 0; i < len(value); i++ {
		if symbol == nil {
			symbol = make([]bool, 0, 32)
		}
		symbol = append(symbol, value[i])

		if count/32 == 1 {
			var r rune
			c := 0
			for j := 0; j < 32; j++ {
				if symbol[j] {
					r += (1 << c)
				}
				c++
			}
			retString += string(r)
			symbol = nil
			count = 0
		}
		count++
	}
	return retString
}

func Encode(img *image.RGBA, channelSelected int) error {
	bounds := img.Bounds()
	if bounds.Max.X < 8 || bounds.Max.Y < 8 {
		return errors.New("image resolution lower than 8x8")
	}
	const sizeOfBlock2D = 64
	const sizeOfBlock1D = 8

	dctMatrix := make([]float32, sizeOfBlock2D)

	for x := 0; x < bounds.Max.X; x += sizeOfBlock1D {
		for y := 0; y < bounds.Max.Y; y += sizeOfBlock1D {
			// Determine block dimensions
			blockX := sizeOfBlock1D
			blockY := sizeOfBlock1D

			if x+blockX > bounds.Max.X {
				blockX = bounds.Max.X - x
			}
			if y+blockY > bounds.Max.Y {
				blockY = bounds.Max.Y - y
			}

			// Perform DCT on the specified block
			dct.MakeDCT(&dctMatrix, &img.Pix, x, y, bounds.Max.X, 4, int8(channelSelected))

			// Write the DCT result back to the correct channel
			for x1 := 0; x1 < blockX; x1++ {
				for y1 := 0; y1 < blockY; y1++ {
					// Calculate the index in the image's Pix based on channelSelected
					pixelIndex := (x + x1 + (y+y1)*bounds.Max.X) * 4 // Multiply by 4 for RGBA
					img.Pix[pixelIndex+channelSelected] = uint8(dctMatrix[x1+y1*sizeOfBlock1D])
				}
			}
		}
	}
	return nil
}

func Decode(img *image.RGBA, channelSelected int) error {
	bounds := img.Bounds()
	if bounds.Max.X < 8 || bounds.Max.Y < 8 {
		return errors.New("image resolution lower than 8x8")
	}

	const sizeOfBlock2D = 64
	const sizeOfBlock1D = 8

	dctMatrix := make([]float32, sizeOfBlock2D)

	for x := 0; x < bounds.Max.X; x += sizeOfBlock1D {
		for y := 0; y < bounds.Max.Y; y += sizeOfBlock1D {

			// Reset dctMatrix for the current block
			for i := 0; i < sizeOfBlock2D; i++ {
				dctMatrix[i] = 0 // or any default value as needed
			}

			for x1 := 0; x1 < sizeOfBlock1D; x1++ {
				for y1 := 0; y1 < sizeOfBlock1D; y1++ {
					// Ensure we're within bounds
					if x+x1 < bounds.Max.X && y+y1 < bounds.Max.Y {
						pixelIndex := (x + x1 + (y+y1)*bounds.Max.X) * 4
						dctMatrix[x1+y1*sizeOfBlock1D] = float32(img.Pix[pixelIndex] % 255)
					}
				}
			}

			//fmt.Println("dctMatrix:", dctMatrix)

			dct.MakeIDCT(&dctMatrix, &img.Pix, x, y, bounds.Max.X, 4, int8(channelSelected))
		}
	}

	return nil
}
