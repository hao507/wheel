package ha

import (
	"os"
	"io"
	"strings"
	"path/filepath"
	"common/single"
)

type HA struct {
	name	string
	single	single.Single
}

func NewHA(single single.Single) *HA {
	var ha HA
	ha.single = single
	ha.name = ha.getApplicationName()

	return &ha
}

func (h *HA) Start() error {
	if err := h.single.Start(); err != nil {
		return err
	}

	other := h.getApplicationOtherName()

	tryAgainApplication := func() (p *os.Process, err error) {
		for i := 0; i < 10; i++ {
			if err := h.copyApplication(h.name, other); err != nil {
				return nil, err
			}
		}

		for i := 0; i < 10; i++ {
			p,err = h.forkApplication(other)
			if err != nil {
				return nil, err
			}
		}

		return p, err
	}

	p,err := tryAgainApplication()
	if err != nil {
		return err
	}

	go func() {
		for {
			p.Wait()
			if p,err = tryAgainApplication(); err != nil {
				break
			}
		}
	}()

	return nil
}

func (h *HA) copyApplication(srcFile string, dstFile string) error {
	src,err := os.Open(srcFile)
	if err != nil {
		return err
	}
	defer src.Close()

	dst,err := os.OpenFile(dstFile, os.O_RDWR | os.O_CREATE | os.O_TRUNC, 0755)
	if err != nil {
		return err
	}
	defer dst.Close()

	_,err = io.Copy(dst, src)
	if err != nil {
		return err
	}

	return err
}

func (h *HA) forkApplication(name string) (p *os.Process, err error) {
	// create a new process, start new agent.
	attr := &os.ProcAttr{
		Files: []*os.File{os.Stdin, os.Stdout, os.Stderr},
	}
	// Absolute path start process.
	dir, err := filepath.Abs(filepath.Dir(os.Args[0]))
	dir = strings.Replace(dir, "\\", "/", -1)
	binary := dir + "/" + name

	p,err = os.StartProcess(binary, []string{binary}, attr)
	if err != nil {
		return nil, err
	}

	return p, err
}

func (h *HA) getApplicationName() string {
	path := strings.Replace(os.Args[0], "\\", "/", -1)
	s := strings.Split(path, "/")
	l := len(s)

	return s[l - 1]
}

func (h *HA) getApplicationOtherName() string {
	if strings.HasPrefix(h.name, ".") {
		return strings.TrimLeft(h.name, ".")
	}

	return "." + h.name
}