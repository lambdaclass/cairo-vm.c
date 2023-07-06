.PHONY: run

run:
	@go run cmd/cli/main.go

test:
	@go test ./...

build:
	@go build ./...
