//usr/bin/go run $0 $@ ; exit
package main

import (
	"fmt"
	"net"
	"os"
	"io/ioutil"
	"strings"
)

const (
	CONN_HOST ="localhost"
	CONN_PORT = "3333"
	CONN_TYPE = "tcp"
)

func main() {
	// Listen for incoming connections.
	l, err := net.Listen(CONN_TYPE, CONN_HOST+":"+CONN_PORT)
	if err != nil {
		fmt.Println("Error listening:", err.Error())
		os.Exit(1)
	}
	// Close the listener when the application closes.
	defer l.Close()
	fmt.Println("Listening on " + CONN_HOST + ":" + CONN_PORT)
	for {
		// Listen for an incoming connection.
		conn, err := l.Accept()
		if err != nil {
			fmt.Println("Error accepting: ", err.Error())
			os.Exit(1)
		}
		// Handle connections in a new goroutine.
		go handleRequest(conn)
	}
}

// Handles incoming requests.
func handleRequest(conn net.Conn) {
	// Make a buffer to hold incoming data.
	buf := make([]byte, 128)
	// Read the incoming connection into the buffer.
	size, err := conn.Read(buf)
	if err != nil {
		fmt.Println("Error reading:", err.Error())
	}
	s := string(buf[:size-1])
	if strings.Index(s, "_") == -1{
		fmt.Println("no '_', skipping connection")
		return
	}
	sl := buf[:size-1]
	fmt.Println(s)
	// Close the connection when you're done with it.
	conn.Close()
	perm := os.FileMode(0644)
	chunks := strings.Split(string(sl), "_")
	if chunks[0] == "ERROR" {
		sl = append(sl, 0x0a)
		err = ioutil.WriteFile("./errors/"+strings.Replace(chunks[2], "/", ".", -1)+"_err.txt", sl, perm)
		check(err)
	} else {
		sl = append(sl, 0x0a)
		err = ioutil.WriteFile("./data/"+chunks[1]+"_"+chunks[2]+".txt", sl, perm)
		check(err)
	}
}

func check(e error){
	if e != nil {
		panic(e)
	}
}
