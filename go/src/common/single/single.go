package single

type Single interface {
	Start() error
}

const defaultMutexName = ".single.mutex."
const defaultSocketPort = 9090