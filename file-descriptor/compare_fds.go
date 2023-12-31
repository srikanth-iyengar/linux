package main

import (
	"fmt"
	"os"
	"strconv"
	"syscall"
)

const (
	SYS_KCMP  = 312
	KCMP_FILE = 0
)

func kcmp_files(pid1, pid2, fd1, fd2 int) (int, error) {
	r1, _, err := syscall.Syscall6(SYS_KCMP, uintptr(pid1), uintptr(pid2), KCMP_FILE, uintptr(fd1), uintptr(fd2), 0)
	return int(r1), err
}

func main() {
	pid1, err := strconv.Atoi(os.Args[1])
	pid2, err := strconv.Atoi(os.Args[2])
	fd1, err := strconv.Atoi(os.Args[3])
	fd2, err := strconv.Atoi(os.Args[4])

	if err != nil {
		panic(err)
	}

	r1, err := kcmp_files(pid1, pid2, fd1, fd2)
	fmt.Println(r1, err)

}
