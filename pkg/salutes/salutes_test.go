package salutes_test

import (
	"testing"

	"github.com/lambdaclass/cairo-vm.go/pkg/salutes"
)

func TestHello(t *testing.T) {
	got := salutes.Hello()
	if got != "Hello, world!" {
		t.Errorf("We should have Hello, World! as the salute, got %s", got)
	}
}
