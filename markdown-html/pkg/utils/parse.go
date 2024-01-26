package utils

import (
	"fmt"
	"os"
	"strings"
)

func FindMarkupfiles(path string) ([]string, error) {
	dirs, err := os.ReadDir(path)
	if err != nil {
		return nil, err
	}

	resp := []string{}
	for _, v := range dirs {
		if v.IsDir() {
			if innerFiles, err := FindMarkupfiles(fmt.Sprintf("%s/%s", path, v.Name())); err == nil {
				resp = append(resp, innerFiles...)
			}
		} else {
			if strings.HasSuffix(v.Name(), ".md") {
				resp = append(resp, fmt.Sprintf("%s/%s", path, v.Name()))
			}
		}
	}
	return resp, nil
}
