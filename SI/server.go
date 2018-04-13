package main

import (
    "fmt"
    "net"
    "os"
    "io/ioutil"
)

const (
    CONN_HOST = "10.1.99.95"
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
        handleRequest(conn)
    }
}

// Handles incoming requests.
func handleRequest(conn net.Conn) {
  // Make a buffer to hold incoming data.
  buf := make([]byte, 1024)
  // Read the incoming connection into the buffer.
  size, err := conn.Read(buf)
  if err != nil {
    fmt.Println("Error reading:", err.Error())
  }
  s := string(buf[:size])
  fmt.Println(s)
  perm := os.FileMode(0755)
  err = ioutil.WriteFile("output.txt", buf[:size], perm)
  check(err)
  // Close the connection when you're done with it.
  conn.Close()
}

func check(e error){
	if e != nil {
		panic(e)
	}
}
