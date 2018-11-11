package workerSDK

import "errors"

var (
	workerTypeError       = errors.New("unknown worker type")
	customNullError       = errors.New("custom is null")
)
