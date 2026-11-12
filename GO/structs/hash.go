package structs

import (
	"encoding/json"
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
	var hash uint = 0
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
	data, _ := json.Marshal(pairs)
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
