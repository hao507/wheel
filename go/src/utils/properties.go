package utils

import (
	"io"
	"sync"
	"io/ioutil"
	"strings"
	"runtime"
	"fmt"
	"bytes"
	"encoding/xml"
	"encoding/json"
)

type PropertiesXMLEntry struct {
	XMLName     struct{}   `xml:"entry"`
	Key         string      `xml:"key,attr"`
	CDATA       string      `xml:",cdata"`
}

type PropertiesXML struct {
	XMLName     struct{}             `xml:"properties"`
	Comment     string                `xml:"comment"`
	Entry       []PropertiesXMLEntry  `xml:"entry"`
}

type PropertiesJSON struct {
	Comment     string                `json:"comment"`
	Entry       map[string]string     `json:"entry"`
}

type Properties struct {
	rwMutex    sync.RWMutex
	separator  string
	comment    string
	list       map[string]string
}

func NewProperties() *Properties {
	var separator string
	switch runtime.GOOS {
	case "windows":
		separator = "\r\n"
	default:
		separator = "\n"
	}

	return &Properties{
		separator : separator,
		list      : make(map[string]string),
	}
}

func (p *Properties) Load(data []byte) {
	p.rwMutex.Lock()
	defer p.rwMutex.Unlock()

	if strings.Contains(string(data), "\r\n") {
		p.separator = "\r\n"
	} else {
		p.separator = "\n"
	}

	lines := strings.Split(string(data), p.separator)
	for _,line := range lines {
		line = strings.TrimSpace(line)

		if line == "" {
			continue
		}

		if strings.HasPrefix(line, "#") {
			if len(p.list) == 0 && p.comment == "" {
				p.comment = strings.TrimLeft(line, "# ")
			}
			continue
		}

		if !strings.Contains(line, "=") {
			continue
		}

		pair := strings.SplitN(line, "=", 2)
		if len(pair) != 2 {
			continue
		}

		key := strings.TrimSpace(pair[0])
		val := strings.TrimSpace(pair[1])

		if key != "" {
			p.list[key] = val
		}
	}
}

func (p *Properties) LoadFromXML(data []byte) error {
	var pXML PropertiesXML
	if err := xml.Unmarshal(data, &pXML); err != nil {
		return err
	}

	p.rwMutex.Lock()
	defer p.rwMutex.Unlock()

	p.comment = pXML.Comment
	for _,v := range pXML.Entry {
		p.list[v.Key] = v.CDATA
	}

	return nil
}

func (p *Properties) LoadFromJSON(data []byte) error {
	var pJson PropertiesJSON
	err := json.Unmarshal(data, &pJson)
	if err != nil {
		return err
	}

	p.rwMutex.Lock()
	defer p.rwMutex.Unlock()

	p.comment = pJson.Comment
	for key,val := range pJson.Entry {
		p.list[key] = val
	}

	return nil
}

func (p *Properties) LoadFromReader(reader io.Reader) error {
	data,err := ioutil.ReadAll(reader)
	if err != nil {
		return err
	}

	p.Load(data)

	return nil
}

func (p *Properties) LoadFromProperties(pro *Properties) {
	p.rwMutex.Lock()
	defer p.rwMutex.Unlock()

	for key,val := range pro.list {
		p.list[key] = val
	}
}

func (p *Properties) Comments() string {
	return p.comment
}

func (p *Properties) WriteComments(comments string) {
	p.comment = comments
}

func (p *Properties) Store() []byte {
	p.rwMutex.RLock()
	defer p.rwMutex.RUnlock()

	var buff bytes.Buffer
	if p.comment != "" {
		buff.WriteString(fmt.Sprintf("# %s%s", p.comment, p.separator))
	}
	for k,v := range p.list {
		buff.WriteString(fmt.Sprintf("%s = %s%s", k, v, p.separator))
	}

	return buff.Bytes()
}

func (p *Properties) StoreToXML() ([]byte, error) {
	p.rwMutex.RLock()
	defer p.rwMutex.RUnlock()

	var pXML PropertiesXML

	pXML.Comment = p.comment
	for key,val := range p.list {
		pXML.Entry = append(pXML.Entry, PropertiesXMLEntry{Key:key, CDATA:val})
	}

	return xml.MarshalIndent(pXML, "", "\t")
}

func (p *Properties) StoreToJSON() ([]byte, error) {
	p.rwMutex.RLock()
	defer p.rwMutex.RUnlock()

	var pJson PropertiesJSON

	pJson.Comment = p.comment
	pJson.Entry = make(map[string]string)
	for key,val := range p.list {
		pJson.Entry[key] = val
	}

	return json.MarshalIndent(pJson, "", "\t")
}

func (p *Properties) StoreToWriter(writer io.Writer) error {
	data := p.Store()
	_,err := writer.Write(data)

	return err
}

func (p *Properties) Clear() {
	p.rwMutex.Lock()
	defer  p.rwMutex.Unlock()

	for k,_ := range p.list {
		delete(p.list, k)
	}
}

func (p *Properties) PropertyNames() []string {
	p.rwMutex.RLock()
	defer p.rwMutex.RUnlock()

	var list []string
	for k,_ := range p.list {
		list = append(list, k)
	}

	return list
}

func (p *Properties) GetProperty(key string) string {
	p.rwMutex.RLock()
	defer p.rwMutex.RUnlock()

	return p.list[key]
}

func (p *Properties) SetProperty(key, val string) {
	p.rwMutex.Lock()
	defer p.rwMutex.Unlock()

	p.list[key] = val
}

func (p *Properties) Range(f func(key, val string)) {
	p.rwMutex.RLock()
	defer p.rwMutex.RUnlock()

	if f != nil {
		for k,v := range p.list {
			f(k,v)
		}
	}
}