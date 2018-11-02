package single

import "common/mutex"

type singleMutex struct{
	name	string
	mutex	*mutex.Mutex
}

func NewSingleMutex(name string) *singleMutex {
	return &singleMutex{name:name, mutex:nil}
}

func (s *singleMutex)Start() error {
	if s.mutex == nil {
		m,err := mutex.NewMutex(s.name)
		if err != nil {
			return err
		}
		s.mutex = m
	}

	if ok,err := s.mutex.TryLock(); err != nil || !ok {
		return err
	}

	return nil
}