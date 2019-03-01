/* Valid raw string literals */
package main

func main() {
	var s string = ` `
	var t string = ` '!"#$%&()*+,-./`
	t = `0123456789`
	t = `:;<=>?`
	t = `@`
	t = `ABCDEFGHIJKLMNOPQRSTUVWXYZ`
	t = `[\]^_`
	t = `abcdefghijklmnopqrtuvwxyz`
	t = `{|}~`

	t = `\1\2\3\a\e\n\r\y\q\3\x\y`
}