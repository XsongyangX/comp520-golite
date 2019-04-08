//~
//~ '!"#$%&()*+,-./
//~0123456789
//~:;<=>?
//~@
//~ABCDEFGHIJKLMNOPQRSTUVWXYZ
//~[\]^_
//~abcdefghijklmnopqrtuvwxyz
//~{|}~
//~\1\2\3\a\e\n\r\y\q\3\x\y
/* Valid raw string literals */
package main

func main() {
	var s string = ``
	println(s)
	var t string = ` '!"#$%&()*+,-./`
	println(t)
	t = `0123456789`
	println(t)
	t = `:;<=>?`
	println(t)
	t = `@`
	println(t)
	t = `ABCDEFGHIJKLMNOPQRSTUVWXYZ`
	println(t)
	t = `[\]^_`
	println(t)
	t = `abcdefghijklmnopqrtuvwxyz`
	println(t)
	t = `{|}~`
	println(t)

	t = `\1\2\3\a\e\n\r\y\q\3\x\y`
	println(t)
}
