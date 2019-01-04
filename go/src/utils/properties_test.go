package utils

import (
	"testing"
	"fmt"
	"os"
)

const properties = `
# this is common
key1 = text-value
# Lines without an equal sign are ignored.
# Ignored when key is empty.
key2 = value
`
const propertiesXML = `
<properties>
	<comment>test property.</comment>
	<entry key="key1"><![CDATA[key2=key3]]></entry>
	<entry key="key2"><![CDATA[this is key2 val.]]></entry>
</properties>
`
const propertiesJSON = `
{
	"comment": "test property.",
	"entry": {
		"key1": "key2=key3",
		"key2": "this is key2 val."
	}
}
`

func TestNewProperties(t *testing.T) {
	p := NewProperties()

	p.SetProperty("key.1", "val.1")
	p.SetProperty("key.2", "val.2")

	fmt.Println("Get key.1:", p.GetProperty("key.1"))
	fmt.Println("Get key.2:", p.GetProperty("key.2"))

	p.WriteComments("this is demo comment.")
	fmt.Println("comment:", p.Comments())

	fmt.Println("proerty names", p.PropertyNames())

	p.Range(func(key, val string) {
		fmt.Println("range - key:", key, "val:", val)
	})

	p.Clear()

	fmt.Println("proerty names", p.PropertyNames())
}

func TestProperties_Load(t *testing.T) {
	p := NewProperties()
	p.Load([]byte(properties))
	fmt.Println("comment:", p.Comments())
	fmt.Println("property names:", p.PropertyNames())
	p.Range(func(key, val string) {
		fmt.Println("range - key:", key, "val:", val)
	})
}

func TestProperties_LoadFromXML(t *testing.T) {
	p := NewProperties()
	if err := p.LoadFromXML([]byte(propertiesXML)); err != nil {
		panic(err)
	}
	fmt.Println("comment:", p.Comments())
	fmt.Println("property names:", p.PropertyNames())
}

func TestProperties_LoadFromJSON(t *testing.T) {
	p := NewProperties()
	if err := p.LoadFromJSON([]byte(propertiesJSON)); err != nil {
		panic(err)
	}
	fmt.Println("comment:", p.Comments())
	fmt.Println("property names:", p.PropertyNames())
	p.Range(func(key, val string) {
		fmt.Println("range - key:", key, "val:", val)
	})
}

func TestProperties_LoadFromReader(t *testing.T) {
	file,err := os.Open("demo/demo.properties")
	if err != nil {
		panic(err)
	}

	p := NewProperties()
	if err := p.LoadFromReader(file); err != nil {
		panic(err)
	}

	fmt.Println("comment:", p.Comments())
	fmt.Println("property names:", p.PropertyNames())
	p.Range(func(key, val string) {
		fmt.Println("range - key:", key, "val:", val)
	})
}

func TestProperties_Store(t *testing.T) {
	p := NewProperties()

	p.SetProperty("key.1", "val.1")
	p.SetProperty("key.2", "val.2")
	p.WriteComments("comment")

	fmt.Println("store:", string(p.Store()))

	p.StoreToWriter(os.Stdout)

	xml,err := p.StoreToXML()
	if err != nil {
		panic(err)
	}
	fmt.Println("store xml:", string(xml))

	json,err := p.StoreToJSON()
	if err != nil {
		panic(err)
	}
	fmt.Println("store json:", string(json))
}