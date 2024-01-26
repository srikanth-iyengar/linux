/*
Copyright © 2024 NAME HERE ksrikanth3012@gmail.com
*/
package main

import (
	"fmt"
	"os"

	"github.com/srikanth-iyengar/linux/markdown-html/cmd"
	"github.com/srikanth-iyengar/linux/markdown-html/pkg/utils"
)

func main() {
	cmd.Execute()
	files, err := utils.FindMarkupfiles("..")
	if err != nil {
		os.Exit(1)
	}
	for _, v := range files {
		fmt.Println(v)
	}
}
