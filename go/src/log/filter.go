package log

import "regexp"

// Filter ASCII color.
func FilterColor(src []byte) []byte {
	var dst []byte
	reg := regexp.MustCompile(`\033\[\d*m`)
	dst = reg.ReplaceAll(src, nil)

	//for _,b := range src {
	//	if b == 033 {
	//
	//	}
	//	if b < 0x20 || b == 0x7F {
	//		continue
	//	}
	//
	//	dst = append(dst, b)
	//}

	return dst
}