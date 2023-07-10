package main

import (
	"fmt"

	"github.com/lambdaclass/cairo-vm.go/pkg/parser"
	"github.com/lambdaclass/cairo-vm.go/pkg/salutes"
)

func main() {
	fmt.Println(salutes.Hello())
	parser.Parse()
}
