package lambdaworks_test

import (
	"testing"

	"github.com/lambdaclass/cairo-vm.go/pkg/lambdaworks"
)

func TestSub(t *testing.T) {
	one := lambdaworks.One()
	expected := lambdaworks.Zero()
	got := lambdaworks.Sub(one, one)
	if !lambdaworks.IsEqual(got, expected) {
		t.Errorf("%+v is not equal to %+v", got, expected)
	}
}

func TestAdd(t *testing.T) {
	expected := lambdaworks.One()
	got := lambdaworks.Add(lambdaworks.Zero(), lambdaworks.One())
	if !lambdaworks.IsEqual(got, expected) {
		t.Errorf("%+v is not equal to %+v", got, expected)
	}
}

func TestMul1(t *testing.T) {
	expected := lambdaworks.One()
	got := lambdaworks.Mul(lambdaworks.One(), lambdaworks.One())
	if !lambdaworks.IsEqual(got, expected) {
		t.Errorf("%+v is not equal to %+v", got, expected)
	}
}

func TestMul0(t *testing.T) {
	expected := lambdaworks.Zero()
	got := lambdaworks.Mul(lambdaworks.One(), lambdaworks.Zero())
	if !lambdaworks.IsEqual(got, expected) {
		t.Errorf("%+v is not equal to %+v", got, expected)
	}
}
func TestDiv1(t *testing.T) {
	expected := lambdaworks.One()
	got := lambdaworks.Div(lambdaworks.From(2), lambdaworks.From(2))
	if !lambdaworks.IsEqual(got, expected) {
		t.Errorf("%+v is not equal to %+v", got, expected)
	}
}
