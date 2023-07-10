.PHONY: run

run:
	@go run cmd/cli/main.go

test:
	@go test ./...

build:
	@cd pkg/lambdaworks/lib/lambdaworks && cargo build --release
	@cp pkg/lambdaworks/lib/lambdaworks/target/release/liblambdaworks.a pkg/lambdaworks/lib
	@go build ./...
