
package structs

import (
	"encoding/binary"
	"encoding/json"
	"errors"
	"io"
	"os"
)

type hNode struct {
	Key   string
	Value string
	Next  *hNode
}

type HashTable struct {
	table [10]*hNode
}

type kvPair struct {
	Key   string `json:"key"`
	Value string `json:"value"`
}

func NewHashTable() *HashTable {
	return &HashTable{}
}

func (h *HashTable) hash(key string) int {
	var hash uint
	for i := 0; i < len(key); i++ {
		hash = hash*31 + uint(key[i])
	}
	return int(hash % 10)
}

func (h *HashTable) Insert(key, value string) {
	idx := h.hash(key)
	for curr := h.table[idx]; curr != nil; curr = curr.Next {
		if curr.Key == key {
			curr.Value = value
			return
		}
	}
	h.table[idx] = &hNode{Key: key, Value: value, Next: h.table[idx]}
}

func (h *HashTable) Find(key string) (string, bool) {
	idx := h.hash(key)
	for curr := h.table[idx]; curr != nil; curr = curr.Next {
		if curr.Key == key {
			return curr.Value, true
		}
	}
	return "", false
}

func (h *HashTable) Delete(key string) bool {
	idx := h.hash(key)
	var prev *hNode
	for curr := h.table[idx]; curr != nil; curr = curr.Next {
		if curr.Key == key {
			if prev == nil {
				h.table[idx] = curr.Next
			} else {
				prev.Next = curr.Next
			}
			return true
		}
		prev = curr
	}
	return false
}

func (h *HashTable) SaveText(file string) error {
	var pairs []kvPair
	for i := 0; i < 10; i++ {
		for curr := h.table[i]; curr != nil; curr = curr.Next {
			pairs = append(pairs, kvPair{Key: curr.Key, Value: curr.Value})
		}
	}
	data, err := json.Marshal(pairs)
	if err != nil {
		return err
	}
	return os.WriteFile(file, data, 0644)
}

func (h *HashTable) LoadText(file string) error {
	data, err := os.ReadFile(file)
	if err != nil {
		return err
	}
	var pairs []kvPair
	if err := json.Unmarshal(data, &pairs); err != nil {
		return err
	}
	h.table = [10]*hNode{}
	for _, p := range pairs {
		h.Insert(p.Key, p.Value)
	}
	return nil
}

func (h *HashTable) SaveBin(file string) error {
	f, err := os.Create(file)
	if err != nil {
		return err
	}
	defer f.Close()

	var count int32
	for i := 0; i < 10; i++ {
		for curr := h.table[i]; curr != nil; curr = curr.Next {
			count++
		}
	}

	if err := binary.Write(f, binary.LittleEndian, count); err != nil {
		return err
	}

	for i := 0; i < 10; i++ {
		for curr := h.table[i]; curr != nil; curr = curr.Next {
			key := []byte(curr.Key)
			value := []byte(curr.Value)

			if err := binary.Write(f, binary.LittleEndian, int32(len(key))); err != nil {
				return err
			}
			if _, err := f.Write(key); err != nil {
				return err
			}

			if err := binary.Write(f, binary.LittleEndian, int32(len(value))); err != nil {
				return err
			}
			if _, err := f.Write(value); err != nil {
				return err
			}
		}
	}
	return nil
}

func (h *HashTable) LoadBin(file string) error {
	f, err := os.Open(file)
	if err != nil {
		return err
	}
	defer f.Close()

	var count int32
	if err := binary.Read(f, binary.LittleEndian, &count); err != nil {
		return err
	}
	if count < 0 {
		return errors.New("invalid file size")
	}

	h.table = [10]*hNode{}

	for i := int32(0); i < count; i++ {
		var keySize int32
		if err := binary.Read(f, binary.LittleEndian, &keySize); err != nil {
			h.table = [10]*hNode{}
			return err
		}
		if keySize < 0 {
			h.table = [10]*hNode{}
			return errors.New("invalid key size")
		}

		key := make([]byte, keySize)
		if _, err := io.ReadFull(f, key); err != nil {
			h.table = [10]*hNode{}
			return err
		}

		var valueSize int32
		if err := binary.Read(f, binary.LittleEndian, &valueSize); err != nil {
			h.table = [10]*hNode{}
			return err
		}
		if valueSize < 0 {
			h.table = [10]*hNode{}
			return errors.New("invalid value size")
		}

		value := make([]byte, valueSize)
		if _, err := io.ReadFull(f, value); err != nil {
			h.table = [10]*hNode{}
			return err
		}

		h.Insert(string(key), string(value))
	}
	return nil
}
