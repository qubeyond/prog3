package main

import (
	"flag"
	"fmt"
	"lab3/internal"
	"os"
)

func main() {
	fileName := flag.String("file", "storage.bin", "Storage file")
	query := flag.String("query", "", "Query to execute")
	flag.Parse()

	if *query == "" {
		fmt.Println("Usage: --file <file> --query <query>")
		os.Exit(1)
	}

	manager := internal.NewDataManager()

	if _, err := os.Stat(*fileName); err == nil {
		manager.LoadBinary(*fileName)
	}

	result := manager.Execute(*query)
	fmt.Println(result)

	err := manager.SaveBinary(*fileName)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Error saving: %v\n", err)
	}
}